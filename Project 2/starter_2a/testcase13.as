	sw	0	1	DnSW	; Store value from $1 at DnSW
UpSW	sw	0	2	0	; Store value from $2 at memory location 0
SmSW	sw	0	3	SmSW	; Store value from $3 at SmSW
DnSW	sw	0	4	0	; Store value from $4 at memory location 0
	sw	0	5	UpSW	; Store value from $5 at UpSW
