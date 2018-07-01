///
/// @file Installer/Installer.h
///
/// Platform agnostic installer application
///


#pragma once
#ifndef __INSTALLER_HEADER__
#define __INSTALLER_HEADER__

#include <Uefi.h>

// INSTALLER_DEFAULT_LOG_PATH
/// The default installer log file path
#define INSTALLER_DEFAULT_LOG_PATH EFI_PROJECT_NAME L"-Installer-" EFI_PROJECT_VERSION L".log"

// INSTALLER_SETTINGS
/// Internal settings for the installer
typedef struct INSTALLER_SETTINGS INSTALLER_SETTINGS;
struct INSTALLER_SETTINGS {

  // LogFilePath
  /// The log file path
  CONST CHAR16 *LogFilePath;
  // AllocateConsole
  /// Whether to allocate a console if needed
  BOOLEAN       AllocateConsole;

};

// gInstallerSettings
/// The internal settings for the installer
EXTERN INSTALLER_SETTINGS gInstallerSettings;

// InstallerLog
/// Log to outputs
/// @param Format The format specifier
/// @param ...    The format arguments
EXTERN
VOID
EFIAPI
InstallerLog (
  IN CONST CHAR16 *Format,
  ...
);

// InstallerPlatformSpecificActions
/// Perform platform specific actions after the arguments have been parsed
EXTERN
VOID
EFIAPI
InstallerPlatformSpecificActions (
  VOID
);

// InstallerEntryPoint
/// The install entry point
/// @param ArgC The count of argument strings
/// @param ArgV The argument strings
/// @retval EFI_SUCCESS The installer was successful
EXTERN
EFI_STATUS
EFIAPI
InstallerEntryPoint (
  IN UINTN          ArgC,
  IN CONST CHAR16 **ArgV
);

#endif // __INSTALLER_HEADER__
