///
/// @file Installer/Windows/Platform.c
///
/// Platform entry point for Windows
///

#include "Installer.h"

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#include <stdio.h>
#include <sys/stat.h>

// mConsoleAttached
/// Whether there is a console attached or not
STATIC BOOL  mConsoleAttached;
// mConsoleAllocated
/// Whether the console was allocated or not
STATIC BOOL  mConsoleAllocated;
// mLogFile
/// The log file handle
STATIC FILE *mLogFile;

// InstallerLog
/// Log to outputs
/// @param Format The format specifier
/// @param ...    The format arguments
VOID
EFIAPI
InstallerLog (
  IN CONST CHAR16 *Format,
  ...
) {
  DWORD    Length;
  CHAR16  *Buffer;
  va_list  Arguments;
  va_list  ArgumentsCopy;
  va_start(Arguments, Format);
  // Get the length of the string to log
  va_copy(ArgumentsCopy, Arguments);
  Length = _vscwprintf(Format, ArgumentsCopy);
  va_end(ArgumentsCopy);
  if (Length != 0) {
    // Get the size needed for the buffer including a null terminator
    Buffer = (CHAR16 *)malloc(Length * sizeof(CHAR16));
    if (Buffer != NULL) {
      // Print the arguments to the buffer
      va_copy(ArgumentsCopy, Arguments);
      if (vswprintf_s(Buffer, Length * sizeof(CHAR16), Format, ArgumentsCopy) > 0) {
        // Only log to the console if attached
        if (mConsoleAttached) {
          DWORD UnusedLength = 0;
          // Write the buffer to the standard output
          WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), Buffer, Length, &UnusedLength, NULL);
        }
        // Output to log file
        if (mLogFile != NULL) {
          fwrite(Buffer, sizeof(CHAR16), Length, mLogFile);
        }
      }
      va_end(ArgumentsCopy);
      // Free the buffer
      free(Buffer);
    }
  }
  va_end(Arguments);
}

// InstallerPlatformSpecificActions
/// Perform platform specific actions after the arguments have been parsed
VOID
EFIAPI
InstallerPlatformSpecificActions (
  VOID
) {
  // Allocate a new console if requested
  if (gInstallerSettings.AllocateConsole) {
    mConsoleAttached = mConsoleAllocated = AllocConsole();
  }
  // Use a console that is already attached if possible
  if (!mConsoleAttached) {
    // Attach to the parent console
    mConsoleAttached = AttachConsole(ATTACH_PARENT_PROCESS);
    if (mConsoleAttached) {
      // Reopen the standard output
      freopen("CON", "w", stdout);
      // Print a new line because the command prompt will print a prompt
      InstallerLog(L"\r\n");
    }
  }
  // Open log file, empty path disables
  if ((gInstallerSettings.LogFilePath != NULL) && (*(gInstallerSettings.LogFilePath) != 0)) {
    // Check if file exists already
    struct _stat Exists;   
    BOOLEAN     DoesNotExist = (_wstat(gInstallerSettings.LogFilePath, &Exists) != 0) ? TRUE : FALSE;
    // Open the file and then check if the file existed before so the byte order mark doesn't get written multiple times
    if ((_wfopen_s(&mLogFile, gInstallerSettings.LogFilePath, L"ab+") == 0) && DoesNotExist) {
      // Write a byte order mark
      fwrite(L"\xFEFF", sizeof(CHAR16), 1, mLogFile);
    }
  }
}

// WinMain
/// The Windows platform specific entry point
/// @param ArgC The count of argument strings
/// @param ArgV The argument strings
/// @return Whether the installation finished successfully or not
INT32
WINAPI
WinMain (
    IN HINSTANCE hInstance,
    IN HINSTANCE hPrevInstance,
    IN LPSTR     lpCmdLine,
    IN INT32     nCmdShow
) {
  EFI_STATUS  Status;
  INT32       ActualArgC;
  LPWSTR     *ActualArgV;
  LPWSTR      CommandLine;
  UNUSED_PARAMETER(hInstance);
  UNUSED_PARAMETER(hPrevInstance);
  UNUSED_PARAMETER(lpCmdLine);
  UNUSED_PARAMETER(nCmdShow);
  // Get the command line
  CommandLine = GetCommandLineW();
  if (CommandLine != NULL) {
    // Convert the command line to the actual arguments
    ActualArgC = 0;
    ActualArgV = CommandLineToArgvW(CommandLine, &ActualArgC);
    if ((ActualArgV == NULL) || (ActualArgC <= 0)) {
      // If there was an error then assume no arguments
      ActualArgC = 0;
    }
  } else {
    // Seems to not be a command line?
    ActualArgC = 0;
    ActualArgV = NULL;
  }
  // Call the actual entry point
  Status = InstallerEntryPoint((UINTN)ActualArgC, (CONST CHAR16 **)ActualArgV);
  // Free the command line arguments list
  if (ActualArgV != NULL) {
    LocalFree(ActualArgV);
  }
  // Close log file if needed
  if (mLogFile != NULL) {
    fclose(mLogFile);
    mLogFile = NULL;
  }
  // Free the console if needed
  if (mConsoleAttached) {
    if (mConsoleAllocated) {
      system("pause");
    }
    FreeConsole();
    mConsoleAttached = FALSE;
  }
  return EFI_ERROR(Status) ? -1 : 0;
}
