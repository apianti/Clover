;
; Library/Uefi/X64/swap.nasm
;
; UEFI implementation IA32 byte swap intrinsics
;

  default rel
  section .text

  global _EfiByteSwap16
  global _EfiByteSwap32
  global _EfiByteSwap64

; EfiByteSwap16
; Swap the endian byte order of a 16bit word
; @param Word The word to byte swap
; @return The byte swapped word
_EfiByteSwap16:

  xor   eax, eax
  mov   ax, [esp + 4]
  xchg  ah, al
  ret

; EfiByteSwap16
; Swap the endian byte order of a 32bit word
; @param Dword The double word to byte swap
; @return The byte swapped double word
_EfiByteSwap32:

  xor   eax, eax
  mov   eax, [esp + 4]
  bswap eax
  ret

; EfiByteSwap64
; Swap the endian byte order of a 64bit quad word
; @param Qword The quad word to byte swap
; @return The byte swapped word
_EfiByteSwap64:

  mov   eax, [esp + 4]
  bswap eax
  mov   edx, [esp + 8]
  bswap edx
  ret
