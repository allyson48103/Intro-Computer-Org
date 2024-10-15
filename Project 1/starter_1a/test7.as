        lw      0       1       three   load reg1 with 3
        lw      0       2       two     load reg2 with 2
        add     1       2       3       add reg1 and reg2, store in reg3
        sub     1       2       4       subtract reg2 from reg1, store in reg4
        beq     4       3       done    branch to done if reg4 == reg3
        noop
done    halt
three   .fill   3
two     .fill   2
