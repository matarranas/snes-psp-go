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

/* =========================================================================
 * JMP Rn (0x98 - 0x9D, ALT0): R15 = GSU.avReg[n]; CLRFLAGS;
 * No R15++, no FETCHPIPE here - dispatch loop handles FETCHPIPE at top.
 * ========================================================================= */
.Lop_jmp:
    andi    $t0, $t3, 0x0F              /* n = opcode & 0x0F */
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0               /* &GSU.avReg[n] */
    lw      $s2, 0($t0)                 /* R15 = GSU.avReg[n] */
    andi    $s2, $s2, 0xFFFF

    andi    $s6, $s6, 0xEFFF            /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * LJMP Rn (0x98 - 0x9D, ALT1):
 * GSU.vPrgBankReg = GSU.avReg[reg] & 0x7f;
 * GSU.pvPrgBank = GSU.apvRomBank[GSU.vPrgBankReg];
 * R15 = SREG;
 * GSU.bCacheActive = FALSE; fx_cache(); R15--;
 * ========================================================================= */
.Lop_ljmp:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0               /* &GSU.avReg[n] */
    lw      $t1, 0($t0)
    andi    $t1, $t1, 0x7F              /* vPrgBankReg = Rn & 0x7f */
    sw      $t1, GSU_PRGBANK_OFF($s0)

    /* pvPrgBank = apvRomBank[vPrgBankReg] */
    sll     $t2, $t1, 2
    addiu   $t3, $s0, GSU_APVROMBANK_OFF
    addu    $t3, $t3, $t2
    lw      $s3, 0($t3)                 /* $s3 = pvPrgBank */
    sw      $s3, GSU_PBR_PTR($s0)

    /* R15 = SREG */
    lw      $s2, 0($s4)
    andi    $s2, $s2, 0xFFFF

    /* Invalidate cache: bCacheActive = FALSE; then fx_cache() logic; then R15-- */
    sb      $zero, GSU_BCACHEACTIVE_OFF($s0)   /* bCacheActive = FALSE */

    /* fx_cache() inline: if (vCacheBaseReg != c || !bCacheActive) { invalidate } */
    andi    $t4, $s2, 0xFFF0            /* c = R15 & 0xFFF0 */
    lw      $t5, GSU_CACHEBASE_OFF($s0)
    lbu     $t6, GSU_BCACHEACTIVE_OFF($s0)
    bne     $t4, $t5, 1f                /* if base changed */
    beqz    $t6, 1f                     /* or cache was inactive */
    j       2f                          /* else cache valid, skip invalidate */
    nop
1:
    sw      $zero, GSU_CACHEFLAG_OFF($s0)    /* vCacheFlags = 0 */
    sw      $t4, GSU_CACHEBASE_OFF($s0)      /* vCacheBaseReg = c */
    li      $t7, 1
    sb      $t7, GSU_BCACHEACTIVE_OFF($s0)   /* bCacheActive = TRUE */
2:

    /* fx_cache does R15++; compensate with R15-- */
    addiu   $s2, $s2, -1                /* R15-- */

    /* CLRFLAGS */
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
