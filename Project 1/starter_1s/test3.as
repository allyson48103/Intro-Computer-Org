	lw	0	1	count	// Load the loop counter (10) into reg1
	lw	0	2	three	// Load constant 1 into reg2
loop	beq	1	0	done	// If reg1 == 0, exit the loop
	add	1	2	1	// Decrement reg1 (reg1 -= 1)
	beq	0	0	loop	// Unconditional branch back to the loop
done	halt				// Stop execution
count	.fill	10			// Initial counter value
three	.fill	3			// Constant 1 for decrementing
