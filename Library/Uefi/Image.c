///
/// @file Library/Uefi/Image.c
///
/// UEFI image override protocol
///

#include <Uefi.h>

// mEfiImageOverrideProtocolGuid
/// The image override protocol unique identifier
STATIC EFI_GUID mEfiImageOverrideProtocolGuid = { 0xE6366EF6, 0x6AC1, 0x472C, { 0xBB, 0x5F, 0xB3, 0xED, 0x0B, 0x2F, 0x30, 0xA1 } };

// EFI_IMAGE_OVERRIDE_PROTOCOL
/// The image override protocol
typedef struct EFI_IMAGE_OVERRIDE_PROTOCOL EFI_IMAGE_OVERRIDE_PROTOCOL;

// EFI_IMAGE_OVERRIDE_CALLBACK_METHOD
/// The image override callback method
typedef union EFI_IMAGE_OVERRIDE_CALLBACK_METHOD EFI_IMAGE_OVERRIDE_CALLBACK_METHOD;
union EFI_IMAGE_OVERRIDE_CALLBACK_METHOD {

  // Default
  /// Default callback method
  EFI_IMAGE_CALLBACK        Default;
  // Load
  /// Load callback method
  EFI_IMAGE_LOAD_CALLBACK   Load;

};

// EFI_IMAGE_OVERRIDE_CALLBACK
/// The image override callback
typedef struct EFI_IMAGE_OVERRIDE_CALLBACK EFI_IMAGE_OVERRIDE_CALLBACK;
struct EFI_IMAGE_OVERRIDE_CALLBACK {

  // Next
  /// The next callback
  EFI_IMAGE_OVERRIDE_CALLBACK        *Next;
  // ImageHandle
  /// The image handle for the image that registered the callback
  EFI_HANDLE                          ImageHandle;
  // Callback
  /// The callback method
  EFI_IMAGE_OVERRIDE_CALLBACK_METHOD  Callback;
  // Data
  /// The data passed when registering the callback
  VOID                               *Data;

};

// EFI_IMAGE_OVERRIDE_PROTOCOL
/// The image override protocol
struct EFI_IMAGE_OVERRIDE_PROTOCOL {

  // ImageLoad
  /// The original boot service method for image load
  EFI_IMAGE_LOAD         ImageLoad;
  // ImageStart
  /// The original boot service method for image start
  EFI_IMAGE_START        ImageStart;
  // ImageExit
  /// The original boot service method for image exit
  EFI_EXIT               ImageExit;
  // ImageUnload
  /// The original boot service method for image unload
  EFI_IMAGE_UNLOAD       ImageUnload;
  // ExitBootServices
  /// The original boot service method for exit boot services
  EFI_EXIT_BOOT_SERVICES ExitBootServices;

  // ImageLoadCallbacks
  /// The image load callbacks
  EFI_IMAGE_OVERRIDE_CALLBACK *ImageLoadCallbacks;
  // ImageStartCallbacks
  /// The image start callbacks
  EFI_IMAGE_OVERRIDE_CALLBACK *ImageStartCallbacks;
  // ImageExitCallbacks
  /// The image exit callbacks
  EFI_IMAGE_OVERRIDE_CALLBACK *ImageExitCallbacks;
  // ImageUnloadCallbacks
  /// The image unload callbacks
  EFI_IMAGE_OVERRIDE_CALLBACK *ImageUnloadCallbacks;
  // ExitBootServicesCallbacks
  /// The exit boot services callbacks
  EFI_IMAGE_OVERRIDE_CALLBACK *ExitBootServicesCallbacks;

};

// ImageOverrideGetProtocol
/// Get the current image override protocol
STATIC
EFI_IMAGE_OVERRIDE_PROTOCOL *
EFIAPI
ImageOverrideGetProtocol (
  VOID
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = NULL;
  // Check if there is already an image override protocol
  if (EFI_ERROR(EfiLocateProtocol(&mEfiImageOverrideProtocolGuid, NULL, (VOID **)&ImageOverride))) {
    return NULL;
  }
  // Return the image override protocol interface
  return ImageOverride;
}

