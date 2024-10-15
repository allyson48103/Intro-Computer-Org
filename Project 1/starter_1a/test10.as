        lw      0       1       value1   load reg1 with value1
        lw      0       2       value2   load reg2 with value2
        add     2       1       4       add reg2 and reg1, store in reg4
        beq     3       0       done    branch to done if reg3 == 0
        beq     4       0       done    branch to done if reg4 == 0
        noop
done    halt
value1   .fill   3
value2   .fill   2
