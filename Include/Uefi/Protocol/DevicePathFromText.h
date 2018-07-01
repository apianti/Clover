///
/// @file Include/Uefi/Protocol/DevicePathFromText.h
///
/// UEFI implementation device path from text protocol
///

#pragma once
#ifndef ___UEFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_HEADER__
#define ___UEFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_HEADER__

#include <Uefi/Protocol/DevicePath.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID
/// Device path from text protocol unique identifier
#define EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID { 0x5C99A21, 0xC70F, 0x4AD2, { 0x8A, 0x5F, 0x35, 0xDF, 0x33, 0x43, 0xF5, 0x1E } }

// gEfiDevicePathFromTextProtocolGuid
/// Device path from text protocol unique identifier
EXTERN EFI_GUID gEfiDevicePathFromTextProtocolGuid;

// EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL
/// Device path from text protocol
typedef struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

// EFI_DEVICE_PATH_FROM_TEXT_NODE
/// Convert text to the binary representation of a device node
/// @param TextDeviceNode The text representation of a device node, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device node which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_FROM_TEXT_NODE) (
  IN CONST CHAR16 *TextDeviceNode
);

// EFI_DEVICE_PATH_FROM_TEXT_PATH
/// Convert a text to its binary device path representation
/// @param TextDeviceNode The text representation of a device path, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device path which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_FROM_TEXT_PATH) (
  IN CONST CHAR16 *TextDevicePath
);

// EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL
/// Device path from text protocol
struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {

  // ConvertTextToDeviceNode
  /// Converts text to a device node
  EFI_DEVICE_PATH_FROM_TEXT_NODE ConvertTextToDeviceNode;
  // ConvertTextToDevicePath
  /// Converts text to a device path
  EFI_DEVICE_PATH_FROM_TEXT_PATH ConvertTextToDevicePath;

};

// EfiConvertTextToDeviceNode
/// Convert text to the binary representation of a device node
/// @param TextDeviceNode The text representation of a device node, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device node which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiConvertTextToDeviceNode (
  IN CONST CHAR16 *TextDeviceNode
);
// EfiConvertTextToDevicePath
/// Convert a text to its binary device path representation
/// @param TextDevicePath The text representation of a device path, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device path which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiConvertTextToDevicePath (
  IN CONST CHAR16 *TextDevicePath
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_HEADER__
