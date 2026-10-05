/*
 * fx_mips_ram.inc - Hand-tuned Super FX RAM Access Handlers (LDW, LDB, STW, STB)
 * Supports little-endian word loads and stores with correct R14 ROM buffer checks.
 */

/* =========================================================================
 * LDW (Rn) (0x40 - 0x4B, ALT0): Load 16-bit word from RAM at Rn
 * ========================================================================= */
.Lop_ldw_rn:
    andi    $t0, $t3, 0x0F          /* n = opcode & 0x0F */
    sll     $t0, $t0, 2             /* n * 4 */
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* $t1 = Address = Rn */
    andi    $t1, $t1, 0xFFFF
    sw      $t1, GSU_LASTRAM_OFF($s0)

    /* RAM pointer */
    lw      $t2, GSU_RAMBANK_PTR($s0)
    addu    $t3, $t2, $t1           /* pvRamBank + Address */
    lbu     $t4, 0($t3)             /* Low byte: RAM(Address) */

    xori    $t5, $t1, 1             /* Address ^ 1 */
    addu    $t5, $t2, $t5           /* pvRamBank + (Address ^ 1) */
    lbu     $t6, 0($t5)             /* High byte: RAM(Address ^ 1) */
    sll     $t6, $t6, 8
    or      $t4, $t4, $t6           /* 16-bit word */

    sw      $t4, 0($s5)             /* *DREG = v */

    /* TESTR14: if pvDreg == &R14, READR14 */
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
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * LDB (Rn) (0x40 - 0x4B, ALT1): Load 8-bit byte from RAM at Rn
 * ========================================================================= */
.Lop_ldb_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* Address */
    andi    $t1, $t1, 0xFFFF
    sw      $t1, GSU_LASTRAM_OFF($s0)

    lw      $t2, GSU_RAMBANK_PTR($s0)
    addu    $t3, $t2, $t1
    lbu     $t4, 0($t3)             /* Byte */
    sw      $t4, 0($s5)             /* *DREG = byte */

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
    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * STW (Rn) (0x30 - 0x3B, ALT0): Store 16-bit word from SREG to RAM at Rn
 * ========================================================================= */
.Lop_stw_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* Address */
    andi    $t1, $t1, 0xFFFF
    sw      $t1, GSU_LASTRAM_OFF($s0)

    lw      $t2, GSU_RAMBANK_PTR($s0)
    lw      $t4, 0($s4)             /* SREG */

    /* RAM(Address) = (uint8)SREG */
    addu    $t3, $t2, $t1
    sb      $t4, 0($t3)

    /* RAM(Address ^ 1) = (uint8)(SREG >> 8) */
    xori    $t5, $t1, 1
    addu    $t5, $t2, $t5
    srl     $t6, $t4, 8
    sb      $t6, 0($t5)

    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero

/* =========================================================================
 * STB (Rn) (0x30 - 0x3B, ALT1): Store 8-bit byte from SREG to RAM at Rn
 * ========================================================================= */
.Lop_stb_rn:
    andi    $t0, $t3, 0x0F
    sll     $t0, $t0, 2
    addu    $t0, $s0, $t0           /* &GSU.avReg[n] */
    lw      $t1, 0($t0)             /* Address */
    andi    $t1, $t1, 0xFFFF
    sw      $t1, GSU_LASTRAM_OFF($s0)

    lw      $t2, GSU_RAMBANK_PTR($s0)
    lw      $t4, 0($s4)             /* SREG */
    addu    $t3, $t2, $t1
    sb      $t4, 0($t3)

    andi    $s6, $s6, 0xEFFF        /* CLRFLAGS */
    addu    $s4, $s0, $zero
    j       .Lfx_dispatch_loop
    addu    $s5, $s0, $zero
