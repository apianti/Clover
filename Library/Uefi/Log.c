///
/// @file Library/Uefi/Log.c
///
/// Logging protocol
///

#include "VirtualMemory.h"

// EFI_LOG_PROTOCOL_IMPL_SIGNATURE
/// The logging protocol implementation signature
#define EFI_LOG_PROTOCOL_IMPL_SIGNATURE 0x00474F4C544E4C43

// EFI_LOG_DEFAULT_PAGE_COUNT
/// The default 4KB page count for the log (32 * 4KB = 128KB, 128 *  4KB = 512KB for debug)
#if !defined EFI_LOG_DEFAULT_PAGE_COUNT
# if defined(EFI_DEBUG)
#  define EFI_LOG_DEFAULT_PAGE_COUNT 128
# else
#  define EFI_LOG_DEFAULT_PAGE_COUNT 32
# endif
#endif

// EFI_LOG_OUTPUT_DEFAULT
/// The default log outputs upon installation
#define EFI_LOG_OUTPUT_DEFAULT EFI_LOG_OUTPUT_NONE
// EFI_LOG_VERBOSITY_DEFAULT
/// The default log verbosity
#if defined(EFI_DEBUG)
# define EFI_LOG_VERBOSITY_DEFAULT EFI_LOG_VERBOSITY_EXTRA_VERBOSE
#else
# define EFI_LOG_VERBOSITY_DEFAULT EFI_LOG_VERBOSITY_NONE
#endif

// mEfiLogProtocolGuid
/// The logging protocol unique identifier
EFI_GUID gEfiLogProtocolGuid = EFI_LOG_PROTOCOL_GUID;

// EFI_LOG_PROTOCOL_IMPL
/// Logging protocol implementation
typedef struct EFI_LOG_PROTOCOL_IMPL EFI_LOG_PROTOCOL_IMPL;
struct EFI_LOG_PROTOCOL_IMPL {

  // Protocol
  /// The logging protocol
  EFI_LOG_PROTOCOL   Protocol;

  // Signature
  /// The signature of this protocol, should be EFI_LOG_PROTOCOL_IMPL_SIGNATURE
  UINT64             Signature;
  // Log
  /// The console log of all output
  CHAR16            *Log;
  // Offset
  /// The offset in characters of the current write position of the log memory
  UINTN              Offset;
  // Size
  /// The size in characters of the log memory
  UINTN              Size;
  // File
  /// The log output file interface
  EFI_FILE_PROTOCOL *File;
  // Path
  /// The log output file path
  CHAR16            *Path;
  // Outputs
  /// The log outputs
  UINT32             Outputs;
  // Verbosity
  /// The verbose level of messages to log, messages logged above this level will be discarded, verbose level zero is never discarded
  UINT32             Verbosity;

};

