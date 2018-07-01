;
; Library/Uefi/X64/store.nasm
;
; UEFI implementation IA32 store intrinsics
;

  default rel
  section .text

  global _EfiStore32
  global _EfiStore64

; EfiStore32
; Store a 32bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
_EfiStore32:

  mov     eax, [esp + 4]
  mov     ecx, [esp + 8]
  mov     [eax], ecx
  sfence
  ret

; EfiStore64
; Store a 64bit value atomically 
; @param Value     On output, the set or unchanged value
; @param NewValue  The value in which to compare to set the value if equal
_EfiStore64:

  mov     eax, [esp + 4]
  mov     ecx, [esp + 8]
  mov     edx, [esp + 12]
  mov     [eax], ecx
  mov     [eax + 4], edx
  sfence
  ret
