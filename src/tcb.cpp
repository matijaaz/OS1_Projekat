#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"

TCB *TCB::running = nullptr;
TCB *TCB::kernel = nullptr;

TCB *TCB::createThread(TCB::Body body, void *arg, uint64 *stack) {
    size_t sz = (sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    TCB* newT = (TCB*)MemoryAllocator::getInstance().mem_alloc(sz);
    newT->body=body;
    if(body!= nullptr)newT->stack=stack;
    else stack= nullptr;
    newT->context={(uint64) &threadWrapper,stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0};
    newT->arg=arg;
    newT->setStatus(CREATED);
    if(body != nullptr) {
        newT->sys_thread = false;
    }else {
        newT->sys_thread = true;
    }

    newT->closed = false;

    //return new TCB(body,arg,stack);
    return newT;
}


void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && old->status != WAITING) { old->ready(); }
    running = Scheduler::get();
    running->setStatus(RUNNING);
    Riscv::restorePrivilege();
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::wait() {
    TCB::running->setStatus(WAITING);
    dispatch();

}

int TCB::release()
{
    if (status != WAITING) {
        return -1;
    }
    ready();
    return 0;
}


void TCB ::threadWrapper() {
    Riscv::restorePrivilege();
    Riscv ::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

int TCB::start()
{
    if (status != CREATED) {
        return -1;
    }
    if(body !=nullptr) {
        ready();
    }
    return 0;
}

void TCB::ready() {
    status = READY;
    Scheduler::put(this);

}

TCB *TCB::kernelThread()
{
   if (!kernel) {
        TCB *thr = new TCB(nullptr, nullptr, nullptr);
        running = kernel = thr;
        kernel->sys_thread = true;
        kernel->setStatus(RUNNING);
    }
    return kernel;
}

int TCB::exit() {

    if(running->status != RUNNING) return -1;

    TCB::running->setStatus(FINISHED);
    dispatch();
    return 0;
}

void TCB::join(TCB * handle) {
    while(!(handle->isFinished())) {
        dispatch();
    }
    return;
}