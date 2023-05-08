        lw      0       0       0
        add     0       0       1        set reg1 to 0
        lw      0       2       mcand    load mcand into reg2
        lw      0       3       mplier   load mplier into reg3
        lw      0       4       counter  repetition count
        lw      0       5       minus
        lw      0       6       mask
        lw      0       0       7
mult    beq     0       3       done     goto end of program when mplier==0
        beq     0       4       done     goto end of program
        add     0       0       7
        nor     6       3       7        mplier mask
        add     6       6       6        shift mask left 1 bit
        nor     3       7       3        shift mplier right 1 bit
        beq     0       7       shift    goto shift when mplier0=1
        add     1       2       1        add mcand to reg1
shift   add     2       2       2        shift mcand left 1 bit
        add     4       5       4        counter minus 1
        beq     0       0       mult     go back to the beginning of the loop    
done    halt                             end of program
mcand   .fill   32766
mplier  .fill   12328
counter .fill   32
minus   .fill   -1
mask    .fill   -2
