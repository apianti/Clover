///
/// @file Installer/Linux/Platform.c
///
/// Platform entry point
///

#include "Installer.h"

// InstallerPlatformEntryPoint
/// The platform specific entry point
/// @param ArgC The count of argument strings
/// @param ArgV The argument strings
/// @return Whether the installation finished successfully or not
INT32
EFIAPI
InstallerPlatformEntryPoint (
  IN INT32   ArgC,
  IN CHAR8 **ArgV
) {
  UNUSED_PARAMETER(ArgC);
  UNUSED_PARAMETER(ArgV);
  return EFI_UNSUPPORTED;
}
