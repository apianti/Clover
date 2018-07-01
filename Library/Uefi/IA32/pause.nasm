;
; Library/Uefi/X64/pause.nasm
;
; UEFI implementation IA32 PAUSE intrinsic
;

  default rel
  section .text

  global _EfiCpuPause

; EfiCpuPause
; Pause the CPU for an idle cycle
_EfiCpuPause:

  pause
  ret