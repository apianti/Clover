;
; Library/Uefi/X64/swap.nasm
;
; UEFI implementation X64 byte swap intrinsics
;

  default rel
  section .text

  global EfiByteSwap16
  global EfiByteSwap32
  global EfiByteSwap64

; EfiByteSwap16
; Swap the endian byte order of a 16bit word
; @param Word The word to byte swap
; @return The byte swapped word
EfiByteSwap16:

  xor   rax, rax
  mov   ax, cx
  xchg  ah, al
  ret

; EfiByteSwap16
; Swap the endian byte order of a 32bit word
; @param Dword The double word to byte swap
; @return The byte swapped double word
EfiByteSwap32:

  xor   rax, rax
  mov   eax, ecx
  bswap eax
  ret

; EfiByteSwap64
; Swap the endian byte order of a 64bit quad word
; @param Qword The quad word to byte swap
; @return The byte swapped word
EfiByteSwap64:

  mov   rax, rcx
  bswap rax
  ret
