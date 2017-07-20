/* Definitions of BA2 special-purpose Registers (SPRs) */

#ifndef _SPR_DEFS_H
#define _SPR_DEFS_H

/* Helper macros */
#define MAX_GRPS                (32)                            /* Maximum number or SPR groups */
#define MAX_SPRS_PER_GRP_BITS   (11)                            /* Number of address bits for each group */
#define MAX_SPRS_PER_GRP        (1 << MAX_SPRS_PER_GRP_BITS)	/* Maximum number of SPRs in each SPR group */
#define MAX_SPRS                (0x10000)			/* Maximum number of SPR registers in all groups together */
#define SPR_GRP_OFF(spr)        ((spr) & (MAX_SPRS_PER_GRP - 1))/* Converts full SPR number to a number within group */

/* Base addresses for the groups */
#define SPRGROUP_SYS            (0 << MAX_SPRS_PER_GRP_BITS)    /* System Control and Status Registers */
#define SPRGROUP_DMMU           (1 << MAX_SPRS_PER_GRP_BITS)    /* Data MMU */
#define SPRGROUP_IMMU           (2 << MAX_SPRS_PER_GRP_BITS)    /* Instruction MMU */
#define SPRGROUP_DC             (3 << MAX_SPRS_PER_GRP_BITS)    /* Data Cache */
#define SPRGROUP_IC             (4 << MAX_SPRS_PER_GRP_BITS)    /* Instruction Cache */
#define SPRGROUP_MAC            (5 << MAX_SPRS_PER_GRP_BITS)    /* MAC Unit */
#define SPRGROUP_D              (6 << MAX_SPRS_PER_GRP_BITS)    /* Debug Unit */
#define SPRGROUP_PC             (7 << MAX_SPRS_PER_GRP_BITS)    /* Performance Counters Unit */
#define SPRGROUP_PM             (8 << MAX_SPRS_PER_GRP_BITS)    /* Power Management */
#define SPRGROUP_PIC            (9 << MAX_SPRS_PER_GRP_BITS)    /* PIC */
#define SPRGROUP_TT             (10 << MAX_SPRS_PER_GRP_BITS)   /* Tick Timer */
#define SPRGROUP_MID            (11 << MAX_SPRS_PER_GRP_BITS)   /* Memory interface for debuggers */
#define SPRGROUP_MPU            (13 << MAX_SPRS_PER_GRP_BITS)   /* Memory Protection Unit */

/* System control and status group */
#define SPR_VR                  (SPRGROUP_SYS + 0)              /* Version Register */
#define SPR_UPR                 (SPRGROUP_SYS + 1)              /* Unit Present Register */
#define SPR_CPUCFGR             (SPRGROUP_SYS + 2)              /* CPU config Register */
#define SPR_DMMUCFGR            (SPRGROUP_SYS + 3)              /* Data MMU config Register */
#define SPR_IMMUCFGR            (SPRGROUP_SYS + 4)              /* Instruction MMU config Register */
#define SPR_DCCFGR              (SPRGROUP_SYS + 5)              /* Data Cache configuration Register */
#define SPR_ICCFGR              (SPRGROUP_SYS + 6)              /* Instruction Cache configuration Register */
#define SPR_DCFGR               (SPRGROUP_SYS + 7)              /* Debug configuration Register */
#define SPR_PCCFGR              (SPRGROUP_SYS + 8)              /* Performance Counters configuration Register */
#define SPR_CPUID               (SPRGROUP_SYS + 9)              /* CPU ID */
#define SPR_NPC                 (SPRGROUP_SYS + 16)             /* PC mapped to SPR space */
#define SPR_SR                  (SPRGROUP_SYS + 17)             /* Supervisor Register */
#define SPR_UR                  (SPRGROUP_SYS + 19)             /* User register */
#define SPR_RIR_MIN             (SPRGROUP_SYS + 20)             /* Range Invalidate Register MIN */
#define SPR_RIR_MAX             (SPRGROUP_SYS + 21)             /* Range Invalidate Register MAX */
#define SPR_EPCR                (SPRGROUP_SYS + 32)             /* Exception PC Register */
#define SPR_EPCR_BASE           (SPRGROUP_SYS + 32)             /* Exception PC Register */
#define SPR_EPCR_LAST           (SPRGROUP_SYS + 47)             /* Last EPCR */
#define SPR_EEAR                (SPRGROUP_SYS + 48)             /* Exception EA Register */
#define SPR_EEAR_BASE           (SPRGROUP_SYS + 48)             /* Exception EA Register */
#define SPR_EEAR_LAST           (SPRGROUP_SYS + 63)             /* Last EEAR */
#define SPR_ESR                 (SPRGROUP_SYS + 64)             /* Exception SR Register */
#define SPR_ESR_BASE            (SPRGROUP_SYS + 64)             /* Exception SR Register */
#define SPR_ESR_LAST            (SPRGROUP_SYS + 79)             /* Last ESR */
#define SPR_JPC                 (SPRGROUP_SYS + 0x80)           /* PC of last jump instruction */
#define SPR_GPR(x)              (SPRGROUP_SYS + 0x400 + (x))    /* GPRs mapped to SPR space - Software should NOT use these! */

