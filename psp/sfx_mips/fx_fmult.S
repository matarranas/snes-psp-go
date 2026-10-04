/* Auto-generated MIPS Allegrex for fx_fmult */
.set noreorder
.text
.globl mips_fx_fmult
mips_fx_fmult:
.set noreorder

fx_fmult:
    # Load R6 and SREG into temporary registers
    lw $t0, 24($a0)          # $t0 = R6
    lw $t1, 104($a0)         # $t1 = pvSreg
    lw $t2, 0($t1)           # $t2 = SREG

    # Convert SREG and R6 to int16
    seb $t2, $t2             # $t2 = (int16)SREG
    seb $t0, $t0             # $t0 = (int16)R6

    # Multiply the two int16 values
    mult $t2, $t0            # Multiply the two values

    # Extract the high 16 bits of the result
    mfhi $t3                 # $t3 = high 16 bits of the result

    # Extract the low 16 bits of the result
    mflo $t4                 # $t4 = low 16 bits of the result

    # Calculate the carry flag
    srl $t5, $t4, 15         # $t5 = (c >> 15) & 1

    # Store the high 16 bits of the result into DREG
    lw $t6, 100($a0)         # $t6 = pvDreg
    sw $t3, 0($t6)           # DREG = $t3

    # Store the high 16 bits of the result into vSign
    sw $t3, 116($a0)         # GSU.vSign = $t3

    # Store the high 16 bits of the result into vZero
    sw $t3, 120($a0)         # GSU.vZero = $t3

    # Store the carry flag into vCarry
    sw $t5, 124($a0)         # GSU.vCarry = $t5

    # Increment R15 (PC)
    lw $t7, 60($a0)          # $t7 = R15
    addiu $t7, $t7, 1        # R15++
    sw $t7, 60($a0)          # Store back R15

    # Clear flags (CLRFLAGS)
    # Assuming CLRFLAGS is a macro or function call that clears flags
    # If CLRFLAGS is a simple instruction, replace this with the actual instruction
    # For example, if CLRFLAGS is a no-op, you can use:
    # nop

    # Return
    jr $ra
    nop

.set reorder
