#include "../include/user.h"
#include "../include/follow_list.h"
#include <iostream>
using namespace std;

User::User(int id, const string &name)
{
    userID = id;
    userName = name;
    following = new FollowList();
}

// Copy constructor
User::User(const User &other)
{
    // Don't copy the following relationships - they should be rebuilt separately
    // This prevents circular dependency issues and dangling pointers
    userID = other.userID;
    userName = other.userName;
    PostNode *otherhead = other.posts.head;
    while (otherhead)
    {
        posts.addPost(*(otherhead->post));
        otherhead = otherhead->next;
    }
    following = new FollowList();
    //
}

// Copy assignment operator
User &User::operator=(const User &other)
{
    if (this != &other)
    {
        userID = other.userID;
        userName = other.userName;
        while (posts.head)
        {
            posts.removePost(posts.head->post->postID);
        }
        PostNode *otherhead = other.posts.head;
        while (otherhead)
        {
            posts.addPost(*(otherhead->post));
            otherhead = otherhead->next;
        }
        delete following;
        following = new FollowList();
        FollowNode *otherfollow = other.following->head;
        while (otherfollow)
        {
            following->addFollowing(otherfollow->user);
            otherfollow = otherfollow->next;
        }
    }
    return *this;
}

// Move constructor
User::User(User &&other) noexcept
{
    userID = move(other.userID);
    userName = move(other.userName);
    following = other.following;
    other.following = nullptr;
    posts = move(other.posts);
}

// Move assignment operator
User &User::operator=(User &&other) noexcept
{
    if (this != &other)
    {
        delete following;
    }
    userID = other.userID;
    userName = other.userName;
    following = other.following;
    other.following = nullptr;
    posts = move(other.posts);
    return *this;
}

User::~User()
{
    delete following;
}

void User::addPost(int postID, const string &category)
{
    if (!posts.findPost(postID))
    {
        posts.addPost(Post(postID, category));
    }
}

void User::followUser(User *otherUser)
{
    if (this != otherUser && otherUser != nullptr)
    {
        if (!following->findFollowing(otherUser->userID))
        {
            following->addFollowing(otherUser);
        }
    }
}

void User::displayFollowing() const
{
    FollowNode* temp = following->head;
    while(temp)
    {
        cout << temp->user->userID << " " << temp->user->userName << endl;
        temp = temp->next;
    }
}