// ImageOverrideImageLoad
/// Loads an EFI image into memory
/// @param BootPolicy        If TRUE, indicates that the request originates from the boot manager, and that the boot manager is attempting to load the device path as a boot selection, ignored if SourceBuffer is not NULL
/// @param ParentImageHandle The calling image handle
/// @param DevicePath        The DeviceHandle specific file path from which the image is loaded
/// @param SourceBuffer      If not NULL, a pointer to the memory location containing a copy of the image to be loaded
/// @param SourceSize        The size in bytes of SourceBuffer, ignored if SourceBuffer is NULL
/// @param ImageHandle       On output, the image handle that is created when the image is successfully loaded
/// @retval EFI_SUCCESS            Image was loaded into memory correctly
/// @retval EFI_NOT_FOUND          Both SourceBuffer and DevicePath are NULL
/// @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER  ImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_UNSUPPORTED        The image type is not supported
/// @retval EFI_OUT_OF_RESOURCES   Image was not loaded due to insufficient resources
/// @retval EFI_LOAD_ERROR         Image was not loaded because the image format was corrupt or not understood
/// @retval EFI_DEVICE_ERROR       Image was not loaded because the device returned a read error
/// @retval EFI_ACCESS_DENIED      Image was not loaded because the platform policy prohibits the image from being loaded. NULL is returned in *ImageHandle
/// @retval EFI_SECURITY_VIOLATION Image was loaded and an ImageHandle was created with a valid EFI_LOADED_IMAGE_PROTOCOL, however, the current platform policy specifies that the image should not be started
STATIC
EFI_STATUS
EFIAPI
ImageOverrideImageLoad (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Get the image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call the image override load methods
  Callbacks = ImageOverride->ImageLoadCallbacks;
  while (Callbacks != NULL) {
    if (Callbacks->Callback.Load != NULL) {
      Status = Callbacks->Callback.Load(DevicePath, Callbacks->Data);
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }
    }
    Callbacks = Callbacks->Next;
  }
  // Call the original image load method
  return ImageOverride->ImageLoad(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);
}
// ImageOverrideImageStart
/// Transfers control to a loaded image’s entry point
/// @param ImageHandle  Handle of image to be started
/// @param ExitDataSize On output, the size, in bytes, of ExitData. If ExitData is NULL, then this parameter is ignored and the contents of ExitDataSize are not modified
/// @param ExitData     On output, a data buffer that includes a Null-terminated string, optionally followed by additional binary data.
///                      The string is a description that the caller may use to further indicate the reason for the image’s exit
/// @retval EFI_INVALID_PARAMETER  ImageHandle is either an invalid image handle or the image has already been initialized with StartImage 
/// @retval EFI_SECURITY_VIOLATION The current platform policy specifies that the image should not be started
/// @retval "Exit code from image" Exit code from image
STATIC
EFI_STATUS
EFIAPI
ImageOverrideImageStart (
  IN  EFI_HANDLE   ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16     **ExitData OPTIONAL
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Get the image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call the image override start methods
  Callbacks = ImageOverride->ImageStartCallbacks;
  while (Callbacks != NULL) {
    if ((Callbacks->Callback.Default != NULL) && ((Callbacks->ImageHandle == NULL) || (Callbacks->ImageHandle == ImageHandle))) {
      Status = Callbacks->Callback.Default(ImageHandle, Callbacks->Data);
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }
    }
    Callbacks = Callbacks->Next;
  }
  // Call the original image start method
  return ImageOverride->ImageStart(ImageHandle, ExitDataSize, ExitData);
}
// ImageOverrideImageExit
/// Terminates a loaded EFI image and returns control to boot services
/// @param ImageHandle  The handle that identifies the image that was passed to the image on entry
/// @param ExitStatus   The exit status
/// @param ExitDataSize The size, in bytes, of the exit data, ignored if ExitStatus is EFI_SUCCESS
/// @param ExitData     The data buffer that includes a null-terminated string, optionally followed by additional binary data. The string
///                      is a description that the caller may use to further indicate the reason for the image’s exit. ExitData is only
///                      valid if ExitStatus is something other than EFI_SUCCESS. The exit data buffer must be allocated from pool memory
/// @retval EFI_SUCCESS           The image specified by ImageHandle was unloaded. This condition only occurs for images that have been loaded with LoadImage() but have not been started with StartImage()
/// @retval EFI_INVALID_PARAMETER The image specified by ImageHandle has been loaded and started with LoadImage() and StartImage(), but the image is not the currently executing image
STATIC
EFI_STATUS
EFIAPI
ImageOverrideImageExit (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16     *ExitData OPTIONAL
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Get the image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call the image override exit methods
  Callbacks = ImageOverride->ImageExitCallbacks;
  while (Callbacks != NULL) {
    if ((Callbacks->Callback.Default != NULL) && ((Callbacks->ImageHandle == NULL) || (Callbacks->ImageHandle == ImageHandle))) {
      Status = Callbacks->Callback.Default(ImageHandle, Callbacks->Data);
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }
    }
    Callbacks = Callbacks->Next;
  }
  // Call the original image exit method
  return ImageOverride->ImageExit(ImageHandle, ExitStatus, ExitDataSize, ExitData);
}
// ImageOverrideUnload
/// Dummy unload image method
/// @param ImageHandle Handle that identifies the image to be unloaded
/// @retval EFI_SUCCESS The image has been unloaded
STATIC
EFI_STATUS
EFIAPI
ImageOverrideUnload (
  IN EFI_HANDLE ImageHandle
) {
  UNUSED_PARAMETER(ImageHandle);
  return EFI_SUCCESS;
}
// ImageOverrideImageUnload
/// Unloads an image
/// @param ImageHandle Handle that identifies the image to be unloaded
/// @retval EFI_SUCCESS                     The image has been unloaded
/// @retval EFI_UNSUPPORTED                 The image has been started, and does not support unload
/// @retval EFI_INVALID_PARAMETER           ImageHandle is not a valid image handle
/// @retval "Exit code from unload handler" Exit code from the image unload function
STATIC
EFI_STATUS
EFIAPI
ImageOverrideImageUnload (
  IN EFI_HANDLE ImageHandle
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  // Get the image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Check the loaded image protocol has unload method
  LoadedImage = EfiLoadedImage(ImageHandle);
  if (LoadedImage == NULL) {
    return EFI_NOT_FOUND;
  }
  // Set dummy unload method if needed
  if (LoadedImage->Unload == NULL) {
    LoadedImage->Unload = ImageOverrideUnload;
  }
  // Call the image override unload methods
  Callbacks = ImageOverride->ImageUnloadCallbacks;
  while (Callbacks != NULL) {
    if ((Callbacks->Callback.Default != NULL) && ((Callbacks->ImageHandle == NULL) || (Callbacks->ImageHandle == ImageHandle))) {
      Status = Callbacks->Callback.Default(ImageHandle, Callbacks->Data);
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }
    }
    Callbacks = Callbacks->Next;
  }
  // Call the original image unload method
  return ImageOverride->ImageUnload(ImageHandle);
}
// ImageOverrideExitBootServices
/// Terminates all boot services
/// @param ImageHandle Handle that identifies the exiting image
/// @param MapKey      Key to the latest memory map
/// @retval EFI_SUCCESS           Boot services have been terminated
/// @retval EFI_INVALID_PARAMETER MapKey is incorrect
STATIC
EFI_STATUS
EFIAPI
ImageOverrideExitBootServices (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Get the image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call the exit boot services callback methods
  Callbacks = ImageOverride->ExitBootServicesCallbacks;
  while (Callbacks != NULL) {
    if (Callbacks->Callback.Default != NULL) {
      Status = Callbacks->Callback.Default(ImageHandle, Callbacks->Data);
      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }
    }
    Callbacks = Callbacks->Next;
  }
  // Restore original exit boot services method
  gEfiBootServices->ExitBootServices = ImageOverride->ExitBootServices;
  // Update the boot services table CRC32
  gEfiBootServices->Hdr.CRC32 = 0;
  EfiCalculateCrc32(gEfiBootServices, gEfiBootServices->Hdr.HeaderSize, &(gEfiBootServices->Hdr.CRC32));
  // Return invalid parameter to have exit boot services called again
  UNUSED_PARAMETER(MapKey);
  return EFI_INVALID_PARAMETER;
}

