.globl _ZN5Riscv10handleIntrEv
.type _ZN5Riscv10handleIntrEv,@function

_ZN5Riscv10handleIntrEv :

    addi sp, sp, -256
    .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv6handleEv

    csrr a1, scause
    addi a1, a1, -8
    beqz a1, syscall
    addi a1, a1, -1
    bnez a1, other
syscall:
    sd x10, 80(sp)
other:
     .irp index, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
     ld x\index, \index * 8(sp)
     .endr
     addi sp, sp, 256
     sret