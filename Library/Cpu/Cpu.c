///
/// @file Library/Cpu/Cpu.c
///
/// CPU features
///

#include <Cpu/Cpu.h>

#include "Amd.h"
#include "Intel.h"

// CpuIsMultiprocessor
/// Check whether there is more than one processor available
BOOLEAN
EFIAPI
CpuIsMultiprocessor (
  VOID
) {
  UINTN NumberOfProcessors = 0;
  UINTN NumberOfEnabledProcessors = 0;
  // Get the number of logical processors to determine multiprocessor
  if (EFI_ERROR(EfiMpServicesGetNumberOfProcessors(&NumberOfProcessors, &NumberOfEnabledProcessors))) {
    return FALSE;
  }
  // Less than or equal to one processor is not multiprocessor
  if (NumberOfProcessors <= 1) {
    return FALSE;
  }
  // Is multiprocessor CPU(s)
  return TRUE;
}

// CpuGetType
/// Get the cpu type
/// @return The CPU type
EXTERN
CPU_TYPE
EFIAPI
CpuGetType (
  VOID
) {
  STATIC CPU_TYPE Type = CpuTypeUnknown;

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  // IA32 and X64 use CPUID instruction to get cpu type
  CHAR8 Signature[13];
  // Get the signature from CPUID instruction
  EfiZeroArray(CHAR8, Signature, sizeof(Signature));
  EfiCpuid(0, NULL, (UINT32 *)(Signature), (UINT32 *)(Signature + 8), (UINT32 *)(Signature + 4));
  if (AsciiStrCmp(Signature, CPU_TYPE_INTEL_SIGNATURE) == 0) {
    // Intel signature
    Type = CpuTypeIntel;
  } else if (AsciiStrCmp(Signature, CPU_TYPE_AMD_SIGNATURE) == 0) {
    // AMD signature
    Type = CpuTypeAmd;
  }

#elif defined(EFI_ARM_)

  // TODO: Get ARM and ARM64 architecture CPU type

#endif

  return Type;
}
// CpuGetSignature
/// Get the CPU signature
/// @param Family   On output, the CPU family
/// @param Model    On output, the CPU model
/// @param Stepping On output, the CPU stepping
/// @return The CPUID signature
UINT32
EFIAPI
CpuGetSignature (
  OUT UINT16 *Family OPTIONAL,
  OUT UINT8  *Model OPTIONAL,
  OUT UINT8  *Stepping OPTIONAL
) {
  UINT32 Signature = 0;
  UINT16 DecodedFamily = 0;
  UINT8  DecodedModel = 0;
  UINT8  DecodedStepping = 0;

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  UINT32 MaxFunction = 0;
  // Get the maximum function which should be more than 1 at least
  EfiCpuid(0, &MaxFunction, NULL, NULL, NULL);
  if (MaxFunction >= 1) {

    // Get the CPU signature
    EfiCpuid(1, &Signature, NULL, NULL, NULL);
    // Decode the family
    DecodedFamily = (UINT16)EFI_BITFIELD(Signature, 8, 11);
    if (DecodedFamily == 0xF) {
      DecodedFamily += (UINT16)EFI_BITFIELD(Signature, 20, 27);
    }
    // Decode the model
    DecodedModel = (UINT8)EFI_BITFIELD(Signature, 4, 7);
    if ((DecodedFamily == 0x6) || (DecodedFamily >= 0xF)) {
      DecodedModel |= (UINT8)EFI_BITFIELDSHIFTRIGHT(Signature, 12, 16, 19);
    }
    // Decode the model stepping
    DecodedStepping = EFI_BITFIELD(Signature, 0, 3);
  }

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: Get ARM and ARM64 CPU signature


#endif

  // Return the decoded family, model, and stepping
  if (Family != NULL) {
    *Family = DecodedFamily;
  }
  if (Model != NULL) {
    *Model = DecodedModel;
  }
  if (Stepping != NULL) {
    *Stepping = DecodedStepping;
  }
  // Return the signature
  return Signature;
}
// CpuGetFamily
/// Get the CPU amily
/// @return The CPU family
UINT16
EFIAPI
CpuGetFamily (
  VOID
) {
  UINT16 Family = 0;
  CpuGetSignature(&Family, NULL, NULL);
  return Family;
}
// CpuGetModel
/// Get the CPU model
/// @return The CPU model
UINT8
EFIAPI
CpuGetModel (
  VOID
) {
  UINT8 Model = 0;
  CpuGetSignature(NULL, &Model, NULL);
  return Model;
}
// CpuGetStepping
/// Get the CPU stepping
/// @return The CPU stepping
UINT8
EFIAPI
CpuGetStepping (
  VOID
) {
  UINT8 Stepping = 0;
  CpuGetSignature(NULL, NULL, &Stepping);
  return Stepping;
}
// CpuGetDescription
/// Get CPU description
/// @return The description of the CPU which must be freed
CHAR16 *
EFIAPI
CpuGetDescription (
  VOID
) {
  CHAR16 *String = NULL;

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  // IA32 and X64 use CPUID instruction to get description
  UINT32 *Ptr;
  UINTN   Index;
  UINT32  MaxFunction = 0;
  UINT32  Function = 0x80000000;
  CHAR8   Buffer[49];
  EfiZeroArray(CHAR8, Buffer, sizeof(Buffer));
  // Check if processor description string support
  EfiCpuid(Function, &MaxFunction, NULL, NULL, NULL);
  if (MaxFunction >= 0x80000004) {
    // Get the string parts
    Ptr = (UINT32 *)Buffer;
    Function = 0x80000002;
    do {
      // Get each chunk of the description
      EfiCpuid(Function, Ptr, Ptr + 1, Ptr + 2, Ptr + 3);
      Ptr += 4;
    } while (++Function <= 0x80000004);
    // Remove extraneous spaces from end of description
    for (Index = sizeof(Buffer) - 2; Index > 0; --Index) {
      if (Buffer[Index] == ' ') {
        Buffer[Index] = 0;
      } else {
        break;
      }
    }
    // Remove extraneous spaces from beginning of description
    for (Index = 0; Index < sizeof(Buffer); ++Index) {
      if (Buffer[Index] != ' ') {
        break;
      }
    }
    // Convert the description buffer
    String = StrFromAscii(Buffer + Index);
  }

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_ARM64)

  // TODO: Get ARM and ARM64 CPU description

