///
/// @file Library/Uefi/EntryPoint.c
///
/// UEFI entry point
///

#include "VirtualMemory.h"

// gEfiImageHandle
/// The image handle for the current execution image
EFI_HANDLE gEfiImageHandle;
// gEfiSystemTable
/// The system services table
EFI_SYSTEM_TABLE *gEfiSystemTable;
// gEfiBootServices
/// The boot services table
EFI_BOOT_SERVICES *gEfiBootServices;
// gEfiRuntimeServices
/// The runtime services table
EFI_RUNTIME_SERVICES *gEfiRuntimeServices;
// gEfiConIn
/// The default console input
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *gEfiConIn;
// gEfiConOut
/// The default console output
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *gEfiConOut;

// gEfiSimpleTextOutputProtocolGuid
/// The simple text output protocol unique identifier
EFI_GUID gEfiSimpleTextOutputProtocolGuid = EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID;
// gEfiSimpleTextInputProtocolGuid
/// The simple text input protocol unique identifier
EFI_GUID gEfiSimpleTextInputProtocolGuid = EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID;
// gEfiSimpleTextInputExProtocolGuid
/// Extended simple text input protocol unique identifier
EFI_GUID gEfiSimpleTextInputExProtocolGuid = EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID;

// gEfiSimplePointerProtocolGuid
/// Simple pointer protocol unique identifier
EFI_GUID gEfiSimplePointerProtocolGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
// gEfiAbsolutePointerProtocolGuid
/// Absolute pointer protocol unique identifier
EFI_GUID gEfiAbsolutePointerProtocolGuid = EFI_ABSOLUTE_POINTER_PROTOCOL_GUID;

// gEfiGraphicsOutputProtocolGuid
/// Graphics output protocol unique identifier
EFI_GUID gEfiGraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
// gEfiUgaDrawProtocolGuid
/// Universal graphics adapter protocol unique identifier
EFI_GUID gEfiUgaDrawProtocolGuid = EFI_UGA_DRAW_PROTOCOL_GUID;

// gEfiLoadedImageProtocolGuid
/// The loaded image protocol unique identifier
EFI_GUID gEfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

// gEfiFileInfoGuid
/// The file information unique identifier
EFI_GUID gEfiFileInfoGuid = EFI_FILE_INFO_GUID;
// gEfiFileSystemInfoGuid
/// The file system information unique identifier
EFI_GUID gEfiFileSystemInfoGuid = EFI_FILE_SYSTEM_INFO_GUID;
// gEfiFileSystemVolumeLabelInfoGuid
/// The file system volume label information unique identifier
EFI_GUID gEfiFileSystemVolumeLabelInfoGuid = EFI_FILE_SYSTEM_VOLUME_LABEL_INFO_GUID;

#if defined(_MSC_VER)

// _fltused
/// Floating point use for MSVC
int _fltused = 1;

#endif

// EfiEncodingInstall
/// Install encoding protocols
/// @retval EFI_SUCCESS The encoding protocols were installed
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingInstall (
  VOID
);
// EfiEncodingUninstall
/// Uninstall encoding protocols
/// @retval EFI_SUCCESS The encoding protocols were uninstalled
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingUninstall (
  VOID
);

// EfiLocaleInstall
/// Install locale protocols
/// @retval EFI_SUCCESS The locale protocols were installed
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleInstall (
  VOID
);
// EfiLocaleUninstall
/// Uninstall locale protocols
/// @retval EFI_SUCCESS The locale protocols were uninstalled
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleUninstall (
  VOID
);

// EfiLogInstall
/// Install logging protocol
/// @retval EFI_SUCCESS The logging protocol was installed
EXTERN
EFI_STATUS
EFIAPI
EfiLogInstall (
  VOID
);
// EfiLogUninstall
/// Uninstall logging protocol
/// @retval EFI_SUCCESS The logging protocol was uninstalled
EXTERN
EFI_STATUS
EFIAPI
EfiLogUninstall (
  VOID
);

// EfiConfigurationInstall
/// Install the configuration protocol
/// @retval EFI_SUCCESS The configuration protocol was installed
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationInstall (
  VOID
);
// EfiConfigurationUninstall
/// Uninstall the configuration protocol
/// @retval EFI_SUCCESS The configuration protocol was installed
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationUninstall (
  VOID
);

// EfiImageOverrideInstall
/// Install image override protocol
/// @retval EFI_SUCCESS The configuration protocol was installed
EXTERN
EFI_STATUS
EFIAPI
EfiImageOverrideInstall (
  VOID
);
// EfiImageOverrideUninstall
/// Uninstall image override protocol
/// @param ImageHandle The image handle or NULL to not call unload callback
/// @retval EFI_SUCCESS The configuration protocol was installed
EXTERN
EFI_STATUS
EFIAPI
EfiImageOverrideUninstall (
  IN EFI_HANDLE ImageHandle OPTIONAL
);

