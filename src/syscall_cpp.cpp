#include "../h/syscall_cpp.hpp"

using size_t = decltype(sizeof(0));

void* operator new (size_t size) {
    return mem_alloc(size);
}


void operator delete (void* addr) {
    mem_free(addr);
}


int Thread::start() {
    return thread_create(&myHandle,body,arg);
}


void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void *thr)
{
    ((Thread*)thr)->run();
}

Thread ::Thread(void (*body)(void *), void *arg) : body(body),arg(arg) {}

Thread ::Thread() : Thread(Thread::wrapper,this){}


Thread::~Thread() {
    thread_delete(myHandle);
}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}


void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}