/*
 * fx_mips_transfers.inc - Register Transfer Handlers (TO, WITH, FROM, IBT, IWT)
 * Hand-tuned for Allegrex with branch delay slot filling and exact register addressing.
 */

/* =========================================================================
 * TO Rn (0x10 - 0x1F): Set DREG to &Rn (or Move if B flag set)
 * ========================================================================= */
.Lop_to_rn:
    andi    $t0, $t3, 0x0F          /* n = opcode & 0x0F */
    sll     $t0, $t0, 2             /* n * 4 */
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */

    andi    $t1, $s6, 0x1000        /* Check B flag (bit 12) */
    beqz    $t1, .Lop_to_rn_no_b
    move    $s5, $t0                /* Delay slot: if not B, pvDreg = &Rn */

    /* B flag is SET: Move SREG into Rn */
    lw      $t2, 0($s4)             /* SREG */
    sw      $t2, 0($t0)             /* Rn = SREG */

    /* If Rn == R14 (n == 14), READR14: vRomBuffer = ROM(R14) */
    andi    $t1, $t3, 0x0F
    xori    $t1, $t1, 14
    bnez    $t1, 1f
    nop
    andi    $t2, $t2, 0xFFFF        /* R14 offset */
    lw      $t4, GSU_ROMBANK_PTR($s0)
    addu    $t4, $t4, $t2
    lbu     $t5, 0($t4)
    sb      $t5, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_to_rn_no_b:
    /* pvDreg already set to &Rn in branch delay slot */
    j       .Lfx_dispatch_loop
    nop

/* =========================================================================
 * WITH Rn (0x20 - 0x2F): Set B flag, pvSreg = pvDreg = &Rn
 * ========================================================================= */
.Lop_with_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    ori     $s6, $s6, 0x1000        /* Set B flag */
    move    $s4, $t0                /* pvSreg = &Rn */
    j       .Lfx_dispatch_loop
    move    $s5, $t0                /* pvDreg = &Rn (delay slot) */

/* =========================================================================
 * FROM Rn (0xB0 - 0xBF): Set SREG to &Rn (or Move Rn to DREG if B set)
 * ========================================================================= */
.Lop_from_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */

    andi    $t1, $s6, 0x1000        /* Check B flag */
    beqz    $t1, .Lop_from_rn_no_b
    move    $s4, $t0                /* Delay slot: if not B, pvSreg = &Rn */

    /* B flag is SET: Move Rn to DREG, set flags */
    lw      $t2, 0($t0)             /* v = Rn */
    sw      $t2, 0($s5)             /* *DREG = v */
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $t4, $t2, 0x80
    sll     $t4, $t4, 16
    sw      $t4, GSU_VOVERFLOW_OFF($s0)

    /* TESTR14: if pvDreg == &R14, READR14 */
    addiu   $t1, $s0, GSU_R14_OFF
    bne     $s5, $t1, 1f
    nop
    lw      $t2, GSU_R14_OFF($s0)
    andi    $t2, $t2, 0xFFFF
    lw      $t4, GSU_ROMBANK_PTR($s0)
    addu    $t4, $t4, $t2
    lbu     $t5, 0($t4)
    sb      $t5, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

.Lop_from_rn_no_b:
    j       .Lfx_dispatch_loop
    nop

/* =========================================================================
 * IBT Rn,#pp (0xA0 - 0xAF): Immediate Byte Transfer to Rn
 * uint8 v = PIPE; R15++; FETCHPIPE; R15++; Rn = SEX8(v); CLRFLAGS;
 * ========================================================================= */
.Lop_ibt_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */

    lbu     $t4, GSU_PIPE_OFF($s0)  /* v = PIPE */
    seb     $t4, $t4                /* SEX8(v) */
    andi    $t4, $t4, 0xFFFF
    sw      $t4, 0($t0)             /* Rn = SEX8(v) */

    addiu   $s2, $s2, 1             /* R15++ */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE */
    sb      $t7, GSU_PIPE_OFF($s0)
    addiu   $s2, $s2, 1             /* R15++ */

    /* If Rn == R14, READR14 */
    andi    $t1, $t3, 0x0F
    xori    $t1, $t1, 14
    bnez    $t1, 1f
    nop
    lw      $t6, GSU_ROMBANK_PTR($s0)
    addu    $t6, $t6, $t4
    lbu     $t8, 0($t6)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * IWT Rn,#xx (0xF0 - 0xFF): Immediate Word Transfer to Rn
 * uint32 v = PIPE; R15++; FETCHPIPE; R15++;
 * v |= USEX8(PIPE) << 8; FETCHPIPE; R15++;
 * Rn = v; CLRFLAGS;
 * ========================================================================= */
.Lop_iwt_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */

    /* Low byte from PIPE */
    lbu     $t4, GSU_PIPE_OFF($s0)  /* v = PIPE */
    addiu   $s2, $s2, 1             /* R15++ */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE */
    sb      $t7, GSU_PIPE_OFF($s0)
    addiu   $s2, $s2, 1             /* R15++ */

    /* High byte from PIPE */
    lbu     $t5, GSU_PIPE_OFF($s0)  /* high byte = PIPE */
    sll     $t5, $t5, 8
    or      $t4, $t4, $t5           /* v |= PIPE << 8 */
    andi    $t4, $t4, 0xFFFF
    sw      $t4, 0($t0)             /* Rn = v */

    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE */
    sb      $t7, GSU_PIPE_OFF($s0)
    addiu   $s2, $s2, 1             /* R15++ */

    /* If Rn == R14, READR14 */
    andi    $t1, $t3, 0x0F
    xori    $t1, $t1, 14
    bnez    $t1, 1f
    nop
    lw      $t6, GSU_ROMBANK_PTR($s0)
    addu    $t6, $t6, $t4
    lbu     $t8, 0($t6)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
