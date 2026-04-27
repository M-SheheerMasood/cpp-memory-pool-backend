#include "../include/post_list.h"
#include <iostream>

PostList::~PostList()
{
    PostNode* temp = head;
    while (temp)
    {
        PostNode* nxt = temp->next;
        delete temp->post;
        delete temp;
        temp = nxt;
    }
}

void PostList::addPost(const Post &p)
{
    Post *newpost = new Post(p);
    PostNode *newnode = new PostNode(newpost);
    newnode->next = head;
    head = newnode;
}

bool PostList::removePost(int postID)
{
    if (head == nullptr)
    {
        return false;
    }
    if (head->post->postID == postID)
    {
        PostNode *del = head;
        head = head->next;
        delete del->post;
        delete del;
        return true;
    }
    PostNode *temp = head;
    while (temp->next)
    {
        PostNode *current = temp->next;
        if (current->post->postID == postID)
        {
            temp->next = current->next;
            delete current->post;
            delete current;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

Post *PostList::findPost(int postID)
{
    PostNode *temp = head;
    while (temp)
    {
        if (temp->post->postID == postID)
        {
            return temp->post;
        }
        temp = temp->next;
    }
    return nullptr;
}

void PostList::displayPosts() const
{
    PostNode *temp = head;
    while (temp)
    {
        cout << temp->post->postID << " " << temp->post->views << " " << temp->post->category << " " << temp->post->content << endl;
        temp = temp->next;
    }
}

bool PostList::isEmpty() const
{
    return head == nullptr;
}