start   lw      0       1       value   load reg1 with value (5)
        beq     1       0       start   infinite loop if reg1 != 0
        noop
        halt
value   .fill   5