// EfiEntryPointInstall
/// Install all entry point protocols
/// @retval EFI_SUCCESS The protocols were installed
STATIC
EFI_STATUS
EFIAPI
EfiEntryPointInstall (
  VOID
) {
  EFI_STATUS Status;
#if defined(EFI_MEMORY_VIRTUAL)
  // Install virtual memory override services
  Status = EfiVirtualMemoryInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
#endif
  // Install the encoding protocols
  Status = EfiEncodingInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Install the locale protocols
  Status = EfiLocaleInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Install log protocol
  Status = EfiLogInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Print the current memory map
#if defined(EFI_DEBUG)
  EfiPrintCurrentMemoryMap();
#endif
  // Install configuration protocol
  Status = EfiConfigurationInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Install image override protocol
  Status = EfiImageOverrideInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  return Status;
}

// EfiEntryPointUninstall
/// Uninstall all entry point protocols
/// @param ImageHandle The image handle
/// @retval EFI_SUCCESS The protocols were uninstalled
STATIC
EFI_STATUS
EFIAPI
EfiEntryPointUninstall (
  IN EFI_HANDLE ImageHandle OPTIONAL
) {
  // Uninstall image override protocol
  EfiImageOverrideUninstall(ImageHandle);
  // Uninstall configuration protocol
  EfiConfigurationUninstall();
#if defined(EFI_MEMORY_VIRTUAL)
  // Print memory allocation records
  EfiPrintMemoryRecords();
#endif
#if defined(EFI_DEBUG)
  // Print the current memory map
  EfiPrintCurrentMemoryMap();
#endif
  // Uninstall log protocol
  EfiLogUninstall();
  // Uninstall the locale protocols
  EfiLocaleUninstall();
  // Uninstall the encoding protocols
  EfiEncodingUninstall();
#if defined(EFI_MEMORY_VIRTUAL)
  // Uninstall virtual memory override services
  EfiVirtualMemoryUninstall();
#endif
  return EFI_SUCCESS;
}

// EfiEntryPointImageUnload
/// Uninstalls protocols when image unloads
/// @param ImageHandle The image handle
/// @param Data        The data used when registering the callback
STATIC
EFI_STATUS
EFIAPI
EfiEntryPointImageUnload (
  IN EFI_HANDLE  ImageHandle,
  IN VOID       *Data
) {
  // Uninstall entry point protocols
  UNUSED_PARAMETER(Data);
  UNUSED_PARAMETER(ImageHandle);
  return EfiEntryPointUninstall(NULL);
}

// EfiDriverEntryPoint
/// Driver entry point for execution
/// @param ImageHandle The image handle for this execution
/// @param SystemTable The UEFI system table
/// @return Whether the execution failed to start, fatally aborted or completed successfully
EFI_STATUS
EFIAPI
EfiDriverEntryPoint (
  EFI_HANDLE        ImageHandle,
  EFI_SYSTEM_TABLE *SystemTable
) {
  EFI_STATUS Status;
  // Set the image handle and system table for this execution
  gEfiImageHandle = ImageHandle;
  gEfiSystemTable = SystemTable;
  // Set the console output, boot services and runtime services
  if (SystemTable != NULL) {
    gEfiConIn = SystemTable->ConIn;
    gEfiConOut = SystemTable->ConOut;
    gEfiBootServices = SystemTable->BootServices;
    gEfiRuntimeServices = SystemTable->RuntimeServices;
  }
  // Install entry point protocols
  Status = EfiEntryPointInstall();
  if (!EFI_ERROR(Status)) {
    // Add image unload callback to uninstall protocols
    Status = EfiImageUnloadCallback(EfiEntryPointImageUnload, NULL);
    if (!EFI_ERROR(Status)) {
      // Call UEFI module entry point
      Status = EfiEntryPoint();
    }
  }
  // On error, the driver should be unloaded so uninstall protocols
  if (EFI_ERROR(Status)) {
    EfiEntryPointUninstall(gEfiImageHandle);
  }
  // Return entry point exit status code
  return Status;
}

// EfiApplicationEntryPoint
/// Application entry point for execution
/// @param ImageHandle The image handle for this execution
/// @param SystemTable The UEFI system table
/// @return Whether the execution failed to start, fatally aborted or completed successfully
EFI_STATUS
EFIAPI
EfiApplicationEntryPoint (
  EFI_HANDLE        ImageHandle,
  EFI_SYSTEM_TABLE *SystemTable
) {
  EFI_STATUS Status;
  // Set the image handle and system table for this execution
  gEfiImageHandle = ImageHandle;
  gEfiSystemTable = SystemTable;
  // Set the console output, boot services and runtime services
  if (SystemTable != NULL) {
    gEfiConIn = SystemTable->ConIn;
    gEfiConOut = SystemTable->ConOut;
    gEfiBootServices = SystemTable->BootServices;
    gEfiRuntimeServices = SystemTable->RuntimeServices;
  }
  // Disable watchdog timer
  Status = EfiSetWatchdogTimer(0, 0, 0, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Install entry point protocols
  Status = EfiEntryPointInstall();
  // Call UEFI module entry point
  if (!EFI_ERROR(Status)) {
    Status = EfiEntryPoint();
  }
  // Uninstall entry point protocols
  EfiEntryPointUninstall(gEfiImageHandle);
  // Return entry point exit status code
  return Status;
}
