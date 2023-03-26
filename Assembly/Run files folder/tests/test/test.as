K:  .data 22
MAIN: mov S1 , W
add r2,STR
S2:  .data -1  ,   123

XYZ:  jmp W(#23,#-9)
  mcr m1
  inc K
  mov S1 , r3 
  endmcr
  prn  #-50

  sub r1 , r4
  m1
  bne L3
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-512,511
S1:  .data 8, 1
.entry K
.entry XYZ
.extern L3
.extern W
