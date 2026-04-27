#include "../include/user_manager.h"
#include "../include/user.h"
#include "../include/follow_list.h"
#include "../include/post_pool.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

LinkedList<User>::Node *UserManager::createUser(int userID, const string &username)
{
    if (this->findUserByID(userID) || this->findUserByName(username))
    {
        return nullptr;
    }
    else
    {
        User newuser = User(userID, username);
        users.push_back(newuser);
        return users.tail();
    }
}

bool UserManager::deleteUser(int userID)
{
    if (!this->findUserByID(userID))
    {
        return false;
    }
    else
    {
        LinkedList<User>::Node *curr = users.head();
        while (curr)
        {
            if (curr->data.userID != userID)
            {
                curr->data.following->removeFollowing(userID);
            }
            curr = curr->next;
        }
        users.remove(this->findUserByID(userID));
        return true;
    }
}

bool UserManager::follow(int followerID, int followeeID)
{
    if (followeeID == followerID)
    {
        return false;
    }
    LinkedList<User>::Node *user1 = this->findUserByID(followeeID);
    LinkedList<User>::Node *follower = this->findUserByID(followerID);
    if (user1 == nullptr || follower == nullptr)
    {
        return false;
    }
    if (follower->data.following->findFollowing(user1->data.userID))
    {
        return false;
    }
    follower->data.followUser(&(user1->data));
    return true;
}

bool UserManager::unfollow(int followerID, int followeeID)
{
    if (followeeID == followerID)
    {
        return false;
    }
    LinkedList<User>::Node *user1 = this->findUserByID(followeeID);
    LinkedList<User>::Node *follower = this->findUserByID(followerID);
    if (user1 == nullptr || follower == nullptr)
    {
        return false;
    }
    if (follower->data.following->findFollowing(user1->data.userID))
    {
        follower->data.following->removeFollowing(user1->data.userID);
        return true;
    }
    return false;
}

bool UserManager::isFollowing(int followerID, int followeeID) const
{
    if (followeeID == followerID)
    {
        return false;
    }
    LinkedList<User>::Node *user1 = const_cast<UserManager *>(this)->findUserByID(followeeID);
    LinkedList<User>::Node *follower = const_cast<UserManager *>(this)->findUserByID(followerID);
    if (user1 == nullptr || follower == nullptr)
    {
        return false;
    }
    if (follower->data.following->findFollowing(user1->data.userID))
    {
        return true;
    }
    return false;
}

bool UserManager::addPost(int userID, Post *post)
{
    if (!post)
    {
        return false;
    }
    LinkedList<User>::Node *u1 = this->findUserByID(userID);
    if (!u1)
    {
        return false;
    }
    if (u1->data.posts.findPost(post->postID))
    {
        return false;
    }
    u1->data.posts.addPost(*post);
    return true;
}

bool UserManager::deletePost(int userID, PostID postID)
{
    LinkedList<User>::Node *u1 = this->findUserByID(userID);
    if (!u1)
    {
        return false;
    }
    return u1->data.posts.removePost(postID);
}

LinkedList<User>::Node *UserManager::findUserByID(int userID)
{
    LinkedList<User>::Node *user1 = users.find([&](const User &u)
                                               { return u.userID == userID; });
    return user1;
}

LinkedList<User>::Node *UserManager::findUserByName(const string &username)
{
    LinkedList<User>::Node *user1 = users.find([&](const User &u)
                                               { return u.userName == username; });
    return user1;
}

void UserManager::exportUsersCSV(const string &path) const
{
    ofstream file(path);
    if (users.size() != 0)
    {
        LinkedList<User>::Node *user = users.head();
        while (user)
        {
            file << user->data.userID << "," << user->data.userName << ",";
            auto followers = user->data.following->head;
            while (followers)
            {
                file << followers->user->userID << "|";
                followers = followers->next;
            }
            file << ",";
            PostNode *posts = user->data.posts.head;
            while (posts)
            {
                file << posts->post->postID << ":" << posts->post->category << ":" << posts->post->views << "|";
                posts = posts->next;
            }
            file << ",";
            user = user->next;
            file << endl;
        }
    }
    file.close();
}

void UserManager::importUsersCSV(const string &path)
{
    ifstream file(path);
    users.clear();
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string userid, username, follows, posts, singlepost = "";
        getline(ss, userid, ',');
        getline(ss, username, ',');
        getline(ss, follows, ',');
        getline(ss, posts, ',');
        int userID = stoi(userid);
        users.push_front(User(userID, username));
        stringstream pss(posts);
        while (getline(pss, singlepost, '|'))
        {
            string postid, postcategory, postviews = "";
            stringstream pssA(singlepost);
            getline(pssA, postid, ':');
            getline(pssA, postcategory, ':');
            getline(pssA, postviews, ':');
            int postID = stoi(postid);
            int postVIEWS = stoi(postviews);
            users.head()->data.addPost(postID, postcategory);
            users.head()->data.posts.head->post->views = postVIEWS;
        }
    }
    file.close();
    file.open(path);
    while (getline(file, line))
    {
        stringstream ss(line);
        string userid, username, follows, posts, singlefollower = "";
        getline(ss, userid, ',');
        getline(ss, username, ',');
        getline(ss, follows, ',');
        getline(ss, posts, ',');
        int userID = stoi(userid);
        stringstream fss(follows);
        LinkedList<User>::Node *u1 = findUserByID(userID);
        while (getline(fss, singlefollower, '|'))
        {
            string followerid = singlefollower;
            int followerID = stoi(singlefollower);
            LinkedList<User>::Node *follower = findUserByID(followerID);
            u1->data.followUser(&(follower->data));
        }
    }
}

void UserManager::dumpAllUsers(ostream &out) const
{
}