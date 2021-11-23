li x2, 0x010000
li x4, 0xdeadbeef
sw x4, 0(x2)
sw x4, -22(x2)
lw x5, 0(x2)
lw x6, -20(x2)
li a7, 1
ecall