	lw	0	1	val1	// Load reg1 with val1
	lw	0	2	val2	// Load reg2 with val2
	lw	0	3	zero	// Load reg3 with 0
	beq	1	2	equal	// If reg1 == reg2, jump to equal
	beq	1	3	less	// If reg1 == 0, jump to less
	beq	0	0	great	// Otherwise, jump to greater
equal	add	1	0	4	// Set reg4 = reg1 (val1)
	beq	0	0	done	// Jump to done
less	add	2	0	5	// Set reg5 = reg2 (val2)
	beq	0	0	done	// Jump to done
great	add	1	2	6	// Set reg6 = reg1 + reg2
done	halt				// Stop execution
val1	.fill	7			// Value for reg1
val2	.fill	7			// Value for reg2
zero	.fill	0			// Constant 0
