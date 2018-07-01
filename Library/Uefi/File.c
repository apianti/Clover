///
/// @file Library/Uefi/File.c
///
/// UEFI implementation file protocol
///

#include <Uefi.h>

#include <Uefi/Protocol/LoadedImage.h>

#include "VirtualMemory.h"

// gEfiSimpleFileSystemProtocolGuid
/// Simple file system protocol unique identiifer
EFI_GUID gEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

// gEfiPartTypeUnusedGuid
/// The unused partition type unique identifier
EFI_GUID gEfiPartTypeUnusedGuid = EFI_PART_TYPE_UNUSED_GUID;
// gEfiPartTypeSystemPartGuid
/// The EFI system partition type unique identifier
EFI_GUID gEfiPartTypeSystemPartGuid = EFI_PART_TYPE_EFI_SYSTEM_PART_GUID;
// gEfiPartTypeLegacyMbrGuid
/// The legacy partition type unique identifier
EFI_GUID gEfiPartTypeLegacyMbrGuid = EFI_PART_TYPE_LEGACY_MBR_GUID;

// EfiFileOpenRoot
/// Opens the root directory on a volume
/// @param This The simple file system protocol interface
/// @param Root On output, the root file protocol interface
/// @retval EFI_SUCCESS          The file volume was opened
/// @retval EFI_UNSUPPORTED      The volume does not support the requested file system type
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES The file volume was not opened
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported. Any existing file handles for this
///                               volume are no longer valid. To access the files on the new medium, the volume must be reopened with OpenVolume()
EFI_STATUS
EFIAPI
EfiFileOpenRoot (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL               **Root
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->OpenVolume == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->OpenVolume(This, Root);
}
// EfiFileOpenRootByHandle
/// Opens the root directory on a volume
/// @param Handle The device handle for simple file system protocol interface
/// @param Root   On output, the root file protocol interface
/// @retval EFI_SUCCESS          The file volume was opened
/// @retval EFI_UNSUPPORTED      The volume does not support the requested file system type
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES The file volume was not opened
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported. Any existing file handles for this
///                               volume are no longer valid. To access the files on the new medium, the volume must be reopened with OpenVolume()
EFI_STATUS
EFIAPI
EfiFileOpenRootByHandle (
  IN  EFI_HANDLE          Handle,
  OUT EFI_FILE_PROTOCOL **Root
) {
  EFI_STATUS                       Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This = NULL;
  if (Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Open file system protocol
  Status = EfiHandleProtocol(Handle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&This);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if ((This == NULL) || (This->OpenVolume == NULL)){
    return EFI_UNSUPPORTED;
  }
  return This->OpenVolume(This, Root);
}

// EfiFileOpen
/// Opens a new file relative to the source file’s location
/// @param This       The file protocol interface
/// @param NewHandle  On output, the opened handle for the new file
/// @param FileName   The null-terminated Unicode string of the name of the file to be opened or NULL to reopen the handle with another interface, reopening a file may not be
///                    supported on all firmwares so only directory reopening should be used, the file name may contain the following path modifiers: "\", ".", and ".."
/// @param OpenMode   The mode to open the file. The only valid combinations that the file may be opened with are: Read, Read/Write, or Create/Read/Write
/// @param Attributes Only valid for EFI_FILE_MODE_CREATE, in which case these are the attribute bits for the newly created file
/// @retval EFI_SUCCESS          The file was opened
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileOpen (
  IN  EFI_FILE_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL **NewHandle,
  IN  CONST CHAR16       *FileName OPTIONAL,
  IN  UINT64              OpenMode,
  IN  UINT64              Attributes
) {
  EFI_STATUS     Status;
  EFI_FILE_INFO *Info = NULL;
  // Check parameters
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Open == NULL) {
    return EFI_UNSUPPORTED;
  }
  // There are some special cases for certain filenames that will be unsupported
  //  but the behavior will be changed to duplicate the current handle
  if ((FileName == NULL) || (*FileName == L'\0')) {
    // Get the file information
    Info = EfiFileInfo(This);
    if (Info == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Check if a directory
    if (EFI_BITS_ANY_SET(Info->Attribute, EFI_FILE_DIRECTORY)) {
      // Reopen the same directory
      Status = This->Open(This, NewHandle, L".", OpenMode, Attributes);
    } else {
      // Append the filename to the parent directory operator for path
      CHAR16 *Path = EfiPoolPrint(L"..\\%s", Info->FileName);
      if (Path == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
      } else {
        // Reopen the file from the parent directory
        Status = This->Open(This, NewHandle, Path, OpenMode, Attributes);
      }
      // Free the path
      EfiFreePool(Path);
    }
    // Free file info
    EfiFreePool(Info);
  } else {
    // Open the file normally
    Status = This->Open(This, NewHandle, (CHAR16 *)FileName, OpenMode, Attributes);
  }
  return Status;
}
// EfiFileOpenPath
/// Opens a new file from a device path string
/// @param NewHandle  On output, the opened handle for the new file
/// @param DevicePath The null-terminated Unicode string of the device path of the file to be opened
/// @param OpenMode   The mode to open the file. The only valid combinations that the file may be opened with are: Read, Read/Write, or Create/Read/Write
/// @param Attributes Only valid for EFI_FILE_MODE_CREATE, in which case these are the attribute bits for the newly created file
/// @retval EFI_SUCCESS          The file was opened
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileOpenPath (
  OUT EFI_FILE_PROTOCOL **NewHandle,
  IN  CONST CHAR16       *DevicePath,
  IN  UINT64              OpenMode,
  IN  UINT64              Attributes
) {
  EFI_STATUS Status;
  // Get the device path from the string
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Open the device path
  Status = EfiFileOpenDevicePath(NewHandle, ActualDevicePath, OpenMode, Attributes);
  // Free the device path
  EfiFreePool(ActualDevicePath);
  return Status;
}
// EfiFileOpenDevicePath
/// Opens a new file from a device path
/// @param NewHandle  On output, the opened handle for the new file
/// @param DevicePath The device path of the file to be opened
/// @param OpenMode   The mode to open the file. The only valid combinations that the file may be opened with are: Read, Read/Write, or Create/Read/Write
/// @param Attributes Only valid for EFI_FILE_MODE_CREATE, in which case these are the attribute bits for the newly created file
/// @retval EFI_SUCCESS          The file was opened
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileOpenDevicePath (
  OUT EFI_FILE_PROTOCOL        **NewHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN  UINT64                     OpenMode,
  IN  UINT64                     Attributes
) {
  EFI_FILE_PROTOCOL *Root = NULL;
  EFI_HANDLE         Handle = NULL;
  CHAR16            *FileName = NULL;
  // Get the device handle and file path
  EFI_STATUS Status = EfiFileLocateDevicePath(&DevicePath, &Handle, &FileName);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check device handle and file path are valid
  if (FileName == NULL) {
    Status = (Handle == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
  } else if (Handle == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (!EFI_ERROR(Status)) {
    // Open the root directory
    Status = EfiFileOpenRootByHandle(Handle, &Root);
    if (!EFI_ERROR(Status)) {
      if (Root == NULL) {
        Status = EFI_NOT_FOUND;
      } else {
        // Open the new file handle
        Status = EfiFileOpen(Root, NewHandle, FileName, OpenMode, Attributes);
      }
    }
  }
  // Cleanup root and file path
  if (Root != NULL) {
    EfiFileClose(Root);
  }
  if (FileName != NULL) {
    EfiFreePool(FileName);
  }
  return Status;
}

// EfiFileExists
/// Check a file exists relative to the source file’s location
/// @param This     The file protocol interface
/// @param FileName The null-terminated Unicode string of the name of the file to check exists, the file name may contain the following path modifiers: "\", ".", and ".."
/// @retval EFI_SUCCESS          The file exists
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileExists (
  IN EFI_FILE_PROTOCOL *This,
  IN CONST CHAR16      *FileName
) {
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *File = NULL;
  // Try to open the file, an error here means it does not exist
  Status = EfiFileOpen(This, &File, FileName, EFI_FILE_MODE_READ, 0);
  if (!EFI_ERROR(Status) && (File != NULL)) {
    // Close the file
    EfiFileClose(File);
  }
  return Status;
}
// EfiFileExistsPath
/// Check a file exists from a device path string
/// @param DevicePath The null-terminated Unicode string of the device path of the file to check exists
/// @retval EFI_SUCCESS          The file exists
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileExistsPath (
  IN CONST CHAR16 *DevicePath
) {
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *File = NULL;
  // Try to open the file, an error here means it does not exist
  Status = EfiFileOpenPath(&File, DevicePath, EFI_FILE_MODE_READ, 0);
  if (File != NULL) {
    // Close the file
    EfiFileClose(File);
  }
  return Status;
}
// EfiFileExistsDevicePath
/// Check a file exists from a device path
/// @param DevicePath The device path of the file to be opened
/// @retval EFI_SUCCESS          The file exists
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileExistsDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *File = NULL;
  // Try to open the file, an error here means it does not exist
  Status = EfiFileOpenDevicePath(&File, DevicePath, EFI_FILE_MODE_READ, 0);
  if (!EFI_ERROR(Status) && (File != NULL)) {
    // Close the file
    EfiFileClose(File);
  }
  return Status;
}

// EfiFileLoad
/// Load a file relative to the source file’s location
/// @param This       The file protocol interface
/// @param FileName   The null-terminated Unicode string of the name of the file to be loaded, the file name may contain the following path modifiers: "\", ".", and ".."
/// @param Size       On output, the size in bytes of the loaded file buffer
/// @param Buffer     On output, the buffer of the loaded file which must be freed
/// @retval EFI_SUCCESS          The file was loaded
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileLoad (
  IN  EFI_FILE_PROTOCOL  *This,
  IN  CONST CHAR16       *FileName,
  OUT UINT64             *Size,
  OUT VOID              **Buffer
) {
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *File = NULL;
  VOID              *FileBuffer = NULL;
  UINT64             BufferSize = 0;
  // Check parameters
  if ((This == NULL) || (FileName == NULL) || (Size == NULL) || (Buffer == NULL) || (*Buffer != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Open the file by path
  Status = EfiFileOpen(This, &File, (CHAR16 *)FileName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Get the file size
  Status = EfiFileGetSize(File, &BufferSize);
  if (!EFI_ERROR(Status)) {
    if (Size == 0) {
      Status = EFI_NOT_FOUND;
    } else {
      // Create buffer
      FileBuffer = EfiAllocate((UINTN)BufferSize);
      if (FileBuffer == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
      } else {
        // Read from the file
        Status = EfiFileRead(File, (UINTN *)&BufferSize, FileBuffer);
        if (EFI_ERROR(Status)) {
          EfiFreePool(Buffer);
        }
      }
    }
  }
  // Close the file
  EfiFileClose(File);
  // Set the buffer size
  if (!EFI_ERROR(Status)) {
    *Buffer = FileBuffer;
    *Size = (UINT64)BufferSize;
  }
  return Status;
}
// EfiFileLoadPath
/// Load a file from a device path string
/// @param DevicePath The null-terminated Unicode string of the device path of the file to be loaded
/// @param Size       On output, the size in bytes of the loaded file buffer
/// @param Buffer     On output, the buffer of the loaded file which must be freed
/// @retval EFI_SUCCESS          The file was loaded
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileLoadPath (
  IN  CONST CHAR16  *DevicePath,
  OUT UINT64        *Size,
  OUT VOID         **Buffer
) {
  EFI_STATUS Status;
  // Get the device path from the string
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Load the device path
  Status = EfiFileLoadDevicePath(ActualDevicePath, Size, Buffer);
  // Free the device path
  EfiFreePool(ActualDevicePath);
  return Status;
}
// EfiFileLoadDevicePath
/// Load a file from a device path
/// @param DevicePath The device path of the file to be loaded
/// @param Size       On output, the size in bytes of the loaded file buffer
/// @param Buffer     On output, the buffer of the loaded file which must be freed
/// @retval EFI_SUCCESS          The file was loaded
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileLoadDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT64                    *Size,
  OUT VOID                     **Buffer
) {
  EFI_FILE_PROTOCOL *Root = NULL;
  EFI_HANDLE         Handle = NULL;
  CHAR16            *FileName = NULL;
  // Get the device handle and file path
  EFI_STATUS Status = EfiFileLocateDevicePath(&DevicePath, &Handle, &FileName);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check device handle and file path are valid
  if (FileName == NULL) {
    Status = (Handle == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
  } else if (Handle == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (!EFI_ERROR(Status)) {
    // Open the root directory
    Status = EfiFileOpenRootByHandle(Handle, &Root);
    if (!EFI_ERROR(Status)) {
      if (Root == NULL) {
        Status = EFI_NOT_FOUND;
      } else {
        // Load the file
        Status = EfiFileLoad(Root, FileName, Size, Buffer);
      }
    }
  }
  // Cleanup root and file path
  if (Root != NULL) {
    EfiFileClose(Root);
  }
  if (FileName != NULL) {
    EfiFreePool(FileName);
  }
  return Status;
}

// EfiFileSave
/// Save a file relative to the source file’s location
/// @param This     The file protocol interface
/// @param FileName The null-terminated Unicode string of the name of the file to be saved, the file name may contain the following path modifiers: "\", ".", and ".."
/// @param Size     The size in bytes of the buffer
/// @param Buffer   The buffer to save to file
/// @retval EFI_SUCCESS          The file was saved
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileSave (
  IN EFI_FILE_PROTOCOL *This,
  IN CONST CHAR16      *FileName,
  IN UINT64             Size,
  IN VOID              *Buffer
) {
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL *File = NULL;
  // Check parameters
  if ((This == NULL) || (FileName == NULL) || (Size == 0) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Open the file by path
  Status = EfiFileOpen(This, &File, (CHAR16 *)FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Write to the file
  Status = EfiFileWrite(File, (UINTN *)&Size, Buffer);
  // Close the file
  EfiFileClose(File);
  return Status;
}
// EfiFileSavePath
/// Save a file specified by a device path string
/// @param DevicePath The null-terminated Unicode string of the device path of the file to be saved
/// @param Size     The size in bytes of the buffer
/// @param Buffer   The buffer to save to file
/// @retval EFI_SUCCESS          The file was saved
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileSavePath (
  IN CONST CHAR16 *DevicePath,
  IN UINT64        Size,
  IN VOID         *Buffer
) {
  EFI_STATUS Status;
  // Get the device path from the string
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Save the file
  Status = EfiFileSaveDevicePath(ActualDevicePath, Size, Buffer);
  // Free the device path
  EfiFreePool(ActualDevicePath);
  return Status;
}
// EfiFileSaveDevicePath
/// Save a file specified by a device path
/// @param DevicePath The device path of the file to be saved
/// @param Size     The size in bytes of the buffer
/// @param Buffer   The buffer to save to file
/// @retval EFI_SUCCESS          The file was saved
/// @retval EFI_NOT_FOUND        The specified file could not be found on the device
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_MEDIA_CHANGED    The device has a different medium in it or the medium is no longer supported
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Not enough resources were available to open the file
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileSaveDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN UINT64                    Size,
  IN VOID                     *Buffer
) {
  EFI_FILE_PROTOCOL *Root = NULL;
  EFI_HANDLE         Handle = NULL;
  CHAR16            *FileName = NULL;
  // Get the device handle and file path
  EFI_STATUS Status = EfiFileLocateDevicePath(&DevicePath, &Handle, &FileName);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check device handle and file path are valid
  if (FileName == NULL) {
    Status = (Handle == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
  } else if (Handle == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (!EFI_ERROR(Status)) {
    // Open the root directory
    Status = EfiFileOpenRootByHandle(Handle, &Root);
    if (!EFI_ERROR(Status)) {
      if (Root == NULL) {
        Status = EFI_NOT_FOUND;
      } else {
        // Save the file
        Status = EfiFileSave(Root, FileName, Size, Buffer);
      }
    }
  }
  // Cleanup root and file path
  if (Root != NULL) {
    EfiFileClose(Root);
  }
  if (FileName != NULL) {
    EfiFreePool(FileName);
  }
  return Status;
}

// EfiFileLocateDevicePath
/// Locate device handle and file path from device path
/// @param DevicePath On input, the device path, on output, the remaining part of the device path
/// @param Device     On output, the device handle
/// @param FileName   On output, the file path which must be freed
/// @retval EFI_SUCCESS           The resulting handle was returned
/// @retval EFI_NOT_FOUND         No handles matched the search
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER DevicePath is NULL
/// @retval EFI_INVALID_PARAMETER A handle matched the search and Device is NULL
EFI_STATUS
EFIAPI
EfiFileLocateDevicePath (
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT    EFI_HANDLE                *Device,
  OUT    CHAR16                   **FileName
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *Node;
  CHAR16                   *Path;
  if ((DevicePath == NULL) || (Device == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Locate the file system protocol device handle
  Status = EfiLocateDevicePath(&gEfiSimpleFileSystemProtocolGuid, DevicePath, Device);
  if (Status == EFI_NOT_FOUND) {
    // Attempt to use the loaded image protocol to use same device handle as this image
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = EfiLoadedImage(gEfiImageHandle);
    if (LoadedImage != NULL) {
      *Device = LoadedImage->DeviceHandle;
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_NOT_FOUND;
    }
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (*Device == NULL) {
    return EFI_NOT_FOUND;
  }
  // Check if the remaining path is file path
  Node = *DevicePath;
  if ((Node->Type == MEDIA_DEVICE_PATH) && (Node->SubType == MEDIA_FILEPATH_DP)) {
    // Convert to text
    Path = EfiDevicePathToText(Node, FALSE, FALSE);
    if (Path == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    if (FileName != NULL) {
      *FileName = Path;
    } else {
      EfiFreePool(Path);
    }
    // Advance the device path to next node
    *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)(((UINT8 *)Node) + EfiGetDevicePathSize(Node));
  } else if (FileName != NULL) {
    *FileName = NULL;
  }
  return Status;
}

// EfiFileClose
/// Closes a specified file handle
/// @param This The file protocol interface
/// @retval EFI_SUCCESS The file was closed
EFI_STATUS
EFIAPI
EfiFileClose (
  IN EFI_FILE_PROTOCOL *This
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Close == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->Close(This);
}

// EfiFileDelete
/// Closes and deletes a file
/// @param This The file protocol interface
/// @retval EFI_SUCCESS             The file was closed and deleted, and the handle was closed
/// @retval EFI_WARN_DELETE_FAILURE The handle was closed, but the file was not deleted
EFI_STATUS
EFIAPI
EfiFileDelete (
  IN EFI_FILE_PROTOCOL *This
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Delete == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->Delete(This);
}

// EfiFileRead
/// Reads data from a file
/// @param This       The file protocol interface
/// @param BufferSize On input, the size in bytes of the read buffer, on output, the size in bytes of the data read
/// @param Buffer     On output, the data which was read
/// @retval EFI_SUCCESS          The data was read
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_DEVICE_ERROR     An attempt was made to read from a deleted file
/// @retval EFI_DEVICE_ERROR     On entry, the current file position is beyond the end of the file
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_BUFFER_TOO_SMALL The BufferSize is too small to read the current directory entry. BufferSize has been updated with the size needed to complete the request
EFI_STATUS
EFIAPI
EfiFileRead (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Read == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->Read(This, BufferSize, Buffer);
}

// EfiFileWrite
/// Writes data to a file
/// @param This       The file protocol interface
/// @param BufferSize On input, the size in bytes of the write buffer, on output, the size in bytes of the data written
/// @param Buffer     The buffer of data to write
/// @retval EFI_SUCCESS          The data was written
/// @retval EFI_UNSUPPORT        Writes to open directory files are not supported
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_DEVICE_ERROR     An attempt was made to write to a deleted file
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  The file or medium is write-protected
/// @retval EFI_ACCESS_DENIED    The file was opened read only
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileWrite (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  IN     VOID              *Buffer
) {
  if ((This == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Write == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->Write(This, BufferSize, Buffer);
}

// EfiFileGetPosition
/// Returns a file’s current position
/// @param This     The file protocol interface
/// @param Position On output, the current byte position from the start of the file
/// @retval EFI_SUCCESS      The position was returned
/// @retval EFI_UNSUPPORTED  The request is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to get the position from a deleted file
EFI_STATUS
EfiFileGetPosition (
  IN  EFI_FILE_PROTOCOL *This,
  OUT UINT64            *Position
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->GetPosition == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->GetPosition(This, Position);
}

// EfiFileSetPosition
/// Sets a file’s current position
/// @param This     The file protocol interface
/// @param Position The byte position from the start of the file to set
/// @retval EFI_SUCCESS      The position was set
/// @retval EFI_UNSUPPORTED  The seek request for nonzero is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to set the position of a deleted file
EFI_STATUS
EFIAPI
EfiFileSetPosition (
  IN EFI_FILE_PROTOCOL *This,
  IN UINT64             Position
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->SetPosition == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->SetPosition(This, Position);
}

// EfiFileGetSize
/// Get the size in bytes of a file
/// @param This The file protocol interface
/// @param Size On output, the size in bytes of the file
/// @retval EFI_SUCCESS           The size of the file was returned
/// @retval EFI_INVALID_PARAMETER If This or Size is NULL
/// @retval EFI_NOT_FOUND         The file does not exist or has no size
EFI_STATUS
EFIAPI
EfiFileGetSize (
  IN  EFI_FILE_PROTOCOL *This,
  OUT UINT64            *Size
) {
  EFI_FILE_INFO *FileInfo;
  if ((This == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the file information
  FileInfo = EfiFileInfo(This);
  if (FileInfo == NULL) {
    return EFI_NOT_FOUND;
  }
  // Get file size in bytes
  *Size = FileInfo->FileSize;
  // Free the file information and return success
  EfiFreePool((VOID *)FileInfo);
  return EFI_SUCCESS;
}

// EfiFileGetInfo
/// Returns information about a file
/// @param This            The file protocol interface
/// @param InformationType The unique identifier for the type of information being requested
/// @param BufferSize      On input, the size in bytes of the information buffer, on output, the size in bytes of data returned in the information buffer
/// @param Buffer          On output, the returned information data indicated by InformationType
/// @retval EFI_SUCCESS          The information was set
/// @retval EFI_UNSUPPORTED      The InformationType is not known
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_BUFFER_TOO_SMALL The BufferSize is too small to read the current directory entry, BufferSize has been updated with the size needed to complete the request
EFI_STATUS
EFIAPI
EfiFileGetInfo (
  IN     EFI_FILE_PROTOCOL *This,
  IN     EFI_GUID          *InformationType,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->GetInfo == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->GetInfo(This, InformationType, BufferSize, Buffer);
}

// EfiSetFileInfo
/// Sets information about a file
/// @param This            The file protocol interface
/// @param InformationType The unique identifier for the type of information being set
/// @param BufferSize      The size, in bytes, of the information data to set
/// @param Buffer          The information data to set as indicated by InformationType
/// @retval EFI_SUCCESS          The information was set
/// @retval EFI_UNSUPPORTED      The InformationType is not known
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  InformationType is EFI_FILE_INFO_ID and the media is read-only
/// @retval EFI_WRITE_PROTECTED  InformationType is EFI_FILE_PROTOCOL_SYSTEM_INFO_ID and the media is read only
/// @retval EFI_WRITE_PROTECTED  InformationType is EFI_FILE_SYSTEM_VOLUME_LABEL_ID and the media is read-only
/// @retval EFI_ACCESS_DENIED    An attempt is made to change the name of a file to a file that is already present
/// @retval EFI_ACCESS_DENIED    An attempt is being made to change the EFI_FILE_DIRECTORY Attribute
/// @retval EFI_ACCESS_DENIED    An attempt is being made to change the size of a directory
/// @retval EFI_ACCESS_DENIED    InformationType is EFI_FILE_INFO_ID and the file was opened read-only and an attempt is being made to modify a field other than Attribute
/// @retval EFI_VOLUME_FULL      The volume is full
/// @retval EFI_BAD_BUFFER_SIZE  BufferSize is smaller than the size of the type indicated by InformationType
EFI_STATUS
EFIAPI
EfiSetFileInfo (
  IN EFI_FILE_PROTOCOL *This,
  IN EFI_GUID          *InformationType,
  IN UINTN              BufferSize,
  IN VOID              *Buffer
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->SetInfo == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->SetInfo(This, InformationType, BufferSize, Buffer);
}

// EfiFileFlush
/// Flushes all modified data associated with a file to a device
/// @param This The file protocol interface
/// @retval EFI_SUCCESS          The data was flushed
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  The file or medium is write-protected
/// @retval EFI_ACCESS_DENIED    The file was opened read-only
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileFlush (
  IN EFI_FILE_PROTOCOL *This
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (This->Flush == NULL) {
    return EFI_UNSUPPORTED;
  }
  return This->Flush(This);
}

//
// Extended file operations (Revision 2.0+)
//

// EfiFileOpenEx
/// Opens a new file relative to the source directory’s location
/// @param This       The file protocol interface
/// @param NewHandle  On output, the opened handle for the new file. For asynchronous I/O, this pointer must remain valid for the duration of the asynchronous operation
/// @param FileName   The mull-terminated Unicode string of the name of the file to be opened. The file name may contain the following path modifiers: "\", ".", and ".."
/// @param OpenMode   The mode to open the file. The only valid combinations that the file may be opened with are: Read, Read/Write, or Create/Read/Write
/// @param Attributes Only valid for EFI_FILE_MODE_CREATE, in which case these are the attribute bits for the newly created file
/// @param Token      The token associated with the transaction
/// @retval EFI_SUCCESS          The file was opened successfully or the request was successfully queued for processing and the event will be signaled upon completion
/// @retval EFI_NOT_FOUND        The device has no medium
/// @retval EFI_NO_MEDIA         The specified file could not be found on the device
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  An attempt was made to create a file, or open a file for write when the media is write-protected
/// @retval EFI_ACCESS_DENIED    The service denied access to the file
/// @retval EFI_OUT_OF_RESOURCES Unable to queue the request or open the file due to lack of resources
/// @retval EFI_VOLUME_FULL      The volume is full
EFI_STATUS
EFIAPI
EfiFileOpenEx (
  IN     EFI_FILE_PROTOCOL  *This,
  OUT    EFI_FILE_PROTOCOL **NewHandle,
  IN     CHAR16             *FileName,
  IN     UINT64              OpenMode,
  IN     UINT64              Attributes,
  IN OUT EFI_FILE_IO_TOKEN  *Token
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((This->Revision < EFI_FILE_PROTOCOL_REVISION2) || (This->OpenEx == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->OpenEx(This, NewHandle, FileName, OpenMode, Attributes, Token);
}

// EfiFileReadEx
/// Reads data from a file
/// @param This  The file protocol interface
/// @param Token The token associated with the transaction
/// @retval EFI_SUCCESS          The data was read successfully or the request was successfully queued for processing and the event will be signaled upon completion
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_DEVICE_ERROR     An attempt was made to read from a deleted file
/// @retval EFI_DEVICE_ERROR     On entry, the current file position is beyond the end of the file
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_OUT_OF_RESOURCES Unable to queue the request due to lack of resources
EFI_STATUS
EFIAPI
EfiFileReadEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((This->Revision < EFI_FILE_PROTOCOL_REVISION2) || (This->ReadEx == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->ReadEx(This, Token);
}

// EfiFileWriteEx
/// Writes data to a file
/// @param This  The file protocol interface
/// @param Token The token associated with the transaction
/// @retval EFI_SUCCESS          The data was written successfully or the request was successfully queued for processing and the event will be signaled upon completion
/// @retval EFI_UNSUPPORTED      Writes to open directory files are not supported
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_DEVICE_ERROR     An attempt was made to write to a deleted file
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  The file or medium is write-protected
/// @retval EFI_ACCESS_DENIED    The file was opened read only
/// @retval EFI_VOLUME_FULL      The volume is full
/// @retval EFI_OUT_OF_RESOURCES Unable to queue the request due to lack of resources
EFI_STATUS
EFIAPI
EfiFileWriteEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((This->Revision < EFI_FILE_PROTOCOL_REVISION2) || (This->WriteEx == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->WriteEx(This, Token);
}

// EfiFileFlushEx
/// Flushes all modified data associated with a file to a device
/// @param This  The file protocol interface
/// @param Token The token associated with the transaction
/// @retval EFI_SUCCESS          The data was flushed successfully or the request was successfully queued for processing and the event will be signaled upon completion
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  The file or medium is write-protected
/// @retval EFI_ACCESS_DENIED    The file was opened read-only
/// @retval EFI_VOLUME_FULL      The volume is full
/// @retval EFI_OUT_OF_RESOURCES Unable to queue the request due to lack of resources
EFI_STATUS
EFIAPI
EfiFileFlushEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
) {
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((This->Revision < EFI_FILE_PROTOCOL_REVISION2) || (This->FlushEx == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return This->FlushEx(This, Token);
}

// EfiFileInfo
/// Get file information
/// @param This The file protocol interface
/// @return The file information which must be freed or NULL if there was an error or no file information
EFI_FILE_INFO *
EFIAPI
EfiFileInfo (
  IN EFI_FILE_PROTOCOL *This
) {
  EFI_FILE_INFO *FileInfo = NULL;
  UINTN          Size = 0;
  // Get the file information size in bytes
  if ((This->GetInfo(This, &gEfiFileInfoGuid, &Size, NULL) == EFI_BUFFER_TOO_SMALL) &&
      ((Size += sizeof(CHAR16)) >= sizeof(EFI_FILE_INFO))) {
    // Create a file information
    FileInfo = (EFI_FILE_INFO *)EfiAllocate(Size);
    if (FileInfo != NULL) {
      // Get the file information
      if (EFI_ERROR(This->GetInfo(This, &gEfiFileInfoGuid, &Size, (VOID *)FileInfo))) {
        // Free the file information on error
        EfiFreePool(FileInfo);
        FileInfo = NULL;
      }
    }
  }
  // Return the file information
  return FileInfo;
}
// EfiFileSystemInfo
/// Get file system information
/// @param This The file protocol interface
/// @return The file system information which must be freed or NULL if there was an error or no file system information
EFI_FILE_SYSTEM_INFO *
EFIAPI
EfiFileSystemInfo (
  IN EFI_FILE_PROTOCOL *This
) {
  EFI_FILE_SYSTEM_INFO *FileSystemInfo = NULL;
  UINTN                 Size = 0;
  // Get the file system information size in bytes
  if ((This->GetInfo(This, &gEfiFileSystemInfoGuid, &Size, NULL) == EFI_BUFFER_TOO_SMALL) &&
      ((Size += sizeof(CHAR16)) >= sizeof(EFI_FILE_SYSTEM_INFO))) {
    // Create a file system information
    FileSystemInfo = (EFI_FILE_SYSTEM_INFO *)EfiAllocate(Size);
    if (FileSystemInfo != NULL) {
      // Get the file system information
      if (EFI_ERROR(This->GetInfo(This, &gEfiFileSystemInfoGuid, &Size, (VOID *)FileSystemInfo))) {
        // Free the file system information on error
        EfiFreePool(FileSystemInfo);
        FileSystemInfo = NULL;
      }
    }
  }
  // Return the file system information
  return FileSystemInfo;
}
// EfiFileSystemVolumeLabelInfo
/// Get file system volume label information
/// @param This The file protocol interface
/// @return The file system volume label information which must be freed or NULL if there was an error or no file system information
EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *
EFIAPI
EfiFileSystemVolumeLabelInfo (
  IN EFI_FILE_PROTOCOL *This
) {
  EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *FileSystemVolumeLabelInfo = NULL;
  UINTN                              Size = 0;
  // Get the file system volume label information size in bytes
  if ((This->GetInfo(This, &gEfiFileSystemVolumeLabelInfoGuid, &Size, NULL) == EFI_BUFFER_TOO_SMALL) &&
      ((Size += sizeof(CHAR16)) >= sizeof(EFI_FILE_SYSTEM_VOLUME_LABEL_INFO))) {
    // Create a file system volume label information
    FileSystemVolumeLabelInfo = (EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *)EfiAllocate(Size);
    if (FileSystemVolumeLabelInfo != NULL) {
      // Get the file system volume label information
      if (EFI_ERROR(This->GetInfo(This, &gEfiFileSystemVolumeLabelInfoGuid, &Size, (VOID *)FileSystemVolumeLabelInfo))) {
        // Free the file system volume label information on error
        EfiFreePool(FileSystemVolumeLabelInfo);
        FileSystemVolumeLabelInfo = NULL;
      }
      // Check to make sure this is not actually file system information in buggy firmware
      if (FileSystemVolumeLabelInfo != NULL) {
        EFI_FILE_SYSTEM_INFO *FileSystemInfo = (EFI_FILE_SYSTEM_INFO *)FileSystemVolumeLabelInfo;
        if (FileSystemInfo->Size == (UINT64)Size) {
          // Get the size in bytes of the system volume label
          Size = StrSize(FileSystemInfo->VolumeLabel);
          if (Size <= sizeof(CHAR16)) {
            // There appears to be no volume label so don't return anything
            FileSystemVolumeLabelInfo = NULL;
          } else {
            // Create a new file system volume label information
            FileSystemVolumeLabelInfo = (EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *)EfiAllocate(Size);
            if (FileSystemVolumeLabelInfo != NULL) {
              // Copy the file system volume label into to file system volume label information
              EfiCopyMem(FileSystemVolumeLabelInfo->VolumeLabel, FileSystemInfo->VolumeLabel, Size);
            }
          }
          // Free the file system information
          EfiFreePool(FileSystemInfo);
        }
      }
    }
  }
  // Return the file system volume label information
  return FileSystemVolumeLabelInfo;
}
