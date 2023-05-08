        lw	    0   1	five
        lw      0   2   go
        lw      0   3   stAddr
        beq     0   0   jump
next    lw      0   5   go
	    noop
done	halt
jump    add     1   2   1 
        sw      0   1   go
        jalr    3   4
five	.fill   5
go   	.fill   2
stAddr	.fill   next
