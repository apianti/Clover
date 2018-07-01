///
/// @file Include/Uefi/Protocol/LoadedImage.h
///
/// UEFI implementation loaded image protocol
///

#pragma once
#ifndef __UEFI_LOADED_IMAGE_PROTOCOL_HEADER__
#define __UEFI_LOADED_IMAGE_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_LOADED_IMAGE_PROTOCOL_GUID
/// The loaded image protocol unique identifier
#define EFI_LOADED_IMAGE_PROTOCOL_GUID { 0x5B1B31A1, 0x9562, 0x11D2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

// EFI_LOADED_IMAGE_PROTOCOL_REVISION
/// The loaded image protocol revision
#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

// gEfiLoadedImageProtocolGuid
/// The loaded image protocol unique identifier
EXTERN EFI_GUID gEfiLoadedImageProtocolGuid;

// EFI_LOADED_IMAGE_PROTOCOL
/// Loaded image protocol
typedef struct EFI_LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL;
struct EFI_LOADED_IMAGE_PROTOCOL {

  // Revision
  /// The revision of this loaded image protocol interface
  UINT32                    Revision;
  // ParentHandle
  /// The image handle of the parent image or NULL if the image is loaded directly from the firmware
  EFI_HANDLE                ParentHandle;
  // SystemTable
  /// The image system table
  EFI_SYSTEM_TABLE         *SystemTable;
  // DeviceHandle
  /// The handle of the device from which the image was loaded
  EFI_HANDLE                DeviceHandle;
  // FilePath
  /// The file path device path
  EFI_DEVICE_PATH_PROTOCOL *FilePath;
  // Reserved
  /// Reserved
  VOID                     *Reserved;
  // LoadOptionsSize
  /// The size in bytes of the load options
  UINTN                     LoadOptionsSize;
  // LoadOptions
  /// The load options for the image
  VOID                     *LoadOptions;
  // ImageBase
  /// The base address at which the image was loaded
  VOID                     *ImageBase;
  // ImageSize
  /// The size in bytes of the loaded image
  UINT64                    ImageSize;
  // ImageCodeType
  /// The image code memory type
  EFI_MEMORY_TYPE           ImageCodeType;
  // ImageDataType
  /// The image data memory type
  EFI_MEMORY_TYPE           ImageDataType;
  // Unload
  /// The image unload method
  EFI_IMAGE_UNLOAD          Unload;

};

// EfiLoadedImage
/// Get the loaded image protocol interface for an image
/// @param ImageHandle The image handle for which to get the loaded image protocol interface
/// @return The loaded image protocol interface
EXTERN
EFI_LOADED_IMAGE_PROTOCOL *
EFIAPI
EfiLoadedImage (
  IN EFI_HANDLE ImageHandle OPTIONAL
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_LOADED_IMAGE_PROTOCOL_HEADER__
