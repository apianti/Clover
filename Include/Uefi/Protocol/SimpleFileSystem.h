///
/// @file Include/Uefi/Protocol/SimpleFileSystem.h
///
/// UEFI implementation simple file system protocol
///

#pragma once
#ifndef ___UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_HEADER__
#define ___UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_HEADER__

#include <Uefi/Protocol/File.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID
/// Simple file system protocol unique identiifer
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID { 0x0964E5B22, 0x6459, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

// EFI_PART_TYPE_UNUSED_GUID
/// The unused partition type unique identifier
#define EFI_PART_TYPE_UNUSED_GUID { 0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
// EFI_PART_TYPE_EFI_SYSTEM_PART_GUID
/// The EFI system partition type unique identifier
#define EFI_PART_TYPE_EFI_SYSTEM_PART_GUID { 0xc12a7328, 0xf81f, 0x11d2, {0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b } }
// EFI_PART_TYPE_LEGACY_MBR_GUID
/// The legacy partition type unique identifier
#define EFI_PART_TYPE_LEGACY_MBR_GUID { 0x024dee41, 0x33e7, 0x11d3, {0x9d, 0x69, 0x00, 0x08, 0xc7, 0x81, 0xf3, 0x9f } }

// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION
/// Simple file system protocol revision
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION 0x00010000

// gEfiSimpleFileSystemProtocolGuid
/// Simple file system protocol unique identiifer
EXTERN EFI_GUID gEfiSimpleFileSystemProtocolGuid;

// gEfiPartTypeUnusedGuid
/// The unused partition type unique identifier
EXTERN EFI_GUID gEfiPartTypeUnusedGuid;
// gEfiPartTypeSystemPartGuid
/// The EFI system partition type unique identifier
EXTERN EFI_GUID gEfiPartTypeSystemPartGuid;
// gEfiPartTypeLegacyMbrGuid
/// The legacy partition type unique identifier
EXTERN EFI_GUID gEfiPartTypeLegacyMbrGuid;

// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
/// Simple file system protocol
typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME
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
typedef
EFI_STATUS
(EFIAPI
*EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME) (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL               **Root
);

// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
/// Simple file system protocol
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {

  // Revision
  /// The version of the simple file system protocol
  UINT64                                      Revision;
  // OpenVolume
  /// Opens the volume for file I/O access
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;

};

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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpenRoot (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *This,
  OUT EFI_FILE_PROTOCOL               **Root
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiFileOpenRootByHandle (
  IN  EFI_HANDLE          Handle,
  OUT EFI_FILE_PROTOCOL **Root
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SIMPLE_FILE_SYSTEM_PROTOCOL_HEADER__