/* Data MMU group */
#define SPR_DTLBEIR             (SPRGROUP_DMMU + 2)             /* Data TLB Entry invalidate Register */
#define SPR_DTLBMR2             (SPRGROUP_DMMU + 8)             /* Data TLB Match Register (v3) */
#define SPR_DTLBTR2             (SPRGROUP_DMMU + 10)            /* Data TLB Translate Register (v3) */
#define SPR_DTLBMR_BASE(WAY)    (SPRGROUP_DMMU + 0x200 + (WAY) * 0x100)
#define SPR_DTLBMR_LAST(WAY)    (SPRGROUP_DMMU + 0x27f + (WAY) * 0x100)
#define SPR_DTLBTR_BASE(WAY)    (SPRGROUP_DMMU + 0x280 + (WAY) * 0x100)
#define SPR_DTLBTR_LAST(WAY)    (SPRGROUP_DMMU + 0x2ff + (WAY) * 0x100)

/* Instruction MMU group */
#define SPR_ITLBEIR             (SPRGROUP_IMMU +  2)            /* Instruction TLB Entry Invalidate Register */
#define SPR_ITLBMR2             (SPRGROUP_IMMU +  8)            /* Instruction TLB Match Register (v3) */
#define SPR_ITLBTR2             (SPRGROUP_IMMU + 10)            /* Instruction TLB Translate Register (v3) */
#define SPR_ITLBMR_BASE(WAY)    (SPRGROUP_IMMU + 0x200 + (WAY) * 0x100)
#define SPR_ITLBMR_LAST(WAY)    (SPRGROUP_IMMU + 0x27f + (WAY) * 0x100)
#define SPR_ITLBTR_BASE(WAY)    (SPRGROUP_IMMU + 0x280 + (WAY) * 0x100)
#define SPR_ITLBTR_LAST(WAY)    (SPRGROUP_IMMU + 0x2ff + (WAY) * 0x100)

/* Data cache group */
#define SPR_DCBFR               (SPRGROUP_DC +  2)              /* Data Cache Block Flush Register */
#define SPR_DCBIR               (SPRGROUP_DC +  3)              /* Data Cache Block Invalidate Register */

/* Instruction cache group */
#define SPR_ICBIR               (SPRGROUP_IC +  2)              /* Instruction Cache Block Invalidate Register */

/* MAC group */
#define SPR_MACLO               (SPRGROUP_MAC + 1)              /* MAC Low */
#define SPR_MACHI               (SPRGROUP_MAC + 2)              /* MAC High */

/* Debug group */
#define SPR_DVR(N)              (SPRGROUP_D + (N))              /* Debug Value Registers */
#define SPR_DCR(N)              (SPRGROUP_D + 8 + (N))          /* Debug Control Registers */
#define SPR_DMR1                (SPRGROUP_D + 16)               /* Debug Mode Register 1 */
#define SPR_DMR2                (SPRGROUP_D + 17)               /* Debug Mode Register 2 */
#define SPR_DSR                 (SPRGROUP_D + 20)               /* Debug Stop Register */
#define SPR_DRR                 (SPRGROUP_D + 21)               /* Debug Reason Register */
#define SPR_DTR                 (SPRGROUP_D + 23)               /* Debug trace port register */
#define SPR_DTUDPR              (SPRGROUP_D + 24)               /* Debug trace port user defined packet register */

/* v2 debug group */
#define SPR_DUIFAC(N)           (SPRGROUP_D + 0x20 + 2*(N))     /* instruction fetch watchpoint registers: control */
#define SPR_DUIFAV(N)           (SPRGROUP_D + 0x21 + 2*(N))     /* instruction fetch watchpoint registers: value */
#define SPR_DULSAC(N)           (SPRGROUP_D + 0x30 + 2*(N))     /* load/store address watchpoint registers: control */
#define SPR_DULSAV(N)           (SPRGROUP_D + 0x31 + 2*(N))     /* load/store address watchpoint registers: value */
#define SPR_DULSDC(N)           (SPRGROUP_D + 0x40 + 2*(N))     /* load/store data watchpoint registers: control */
#define SPR_DULSDV(N)           (SPRGROUP_D + 0x41 + 2*(N))     /* load/store data watchpoint registers: value */
#define SPR_DUWR                (SPRGROUP_D + 0x50)             /* watchpoints enabled register */
#define SPR_DUHR                (SPRGROUP_D + 0x51)             /* watchpoints hit register */

/* Trace Buffer */
#define SPR_DTBRP               (SPRGROUP_D + 0xfe)             /* Trace Buffer Read Pointer */
#define SPR_DTBWP               (SPRGROUP_D + 0xff)             /* Trace Buffer Write Pointer */
#define SPR_DTBRBASE            (SPRGROUP_D + 0x100)            /* Program Counters */
#define SPR_DTBIA               (SPRGROUP_D + 0x200)            /* High 4 bytes of Instruction */
#define SPR_DTBAR               (SPRGROUP_D + 0x300)            /* Data written to Register File */
#define SPR_DTBTS               (SPRGROUP_D + 0x400)            /* Lower 2 bytes of the Instruction [31:16], Free Running Counter [15:0] */
#define SPR_DTBFC               (SPRGROUP_D + 0x500)            /* Non-architectural state */

