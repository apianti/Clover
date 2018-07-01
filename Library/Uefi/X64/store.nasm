;
; Library/Uefi/X64/store.nasm
;
; UEFI implementation X64 store intrinsics
;

  default rel
  section .text

  global EfiStore32
  global EfiStore64

; EfiStore32
; Store a 32bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
EfiStore32:

  mov     [rcx], edx
  sfence
  ret

; EfiStore64
; Store a 64bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
EfiStore64:

  mov     [rcx], rdx
  sfence
  ret
