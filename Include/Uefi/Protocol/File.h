///
/// @file Include/Uefi/Protocol/File.h
///
/// UEFI implementation file protocol
///

#pragma once
#ifndef __UEFI_FILE_PROTOCOL_HEADER__
#define __UEFI_FILE_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_FILE_INFO_GUID
/// The file information unique identifier
#define EFI_FILE_INFO_GUID { 0x9576E92, 0x6D3F, 0x11D2, {0x8E, 0x39, 0x0, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }
// EFI_FILE_SYSTEM_INFO_GUID
/// The file system information unique identifier
#define EFI_FILE_SYSTEM_INFO_GUID { 0x9576E93, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x0, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }
// EFI_FILE_SYSTEM_VOLUME_LABEL_INFO_GUID
/// The file system volume label information unique identifier
#define EFI_FILE_SYSTEM_VOLUME_LABEL_INFO_GUID { 0xDB47D7D3, 0xFE81, 0x11d3, { 0x9A, 0x35, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } }

// EFI_FILE_PROTOCOL_REVISION
/// The file protocol revision
#define EFI_FILE_PROTOCOL_REVISION 0x00010000
// EFI_FILE_PROTOCOL_REVISION2
/// The file protocol revision 2
#define EFI_FILE_PROTOCOL_REVISION2 0x00020000
// EFI_FILE_PROTOCOL_LATEST_REVISION
/// The file protocol latest revision
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

//
// File open modes
//

// EFI_FILE_MODE_READ
/// Open file for read mode
#define EFI_FILE_MODE_READ 0x0000000000000001
// EFI_FILE_MODE_WRITE
/// Open file for write mode
#define EFI_FILE_MODE_WRITE 0x0000000000000002
// EFI_FILE_MODE_CREATE
/// Create the file if the file does not already exist
#define EFI_FILE_MODE_CREATE 0x8000000000000000

//
// File attributes
//

// EFI_FILE_READ_ONLY
/// Indicates file is read-only
#define EFI_FILE_READ_ONLY 0x0000000000000001
// EFI_FILE_HIDDEN
/// Indicates file is hidden
#define EFI_FILE_HIDDEN 0x0000000000000002
// EFI_FILE_SYSTEM
/// Indicates file is system
#define EFI_FILE_SYSTEM 0x0000000000000004
// EFI_FILE_RESERVED
/// Indicates file is reserved
#define EFI_FILE_RESERVED 0x0000000000000008
// EFI_FILE_DIRECTORY
/// Indicates file is directory
#define EFI_FILE_DIRECTORY 0x0000000000000010
// EFI_FILE_ARCHIVE
/// Indicates file is archive
#define EFI_FILE_ARCHIVE 0x0000000000000020
// EFI_FILE_VALID_ATTR
/// Valid file attributes bitmask
#define EFI_FILE_VALID_ATTR 0x0000000000000037

//
// File positions
//

// EFI_FILE_POSITION_START
/// The file position for the start of the file
#define EFI_FILE_POSITION_START ((UINT64)0)
// EFI_FILE_POSITION_END
/// The file position for the end of the file
#define EFI_FILE_POSITION_END ((UINT64)0xFFFFFFFFFFFFFFFF)

// gEfiFileInfoGuid
/// The file information unique identifier
EXTERN EFI_GUID gEfiFileInfoGuid;
// gEfiFileSystemInfoGuid
/// The file system information unique identifier
EXTERN EFI_GUID gEfiFileSystemInfoGuid;
// gEfiFileSystemVolumeLabelInfoGuid
/// The file system volume label information unique identifier
EXTERN EFI_GUID gEfiFileSystemVolumeLabelInfoGuid;

// EFI_FILE_INFO
/// File information
typedef struct EFI_FILE_INFO EFI_FILE_INFO;
struct EFI_FILE_INFO {

  // Size
  /// The size in bytes of this file information including the null-terminated file name
  UINT64   Size;
  // FileSize
  /// The size in bytes of the file
  UINT64   FileSize;
  // PhysicalSize
  /// The size in bytes that is used by the file system for this file
  UINT64   PhysicalSize;
  // CreateTime
  /// The file creation time
  EFI_TIME CreateTime;
  // LastAccessTime
  /// The last access time
  EFI_TIME LastAccessTime;
  // ModificationTime
  /// The last modification time
  EFI_TIME ModificationTime;
  // Attribute
  /// The file attributes
  UINT64   Attribute;
  // FileName
  /// The null-terminated name of the file
  CHAR16   FileName[1];

};

// EFI_FILE_SYSTEM_INFO
/// File system information
typedef struct EFI_FILE_SYSTEM_INFO EFI_FILE_SYSTEM_INFO;
struct EFI_FILE_SYSTEM_INFO {

