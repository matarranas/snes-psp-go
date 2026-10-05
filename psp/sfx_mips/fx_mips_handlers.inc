/*
 * fx_mips_handlers.inc - Cleaned & Hand-tuned Branch/Shift Handlers
 */

/* 0x03: LSR */
.Lop_lsr:
    lw      $t0, 0($s4)          /* Load SREG */
    andi    $t1, $t0, 1          /* Extract carry bit */
    sw      $t1, GSU_VCARRY_OFF($s0)
    srl     $t2, $t0, 1
    andi    $t2, $t2, 0x7FFF
    sw      $t2, 0($s5)          /* DREG */
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF     /* CLRFLAGS */
    addu    $s4, $s0, $zero      /* reset pvSreg to R0 */
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero      /* reset pvDreg to R0 */

/* 0x04: ROL */
.Lop_rol:
    lw      $t0, 0($s4)          /* SREG */
    lw      $t1, GSU_VCARRY_OFF($s0)
    andi    $t1, $t1, 1
    sll     $t2, $t0, 1
    or      $t2, $t2, $t1
    srl     $t3, $t0, 15
    andi    $t3, $t3, 1
    sw      $t3, GSU_VCARRY_OFF($s0)
    andi    $t2, $t2, 0xFFFF
    sw      $t2, 0($s5)          /* DREG */
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF     /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* 0x05: BRA */
.Lop_bra:
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)          /* signed 8-bit offset */
    addiu   $s2, $s2, 1
    addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x08: BNE */
.Lop_bne:
    lw      $t4, GSU_VZERO_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 0xFFFF
    bnez    $t4, 1f
    addiu   $s2, $s2, 1          /* delay slot: always advance past offset byte */
    /* Not branched: R15++ an extra time as per SuperFX pipeline */
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x09: BEQ */
.Lop_beq:
    lw      $t4, GSU_VZERO_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 0xFFFF
    beqz    $t4, 1f
    addiu   $s2, $s2, 1
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x0A: BPL */
.Lop_bpl:
    lw      $t4, GSU_VSIGN_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 0x8000
    beqz    $t4, 1f
    addiu   $s2, $s2, 1
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x0B: BMI */
.Lop_bmi:
    lw      $t4, GSU_VSIGN_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 0x8000
    bnez    $t4, 1f
    addiu   $s2, $s2, 1
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x0C: BCC */
.Lop_bcc:
    lw      $t4, GSU_VCARRY_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 1
    beqz    $t4, 1f
    addiu   $s2, $s2, 1
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x0D: BCS */
.Lop_bcs:
    lw      $t4, GSU_VCARRY_OFF($s0)
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lb      $t0, 0($t2)
    andi    $t4, $t4, 1
    bnez    $t4, 1f
    addiu   $s2, $s2, 1
    addiu   $s2, $s2, 1
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0
    j       .Lfx_dispatch_loop
    nop

/* 0x3C: LOOP */
.Lop_loop:
    lw      $t0, GSU_R12_OFF($s0)
    addiu   $t0, $t0, -1
    andi    $t0, $t0, 0xFFFF
    sw      $t0, GSU_R12_OFF($s0)
    sw      $t0, GSU_VSIGN_OFF($s0)
    sw      $t0, GSU_VZERO_OFF($s0)
    bnez    $t0, 1f
    nop
    addiu   $s2, $s2, 1
    j       2f
    nop
1:  lw      $s2, GSU_R13_OFF($s0)
2:  andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* 0x3D: ALT1 */
.Lop_alt1:
    ori     $s6, $s6, 0x0100
    andi    $s6, $s6, 0xEFFF
    j       .Lfx_dispatch_loop
    nop

/* 0x3E: ALT2 */
.Lop_alt2:
    ori     $s6, $s6, 0x0200
    andi    $s6, $s6, 0xEFFF
    j       .Lfx_dispatch_loop
    nop

/* 0x3F: ALT3 */
.Lop_alt3:
    ori     $s6, $s6, 0x0300
    andi    $s6, $s6, 0xEFFF
    j       .Lfx_dispatch_loop
    nop

/* 0x96: ASR */
.Lop_asr:
    lw      $t0, 0($s4)
    andi    $t1, $t0, 1
    sw      $t1, GSU_VCARRY_OFF($s0)
    seh     $t2, $t0
    sra     $t2, $t2, 1
    andi    $t2, $t2, 0xFFFF
    sw      $t2, 0($s5)
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* 0x97: ROR */
.Lop_ror:
    lw      $t0, 0($s4)
    lw      $t1, GSU_VCARRY_OFF($s0)
    andi    $t2, $t0, 1
    andi    $t3, $t0, 0xFFFF
    srl     $t3, $t3, 1
    sll     $t4, $t1, 15
    or      $t3, $t3, $t4
    andi    $t3, $t3, 0xFFFF
    sw      $t2, GSU_VCARRY_OFF($s0)
    sw      $t3, 0($s5)
    sw      $t3, GSU_VSIGN_OFF($s0)
    sw      $t3, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* 0x9E: LOB */
.Lop_lob:
    lw      $t0, 0($s4)
    andi    $t1, $t0, 0x00FF
    sw      $t1, 0($s5)
    sll     $t2, $t1, 8
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* 0xC0: HIB */
.Lop_hib:
    lw      $t0, 0($s4)
    srl     $t1, $t0, 8
    andi    $t1, $t1, 0x00FF
    sw      $t1, 0($s5)
    sll     $t2, $t1, 8
    sw      $t2, GSU_VSIGN_OFF($s0)
    sw      $t2, GSU_VZERO_OFF($s0)
    andi    $s6, $s6, 0xEFFF
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
