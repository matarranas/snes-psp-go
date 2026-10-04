/* Phase 3A: RAM Load & Store Handlers (LDW, LDB, STW, STB) */

.Lop_ldw_rn:
    # n = opcode & 0x0F
    andi $t0, $a0, 0x0F
    # Address = Rn & 0xFFFF
    lw $t1, 0($t0)
    andi $t1, $t1, 0xFFFF
    # Load 16-bit word little endian from pvRamBank[Address]
    lw $t2, GSU_RAMBANK_PTR($s0)
    lw $t3, 0($t1, $t2)
    # Store into DREG
    sw $t3, 0($s5)
    # Update vLastRamAdr
    sw $t1, GSU_LASTRAM_OFF($s0)
    # If pvDreg == &R14, READR14
    beq $s5, $s4, .READR14
    j .CLRFLAGS
.READR14:
    lw $t4, GSU_R14_OFF($s0)
    sw $t4, 0($s5)
.CLRFALGS:
    andi $s6, $s6, 0xEFFF
    # j .Lfx_dispatch_loop
    j $ra

.Lop_ldb_rn:
    # n = opcode & 0x0F
    andi $t0, $a0, 0x0F
    # Address = Rn & 0xFFFF
    lw $t1, 0($t0)
    andi $t1, $t1, 0xFFFF
    # Load 8-bit byte from pvRamBank[Address]
    lb $t2, 0($t1, $t2)
    # Store into DREG
    sw $t2, 0($s5)
    # Update vLastRamAdr
    sw $t1, GSU_LASTRAM_OFF($s0)
    # If pvDreg == &R14, READR14
    beq $s5, $s4, .READR14
    j .CLRFLAGS
.READR14:
    lw $t4, GSU_R14_OFF($s0)
    sw $t4, 0($s5)
.CLRFALGS:
    andi $s6, $s6, 0xEFFF
    # j .Lfx_dispatch_loop
    j $ra

.Lop_stw_rn:
    # n = opcode & 0x0F
    andi $t0, $a0, 0x0F
    # Address = Rn & 0xFFFF
    lw $t1, 0($t0)
    andi $t1, $t1, 0xFFFF
    # Store 16-bit word little endian from SREG into pvRamBank[Address]
    lw $t2, GSU_RAMBANK_PTR($s0)
    lw $t3, 0($s4)
    sw $t3, 0($t1, $t2)
    # Update vLastRamAdr
    sw $t1, GSU_LASTRAM_OFF($s0)
.CLRFALGS:
    andi $s6, $s6, 0xEFFF
    # j .Lfx_dispatch_loop
    j $ra

.Lop_stb_rn:
    # n = opcode & 0x0F
    andi $t0, $a0, 0x0F
    # Address = Rn & 0xFFFF
    lw $t1, 0($t0)
    andi $t1, $t1, 0xFFFF
    # Store 8-bit lower byte from SREG into pvRamBank[Address]
    lb $t2, 0($s4)
    sb $t2, 0($t1, $t2)
    # Update vLastRamAdr
    sw $t1, GSU_LASTRAM_OFF($s0)
.CLRFALGS:
    andi $s6, $s6, 0xEFFF
    # j .Lfx_dispatch_loop
    j $ra
