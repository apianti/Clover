///
/// @file Include/Uefi/Protocol/SimpleTextInput.h
///
/// UEFI implementation extended simple text input protocol
///

#pragma once
#ifndef ___UEFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_HEADER__
#define ___UEFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID
/// Extended simple text input protocol unique identifier
#define EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID { 0xDD9E7534, 0x7762, 0x4698, { 0x8C, 0x14, 0xF5, 0x85, 0x17, 0xA6, 0x25, 0xAA } }

// Extended key scan codes

// SCAN_F11
/// Function 11
#define SCAN_F11 0x0015
// SCAN_F12
/// Function 12
#define SCAN_F12 0x0016
// SCAN_PAUSE
/// 
#define SCAN_PAUSE 0x0048
// SCAN_F13
/// Function 13
#define SCAN_F13 0x0068
// SCAN_F14
/// Function 14
#define SCAN_F14 0x0069
// SCAN_F15
/// Function 15
#define SCAN_F15 0x006A
// SCAN_F16
/// Function 16
#define SCAN_F16 0x006B
// SCAN_F17
/// Function 17
#define SCAN_F17 0x006C
// SCAN_F18
/// Function 18
#define SCAN_F18 0x006D
// SCAN_F19
/// Function 19
#define SCAN_F19 0x006E
// SCAN_F20
/// Function 20
#define SCAN_F20 0x006F
// SCAN_F21
/// Function 21
#define SCAN_F21 0x0070
// SCAN_F22
/// Function 22
#define SCAN_F22 0x0071
// SCAN_F23
/// Function 23
#define SCAN_F23 0x0072
// SCAN_F24
/// Function 24
#define SCAN_F24 0x0073
// SCAN_MUTE
/// Mute
#define SCAN_MUTE 0x007F
// SCAN_VOLUME_UP
/// Volume up
#define SCAN_VOLUME_UP 0x0080
// SCAN_VOLUME_DOWN
/// Volume down
#define SCAN_VOLUME_DOWN 0x0081
// SCAN_BRIGHTNESS_UP
/// Brightness up
#define SCAN_BRIGHTNESS_UP 0x0100
// SCAN_BRIGHTNESS_DOWN
/// Brightness down
#define SCAN_BRIGHTNESS_DOWN 0x0101
// SCAN_SUSPEND
/// Suspend
#define SCAN_SUSPEND 0x0102
// SCAN_HIBERNATE
/// Hibernate
#define SCAN_HIBERNATE 0x0103
// SCAN_TOGGLE_DISPLAY
/// Toggle display
#define SCAN_TOGGLE_DISPLAY 0x0104
// SCAN_RECOVERY
/// Recovery
#define SCAN_RECOVERY 0x0105
// SCAN_EJECT
/// Eject
#define SCAN_EJECT 0x0106

// Extended key shift states documentation

// EFI_SHIFT_STATE_VALID
/// Shift state is valid
#define EFI_SHIFT_STATE_VALID 0x80000000
// EFI_RIGHT_SHIFT_PRESSED
/// Right shift pressed
#define EFI_RIGHT_SHIFT_PRESSED 0x00000001
// EFI_LEFT_SHIFT_PRESSED
/// Left shift pressed
#define EFI_LEFT_SHIFT_PRESSED 0x00000002
// EFI_RIGHT_CONTROL_PRESSED
/// Right control pressed
#define EFI_RIGHT_CONTROL_PRESSED 0x00000004
// EFI_LEFT_CONTROL_PRESSED
/// Left control pressed
#define EFI_LEFT_CONTROL_PRESSED 0x00000008
// EFI_RIGHT_ALT_PRESSED
/// Right alt pressed
#define EFI_RIGHT_ALT_PRESSED 0x00000010
// EFI_LEFT_ALT_PRESSED
/// Left alt pressed
#define EFI_LEFT_ALT_PRESSED 0x00000020
// EFI_RIGHT_LOGO_PRESSED
/// Right logo pressed
#define EFI_RIGHT_LOGO_PRESSED 0x00000040
// EFI_LEFT_LOGO_PRESSED
/// Left logo pressed
#define EFI_LEFT_LOGO_PRESSED 0x00000080
// EFI_MENU_KEY_PRESSED
/// Menu pressed
#define EFI_MENU_KEY_PRESSED 0x00000100
// EFI_SYS_REQ_PRESSED
/// System request pressed
#define EFI_SYS_REQ_PRESSED 0x00000200

// EFI_TOGGLE_STATE_VALID
/// Toggle state is valid
#define EFI_TOGGLE_STATE_VALID 0x80
// EFI_KEY_STATE_EXPOSED
/// All keystrokes are read including modifiers like shift, control, alt
#define EFI_KEY_STATE_EXPOSED 0x40
// EFI_SCROLL_LOCK_ACTIVE
/// Scroll lock active
#define EFI_SCROLL_LOCK_ACTIVE 0x01
// EFI_NUM_LOCK_ACTIVE
/// Num lock active
#define EFI_NUM_LOCK_ACTIVE 0x02
// EFI_CAPS_LOCK_ACTIVE
/// Caps lock active
#define EFI_CAPS_LOCK_ACTIVE 0x04

// gEfiSimpleTextInputExProtocolGuid
/// Extended simple text input protocol unique identifier
EXTERN EFI_GUID gEfiSimpleTextInputExProtocolGuid;

// EFI_KEY_TOGGLE_STATE
/// Key toggle state information
typedef UINT8 EFI_KEY_TOGGLE_STATE;

