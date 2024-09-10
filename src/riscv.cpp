#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/printing.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"
#include "../h/_sem.hpp"
#include "../lib/mem.h"

void Riscv::restorePrivilege() {
    if (TCB::running->is_systhread()) {
        ms_sstatus(SSTATUS_SPP);
    } else {
        mc_sstatus(SSTATUS_SPP);
    }
}


inline void Riscv ::loadParams(uint64* arr) {
    __asm__ volatile ("mv %0, a0" : "=r" (arr[0]));
    __asm__ volatile ("mv %0, a1" : "=r" (arr[1]));
    __asm__ volatile ("mv %0, a2" : "=r" (arr[2]));
    __asm__ volatile ("mv %0, a3" : "=r" (arr[3]));
    __asm__ volatile ("mv %0, a4" : "=r" (arr[4]));
}

inline void Riscv::handleUnexpectedTrap()
{

    uint64 scause = r_scause();
    uint64 stvec = r_stvec();
    uint64 sepc = r_sepc();

    printString("scause: ");
    printHexa(scause);
    printString("\n");

    printString("stvec: ");
    printHexa(stvec);
    printString("\n");

    printString("sepc: ");
    printHexa(sepc);
    printString("\n");

    while(true) {}
}

uint64 Riscv::syscall(uint64 *args) {
    uint64 code = args[0];
    uint64 retval = 0;

    if (code == 0x01) { //mem_alloc
        size_t size = args[1];
        retval = (uint64) MemoryAllocator::getInstance().mem_alloc(size);
    } else if (code == 0x02) { //mem_free
        void *p = (void *) args[1];
        retval = MemoryAllocator::getInstance().mem_free(p);
    } else if (code == 0x11) { //thread_create
        thread_t *handle = (thread_t *) args[1];
        TCB::Body routine = (TCB::Body) args[2];
        void *arg = (void *) args[3];
        uint64 *stack = (uint64 *) args[4];
        *handle = TCB::createThread(routine, arg, stack);
        retval = (*handle)->start();
    } else if (code == 0x12) { // thread_exit
        retval = TCB::exit();
    } else if (code == 0x13) { // thread_dispatch
        TCB::dispatch();
    } else if (code == 0x14) { // thread_join
        thread_t handle = (thread_t)args[1];
        TCB::join(handle);
    }else if(code == 0x42) { //putc
        char c = args[1];
        __putc(c);
    }else if(code == 0x17) { // thread_delete
        thread_t handle = (thread_t)args[1];
        delete handle;
    }else if (code == 0x41) { //getc
        char c = __getc();
        retval = c;
    }else if(code == 0x21) { //sem_open
        sem_t* sem = (sem_t*)args[1];
        unsigned init = (unsigned )args[2];
        new _sem(sem,init);
    }else if(code == 0x22) { // sem_close
        sem_t sem = (sem_t)args[1];
        if(sem == nullptr) {retval = -1;}
        else {retval = sem->close();delete sem;}
    } else if(code == 0x23) { // sem_wait
        sem_t sem = (sem_t)args[1];
        if(sem == nullptr) {retval = -1;}
        else {retval = sem->wait();}
    } else if(code == 0x24) { // sem_signal
        sem_t sem = (sem_t)args[1];
        if(sem == nullptr) {retval = -1;}
        else {retval = sem->signal();}
    }

    return retval;

}

void Riscv::timer() {
    mc_sip(SIP_SSIP);
}

void Riscv::console() {
    console_handler();
}


void Riscv::handle() {

    uint64 args[5];
    Riscv::loadParams(args);


    uint64 scause;
    __asm__ volatile("csrr %[s],scause" : [s]"=r"(scause));

    if(scause == ECALL_SUPER || scause == ECALL_USER){

        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();
        uint64 value = syscall(args);
        w_sepc(sepc);
        w_sstatus(sstatus);
        w_scause(scause); // zbog getc jer konzola promeni scause i onda posle promeni i a0 registar
        __asm__ volatile("mv a0,%[v]" : :[v] "r"(value));

    }else {
        handleUnexpectedTrap();
    }

}

void Riscv::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

