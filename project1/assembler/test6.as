        lw      0       1       five
        lw      1       8       3       error checking : register outside the range
start   add     1       8       1
        beq     0       1       8
        beq     0       0       start
        noop
done    halt
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start
