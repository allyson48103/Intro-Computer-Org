        lw      0       1       seven   load reg1 with 7
        lw      0       2       seven   load reg2 with 7
        beq     1       2       equal   branch to equal if reg1 == reg2
        lw      0       3       zero    load reg3 with 0
        halt
equal   lw      0       3       one     load reg3 with 1
        halt
seven   .fill   7
zero    .fill   0
one     .fill   1
