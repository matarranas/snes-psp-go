/* Auto-generated MIPS Allegrex for fx_dec */
.set noreorder
.text
.globl mips_fx_dec
mips_fx_dec:
.set noreorder

fx_dec_reg:
    # Load the register index into $t0
    move $t0, $a1

    # Calculate the offset for the register
    sll $t0, $t0, 2          # Multiply reg by 4

    # Load the register value into $v0
    lw $v0, 0($a0)           # Load R0
    addu $t1, $a0, $t0       # Calculate the address of the target register
    lw $v0, 0($t1)           # Load the target register value

    # Decrement the register value
    addiu $v0, $v0, -1       # Decrement the register value

    # Mask the value to 16 bits
    andi $v0, $v0, 0xffff    # Mask to 16 bits

    # Store the decremented value back to the register
    sw $v0, 0($t1)           # Store the decremented value

    # Update vSign with the decremented value
    sw $v0, 116($a0)         # Store to vSign

    # Update vZero with the decremented value
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
