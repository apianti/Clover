;
; Library/Uefi/X64/msr.nasm
;
; UEFI implementation IA32 model specific register intrinsics
;

  default rel
  section .text

  global _EfiReadMsr
  global _EfiWriteMsr

; EfiReadMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
_EfiReadMsr:

  mov    ecx, [esp + 4]
  rdmsr
  ret

; EfiWriteMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
_EfiWriteMsr:

  mov    edx, [esp + 12]
  mov    eax, [esp + 8]
  mov    ecx, [esp + 4]
  wrmsr
  ret