// OpenOutputFile
/// Open the log output file
/// @param OutputFile     On input, the current output file or NULL, on output, the current or opened output file
/// @param OutputFilePath The output file path
/// @return Whether the output file was opened or not
STATIC
EFI_STATUS
EFIAPI
OpenOutputFile (
  IN OUT EFI_FILE_PROTOCOL **OutputFile,
  IN OUT CHAR16            **OutputFilePath
) {
  // Check output file pointer is valid
  if ((OutputFile == NULL) || (OutputFilePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check output path is valid
  if (*OutputFilePath == NULL) {
    UINTN   Length;
    CHAR16 *ImagePath;
    CHAR16 *Path;
    // Get loaded image protocol for file name
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = EfiLoadedImage(gEfiImageHandle);
    if (LoadedImage == NULL) {
      return EFI_NOT_FOUND;
    }
    // Get the path of the loaded image
    ImagePath = EfiDevicePathToText(LoadedImage->FilePath, FALSE, FALSE);
    if (ImagePath == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Get the length of the path string
    Length = StrLen(ImagePath);
    if (Length == 0) {
      EfiFreePool(ImagePath);
      return EFI_OUT_OF_RESOURCES;
    }
    // Reallocate the string to have four more characters for ".log" appended
    Path = (CHAR16 *)
#if defined(EFI_MEMORY_VIRTUAL)
      EfiInternalAllocate((Length + 5) * sizeof(CHAR16));
#else
      EfiAllocate((Length + 5) * sizeof(CHAR16));
#endif
    if (Path == NULL) {
      EfiFreePool(ImagePath);
      return EFI_OUT_OF_RESOURCES;
    }
    // Copy the image path to the output file path
    EfiCopyArray(CHAR16, Path, ImagePath, Length);
    EfiFreePool(ImagePath);
    // Append ".log"
    EfiCopyArray(CHAR16, Path + Length, L".log", 5);
    // Set the output file path
    *OutputFilePath = Path;
  }
  // Only open if not open already
  if (*OutputFile == NULL) {
    EFI_STATUS Status;
    UINT64     Offset = 0;
    Status = EfiFileOpenPath(OutputFile, *OutputFilePath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if (*OutputFile == NULL) {
      return EFI_NOT_FOUND;
    }
    // Append don't replace, so seek to end of file
    Status = EfiFileSetPosition(*OutputFile, 0xFFFFFFFFFFFFFFFF);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    // Write extra new line if there is already file contents
    if (!EFI_ERROR(EfiFileGetPosition(*OutputFile, &Offset)) && (Offset != 0)) {
      UINTN Size = 2 * sizeof(CHAR16);
      EfiFileWrite(*OutputFile, &Size, (VOID *)L"\r\n");
      EfiFileFlush(*OutputFile);
    }
  }
  // Return whether output file was opened
  return EFI_SUCCESS;
}
// OutputFilePrint
/// Output to the log output file
/// @param OutputFile     On input, the current output file or NULL, on output, the current or opened output file
/// @param OutputFilePath The output file path
/// @param Text           The text to print to the output file
/// @param Size           The size in bytes of the text
STATIC
EFI_STATUS
EFIAPI
OutputFilePrint (
  IN OUT EFI_FILE_PROTOCOL **OutputFile,
  IN OUT CHAR16            **OutputFilePath,
  IN     CONST CHAR16       *Text,
  IN     UINTN               Size
) {
  EFI_STATUS Status;
  // Open the output log file if needed
  Status = OpenOutputFile(OutputFile, OutputFilePath);
  if (!EFI_ERROR(Status)) {
    // Write to the output file
    Status = EfiFileWrite(*OutputFile, &Size, (VOID *)Text);
    if (!EFI_ERROR(Status)) {
      // Flush the output file
      Status = EfiFileFlush(*OutputFile);
    }
  }
  return Status;
}

// LogVLog
/// Print a formatted character string to log
/// @param This   The logging protocol interface
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
STATIC
UINTN
EFIAPI
LogVLog (
  IN EFI_LOG_PROTOCOL_IMPL *This,
  IN CONST CHAR16          *Format,
  IN VA_LIST                Marker
) {
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      Size;
  // Check parameters
  if ((This == NULL) || (Format == NULL) || (Marker == NULL) ||
      (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    return 0;
  }
  // Check if the log buffer exists
  if (This->Log == NULL) {
    EFI_PHYSICAL_ADDRESS Address = NULL;
    // Allocate the log buffer with the default size
    This->Offset = 1;
    This->Size = (EFI_PAGES_TO_SIZE(EFI_LOG_DEFAULT_PAGE_COUNT) / sizeof(CHAR16));
#if defined(EFI_MEMORY_VIRTUAL)
    Status = EfiInternalAllocatePages(EFI_LOG_DEFAULT_PAGE_COUNT, &Address);
#else
    Status = EfiAllocatePages(AllocateAnyPages, EFI_MEMORY_TYPE_DEFAULT_POOL, EFI_LOG_DEFAULT_PAGE_COUNT, &Address);
#endif
    if (EFI_ERROR(Status)) {
      return 0;
    }
    // Set the new buffer
    This->Log = (CHAR16 *)(UINTN)Address;
    if (This->Log == NULL) {
      // Failed to allocate the pages for log!
      return 0;
    }
    // Start the log with a UTF-16 byte order mark
    *(This->Log) = L'\xFEFF';
  }
  // Get the remaining size of the log buffer
  Size = (This->Size - This->Offset) * sizeof(CHAR16);
  // Attempt to print to the log buffer
  Status = EfiVSPrint(This->Log + This->Offset, &Size, Format, Marker);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    EFI_PHYSICAL_ADDRESS Address = NULL;
    // Get the new page count needed
    UINTN Previous = EFI_SIZE_TO_PAGES(This->Size * sizeof(CHAR16));
    UINTN Count = EFI_SIZE_TO_PAGES(Size);
    if (Count < EFI_LOG_DEFAULT_PAGE_COUNT) {
      Count = EFI_LOG_DEFAULT_PAGE_COUNT;
    }
    Count += Previous;
    // Reallocate the log buffer
#if defined(EFI_MEMORY_VIRTUAL)
    Status = EfiInternalAllocatePages(Count, &Address);
#else
    Status = EfiAllocatePages(AllocateAnyPages, EFI_MEMORY_TYPE_DEFAULT_POOL, Count, &Address);
#endif
    if (EFI_ERROR(Status)) {
      return 0;
    }
    // Copy the old buffer to the new
    EfiCopyArray(CHAR16, (UINTN)Address, This->Log, (This->Offset + 1));
    // Free the old buffer
    EfiFreePages((EFI_PHYSICAL_ADDRESS)(UINTN)(This->Log), Previous);
    // Set the new buffer
    This->Log = (CHAR16 *)(UINTN)Address;
    This->Size = (EFI_PAGES_TO_SIZE(Count) / sizeof(CHAR16));
    // Get the new remaining size of the log buffer
    Size = (This->Size - This->Offset) * sizeof(CHAR16);
    // Attempt to print to the log buffer again
    Status = EfiVSPrint(This->Log + This->Offset, &Size, Format, Marker);
  }
  if (EFI_ERROR(Status)) {
    return 0;
  }
  // Check any characters were written
  if (Size <= sizeof(CHAR16)) {
    return 0;
  }
  // Get the count of characters from the size, excluding the null terminator
  Size /= sizeof(CHAR16);
  --Size;
  // Make sure the log has a null terminator
  This->Log[This->Offset + Size] = 0;
  // Log to outputs
  if (EFI_BITS_ANY_SET(This->Outputs, EFI_LOG_OUTPUT_CONSOLE)) {
    // Log console output
    if (EFI_ERROR(EfiConOutPrint(This->Log + This->Offset))) {
      This->Outputs &= ~EFI_LOG_OUTPUT_CONSOLE;
    }
  }
  if (EFI_BITS_ANY_SET(This->Outputs, EFI_LOG_OUTPUT_FILE)) {
    // Log file output
    if (EFI_ERROR(OutputFilePrint(&(This->File), &(This->Path), This->Log + This->Offset, Size * sizeof(CHAR16)))) {
      // Disable file output on an error
      This->Outputs &= ~EFI_LOG_OUTPUT_FILE;
    }
  }
#if !defined(EFI_SERIAL_DISABLE)
  if (EFI_BITS_ANY_SET(This->Outputs, EFI_LOG_OUTPUT_SERIAL)) {
    // Log serial output
    if (EFI_ERROR(EfiSerialStrWriteAll(This->Log + This->Offset))) {
      This->Outputs &= ~EFI_LOG_OUTPUT_SERIAL;
    }
  }
#endif
  // Get the count of characters written and advance the offset
  This->Offset += Size;
  return Size;
}
// LogVVerbose
/// Print a formatted character string to log with verbosity
/// @param This      The logging protocol interface
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param Marker    The arguments to format
/// @return The number of characters printed
STATIC
UINTN
EFIAPI
LogVVerbose (
  IN EFI_LOG_PROTOCOL_IMPL *This,
  IN UINT32                 Verbosity,
  IN CONST CHAR16          *Format,
  IN VA_LIST                Marker
) {
  // Check parameters
  if ((This == NULL) || (Format == NULL) || (Marker == NULL) ||
      (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    return 0;
  }
  // Check the verbosity
  if (Verbosity > This->Verbosity) {
    return 0;
  }
  // Log the message
  return This->Protocol.VLog(&(This->Protocol), Format, Marker);
}
// LogGetContents
/// Get the log contents
/// @param This The logging protocol interface
/// @return The log contents
STATIC
CONST CHAR16 *
EFIAPI
LogGetContents (
  IN EFI_LOG_PROTOCOL_IMPL *This
) {
  return ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) ? NULL : This->Log;
}
// LogClear
/// Clear the log contents
/// @param This The logging protocol interface
STATIC
VOID
EFIAPI
LogClear (
  IN EFI_LOG_PROTOCOL_IMPL *This
) {
  if ((This != NULL) && (This->Signature == EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    if (This->Log != NULL) {
      EfiFreePages((EFI_PHYSICAL_ADDRESS)(UINTN)(This->Log), EFI_SIZE_TO_PAGES(This->Size * sizeof(CHAR16)));
      This->Log = NULL;
    }
    if (This->File != NULL) {
      EfiFileClose(This->File);
      This->File = NULL;
    }
    if (This->Path != NULL) {
      EfiFreePool(This->Path);
      This->Path = NULL;
    }
    This->Size = 0;
    This->Offset = 0;
    This->Outputs = EFI_LOG_OUTPUT_DEFAULT;
    This->Verbosity = EFI_LOG_VERBOSITY_DEFAULT;
  }
}
// LogGetOutputs
/// Get the current enabled log outputs
/// @param This The logging protocol interface
/// @return The current enabled log outputs
STATIC
UINT32
EFIAPI
LogGetOutputs (
  IN EFI_LOG_PROTOCOL_IMPL *This
) {
  return ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) ? 0 : This->Outputs;
}
// LogSetOutputs
/// Enable or disable log outpute
/// @param This    The logging protocol interface
/// @param Outputs The outputs that should be enabled, otherwise disabled
/// @param Flush   Whether to output the previous log now
STATIC
BOOLEAN
EFIAPI
LogSetOutputs (
  IN EFI_LOG_PROTOCOL_IMPL *This,
  IN UINT32                 Outputs,
  IN BOOLEAN                Flush
) {
  if ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    return FALSE;
  }
  // Check if flushing or just changing outputs
  if (Flush) {
    // Flush the outputs, there should be more than one character as the first should be the byte order mark
    if ((This->Log != NULL) && (This->Offset > 1)) {
      // Always flush the console even if already an active output
      if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_CONSOLE)) {
        // Flush file output
        EfiConOutPrint((This->Log) + 1);
      }
      // Only flush to file if not already an active output
      if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_FILE)) {
        // Flush file output
        if (EFI_ERROR(OutputFilePrint(&(This->File), &(This->Path), This->Log, This->Offset * sizeof(CHAR16)))) {
          // Disable file output on an error
          Outputs &= ~EFI_LOG_OUTPUT_FILE;
        }
      }
#if !defined(EFI_SERIAL_DISABLE)
      // Only flush to serial if not already an active output
      if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_SERIAL) && EFI_BITS_ARE_UNSET(This->Outputs, EFI_LOG_OUTPUT_SERIAL)) {
        // Flush serial output
        if (EFI_ERROR(EfiSerialStrWriteAll(This->Log))) {
          Outputs &= ~EFI_LOG_OUTPUT_SERIAL;
        }
      }
