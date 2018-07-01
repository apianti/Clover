///
/// @file Include/Uefi/Protocol/Log.h
///
/// Logging protocol
///

#pragma once
#ifndef __UEFI_LOG_HEADER__
#define __UEFI_LOG_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_LOG_PROTOCOL_GUID
/// The logging protocol unique identifier
#define EFI_LOG_PROTOCOL_GUID { 0x056FC259, 0x400B, 0x443E, { 0x92, 0x65, 0x1B, 0x19, 0xDB, 0xFC, 0x0B, 0x5F } }

// EFI_LOG_OUTPUT_NONE
/// Disable outputs
#define EFI_LOG_OUTPUT_NONE 0
// EFI_LOG_OUTPUT_CONSOLE
/// Output to the console
#define EFI_LOG_OUTPUT_CONSOLE EFI_BIT(0)
// EFI_LOG_OUTPUT_FILE
/// Output to the log file
#define EFI_LOG_OUTPUT_FILE EFI_BIT(1)
// EFI_LOG_OUTPUT_SERIAL
/// Output to serial port
#define EFI_LOG_OUTPUT_SERIAL EFI_BIT(2)
// EFI_LOG_OUTPUT_ALL
/// Output to all log outputs
#if defined(EFI_SERIAL_DISABLE)
# define EFI_LOG_OUTPUT_ALL (EFI_LOG_OUTPUT_CONSOLE | EFI_LOG_OUTPUT_FILE)
#else
# define EFI_LOG_OUTPUT_ALL (EFI_LOG_OUTPUT_CONSOLE | EFI_LOG_OUTPUT_FILE | EFI_LOG_OUTPUT_SERIAL)
#endif

// EFI_LOG_VERBOSITY_NONE
/// Normal logging
#define EFI_LOG_VERBOSITY_NONE 0
// EFI_LOG_VERBOSITY_VERBOSE
/// Verbose logging
#define EFI_LOG_VERBOSITY_VERBOSE 1
// EFI_LOG_VERBOSITY_EXTRA_VERBOSE
/// Extra verbose logging
#define EFI_LOG_VERBOSITY_EXTRA_VERBOSE 2

// EFI_LOG_DIVIDER
/// The log divider line
#define EFI_LOG_DIVIDER L"=========================================================================================================================\n"

// LOG
/// Log at no verbosity
#define LOG(...) EfiLog(__VA_ARGS__)
// VERBOSE
/// Log at verbose
#define VERBOSE(...) EfiVerbose(EFI_LOG_VERBOSITY_VERBOSE, ## __VA_ARGS__)
// WHISPER
/// Log at extra verbose
#define WHISPER(...) EfiVerbose(EFI_LOG_VERBOSITY_EXTRA_VERBOSE, ## __VA_ARGS__)
// LOGDIV
/// Log a divider line
#define LOGDIV() LOG(EFI_LOG_DIVIDER);
// VERBOSEDIV
/// Log a verbose divider line
#define VERBOSEDIV() VERBOSE(EFI_LOG_DIVIDER);
// WHISPERDIV
/// Log an extra verbose divider line
#define WHISPERDIV() WHISPER(EFI_LOG_DIVIDER);

// mEfiLogProtocolGuid
/// The logging protocol unique identifier
EXTERN EFI_GUID gEfiLogProtocolGuid;

// EFI_LOG_PROTOCOL
/// Logging protocol
typedef struct EFI_LOG_PROTOCOL EFI_LOG_PROTOCOL;

// EFI_LOG_VLOG
/// Print a formatted character string to log
/// @param This   The logging protocol interface
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
typedef
UINTN
(EFIAPI
*EFI_LOG_VLOG) (
  IN EFI_LOG_PROTOCOL *This,
  IN CONST CHAR16     *Format,
  IN VA_LIST           Marker
);
// EFI_LOG_VVERBOSE
/// Print a formatted character string to log with verbosity
/// @param This      The logging protocol interface
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param Marker    The arguments to format
/// @return The number of characters printed
typedef
UINTN
(EFIAPI
*EFI_LOG_VVERBOSE) (
  IN EFI_LOG_PROTOCOL *This,
  IN UINT32            Verbosity,
  IN CONST CHAR16     *Format,
  IN VA_LIST           Marker
);
// EFI_LOG_GET_CONTENTS
/// Get the log contents
/// @param This The logging protocol interface
/// @return The log contents
typedef
CONST CHAR16 *
(EFIAPI
*EFI_LOG_GET_CONTENTS) (
  IN EFI_LOG_PROTOCOL *This
);
// EFI_LOG_CLEAR
/// Clear the log contents
/// @param This The logging protocol interface
typedef
VOID
(EFIAPI
*EFI_LOG_CLEAR) (
  IN EFI_LOG_PROTOCOL *This
);
// EFI_LOG_GET_OUTPUTS
/// Get the current enabled log outputs
/// @param This The logging protocol interface
/// @return The current enabled log outputs
typedef
UINT32
(EFIAPI
*EFI_LOG_GET_OUTPUTS) (
  IN EFI_LOG_PROTOCOL *This
);
// EFI_LOG_SET_OUTPUTS
/// Enable or disable log outpute
/// @param This   The logging protocol interface
/// @param Output The outputs that should be enabled, otherwise disabled
/// @param Flush  Whether to output the previous log now
typedef
BOOLEAN
(EFIAPI
*EFI_LOG_SET_OUTPUTS) (
  IN EFI_LOG_PROTOCOL *This,
  IN UINT32            Outputs,
  IN BOOLEAN           Flush
);
// EFI_LOG_GET_FILE
/// Get the console output file path
/// @param This The logging protocol interface
/// @return The log output file path
typedef
CONST CHAR16 *
(EFIAPI
*EFI_LOG_GET_FILE) (
  IN EFI_LOG_PROTOCOL *This
);
// EFI_LOG_SET_FILE
/// Set the console output file path
/// @param This    The logging protocol interface
/// @param LogPath The log output file path
typedef
EFI_STATUS
(EFIAPI
*EFI_LOG_SET_FILE) (
  IN EFI_LOG_PROTOCOL *This,
  IN CONST CHAR16     *LogPath
);
// EFI_LOG_GET_VERBOSITY
/// Get the current log verbosity level
/// @param This The logging protocol interface
/// @return The current log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
typedef
UINT32
(EFIAPI
*EFI_LOG_GET_VERBOSITY) (
  IN EFI_LOG_PROTOCOL *This
);
// EFI_LOG_SET_VERBOSITY
/// Set the log verbosity level
/// @param This      The logging protocol interface
/// @param Verbosity The log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
typedef
BOOLEAN
(EFIAPI
*EFI_LOG_SET_VERBOSITY) (
  IN EFI_LOG_PROTOCOL *This,
  IN UINT32            Verbosity
);

