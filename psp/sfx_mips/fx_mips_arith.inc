/*
 * fx_mips_arith.inc - Fast Super FX Arithmetic & Logic Handlers in MIPS Assembly
 * Hand-tuned for Allegrex pipeline with branch delay slots and exact flag semantics.
 */

/* =========================================================================
 * ADD Rn (0x50 - 0x5F, ALT0): SREG + Rn
 * ========================================================================= */
.Lop_add_rn:
    andi    $t0, $t3, 0x0F          /* n = opcode & 0x0F */
    sll     $t0, $t0, 2             /* n * 4 */
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* $t1 = Rn */
    lw      $t2, 0($s4)             /* $t2 = SREG */
    andi    $t1, $t1, 0xFFFF
    andi    $t2, $t2, 0xFFFF
    addu    $t3, $t2, $t1           /* s = SREG + Rn */

    /* Carry: s >= 0x10000 */
    srl     $t4, $t3, 16
    andi    $t4, $t4, 1
    sw      $t4, GSU_VCARRY_OFF($s0)

    /* Overflow: ~(SREG ^ Rn) & (Rn ^ s) & 0x8000 */
    xor     $t5, $t2, $t1
    not     $t5, $t5
    xor     $t6, $t1, $t3
    and     $t5, $t5, $t6
    andi    $t5, $t5, 0x8000
    sw      $t5, GSU_VOVERFLOW_OFF($s0)

    /* Mask result to 16-bit and store */
    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)             /* *DREG = s */
    sw      $t3, GSU_VSIGN_OFF($s0) /* vSign = s */
    sw      $t3, GSU_VZERO_OFF($s0) /* vZero = s */

    /* CLRFLAGS: Clear ALT1, ALT2, B; reset SREG/DREG to R0 */
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero         /* pvSreg = &R0 */
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero         /* pvDreg = &R0 (delay slot) */

/* =========================================================================
 * ADC Rn (0x50 - 0x5F, ALT1): SREG + Rn + Carry
 * ========================================================================= */
.Lop_adc_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    lw      $t7, GSU_VCARRY_OFF($s0)/* vCarry */
    andi    $t1, $t1, 0xFFFF
    andi    $t2, $t2, 0xFFFF
    andi    $t7, $t7, 1
    addu    $t3, $t2, $t1
    addu    $t3, $t3, $t7           /* s = SREG + Rn + Carry */

    /* Carry: s >= 0x10000 */
    srl     $t4, $t3, 16
    andi    $t4, $t4, 1
    sw      $t4, GSU_VCARRY_OFF($s0)

    /* Overflow */
    xor     $t5, $t2, $t1
    not     $t5, $t5
    xor     $t6, $t1, $t3
    and     $t5, $t5, $t6
    andi    $t5, $t5, 0x8000
    sw      $t5, GSU_VOVERFLOW_OFF($s0)

    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)

    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * SUB Rn (0x60 - 0x6F, ALT0): SREG - Rn
 * ========================================================================= */
.Lop_sub_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    andi    $t1, $t1, 0xFFFF
    andi    $t2, $t2, 0xFFFF
    subu    $t3, $t2, $t1           /* s = SREG - Rn */

    /* Carry: s >= 0 (Borrow clear) -> not (s < 0) */
    slt     $t4, $t3, $zero
    xori    $t4, $t4, 1
    sw      $t4, GSU_VCARRY_OFF($s0)

    /* Overflow: (SREG ^ Rn) & ~(Rn ^ s) & 0x8000 */
    xor     $t5, $t2, $t1
    xor     $t6, $t1, $t3
    not     $t6, $t6
    and     $t5, $t5, $t6
    andi    $t5, $t5, 0x8000
    sw      $t5, GSU_VOVERFLOW_OFF($s0)

    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)

    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * SBC Rn (0x60 - 0x6F, ALT1): SREG - Rn - (1 - Carry)
 * ========================================================================= */
.Lop_sbc_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    lw      $t7, GSU_VCARRY_OFF($s0)/* Carry */
    andi    $t1, $t1, 0xFFFF
    andi    $t2, $t2, 0xFFFF
    andi    $t7, $t7, 1
    xori    $t7, $t7, 1             /* Borrow = 1 - Carry */
    subu    $t3, $t2, $t1
    subu    $t3, $t3, $t7           /* s = SREG - Rn - Borrow */

    /* Carry: s >= 0 (Borrow clear) -> not (s < 0) */
    slt     $t4, $t3, $zero
    xori    $t4, $t4, 1
    sw      $t4, GSU_VCARRY_OFF($s0)

    xor     $t5, $t2, $t1
    xor     $t6, $t1, $t3
    not     $t6, $t6
    and     $t5, $t5, $t6
    andi    $t5, $t5, 0x8000
    sw      $t5, GSU_VOVERFLOW_OFF($s0)

    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)

    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * CMP Rn (0x60 - 0x6F, ALT3): Compare SREG with Rn (like SUB but no write to DREG)
 * ========================================================================= */
.Lop_cmp_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    andi    $t1, $t1, 0xFFFF
    andi    $t2, $t2, 0xFFFF
    subu    $t3, $t2, $t1

    /* Carry: s >= 0 (Borrow clear) -> not (s < 0) */
    slt     $t4, $t3, $zero
    xori    $t4, $t4, 1
    sw      $t4, GSU_VCARRY_OFF($s0)

    xor     $t5, $t2, $t1
    xor     $t6, $t1, $t3
    not     $t6, $t6
    and     $t5, $t5, $t6
    andi    $t5, $t5, 0x8000
    sw      $t5, GSU_VOVERFLOW_OFF($s0)

    andi    $t3, $t3, 0xFFFF
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)

    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * AND Rn (0x71 - 0x7F, ALT0): SREG & Rn
 * ========================================================================= */
.Lop_and_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    and     $t3, $t2, $t1
    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)             /* DREG */
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * BIC Rn (0x71 - 0x7F, ALT1): SREG & ~Rn
 * ========================================================================= */
.Lop_bic_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    not     $t1, $t1
    and     $t3, $t2, $t1
    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)             /* DREG */
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * OR Rn (0xC1 - 0xCF, ALT0): SREG | Rn
 * ========================================================================= */
.Lop_or_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    or      $t3, $t2, $t1
    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)             /* DREG */
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * XOR Rn (0xC1 - 0xCF, ALT1): SREG ^ Rn
 * ========================================================================= */
.Lop_xor_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)             /* Rn */
    lw      $t2, 0($s4)             /* SREG */
    xor     $t3, $t2, $t1
    andi    $t3, $t3, 0xFFFF
    sw      $t3, 0($s5)             /* DREG */
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * INC Rn (0xD0 - 0xDE): Rn = Rn + 1
 * ========================================================================= */
.Lop_inc_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)
    addiu   $t1, $t1, 1
    andi    $t1, $t1, 0xFFFF
    sw      $t1, 0($t0)             /* Rn = Rn + 1 */
    sw      $t1, GSU_VSIGN_OFF($s0)
    sw      $t1, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * DEC Rn (0xE0 - 0xEE): Rn = Rn - 1
 * ========================================================================= */
.Lop_dec_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0
    lw      $t1, 0($t0)
    addiu   $t1, $t1, -1
    andi    $t1, $t1, 0xFFFF
    sw      $t1, 0($t0)             /* Rn = Rn - 1 */
    sw      $t1, GSU_VSIGN_OFF($s0)
    sw      $t1, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
