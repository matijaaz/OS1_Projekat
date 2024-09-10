#ifndef SYSCALL_C_H
#define SYSCALL_C_H
#include "../lib/hw.h"

// ============== memorija ===============

void* mem_alloc (size_t size);
int mem_free (void*);


// ============== niti ===============

class TCB;
typedef TCB *thread_t;
int thread_create(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);

int thread_exit ();

int thread_delete(thread_t handle);

void thread_dispatch ();

void thread_join (
        thread_t handle
);

// ============== semafori ===============

class _sem;
typedef _sem* sem_t;
int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

// ============== konzola ===============

void putc (char);

const int EOF = -1;
char getc ();



#endif //SYSCALL_C_H