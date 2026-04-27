#include "../include/post_pool.h"
using namespace std;

PostPool::PostPool(size_t block_size)
{
    if (block_size > 0)
    {
        this->block_size = block_size;
        current_block_index = 0;
        alloc_count = 0;
        reuse_count = 0;
        this->allocateBlock();
    }
}

PostPool::~PostPool()
{
    this->purge();
}

Post *PostPool::allocPost()
{
    if (free_list.empty())
    {
        if (block_size == current_block_index)
        {
            this->allocateBlock();
        }
        return &blocks[alloc_count - 1][current_block_index++];
    }
    else
    {
        Post *temp = free_list.back();
        free_list.pop_back();
        reuse_count++;
        return temp;
    }
}

void PostPool::freePost(Post *p)
{
    if (p != nullptr)
    {
        p->~Post();
        new (p) Post();
        free_list.push_back(p);
    }
}

size_t PostPool::totalAllocations() const
{
    return alloc_count;
}

size_t PostPool::reuseCount() const
{
    return reuse_count;
}

void PostPool::purge()
{
    int counter = 0;
    while (counter < alloc_count)
    {
        delete[] blocks[counter];
        counter++;
    }
    blocks.clear();
    free_list.clear();
    alloc_count = 0;
    reuse_count = 0;
    current_block_index = 0;
}

void PostPool::allocateBlock()
{
    Post *newblock = new Post[block_size];
    blocks.push_back(newblock);
    current_block_index = 0;
    alloc_count++;
}