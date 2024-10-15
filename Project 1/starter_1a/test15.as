	lw	0	1	maxneg
	lw	0	1	maxpos
	halt
maxpos	.fill	32767			# Maximum negative offset (16-bit signed integer)
maxneg	.fill	-32768			# Maximum negative offset (16-bit signed integer)
