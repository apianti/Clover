///
/// @file Library/Cpu/Amd.c
///
/// AMD specific CPU features
///

#pragma once
#ifndef __AMD_CPU_HEADER__
#define __AMD_CPU_HEADER__

#include <Cpu/Cpu.h>

// CPU_TYPE_AMD_SIGNATURE
/// The AMD CPU type signature
#define CPU_TYPE_AMD_SIGNATURE "AuthenticAMD"

// AmdCpuGetFeatures
/// Get CPU features
/// @param Features On output, the CPU features
/// @retval EFI_INVALID_PARAMETER If Features is NULL
/// @retval EFI_SUCCESS           If the CPU features were returned successfully
EXTERN
EFI_STATUS
EFIAPI
AmdCpuGetFeatures (
  OUT CPU_FEATURES *Features
);
// AmdCpuGetNumberOfProcessors
/// Get the number of processors
/// @param NumberOfLogicalProcessors  On output, the number of logical processors
/// @param NumberOfPhysicalProcessors On output, the number of physical processors
/// @param NumberOfPackages           On output, the number of physical packages
/// @retval EFI_INVALID_PARAMETER If NumberOfLogicalProcessors, NumberOfPhysicalProcessors, or NumberOfPackages is NULL
/// @retval EFI_SUCCEESS          The number of processors was returned successfully
EXTERN
EFI_STATUS
EFIAPI
AmdCpuGetNumberOfProcessors (
  OUT UINTN *NumberOfLogicalProcessors,
  OUT UINTN *NumberOfPhysicalProcessors,
  OUT UINTN *NumberOfPackages
);

#endif // __ARM_CPU_HEADER__
