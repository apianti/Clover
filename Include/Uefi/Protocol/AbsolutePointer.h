///
/// @file Include/Uefi/Protocol/AbsolutePointer.h
///
/// UEFI implementation absolute pointer protocol
///

#pragma once
#ifndef ___UEFI_ABSOLUTE_POINTER_PROTOCOL_HEADER__
#define ___UEFI_ABSOLUTE_POINTER_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_ABSOLUTE_POINTER_PROTOCOL_GUID
/// Absolute pointer protocol unique identifier
#define EFI_ABSOLUTE_POINTER_PROTOCOL_GUID { 0x8D59D32B, 0xC655, 0x4AE9, { 0x9B, 0x15, 0xF2, 0x59, 0x04, 0x99, 0x2A, 0x43 } }

// EFI_ABSP_SupportsAltActive
/// Indicates this device supports an alternate button input
#define EFI_ABSP_SupportsAltActive 0x00000001
// EFI_ABSP_SupportsPressureAsZ
/// Indicates this device returns pressure data in parameter CurrentZ
#define EFI_ABSP_SupportsPressureAsZ 0x00000002

// EFI_ABSP_TouchActive
/// Indicates the touch sensor is active
#define EFI_ABSP_TouchActive 0x00000001
// EFI_ABSP_AltActive
/// Indicates the alt sensor, such as pen-side button, is active
#define EFI_ABSP_AltActive 0x00000002

// gEfiAbsolutePointerProtocolGuid
/// Absolute pointer protocol unique identifier
EXTERN EFI_GUID gEfiAbsolutePointerProtocolGuid;

// EFI_ABSOLUTE_POINTER_MODE
/// Absolute pointer mode
typedef struct EFI_ABSOLUTE_POINTER_MODE EFI_ABSOLUTE_POINTER_MODE;
struct EFI_ABSOLUTE_POINTER_MODE {

  // AbsoluteMinX
  /// The absolute minimum of the device on the x-axis
  UINT64 AbsoluteMinX;
  // AbsoluteMinY
  /// The absolute minimum of the device on the y-axis
  UINT64 AbsoluteMinY;
  // AbsoluteMinZ
  /// The absolute minimum of the device on the z-axis
  UINT64 AbsoluteMinZ;
  // AbsoluteMaxX
  /// The absolute maximum of the device on the x-axis. If 0, and AbsoluteMinX is 0, then the pointer device does not support a x-axis
  UINT64 AbsoluteMaxX;
  // AbsoluteMaxY
  /// The absolute maximum of the device on the y-axis. If 0, and AbsoluteMinY is 0, then the pointer device does not support a y-axis
  UINT64 AbsoluteMaxY;
  // AbsoluteMaxZ
  /// The absolute maximum of the device on the z-axis. If 0, and AbsoluteMinZ is 0, then the pointer device does not support a z-axis
  UINT64 AbsoluteMaxZ;
  // Attributes
  /// Indicates the supported capabilities of the device
  UINT32 Attributes;

};

// EFI_ABSOLUTE_POINTER_STATE
/// Absolute pointer state information
typedef struct EFI_ABSOLUTE_POINTER_STATE EFI_ABSOLUTE_POINTER_STATE;
struct EFI_ABSOLUTE_POINTER_STATE {

  // CurrentX
  /// The unsigned position of the activation on the x-axis
  UINT64 CurrentX;
  // CurrentY
  /// The unsigned position of the activation on the y-axis
  UINT64 CurrentY;
  // CurrentZ
  /// The unsigned position of the activation on the z-axis
  UINT64 CurrentZ;
  // ActiveButtons
  /// Bitmask of active buttons
  UINT32 ActiveButtons;

};

// EFI_ABSOLUTE_POINTER_PROTOCOL
/// Absolute pointer protocol
typedef struct EFI_ABSOLUTE_POINTER_PROTOCOL EFI_ABSOLUTE_POINTER_PROTOCOL;

// EFI_ABSOLUTE_POINTER_RESET
/// Resets the pointer device hardware
/// @param This                 The absolute pointer protocol interface
/// @param ExtendedVerification Indicates that the driver may perform an extended verification of the device during reset
/// @retval EFI_SUCCESS      The device was reset
/// @retval EFI_DEVICE_ERROR The device is not functioning correctly and could not be reset
typedef
EFI_STATUS
(EFIAPI
*EFI_ABSOLUTE_POINTER_RESET) (
  IN EFI_ABSOLUTE_POINTER_PROTOCOL *This,
  IN BOOLEAN                        ExtendedVerification
);

// EFI_ABSOLUTE_POINTER_GET_STATE
/// Retrieves the current state of a pointer device
/// @param This  The absolute pointer protocol interface
/// @param State On output, the state information for the pointer device
/// @retval EFI_SUCCESS The state of the pointer device was returned in State
/// @retval EFI_NOT_READY The state of the pointer device has not changed since the last call to GetState()
/// @retval EFI_DEVICE_ERROR A device error occurred while attempting to retrieve the pointer device's current state
typedef
EFI_STATUS
(EFIAPI
*EFI_ABSOLUTE_POINTER_GET_STATE) (
  IN     EFI_ABSOLUTE_POINTER_PROTOCOL *This,
  IN OUT EFI_ABSOLUTE_POINTER_STATE    *State
);

// EFI_ABSOLUTE_POINTER_PROTOCOL
/// Absolute pointer protocol
struct EFI_ABSOLUTE_POINTER_PROTOCOL {

  // Reset
  /// Resets the pointer device
  EFI_ABSOLUTE_POINTER_RESET      Reset;
  // GetState
  /// Retrieves the current state of the pointer device
  EFI_ABSOLUTE_POINTER_GET_STATE  GetState;
  // WaitForInput
  /// Event to use to wait for input from the pointer device
  EFI_EVENT                       WaitForInput;
  // Mode
  /// The absolute pointer mode
  EFI_ABSOLUTE_POINTER_MODE      *Mode;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_ABSOLUTE_POINTER_PROTOCOL_HEADER__