#endif
    }
  } else if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_FILE)) {
    // Open the output log file if needed
    if (EFI_ERROR(OpenOutputFile(&(This->File), &(This->Path)))) {
      Outputs &= ~EFI_LOG_OUTPUT_FILE;
    }
  }
  // Change the outputs
  This->Outputs = Outputs;
  return TRUE;
}
// LogGetFile
/// Get the log output file path
/// @param This The logging protocol interface
/// @return The log output file path
STATIC
CONST CHAR16 *
EFIAPI
LogGetFile (
  IN EFI_LOG_PROTOCOL_IMPL *This
) {
  if ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE) || EFI_BITS_ARE_UNSET(This->Outputs, EFI_LOG_OUTPUT_FILE)) {
   return  NULL;
  }
  return This->Path;
}
// LogSetFile
/// Set the log output file path
/// @param This    The logging protocol interface
/// @param LogPath The log output file path
STATIC
EFI_STATUS
EFIAPI
LogSetFile (
  IN EFI_LOG_PROTOCOL_IMPL *This,
  IN CONST CHAR16          *LogPath
) {
  EFI_STATUS Status;
  // Check parameters
  if ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Free old output file path if any
  if (This->Path != NULL) {
    EfiFreePool(This->Path);
  }
  // Duplicate the output file path
  This->Path = StrDup(LogPath);
  // Close any previous output file
  if (This->File != NULL) {
    EfiFileClose(This->File);
    This->File = NULL;
  }
  // Do not attempt to open file if output not enable
  if (EFI_BITS_ARE_UNSET(This->Outputs, EFI_LOG_OUTPUT_FILE)) {
    return EFI_SUCCESS;
  }
  // Attempt to open output file
  Status = OpenOutputFile(&(This->File), &(This->Path));
  if (EFI_ERROR(Status)) {
    // Disable the log file if something failed
    This->Outputs &= ~EFI_LOG_OUTPUT_FILE;
  }
  return Status;
}
// LogGetVerbosity
/// Get the current log verbosity level
/// @return The current log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
STATIC
UINT32
EFIAPI
LogGetVerbosity (
  IN EFI_LOG_PROTOCOL_IMPL *This
) {
  return ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE))  ? NULL : This->Verbosity;
}
// LogGetVerbosity
/// Set the log verbosity level
/// @param This    The logging protocol interface
/// @param Verbosity The log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
STATIC
BOOLEAN
EFIAPI
LogSetVerbosity (
  IN EFI_LOG_PROTOCOL_IMPL *This,
  IN UINT32                 Verbosity
) {
  if ((This == NULL) || (This->Signature != EFI_LOG_PROTOCOL_IMPL_SIGNATURE)) {
    return FALSE;
  }
  This->Verbosity = Verbosity;
  return TRUE;
}

