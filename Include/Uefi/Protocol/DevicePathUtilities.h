///
/// @file Include/Uefi/Protocol/DevicePathUtilities.h
///
/// UEFI implementation device path utilities protocol
///

#pragma once
#ifndef ___UEFI_DEVICE_PATH_UTILITIES_PROTOCOL_HEADER__
#define ___UEFI_DEVICE_PATH_UTILITIES_PROTOCOL_HEADER__

#include <Uefi/Protocol/DevicePath.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID
/// Device path utilities protocol unique identifier
#define EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID { 0x379BE4E, 0xD706, 0x437D, { 0xB0, 0x37, 0xED, 0xB8, 0x2F, 0xB7, 0x72, 0xA4 } }

// gEfiDevicePathUtilitiesProtocolGuid
/// Device path utilities protocol unique identifier
EXTERN EFI_GUID gEfiDevicePathUtilitiesProtocolGuid;

// EFI_DEVICE_PATH_UTILITIES_PROTOCOL
/// Device path utilities protocol
typedef struct EFI_DEVICE_PATH_UTILITIES_PROTOCOL EFI_DEVICE_PATH_UTILITIES_PROTOCOL;

// EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE
/// Returns the size of the device path, in bytes
/// @param DevicePath The device path of which to get the size
/// @return The size of the device path in bytes
typedef
UINTN
(EFIAPI
*EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH
/// Create a duplicate of the specified path
/// @param DevicePath The device path of which to duplicate
/// @return The duplicate device path which must be freed or NULL if the device path could not be allocated
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EFI_DEVICE_PATH_UTILS_APPEND_PATH
/// Create a new path by appending the second device path to the first
/// @param Src1 The first device path
/// @param Src2 The second device path
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_APPEND_PATH) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src2
);

// EFI_DEVICE_PATH_UTILS_APPEND_NODE
/// Creates a new path by appending the device node to the device path
/// @param DevicePath The device path
/// @param DeviceNode The device node
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_APPEND_NODE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode
);

// EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE
/// Creates a new path by appending the specified device path instance to the specified device path
/// @param DevicePath         The device path
/// @param DevicePathInstance The device path instance
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath OPTIONAL,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance
);

// EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE
/// Creates a copy of the current device path instance and returns a pointer to the next device path instance
/// @param DevicePathInstance     On input, the current device path instance, on output, the next device path instance or NULL if there are no more device path instances in the device path
/// @param DevicePathInstanceSize On output, the size in bytes of the created device path instance
/// @return The newly created device path instance which must be freed or NULL if the device path instance could not be allocated
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE) (
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePathInstance,
  OUT    UINTN                     *DevicePathInstanceSize OPTIONAL
);

// EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE
/// Returns whether a device path is multi-instance
/// @param DevicePath The device path to check is multi-instance
/// @return Whether the device path is multi-instance or not
typedef
BOOLEAN
(EFIAPI
*EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE) (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EFI_DEVICE_PATH_UTILS_CREATE_NODE
/// Creates a device node
/// @param NodeType    The device node type for the new device node
/// @param NodeSubType The device node sub-type for the new device node
/// @param NodeLength  The length of the device node for the new device node
/// @return The newly created device node which must be freed or NULL if the device node could not be allocated or NodeLength is less than the size of the device path header
typedef
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI
*EFI_DEVICE_PATH_UTILS_CREATE_NODE) (
  IN UINT8  NodeType,
  IN UINT8  NodeSubType,
  IN UINT16 NodeLength
);

// EFI_DEVICE_PATH_UTILITIES_PROTOCOL
/// Device path utilities protocol
struct EFI_DEVICE_PATH_UTILITIES_PROTOCOL {

  // GetDevicePathSize
  /// Returns the size of the specified device path, in bytes
  EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE GetDevicePathSize;
  // DuplicateDevicePath
  /// Duplicates a device path structure
  EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH      DuplicateDevicePath;
  // AppendDevicePath
  /// Appends the device path to the specified device path
  EFI_DEVICE_PATH_UTILS_APPEND_PATH          AppendDevicePath;
  // AppendDeviceNode
  /// Appends the device node to the specified device path
  EFI_DEVICE_PATH_UTILS_APPEND_NODE          AppendDeviceNode;
  // AppendDevicePathInstance
  /// Appends a device path instance to another device path
  EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE      AppendDevicePathInstance;
  // GetNextDevicePathInstance
  /// Retrieves the next device path instance from device path data
  EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE    GetNextDevicePathInstance;
  // IsDevicePathMultiInstance
  /// Returns TRUE if this is a multi-instance device path
  EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE    IsDevicePathMultiInstance;
  // CreateDeviceNode
  /// Allocates memory for a device node with the specified type and sub-type
  EFI_DEVICE_PATH_UTILS_CREATE_NODE          CreateDeviceNode;

};

// EfiGetDevicePathSize
/// Returns the size of the device path, in bytes
/// @param DevicePath The device path of which to get the size
/// @return The size of the device path in bytes
EXTERN
UINTN
EFIAPI
EfiGetDevicePathSize (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EfiDuplicateDevicePath
/// Create a duplicate of the specified path
/// @param DevicePath The device path of which to duplicate
/// @return The duplicate device path which must be freed or NULL if the device path could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiDuplicateDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EfiAppendDevicePath
/// Create a new path by appending the second device path to the first
/// @param Src1 The first device path
/// @param Src2 The second device path
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src2
);

// EfiAppendDeviceNode
/// Creates a new path by appending the device node to the device path
/// @param DevicePath The device path
/// @param DeviceNode The device node
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDeviceNode (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode
);

// EfiAppendDevicePathInstance
/// Creates a new path by appending the specified device path instance to the specified device path
/// @param DevicePath         The device path
/// @param DevicePathInstance The device path instance
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDevicePathInstance (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath OPTIONAL,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance
);

// EfiGetNextDevicePathInstance
/// Creates a copy of the current device path instance and returns a pointer to the next device path instance
/// @param DevicePathInstance     On input, the current device path instance, on output, the next device path instance or NULL if there are no more device path instances in the device path
/// @param DevicePathInstanceSize On output, the size in bytes of the created device path instance
/// @return The newly created device path instance which must be freed or NULL if the device path instance could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiGetNextDevicePathInstance (
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePathInstance,
  OUT    UINTN                     *DevicePathInstanceSize OPTIONAL
);

// EfiIsDevicePathMultiInstance
/// Returns whether a device path is multi-instance
/// @param DevicePath The device path to check is multi-instance
/// @return Whether the device path is multi-instance or not
EXTERN
BOOLEAN
EFIAPI
EfiIsDevicePathMultiInstance (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// EfiCreateDeviceNode
/// Creates a device node
/// @param NodeType    The device node type for the new device node
/// @param NodeSubType The device node sub-type for the new device node
/// @param NodeLength  The length of the device node for the new device node
/// @return The newly created device node which must be freed or NULL if the device node could not be allocated or NodeLength is less than the size of the device path header
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiCreateDeviceNode (
  IN UINT8  NodeType,
  IN UINT8  NodeSubType,
  IN UINT16 NodeLength
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_DEVICE_PATH_UTILITIES_PROTOCOL_HEADER__
