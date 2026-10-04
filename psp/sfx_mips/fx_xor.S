/* Auto-generated MIPS Allegrex for fx_xor */
.set noreorder
.text
.globl mips_fx_xor
mips_fx_xor:
.set noreorder

fx_xor:
    # Load the GSU struct pointer from $a0
    lw $t0, 100($a0)          # Load pvDreg into $t0
    lw $t1, 104($a0)          # Load pvSreg into $t1
    lw $t2, 120($a0)          # Load vZero into $t2

    # Load the register value from GSU.avReg[reg]
    sll $t3, $a1, 2           # $t3 = reg * 4
    addu $t3, $a0, $t3        # $t3 = &GSU.avReg[reg]
    lw $t4, 0($t3)            # Load GSU.avReg[reg] into $t4

    # XOR the register value with SREG
    lw $t5, 0($t1)            # Load SREG into $t5
    xor $t6, $t5, $t4         # $t6 = SREG ^ GSU.avReg[reg]

    # Mask the result to 0xffff
    andi $t6, $t6, 0xffff     # $t6 = ($t6 & 0xffff)

    # Store the result to DREG
    sw $t6, 0($t0)            # DREG = $t6

    # Update vSign with the result
    sw $t6, 116($a0)          # GSU.vSign = $t6

    # Update vZero with the result
    beqz $t6, 1f              # If $t6 == 0, set vZero to 1
    sw $zero, 120($a0)        # GSU.vZero = 0
    j 2f
1:
    sw $zero, 120($a0)        # GSU.vZero = 1
2:

    # Clear flags (vCarry, vOverflow)
    sw $zero, 124($a0)        # GSU.vCarry = 0
    sw $zero, 128($a0)        # GSU.vOverflow = 0

    # Increment R15 (PC)
    lw $t7, 60($a0)          # Load R15 into $t7
    addiu $t7, $t7, 1         # R15++
    sw $t7, 60($a0)          # Store R15 back

    # Return
    jr $ra
    nop

    .set reorder