#endif

  // Check that the description string is valid
  if ((String == NULL) || (*String == 0)) {
    if (String != NULL) {
      EfiFreePool(String);
    }
    // Set unknown cpu description
    String = StrDup(L"Unknown CPU");
  }
  return String;
}
// CpuGetFeatures
/// Get CPU features
/// @param Features On output, the CPU features
/// @retval EFI_INVALID_PARAMETER If Features is NULL
/// @retval EFI_SUCCESS           If the CPU features were returned successfully
EFI_STATUS
EFIAPI
CpuGetFeatures (
  OUT CPU_FEATURES *Features
) {
  EFI_STATUS Status = EFI_UNSUPPORTED;
  // Check parameters
  if (Features == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Zero features
  EfiZero(CPU_FEATURES, Features);

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  // Determine the CPU type
  switch (CpuGetType()) {
    case CpuTypeIntel:
      // Get features for Intel CPU
      Status = IntelCpuGetFeatures(Features);
      break;

    case CpuTypeAmd:
      // Get features for AMD CPU
      Status = AmdCpuGetFeatures(Features);
      break;

    case CpuTypeUnknown:
    default:
      // Unsupported or unknown CPU
      break;
  }

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: Get ARM and ARM64 CPU features

#endif

  return Status;
}

// CpuGetNumberOfProcessors
/// Get the number of processors
/// @param NumberOfLogicalProcessors  On output, the number of logical processors
/// @param NumberOfPhysicalProcessors On output, the number of physical processors
/// @param NumberOfPackages           On output, the number of packages
/// @retval EFI_INVALID_PARAMETER If NumberOfLogicalProcessors, NumberOfPhysicalProcessors, and NumberOfPackages are all NULL
/// @retval EFI_SUCCEESS          If the number of processors was returned successfully
EFI_STATUS
EFIAPI
CpuGetNumberOfProcessors (
  OUT UINTN *NumberOfLogicalProcessors OPTIONAL,
  OUT UINTN *NumberOfPhysicalProcessors OPTIONAL,
  OUT UINTN *NumberOfPackages OPTIONAL
) {
  EFI_STATUS Status = EFI_UNSUPPORTED;
  UINTN      Packages;
  UINTN      LogicalProcessors;
  UINTN      PhysicalProcessors;
  UINTN      Index;
  // Check parameters
  if ((NumberOfLogicalProcessors == NULL) && (NumberOfPhysicalProcessors == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Attempt to use multiprocessor protocol
  Packages = 0;
  LogicalProcessors = 0;
  PhysicalProcessors = 0;
  Status = EfiMpServicesGetNumberOfProcessors(&LogicalProcessors, NULL);
  if (!EFI_ERROR(Status)) {
    for (Index = 0; Index < LogicalProcessors; ++Index) {
      EFI_PROCESSOR_INFORMATION ProcessorInfo;
      EfiZero(EFI_PROCESSOR_INFORMATION, &ProcessorInfo);
      Status = EfiMpServicesGetProcessorInfo(Index, &ProcessorInfo);
      if (EFI_ERROR(Status)) {
        break;
      }
      VERBOSE(L"Found CPU%03lu at %u:%u:%u", ProcessorInfo.ProcessorId, (UINTN)(ProcessorInfo.Location.Package), (UINTN)(ProcessorInfo.Location.Core), (UINTN)(ProcessorInfo.Location.Thread));
      if (ProcessorInfo.StatusFlag != 0) {
        if (EFI_BITS_ANY_SET(ProcessorInfo.StatusFlag, PROCESSOR_AS_BSP_BIT)) {
          VERBOSE(L", Boot services");
        }
        if (EFI_BITS_ANY_SET(ProcessorInfo.StatusFlag, PROCESSOR_ENABLED_BIT)) {
          VERBOSE(L", Enabled");
        }
        if (EFI_BITS_ANY_SET(ProcessorInfo.StatusFlag, PROCESSOR_HEALTH_STATUS_BIT)) {
          VERBOSE(L", Healthy");
        }
      }
      VERBOSE(L"\n");
      // Get the count of packages
      if (Packages <= ProcessorInfo.Location.Package) {
        Packages = ProcessorInfo.Location.Package + 1;
      }
      // Get the count of physical cores
      if (PhysicalProcessors <= ProcessorInfo.Location.Core) {
        PhysicalProcessors = ProcessorInfo.Location.Core + 1;
      }
    }
    // Was able to determine all the processor information by the multiprocessor services protocol
    if ((Index >= LogicalProcessors) && (LogicalProcessors != 0) && (PhysicalProcessors != 0) && (Packages != 0)) {
      // If there is more than one package put the correct amount of total physical processors
      if (Packages > 1) {
        PhysicalProcessors *= Packages;
      }
      // Return processor counts
      if (NumberOfLogicalProcessors != NULL) {
        *NumberOfLogicalProcessors = LogicalProcessors;
      }
      if (NumberOfPhysicalProcessors != NULL) {
        *NumberOfPhysicalProcessors = PhysicalProcessors;
      }
      if (NumberOfPackages != NULL) {
        *NumberOfPackages = Packages;
      }
      return Status;
    }
  }

  // Determine the CPU type
  switch (CpuGetType()) {

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

    case CpuTypeIntel:
      // Get processor count for Intel CPU
      Status = IntelCpuGetNumberOfProcessors(&LogicalProcessors, &PhysicalProcessors, &Packages);
      break;

    case CpuTypeAmd:
      // Get processor count for AMD CPU
      Status = AmdCpuGetNumberOfProcessors(&LogicalProcessors, &PhysicalProcessors, &Packages);
      break;

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: Get ARM and ARM64 CPU processor count

#endif

    case CpuTypeUnknown:
    default:
      // Unsupported or unknown CPU
      break;
  }

  // Check there is at least one processor
  if (LogicalProcessors < 1) {
    LogicalProcessors = 1;
  }
  if (PhysicalProcessors < 1) {
    PhysicalProcessors = 1;
  }
  if (Packages < 1) {
    Packages = 1;
  }
  // Return processor counts
  if (NumberOfLogicalProcessors != NULL) {
    *NumberOfLogicalProcessors = LogicalProcessors;
  }
  if (NumberOfPhysicalProcessors != NULL) {
    *NumberOfPhysicalProcessors = PhysicalProcessors;
  }
  if (NumberOfPackages != NULL) {
    *NumberOfPackages = Packages;
  }
  return Status;
}