  // Size
  /// The size in bytes of this file system information including the null-terminated volume label
  UINT64  Size;
  // ReadOnly
  /// Whether the volume is read only
  BOOLEAN ReadOnly;
  // VolumeSize
  /// The total volume size in bytes
  UINT64  VolumeSize;
  // FreeSpace
  /// The available free size in bytes
  UINT64  FreeSpace;
  // BlockSize
  /// The size in bytes of a volume allocation block
  UINT32  BlockSize;
  // VolumeLabel
  /// The null-terminated label of the volume
  CHAR16  VolumeLabel[1];

};

// EFI_FILE_SYSTEM_VOLUME_LABEL_INFO 
/// File system volume label information
typedef struct EFI_FILE_SYSTEM_VOLUME_LABEL_INFO EFI_FILE_SYSTEM_VOLUME_LABEL_INFO;
struct EFI_FILE_SYSTEM_VOLUME_LABEL_INFO {

  // VolumeLabel
  /// The file system volume label
  CHAR16 VolumeLabel[1];

};

// EFI_FILE_PROTOCOL
/// File protocol
typedef struct EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;

//
// File operations (Revision 1.0+)
//

// EFI_FILE_OPEN
/// Opens a new file relative to the source file’s location
/// @param This       The file protocol interface
/// @param NewHandle  On output, the opened handle for the new file
/// @param FileName   The null-terminated Unicode string of the name of the file to be opened. The file name may contain the following path modifiers: "\", ".", and ".."
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_OPEN) (
  IN  EFI_FILE_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL **NewHandle,
  IN  CHAR16             *FileName,
  IN  UINT64              OpenMode,
  IN  UINT64              Attributes
);

// EFI_FILE_CLOSE
/// Closes a specified file handle
/// @param This The file protocol interface
/// @retval EFI_SUCCESS The file was closed
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_CLOSE) (
  IN EFI_FILE_PROTOCOL *This
);

// EFI_FILE_DELETE
/// Closes and deletes a file
/// @param This The file protocol interface
/// @retval EFI_SUCCESS             The file was closed and deleted, and the handle was closed
/// @retval EFI_WARN_DELETE_FAILURE The handle was closed, but the file was not deleted
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_DELETE) (
  IN EFI_FILE_PROTOCOL *This
);

// EFI_FILE_READ
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_READ) (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer
);

// EFI_FILE_WRITE
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_WRITE) (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  IN     VOID              *Buffer
);

// EFI_FILE_GET_POSITION
/// Returns a file’s current position
/// @param This     The file protocol interface
/// @param Position On output, the current byte position from the start of the file
/// @retval EFI_SUCCESS      The position was returned
/// @retval EFI_UNSUPPORTED  The request is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to get the position from a deleted file
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_GET_POSITION) (
  IN  EFI_FILE_PROTOCOL *This,
  OUT UINT64            *Position
);

// EFI_FILE_SET_POSITION
/// Sets a file’s current position
/// @param This     The file protocol interface
/// @param Position The byte position from the start of the file to set
/// @retval EFI_SUCCESS      The position was set
/// @retval EFI_UNSUPPORTED  The seek request for nonzero is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to set the position of a deleted file
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_SET_POSITION) (
  IN EFI_FILE_PROTOCOL *This,
  IN UINT64             Position
);

// EFI_FILE_GET_INFO
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_GET_INFO) (
  IN     EFI_FILE_PROTOCOL *This,
  IN     EFI_GUID          *InformationType,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer
);

// EFI_FILE_SET_INFO
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_SET_INFO) (
  IN EFI_FILE_PROTOCOL *This,
  IN EFI_GUID          *InformationType,
  IN UINTN              BufferSize,
  IN VOID              *Buffer
);

// EFI_FILE_FLUSH
/// Flushes all modified data associated with a file to a device
/// @param This The file protocol interface
/// @retval EFI_SUCCESS          The data was flushed
/// @retval EFI_NO_MEDIA         The device has no medium
/// @retval EFI_DEVICE_ERROR     The device reported an error
/// @retval EFI_VOLUME_CORRUPTED The file system structures are corrupted
/// @retval EFI_WRITE_PROTECTED  The file or medium is write-protected
/// @retval EFI_ACCESS_DENIED    The file was opened read-only
/// @retval EFI_VOLUME_FULL      The volume is full
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_FLUSH) (
  IN EFI_FILE_PROTOCOL *This
);

//
// Extended file operations (Revision 2.0+)
//

// EFI_FILE_IO_TOKEN
/// File token
typedef struct EFI_FILE_IO_TOKEN EFI_FILE_IO_TOKEN;
struct EFI_FILE_IO_TOKEN {

