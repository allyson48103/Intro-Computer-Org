	sw	0	3	two
	lw	3	2	3
	noop
begin	nor	3	2	3
	jalr	0	3
	beq	0	0	begin
done	halt
two	.fill	2
neg3	.fill	-3
stadd	.fill	begin
