start	lw	0	1	count	; Load count
	beq	1	0	done	; Branch if $1 == 0
	beq	0	0	start	; Loop back to start
done	halt
count	.fill	5
