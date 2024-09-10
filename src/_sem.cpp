#include "../h/_sem.hpp"

_sem::_sem(sem_t *handle, unsigned int init) {
    *handle = this;
    val = init;
}


int _sem::wait() {

    TCB :: running ->setFlag(false);
    val = val -1;
    if(val < 0) {
        waiting.addLast(TCB::running);
        TCB::wait();
    }

    return (TCB::running->getFlag() ? -1 : 0);

}

int _sem::signal() {


    val = val+1;
    if (val <= 0) {
        TCB *tcb = waiting.removeFirst();
        tcb->release();
    }

    return 0;

}


int _sem::close() {

    TCB* tcb = waiting.removeFirst();
    while (tcb != nullptr) {
        tcb->setFlag(true);
        tcb->release();
        tcb = waiting.removeFirst();
    }


    return 0;

}



_sem::~_sem() {
    close();
}