/* Performance counters group */
#define SPR_PCCR(N)             (SPRGROUP_PC + (N))             /* Performance Counters Count Registers */
#define SPR_PCMR(N)             (SPRGROUP_PC + 8 + (N))         /* Performance Counters Count Mode Registers */

/* Power management group */
#define SPR_PMR                 (SPRGROUP_PM + 0)               /* Power Management Register Group (alias for PMEVNT) */
#define SPR_PM_EVENT_CTRL       (SPRGROUP_PM + 0)               /* Power Management Event Control register */
#define SPR_PM_UNITS_PD         (SPRGROUP_PM + 2)               /* Power Management Unit(s) Power Down register */

/* PIC group */
#define SPR_PICMR0              (SPRGROUP_PIC + 0x0)            /* PIC Mask Register 0 (interrupts 0-31)  */
#define SPR_PICMR1              (SPRGROUP_PIC + 0x1)            /* PIC Mask Register 1 (interrupts 32-63) */
#define SPR_PICSR0              (SPRGROUP_PIC + 0x2)            /* PIC Status Register 0 (interrupts 0-31)  */
#define SPR_PICSR1              (SPRGROUP_PIC + 0x3)            /* PIC Status Register 1 (interrupts 32-63) */
#define SPR_PICTR0              (SPRGROUP_PIC + 0x4)            /* PIC Trigger Register 0 (interrupts 0-31)  */
#define SPR_PICTR1              (SPRGROUP_PIC + 0x5)            /* PIC Trigger Register 1 (interrupts 32-63) */
#define SPR_PICMR2              (SPRGROUP_PIC + 0x8)            /* PIC Mask Register 2 (interrupts 64-95)  */
#define SPR_PICMR3              (SPRGROUP_PIC + 0x9)            /* PIC Mask Register 3 (interrupts 96-127) */
#define SPR_PICSR2              (SPRGROUP_PIC + 0xa)            /* PIC Status Register 2 (interrupts 64-95)  */
#define SPR_PICSR3              (SPRGROUP_PIC + 0xb)            /* PIC Status Register 3 (interrupts 96-127) */
#define SPR_PICTR2              (SPRGROUP_PIC + 0xc)            /* PIC Trigger Register 2 (interrupts 64-95)  */
#define SPR_PICTR3              (SPRGROUP_PIC + 0xd)            /* PIC Trigger Register 3 (interrupts 96-127) */
/* PIC register name aliases */
#define SPR_PICMR               (SPR_PICMR0)
#define SPR_PICMRE              (SPR_PICMR1)
#define SPR_PICSR               (SPR_PICSR0)
#define SPR_PICSRE              (SPR_PICSR1)
#define SPR_PICTR               (SPR_PICTR0)
#define SPR_PICTRE              (SPR_PICTR1)

/* VIC */
#define SPR_VICMR0              (SPRGROUP_PIC + 0x200)          /* VIC Mask Register 0 (interrupts 0-31) */
#define SPR_VICMR1              (SPRGROUP_PIC + 0x201)          /* VIC Mask Register 1 (interrupts 32-63) */
#define SPR_VICMR2              (SPRGROUP_PIC + 0x202)          /* VIC Mask Register 2 (interrupts 64-95) */
#define SPR_VICMR3              (SPRGROUP_PIC + 0x203)          /* VIC Mask Register 3 (interrupts 96-127) */
#define SPR_VICSR0              (SPRGROUP_PIC + 0x204)          /* VIC Status Register 0 (interrupts 0-31) */
#define SPR_VICSR1              (SPRGROUP_PIC + 0x205)          /* VIC Status Register 1 (interrupts 32-63) */
#define SPR_VICSR2              (SPRGROUP_PIC + 0x206)          /* VIC Status Register 2 (interrupts 64-95) */
#define SPR_VICSR3              (SPRGROUP_PIC + 0x207)          /* VIC Status Register 3 (interrupts 96-127) */
#define SPR_VICTR0              (SPRGROUP_PIC + 0x208)          /* VIC Trigger Register 0 (interrupts 0-31) */
#define SPR_VICTR1              (SPRGROUP_PIC + 0x209)          /* VIC Trigger Register 1 (interrupts 32-63) */
#define SPR_VICTR2              (SPRGROUP_PIC + 0x20a)          /* VIC Trigger Register 2 (interrupts 64-95) */
#define SPR_VICTR3              (SPRGROUP_PIC + 0x20b)          /* VIC Trigger Register 3 (interrupts 96-127) */
#define SPR_VICMR(x)            (SPRGROUP_PIC + 0x200 + (x))    /* VIC mask registers */
#define SPR_VICSR(x)            (SPRGROUP_PIC + 0x204 + (x))    /* VIC status registers */
#define SPR_VICTR(x)            (SPRGROUP_PIC + 0x208 + (x))    /* VIC trigger registers */
#define SPR_VICMPR              (SPRGROUP_PIC + 0x20c)          /* VIC minimum priority register */
#define SPR_VICPR(x)            (SPRGROUP_PIC + 0x300 + (x))    /* VIC priority registers (1-127) */
#define SPR_VICVA(x)            (SPRGROUP_PIC + 0x380 + (x))    /* VIC vector address registers (1-127) */