// EfiImageOverrideInstall
/// Install image override protocol
/// @retval EFI_SUCCESS The image override protocol was installed
EFI_STATUS
EFIAPI
EfiImageOverrideInstall (
  VOID
) {
  EFI_STATUS Status;
  // Check if there is already a image override protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  if (ImageOverride != NULL) {
    return EFI_SUCCESS;
  }
  // Allocate the image override protocol implementation interface
  ImageOverride = EfiAllocateByType(EFI_IMAGE_OVERRIDE_PROTOCOL);
  if (ImageOverride == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Setup the image override callbacks
  ImageOverride->ImageLoadCallbacks = NULL;
  ImageOverride->ImageStartCallbacks = NULL;
  ImageOverride->ImageExitCallbacks = NULL;
  ImageOverride->ImageUnloadCallbacks = NULL;
  ImageOverride->ExitBootServicesCallbacks = NULL;
  // Save the image methods from boot services
  ImageOverride->ImageLoad = gEfiBootServices->LoadImage;
  ImageOverride->ImageStart = gEfiBootServices->StartImage;
  ImageOverride->ImageExit = gEfiBootServices->Exit;
  ImageOverride->ImageUnload = gEfiBootServices->UnloadImage;
  ImageOverride->ExitBootServices = gEfiBootServices->ExitBootServices;
  // Replace the boot services methods
  gEfiBootServices->LoadImage = ImageOverrideImageLoad;
  gEfiBootServices->StartImage = ImageOverrideImageStart;
  gEfiBootServices->Exit = ImageOverrideImageExit;
  gEfiBootServices->UnloadImage = ImageOverrideImageUnload;
  gEfiBootServices->ExitBootServices = ImageOverrideExitBootServices;
  gEfiBootServices->Hdr.CRC32 = 0;
  Status = gEfiBootServices->CalculateCrc32(gEfiBootServices, gEfiBootServices->Hdr.HeaderSize, &(gEfiBootServices->Hdr.CRC32));
  // Install the image override protocol
  if (!EFI_ERROR(Status)) {
    Status = EfiInstallProtocolInterface(&gEfiImageHandle, &mEfiImageOverrideProtocolGuid, EFI_NATIVE_INTERFACE, (VOID *)ImageOverride);
  }
  if (EFI_ERROR(Status)) {
    // Free the image override protocol interface since there was an error
    EfiFreePool((VOID *)ImageOverride);
  }
  // Return the status code
  return Status;
}
// EfiImageOverrideUninstall
/// Uninstall image override protocol
/// @param ImageHandle The image handle or NULL to not call unload callback
/// @retval EFI_SUCCESS The configuration protocol was installed
EFI_STATUS
EFIAPI
EfiImageOverrideUninstall (
  IN EFI_HANDLE ImageHandle OPTIONAL
) {
  EFI_STATUS                   Status;
  EFI_IMAGE_OVERRIDE_CALLBACK *Callback;
  // Get protocol
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  // Check protocol is installed
  if (ImageOverride == NULL) {
    return EFI_NOT_FOUND;
  }
  // Uninstall the protocol
  Status = EfiUninstallProtocolInterface(gEfiImageHandle, &mEfiImageOverrideProtocolGuid, (VOID *)ImageOverride);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Call the unload method if needed
  if (ImageHandle != NULL) {
    Callback = ImageOverride->ImageUnloadCallbacks;
    while (Callback != NULL) {
      if ((Callback->ImageHandle == ImageHandle) && (Callback->Callback.Default != NULL)) {
        Callback->Callback.Default(ImageHandle, Callback->Data);
      }
      Callback = Callback->Next;
    }
  }
  // Restore original boot services methods
  gEfiBootServices->LoadImage = ImageOverride->ImageLoad;
  gEfiBootServices->StartImage = ImageOverride->ImageStart;
  gEfiBootServices->Exit = ImageOverride->ImageExit;
  gEfiBootServices->UnloadImage = ImageOverride->ImageUnload;
  gEfiBootServices->ExitBootServices = ImageOverride->ExitBootServices;
  // Update the boot services table CRC32
  gEfiBootServices->Hdr.CRC32 = 0;
  EfiCalculateCrc32(gEfiBootServices, gEfiBootServices->Hdr.HeaderSize, &(gEfiBootServices->Hdr.CRC32));
  // Free the image override callbacks
  Callback = ImageOverride->ImageLoadCallbacks;
  ImageOverride->ImageLoadCallbacks = NULL;
  while (Callback != NULL) {
    EFI_IMAGE_OVERRIDE_CALLBACK *Next = Callback->Next;
    EfiFreePool((VOID *)Callback);
    Callback = Next;
  }
  Callback = ImageOverride->ImageStartCallbacks;
  ImageOverride->ImageStartCallbacks = NULL;
  while (Callback != NULL) {
    EFI_IMAGE_OVERRIDE_CALLBACK *Next = Callback->Next;
    EfiFreePool((VOID *)Callback);
    Callback = Next;
  }
  Callback = ImageOverride->ImageExitCallbacks;
  ImageOverride->ImageExitCallbacks = NULL;
  while (Callback != NULL) {
    EFI_IMAGE_OVERRIDE_CALLBACK *Next = Callback->Next;
    EfiFreePool((VOID *)Callback);
    Callback = Next;
  }
  Callback = ImageOverride->ImageUnloadCallbacks;
  ImageOverride->ImageUnloadCallbacks = NULL;
  while (Callback != NULL) {
    EFI_IMAGE_OVERRIDE_CALLBACK *Next = Callback->Next;
    EfiFreePool((VOID *)Callback);
    Callback = Next;
  }
  Callback = ImageOverride->ExitBootServicesCallbacks;
  ImageOverride->ExitBootServicesCallbacks = NULL;
  while (Callback != NULL) {
    EFI_IMAGE_OVERRIDE_CALLBACK *Next = Callback->Next;
    EfiFreePool((VOID *)Callback);
    Callback = Next;
  }
  // Free the image override protocol
  return EfiFreePool((VOID *)ImageOverride);
}

