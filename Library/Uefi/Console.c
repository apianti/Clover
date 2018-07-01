///
/// @file Library/Uefi/Console.c
///
/// UEFI console utilities
///

#include <Uefi.h>

// EfiConOutSetAttributes
/// Set the attributes for the console output
/// @param Attributes The attributes to set for the console output
EFI_STATUS
EFIAPI
EfiConOutSetAttributes (
  IN UINT32 Attributes
) {
  if ((gEfiConOut == NULL) || (gEfiConOut->SetAttribute == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiConOut->SetAttribute(gEfiConOut, Attributes);
}

// EfiConOutEnableCursor
/// Enable or disable the cursor for console output
/// @param Enable Whether to enable or disable the cursor for console output
EFI_STATUS
EFIAPI
EfiConOutEnableCursor (
  IN BOOLEAN Enable
) {
  if ((gEfiConOut == NULL) || (gEfiConOut->EnableCursor == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiConOut->EnableCursor(gEfiConOut, Enable);
}

// EfiConOutClear
/// Clear the console output
/// @param Attributes   The attributes to set before clearing the console output
/// @param EnableCursor Whether to enable or disable the cursor for console output
EFI_STATUS
EFIAPI
EfiConOutClear (
  IN UINT32  Attributes,
  IN BOOLEAN EnableCursor
) {
  if ((gEfiConOut == NULL) || (gEfiConOut->ClearScreen == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if (gEfiConOut->SetAttribute == NULL) {
    gEfiConOut->SetAttribute(gEfiConOut, Attributes);
  }
  if (gEfiConOut->EnableCursor == NULL) {
    gEfiConOut->EnableCursor(gEfiConOut, EnableCursor);
  }
  return gEfiConOut->ClearScreen(gEfiConOut);
}

// EfiConOutCurrentMode
/// Get the current mode number for the console device
/// @return The current mode number for the console device
EXTERN
UINTN
EFIAPI
EfiConOutCurrentMode (
  VOID
) {
  // Check console mode is valid
  if ((gEfiConOut == NULL) || (gEfiConOut->Mode == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return (UINTN)(gEfiConOut->Mode->Mode);
}
// EfiConOutQueryMode
/// Returns information for an available text mode that the output console device supports
/// @param ModeNumber The mode number on which to return information
/// @param Columns    On output, the columns of the text output device for the request ModeNumber
/// @param Rows       On output, the rows of the text output device for the request ModeNumber
/// @retval EFI_SUCCESS      The requested mode information was returned
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  The mode number was not valid
EFI_STATUS
EFIAPI
EfiConOutQueryMode (
  IN  UINTN  ModeNumber,
  OUT UINTN *Columns,
  OUT UINTN *Rows
) {
  // Check console is valid
  if ((gEfiConOut == NULL) || (gEfiConOut->QueryMode == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiConOut->QueryMode(gEfiConOut, ModeNumber, Columns, Rows);
}
// EfiConOutSetMode
/// Sets the console output device to a specified mode
/// @param ModeNumber The text mode to set
/// @retval EFI_SUCCESS The requested text mode was set
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED The mode number was not valid
EFI_STATUS
EFIAPI
EfiConOutSetMode (
  IN UINTN ModeNumber
) {
  // Check console is valid
  if ((gEfiConOut == NULL) || (gEfiConOut->SetMode == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiConOut->SetMode(gEfiConOut, ModeNumber);
}

// EfiConOutSwitchToBestMode
/// Switch the console output to the best mode
/// @return Whether the console output is in the best mode or not
EFI_STATUS
EFIAPI
EfiConOutSwitchToBestMode (
  VOID
) {
  UINTN Index = 0;
  UINTN BestMode;
  UINTN BestSize = 0;
  UINTN ModeCols = 0;
  UINTN ModeRows = 0;
  // Check console is valid
  if ((gEfiConOut == NULL) || (gEfiConOut->Mode == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Output current console mode
  BestMode = gEfiConOut->Mode->Mode;
  while (Index < (UINTN)gEfiConOut->Mode->MaxMode) {
    // Query the next mode
    if (!EFI_ERROR(EfiConOutQueryMode(Index, &ModeCols, &ModeRows))) {
      // Check if best mode by finding area size of characters and comparing
      UINTN ModeSize = ModeCols * ModeRows;
      if (ModeSize > BestSize) {
        BestSize = ModeSize;
        BestMode = Index;
      }
    }
    ++Index;
  }
  // Check if already best mode
  if (BestMode == (UINTN)gEfiConOut->Mode->Mode) {
    // No need to switch if already best mode
    return EFI_SUCCESS;
  }
  // Switch to the best mode
  return EfiConOutSetMode(BestMode);
}
// EfiConOutSwitchToMode
/// Switch the console output to the mode
/// @param Columns The number of characters represented horizontally or zero for best
/// @param Rows    The number of characters represented vertically or zero for best
/// @return Whether the console out is in the mode or not
EFI_STATUS
EFIAPI
EfiConOutSwitchToMode (
  IN UINTN Columns,
  IN UINTN Rows
) {
  UINTN Index = 0;
  UINTN ModeCols = 0;
  UINTN ModeRows = 0;
  // Check parameters
  if ((Columns == 0) && (Rows == 0)) {
    // Switch to best mode on empty string
    return EfiConOutSwitchToBestMode();
  }
  // Check console is valid
  if ((gEfiConOut == NULL) || (gEfiConOut->Mode == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Output current console mode
  while (Index < (UINTN)gEfiConOut->Mode->MaxMode) {
    // Query the next mode
    if (!EFI_ERROR(EfiConOutQueryMode(Index, &ModeCols, &ModeRows))) {
      // Check if this mode matches the with requested
      if (((Columns == ModeCols) || (Columns == 0)) && ((Rows == ModeRows) || (Rows == 0))) {
        // Switch to the best mode
        return EfiConOutSetMode(Index);
      }
    }
    ++Index;
  }
  // Requested mode not found
  return EFI_NOT_FOUND;
}
// EfiConOutSwitchToMode
/// Switch the console output to the mode
/// @param ColumnsAndRows A null-terminated Unicode string that represents the columns and rows separated by non-numeral characters
/// @return Whether the console out is in the mode or not
EFI_STATUS
EFIAPI
EfiConOutSwitchToModeString (
  IN CONST CHAR16 *ColumnsAndRows
) {
  EFI_STATUS Status;
  UINT64     Columns = 0;
  UINT64     Rows = 0;
  // Check parameters
  if ((ColumnsAndRows == NULL) || (*ColumnsAndRows == 0)) {
    // Switch to best mode on empty string
    return EfiConOutSwitchToBestMode();
  }
  // Convert the columns string to unsigned integer
  Status = StrToUnsigned(&ColumnsAndRows, &Columns, 10);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Skip any intermediate characters that aren't digits
  while ((*ColumnsAndRows != 0) && (*ColumnsAndRows >= '0') && (*ColumnsAndRows <= '9')) {
    ++ColumnsAndRows;
  }
  // Convert the rows string to unsigned integer
  Status = StrToUnsigned(&ColumnsAndRows, &Rows, 10);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Switch to the requested mode
  return EfiConOutSwitchToMode((UINTN)Columns, (UINTN)Rows);
}

// EfiConOutPrint
/// Print a string to boot services console output
/// @param String The string to print
/// @retval EFI_SUCCESS            The string was output to the device
/// @retval EFI_DEVICE_ERROR       The device reported an error while attempting to output the text
/// @retval EFI_UNSUPPORTED        The output device’s mode is not currently in a defined text mode
/// @retval EFI_WARN_UNKNOWN_GLYPH This warning code indicates that some of the characters in the string could not be rendered and were skipped
EFI_STATUS
EFIAPI
EfiConOutPrint (
  IN CONST CHAR16 *String
) {
  if ((String == NULL) || (*String == 0)) {
    return EFI_SUCCESS;
  }
  if ((gEfiConOut == NULL) || (gEfiConOut->OutputString == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiConOut->OutputString(gEfiConOut, (CHAR16 *)String);
}

// EfiConInWaitForKey
/// Wait for a key from boot service console input
/// @param Key On output, the key read from the console input
/// @retval EFI_SUCCESS      The keystroke information was returned
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
EFI_STATUS
EFIAPI
EfiConInWaitForKey (
  OUT EFI_INPUT_KEY *Key
) {
  EFI_STATUS Status;
  if (Key == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((gEfiConIn == NULL) || (gEfiConIn->ReadKeyStroke == NULL)) {
    return EFI_UNSUPPORTED;
  }
  do {
    Status = gEfiConIn->ReadKeyStroke(gEfiConIn, Key);
  } while (Status == EFI_NOT_READY);
  return Status;
}
// EfiConInWaitForAnyKey
/// Wait for any key from boot service console input
/// @retval EFI_SUCCESS      The keystroke information was discarded
/// @retval EFI_DEVICE_ERROR The keystroke information was not returned due to hardware errors
EFI_STATUS
EFIAPI
EfiConInWaitForAnyKey (
  VOID
) {
  EFI_INPUT_KEY Key = { 0, 0 };
  return EfiConInWaitForKey(&Key);
}
