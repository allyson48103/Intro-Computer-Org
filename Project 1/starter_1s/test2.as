	lw	0	1	outOfB	// Load reg1 with an out-of-bounds memory address
	lw	1	2	0	// Attempt to load from an invalid address, expect failure
	halt				// Stop execution
outOfB	.fill	65536			// Out-of-bounds memory address (65536, which exceeds the valid range)
