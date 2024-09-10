#ifndef _MEMORYALLOCATOR_HPP
#define _MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

enum MemStatus {
    FREE,
    ALLOCATED
};

struct FreeMem {
    FreeMem* next;
    FreeMem* prev;
    size_t sz;
    MemStatus status = FREE ;
};

class MemoryAllocator {
private :
    FreeMem* head = nullptr;
    MemoryAllocator();
    void tryToJoin(FreeMem* cur);
public :
    //MemoryAllocator(const MemoryAllocator& obj) = delete;
    static MemoryAllocator& getInstance();
    void* mem_alloc (size_t size);
    int mem_free (void* adr);
};


#endif //_MEMORYALLOCATOR_HPP