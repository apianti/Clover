///
/// @file Include/Uefi/Protocol/SimplePointer.h
///
/// UEFI implementation simple pointer protocol
///

#pragma once
#ifndef ___UEFI_SIMPLE_POINTER_PROTOCOL_HEADER__
#define ___UEFI_SIMPLE_POINTER_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SIMPLE_POINTER_PROTOCOL_GUID
/// Simple pointer protocol unique identifier
#define EFI_SIMPLE_POINTER_PROTOCOL_GUID { 0x31878C87, 0xB75, 0x11D5, { 0x9A, 0x4F, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } }

// gEfiSimplePointerProtocolGuid
/// Simple pointer protocol unique identifier
EXTERN EFI_GUID gEfiSimplePointerProtocolGuid;

// EFI_SIMPLE_POINTER_MODE
/// Simple pointer mode
typedef struct EFI_SIMPLE_POINTER_MODE EFI_SIMPLE_POINTER_MODE;
struct EFI_SIMPLE_POINTER_MODE {

  // ResolutionX
  /// The resolution of the pointer device on the x-axis in counts/mm. If 0, then the pointer device does not support an x-axis
  UINT64  ResolutionX;
  // ResolutionY
  /// The resolution of the pointer device on the y-axis in counts/mm. If 0, then the pointer device does not support a y-axis
  UINT64  ResolutionY;
  // ResolutionZ
  /// The resolution of the pointer device on the z-axis in counts/mm. If 0, then the pointer device does not support a z-axis
  UINT64  ResolutionZ;
  // LeftButton
  /// TRUE if a left button is present on the pointer device, otherwise FALSE
  BOOLEAN LeftButton;
  // RightButton
  /// TRUE if a right button is present on the pointer device, otherwise FALSE
  BOOLEAN RightButton;

};

// EFI_SIMPLE_POINTER_STATE
/// Simple pointer state information
typedef struct EFI_SIMPLE_POINTER_STATE EFI_SIMPLE_POINTER_STATE;
struct EFI_SIMPLE_POINTER_STATE {

  // RelativeMovementX
  /// The signed distance in counts that the pointer device has been moved along the x-axis. The actual distance
  /// moved is RelativeMovementX / ResolutionX millimeters. If the ResolutionX field of the EFI_SIMPLE_POINTER_MODE
  /// structure is 0, then this pointer device does not support an x-axis, and this field must be ignored
  INT32   RelativeMovementX;
  // RelativeMovementY
  /// The signed distance in counts that the pointer device has been moved along the y-axis. The actual distance
  ///  moved is RelativeMovementY / ResolutionY millimeters. If the ResolutionY field of the EFI_SIMPLE_POINTER_MODE
  ///  structure is 0, then this pointer device does not support a y-axis, and this field must be ignored
  INT32   RelativeMovementY;
  // RelativeMovementZ
  /// The signed distance in counts that the pointer device has been moved along the z-axis. The actual distance
  ///  moved is RelativeMovementZ / ResolutionZ millimeters. If the ResolutionZ field of the EFI_SIMPLE_POINTER_MODE
  ///  structure is 0, then this pointer device does not support a z-axis, and this field must be ignored
  INT32   RelativeMovementZ;
  // LeftButton
  /// Indicates if the left button is being pressed
  BOOLEAN LeftButton;
  // RightButton
  /// Indicates if the right button is being pressed
  BOOLEAN RightButton;

};

// EFI_SIMPLE_POINTER_PROTOCOL
/// Simple pointer protocol
typedef struct EFI_SIMPLE_POINTER_PROTOCOL EFI_SIMPLE_POINTER_PROTOCOL;

// EFI_SIMPLE_POINTER_RESET
/// Resets the pointer device hardware
/// @param This                 The simple pointer protocol interface
/// @param ExtendedVerification Indicates that the driver may perform an extended verification of the device during reset
/// @retval EFI_SUCCESS      The device was reset
/// @retval EFI_DEVICE_ERROR The device is not functioning correctly and could not be reset
typedef
EFI_STATUS
(EFIAPI
*EFI_SIMPLE_POINTER_RESET) (
  IN EFI_SIMPLE_POINTER_PROTOCOL *This,
  IN BOOLEAN                      ExtendedVerification
);

// EFI_SIMPLE_POINTER_GET_STATE
/// Retrieves the current state of a pointer device
/// @param This  The simple pointer protocol interface
/// @param State On output, the state information for the pointer device
/// @retval EFI_SUCCESS The state of the pointer device was returned in State
/// @retval EFI_NOT_READY The state of the pointer device has not changed since the last call to GetState()
/// @retval EFI_DEVICE_ERROR A device error occurred while attempting to retrieve the pointer device's current state
typedef
EFI_STATUS
(EFIAPI
*EFI_SIMPLE_POINTER_GET_STATE) (
  IN     EFI_SIMPLE_POINTER_PROTOCOL *This,
  IN OUT EFI_SIMPLE_POINTER_STATE    *State
);

// EFI_SIMPLE_POINTER_PROTOCOL
/// Simple pointer protocol
struct EFI_SIMPLE_POINTER_PROTOCOL {

  // Reset
  /// Resets the pointer device
  EFI_SIMPLE_POINTER_RESET      Reset;
  // GetState
  /// Retrieves the current state of the pointer device
  EFI_SIMPLE_POINTER_GET_STATE  GetState;
  // WaitForInput
  /// Event to use to wait for input from the pointer device
  EFI_EVENT                     WaitForInput;
  // Mode
  /// The simple pointer mode
  EFI_SIMPLE_POINTER_MODE      *Mode;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SIMPLE_POINTER_PROTOCOL_HEADER__