  // Event
  /// If NULL, then blocking I/O is performed, else if non-blocking I/O is supported, then non-blocking I/O is performed,
  ///  and the event will be signaled when the read request is completed. The caller must be prepared to handle the case
  ///  where the callback associated with Event occurs before the original asynchronous I/O request call returns
  EFI_EVENT   Event;
  // Status
  /// The status of the operation
  EFI_STATUS  Status;
  // BufferSize
  /// For OpenEx(): Not Used, ignored
  ///  For ReadEx(): On input, the size in bytes of the read buffer, on output, the size in bytes of the data read
  ///  For WriteEx(): On input, the size in bytes of the write buffer, on output, the size in bytes of the data written
  ///  For FlushEx(): Not used, ignored
  UINTN       BufferSize;
  // Buffer
  /// For OpenEx(): Not Used, ignored
  ///  For ReadEx(): The buffer into which the data is read
  ///  For WriteEx(): The buffer of data to write
  ///  For FlushEx(): Not Used, ignored
  VOID       *Buffer;

};

// EFI_FILE_OPEN_EX
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_OPEN_EX) (
  IN     EFI_FILE_PROTOCOL  *This,
  OUT    EFI_FILE_PROTOCOL **NewHandle,
  IN     CHAR16             *FileName,
  IN     UINT64              OpenMode,
  IN     UINT64              Attributes,
  IN OUT EFI_FILE_IO_TOKEN  *Token
);