// EfiImageLoadCallback
/// Set an image load callback
/// @param Callback     The callback function to call when an image is being loaded
/// @param CallbackData The data to pass to the callback function
EFI_STATUS
EFIAPI
EfiImageLoadCallback (
  IN EFI_IMAGE_LOAD_CALLBACK  Callback,
  IN VOID                    *CallbackData OPTIONAL
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Check parameters
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (Callback == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Create callback
  Callbacks = EfiAllocateByType(EFI_IMAGE_OVERRIDE_CALLBACK);
  if (Callbacks == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set callback information
  Callbacks->ImageHandle = gEfiImageHandle;
  Callbacks->Callback.Load = Callback;
  Callbacks->Data = CallbackData;
  // Add callback
  Callbacks->Next = ImageOverride->ImageLoadCallbacks;
  ImageOverride->ImageLoadCallbacks = Callbacks;
  return EFI_SUCCESS;
}
// EfiImageStartCallback
/// Set an image start callback
/// @param Callback     The callback function to call when an image is started
/// @param CallbackData The data to pass to the callback function
EFI_STATUS
EFIAPI
EfiImageStartCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Check parameters
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (Callback == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Create callback
  Callbacks = EfiAllocateByType(EFI_IMAGE_OVERRIDE_CALLBACK);
  if (Callbacks == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set callback information
  Callbacks->ImageHandle = gEfiImageHandle;
  Callbacks->Callback.Default = Callback;
  Callbacks->Data = CallbackData;
  // Add callback
  Callbacks->Next = ImageOverride->ImageStartCallbacks;
  ImageOverride->ImageStartCallbacks = Callbacks;
  return EFI_SUCCESS;
}
// EfiImageExitCallback
/// Set an image exit callback
/// @param Callback     The callback function to call when this image is exited
/// @param CallbackData The data to pass to the callback function
EFI_STATUS
EFIAPI
EfiImageExitCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Check parameters
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (Callback == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Create callback
  Callbacks = EfiAllocateByType(EFI_IMAGE_OVERRIDE_CALLBACK);
  if (Callbacks == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set callback information
  Callbacks->ImageHandle = gEfiImageHandle;
  Callbacks->Callback.Default = Callback;
  Callbacks->Data = CallbackData;
  // Add callback
  Callbacks->Next = ImageOverride->ImageExitCallbacks;
  ImageOverride->ImageExitCallbacks = Callbacks;
  return EFI_SUCCESS;
}
// EfiImageUnloadCallback
/// Set an image unload callback for this image
/// @param Callback     The callback function to call when this image is unloaded
/// @param CallbackData The data to pass to the callback function
EFI_STATUS
EFIAPI
EfiImageUnloadCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Check parameters
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (Callback == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Create callback
  Callbacks = EfiAllocateByType(EFI_IMAGE_OVERRIDE_CALLBACK);
  if (Callbacks == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set callback information
  Callbacks->ImageHandle = gEfiImageHandle;
  Callbacks->Callback.Default = Callback;
  Callbacks->Data = CallbackData;
  // Add callback
  Callbacks->Next = ImageOverride->ImageUnloadCallbacks;
  ImageOverride->ImageUnloadCallbacks = Callbacks;
  return EFI_SUCCESS;
}
// EfiExitBootServicesCallback
/// Set an exit boot services callback
/// @param Callback     The callback function to call when exit boot services is called
/// @param CallbackData The data to pass to the callback function
EFI_STATUS
EFIAPI
EfiExitBootServicesCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
) {
  EFI_IMAGE_OVERRIDE_PROTOCOL *ImageOverride = ImageOverrideGetProtocol();
  EFI_IMAGE_OVERRIDE_CALLBACK *Callbacks;
  // Check parameters
  if (ImageOverride == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (Callback == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Create callback
  Callbacks = EfiAllocateByType(EFI_IMAGE_OVERRIDE_CALLBACK);
  if (Callbacks == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set callback information
  Callbacks->ImageHandle = gEfiImageHandle;
  Callbacks->Callback.Default = Callback;
  Callbacks->Data = CallbackData;
  // Add callback
  Callbacks->Next = ImageOverride->ExitBootServicesCallbacks;
  ImageOverride->ExitBootServicesCallbacks = Callbacks;
  return EFI_SUCCESS;
}