/* Tick Timer group */
#define SPR_TTMR                (SPRGROUP_TT + 0)               /* Tick Timer Mode Register */
#define SPR_TTCR                (SPRGROUP_TT + 1)               /* Tick Timer Count Register */
#define SPR_TTRS                (SPRGROUP_TT + 2)               /* Tick Timer Restart Register */
#define SPR_TTMTS_LO            (SPRGROUP_TT + 4)               /* Tick Timer Monotonous Timestamp Low */
#define SPR_TTMTS_HI            (SPRGROUP_TT + 5)               /* Tick Timer Monotonous Timestamp High */

/* Memory interface for debuggers group */
#define SPR_DADDR               (SPRGROUP_MID + 0)              /* Memory address Register */
#define SPR_DCMD                (SPRGROUP_MID + 1)              /* Memory access command Register */
#define SPR_DDATA               (SPRGROUP_MID + 2)              /* Memory read/write data Register */

/* memory protection unit group */
#define SPR_MPGCR               (SPRGROUP_MPU + 0)              /* MPU Global Control Register */
#define SPR_MPLBR(N)            (SPRGROUP_MPU + N*3 + 1)        /* MPU lower boundary register(s) */
#define SPR_MPUBR(N)            (SPRGROUP_MPU + N*3 + 2)        /* MPU upper boundary register(s) */
#define SPR_MPCR(N)             (SPRGROUP_MPU + N*3 + 3)        /* MPU control register(s) */

/*
 * Bit definitions for the Version Register
 */
#define SPR_VR_MODEL            0xff000000                      /* Processor Model */
#define SPR_VR_MA               0x0000f000                      /* Processor major version */
#define SPR_VR_MI               0x00000f00                      /* Processor minor version */
#define SPR_VR_REV              0x000000ff                      /* Processor revision */

/*
 * Bit definitions for the Unit Present Register
 */
#define SPR_UPR_UP              0x00000001                      /* UPR present */
#define SPR_UPR_DCP             0x00000002                      /* Data cache present */
#define SPR_UPR_ICP             0x00000004                      /* Instruction cache present */
#define SPR_UPR_DMP             0x00000008                      /* Data MMU present */
#define SPR_UPR_IMP             0x00000010                      /* Instruction MMU present */
#define SPR_UPR_MP              0x00000020                      /* MAC Present */
#define SPR_UPR_DUP             0x00000040                      /* Debug unit present */
#define SPR_UPR_PCUP            0x00000080                      /* Performance counters unit present */
#define SPR_UPR_PMP             0x00000100                      /* Power management present */
#define SPR_UPR_PICP            0x00000200                      /* PIC present */
#define SPR_UPR_TTP             0x00000400                      /* Tick timer present */
#define SPR_UPR_FPUSP           0x00000800                      /* Floating Point Single Precision present */
#define SPR_UPR_FPUDP           0x00001000                      /* Floating Point Double Precision present */
#define SPR_UPR_VICP            0x00002000                      /* Vector interrupt controller present */
#define SPR_UPR_COPP            0x00004000                      /* Coprocessor present */
#define SPR_UPR_MPUP            0x00008000                      /* Memory Protection Unit present */
#define SPR_UPR_CNG             0x00800000                      /* Custom number of registers (16) */
#define SPR_UPR_MDB             0x01000000                      /* Memory interface for debuggers present */
#define SPR_UPR_CUST            0xfe000000                      /* Custom units */

/*
 * Bit definitions for the Supervision Register
 */
