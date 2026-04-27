#include "../include/ingest_queue.h"
using namespace std;

IngestQueue::IngestQueue(size_t capacity)
{
    this->capacity = capacity;
    head_idx = 0;
    tail_idx = 0;
    buffer = new Post *[capacity]();
    count = 0;
}

IngestQueue::~IngestQueue()
{
}

bool IngestQueue::enqueue(Post *p)
{
    if (count == capacity)
    {
        return false;
    }
    buffer[tail_idx] = p;
    tail_idx = (tail_idx + 1) % capacity;
    count++;
    return true;
}

Post *IngestQueue::dequeue()
{
    if (count == 0)
    {
        return nullptr;
    }
    Post *out = buffer[head_idx];
    buffer[head_idx] = nullptr;
    head_idx = (head_idx + 1) % capacity;
    count--;
    return out;
}

size_t IngestQueue::size() const
{
    return count;
}

bool IngestQueue::empty() const
{
    return count == 0;
}

size_t IngestQueue::dequeueBatch(Post **out_array, size_t max_k)
{
    size_t actual;
    if(max_k < count)
    {
        actual = max_k;
    }
    else
    {
        actual = count;
    }
    for (size_t i = 0; i < actual; i++)
    {
        out_array[i] = dequeue();
    }
    return actual;
}