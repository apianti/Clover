///
/// @file Installer/Installer.c
///
/// Platform agnostic installer application
///

#include "Installer.h"

#include <wchar.h>
#include <string.h>

// gInstallerSettings
/// The internal settings for the installer
INSTALLER_SETTINGS gInstallerSettings = {

  // LogFilePath
  INSTALLER_DEFAULT_LOG_PATH,
  // AllocateConsole
  FALSE,

};

// InstallerParseArguments
/// Parse installer arguments
STATIC
VOID
EFIAPI
InstallerParseArguments (
  IN UINTN          ArgC,
  IN CONST CHAR16 **ArgV
) {
  UINTN Index;
  if (ArgV != NULL) {
    // Parse the arguments
    for (Index = 0; Index < ArgC; ++Index) {
      // Skip any empty arguments
      if ((ArgV[Index] == NULL) || (*ArgV[Index] == 0)) {
        continue; 
      }
      // Check the arguments
      if ((_wcsicmp(ArgV[Index], L"-c") == 0) ||
          (_wcsicmp(ArgV[Index], L"--console") == 0)) {
        // Allocate a console for the installer log
        gInstallerSettings.AllocateConsole = TRUE;
      } else if ((_wcsicmp(ArgV[Index], L"-l") == 0) ||
                 (_wcsicmp(ArgV[Index], L"--logfile") == 0)) {
        // Specify a file path for the log file
        if (++Index < ArgC) {
          gInstallerSettings.LogFilePath = ArgV[Index];
        }
      } else if ((_wcsicmp(ArgV[Index], L"-u") == 0) ||
                 (_wcsicmp(ArgV[Index], L"--upgrade") == 0)) {
        // TODO: Upgrade previous installation
      } else if (_wcsicmp(ArgV[Index], L"--uninstall") == 0) {
        // TODO: Uninstall previous installation
      }
    }
  }
}

// InstallerEntryPoint
/// TODO: The install entry point
/// @param ArgC The count of argument strings
/// @param ArgV The argument strings
/// @retval EFI_SUCCESS The installer was successful
EFI_STATUS
EFIAPI
InstallerEntryPoint (
  IN UINTN          ArgC,
  IN CONST CHAR16 **ArgV
) {
  // Parse arguments
  InstallerParseArguments(ArgC, ArgV);
  // Perform platform specific actions configured by arguments
  InstallerPlatformSpecificActions();
  // Log the version information
  InstallerLog(EFI_PROJECT_NAME L" Boot Manager Installer\r\nVersion: " EFI_PROJECT_VERSION L"\r\nArchitecture: " EFI_PROJECT_ARCH L"\r\n");

  return EFI_UNSUPPORTED;
}
