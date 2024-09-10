#ifndef __sem_hpp
#define __sem_hpp

#include "syscall_c.hpp"
#include "list.hpp"
#include "tcb.hpp"
#include "../h/MemoryAllocator.hpp"

class _sem {
public:

    _sem(sem_t *handle, unsigned init);

    ~_sem();

    int wait();

    int signal();

    int close(); // oslobađa sve niti sa semafora i onemogućava dalje korišćenje semafora

    void* operator new(size_t size) {
        size_t sz = (sizeof(_sem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
        _sem* newT = (_sem*)MemoryAllocator::getInstance().mem_alloc(sz);
        return newT;
    }
    void operator delete(void *ptr) { MemoryAllocator::getInstance().mem_free(ptr); }

private:
    List waiting; //lista niti koje cekaju

    int val;
};

#endif //__sem_hpp