///
/// @file Library/Uefi/ARM/helper.c
///
/// GUI language parser implementation
///

#include <Uefi.h>

// EfiByteSwap16
/// Swap the endianness of a 16bit word
/// @param Word The 16bit word of which to swap endianness
/// @return The 16bit word with swapped endianess
UINT16
EFIAPI
EfiByteSwap16 (
  IN UINT16 Word
) {
  return ((EFI_BITFIELD(Word, 0, 7) << 8) | (EFI_BITFIELD(Word, 8, 15) >> 8));
}
// EfiByteSwap32
/// Swap the endianness of a 32bit word
/// @param Dword The 32bit word of which to swap endianness
/// @return The 32bit word with swapped endianess
UINT32
EFIAPI
EfiByteSwap32 (
  IN UINT32 Dword
) {
  return (EfiByteSwap16(EFI_BITFIELD(Dword, 0, 15) << 16) | EfiByteSwap16(EFI_BITFIELD(Dword, 16, 31) >> 16));
}
// EfiByteSwap64
/// Swap the endianness of a 64bit word
/// @param Qword The 64bit word of which to swap endianness
/// @return The 64bit word with swapped endianess
UINT64
EFIAPI
EfiByteSwap64 (
  IN UINT64 Qword
) {
  return (EfiByteSwap32(EFI_BITFIELD(Qword, 0, 31) << 32) | EfiByteSwap32(EFI_BITFIELD(Qword, 32, 63) >> 32));
}

// _helper_divide_by_0
/// Divide by zero exception handler helper
VOID
EFIAPI
__helper_divide_by_0 (
  VOID
) {
}
