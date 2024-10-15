gBit	nor	2	2	7	// r2 nor r2 -> r7
	nor	3	3	6	// r3 nor r3 -> r6
	nor	7	6	4	// r7 nor r6 -> r4 (r2 and r3 -> r4)
	beq	4	0	leftSh	// if r4 == 0, jump to leftSh, else proceed
tOne	add	0	5	0	// r0 = r0 + r5 (accumulate in r0)
leftSh	add	5	5	5	// r5 = r5 * 2 (shift r5 left by 1 for next sum)
