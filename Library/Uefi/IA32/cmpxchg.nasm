;
; Library/Uefi/X64/tables.nasm
;
; UEFI implementation IA32 compare and exchange intrinsics
;

  default rel
  section .text

  global _EfiCompareAndExchange32
  global _EfiCompareAndExchange64

; EfiCompareAndExchange32
; Compare and exchange a 32bit value atomically 
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
_EfiCompareAndExchange32:

  mov           ecx, [esp + 4]
  mov           eax, [esp + 8]
  mov           edx, [esp + 12]
  lock cmpxchg  [ecx], edx
  ret

; EfiCompareAndExchange64
; Compare and exchange a 32bit value atomically
; @param Value         On output, the set or unchanged value
; @param CompareValue  The value in which to compare to set the value if equal
; @param ExchangeValue The value to set
; @return The initial value
_EfiCompareAndExchange64:

  push            esi
  push            ebx
  mov             esi, [esp + 12]
  mov             eax, [esp + 16]
  mov             edx, [esp + 20]
  mov             ebx, [esp + 24]
  mov             ecx, [esp + 28]
  lock cmpxchg8b  [esi]
  pop             ebx
  pop             esi
  ret