// EFI_KEY_STATE
/// Key state information
typedef struct EFI_KEY_STATE EFI_KEY_STATE;
struct EFI_KEY_STATE {

  // KeyShiftState
  /// The currently pressed shift modifiers and is valid only if the high order bit has been set
  UINT32               KeyShiftState;
  // KeyToggleState
  /// The current internal state of various toggled attributes and is valid only if the high order bit has been set
  EFI_KEY_TOGGLE_STATE KeyToggleState;

};

// EFI_KEY_DATA
/// Input extended keystroke information
typedef struct EFI_KEY_DATA EFI_KEY_DATA;
struct EFI_KEY_DATA {

  // Key
  /// The key scan code and Unicode character returned from the input device
  EFI_INPUT_KEY Key;
  // KeyState
  /// The current state of various toggled attributes as well as input modifier values
  EFI_KEY_STATE KeyState;

};

// EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
/// Extended simple text input protocol
typedef struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

// EFI_KEY_NOTIFY_FUNCTION
/// Keystroke notification function
/// @param KeyData The keystroke information
typedef
EFI_STATUS
(EFIAPI
*EFI_KEY_NOTIFY_FUNCTION) (
   IN EFI_KEY_DATA *KeyData
);

// EFI_INPUT_RESET_EX
/// Resets the input device hardware
/// @param This                 The extended simple text input protocol interface
/// @param ExtendedVerification Indicates that the driver may perform an extended verification of the device during reset
/// @retval EFI_SUCCESS      The device was reset
/// @retval EFI_DEVICE_ERROR The device is not functioning correctly and could not be reset
typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET_EX) (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN BOOLEAN                            ExtendedVerification
);

// EFI_INPUT_READ_KEY_EX
/// Reads the next keystroke from the input device
/// @param This    The extended simple text input protocol interface
/// @param KeyData On output, the keystroke state data for the key that was pressed
/// @retval EFI_SUCCESS      The keystroke information was returned
/// @retval EFI_NOT_READY    There was no keystroke data available
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
typedef
EFI_STATUS
(EFIAPI
*EFI_INPUT_READ_KEY_EX) (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  OUT EFI_KEY_DATA                      *KeyData
);

// EFI_SET_STATE
/// Set certain state for the input device
/// @param This           The extended simple text input protocol interface
/// @param KeyToggleState The toggle state to set for the input device
/// @retval EFI_SUCCESS      The device state was set appropriately
/// @retval EFI_DEVICE_ERROR The device is not functioning correctly and could not have the setting adjusted
/// @retval EFI_UNSUPPORTED  The device does not support the ability to have its state set or the requested state change was not supported
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_STATE) (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN EFI_KEY_TOGGLE_STATE              *KeyToggleState
);

// EFI_REGISTER_KEYSTROKE_NOTIFY
/// Register a notification function for a particular keystroke for the input device
/// @param This                    The extended simple text input protocol interface
/// @param KeyData                 A buffer that is filled in with the keystroke information for the key that was pressed to be used by
///                                 the notification function. If KeyData.Key, KeyData.KeyState.KeyToggleState and KeyData.KeyState.KeyShiftState
///                                 are 0, then any incomplete keystroke will trigger a notification of the KeyNotificationFunction
/// @param KeyNotificationFunction The notification function to be called when the key sequence specified by
///                                 KeyData is typed. This notification function should be called at <= TPL_CALLBACK
/// @param NotifyHandle            On output, the unique handle assigned to the registered notification
/// @retval EFI_SUCCESS          Key notify was registered successfully
/// @retval EFI_OUT_OF_RESOURCES Unable to allocate necessary data structures
typedef
EFI_STATUS
(EFIAPI
*EFI_REGISTER_KEYSTROKE_NOTIFY) (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN  EFI_KEY_DATA                       *KeyData,
  IN  EFI_KEY_NOTIFY_FUNCTION             KeyNotificationFunction,
  OUT VOID                              **NotifyHandle
);

// EFI_UNREGISTER_KEYSTROKE_NOTIFY
/// Remove the notification that was previously registered
/// @param This               The extended simple text input protocol interface
/// @param NotificationHandle The handle of the notification function being unregistered
/// @retval EFI_SUCCESS Key notify was unregistered successfully
/// @retval EFI_INVALID_PARAMETER The NotificationHandle is invalid
typedef
EFI_STATUS
(EFIAPI *EFI_UNREGISTER_KEYSTROKE_NOTIFY) (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN VOID                              *NotificationHandle
);

// EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
/// Extended simple text input protocol
struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL {

  // Reset
  /// Reset the ConsoleIn device
  EFI_INPUT_RESET_EX              Reset;
  // ReadKeyStrokeEx
  /// Returns the next input character
  EFI_INPUT_READ_KEY_EX           ReadKeyStrokeEx;
  // WaitForKeyEx
  /// Event to use to wait for a key to be available, an Event will only be triggered if KeyData.Key has information
  EFI_EVENT                       WaitForKeyEx;
  // SetState
  /// Set the toggle state settings for the input device
  EFI_SET_STATE                   SetState;
  // RegisterKeyNotify
  /// Register a notification function to be called when a given key sequence is hit
  EFI_REGISTER_KEYSTROKE_NOTIFY   RegisterKeyNotify;
  // UnregisterKeyNotify
  /// Removes a specific notification function
  EFI_UNREGISTER_KEYSTROKE_NOTIFY UnregisterKeyNotify;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_HEADER__
