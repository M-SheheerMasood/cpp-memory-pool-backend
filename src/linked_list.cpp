#include "../include/linked_list.h"
#include "../include/user.h" // Add this so 'User' is complete
using namespace std;


template <typename T>
LinkedList<T>::LinkedList()
{
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    clear();
}

template <typename T>
typename LinkedList<T>::Node *LinkedList<T>::push_back(const T &val)
{
    Node *newnode = new Node(val);
    if (!_head)
    {
        _head = newnode;
        _tail = newnode;
    }
    else
    {
        _tail->next = newnode;
        newnode->prev = _tail;
        _tail = newnode;
    }
    _size++;
    return newnode;
}

template <typename T>
typename LinkedList<T>::Node *LinkedList<T>::push_front(const T &val)
{
    Node *newnode = new Node(val);
    if (!_head)
    {
        _head = newnode;
        _tail = newnode;
    }
    else
    {
        _head->prev = newnode;
        newnode->next = _head;
        _head = newnode;
    }
    _size++;
    return newnode;
}

template <typename T>
void LinkedList<T>::insert_after(Node *pos, const T &val)
{
    if (pos)
    {
        Node *newnode = new Node(val);
        if (pos == _tail)
        {
            _tail->next = newnode;
            newnode->prev = _tail;
            _tail = newnode;
        }
        else
        {
            newnode->prev = pos;
            newnode->next = pos->next;
            pos->next->prev = newnode;
            pos->next = newnode;
        }
        _size++;
    }
}

template <typename T>
void LinkedList<T>::remove(Node *node)
{
    if (node)
    {
        if (node == _head && node == _tail)
        {
            _head = nullptr;
            _tail = nullptr;
        }
        else if (node == _tail)
        {
            _tail->prev->next = nullptr;
            _tail = _tail->prev;
        }
        else if (node == _head)
        {
            _head->next->prev = nullptr;
            _head = _head->next;
        }
        else
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        delete node;
        _size--;
    }
}

template <typename T>
typename LinkedList<T>::Node *LinkedList<T>::find(function<bool(const T &)> pred)
{
    Node* temp = _head;
    while (temp)
    {
        if(pred(temp->data))
        {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

template <typename T>
typename LinkedList<T>::Node *LinkedList<T>::head() const
{
    return _head;
}

template <typename T>
typename LinkedList<T>::Node *LinkedList<T>::tail() const
{
    return _tail;
}

template <typename T>
size_t LinkedList<T>::size() const
{
    return _size;
}

template <typename T>
void LinkedList<T>::clear()
{
    while(_head)
    {
        Node* next = _head->next;
        delete _head;
        _head = next;
    }
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}

// Explicit template instantiation for commonly used types
template class LinkedList<int>;
template class LinkedList<string>;

// Forward declare User struct for template instantiation
struct User;
template class LinkedList<User>;