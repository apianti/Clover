;
; Library/Uefi/X64/pause.nasm
;
; UEFI implementation X64 PAUSE intrinsic
;

  default rel
  section .text

  global EfiCpuPause

; EfiCpuPause
; Pause the CPU for an idle cycle
EfiCpuPause:

  pause
  ret