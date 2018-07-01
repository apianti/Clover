;
; Library/Uefi/X64/tables.nasm
;
; UEFI implementation X64 CPU table pointer intrinsics
;

  default rel
  section .text

  global EfiGetInterruptDescriptorTable
  global EfiGetGlobalDescriptorTable

; EfiGetInterruptDescriptorTable
; Get the interrupt descriptor address and offset
; @return The current interrupt descriptor address and offset
EfiGetInterruptDescriptorTable:

  sidt  [rcx]
  ret

; EfiGetGlobalDescriptorTable
; Get the global descriptor address and offset
; @return The current global descriptor address and offset
EfiGetGlobalDescriptorTable:

  sgdt  [rcx]
  ret