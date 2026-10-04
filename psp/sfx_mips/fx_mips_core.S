/*
 * fx_mips_core.S - Monolithic MIPS Allegrex Super FX 2 (GSU-2) Core for PSP
 * Part of the Snes9xTYL MIPS JIT / Fast Core Recompiler Project
 */

#include "sfx_mips_asm.h"

.set noreorder
.text
.align 4

.globl mips_sfx_run
.ent mips_sfx_run
mips_sfx_run:
    /* Prologue: Allocate stack frame and save callee-saved registers ($s0-$s7, $fp, $ra) */
    addiu   $sp, $sp, -48
    sw      $ra, 44($sp)
    sw      $fp, 40($sp)
    sw      $s7, 36($sp)
    sw      $s6, 32($sp)
    sw      $s5, 28($sp)
    sw      $s4, 24($sp)
    sw      $s3, 20($sp)
    sw      $s2, 16($sp)
    sw      $s1, 12($sp)
    sw      $s0, 8($sp)

    /* Move GSU struct pointer to $s0 (Callee-saved context pointer) */
    la      $s0, GSU
    move    $s1, $a0                /* $s1 = nInstructions cycle counter */

    /* Load frequently used GSU registers into dedicated MIPS registers */
    lw      $s2, GSU_R15_OFF($s0)   /* $s2 = R15 (Program Counter) */
    lw      $s3, GSU_PBR_PTR($s0)   /* $s3 = pvPrgBank pointer */
    lw      $s4, GSU_SREG_OFF($s0)  /* $s4 = pvSreg pointer */
    lw      $s5, GSU_DREG_OFF($s0)  /* $s5 = pvDreg pointer */
    lw      $s6, GSU_STATUS_OFF($s0)/* $s6 = vStatusReg */

.Lfx_dispatch_loop:
    /* Check if G flag is set in vStatusReg (bit 5) and instructions counter > 0 */
    andi    $t0, $s6, 0x0020
    beqz    $t0, .Lfx_exit_loop
    subiu   $s1, $s1, 1             /* Decrement instruction counter in delay slot */

    bltz    $s1, .Lfx_exit_loop
    nop

    /* Read byte from ROM/Cache: PIPE = PRGBANK(R15) */
    andi    $t1, $s2, 0xFFFF        /* Mask R15 to 16 bits */
    addu    $t2, $s3, $t1           /* $t2 = pvPrgBank + (R15 & 0xFFFF) */
    lbu     $t3, 0($t2)             /* $t3 = Opcode */

    /* Increment R15 (PC) */
    addiu   $s2, $s2, 1

    /* Calculate dispatch address in 1024-entry jump table:
     * Index = (vStatusReg & 0x300) | opcode
     */
    andi    $t4, $s6, 0x0300
    or      $t4, $t4, $t3           /* Full opcode index (0-1023) */
    sll     $t4, $t4, 2             /* Index * 4 */
    la      $t5, fx_mips_jump_table
    addu    $t5, $t5, $t4
    lw      $t6, 0($t5)             /* Target handler address */
    jr      $t6
    nop                             /* Branch delay slot */

/* =========================================================================
 * Opcode Handlers (Base Group)
 * ========================================================================= */

.Lop_nop:
    /* NOP: CLRFLAGS; R15 already incremented */
    andi    $s6, $s6, 0xEFFF        /* Clear ALT1, ALT2, B flags */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_stop:
    /* STOP: Clear G flag, set counter = 0 */
    andi    $s6, $s6, 0xFFDF        /* Clear G flag */
    j       .Lfx_exit_loop
    move    $s1, $zero

.Lop_swap:
    /* SWAP: Swap bytes of SREG */
    lw      $t0, 0($s4)             /* Load SREG value */
    andi    $t1, $t0, 0x00FF        /* Lower byte */
    srl     $t2, $t0, 8             /* Upper byte */
    sll     $t1, $t1, 8
    or      $t1, $t1, $t2           /* Swapped 16-bit word */
    sw      $t1, 0($s5)             /* Store into DREG */
    sw      $t1, GSU_VSIGN_OFF($s0) /* vSign = result */
    sw      $t1, GSU_VZERO_OFF($s0) /* vZero = result */
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_not:
    /* NOT: Bitwise NOT of SREG */
    lw      $t0, 0($s4)             /* Load SREG */
    not     $t1, $t0
    andi    $t1, $t1, 0xFFFF
    sw      $t1, 0($s5)             /* Store DREG */
    sw      $t1, GSU_VSIGN_OFF($s0)
    sw      $t1, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_sex:
    /* SEX: Sign extend 8-bit to 16-bit */
    lw      $t0, 0($s4)
    seb     $t1, $t0                /* Sign extend byte */
    andi    $t1, $t1, 0xFFFF
    sw      $t1, 0($s5)             /* Store DREG */
    sw      $t1, GSU_VSIGN_OFF($s0)
    sw      $t1, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_fmult:
    /* FMULT: 16-bit signed multiplication using MIPS hardware MULT */
    lw      $t0, 0($s4)             /* SREG */
    lw      $t1, GSU_R6_OFF($s0)    /* R6 */
    seh     $t0, $t0                /* Sign-extend halfword */
    seh     $t1, $t1
    mult    $t0, $t1                /* Hardware multiply in 1 cycle */
    mfhi    $t2                     /* Upper 16 bits */
    mflo    $t3                     /* Lower 16 bits */
    andi    $t4, $t2, 0xFFFF
    sw      $t4, 0($s5)             /* DREG = upper word */
    sw      $t4, GSU_VSIGN_OFF($s0)
    sw      $t4, GSU_VZERO_OFF($s0)
    srl     $t5, $t3, 15            /* Carry = bit 15 of lower result */
    andi    $t5, $t5, 1
    sw      $t5, GSU_VCARRY_OFF($s0)
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* Include Monolithic Handlers (Branches, Shifts, Modes, Arithmetic, Transfers) */
#include "fx_mips_handlers.inc"
#include "fx_mips_arith.inc"
#include "fx_mips_transfers.inc"

.Lop_default_fallback:
    /* Default fallback: loop continuation for unimplemented opcodes */
    j       .Lfx_dispatch_loop
    nop

.Lfx_exit_loop:
    /* Epilogue: Save updated registers back to struct FxRegs_s GSU */
    sw      $s2, GSU_R15_OFF($s0)   /* Save R15 (PC) */
    sw      $s6, GSU_STATUS_OFF($s0)/* Save vStatusReg */
    sw      $s1, GSU_VCOUNT_OFF($s0)/* Save remaining cycle count */
    sw      $s4, GSU_SREG_OFF($s0)  /* Save pvSreg */
    sw      $s5, GSU_DREG_OFF($s0)  /* Save pvDreg */

    /* Restore callee-saved registers */
    lw      $s0, 8($sp)
    lw      $s1, 12($sp)
    lw      $s2, 16($sp)
    lw      $s3, 20($sp)
    lw      $s4, 24($sp)
    lw      $s5, 28($sp)
    lw      $s6, 32($sp)
    lw      $s7, 36($sp)
    lw      $fp, 40($sp)
    lw      $ra, 44($sp)
    addiu   $sp, $sp, 48

    jr      $ra
    nop
.end mips_sfx_run

/* Include the 1024-entry Jump Table */
#include "fx_mips_table.inc"
