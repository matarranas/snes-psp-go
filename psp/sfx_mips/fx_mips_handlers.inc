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

/* 0x05: BRA - Branch Always */
.Lop_bra:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */
    addu    $s2, $s2, $t0           /* R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x06: BLT - Branch on Less Than ((TEST_S != 0) != (TEST_OV != 0)) */
.Lop_blt:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VSIGN_OFF($s0)
    lw      $t5, GSU_VOVERFLOW_OFF($s0)
    andi    $t4, $t4, 0x8000
    srl     $t4, $t4, 15            /* sign bit (0 or 1) */
    slti    $t6, $t5, 0x8000
    xori    $t6, $t6, 1             /* overflow high (0 or 1) */
    li      $t7, -0x8000
    slt     $t7, $t5, $t7           /* overflow low (0 or 1) */
    or      $t6, $t6, $t7           /* ov != 0 */
    xor     $t8, $t4, $t6           /* cond = (sign != 0) != (ov != 0) */
    bnez    $t8, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x07: BGE - Branch on Greater or Equal ((TEST_S != 0) == (TEST_OV != 0)) */
.Lop_bge:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VSIGN_OFF($s0)
    lw      $t5, GSU_VOVERFLOW_OFF($s0)
    andi    $t4, $t4, 0x8000
    srl     $t4, $t4, 15
    slti    $t6, $t5, 0x8000
    xori    $t6, $t6, 1
    li      $t7, -0x8000
    slt     $t7, $t5, $t7
    or      $t6, $t6, $t7
    xor     $t8, $t4, $t6
    beqz    $t8, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x08: BNE */
.Lop_bne:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VZERO_OFF($s0)
    andi    $t4, $t4, 0xFFFF
    bnez    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x09: BEQ */
.Lop_beq:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VZERO_OFF($s0)
    andi    $t4, $t4, 0xFFFF
    beqz    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0A: BPL */
.Lop_bpl:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VSIGN_OFF($s0)
    andi    $t4, $t4, 0x8000
    beqz    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0B: BMI */
.Lop_bmi:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VSIGN_OFF($s0)
    andi    $t4, $t4, 0x8000
    bnez    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0C: BCC */
.Lop_bcc:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VCARRY_OFF($s0)
    andi    $t4, $t4, 1
    beqz    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0D: BCS */
.Lop_bcs:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t4, GSU_VCARRY_OFF($s0)
    andi    $t4, $t4, 1
    bnez    $t4, 1f
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0E: BVC - Branch on Overflow Clear (!TEST_OV) */
.Lop_bvc:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t5, GSU_VOVERFLOW_OFF($s0)
    slti    $t6, $t5, 0x8000
    xori    $t6, $t6, 1             /* overflow high (0 or 1) */
    li      $t7, -0x8000
    slt     $t7, $t5, $t7           /* overflow low (0 or 1) */
    or      $t6, $t6, $t7           /* ov != 0 */
    beqz    $t6, 1f                 /* if ov == 0, branch taken! */
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
    j       .Lfx_dispatch_loop
    nop

/* 0x0F: BVS - Branch on Overflow Set (TEST_OV) */
.Lop_bvs:
    addiu   $s2, $s2, 1             /* R15++ (skip opcode, point to offset byte) */
    andi    $t1, $s2, 0xFFFF
    addu    $t2, $s3, $t1
    lbu     $t7, 0($t2)             /* FETCHPIPE: load offset byte */
    sb      $t7, GSU_PIPE_OFF($s0)
    lbu     $t0, GSU_PIPE_OFF($s0)  /* $t0 = offset from PIPE */
    seb     $t0, $t0                /* sign-extend 8-bit offset */

    lw      $t5, GSU_VOVERFLOW_OFF($s0)
    slti    $t6, $t5, 0x8000
    xori    $t6, $t6, 1
    li      $t7, -0x8000
    slt     $t7, $t5, $t7
    or      $t6, $t6, $t7
    bnez    $t6, 1f                 /* if ov != 0, branch taken! */
    nop
    addiu   $s2, $s2, 1             /* not taken: R15++ (skip offset byte) */
    j       .Lfx_dispatch_loop
    nop
1:  addu    $s2, $s2, $t0           /* taken: R15 += SEX8(v) */
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

/* 0x4E: COLOR (ALT0) - Copy source register to color register */
.Lop_color:
    lw      $t0, 0($s4)          /* c = (uint8)SREG */
    andi    $t0, $t0, 0x00FF
    lw      $t1, GSU_PLOTOPT_OFF($s0) /* vPlotOptionReg */
    /* if (vPlotOptionReg & 0x04) c = (c & 0xF0) | (c >> 4) */
    andi    $t2, $t1, 0x04
    beqz    $t2, 1f
    nop
    andi    $t3, $t0, 0xF0
    srl     $t4, $t0, 4
    or      $t0, $t3, $t4
1:
    /* if (vPlotOptionReg & 0x08) { vColorReg &= 0xF0; vColorReg |= c & 0x0F; } else vColorReg = USEX8(c); */
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
    andi    $t0, $t0, 0x00FF
    sw      $t0, GSU_COLOR_OFF($s0)
3:
    andi    $s6, $s6, 0xEFFF     /* CLRFLAGS */
    addiu   $s2, $s2, 1          /* R15++ */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

