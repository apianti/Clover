///
/// @file Library/Uefi/VirtualMachine.c
///
/// Virtual machine environment utilities
///

#include <Uefi.h>

// EfiIsVirtualMachine
/// Detect virtual machine environment
/// @return Whether the environment is a virtual machine not
BOOLEAN
EFIAPI
EfiIsVirtualMachine (
  VOID
) {
  STATIC BOOLEAN NotDetected = TRUE;
  STATIC BOOLEAN IsVirtualMachine = FALSE;
  // Check if the detection has already occured
  if (NotDetected) {
    // Detect virtual machine environment with MSVC
#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)
    // Detect virtual machine environment with MSVC for IA32 and X64 architectures
    UINT8 GDTR[sizeof(UINT32) + sizeof(UINT16)];
    UINT8 IDTR[sizeof(UINT32) + sizeof(UINT16)];
# if defined(EFI_DEBUG)
    UINTN Index;
# endif
    // Check the firmware string
    if ((gEfiSystemTable != NULL) && (gEfiSystemTable->FirmwareVendor != NULL)) {
      // VMware is easiest virtual machine to detect by just checking firmware vendor string
      if (StriCmp(gEfiSystemTable->FirmwareVendor, L"VMware, Inc.") == 0) {
        IsVirtualMachine = TRUE;
      }
    }
    // Get the GDTR
    EfiZeroMem(GDTR, sizeof(GDTR));
    EfiGetGlobalDescriptorTable(GDTR);
# if defined(EFI_DEBUG)
    // Output the GDTR
    VERBOSE(L"GDTR:");
    for (Index = 0; Index < sizeof(GDTR); ++Index) {
      VERBOSE(L" 0x%02x", (UINTN)GDTR[Index]);
    }
    VERBOSE(L"\n");
# endif
    // Get the IDTR
    EfiZeroMem(IDTR, sizeof(IDTR));
    EfiGetInterruptDescriptorTable(IDTR);
# if defined(EFI_DEBUG)
    // Output the IDTR
    VERBOSE(L"IDTR:");
    for (Index = 0; Index < sizeof(IDTR); ++Index) {
      VERBOSE(L" 0x%02x", (UINTN)IDTR[Index]);
    }
    VERBOSE(L"\n");
# endif
    // Check the GDTR and IDTR to determine if virtual machine environment
    if ((IDTR[4] != GDTR[4]) || (IDTR[5] != GDTR[5])) {
      IsVirtualMachine = TRUE;
    }
#elif defined(EFI_ARCH_ARM)
    // TODO: Detect virtual machine environment for ARM architecture
#elif defined(EFI_ARCH_AA64)
    // TODO: Detect virtual machine environment for ARM64 architecture
#endif
    // Detection has taken place so there is no need to do again if recalled
    NotDetected = FALSE;
  }
  // Return result of virtual machine environment detection
  return EfiConfigurationGetBoolean(L"/VirtualMachine", IsVirtualMachine);
}
