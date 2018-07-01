;
; Library/Uefi/X64/tables.nasm
;
; UEFI implementation X64 compare and exchange intrinsics
;

  default rel
  section .text

  global EfiCompareAndExchange32
  global EfiCompareAndExchange64

; EfiCompareAndExchange32
; Compare and exchange a 32bit value atomically 
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
EfiCompareAndExchange32:

  mov           eax, edx
  lock cmpxchg  [rcx], r8d
  ret

; EfiCompareAndExchange64
; Compare and exchange a 32bit value atomically 
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
EfiCompareAndExchange64:

  mov           rax, rdx
  lock cmpxchg  [rcx], r8
  ret
