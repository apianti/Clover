;
; Library/Uefi/X64/msr.nasm
;
; UEFI implementation X64 model specific register intrinsics
;

  default rel
  section .text

  global EfiReadMsr
  global EfiWriteMsr

; EfiReadMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
EfiReadMsr:

  rdmsr
  ret

; EfiWriteMsr
; Read CPU model specific register
; @param Msr The model specific register to read
; @return The value of the model specific register
EfiWriteMsr:

  mov   r9, rdx
  mov   rdx, r8
  mov   r8, r9
  wrmsr
  ret
