        lw      0       1       five
        lw      1       2       3
loop    add     1       2       1
        beq     0       1       2
        beq     0       0       loop
        noop
loop    halt                            error checking : duplicate definition of label
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start
