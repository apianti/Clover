///
/// @file Include/Uefi/Intrinsics.h
///
/// UEFI intrinsicx
///

#pragma once
#ifndef ___UEFI_INTRINSICS_HEADER__
#define ___UEFI_INTRINSICS_HEADER__

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <Uefi/Base.h>

// EfiStore32
/// Store a 32bit value atomically 
/// @param Value     On output, the set or unchanged value
/// @param NewValue  The value in which to compare to set the value if equal
EXTERN
VOID
EFIAPI
EfiStore32 (
  IN OUT UINT32 *Value,
  IN     UINT32  NewValue
);
// EfiStore64
/// Store a 64bit value atomically 
/// @param Value     On output, the set or unchanged value
/// @param NewValue  The value in which to compare to set the value if equal
EXTERN
VOID
EFIAPI
EfiStore64 (
  IN OUT UINT64 *Value,
  IN     UINT64  NewValue
);

// EfiByteSwap16
/// Swap the endianness of a 16bit word
/// @param Word The 16bit word of which to swap endianness
/// @return The 16bit word with swapped endianess
EXTERN
UINT16
EFIAPI
EfiByteSwap16 (
  IN UINT16 Word
);
// EfiByteSwap32
/// Swap the endianness of a 32bit word
/// @param Dword The 32bit word of which to swap endianness
/// @return The 32bit word with swapped endianess
EXTERN
UINT32
EFIAPI
EfiByteSwap32 (
  IN UINT32 Dword
);
// EfiByteSwap64
/// Swap the endianness of a 64bit word
/// @param Qword The 64bit word of which to swap endianness
/// @return The 64bit word with swapped endianess
EXTERN
UINT64
EFIAPI
EfiByteSwap64 (
  IN UINT64 Qword
);

# if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

// EfiReadMsr
/// Read CPU model specific register
/// @param Msr The model specific register to read
/// @return The value of the model specific register
EXTERN
UINT64
EFIAPI
ReadMsr (
  IN UINT32 Msr
);
// EfiReadMsr
/// Read CPU model specific register
/// @param Msr   The model specific register to write
/// @param Value The value to write to the model specific register
/// @return The value of the model specific register
EXTERN
UINT64
EFIAPI
WriteMsr (
  IN UINT32 Msr,
  IN UINT64 Value
);
// EfiCpuid
/// Call CPUID instruction
/// @param Function The function to execute
/// @param Eax      On output, the register returned
/// @param Ebx      On output, the register returned
/// @param Ecx      On output, the register returned
/// @param Edx      On output, the register returned
EXTERN
VOID
EFIAPI
EfiCpuid (
  IN  UINT32  Function,
  OUT UINT32 *Eax OPTIONAL,
  OUT UINT32 *Ebx OPTIONAL,
  OUT UINT32 *Ecx OPTIONAL,
  OUT UINT32 *Edx OPTIONAL
);
// EfiCpuidEx
/// Call CPUID instruction
/// @param Function    The function to execute
/// @param SubFunction The sub function to execute
/// @param Eax         On output, the register returned
/// @param Ebx         On output, the register returned
/// @param Ecx         On output, the register returned
/// @param Edx         On output, the register returned
EXTERN
VOID
EFIAPI
EfiCpuidEx (
  IN  UINT32  Function,
  IN  UINT32  SubFunction,
  OUT UINT32 *Eax OPTIONAL,
  OUT UINT32 *Ebx OPTIONAL,
  OUT UINT32 *Ecx OPTIONAL,
  OUT UINT32 *Edx OPTIONAL
);
// EfiGetTaskRegister
/// Store local descriptor table
/// @param Descriptor On output, the task register
EXTERN
VOID
EFIAPI
EfiGetTaskRegister (
  OUT UINT8 Descriptor[sizeof(UINT16)]
);
// EfiGetLocalDescriptorTable
/// Store local descriptor table
/// @param Descriptor On output, the table descriptor offset
EXTERN
VOID
EFIAPI
EfiGetLocalDescriptorTable (
  OUT UINT8 Descriptor[sizeof(UINT16)]
);
// EfiGetInterruptDescriptorTable
/// Store interrupt descriptor table
/// @param DescriptorAndAddress On output, the descriptor offset is in the low two bytes and the interrupt descriptor table address is in the high four bytes
EXTERN
VOID
EFIAPI
EfiGetInterruptDescriptorTable (
  OUT UINT8 DescriptorAndAddress[sizeof(UINT16) + sizeof(UINT32)]
);
// EfiGetGlobalDescriptorTable
/// Store global descriptor table
/// @param DescriptorAndAddress On output, the descriptor offset is in the low two bytes and the global descriptor table address is in the high four bytes
EXTERN
VOID
EFIAPI
EfiGetGlobalDescriptorTable (
  OUT UINT8 DescriptorAndAddress[sizeof(UINT16) + sizeof(UINT32)]
);

#endif // EFI_ARCH_IA32 || EFI_ARCH_X64

// EfiCpuPause
/// Pause the CPU for an idle cycle
EXTERN
VOID
EFIAPI
EfiCpuPause (
  VOID
);
// EfiCompareAndExchange32
/// Compare and exchange a 32bit value atomically 
/// @param Value         On output, the set or unchanged value
/// @param CompareValue  The value in which to compare to set the value if equal
/// @param ExchangeValue The value to set
/// @return The initial value
EXTERN
UINT64
EFIAPI
EfiCompareAndExchange32 (
  IN OUT UINT32 *Value,
  IN     UINT32  CompareValue,
  IN     UINT32  ExchangeValue
);
// EfiCompareAndExchange64
/// Compare and exchange a 64bit value atomically 
/// @param Value         On output, the set or unchanged value
/// @param CompareValue  The value in which to compare to set the value if equal
/// @param ExchangeValue The value to set
/// @return The initial value
EXTERN
UINT64
EFIAPI
EfiCompareAndExchange64 (
  IN OUT UINT64 *Value,
  IN     UINT64  CompareValue,
  IN     UINT64  ExchangeValue
);
// EfiCompareAndExchange
/// Compare and exchange a native bit width value atomically 
/// @param Value         On output, the set or unchanged value
/// @param CompareValue  The value in which to compare to set the value if equal
/// @param ExchangeValue The value to set
/// @return The initial value
# if EFI_ARCH_BITS == 64
#  define EfiCompareAndExchange EfiCompareAndExchange64
# else
#  define EfiCompareAndExchange EfiCompareAndExchange32
# endif

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_INTRINSICS_HEADER__
