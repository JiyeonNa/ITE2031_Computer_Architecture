        lw      0       2       mcand    load mcand into reg2
        lw      0       3       mplier   load mplier into reg3
        lw      0       4       maximum
        lw      0       5       count
mult    nor     3       3       3        mplier 1's complement
        nor     5       5       5        count 1's complement
        nor     3       5       6
        nor     3       3       3        restore mplier
        nor     5       5       5        restore count
        beq     0       6       shift
        add     1       2       1        add mcand to reg1
shift   add     2       2       2        shift mcand left 1 bit
        add     5       5       5        shift count left 1 bit
        beq     5       4       done
        beq     0       0       mult     go back to the beginning of the loop    
done    halt                             end of program
mcand   .fill   32766
mplier  .fill   12328
maximum .fill   32768
count   .fill   1
