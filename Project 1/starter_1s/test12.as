	lw	0	1	five	// Load 5 into reg1
	lw	0	2	neg1	// Load -1 into reg2
	sw	1	2	-32768	// Attempt to store reg2 to a very low (negative) memory address (-32768)
	add	1	2	3	// Simple arithmetic (5 + -1 = 4)
done	halt				// Stop execution
five	.fill	5			// Define value 5
neg1	.fill	-1			// Define value -1
