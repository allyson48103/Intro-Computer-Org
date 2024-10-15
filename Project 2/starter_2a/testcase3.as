	lw	0	1	one	; Load 1 into $1
	lw	0	2	one	; Load 1 into $2
	beq	1	2	end	; Branch if equal
	halt				; End if not equal
end	halt				; End if equal
one	.fill	1			; Define constant 1
