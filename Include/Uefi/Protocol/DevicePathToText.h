///
/// @file Include/Uefi/Protocol/DevicePathToText.h
///
/// UEFI implementation device path to text protocol
///

#pragma once
#ifndef ___UEFI_DEVICE_PATH_TO_TEXT_PROTOCOL_HEADER__
#define ___UEFI_DEVICE_PATH_TO_TEXT_PROTOCOL_HEADER__

#include <Uefi/Protocol/DevicePath.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID
/// Device path to text protocol unique identifier
#define EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID { 0x8B843E20, 0x8132, 0x4852, { 0x90, 0xCC, 0x55, 0x1A, 0x4E, 0x4A, 0x7F, 0x1C } }

// gEfiDevicePathToTextProtocolGuid
/// Device path to text protocol unique identifier
EXTERN EFI_GUID gEfiDevicePathToTextProtocolGuid;

// EFI_DEVICE_PATH_TO_TEXT_PROTOCOL
/// Device path to text protocol
typedef struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

// EFI_DEVICE_PATH_TO_TEXT_NODE
/// Convert a device node to its text representation
/// @param DeviceNode     The device node to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
typedef
CHAR16 *
(EFIAPI
*EFI_DEVICE_PATH_TO_TEXT_NODE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
);

// EFI_DEVICE_PATH_TO_TEXT_PATH
/// Convert a device path to its text representation
/// @param DevicePath     The device path to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
typedef
CHAR16 *
(EFIAPI
*EFI_DEVICE_PATH_TO_TEXT_PATH) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
);

// EFI_DEVICE_PATH_TO_TEXT_PROTOCOL
/// Device path to text protocol
struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {

  // ConvertDeviceNodeToText
  /// Converts a device node to text
  EFI_DEVICE_PATH_TO_TEXT_NODE ConvertDeviceNodeToText;
  // ConvertDevicePathToText
  /// Converts a device path to text
  EFI_DEVICE_PATH_TO_TEXT_PATH ConvertDevicePathToText;

};

// EfiConvertDeviceNodeToText
/// Convert a device node to its text representation
/// @param DeviceNode     The device node to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
EXTERN
CHAR16 *
EFIAPI
EfiConvertDeviceNodeToText (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
);

// EfiDevicePathToText
/// Convert a device path to its text representation
/// @param DevicePath     The device path to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
EXTERN
CHAR16 *
EFIAPI
EfiDevicePathToText (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_DEVICE_PATH_TO_TEXT_PROTOCOL_HEADER__
