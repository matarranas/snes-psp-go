/* Auto-generated MIPS Allegrex for fx_bmi */
.set noreorder
.text
.globl mips_fx_bmi
mips_fx_bmi:
.set noreorder

fx_bmi:
    # Load the GSU struct pointer from $a0
    lw $t0, 60($a0)          # Load R15 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1        # R15++

    # Store the incremented R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Load the vSign value from the GSU struct
    lw $t1, 116($a0)         # Load vSign into $t1

    # Check if vSign & 0x8000 is set
    andi $t2, $t1, 0x8000    # $t2 = vSign & 0x8000

    # Load the PIPE value into $t3
    lbu $t3, 0($t0)          # Load PIPE into $t3

    # Check if vSign & 0x8000 is set
    beqz $t2, increment_pc   # If vSign & 0x8000 is not set, jump to increment_pc

    # If vSign & 0x8000 is set, add (int32)(int8)v to R15
    add $t0, $t0, $t3        # R15 += (int32)(int8)v

    # Store the updated R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Jump to the end of the function
    j end_fx_bmi
    nop                      # Fill branch delay slot

increment_pc:
    # Increment R15 (PC) again
    addiu $t0, $t0, 1        # R15++

    # Store the incremented R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

end_fx_bmi:
    # Return from the function
    jr $ra
    nop                      # Fill branch delay slot

    .set reorder
