#ifndef _PRINTING_HPP_
#define _PRINTING_HPP_

#include "syscall_c.hpp"
#include "../lib/console.h"

typedef unsigned long uint64;

extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

void printString(char const *string);

char* getString(char *buf, int max);

int stringToInt(const char *s);

void printHexa(uint64 xx, uint padding=0);

void printInt(int xx, int base=10, int sgn=0);

#endif // _PRINTING_HPP_

