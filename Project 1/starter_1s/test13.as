	lw	0	1	five	// Load 5 into reg1
	lw	0	2	zero	// Load 0 into reg2
	beq	1	2	jump	// If reg1 != reg2, jump to 'jump' label (won't branch here)
	add	1	2	1	// Add reg1 and reg2 (5 + 0 = 5)
jump	beq	0	0	end	// Unconditional branch to 'end'
	noop				// No operation
	beq	0	0	start	// Large negative branch back to the 'start' (does not execute)
end	halt				// Stop execution
five	.fill	5			// Define value 5
zero	.fill	0			// Define value 0
