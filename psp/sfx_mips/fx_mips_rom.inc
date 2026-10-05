/*
 * fx_mips_rom.inc - Hand-tuned Super FX ROM Access Handlers
 * GETC, GETB, GETBL, GETBH, GETBS
 */

/* =========================================================================
 * GETC (0xDF, ALT0/ALT1): Transfer ROM buffer to Color Register
 * ========================================================================= */
.Lop_getc:
    lbu     $t0, GSU_ROMBUF_OFF($s0)    /* c = GSU.vRomBuffer */
    lw      $t1, GSU_PLOTOPT_OFF($s0)   /* vPlotOptionReg */

    /* If (vPlotOptionReg & 0x04) c = (c & 0xF0) | (c >> 4) */
    andi    $t2, $t1, 0x04
    beqz    $t2, 1f
    nop
    andi    $t3, $t0, 0xF0
    srl     $t4, $t0, 4
    or      $t0, $t3, $t4

1:
    /* If (vPlotOptionReg & 0x08) vColorReg = (vColorReg & 0xF0) | (c & 0x0F) */
    andi    $t2, $t1, 0x08
    beqz    $t2, 2f
    nop
    lw      $t5, GSU_COLOR_OFF($s0)
    andi    $t5, $t5, 0xF0
    andi    $t0, $t0, 0x0F
    or      $t0, $t5, $t0
    sw      $t0, GSU_COLOR_OFF($s0)
    b       3f
    nop

2:
    /* Else: vColorReg = USEX8(c) */
    andi    $t0, $t0, 0x00FF
    sw      $t0, GSU_COLOR_OFF($s0)

3:
    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * GETB (0xEF, ALT0): DREG = ROM(R14) byte
 * ========================================================================= */
.Lop_getb:
    lbu     $t0, GSU_ROMBUF_OFF($s0)
    sw      $t0, 0($s5)                 /* *DREG = v */

    /* TESTR14 */
    addiu   $t7, $s0, GSU_R14_OFF
    bne     $s5, $t7, 1f
    nop
    lw      $t8, GSU_R14_OFF($s0)
    andi    $t8, $t8, 0xFFFF
    lw      $t9, GSU_ROMBANK_PTR($s0)
    addu    $t9, $t9, $t8
    lbu     $t8, 0($t9)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * GETBH (0xEF, ALT1): DREG = (SREG & 0x00FF) | (c << 8)
 * ========================================================================= */
.Lop_getbh:
    lbu     $t0, GSU_ROMBUF_OFF($s0)    /* c */
    sll     $t0, $t0, 8
    lw      $t1, 0($s4)                 /* SREG */
    andi    $t1, $t1, 0x00FF
    or      $t0, $t1, $t0
    sw      $t0, 0($s5)                 /* *DREG = v */

    /* TESTR14 */
    addiu   $t7, $s0, GSU_R14_OFF
    bne     $s5, $t7, 1f
    nop
    lw      $t8, GSU_R14_OFF($s0)
    andi    $t8, $t8, 0xFFFF
    lw      $t9, GSU_ROMBANK_PTR($s0)
    addu    $t9, $t9, $t8
    lbu     $t8, 0($t9)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * GETBL (0xEF, ALT2): DREG = (SREG & 0xFF00) | c
 * ========================================================================= */
.Lop_getbl:
    lbu     $t0, GSU_ROMBUF_OFF($s0)    /* c */
    lw      $t1, 0($s4)                 /* SREG */
    andi    $t1, $t1, 0xFF00
    or      $t0, $t1, $t0
    sw      $t0, 0($s5)                 /* *DREG = v */

    /* TESTR14 */
    addiu   $t7, $s0, GSU_R14_OFF
    bne     $s5, $t7, 1f
    nop
    lw      $t8, GSU_R14_OFF($s0)
    andi    $t8, $t8, 0xFFFF
    lw      $t9, GSU_ROMBANK_PTR($s0)
    addu    $t9, $t9, $t8
    lbu     $t8, 0($t9)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * GETBS (0xEF, ALT3): DREG = SEX8(c)
 * ========================================================================= */
.Lop_getbs:
    lb      $t0, GSU_ROMBUF_OFF($s0)    /* Sign-extended byte */
    andi    $t0, $t0, 0xFFFF
    sw      $t0, 0($s5)                 /* *DREG = v */

    /* TESTR14 */
    addiu   $t7, $s0, GSU_R14_OFF
    bne     $s5, $t7, 1f
    nop
    lw      $t8, GSU_R14_OFF($s0)
    andi    $t8, $t8, 0xFFFF
    lw      $t9, GSU_ROMBANK_PTR($s0)
    addu    $t9, $t9, $t8
    lbu     $t8, 0($t9)
    sb      $t8, GSU_ROMBUF_OFF($s0)

1:
    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