// EFI_LOG_PROTOCOL
/// Logging protocol
struct EFI_LOG_PROTOCOL {

  // VLog
  /// Print a formatted character string to log
  EFI_LOG_VLOG          VLog;
  // VVerbose
  /// Print a formatted character string to log with verbosity
  EFI_LOG_VVERBOSE      VVerbose;
  // GetContents
  /// Get the log contents
  EFI_LOG_GET_CONTENTS  GetContents;
  // Clear
  /// Clear the log contents
  EFI_LOG_CLEAR         Clear;
  // GetOutputs
  /// Get the current enabled log outputs
  EFI_LOG_GET_OUTPUTS   GetOutputs;
  // SetOutputs
  /// Enable or disable log outpute
  EFI_LOG_SET_OUTPUTS   SetOutputs;
  // GetFile
  /// Get the console output file path
  EFI_LOG_GET_FILE      GetFile;
  // SetFile
  /// Set the console output file path
  EFI_LOG_SET_FILE      SetFile;
  // GetVerbosity
  /// Get the current log verbosity level
  EFI_LOG_GET_VERBOSITY GetVerbosity;
  // SetVerbosity
  /// Set the log verbosity level
  EFI_LOG_SET_VERBOSITY SetVerbosity;

};

// EfiLog
/// Print a formatted character string to log
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EXTERN
UINTN
EFIAPI
EfiLog (
  CONST CHAR16 *Format,
  ...
);
// EfiVLog
/// Print a formatted character string to log
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
EXTERN
UINTN
EFIAPI
EfiVLog (
  CONST CHAR16 *Format,
  VA_LIST       Marker
);

// EfiVerbose
/// Print a formatted character string to log with verbosity
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param ...       The arguments to format
/// @return The number of characters printed
EXTERN
UINTN
EFIAPI
EfiVerbose (
  UINT32        Verbosity,
  CONST CHAR16 *Format,
  ...
);
// EfiVVerbose
/// Print a formatted character string to log with verbosity
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param Marker    The arguments to format
/// @return The number of characters printed
EXTERN
UINTN
EFIAPI
EfiVVerbose (
  UINT32        Verbosity,
  CONST CHAR16 *Format,
  VA_LIST       Marker
);

// EfiLogGetContents
/// Get the log contents
/// @return The log contents
EXTERN
CONST CHAR16 *
EFIAPI
EfiLogGetContents (
  VOID
);

// EfiLogClear
/// Clear the log contents
EXTERN
VOID
EFIAPI
EfiLogClear (
  VOID
);

// EfiLogGetDefaultOutputs
/// Get the default log outputs
/// @return The default log outputs
EXTERN
UINT32
EFIAPI
EfiLogGetDefaultOutputs (
  VOID
);
// EfiLogGetOutputs
/// Get the current enabled log outputs
/// @return The current enabled log outputs
EXTERN
UINT32
EFIAPI
EfiLogGetOutputs (
  VOID
);
// EfiLogSetOutputs
/// Enable or disable log outpute
/// @param Outputs The outputs that should be enabled, otherwise disabled
/// @param Flush   Whether to output the previous log now
EXTERN
BOOLEAN
EFIAPI
EfiLogSetOutputs (
  UINT32  Outputs,
  BOOLEAN Flush
);

// EfiLogGetFile
/// Get the log output file path
/// @return The log output file path
EXTERN
CONST CHAR16 *
EFIAPI
EfiLogGetFile (
  VOID
);
// EfiLogSetFile
/// Set the log output file path
/// @param LogPath The log output file path
EXTERN
EFI_STATUS
EFIAPI
EfiLogSetFile (
  CONST CHAR16 *LogPath
);

// EfiLogGetVerbosity
/// Get the current log verbosity level
/// @return The current log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
EXTERN
UINT32
EFIAPI
EfiLogGetVerbosity (
  VOID
);
// EfiLogGetVerbosity
/// Set the log verbosity level
/// @param Verbosity The log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
EXTERN
BOOLEAN
EFIAPI
EfiLogSetVerbosity (
  IN UINT32 Verbosity
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_LOG_HEADER__
