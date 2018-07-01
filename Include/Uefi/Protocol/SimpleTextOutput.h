///
/// @file Include/Uefi/Protocol/SimpleTextOutput.h
///
/// UEFI implementation simple text output protocol
///

#pragma once
#ifndef ___UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HEADER__
#define ___UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID
/// Simple text output protocol unique identifier
#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID { 0x387477C2, 0x69C7, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

// EFI_BLACK
/// Black foreground color
#define EFI_BLACK 0x00
// EFI_BLUE
/// Blue foreground color
#define EFI_BLUE 0x01
// EFI_GREEN
/// Green foreground color
#define EFI_GREEN 0x02
// EFI_CYAN
/// Cyan foreground color
#define EFI_CYAN 0x03
// EFI_RED
/// Red foreground color
#define EFI_RED 0x04
// EFI_MAGENTA
/// Magenta foreground color
#define EFI_MAGENTA 0x05
// EFI_BROWN
/// Brown foreground color
#define EFI_BROWN 0x06
// EFI_LIGHTGRAY
/// Light gray foreground color
#define EFI_LIGHTGRAY 0x07
// EFI_BRIGHT
/// Brighten forground color
#define EFI_BRIGHT 0x08
// EFI_DARKGRAY
/// Dark gray foreground color
#define EFI_DARKGRAY 0x8
// EFI_LIGHTBLUE
/// Light blue foreground color
#define EFI_LIGHTBLUE 0x09
// EFI_LIGHTGREEN
/// Light green foreground color
#define EFI_LIGHTGREEN 0x0A
// EFI_LIGHTCYAN
/// Light cyan foreground color
#define EFI_LIGHTCYAN 0x0B
// EFI_LIGHTRED
/// Light red foreground color
#define EFI_LIGHTRED 0x0C
// EFI_LIGHTMAGENTA
/// Light magenta foreground color
#define EFI_LIGHTMAGENTA 0x0D
// EFI_YELLOW
/// Yellow foreground color
#define EFI_YELLOW 0x0E
// EFI_WHITE
/// White foreground color
#define EFI_WHITE 0x0F

// EFI_BACKGROUND_BLACK
/// Black background color
#define EFI_BACKGROUND_BLACK 0x00
// EFI_BACKGROUND_BLUE
/// Blue background color
#define EFI_BACKGROUND_BLUE 0x10
// EFI_BACKGROUND_GREEN
/// Green background color
#define EFI_BACKGROUND_GREEN 0x20
// EFI_BACKGROUND_CYAN
/// Cyan background color
#define EFI_BACKGROUND_CYAN 0x30
// EFI_BACKGROUND_RED
/// Red background color
#define EFI_BACKGROUND_RED 0x40
// EFI_BACKGROUND_MAGENTA
/// Magenta background color
#define EFI_BACKGROUND_MAGENTA 0x50
// EFI_BACKGROUND_BROWN
/// Brown background color
#define EFI_BACKGROUND_BROWN 0x60
// EFI_BACKGROUND_LIGHTGRAY
/// Light gray background color
#define EFI_BACKGROUND_LIGHTGRAY 0x70

// gEfiSimpleTextOutputProtocolGuid
/// The simple text output protocol unique identifier
EXTERN EFI_GUID gEfiSimpleTextOutputProtocolGuid;

// SIMPLE_TEXT_OUTPUT_MODE
/// Simple text output mode descriptor
typedef struct SIMPLE_TEXT_OUTPUT_MODE SIMPLE_TEXT_OUTPUT_MODE;
struct SIMPLE_TEXT_OUTPUT_MODE {

  // MaxMode
  /// The maximum count of modes supported by the simple text output protocol interface
  INT32   MaxMode;
  // Mode
  /// The current mode of the simple text output protocol interface
  INT32   Mode;
  // Attribute
  /// The current character output attribute
  INT32   Attribute;
  // CursorColumn
  /// The cursor column
  INT32   CursorColumn;
  // CursorRow
  /// The cursor row
  INT32   CursorRow;
  // CursorVisible
  /// If the cursor is currently visible or not
  BOOLEAN CursorVisible;

};

// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
/// Simple text output protocol
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// EFI_TEXT_RESET
/// Resets the text output device hardware
/// @param This                 The simple text output protocol interface
/// @param ExtendedVerification Indicates that the driver may perform an extended verification of the device during reset
/// @param EFI_SUCCESS      The text output device was reset
/// @param EFI_DEVICE_ERROR The text output device is not functioning correctly and could not be reset
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_RESET) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN BOOLEAN                          ExtendedVerification
);

