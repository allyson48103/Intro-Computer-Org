	lw	0	1	jump	// Load jump address into reg1
	jalr	1	2		// Jump to address in reg1, store return address in reg2
	add	0	0	3	// Shouldn't reach here if jump works correctly
done	halt				// Stop execution
jump	.fill	done			// Address to jump to (the done label)
