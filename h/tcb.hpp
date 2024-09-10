#ifndef TCB_HPP
#define TCB_HPP
#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

class TCB {

public:

    enum Status
    {
        FINISHED,
        RUNNING,
        READY,
        CREATED,
        WAITING,
    };

    ~TCB() {
        MemoryAllocator::getInstance().mem_free(stack);

    }

    static TCB* kernel;
    static TCB *kernelThread();
    int release();
    static void wait();
    bool is_systhread() const { return sys_thread; } // da li je nit sistemska (a ne korisnička)

    void setFlag(bool flag) {
        closed = flag;
    }

    bool getFlag() const {
        return closed;
    }

    bool isFinished() const { return status == FINISHED; }

    void setStatus(Status s) { status = s; }


    using Body = void (*)(void*);

    static TCB* createThread(Body body,void* arg, uint64* stack);

    static TCB *running;

    int start();//zapocinje kreiranu nit

    void* operator new(size_t size) {
        size_t sz = (sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
        TCB* newT = (TCB*)MemoryAllocator::getInstance().mem_alloc(sz);
        return newT;
    }

    void operator delete(void *ptr) { MemoryAllocator::getInstance().mem_free(ptr);}

private :

    explicit TCB(Body body,void* arg,uint64* stack) :
            body(body),
            arg(arg),
            sys_thread(false),
            status(CREATED),
            stack(body != nullptr ? stack : nullptr),
            context({(uint64)&threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    })

    {}

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };



    friend class Riscv;
    Body body;
    void* arg;
    bool sys_thread;
    Status status;
    uint64 *stack;
    Context context;
    bool closed;



    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static void threadWrapper();

    void ready(); //ubacuje nit u scheduler

    static int exit();

    static void join(TCB*);

};








#endif //TCB_HPP