// EFI_TEXT_STRING
/// Writes a string to the output device
/// @param This   The simple text output protocol interface
/// @param String The null-terminated string to be displayed
/// @retval EFI_SUCCESS            The string was output to the device
/// @retval EFI_DEVICE_ERROR       The device reported an error while attempting to output the text
/// @retval EFI_UNSUPPORTED        The output device’s mode is not currently in a defined text mode
/// @retval EFI_WARN_UNKNOWN_GLYPH This warning code indicates that some of the characters in the string could not be rendered and were skipped
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_STRING) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
);

// EFI_TEXT_TEST_STRING
/// Verifies that all characters in a string can be output to the target device
/// @param This   The simple text output protocol interface
/// @param String The null-terminated string to be examined
/// @retval EFI_SUCCESS     The device(s) are capable of rendering the output string
/// @retval EFI_UNSUPPORTED Some of the characters in the string cannot be rendered by one or more of the output devices mapped by the EFI handle
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_TEST_STRING) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
);

// EFI_TEXT_QUERY_MODE
/// Returns information for an available text mode that the output device(s) supports
/// @param This       The simple text output protocol interface
/// @param ModeNumber The mode number on which to return information
/// @param Columns    On output, the columns of the text output device for the request ModeNumber
/// @param Rows       On output, the rows of the text output device for the request ModeNumber
/// @retval EFI_SUCCESS      The requested mode information was returned
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  The mode number was not valid
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_QUERY_MODE) (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN  UINTN                            ModeNumber,
  OUT UINTN                           *Columns,
  OUT UINTN                           *Rows
);

// EFI_TEXT_SET_MODE
/// Sets the output device(s) to a specified mode
/// @param This       The simple text output protocol interface
/// @param ModeNumber The text mode to set
/// @retval EFI_SUCCESS The requested text mode was set
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED The mode number was not valid
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_SET_MODE) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                            ModeNumber
);

// EFI_TEXT_SET_ATTRIBUTE
/// Sets the background and foreground colors
/// @param This      The simple text output protocol interface
/// @param Attribute The attribute to set. Bits 0 ... 3 are the foreground color, and bits 4 ... 6 are the background color
/// @retval EFI_SUCCESS The requested attributes were set
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_SET_ATTRIBUTE) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                            Attribute
);

// EFI_TEXT_CLEAR_SCREEN
/// Clears the output device(s) display to the currently selected background color
/// @param This The simple text output protocol interface
/// @retval EFI_SUCCESS      The operation completed successfully
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  The output device is not in a valid text mode
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_CLEAR_SCREEN) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

// EFI_TEXT_SET_CURSOR_POSITION
/// Sets the current coordinates of the cursor position
/// @param This   The simple text output protocol interface
/// @param Column The column for the cursor position
/// @param Row    The row for the cursor position
/// @retval EFI_SUCCESS      The operation completed successfully
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  The output device is not in a valid text mode, or the cursor position is invalid for the current mode
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_SET_CURSOR_POSITION) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                            Column,
  IN UINTN                            Row
);

// EFI_TEXT_ENABLE_CURSOR
/// Makes the cursor visible or invisible
/// @param This    The simple text output protocol interface
/// @param Visible If TRUE, the cursor is set to be visible, if FALSE, the cursor is set to be invisible
/// @retval EFI_SUCCESS The operation completed successfully
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request or the device does not support changing the cursor mode
/// @retval EFI_UNSUPPORTED The output device does not support visibility control of the cursor
typedef
EFI_STATUS
(EFIAPI
*EFI_TEXT_ENABLE_CURSOR) (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN BOOLEAN                          Visible
);

// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
/// Simple text output protocol
struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {

  // Reset
  /// Reset the ConsoleOut device
  EFI_TEXT_RESET                Reset;
  // OutputString
  /// Displays the string on the device at the current cursor location
  EFI_TEXT_STRING               OutputString;
  // TestString
  /// Tests to see if the ConsoleOut device supports this string
  EFI_TEXT_TEST_STRING          TestString;
  // QueryMode
  /// Queries information concerning the output device’s supported text mode
  EFI_TEXT_QUERY_MODE           QueryMode;
  // SetMode
  /// Sets the current mode of the output device
  EFI_TEXT_SET_MODE             SetMode;
  // SetAttribute
  /// Sets the foreground and background color of the text that is output
  EFI_TEXT_SET_ATTRIBUTE        SetAttribute;
  // ClearScreen
  /// Clears the screen with the currently set background color
  EFI_TEXT_CLEAR_SCREEN         ClearScreen;
  // SetCursorPosition
  /// Sets the current cursor position
  EFI_TEXT_SET_CURSOR_POSITION  SetCursorPosition;
  // EnableCursor
  /// Turns the visibility of the cursor on/off
  EFI_TEXT_ENABLE_CURSOR        EnableCursor;
  // Mode
  /// The current simple text output mode descriptor
  SIMPLE_TEXT_OUTPUT_MODE      *Mode;

};

// EfiConOutPrint
/// Print a string to boot services console output
/// @param String The string to print
/// @retval EFI_SUCCESS            The string was output to the device
/// @retval EFI_DEVICE_ERROR       The device reported an error while attempting to output the text
/// @retval EFI_UNSUPPORTED        The output device’s mode is not currently in a defined text mode
/// @retval EFI_WARN_UNKNOWN_GLYPH This warning code indicates that some of the characters in the string could not be rendered and were skipped
EXTERN
EFI_STATUS
EFIAPI
EfiConOutPrint (
  IN CONST CHAR16 *String
);

// EfiConOutSetAttributes
/// Set the attributes for the console output
/// @param Attributes The attributes to set for the console output
EXTERN
EFI_STATUS
EFIAPI
EfiConOutSetAttributes (
  IN UINT32 Attributes
);

// EfiConOutEnableCursor
/// Enable or disable the cursor for console output
/// @param Enable Whether to enable or disable the cursor for console output
EXTERN
EFI_STATUS
EFIAPI
EfiConOutEnableCursor (
  IN BOOLEAN Enable
);

// EfiConOutClear
/// Clear the console output
/// @param Attributes   The attributes to set before clearing the console output
/// @param EnableCursor Whether to enable or disable the cursor for console output
EXTERN
EFI_STATUS
EFIAPI
EfiConOutClear (
  IN UINT32  Attributes,
  IN BOOLEAN EnableCursor
);

// EfiConOutCurrentMode
/// Get the current mode number for the console device
/// @return The current mode number for the console device
EXTERN
UINTN
EFIAPI
EfiConOutCurrentMode (
  VOID
);
// EfiConOutQueryMode
/// Returns information for an available text mode that the output console device supports
/// @param ModeNumber The mode number on which to return information
/// @param Columns    On output, the columns of the text output device for the request ModeNumber
/// @param Rows       On output, the rows of the text output device for the request ModeNumber
/// @retval EFI_SUCCESS      The requested mode information was returned
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  The mode number was not valid
EXTERN
EFI_STATUS
EFIAPI
EfiConOutQueryMode (
  IN  UINTN  ModeNumber,
  OUT UINTN *Columns,
  OUT UINTN *Rows
);
// EfiConOutSetMode
/// Sets the console output device to a specified mode
/// @param ModeNumber The text mode to set
/// @retval EFI_SUCCESS The requested text mode was set
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED The mode number was not valid
EXTERN
EFI_STATUS
EFIAPI
EfiConOutSetMode (
  IN UINTN ModeNumber
);

// EfiConOutSwitchToBestMode
/// Switch the console output to the best mode
/// @return Whether the console output is in the best mode or not
EXTERN
EFI_STATUS
EFIAPI
EfiConOutSwitchToBestMode (
  VOID
);
// EfiConOutSwitchToMode
/// Switch the console output to the mode
/// @param Columns The number of characters represented horizontally or zero for best
/// @param Rows    The number of characters represented vertically or zero for best
/// @return Whether the console out is in the mode or not
EXTERN
EFI_STATUS
EFIAPI
EfiConOutSwitchToMode (
  IN UINTN Columns,
  IN UINTN Rows
);
// EfiConOutSwitchToMode
/// Switch the console output to the mode
/// @param ColumnsAndRows A null-terminated Unicode string that represents the columns and rows separated by non-numeral characters
/// @return Whether the console out is in the mode or not
EXTERN
EFI_STATUS
EFIAPI
EfiConOutSwitchToModeString (
  IN CONST CHAR16 *ColumnsAndRows
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HEADER__
