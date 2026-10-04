/* Auto-generated MIPS Allegrex for fx_add */
.set noreorder
.text
.globl mips_fx_add
mips_fx_add:
.set noreorder

fx_add:
    # Load the register index into $t0
    move $t0, $a1

    # Load SREG into $t1
    lw $t1, 104($a0)

    # Load the value of GSU.avReg[reg] into $t2
    sll $t0, $t0, 2          # Multiply reg by 4 to get offset
    addu $t0, $a0, $t0       # Add base address to get pointer to GSU.avReg[reg]
    lw $t2, 0($t0)           # Load the value

    # Convert SREG and GSU.avReg[reg] to int16 and add them
    seb $t1, $t1             # Sign-extend SREG to int32
    seb $t2, $t2             # Sign-extend GSU.avReg[reg] to int32
    add $t3, $t1, $t2         # Add the two values

    # Update vCarry
    sltu $t4, $t3, $zero     # Set $t4 to 1 if $t3 < 0, else 0
    sw $t4, 124($a0)         # Store vCarry

    # Update vOverflow
    xor $t5, $t1, $t2        # XOR SREG and GSU.avReg[reg]
    xor $t6, $t2, $t3        # XOR GSU.avReg[reg] and result
    and $t7, $t5, $t6        # AND the results
    andi $t7, $t7, 0x8000    # Mask to get overflow bit
    sw $t7, 128($a0)         # Store vOverflow

    # Update vSign
    sw $t3, 116($a0)         # Store vSign

    # Update vZero
    sw $t3, 120($a0)         # Store vZero

    # Update R15 (PC)
    lw $t8, 60($a0)          # Load R15
    addiu $t8, $t8, 1        # Increment R15
    sw $t8, 60($a0)          # Store R15

    # Update DREG
    andi $t9, $t3, 0xffff    # Mask to get lower 16 bits
    sw $t9, 100($a0)         # Store DREG

    # Return
    jr $ra
    nop