// GetLogProtocol
/// Get the current logging protocol
STATIC
EFI_LOG_PROTOCOL *
EFIAPI
GetLogProtocol (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = NULL;
  // Check if there is already a logging protocol
  if (EFI_ERROR(EfiLocateProtocol(&gEfiLogProtocolGuid, NULL, (VOID **)&Log))) {
    // Logging protocol not found
    return NULL;
  }
  // Return the logging protocol interface
  return Log;
}

// EfiLogInstall
/// Install logging protocol
/// @retval EFI_SUCCESS The logging protocol was installed
EFI_STATUS
EFIAPI
EfiLogInstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_LOG_PROTOCOL_IMPL *Impl;
  // Check if there is already a logging protocol
  EFI_LOG_PROTOCOL      *Log = GetLogProtocol();
  if (Log != NULL) {
    return EFI_SUCCESS;
  }
  // Allocate the log protocol interface
  Impl = (EFI_LOG_PROTOCOL_IMPL *)
#if defined(EFI_MEMORY_VIRTUAL)
  EfiInternalAllocate(sizeof(EFI_LOG_PROTOCOL_IMPL));
#else
  EfiAllocate(sizeof(EFI_LOG_PROTOCOL_IMPL));
#endif
  if (Impl == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Setup the logging protocol interface
  Impl->Protocol.VLog = (EFI_LOG_VLOG)LogVLog;
  Impl->Protocol.VVerbose = (EFI_LOG_VVERBOSE)LogVVerbose;
  Impl->Protocol.GetContents = (EFI_LOG_GET_CONTENTS)LogGetContents;
  Impl->Protocol.Clear = (EFI_LOG_CLEAR)LogClear;
  Impl->Protocol.GetOutputs = (EFI_LOG_GET_OUTPUTS)LogGetOutputs;
  Impl->Protocol.SetOutputs = (EFI_LOG_SET_OUTPUTS)LogSetOutputs;
  Impl->Protocol.GetFile = (EFI_LOG_GET_FILE)LogGetFile;
  Impl->Protocol.SetFile = (EFI_LOG_SET_FILE)LogSetFile;
  Impl->Protocol.GetVerbosity = (EFI_LOG_GET_VERBOSITY)LogGetVerbosity;
  Impl->Protocol.SetVerbosity = (EFI_LOG_SET_VERBOSITY)LogSetVerbosity;
  // Setup the logging protocol implementation interface
  Impl->Signature = EFI_LOG_PROTOCOL_IMPL_SIGNATURE;
  Impl->Log = NULL;
  Impl->Offset = 0;
  Impl->Size = 0;
  Impl->File = NULL;
  Impl->Path = NULL;
  Impl->Outputs = EFI_LOG_OUTPUT_DEFAULT;
  Impl->Verbosity = EFI_LOG_VERBOSITY_DEFAULT;
  // Install the logging protocol
  Status = EfiInstallProtocolInterface(&gEfiImageHandle, &gEfiLogProtocolGuid, EFI_NATIVE_INTERFACE, (VOID *)Impl);
  if (EFI_ERROR(Status)) {
    // Free the logging protocol interface since there was an error
#if defined(EFI_MEMORY_VIRTUAL)
    EfiInternalFreePool(Impl);
#else
    EfiFreePool(Impl);
#endif
  }
  // Return the status code
  return Status;
}
// EfiLogUninstall
/// Uninstall logging protocol
/// @retval EFI_SUCCESS The logging protocol was uninstalled
EFI_STATUS
EFIAPI
EfiLogUninstall (
  VOID
) {
  // Check if the protocol is installed by trying to get the interface
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if (Log == NULL) {
    return EFI_NOT_FOUND;
  }
  // Uninstall the protocol
  EfiUninstallProtocolInterface(gEfiImageHandle, &gEfiLogProtocolGuid, (VOID *)Log);
  // Clear the protocol contents
  if (Log->Clear != NULL) {
    Log->Clear(Log);
  }
  // Free the protocol interface
  return EfiFreePool(Log);
}