// EFI_FILE_READ_EX
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_READ_EX) (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

// EFI_FILE_WRITE_EX
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_WRITE_EX) (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

// EFI_FILE_FLUSH_EX
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
typedef
EFI_STATUS
(EFIAPI
*EFI_FILE_FLUSH_EX) (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

// EFI_FILE_PROTOCOL
/// File protocol
struct EFI_FILE_PROTOCOL {

  // Revision
  /// The file protocol revision
  UINT64 Revision;

  //
  // File operations (Revision 1.0+)
  //

  // Open
  /// Opens or creates a new file
  EFI_FILE_OPEN         Open;
  // Close
  /// Closes the current file handle
  EFI_FILE_CLOSE        Close;
  // Delete
  /// Deletes a file
  EFI_FILE_DELETE       Delete;
  // Read
  /// Reads bytes from a file
  EFI_FILE_READ         Read;
  // Write
  /// Writes bytes to a file
  EFI_FILE_WRITE        Write;
  // GetPosition
  /// Returns the current file position
  EFI_FILE_GET_POSITION GetPosition;
  // SetPosition
  /// Sets the current file position
  EFI_FILE_SET_POSITION SetPosition;
  // GetInfo
  /// Gets the requested file or volume information
  EFI_FILE_GET_INFO     GetInfo;
  // SetInfo
  /// Sets the requested file information
  EFI_FILE_SET_INFO     SetInfo;
  // Flush
  /// Flushes all modified data associated with the file to the device
  EFI_FILE_FLUSH        Flush;

  //
  // Extended file operations (Revision 2.0+)
  //

  // OpenEx
  /// Opens a new file relative to the source directory’s location
  EFI_FILE_OPEN_EX  OpenEx;
  // ReadEx
  /// Reads data from a file
  EFI_FILE_READ_EX  ReadEx;
  // WriteEx
  /// Writes data to a file
  EFI_FILE_WRITE_EX WriteEx;
  // FlushEx
  /// Flushes all modified data associated with a file to a device
  EFI_FILE_FLUSH_EX FlushEx;

};

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpen (
  IN  EFI_FILE_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL **NewHandle,
  IN  CONST CHAR16       *FileName OPTIONAL,
  IN  UINT64              OpenMode,
  IN  UINT64              Attributes
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpenPath (
  OUT EFI_FILE_PROTOCOL **NewHandle,
  IN  CONST CHAR16       *DevicePath,
  IN  UINT64              OpenMode,
  IN  UINT64              Attributes
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpenDevicePath (
  OUT EFI_FILE_PROTOCOL        **NewHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN  UINT64                     OpenMode,
  IN  UINT64                     Attributes
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileExists (
  IN  EFI_FILE_PROTOCOL *This,
  IN  CONST CHAR16      *FileName
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileExistsPath (
  IN CONST CHAR16 *DevicePath
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileExistsDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileLoad (
  IN  EFI_FILE_PROTOCOL  *This,
  IN  CONST CHAR16       *FileName,
  OUT UINT64             *Size,
  OUT VOID              **Buffer
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileLoadPath (
  IN  CONST CHAR16  *DevicePath,
  OUT UINT64        *Size,
  OUT VOID         **Buffer
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileLoadDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT64                    *Size,
  OUT VOID                     **Buffer
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileSave (
  IN EFI_FILE_PROTOCOL *This,
  IN CONST CHAR16      *FileName,
  IN UINT64             Size,
  IN VOID              *Buffer
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileSavePath (
  IN CONST CHAR16 *DevicePath,
  IN UINT64        Size,
  IN VOID         *Buffer
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileSaveDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN UINT64                    Size,
  IN VOID                     *Buffer
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileLocateDevicePath (
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT    EFI_HANDLE                *Device,
  OUT    CHAR16                   **FileName
);

// EfiFileClose
/// Closes a specified file handle
/// @param This The file protocol interface
/// @retval EFI_SUCCESS The file was closed
EXTERN
EFI_STATUS
EFIAPI
EfiFileClose (
  IN EFI_FILE_PROTOCOL *This
);

// EfiFileDelete
/// Closes and deletes a file
/// @param This The file protocol interface
/// @retval EFI_SUCCESS             The file was closed and deleted, and the handle was closed
/// @retval EFI_WARN_DELETE_FAILURE The handle was closed, but the file was not deleted
EXTERN
EFI_STATUS
EFIAPI
EfiFileDelete (
  IN EFI_FILE_PROTOCOL *This
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileRead (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer OPTIONAL
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileWrite (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT UINTN             *BufferSize,
  IN     VOID              *Buffer
);

// EfiFileGetPosition
/// Returns a file’s current position
/// @param This     The file protocol interface
/// @param Position On output, the current byte position from the start of the file
/// @retval EFI_SUCCESS      The position was returned
/// @retval EFI_UNSUPPORTED  The request is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to get the position from a deleted file
EXTERN
EFI_STATUS
EfiFileGetPosition (
  IN  EFI_FILE_PROTOCOL *This,
  OUT UINT64            *Position
);

// EfiFileSetPosition
/// Sets a file’s current position
/// @param This     The file protocol interface
/// @param Position The byte position from the start of the file to set
/// @retval EFI_SUCCESS      The position was set
/// @retval EFI_UNSUPPORTED  The seek request for nonzero is not valid on open directories
/// @retval EFI_DEVICE_ERROR An attempt was made to set the position of a deleted file
EXTERN
EFI_STATUS
EFIAPI
EfiFileSetPosition (
  IN EFI_FILE_PROTOCOL *This,
  IN UINT64             Position
);

// EfiFileGetSize
/// Get the size in bytes of a file
/// @param This The file protocol interface
/// @param Size On output, the size in bytes of the file
/// @retval EFI_SUCCESS           The size of the file was returned
/// @retval EFI_INVALID_PARAMETER If This or Size is NULL
/// @retval EFI_NOT_FOUND         The file does not exist or has no size
EXTERN
EFI_STATUS
EFIAPI
EfiFileGetSize (
  IN  EFI_FILE_PROTOCOL *This,
  OUT UINT64            *Size
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileGetInfo (
  IN     EFI_FILE_PROTOCOL *This,
  IN     EFI_GUID          *InformationType,
  IN OUT UINTN             *BufferSize,
  OUT    VOID              *Buffer OPTIONAL
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiSetFileInfo (
  IN EFI_FILE_PROTOCOL *This,
  IN EFI_GUID          *InformationType,
  IN UINTN              BufferSize,
  IN VOID              *Buffer
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileFlush (
  IN EFI_FILE_PROTOCOL *This
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpenEx (
  IN     EFI_FILE_PROTOCOL  *This,
  OUT    EFI_FILE_PROTOCOL **NewHandle,
  IN     CHAR16             *FileName,
  IN     UINT64              OpenMode,
  IN     UINT64              Attributes,
  IN OUT EFI_FILE_IO_TOKEN  *Token
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileReadEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileWriteEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileFlushEx (
  IN     EFI_FILE_PROTOCOL *This,
  IN OUT EFI_FILE_IO_TOKEN *Token
);

// EfiFileInfo
/// Get file information
/// @param This The file protocol interface
/// @return The file information which must be freed or NULL if there was an error or no file information
EXTERN
EFI_FILE_INFO *
EFIAPI
EfiFileInfo (
  IN EFI_FILE_PROTOCOL *This
);
// EfiFileSystemInfo
/// Get file system information
/// @param This The file protocol interface
/// @return The file system information which must be freed or NULL if there was an error or no file system information
EXTERN
EFI_FILE_SYSTEM_INFO *
EFIAPI
EfiFileSystemInfo (
  IN EFI_FILE_PROTOCOL *This
);
// EfiFileSystemVolumeLabelInfo
/// Get file system volume label information
/// @param This The file protocol interface
/// @return The file system volume label information which must be freed or NULL if there was an error or no file system information
EXTERN
EFI_FILE_SYSTEM_VOLUME_LABEL_INFO *
EFIAPI
EfiFileSystemVolumeLabelInfo (
  IN EFI_FILE_PROTOCOL *This
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_FILE_PROTOCOL_HEADER__
