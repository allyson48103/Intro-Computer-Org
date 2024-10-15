	lw	0	1	laPos1	// Load reg1 with a large positive number
	lw	0	2	laPos2	// Load reg2 with another large positive number
	add	1	2	3	// Add reg1 and reg2, store result in reg3 (overflow expected)
	halt				// Stop execution
laPos1	.fill	2147483647			// Largest 32-bit signed positive number
laPos2	.fill	1			// Adding 1 will cause an overflow
