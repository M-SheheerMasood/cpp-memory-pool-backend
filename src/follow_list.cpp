#include "../include/follow_list.h"
#include "../include/user.h"
#include <iostream>
using namespace std;

FollowList::~FollowList()
{
    FollowNode* temp = head;
    while(temp)
    {
        FollowNode* nxt = temp->next;
        delete temp;
        temp = nxt;
    }
}

void FollowList::addFollowing(User *u)
{
    if (u)
    {
        FollowNode *temp = head;
        while (temp)
        {
            if (temp->user->userID == u->userID)
            {
                return;
            }
            temp = temp->next;
        }
        FollowNode *newnode = new FollowNode(u);
        newnode->next = head;
        head = newnode;
    }
}

bool FollowList::removeFollowing(int userID)
{
    if (head == nullptr)
    {
        return false;
    }
    if (head->user->userID == userID)
    {
        FollowNode *temp = head->next;
        delete head;
        head = temp;
        return true;
    }
    FollowNode *temp = head;
    while (temp->next)
    {
        FollowNode *current = temp->next;
        if (current->user->userID == userID)
        {
            temp->next = current->next;
            delete current;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

User *FollowList::findFollowing(int userID)
{
    FollowNode *temp = head;
    while (temp)
    {
        if (temp->user->userID == userID)
        {
            return temp->user;
        }
        temp = temp->next;
    }
    return nullptr;
}

void FollowList::displayFollowing() const
{
    FollowNode* temp = head;
    while(temp)
    {
        cout << temp->user->userID << " " << temp->user->userName << endl;
        temp = temp->next;
    }
}