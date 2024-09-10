#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"

MemoryAllocator ::MemoryAllocator() {
    head = (FreeMem*)((char*)HEAP_START_ADDR);
    head->sz = ((uint64)((char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR))/MEM_BLOCK_SIZE;
    head->next = nullptr;
    head->prev = nullptr;
    head->status = FREE;
}

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    void* addr = nullptr;
    for (FreeMem* fm = head; fm != nullptr; fm = fm->next) {
        if (fm->sz>=size+1) {
            addr = fm;
            if (fm->sz-size-1==0) {
                if(fm->prev!=nullptr)fm->prev->next = fm->next;
                else  head=fm->next;
                if(fm->next!=nullptr)fm->next->prev =fm->prev;
                fm->sz = size+1;
                fm->status = ALLOCATED;
                break;
            }
            FreeMem* newFm = (FreeMem*)((char*)fm + (size+1)*MEM_BLOCK_SIZE);
            if(fm->prev)fm->prev->next=newFm;
            else head=newFm;
            if(fm->next)fm->next->prev=newFm;
            newFm->prev=fm->prev;
            newFm->next=fm->next;
            newFm->sz=fm->sz-size-1;
            newFm->status = FREE;
            fm->sz = size+1;
            fm->status = ALLOCATED;
            break;
        }
    }

    if (addr == nullptr) return nullptr;
    return (void*)((char*)addr+MEM_BLOCK_SIZE);

}

int MemoryAllocator::mem_free(void* addr) {
    if(addr == nullptr) return -1;
    if((char*)addr>(char*)((char*)HEAP_END_ADDR-1) || (char*)addr<(char*)HEAP_START_ADDR) return -1;
    FreeMem* cur;
    if (!head || ((char*)addr-MEM_BLOCK_SIZE)<(char*)head)
        cur = nullptr;
    else{
        for (cur=head; cur->next!=0 && ((char*)addr-MEM_BLOCK_SIZE)>(char*)(cur->next);cur=cur->next);
    }
    FreeMem* seg = (FreeMem*)((char*)addr-MEM_BLOCK_SIZE);
    if(seg->status != ALLOCATED) {
        return  -1;
    }
    seg->status = FREE;
    seg->prev = cur;
    if (cur) {
        seg->next = cur->next;
        cur->next = seg;
    }
    else {
        seg->next = head;
        //head = seg->next;
        head = seg;
    }
    if (seg->next) seg->next->prev = seg;
    tryToJoin(seg);
    tryToJoin(cur);
    return 0;
}


void MemoryAllocator::tryToJoin(FreeMem *cur) {
    if (!cur) return;
    if (cur->next && (char*)cur+cur->sz*MEM_BLOCK_SIZE == (char*)(cur->next)) {
        cur->sz += cur->next->sz;
        cur->next = cur->next->next;
        if (cur->next) cur->next->prev = cur;
    }
}
