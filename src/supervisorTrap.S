.align 4
.globl _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv,@function

_ZN5Riscv14supervisorTrapEv :
    j _ZN5Riscv10handleIntrEv
    j _ZN5Riscv11handleTimerEv
    li x0,1
    li x0,1
    li x0,1
    li x0,1
    li x0,1
    li x0,1
    li x0,1
    j _ZN5Riscv14handleHardwareEv