// EfiLog
/// Print a formatted character string to log
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiLog (
  CONST CHAR16 *Format,
  ...
) {
  UINTN   Count;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Count = EfiVLog(Format, Marker);
  VA_END(Marker);
  return Count;
}
// EfiVLog
/// Print a formatted character string to log
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiVLog (
  CONST CHAR16 *Format,
  VA_LIST       Marker
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->VLog == NULL)) {
    return 0;
  }
  return Log->VLog(Log, Format, Marker);
}

// EfiVerbose
/// Print a formatted character string to log with verbosity
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param ...       The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiVerbose (
  UINT32        Verbosity,
  CONST CHAR16 *Format,
  ...
) {
  UINTN   Count;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Count = EfiVVerbose(Verbosity, Format, Marker);
  VA_END(Marker);
  return Count;
}
// EfiVVerbose
/// Print a formatted character string to log with verbosity
/// @param Verbosity The verbosity level of this message
/// @param Format    The format specifier string
/// @param Marker    The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiVVerbose (
  UINT32        Verbosity,
  CONST CHAR16 *Format,
  VA_LIST       Marker
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->VVerbose == NULL)) {
    return 0;
  }
  return Log->VVerbose(Log, Verbosity, Format, Marker);
}

// EfiLogGetContents
/// Get the log contents
/// @return The log contents
CONST CHAR16 *
EFIAPI
EfiLogGetContents (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->GetContents == NULL)) {
    return NULL;
  }
  return Log->GetContents(Log);
}

