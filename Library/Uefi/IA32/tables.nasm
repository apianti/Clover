;
; Library/Uefi/X64/tables.nasm
;
; UEFI implementation IA32 CPU table pointer intrinsics
;

  default rel
  section .text

  global _EfiGetInterruptDescriptorTable
  global _EfiGetGlobalDescriptorTable

; EfiGetInterruptDescriptorTable
; Get the interrupt descriptor address and offset
; @return The current interrupt descriptor address and offset
_EfiGetInterruptDescriptorTable:

  mov   ecx, [esp + 4]
  sidt  [ecx]
  ret

; EfiGetGlobalDescriptorTable
; Get the global descriptor address and offset
; @return The current global descriptor address and offset
_EfiGetGlobalDescriptorTable:

  mov   ecx, [esp + 4]
  sgdt  [ecx]
  ret