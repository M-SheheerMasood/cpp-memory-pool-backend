#include "../include/operation_stack.h"
#include <cstddef> // size_t
using namespace std;

UndoRedoManager::UndoRedoManager(UserManager &um, PostPool &pool) : userManager(um), postPool(pool) {}

void UndoRedoManager::beginTransaction()
{
    transactionMarkers.push(undoStack.size());
}

void UndoRedoManager::commitTransaction()
{
    if (!transactionMarkers.empty())
    {
        transactionMarkers.pop();
    }
    size_t i = 0;
    while (i < trashList.size())
    {
        Post *post = static_cast<Post *>(trashList[i]);
        postPool.freePost(post);
        i++;
    }
    trashList.clear();
}

void UndoRedoManager::rollbackTransaction()
{
    if (transactionMarkers.empty())
    {
        return;
    }
    size_t marker = transactionMarkers.top();
    transactionMarkers.pop();
    while (undoStack.size() > marker)
    {
        undo();
    }
    size_t i = 0;
    while (i < trashList.size())
    {
        Post *post = static_cast<Post *>(trashList[i]);
        postPool.freePost(post);
        i++;
    }
    trashList.clear();
}

void UndoRedoManager::record(const OpFrame &f)
{
    undoStack.push_back(f);
    redoStack.clear();
}

bool UndoRedoManager::undo()
{
    if (undoStack.empty())
    {
        return false;
    }
    OpFrame frame = undoStack.back();
    undoStack.pop_back();
    switch (frame.type)
    {
    case OpType::CREATE_USER:
        userManager.deleteUser(frame.userID);
        break;
    case OpType::DELETE_USER:
        userManager.createUser(frame.userID, frame.snapshot_username_or_content);
        break;
    case OpType::FOLLOW:
        userManager.unfollow(frame.userID, frame.postID);
        break;
    case OpType::UNFOLLOW:
        userManager.follow(frame.userID, frame.postID);
        break;
    default:
        break;
    }
    redoStack.push_back(frame);
    return true;
}

bool UndoRedoManager::redo()
{
    if (redoStack.empty())
    {
        return false;
    }
    OpFrame frame = redoStack.back();
    redoStack.pop_back();
    switch (frame.type)
    {
    case OpType::CREATE_USER:
        userManager.createUser(frame.userID, frame.snapshot_username_or_content);
        break;
    case OpType::DELETE_USER:
        userManager.deleteUser(frame.userID);
        break;
    case OpType::FOLLOW:
        userManager.follow(frame.userID, frame.postID);
        break;
    case OpType::UNFOLLOW:
        userManager.unfollow(frame.userID, frame.postID);
        break;
    default:
        break;
    }
    undoStack.push_back(frame);
    return true;
}
