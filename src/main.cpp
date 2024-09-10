#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/userMain.hpp"
#include "../h/tcb.hpp"
#include "../h/printing.hpp"




void user_wrapper(void *sem)
{
    userMain();
}


void main() {
    Riscv ::w_stvec((uint64)(&Riscv::supervisorTrap)+1);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    TCB* mainNit = nullptr;

    thread_create(&mainNit,nullptr,nullptr);
    TCB :: running = mainNit;
    mainNit->setStatus(TCB::RUNNING);

    //TCB *glavna = TCB::kernelThread();

    thread_t user;
    thread_create(&user,user_wrapper, nullptr);


    thread_join(user);


    delete user;
    delete mainNit;

    //__mem_free(kernel); //delete kernel






}
