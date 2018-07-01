///
/// @file Library/Cpu/Intel.c
///
/// Intel specific CPU features
///

#pragma once
#ifndef __INTEL_CPU_HEADER__
#define __INTEL_CPU_HEADER__

#include <Cpu/Cpu.h>

// CPU_TYPE_INTEL_SIGNATURE
/// The Intel CPU type signature
#define CPU_TYPE_INTEL_SIGNATURE "GenuineIntel"

// IntelCpuGetFeatures
/// Get CPU features
/// @param Features On output, the CPU features
/// @retval EFI_INVALID_PARAMETER If Features is NULL
/// @retval EFI_SUCCESS           If the CPU features were returned successfully
EXTERN
EFI_STATUS
EFIAPI
IntelCpuGetFeatures (
  OUT CPU_FEATURES *Features
);
// CpuGetNumberOfProcessors
/// Get the number of processors
/// @param NumberOfLogicalProcessors  On output, the number of logical processors
/// @param NumberOfPhysicalProcessors On output, the number of physical processors
/// @param NumberOfPackages           On output, the number of physical packages
/// @retval EFI_INVALID_PARAMETER If NumberOfLogicalProcessors, NumberOfPhysicalProcessors, or NumberOfPackages is NULL
/// @retval EFI_SUCCEESS          The number of processors was returned successfully
EXTERN
EFI_STATUS
EFIAPI
IntelCpuGetNumberOfProcessors (
  OUT UINTN *NumberOfLogicalProcessors,
  OUT UINTN *NumberOfPhysicalProcessors,
  OUT UINTN *NumberOfPackages
);

#endif // __INTEL_CPU_HEADER__
