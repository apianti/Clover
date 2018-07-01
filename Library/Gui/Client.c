///
/// @file Library/Gui/Client.c
///
/// Graphical interface client
///

#include <Gui/Client.h>

// GuiServerSendLogMessage
/// Send a log message to the server
/// @param Format The format specifier
/// @param ...    The format arguments
/// @retval EFI_SUCCESS The quit message was sent successfully
//EXTERN
//EFI_STATUS
//EFIAPI
//GuiServerSendLogMessage (
//  IN CONST CHAR16 *Format,
//  IN ...
//) {
//  EFI_STATUS   Status;
//  GUI_MESSAGE *LogMessage;
//  UINTN        Size;
//  VA_LIST      Arguments;
//  // Check parameters
//  if (Format == NULL) {
//    return EFI_INVALID_PARAMETER;
//  }
//  // Get the text argument size
//  Size = 0;
//  VA_START(Arguments, Format);
//  Status = EfiVSPrint(NULL, &Size, Format, Arguments);
//  if (Status == EFI_BUFFER_TOO_SMALL) {
//    // Allocate the message
//    LogMessage = EfiAllocate(sizeof(GUI_MESSAGE) + Size);
//    if (LogMessage == NULL) {
//      return EFI_INVALID_PARAMETER;
//    }
//    // Print the text to the message
//    Status = EfiVSPrint((CHAR16 *)(LogMessage + 1), &Size, Format, Arguments);
//    if (!EFI_ERROR(Status)) {
//      // Send the log message
//      LogMessage->Type = GUI_SERVER_MESSAGE_TYPE_TEXT;
//      LogMessage->Size = (UINT32)(sizeof(GUI_MESSAGE) + Size);
//      Status = GuiServerSendMessage(NULL, LogMessage);
//    }
//    // Free the message
//    EfiFreePool(LogMessage);
//  }
//  VA_END(Arguments);
//  return Status;
//}
