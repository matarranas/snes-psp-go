/* Auto-generated MIPS Allegrex for fx_inc */
.set noreorder
.text
.globl mips_fx_inc
mips_fx_inc:
.set noreorder

fx_inc_reg:
    # Load the register index into $t0
    move $t0, $a1

    # Calculate the offset for avReg[reg]
    sll $t0, $t0, 2          # Multiply reg by 4 to get byte offset

    # Load the current value of avReg[reg] into $v0
    lw $v0, 0($a0)           # Load R0
    addu $t1, $a0, $t0       # Calculate address of avReg[reg]
    lw $v0, 0($t1)           # Load avReg[reg]

    # Increment the register value and mask with 0xffff
    addiu $v0, $v0, 1        # Increment by 1
    andi $v0, $v0, 0xffff    # Mask with 0xffff

    # Store the incremented value back to avReg[reg]
    sw $v0, 0($t1)           # Store back to avReg[reg]

    # Update vSign with the incremented value
    sw $v0, 116($a0)         # Store to vSign

    # Update vZero with the incremented value
    sw $v0, 120($a0)         # Store to vZero

    # Increment R15 (PC)
    lw $t2, 60($a0)          # Load R15
    addiu $t2, $t2, 1        # Increment R15
    sw $t2, 60($a0)          # Store back R15

    # Clear flags (CLRFLAGS)
    sw $zero, 124($a0)       # Clear vCarry
    sw $zero, 128($a0)       # Clear vOverflow

    # Return
    jr $ra
    nop                     # Fill branch delay slot

    .set reorder