#define SPR_SR_SM               0x00000001                      /* Supervisor Mode */
#define SPR_SR_TEE              0x00000002                      /* Tick timer Exception Enable */
#define SPR_SR_IEE              0x00000004                      /* Interrupt Exception Enable */
#define SPR_SR_DCE              0x00000008                      /* Data Cache Enable */
#define SPR_SR_ICE              0x00000010                      /* Instruction Cache Enable */
#define SPR_SR_DME              0x00000020                      /* Data MMU Enable */
#define SPR_SR_IME              0x00000040                      /* Instruction MMU Enable */
#define SPR_SR_LEE              0x00000080                      /* Little Endian Enable */
#define SPR_SR_SBE              0x00000100                      /* Store Buffer Enable */
#define SPR_SR_F                0x00000200                      /* Condition Flag */
#define SPR_SR_CY               0x00000400                      /* Carry flag */
#define SPR_SR_OV               0x00000800                      /* Overflow flag */
#define SPR_SR_TED              0x00001000                      /* Trap Exception Disabled */
#define SPR_SR_ST               0x00002000                      /* Single-step trace */
#define SPR_SR_EPH              0x00004000                      /* Exception Prefix High */
#define SPR_SR_FO               0x00008000                      /* Fixed one */
#define SPR_SR_FPEE_FOV         0x00010000                      /* Trigger Exception on Overflow */
#define SPR_SR_FPEE_FUN         0x00020000                      /* Trigger Exception on Underflow */
#define SPR_SR_FPEE_FSN         0x00040000                      /* Trigger Exception on Signaling NaN */
#define SPR_SR_FPEE_FQN         0x00080000                      /* Trigger Exception on Quiet NaN */
#define SPR_SR_FPEE_FZ          0x00100000                      /* Trigger Exception on Zero */
#define SPR_SR_FPEE_FIX         0x00200000                      /* Trigger Exception on Inexact */
#define SPR_SR_FPEE_FIV         0x00400000                      /* Trigger Exception on Invalid */
#define SPR_SR_FPEE_FIN         0x00800000                      /* Trigger Exception on Infinity */
#define SPR_SR_FPEE_FDZ         0x01000000                      /* Trigger Exception on Division by Zero */
#define SPR_SR_FPEE             0x01FF0000                      /* Trigger Exception on All */
#define SPR_SR_CID              0xf0000000                      /* Context ID */

/*
 * Bit definitions for the User Register
 */
#define SPR_UR_MACCY            0x00000400                      /* MAC Set Carry */
#define SPR_UR_MACOVH           0x00000800                      /* MAC Set Overflow High */
#define SPR_UR_MACOVL           0x00001000                      /* MAC Set Overflow Low */
#define SPR_UR_FOV              0x00010000                      /* Overflow */
#define SPR_UR_FUN              0x00020000                      /* Underflow */
#define SPR_UR_FSN              0x00040000                      /* Signaling NaN */
#define SPR_UR_FQN              0x00080000                      /* Quiet NaN */
#define SPR_UR_FZ               0x00100000                      /* Zero */
#define SPR_UR_FIX              0x00200000                      /* Inexact */
#define SPR_UR_FIV              0x00400000                      /* Invalid */
#define SPR_UR_FIN              0x00800000                      /* Infinity */
#define SPR_UR_FDZ              0x01000000                      /* Division by Zero */
#define SPR_UR_FRM              0x06000000                      /* Rounding Mode */
#define SPR_UR_DPFW             0x80000000                      /* Data Page Fault Write */

/* Rounding mode select */
#define SPR_UR_FRM_RNEAR        0
#define SPR_UR_FRM_RZERO        1
#define SPR_UR_FRM_RPINF        2
#define SPR_UR_FRM_RNINF        3

/*
 * Range invalidate max register
 */
#define SPR_RIR_MAX_IH          0x01                            /* "if hit" is used */
#define SPR_RIR_MAX_IC          0x02                            /* invalidate instruction cache */
#define SPR_RIR_MAX_DC          0x04                            /* invalidate data cache */
#define SPR_RIR_MAX_ITLB        0x0A                            /* invalidate ITLB */
#define SPR_RIR_MAX_DTLB        0x0C                            /* invalidate DTLB */
#define SPR_RIR_MAX_ADDR	0xfffffff0                      /* RIR_MAX address mask */

/*
 * Bit definitions for the Data TLB Match Register v3
 */
#define SPR_DTLBMR2_VPN         0xffffe000                      /* Virtual Page Number */

/*
 * Bit definitions for the Data TLB Translate Register v3
 */
#define SPR_DTLBTR2_P           0x00000001                      /* Present */
#define SPR_DTLBTR2_CI          0x00000002                      /* Cache inhibit */
#define SPR_DTLBTR2_U           0x00000004                      /* Everything that the supervisor can do on the page, userspace can also do */
#define SPR_DTLBTR2_R           0x00000008                      /* Supervisor mode can read the page */
#define SPR_DTLBTR2_W           0x00000010                      /* Supervisor mode can write the page */
#define SPR_DTLBTR2_PPN         0xffffe000                      /* Physical Page Number */

/*
 * Bit definitions for the Instruction TLB Match Register v3
 */
#define SPR_ITLBMR2_VPN         0xffffe000                      /* Virtual Page Number */

/*
 * Bit definitions for the Instruction TLB Translate Register v3
 */
#define SPR_ITLBTR2_P           0x00000001                      /* Present */
#define SPR_ITLBTR2_CI          0x00000002                      /* Cache inhibit */
#define SPR_ITLBTR2_U           0x00000004                      /* Everything that the supervisor can do on the page, userspace can also do */
#define SPR_ITLBTR2_E           0x00000020                      /* Supervisor mode can execute the page */
#define SPR_ITLBTR2_PPN         0xffffe000                      /* Physical Page Number */

/*
 * Bit definitions for Debug Control Register
 */
#define SPR_DCFGR_NDP		0x0000000f			/* DVR/DCR pairs */
#define SPR_DCFGR_WPCI		0x00000010			/* performance counters */

/*
 * Bit definitions for Trace Buffer
 */
