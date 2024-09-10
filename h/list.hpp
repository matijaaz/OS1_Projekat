#ifndef LIST_HPP
#define LIST_HPP
#include "../lib/mem.h"
#include "../lib/hw.h"
#include "../h/printing.hpp"
#include "../h/MemoryAllocator.hpp"



class List
{
private:
    struct Elem
    {
        TCB *data;
        Elem *next;

        Elem(TCB *data, Elem *next) : data(data), next(next) {}

        void *operator new(size_t size) {
            size_t sz = (sizeof(Elem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
            Elem* elem = (Elem*)MemoryAllocator::getInstance().mem_alloc(sz);
            return elem;
        }

        void operator delete(void *ptr) { MemoryAllocator::getInstance().mem_free(ptr);}

    };

    Elem *head, *tail;


public:
    List() : head(nullptr), tail(nullptr) {}


    List(const List &) = delete;

    List &operator=(const List &) = delete;

    void addFirst(TCB *data)
    {
        Elem* elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(TCB *data)
    {
        Elem* elem = new Elem(data,nullptr);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return nullptr; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = nullptr; }

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekFirst()
    {
        if (!head) { return nullptr; }
        return head->data;
    }


    TCB *removeLast()
    {
        if (!head) { return nullptr; }

        Elem *prev = nullptr;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = nullptr; }
        else { head = nullptr;}
        tail = prev;

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekLast()
    {
        if (!tail) { return nullptr; }
        return tail->data;
    }


};

#endif //LIST_HPP