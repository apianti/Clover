///
/// @file Include/Uefi/Protocol/SimpleTextInput.h
///
/// UEFI implementation simple text input protocol
///

#pragma once
#ifndef ___UEFI_SIMPLE_TEXT_INPUT_PROTOCOL_HEADER__
#define ___UEFI_SIMPLE_TEXT_INPUT_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID
/// Simple text input protocol unique identifier
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID { 0x387477C1, 0x69C7, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

// gEfiSimpleTextInputProtocolGuid
/// The simple text input protocol unique identifier
EXTERN EFI_GUID gEfiSimpleTextInputProtocolGuid;

// CHAR_BACKSPACE
/// Backspace character
#define CHAR_BACKSPACE 0x0008
// CHAR_TAB
/// Tab character
#define CHAR_TAB 0x0009
// CHAR_LINEFEED
/// Line feed character
#define CHAR_LINEFEED 0x000A
// CHAR_CARRIAGE_RETURN
/// Carriage return character
#define CHAR_CARRIAGE_RETURN 0x000D

// SCAN_NULL
/// Null scan code
#define SCAN_NULL 0x0000
// SCAN_UP
/// Up arrow
#define SCAN_UP 0x0001
// SCAN_DOWN
/// Down arrow
#define SCAN_DOWN 0x0002
// SCAN_RIGHT
/// Right arrow
#define SCAN_RIGHT 0x0003
// SCAN_LEFT
/// Left arrow
#define SCAN_LEFT 0x0004
// SCAN_HOME
/// Home
#define SCAN_HOME 0x0005
// SCAN_END
/// End
#define SCAN_END 0x0006
// SCAN_INSERT
/// Insert
#define SCAN_INSERT 0x0007
// SCAN_DELETE
/// Delete
#define SCAN_DELETE 0x0008
// SCAN_PAGE_UP
/// Page up
#define SCAN_PAGE_UP 0x0009
// SCAN_PAGE_DOWN
/// Page down
#define SCAN_PAGE_DOWN 0x000A
// SCAN_F1
/// Function 1
#define SCAN_F1 0x000B
// SCAN_F2
/// Function 2
#define SCAN_F2 0x000C
// SCAN_F3
/// Function 3
#define SCAN_F3 0x000D
// SCAN_F4
/// Function 4
#define SCAN_F4 0x000E
// SCAN_F5
/// Function 5
#define SCAN_F5 0x000F
// SCAN_F6
/// Function 6
#define SCAN_F6 0x0010
// SCAN_F7
/// Function 7
#define SCAN_F7 0x0011
// SCAN_F8
/// Function 8
#define SCAN_F8 0x0012
// SCAN_F9
/// Function 9
#define SCAN_F9 0x0013
// SCAN_F10
/// Function 10
#define SCAN_F10 0x0014
// SCAN_ESC
/// Escape
#define SCAN_ESC 0x0017

// EFI_INPUT_KEY
/// Input keystroke information
typedef struct EFI_INPUT_KEY EFI_INPUT_KEY;
struct EFI_INPUT_KEY {

  // ScanCode
  /// The input key scan code
  UINT16 ScanCode;
  // UnicodeChar
  /// The printable Unicode character
  CHAR16 UnicodeChar;

};

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL
/// Simple text input protocol
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EFI_INPUT_RESET
/// Resets the input device hardware
/// @param This                 The simple text input protocol interface
/// @param ExtendedVerification Indicates that the driver may perform an extended verification of the device during reset
/// @retval EFI_SUCCESS      The device was reset
/// @retval EFI_DEVICE_ERROR The device is not functioning correctly and could not be reset
typedef
EFI_STATUS
(EFIAPI
*EFI_INPUT_RESET) (
  IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  IN BOOLEAN                         ExtendedVerification
);

// EFI_INPUT_READ_KEY
/// Reads the next keystroke from the input device
/// @param This The simple text input protocol interface
/// @param Key  On output, the keystroke information for the key that was pressed
/// @retval EFI_SUCCESS      The keystroke information was returned
/// @retval EFI_NOT_READY    There was no keystroke data available
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
typedef
EFI_STATUS
(EFIAPI
*EFI_INPUT_READ_KEY) (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  OUT EFI_INPUT_KEY                  *Key
);

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL
/// Simple text input protocol
struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {

  // Reset
  /// Reset the ConsoleIn device
  EFI_INPUT_RESET    Reset;
  // ReadKeyStroke
  /// Returns the next input character
  EFI_INPUT_READ_KEY ReadKeyStroke;
  // WaitForKey
  /// Event to use to wait for a key to be available
  EFI_EVENT          WaitForKey;

};

// EfiConInWaitForKey
/// Wait for a key from boot service console input
/// @param Key On output, the key read from the console input
/// @retval EFI_SUCCESS      The keystroke information was returned
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
EXTERN
EFI_STATUS
EFIAPI
EfiConInWaitForKey (
  OUT EFI_INPUT_KEY *Key
);
// EfiConInWaitForAnyKey
/// Wait for any key from boot service console input
/// @retval EFI_SUCCESS      The keystroke information was discarded
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
EXTERN
EFI_STATUS
EFIAPI
EfiConInWaitForAnyKey (
  VOID
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SIMPLE_TEXT_INPUT_PROTOCOL_HEADER__
