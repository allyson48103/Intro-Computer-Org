	lw	0	3	answer
	lw	0	2	mcand
	lw	0	1	mplier
	lw	0	4	neg1
	lw	0	6	negm
	lw	0	7	bitcnt
begin	nor	6	1	5
	lw	0	4	negp
	beq	5	4	jump
	add	3	2	3
jump	lw	0	4	neg1
	add	1	1	1
	add	7	4	7
	beq	7	0	finish
	add	3	3	3
	beq	0	0	begin
	noop
finish	halt
answer	.fill	0
mcand	.fill	6203
mplier	.fill	1429
neg1	.fill	-1
negm	.fill	-16385
bitcnt	.fill	15
negp	.fill	16384
