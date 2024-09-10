#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List readyCoroutineQueue;

public:
    static TCB *get();
    static void put(TCB *tcb);

};

#endif //SCHEDULER_HPP