	lw	0	1	astart	; Load address of array into $1
	lw	0	2	Count	; Load count into $2
	lw	0	3	func	; Load function address into $3
	jalr	3	7		; Jump to function, save return in $7
	halt
Search	lw	0	6	minOne	; Load -1 into $6
	add	0	2	5	; Set $5 as a copy of $2
Begin	add	2	6	2	; Decrement $2
	add	1	2	4	; Calculate index in array
	lw	4	3	0	; Load the value from array into $3
	beq	3	0	bypass	; If $3 is 0, skip the next steps
	add	5	6	5	; Update $5
bypass	beq	0	2	End	; If $2 == 0, branch to End
	beq	0	0	Begin	; Loop back to Begin
End	jalr	7	3		; Return from function
Zero	.fill	0
minOne	.fill	-1
func	.fill	Search
astart	.fill	Array
Count	.fill	9
Array	.fill	0
	.fill	1
	.fill	1
	.fill	1111
	.fill	0
	.fill	6
	.fill	0
	.fill	1
	.fill	-5