#define SPR_DCFGR_TB            0x00000020                      /* trace buffer implemented */
#define SPR_DCFGR_TB_SIZE       0x000003c0                      /* log2 trace buffer depth */

/*
 * Bit definitions for the CPU Configuration Register
 */
/* following bits are valid for BA22 5.1+ */
#define SPR_CPUCFGR_BA32S       0x00000020                      /* Basic 32 bit instruction set supported */
#define SPR_CPUCFGR_LE          0x00000400                      /* CPU is little endian */
#define SPR_CPUCFGR_PCGPR       0x00000800                      /* PC is mapped to GPR (31) */

/*
 * Bit definitions for Data Cache Configuration Register
 */
#define SPR_DCCFGR_NCW          0x0000000F                      /* number of ways */
#define SPR_DCCFGR_NCS          0x000000F0                      /* log2 of number of sets */
#define SPR_DCCFGR_CBS          0x00000F00                      /* lof2 of block size (in words) */
#define SPR_DCCFGR_CWS          0x00001000                      /* write strategy */
#define SPR_DCCFGR_CBIRI        0x00002000                      /* cache invalidate block */
#define SPR_DCCFGR_CBPRI        0x00004000                      /* cache prefetch Register */
#define SPR_DCCFGR_CBLRI        0x00008000                      /* cache lock Register */
#define SPR_DCCFGR_CBFRI        0x00010000                      /* cache flush Register */
#define SPR_DCCFGR_CBWBRI       0x00020000                      /* cache write back Register */
#define SPR_DCCFGR_RIRI         0x00040000                      /* cache range invalidation registers */

/*
 * Bit definitions for Instruction Cache Configuration Register
 */
#define SPR_ICCFGR_NCW          0x0000000F                      /* number of ways */
#define SPR_ICCFGR_NCS          0x000000F0                      /* log2 of number of sets */
#define SPR_ICCFGR_CBS          0x00000F00                      /* log2 of block size (in words) */
#define SPR_ICCFGR_CBIRI        0x00002000                      /* cache invalidate Register */
#define SPR_ICCFGR_CBPRI        0x00004000                      /* cache prefetch Register */
#define SPR_ICCFGR_CBLRI        0x00008000                      /* cache lock Register */
#define SPR_ICCFGR_RIRI         0x00040000                      /* cache range invalidation registers */

/*
 * Bit definitions for Data MMU Configuration Register
 */
#define SPR_DMMUCFGR_NTW        0x0000000F                      /* number of ways */
#define SPR_DMMUCFGR_NTS        0x000000F0                      /* log2 of number of sets */
#define SPR_DMMUCFGR_CRI        0x00000100                      /* control Register implemented */
#define SPR_DMMUCFGR_PRI        0x00000200                      /* protection Register */
#define SPR_DMMUCFGR_TEIRI      0x00000400                      /* TLB invalidate Register */
#define SPR_DMMUCFGR_HTR        0x00000800                      /* HW TLB reload */
#define SPR_DMMUCFGR_RIRI       0x00001000                      /* TLB range invalidation registers */
#define SPR_DMMUCFGR_NTS_SHIFT  0x4                             /* Shift 4 bits */

/*
 * Bit definitions for Instruction MMU Configuration Register
 */
#define SPR_IMMUCFGR_NTW        0x0000000F                      /* number of ways */
#define SPR_IMMUCFGR_NTS        0x000000F0                      /* log2 of number of sets */
#define SPR_IMMUCFGR_CRI        0x00000100                      /* control Register implemented */
#define SPR_IMMUCFGR_PRI        0x00000200                      /* protection Register */
#define SPR_IMMUCFGR_TEIRI      0x00000400                      /* TLB invalidate Register */
#define SPR_IMMUCFGR_HTR        0x00000800                      /* HW TLB reload */
#define SPR_IMMUCFGR_RIRI       0x00001000                      /* TLB range invalidation registers */
#define SPR_IMMUCFGR_NTS_SHIFT  0x4                             /* Shift 4 bits */

/*
 * Bit definitions for Debug Control Registers
 */
#define SPR_DCR_DP              0x00000001                      /* DVR/DCR present */
#define SPR_DCR_CC              0x0000000e                      /* Compare condition */
#define SPR_DCR_SC              0x00000010                      /* Signed compare */
#define SPR_DCR_CT              0x000000e0                      /* Compare to */
#define SPR_DCR_RBITS		0xffffff00			/* reserved bits */

/* Bit results with SPR_DCR_CC mask */
#define SPR_DCR_CC_MASKED       0x00000000                      /* masked */
#define SPR_DCR_CC_EQUAL        0x00000002                      /* equal */
#define SPR_DCR_CC_LESS         0x00000004                      /* less than */
#define SPR_DCR_CC_LESSE        0x00000006                      /* less than or equal */
#define SPR_DCR_CC_GREAT        0x00000008                      /* greater than */
#define SPR_DCR_CC_GREATE       0x0000000a                      /* greater than or equal */
#define SPR_DCR_CC_NEQUAL       0x0000000c                      /* reserved */

