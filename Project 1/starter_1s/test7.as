	lw	0	1	dataA	// Load reg1 with dataA
	add	1	1	2	// Double reg1 value, store result in reg2
	add	2	1	3	// Add reg2 and reg1, store result in reg3 (should be 3 * reg1)
	halt				// Stop execution
dataA	.fill	5			// Data value to load
