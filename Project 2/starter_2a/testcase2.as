	lw	0	1	label	; $1 = memory[label]
	sw	1	2	label	; memory[label] = $2
	.fill	123			; Data stored in memory
label	.fill	456			; Store address
