#include "../h/syscall_c.hpp"

inline void load_args()
{
    __asm__ volatile ("mv a4, a3");
    __asm__ volatile ("mv a3, a2");
    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
}

void *mem_alloc(size_t size) {
    if(!size) return nullptr;
    uint64 oper=0x01;
    size_t sz=(size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (sz));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));

    return (void*)x;
}

int mem_free (void* addr) {
    uint64 oper = 0x02;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (addr));
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}

int thread_create(thread_t *handle,void(*start_routine)(void*),void *arg) {
    if (!handle) { return -1; };
    uint64 *stack = (uint64*)mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    if (!stack) { return -2; }
    uint64 oper = 0x11;
    __asm__ volatile ("mv a4,%[op]" : :[op]"r" (stack));
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (handle));
    __asm__ volatile ("mv a2,%[op]" : :[op]"r" (start_routine));
    __asm__ volatile ("mv a3,%[op]" : :[op]"r" (arg));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}

int thread_exit () {
    uint64 oper = 0x12;
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}

void thread_dispatch () {
    uint64 oper = 0x13;
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
}


void thread_join (thread_t handle) {
    if(handle == nullptr) return;
    uint64 oper = 0x14;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");

}

int thread_delete(thread_t handle) {
    if(handle == nullptr) return 0;
    uint64 oper = 0x17;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}

void putc (char c) {
    uint64 oper = 0x42;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
}

char getc () {
    uint64 oper = 0x41;
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    int x = 0;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (char)x;
}

int sem_open (sem_t* handle,unsigned init) {
    if(!handle) {return -1;}
    uint64 oper = 0x21;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    int x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;

}


int sem_close (sem_t handle){
    uint64 oper = 0x22;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    int x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}



int sem_wait (sem_t id) {
    uint64 oper = 0x23;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    int x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;

}

int sem_signal (sem_t id) {
    uint64 oper = 0x24;
    load_args();
    __asm__ volatile ("mv a0,%[op]" : :[op]"r" (oper));
    __asm__ volatile ("ecall");
    int x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return x;
}