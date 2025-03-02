#include "../h/syscall_c.hpp"


void *operator new[](size_t n)
{
    return mem_alloc(n);
}


void operator delete[](void *p) noexcept
{
   mem_free(p);
}