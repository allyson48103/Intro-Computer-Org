	lw	0	2	1000	; Load memory[1000] into register 2
loop	beq	2	5	finish	; If $2 == $5, branch to finish
	jalr	4	7		; Jump to address in $4, save return in $7
	nor	2	3	2	; Perform NOR on $2 and $3, store result in $2
	beq	0	0	loop	; Unconditional branch to loop
	noop				; No operation
finish	halt				; End of the program