// EfiLogClear
/// Clear the log contents
VOID
EFIAPI
EfiLogClear (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log != NULL) && (Log->Clear != NULL)) {
    Log->Clear(Log);
  }
}

// EfiLogGetDefaultOutputs
/// Get the default log outputs
/// @return The default log outputs
UINT32
EFIAPI
EfiLogGetDefaultOutputs (
  VOID
) {
  UINT32 Outputs = 0;
  if (EfiConfigurationGetBoolean(L"/Boot/Output/Console", TRUE)) {
    Outputs |= EFI_LOG_OUTPUT_CONSOLE;
  }
  if (EfiConfigurationGetBoolean(L"/Boot/Output/File", FALSE)) {
    Outputs |= EFI_LOG_OUTPUT_FILE;
  }
  if (EfiConfigurationGetBoolean(L"/Boot/Output/Serial", FALSE)) {
    Outputs |= EFI_LOG_OUTPUT_SERIAL;
  }
  return Outputs;
}
// EfiLogGetOutputs
/// Get the current enabled log outputs
/// @return The current enabled log outputs
UINT32
EFIAPI
EfiLogGetOutputs (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->GetOutputs == NULL)) {
    return 0;
  }
  return Log->GetOutputs(Log);
}
// EfiLogSetOutputs
/// Enable or disable log outpute
/// @param Outputs The outputs that should be enabled, otherwise disabled
/// @param Flush   Whether to output the previous log now
BOOLEAN
EFIAPI
EfiLogSetOutputs (
  UINT32  Outputs,
  BOOLEAN Flush
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->SetOutputs == NULL)) {
    return FALSE;
  }
  return Log->SetOutputs(Log, Outputs, Flush);
}

// EfiLogGetFile
/// Get the log output file path
/// @return The log output file path
CONST CHAR16 *
EFIAPI
EfiLogGetFile (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->GetFile == NULL)) {
    return NULL;
  }
  return Log->GetFile(Log);
}
// EfiLogSetFile
/// Set the log output file path
/// @param LogPath The log output file path
EFI_STATUS
EFIAPI
EfiLogSetFile (
  CONST CHAR16 *LogPath
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->SetFile == NULL)) {
    return EFI_NOT_FOUND;
  }
  return Log->SetFile(Log, LogPath);
}

// EfiLogGetVerbosity
/// Get the current log verbosity level
/// @return The current log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
UINT32
EFIAPI
EfiLogGetVerbosity (
  VOID
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->GetVerbosity == NULL)) {
    return 0;
  }
  return Log->GetVerbosity(Log);
}
// EfiLogGetVerbosity
/// Set the log verbosity level
/// @param Verbosity The log verbosity level, messages logged above this level will be discarded, verbose level zero is never discarded
BOOLEAN
EFIAPI
EfiLogSetVerbosity (
  IN UINT32 Verbosity
) {
  EFI_LOG_PROTOCOL *Log = GetLogProtocol();
  if ((Log == NULL) || (Log->SetVerbosity == NULL)) {
    return FALSE;
  }
  return Log->SetVerbosity(Log, Verbosity);
}
