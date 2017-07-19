#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

/**
 * Relative offsets for registers stored on stack
 * within exception handlers.
 */

#define GPR1  0x0
#define GPR2  0x4
#define GPR3  0x8
#define GPR4  0xc
#define GPR5  0x10
#define GPR6  0x14
#define GPR7  0x18
#define GPR8  0x1c
#define GPR9  0x20
#define GPR10 0x24
#define GPR11 0x28
#define GPR12 0x2c
#define GPR13 0x30
#define GPR14 0x34
#define GPR15 0x38

/* (__BA_SIXTEEN_REGS__ is defined only when -m16regs switch is selected) */
#ifndef __BA_SIXTEEN_REGS__

/* this version for full-scale (32 GPR) BA22 */
#define GPR16 0x3c
#define GPR17 0x40
#define GPR18 0x44
#define GPR19 0x48
#define GPR20 0x4c
#define GPR21 0x50
#define GPR22 0x54
#define GPR23 0x58
#define GPR24 0x5c
#define GPR25 0x60
#define GPR26 0x64
#define GPR27 0x68
#define GPR28 0x6c
#define GPR29 0x70
#define GPR30 0x74
#define GPR31 0x78
#define EPCR  0x7c
#define ESR   0x80
#define EEAR  0x84

#else

/* this version for BA22-DE (16 GPR) */
#define EPCR  0x3c
#define ESR   0x40
#define EEAR  0x44

#endif

/**
 * Red zone is area below SP that compiled programs can use for their
 * temporary storage. Exception handlers must not write over that.
 *
 * If you use different red zone size (through -mredzone-size compiler switch)
 * you need to change this value (or you will (temporarily) waste some stack
 * in each exception handler.
 */
#define RED_ZONE_SIZE   (0x80)

/**
 * Total storage required by all the registers we save in generic
 * exception handler.
 */
#define INT_STACK_FRAME (EEAR+4)

#ifndef __BA_SIXTEEN_REGS__

/**
 * This macro can be used at exception address to define
 * exception handler that:
 *   1) creates new stack frame (skipping red zone)
 *   2) saves all general purpose registers to stack
 *   3) saves EPCR, ESR, EEAR to stack
 *   4) calls (using b.jal) exception handler subroutine.
 *      This can be a C routine, declared as either:
 *        -void handler_name()
 *        -void handler_name(unsigned long sp) or
 *         void handler_name(void *sp)
 *           (this version allows you to access stored registers,
 *            if you never use this you can remove mov r3,r1 below)
 *   5) restores EPCR, ESR, EEAR (from stack)
 *   5) restores general purpose registers from stack
 *   6) returns from exception (restoring PC from EPCR, etc.)
 * PARAMETERS:
 *   handler: name of C exception handler routine
 * NOTES:
 *  If you handle exception wholly in assembler, optimizations are
 * possible, as you don't need to save all registers etc.
 *
 *  This version does not enable interrupts (so all of your handler
 * will run with interrupts disabled, unless you enable them).
 * b.di below is redundant for these samples, however in no case
 * can interrupts be enabled during context switch.
 *
 * This version is for full-scale BA22.
 * (this will be compiled only if -m16regs switch is NOT selected)
 */
#define HANDLE_EXCEPTION(handler)                               \
        b.sw    GPR1-INT_STACK_FRAME-RED_ZONE_SIZE(r1),r1;      \
        b.addi  r1,r1,-INT_STACK_FRAME-RED_ZONE_SIZE;           \
        b.msw   GPR2(r1),r2,1;                                  \
        b.msw   GPR5(r1),r5,1;                                  \
        b.msw   GPR8(r1),r8,3;                                  \
        b.msw   GPR16(r1),r16,3;                                \
        b.msw   GPR24(r1),r24,3;                                \
        b.mfspr r13,r0,SPR_EPCR_BASE;                           \
        b.mfspr r14,r0,SPR_ESR_BASE;                            \
        b.mfspr r15,r0,SPR_EEAR_BASE;                           \
        b.msw   EPCR(r1),r13,1;                                 \
        b.mov   r3,r1;                                          \
        b.jal   handler;                                        \
        b.di;                                                   \
        b.mlwz  r13,EPCR(r1),1;                                 \
        b.mtspr r0,r13,SPR_EPCR_BASE;                           \
        b.mtspr r0,r14,SPR_ESR_BASE;                            \
        b.mtspr r0,r15,SPR_EEAR_BASE;                           \
        b.mlwz  r2,GPR2(r1),1;                                  \
        b.mlwz  r5,GPR5(r1),1;                                  \
        b.mlwz  r8,GPR8(r1),3;                                  \
        b.mlwz  r16,GPR16(r1),3;                                \
        b.mlwz  r24,GPR24(r1),3;                                \
        b.lwz   r1,GPR1(r1);                                    \
        b.rfe

#else

/* BA22-DE (16 GPR) version of exception handler stub. Same comments
 * as above apply. */
#define HANDLE_EXCEPTION(handler)                               \
        b.sw    GPR1-INT_STACK_FRAME-RED_ZONE_SIZE(r1),r1;      \
        b.addi  r1,r1,-INT_STACK_FRAME-RED_ZONE_SIZE;           \
        b.msw   GPR2(r1),r2,1;                                  \
        b.msw   GPR5(r1),r5,1;                                  \
        b.msw   GPR8(r1),r8,3;                                  \
        b.mfspr r13,r0,SPR_EPCR_BASE;                           \
        b.mfspr r14,r0,SPR_ESR_BASE;                            \
        b.mfspr r15,r0,SPR_EEAR_BASE;                           \
        b.msw   EPCR(r1),r13,1;                                 \
        b.mov   r3,r1;                                          \
        b.jal   handler;                                        \
        b.di;                                                   \
        b.mlwz  r13,EPCR(r1),1;                                 \
        b.mtspr r0,r13,SPR_EPCR_BASE;                           \
        b.mtspr r0,r14,SPR_ESR_BASE;                            \
        b.mtspr r0,r15,SPR_EEAR_BASE;                           \
        b.mlwz  r2,GPR2(r1),1;                                  \
        b.mlwz  r5,GPR5(r1),1;                                  \
        b.mlwz  r8,GPR8(r1),3;                                  \
        b.lwz   r1,GPR1(r1);                                    \
        b.rfe

#endif

/**
 * This macro can be used for exceptions, you don't handle.
 * It will keep looping at exception location, which allows
 * you to detect unexpected condition by attaching the debugger.
 * Never use this in production code.
 */
#define UNHANDLED_EXCEPTION \
1:      b.nop 1; \
        b.j 1b

#endif /* EXCEPTIONS_H_ */
