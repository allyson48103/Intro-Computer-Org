        lw      0       1       data1   load reg1 with data1
        lw      0       2       data2   load reg2 with data2
        add     1       2       3       add reg1 and reg2, store in reg3
        sw      0       3       result  store reg3 to result
        halt
data1   .fill   15
data2   .fill   25
result  .fill   0                       result location
