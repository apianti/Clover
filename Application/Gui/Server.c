///
/// @file Library/Gui/Server.c
///
/// Graphical interface server
///

#include <Gui/Client.h>

// GUI_SERVER
/// The GUI server protocol implemention
typedef struct GUI_SERVER GUI_SERVER;
struct GUI_SERVER {

  // Lock
  /// The server GUI lock
  EFI_LOCK        Lock;
  // ConOutOutputString
  /// The previous console output method
  EFI_TEXT_STRING ConOutOutputString;
  // Running
  /// Whether the server is running
  BOOLEAN         Running;

};

// mGuiServer
/// The GUI server
STATIC GUI_SERVER *mGuiServer;

// GuiServerThread
/// Thread for no boot services tasks for multithreaded GUI
STATIC
VOID
EFIAPI
GuiServerThread (
  IN GUI_SERVER *Server
) {
  // Running needs to set to true due to optimization and out of order
  BOOLEAN Running = TRUE;
  if (Server != NULL) {
    // The draw run loop
    do {
      // Lock the server
      EfiLock(&(Server->Lock));
      // Check if the server is still running
      Running = Server->Running;
      if (Running) {
        // TODO: Draw the GUI
        
      }
      // Unlock the server
      EfiUnlock(&(Server->Lock));
      // Pause the loop momentarily if needed
      EfiCpuPause();
    } while (Running);
  }
}
// ServerNullOutputString
/// Dummy method to output nothing to console
/// @param This   The simple text output protocol interface
/// @param String The null-terminated string to be displayed
/// @retval EFI_SUCCESS            The string was output to the device
/// @retval EFI_DEVICE_ERROR       The device reported an error while attempting to output the text
/// @retval EFI_UNSUPPORTED        The output device’s mode is not currently in a defined text mode
/// @retval EFI_WARN_UNKNOWN_GLYPH This warning code indicates that some of the characters in the string could not be rendered and were skipped
STATIC
EFI_STATUS
EFIAPI
ServerNullOutputString (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
) {
  UNUSED_PARAMETER(This);
  UNUSED_PARAMETER(String);
  return EFI_SUCCESS;
}
// GuiRedirectConsole
/// Setup logging for the GUI
STATIC
VOID
EFIAPI
GuiRedirectConsole (
  VOID
) {
  if ((mGuiServer != NULL) && (mGuiServer->ConOutOutputString == NULL)) {
    // Store old console output string method and replace with null output method
    if ((gEfiConOut != NULL) && EfiConfigurationGetBoolean(L"/Boot/Console/Redirect", TRUE)) {
      mGuiServer->ConOutOutputString = gEfiConOut->OutputString;
      gEfiConOut->OutputString = ServerNullOutputString;
    }
  }
}
// GuiUnredirectConsole
/// Setup logging for the GUI
STATIC
VOID
EFIAPI
GuiUnredirectConsole (
  VOID
) {
  // Restore old method for console output
  if ((gEfiConOut != NULL) && (mGuiServer != NULL) && (mGuiServer->ConOutOutputString != NULL)) {
    gEfiConOut->OutputString = mGuiServer->ConOutOutputString;
  }
}
// GuiIsConsoleOnly
/// Check if the configuration suggests a console only GUI should be used
/// @return Whether the multithreaded GUI should be used
BOOLEAN
EFIAPI
GuiIsConsoleOnly (
  VOID
) {
  return EfiConfigurationGetBoolean(L"/Gui/ConsoleOnly", FALSE);
}
// GuiIsMultithreaded
/// Check if the configuration suggests a multithreaded GUI should be used
/// @return Whether the multithreaded GUI should be used
BOOLEAN
EFIAPI
GuiIsMultithreaded (
  VOID
) {
  return EfiConfigurationGetBoolean(L"/Gui/Multithreaded", TRUE);
}
// GuiServerStart
/// GUI server start
/// @param This The GUI server protocol
/// @retval EFI_SUCCESS The GUI started successfully
EFI_STATUS
EFIAPI
GuiServerStart (
  VOID
) {
  EFI_STATUS                 Status;
  EFI_EVENT                  Event;
  UINTN                      Index;
  UINTN                      NumberOfProcessors;
  EFI_PROCESSOR_INFORMATION  ProcessorInfo;
  // Get the number of available processors
  NumberOfProcessors = 1;
  Status = CpuGetNumberOfProcessors(&NumberOfProcessors, NULL, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (NumberOfProcessors <= 1) {
    return EFI_NOT_READY;
  }
  // Create the server
  if (mGuiServer == NULL) {
    mGuiServer = EfiAllocateByType(GUI_SERVER);
    if (mGuiServer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    mGuiServer->Running = FALSE;
    mGuiServer->ConOutOutputString = NULL;
    // Initialize the server lock
    EfiLockInitialize(&(mGuiServer->Lock));
  }
  // Check if multithreading is disabled or console only
  if (GuiIsConsoleOnly()) {
    LOG(L"GUI starting in console only mode ...\n");
    return EFI_SUCCESS;
  }
  if (!GuiIsMultithreaded()) {
    LOG(L"GUI starting in singlethreaded mode ...\n");
    return EFI_SUCCESS;
  }
  // Start multithreaded
  LOG(L"GUI starting in multithreaded mode ...\n");
  // Lock the server
  EfiLock(&(mGuiServer->Lock));
  // TODO: Initialize the graphics

  // Server is running
  mGuiServer->Running = TRUE;
  // Find a processor that can start a thread
  for (Index = 0; Index < NumberOfProcessors; ++Index) {
    EfiZero(EFI_PROCESSOR_INFORMATION, &ProcessorInfo);
    Status = EfiMpServicesGetProcessorInfo(Index, &ProcessorInfo);
    if (!EFI_ERROR(Status)) {
      // No thread can be started on the boot services processor, or a disabled and/or unhealthy processor
      if (EFI_BITS_ARE_UNSET(ProcessorInfo.StatusFlag, PROCESSOR_AS_BSP_BIT) &&
          EFI_BITS_ARE_SET(ProcessorInfo.StatusFlag, PROCESSOR_ENABLED_BIT | PROCESSOR_HEALTH_STATUS_BIT)) {
        VERBOSE(L"Thread starting on processor CPU%03u ... ", ProcessorInfo.ProcessorId);
        // Create an event for the thread
        Event = NULL;
        Status = EfiCreateEvent(0, TPL_NOTIFY, NULL, NULL, &Event);
        if (!EFI_ERROR(Status)) {
          // Execute the procedure on the selected processor
          Status = EfiMpServicesStartupThisAP((EFI_AP_PROCEDURE)GuiServerThread, Index, Event, 0, (VOID *)mGuiServer, NULL);
        }
        VERBOSE(L"%r\n", Status);
        if (EFI_ERROR(Status)) {
          // Try next processor
          continue;
        }
        break;
      }
    }
  }
  // Check if multithreading failed
  if (EFI_ERROR(Status)) {
    // The server is not running should attempt singlethreaded after function return
    mGuiServer->Running = FALSE;
  }
  // Unlock the server
  EfiUnlock(&(mGuiServer->Lock));
  LOG(L"GUI multithread start ... %r\n", Status);
  if (EFI_ERROR(Status)) {
    LOG(L"GUI starting in singlethreaded mode ...\n");
  }
  return Status;
}
// GuiServerRunLoopStep
/// TODO: Run loop for GUI
/// @return Whether the GUI is still running or not
STATIC
BOOLEAN
EFIAPI
GuiServerRunLoopStep (
  VOID
) {
  // Running needs to set to true due to optimization and out of order
  BOOLEAN Running = TRUE;
  // Lock the server
  EfiLock(&(mGuiServer->Lock));
  // Check if the server is still running
  Running = mGuiServer->Running;
  if (Running) {
    // TODO: Server run step

  }
  // Unlock the server
  EfiUnlock(&(mGuiServer->Lock));

  // TODO: Temporarily stall here for a second
  EfiCpuPause();
  // Lock the server
  EfiLock(&(mGuiServer->Lock));
  // TODO: Temporarily just quit
  mGuiServer->Running = FALSE;
  // Unlock the server
  EfiUnlock(&(mGuiServer->Lock));
  // Pause the CPU if needed
  EfiCpuPause();
  return Running;
}
// ServerMultithreadRunLoop
/// TODO: Run loop for multithreaded GUI
/// @param This        The GUI server protocol
/// @retval EFI_INVALID_PARAMETER If Server is NULL or not valid
/// @retvel EFI_UNSUPPORTED       If not multiprocessing capable or the draw thread failed
/// @retval EFI_SUCCESS           The run loop finished successfully
STATIC
EFI_STATUS
EFIAPI
ServerMultithreadRunLoop (
  VOID
) {
  // Running needs to set to true due to optimization and out of order
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    Running = TRUE;
  // TODO: The main run loop
  do {
    // Perform the steps needed in all GUI loops
    Running = GuiServerRunLoopStep();
  } while (Running);
  return Status;
}
// ServerSinglethreadRunLoop
/// TODO: Run loop for single-threaded GUI
/// @param This        The GUI server protocol
/// @retval EFI_INVALID_PARAMETER If Server is NULL or not valid
/// @retval EFI_SUCCESS           The run loop finished successfully
STATIC
EFI_STATUS
EFIAPI
ServerSinglethreadRunLoop (
  VOID
) {
  // Running needs to set to true due to optimization and out of order
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    Running = TRUE;
  // TODO: The main run loop
  do {
    // Perform the steps needed in all GUI loops
    Running = GuiServerRunLoopStep();
  } while (Running);
  return Status;
}
// ServerConsoleRunLoop
/// TODO: Run loop for single-threaded GUI
/// @param This        The GUI server protocol
/// @retval EFI_INVALID_PARAMETER If Server is NULL or not valid
/// @retval EFI_SUCCESS           The run loop finished successfully
STATIC
EFI_STATUS
EFIAPI
ServerConsoleRunLoop (
  VOID
) {
  // Running needs to set to true due to optimization and out of order
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    Running = TRUE;
  // TODO: The main run loop
  do {
    // Perform the steps needed in all GUI loops
    Running = GuiServerRunLoopStep();
  } while (Running);
  return Status;
}
// GuiServerRun
/// GUI server run loop
/// @retval EFI_INVALID_PARAMETER If Server is NULL or not valid
/// @retval EFI_NOT_READY         If the server was not started
/// @retval EFI_SUCCESS           The run loop finished successfully
EFI_STATUS
EFIAPI
GuiServerRun (
  VOID
) {
  // Running needs to set to true due to optimization and out of order
  EFI_STATUS Status = EFI_UNSUPPORTED;
  BOOLEAN    Running = TRUE;
  // Check if the server is started
  if (mGuiServer == NULL) {
    return EFI_NOT_READY;
  }
  // Check if GUI is console only
  if (GuiIsConsoleOnly()) {
    // Lock the server
    EfiLock(&(mGuiServer->Lock));
    // Start running the server
    mGuiServer->Running = TRUE;
    // Unlock the server
    EfiUnlock(&(mGuiServer->Lock));
    // Console only GUI
    Status = ServerConsoleRunLoop();
  } else {
    // Lock the server
    EfiLock(&(mGuiServer->Lock));
    // Redirect console output
    GuiRedirectConsole();
    // Check if the server is running the server
    Running = mGuiServer->Running;
    // Unlock the server
    EfiUnlock(&(mGuiServer->Lock));
    if (GuiIsMultithreaded() && Running) {
      // Multithreaded GUI if already running
      Status = ServerMultithreadRunLoop();
    } else {
      // Lock the server
      EfiLock(&(mGuiServer->Lock));
      // Start running the server
      mGuiServer->Running = TRUE;
      // Unlock the server
      EfiUnlock(&(mGuiServer->Lock));
      // Singlethreaded GUI
      Status = ServerSinglethreadRunLoop();
    }
    // Lock the server
    EfiLock(&(mGuiServer->Lock));
    // Unredirect the console output
    GuiUnredirectConsole();
    // Unlock the server
    EfiUnlock(&(mGuiServer->Lock));
  }
  return Status;
}
// GuiServerFinish
/// GUI server finish execution
/// @retval EFI_INVALID_PARAMETER If This is NULL
/// @retval EFI_SUCCESS           The GUI finished successfully
EFI_STATUS
EFIAPI
GuiServerFinish (
  VOID
) {
  if (mGuiServer != NULL) {
    // Lock the server
    EfiLock(&(mGuiServer->Lock));
    // Stop running the server
    mGuiServer->Running = FALSE;
    // Unlock the server
    EfiUnlock(&(mGuiServer->Lock));
    // Pause the CPU to synchronize
    EfiCpuPause();
    // Lock the server
    EfiLock(&(mGuiServer->Lock));
    // Free the server
    EfiFreePool(mGuiServer);
    mGuiServer = NULL;
  }
  return EFI_SUCCESS;
}