/* Bit results with SPR_DCR_CT mask */
#define SPR_DCR_CT_DISABLED     0x00000000                      /* comparison disabled */
#define SPR_DCR_CT_IFEA         0x00000020                      /* instruction fetch EA */
#define SPR_DCR_CT_LEA          0x00000040                      /* load EA */
#define SPR_DCR_CT_SEA          0x00000060                      /* store EA */
#define SPR_DCR_CT_LD           0x00000080                      /* load data */
#define SPR_DCR_CT_SD           0x000000a0                      /* store data */
#define SPR_DCR_CT_LSEA         0x000000c0                      /* load/store EA */
#define SPR_DCR_CT_LSD          0x000000e0                      /* load/store data */

/*
 * Bit definitions for Debug Mode 1 Register
 */
#define SPR_DMR1_CW0            0x00000003                      /* Chain watchpoint 0 */
#define SPR_DMR1_CW1            0x0000000c                      /* Chain watchpoint 1 */
#define SPR_DMR1_CW2            0x00000030                      /* Chain watchpoint 2 */
#define SPR_DMR1_CW3            0x000000c0                      /* Chain watchpoint 3 */
#define SPR_DMR1_CW4            0x00000300                      /* Chain watchpoint 4 */
#define SPR_DMR1_CW5            0x00000c00                      /* Chain watchpoint 5 */
#define SPR_DMR1_CW6            0x00003000                      /* Chain watchpoint 6 */
#define SPR_DMR1_CW7            0x0000c000                      /* Chain watchpoint 7 */
#define SPR_DMR1_CW8            0x00030000                      /* Chain watchpoint 8 */
#define SPR_DMR1_CW9            0x000c0000                      /* Chain watchpoint 9 */
#define SPR_DMR1_CW10           0x00300000                      /* Chain watchpoint 10 */
#define SPR_DMR1_ST             0x00400000                      /* Single-step trace*/

#define SPR_DMR1_CHAIN_01 0x4

/*
 * Bit definitions for Debug Mode 2 Register
 */
#define SPR_DMR2_WGB            0x003ff000                      /* Watchpoints generating breakpoint */
#define SPR_DMR2_WBS            0xffc00000                      /* Watchpoints generated breakpoint */
#define SPR_DMR2_RBITS		0x00000fff			/* reserved bits */


/*
 * Bit definitions for Debug stop Register
 */
#define SPR_DSR_RSTE            0x00000001                      /* Reset exception */
#define SPR_DSR_BUSEE           0x00000002                      /* Bus error exception */
#define SPR_DSR_DPFE            0x00000004                      /* Data Page Fault exception */
#define SPR_DSR_IPFE            0x00000008                      /* Insn Page Fault exception */
#define SPR_DSR_TTE             0x00000010                      /* Tick Timer exception */
#define SPR_DSR_AE              0x00000020                      /* Alignment exception */
#define SPR_DSR_IIE             0x00000040                      /* Illegal Instruction exception */
#define SPR_DSR_IE              0x00000080                      /* Interrupt exception */
#define SPR_DSR_DME             0x00000100                      /* DTLB miss exception */
#define SPR_DSR_IME             0x00000200                      /* ITLB miss exception */
#define SPR_DSR_RE              0x00000400                      /* Range exception */
#define SPR_DSR_SCE             0x00000800                      /* System call exception */
#define SPR_DSR_FPE             0x00001000                      /* Floating Point Exception */
#define SPR_DSR_TE              0x00002000                      /* Trap exception */
#define SPR_DSR_RBITS		0xffffc000			/* reserved bits */

/*
 * Bit definitions for Debug reason Register
 */
#define SPR_DRR_RSTE            0x00000001                      /* Reset exception */
#define SPR_DRR_BUSEE           0x00000002                      /* Bus error exception */
#define SPR_DRR_DPFE            0x00000004                      /* Data Page Fault exception */
#define SPR_DRR_IPFE            0x00000008                      /* Insn Page Fault exception */
#define SPR_DRR_TTE             0x00000010                      /* Tick Timer exception */
#define SPR_DRR_AE              0x00000020                      /* Alignment exception */
#define SPR_DRR_IIE             0x00000040                      /* Illegal Instruction exception */
#define SPR_DRR_IE              0x00000080                      /* Interrupt exception */
#define SPR_DRR_DME             0x00000100                      /* DTLB miss exception */
#define SPR_DRR_IME             0x00000200                      /* ITLB miss exception */
#define SPR_DRR_RE              0x00000400                      /* Range exception */
#define SPR_DRR_SCE             0x00000800                      /* System call exception */
#define SPR_DRR_FPE             0x00001000                      /* Floating Point Exception */
#define SPR_DRR_TE              0x00002000                      /* Trap exception */
#define SPR_DRR_RBITS		0xffffc000			/* reserved bits */

/*
 * Bit definitions for Debug Traceport Register
 */
#define SPR_DTR_EN              0x00000001                      /* Trace port enabled */
#define SPR_DTR_OV              0x00000002                      /* Trace port overflow */
#define SPR_DTR_SYN             0x00000004                      /* Synchronization requested */

