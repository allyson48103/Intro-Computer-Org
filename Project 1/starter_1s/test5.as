	lw	0	1	shift	// Load reg1 with a value to shift
	lw	0	2	two	// Load reg2 with constant 2
	add	1	1	1	// Shift reg1 left by 1 bit (equivalent to multiplying by 2)
	nor	1	2	3	// NOR reg1 and reg2, store result in reg3
	halt				// Stop execution
shift	.fill	4			// Initial value for shifting
two	.fill	-2			// Constant 2
