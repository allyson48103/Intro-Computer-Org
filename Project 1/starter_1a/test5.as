        lw      0       1       neg2    load reg1 with -2
        lw      0       2       five    load reg2 with 5
        add     1       2       3       add reg1 and reg2, store in reg3
        beq     0       3       done    branch to done if reg3 == 0
        noop
done    halt
neg2    .fill   -2
five    .fill   5