/* 
 * Bit definitions for the Power management PMEVNT register
 */
#define SPR_PM_EVENT_TT         0x2                             /* Tick timer interrupt triggers PM events */
#define SPR_PM_EVENT_PIC        0x1                             /* PIC interrupt trigger PM events */

/*
 * Bit definitions for the Power management PMUNITPD register
 */
#define SPR_PM_DU_CLK_EN        0x1                             /* Debug unit clock enable output signal */

/*
 * Bit definitions for PICMR
 */
#define SPR_PICMR_IUM           0xffffffff                      /* Interrupt unmask */
#define SPR_VICMR_IUM           0xffffffff                      /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 */
#define SPR_PICPR_IPRIO		0xfffffffc			/* Interrupt priority */

/*
 * Bit definitions for PICSR
 */
#define SPR_PICSR_IS            0xffffffff                      /* Interrupt status */

/*
 * Bit definitions for Tick Timer Count Register
 */
#define SPR_TTCR_PERIOD	        0x0fffffff                      /* Part of TTCR used when matching TTMR period */
#define SPR_TTCR_FULL_PERIOD    0xffffffff                      /* Bits in TTCR used to count */

/*
 * Bit definitions for Tick Timer Mode Register
 */
#define SPR_TTMR_PERIOD         0x0fffffff                      /* Timer period */
#define SPR_TTMR_IP             0x10000000                      /* Interrupt Pending */
#define SPR_TTMR_IE             0x20000000                      /* Interrupt Enable */
#define SPR_TTMR_RT             0x40000000                      /* Restart mode */
#define SPR_TTMR_SR             0x80000000                      /* Single run mode */
#define SPR_TTMR_CR             0xc0000000                      /* Continuous run mode */
#define SPR_TTMR_M              0xc0000000                      /* Timer mode mask */

/**
 * Bit definitions for M.I.D. Memory access command register
 */
#define SPR_DCMD_AUTOCLEAR              0x80000000              /* auto clear: When set to 1, the status bits in this register are cleared when reading it. */
#define SPR_DCMD_RESERVED               0x7fe0fff4              /* reserved: Only zeroes should be written here. */
#define SPR_DCMD_ALIGNMENT              0x00100000              /* The given address was not aligned on a natural boundary when the access occurred. */
#define SPR_DCMD_CACHE_MISS             0x00080000              /* The given access missed in the cache. */
#define SPR_DCMD_PAGE_FAULT             0x00040000              /* The given access caused a page fault to occur. */
#define SPR_DCMD_TLB_MISS               0x00020000              /* A TLB miss occurred. */
#define SPR_DCMD_BUS_ERROR              0x00010000              /* The given access caused bus error. */
#define SPR_DCMD_AUTOINCREMENT          0x00000008              /* If this bit is set to 1, the address is incremented by the access size finished access. */
#define SPR_DCMD_ACCES_SIZE_MASK        0x00000003              /* Gives the size of the accesses: */
#define SPR_DCMD_ACCES_SIZE_BYTE        0                       /* Byte access */
#define SPR_DCMD_ACCES_SIZE_HALFWORD    1                       /* Half-word (16-bit) access */
#define SPR_DCMD_ACCES_SIZE_WORD        2                       /* Word (32-bit) access */

/**
 * Bit definitions for MPU registers
 */
#define SPR_MPGCR_ENABLE        0x00000001                      /* enable MPU */
#define SPR_MPGCR_UR            0x00000002                      /* allow user read */
#define SPR_MPGCR_UW            0x00000004                      /* allow user write */
#define SPR_MPGCR_SR            0x00000008                      /* allow supervisor read */
#define SPR_MPGCR_SW            0x00000010                      /* allow supervisor write */
#define SPR_MPGCR_UX            0x00000020                      /* allow user execute */
#define SPR_MPGCR_SX            0x00000040                      /* allow supervisor execute */

#define SPR_MPCR_UR             SPR_MPGCR_UR                    /* allow user read */
#define SPR_MPCR_UW             SPR_MPGCR_UW                    /* allow user write */
#define SPR_MPCR_SR             SPR_MPGCR_SR                    /* allow supervisor read */
#define SPR_MPCR_SW             SPR_MPGCR_SW                    /* allow supervisor write */
#define SPR_MPCR_UX             SPR_MPGCR_UX                    /* allow user execute */
#define SPR_MPCR_SX             SPR_MPGCR_SX                    /* allow supervisor execute */

/*
 * b.nop constants (simulators only)
 */
#define NOP_NOP                 0x0000                          /* Normal nop instruction */
#define NOP_EXIT                0x0001                          /* End of simulation */
#define NOP_REPORT              0x0002                          /* Simple report */
#define NOP_PRINTF              0x0003                          /* Simprintf instruction */
#define NOP_CNT_RESET           0x0005                          /* Reset statistics counters */
#define NOP_SIM_SYS_CALL        0x0006                          /* Simulator syscall emulation */
#define NOP_REPORT_FIRST        0x0400                          /* Report with number */
#define NOP_REPORT_LAST         0x03ff                          /* Report with number */

#endif
