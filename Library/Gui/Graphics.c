///
/// @file Library/Gui/Graphics.c
///
/// Graphics engine
///

#include <Gui/Client.h>

// GUI_GRAPHICS_DISPLAY
/// Graphics display
struct GUI_GRAPHICS_DISPLAY {

  // Next
  /// The next graphics display
  GUI_GRAPHICS_DISPLAY         *Next;
  // Output
  /// The output protocol
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Output;
  // HorizontalOffset
  /// Horizontal offset position
  UINTN                         HorizontalOffset;
  // VerticalOffset
  /// Vertical offset position
  UINTN                         VerticalOffset;

};
// GUI_GRAPHICS
/// Graphics
struct GUI_GRAPHICS {

  // Type
  /// The display type
  GUI_GRAPHICS_TYPE     Type;
  // Displays
  /// The displays
  GUI_GRAPHICS_DISPLAY *Displays;
  // Objects
  /// The objects to draw on this screen
  //GUI_OBJECT           *Objects;

};

// GuiGraphicsStart
/// TODO: Start the graphics engine by enumerating the graphics adapters and initializing graphics displays
/// @param Graphics On output, the graphics engine
/// @retval EFI_INVALID_PARAMETER If Graphics is NULL or *Graphics is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If there was not enough resources to initialize the graphics engine
/// @retval EFI_SUCCESS           The graphics engine was started successfully
EFI_STATUS
EFIAPI
GuiGraphicsStart (
  OUT GUI_GRAPHICS **Graphics
) {
  // Check parameters
  if ((Graphics == NULL) || (*Graphics != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  return EFI_UNSUPPORTED;
}
// GuiGraphicsFinish
/// TODO: Finish the graphics engine
/// @param Graphics The graphics engine
/// @retval EFI_INVALID_PARAMETER If Graphics is NULL
/// @retval EFI_SUCCESS           The graphics engine was finished and can no longer be used
EFI_STATUS
EFIAPI
GuiGraphicsFinish (
  IN GUI_GRAPHICS *Graphics
) {
  // Check parameters
  if (Graphics == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  return EFI_UNSUPPORTED;
}

//#define GRAPHICS_DEFAULT_WIDTH 1024
//#define GRAPHICS_DEFAULT_HEIGHT 768
//
//// mAdapters
///// The graphics adapters
//STATIC ObjectList<CONST Device> mAdapters;
//
//// Graphics::~Device
///// Graphics device destructor
//Device::~Device (
//  VOID
//) {
//}
//
//// Graphics::Device
///// Graphics device constructor
///// @param Handle The handle for the graphics device
//Device::Device (
//  EFI_HANDLE Handle
//) {
//  mHandle = Handle;
//}
//
//// GetResolution
///// Get the resolution of the graphics
///// @return The resolution of the graphics
//Size
//Device::GetResolution (
//  VOID
//) CONST {
//  return Size(GRAPHICS_DEFAULT_WIDTH, GRAPHICS_DEFAULT_HEIGHT);
//}
//// GetResolutions
///// Get the available resolutions of the graphics
///// @return The available resolutions of the graphics
//List<Size>
//Device::GetResolutions (
//  VOID
//) CONST {
//  List<Size> AvailableResolutions;
//  AvailableResolutions.Add(Size(GRAPHICS_DEFAULT_WIDTH, GRAPHICS_DEFAULT_HEIGHT));
//  return AvailableResolutions;
//}
//
//// Graphics::Device::GetHandle
///// Get the handle for graphicsthe device
///// @return The graphics device handle
//EFI_HANDLE
//Device::GetHandle (
//  VOID
//) CONST {
//  return mHandle;
//}
//
//// SwitchToMode
///// Switch to graphics mode
///// @param Resolution The resolution of the graphics mode to switch
///// @return Whether the graphics mode was changed or not
//EFI_STATUS
//Device::SwitchToMode (
//  CONST Size & // Resolution
//) {
//  return EFI_UNSUPPORTED;
//}
//// SwitchToBestMode
///// Switch to the best graphics mode
///// @return Whether the graphics mode was changed or not
//BOOLEAN
//Device::SwitchToBestMode (
//  VOID
//) {
//  Size CurrentResolution = GetResolution();
//  Size BestResolution;
//  Boot::Console << L"    Current graphics mode: " << CurrentResolution.Width << L"x" << CurrentResolution.Height << NewLine;
//  Boot::Console << L"    Detecting best graphics mode ..." << NewLine;
//  List<Size> AvailableResolutions = GetResolutions();
//  for (UINTN Index = 0; Index < AvailableResolutions.GetCount(); ++Index) {
//    Boot::Console << L"      Graphics mode: " << AvailableResolutions[Index].Width << L"x" << AvailableResolutions[Index].Height << NewLine;
//    if ((AvailableResolutions[Index].Width * AvailableResolutions[Index].Height) > (BestResolution.Width * BestResolution.Height)){
//      BestResolution = AvailableResolutions[Index];
//    }
//  }
//  Boot::Console << L"    Best graphics mode: " << BestResolution.Width << L"x" << BestResolution.Height << NewLine;
//  if (BestResolution == CurrentResolution) {
//    Boot::Console << L"    Best graphics mode is current graphics mode" << NewLine;
//    return TRUE;
//  }
//  Boot::Console << L"    Switching to best graphics mode ... ";
//  EFI_STATUS Status = SwitchToMode(BestResolution);
//  Boot::Console << EfiStatusToString(Status) << NewLine;
//  return !EFI_ERROR(Status);
//}
//
//// EfiDevice::~EfiDevice
///// UEFI graphics destructor
//EfiDevice::~EfiDevice (
//  VOID
//) {
//}
//
//// EfiDevice::EfiDevice
///// UEFI graphics constructor
///// @param Handle The handle for the graphics device
///// @param GOP    The graphics output protocol interface
//EfiDevice::EfiDevice (
//  EFI_HANDLE                    Handle,
//  EFI_GRAPHICS_OUTPUT_PROTOCOL *Graphics
//) : Device(Handle) {
//  mGraphics = Graphics;
//}
//
//// EfiDevice::Create
///// UEFI graphics constructor
///// @param Handle The handle for the graphics device
///// @param GOP    The graphics output protocol interface
///// @return The UEFI graphics device
//EfiDevice *
//EfiDevice::Create (
//  EFI_HANDLE                    Handle,
//  EFI_GRAPHICS_OUTPUT_PROTOCOL *Graphics
//) {
//  if ((Handle == NULL) || (Graphics == NULL)) {
//    return NULL;
//  }
//  return new EfiDevice(Handle, Graphics);
//}
//
//// GetResolution
///// Get the resolution of the graphics
///// @return The resolution of the graphics
//Size
//EfiDevice::GetResolution (
//  VOID
//) CONST {
//  if ((mGraphics == NULL) || (mGraphics->Mode == NULL) || (mGraphics->Mode->Info == NULL)) {
//    return Size(GRAPHICS_DEFAULT_WIDTH, GRAPHICS_DEFAULT_HEIGHT);
//  }
//  return Size(mGraphics->Mode->Info->HorizontalResolution, mGraphics->Mode->Info->VerticalResolution);
//}
//// GetResolutions
///// Get the available resolutions of the graphics
///// @return The available resolutions of the graphics
//List<Size>
//EfiDevice::GetResolutions (
//  VOID
//) CONST {
//  List<Size> AvailableResolutions;
//  // Check if graphics modes are available
//  if ((mGraphics != NULL) && (mGraphics->Mode != NULL) && (mGraphics->QueryMode != NULL)) {
//    UINT32 MaxMode = mGraphics->Mode->MaxMode;
//    // Iterate through the graphics modes
//    for (UINT32 Index = 0; Index < MaxMode; ++Index) {
//      EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = NULL;
//      UINTN                                 InfoSize = 0;
//      // Get each graphics resolution
//      if (!EFI_ERROR(mGraphics->QueryMode(mGraphics, Index, &InfoSize, &Info)) && (Info != NULL)) {
//        AvailableResolutions.AddOnce(Size(Info->HorizontalResolution, Info->VerticalResolution));
//        EfiFreePool(Info);
//      }
//    }
//  }
//  // Add the default resolution if no other resolutions
//  if (AvailableResolutions.GetCount() == 0) {
//    AvailableResolutions.Add(GetResolution());
//  }
//  return AvailableResolutions;
//}
//
//// SwitchToMode
///// Switch to graphics mode
///// @param Resolution The resolution of the graphics mode to switch
///// @return Whether the graphics mode was changed or not
//EFI_STATUS
//EfiDevice::SwitchToMode (
//  CONST Size &Resolution
//) {
//  if ((mGraphics == NULL) || (mGraphics->Mode == NULL) ||
//      (mGraphics->QueryMode == NULL) || (mGraphics->SetMode == NULL)) {
//    return EFI_UNSUPPORTED;
//  }
//  // Determine the mode number
//  UINT32 Mode = mGraphics->Mode->Mode;
//  for (UINT32 Index = 0; Index < mGraphics->Mode->MaxMode; ++Index) {
//    UINTN Size = 0;
//    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = NULL;
//    if (!EFI_ERROR(mGraphics->QueryMode(mGraphics, Index, &Size, &Info)) && (Info != NULL)) {
//      if ((Resolution.Width == Info->HorizontalResolution) && (Resolution.Height == Info->VerticalResolution)) {
//        Mode = Index;
//      }
//      EfiFreePool(Info);
//    }
//  }
//  // Check if already the mode
//  if (Mode == mGraphics->Mode->Mode) {
//    return EFI_SUCCESS;
//  }
//  // Change the mode
//  return mGraphics->SetMode(mGraphics, Mode);
//}
//
//// EFIDevice::~EFIDevice
///// EFI graphics destructor
//EFIDevice::~EFIDevice (
//  VOID
//) {
//}
//
//// EFIDevice::EFIDevice
///// EFI graphics constructor
///// @param Handle The handle for the graphics device
///// @param GOP    The universal graphics adapter protocol interface
//EFIDevice::EFIDevice (
//  EFI_HANDLE             Handle,
//  EFI_UGA_DRAW_PROTOCOL *Graphics
//) : Device(Handle) {
//  mGraphics = Graphics;
//}
//
//// EFIDevice::Create
///// EFI graphics constructor
///// @param Handle The handle for the graphics device
///// @param GOP    The universal graphics adapter protocol interface
///// @return The EFI graphics device
//EFIDevice *
//EFIDevice::Create (
//  EFI_HANDLE             Handle,
//  EFI_UGA_DRAW_PROTOCOL *Graphics
//) {
//  if ((Handle == NULL) || (Graphics == NULL)) {
//    return NULL;
//  }
//  return new EFIDevice(Handle, Graphics);
//}
//
//// GetResolution
///// Get the resolution of the graphics
///// @return The resolution of the graphics
//Size
//EFIDevice::GetResolution (
//  VOID
//) CONST {
//  UINT32 Width = GRAPHICS_DEFAULT_WIDTH;
//  UINT32 Height = GRAPHICS_DEFAULT_HEIGHT;
//  UINT32 Depth = 0;
//  UINT32 Rate = 0;
//  if ((mGraphics == NULL) || (mGraphics->GetMode == NULL) || EFI_ERROR(mGraphics->GetMode(mGraphics, &Width, &Height, &Depth, &Rate))) {
//    return Size(GRAPHICS_DEFAULT_WIDTH, GRAPHICS_DEFAULT_HEIGHT);
//  }
//  return Size(Width, Height);
//}
//// GetResolutions
///// Get the available resolutions of the graphics
///// @return The available resolutions of the graphics
//List<Size>
//EFIDevice::GetResolutions (
//  VOID
//) CONST {
//  List<Size> AvailableResolutions;
//  if ((mGraphics != NULL) && (mGraphics->GetMode != NULL)) {
//    UINT32 Width = GRAPHICS_DEFAULT_WIDTH;
//    UINT32 Height = GRAPHICS_DEFAULT_HEIGHT;
//    UINT32 Depth = 0;
//    UINT32 Rate = 0;
//    if (!EFI_ERROR(mGraphics->GetMode(mGraphics, &Width, &Height, &Depth, &Rate))) {
//      AvailableResolutions.AddOnce(Size(Width, Height));
//    }
//  }
//  // Add the default resolution if no other resolutions
//  if (AvailableResolutions.GetCount() == 0) {
//    AvailableResolutions.Add(GetResolution());
//  }
//  return AvailableResolutions;
//}
//
//// SwitchToMode
///// Switch to graphics mode
///// @param Resolution The resolution of the graphics mode to switch
///// @return Whether the graphics mode was changed or not
//EFI_STATUS
//EFIDevice::SwitchToMode (
//  CONST Size &Resolution
//) {
//  if ((mGraphics == NULL) || (mGraphics->GetMode == NULL) || (mGraphics->SetMode == NULL)) {
//    return EFI_UNSUPPORTED;
//  }
//  UINT32 Width = GRAPHICS_DEFAULT_WIDTH;
//  UINT32 Height = GRAPHICS_DEFAULT_HEIGHT;
//  UINT32 Depth = 0;
//  UINT32 Rate = 0;
//  // Get the current mode
//  EFI_STATUS Status = mGraphics->GetMode(mGraphics, &Width, &Height, &Depth, &Rate);
//  if (EFI_ERROR(Status)) {
//    return Status;
//  }
//  // Check if already correct mode
//  if ((Resolution.Width == Width) && (Resolution.Height == Height)) {
//    return EFI_SUCCESS;
//  }
//  // Change mode
//  return mGraphics->SetMode(mGraphics, (UINT32)Resolution.Width, (UINT32)Resolution.Height, Depth, Rate);
//}
//
//
//// Adapters
///// Get the graphics adapters
///// @return The graphics adapters
//CONST ObjectList<CONST Device> &
//Adapters (
//  VOID
//) {
//  return mAdapters;
//}
//
//// Enumerate
///// Enumerate graphics
//VOID
//Enumerate (
//  VOID
//) {
//  List<EFI_HANDLE>  GraphicsHandles;
//  EFI_HANDLE       *Handles = NULL;
//  UINTN             Count = 0;
//  mAdapters.Clear();
//  Boot::Console << L"Enumerating graphics ..." << NewLine;
//  // Locate all the graphics output protocol handles
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
//    // Iterate through each handle and get the graphics output protocol interface
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      if (Handles[Index] != NULL) {
//        // Add the handle to the list of handles
//        GraphicsHandles.AddOnce(Handles[Index]);
//      }
//    }
//    EfiFreePool(Handles);
//  }
//  // Locate all the universal graphics adapter protocol handles
//  Count = 0;
//  Handles = NULL;
//  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiUgaDrawProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
//    // Iterate through each handle and get the universal graphics adapter protocol interface
//    for (UINTN Index = 0; Index < Count; ++Index) {
//      if (Handles[Index] != NULL) {
//        // Add the handle to the list of handles
//        GraphicsHandles.AddOnce(Handles[Index]);
//      }
//    }
//    EfiFreePool(Handles);
//  }
//  // Iterate through the graphics handles, only handle one of the graphics protocols with preference for graphics output protocol
//  for (UINTN Index = 0; Index < GraphicsHandles.GetCount(); ++Index) {
//    EFI_HANDLE Handle = GraphicsHandles[Index];
//    if (Handle != NULL) {
//     // Get the graphics output protocol interface
//      EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = NULL;
//      if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiGraphicsOutputProtocolGuid, (VOID **)&GOP)) && (GOP != NULL)) {
//        CHAR16 *DevicePath = EfiDevicePathFullTextFromHandle(Handle);
//        if (DevicePath != NULL) {
//          Boot::Console << L"  Found UEFI graphics: " << DevicePath << NewLine;
//          EfiFreePool(DevicePath);
//          // Create a new graphics instance from graphics output protocol interface
//          EfiDevice *Graphics = EfiDevice::Create(Handle, GOP);
//          if (Graphics != NULL) {
//            // Add the graphics to the graphics list
//            mAdapters.Add(Graphics);
//            // Switch to best graphics mode
//            Graphics->SwitchToBestMode();
//            Graphics->Release();
//            continue;
//          }
//        }
//      }
//      // Get the universal graphics adapter protocol interface
//      EFI_UGA_DRAW_PROTOCOL *UGA = NULL;
//      if (!EFI_ERROR(EfiHandleProtocol(Handle, &gEfiUgaDrawProtocolGuid, (VOID **)&UGA)) && (UGA != NULL)) {
//        CHAR16 *DevicePath = EfiDevicePathFullTextFromHandle(Handle);
//        if (DevicePath != NULL) {
//          Boot::Console << L"  Found EFI graphics: " << DevicePath << NewLine;
//          EfiFreePool(DevicePath);
//          // Create a new graphics instance from universal graphics adapter interface
//          EFIDevice *Graphics = EFIDevice::Create(Handle, UGA);
//          if (Graphics != NULL) {
//            // Add the graphics to the graphics list
//            mAdapters.Add(Graphics);
//            // Switch to best graphics mode
//            Graphics->SwitchToBestMode();
//            Graphics->Release();
//          }
//        }
//      }
//    }
//  }
//}
//
//};
//
//};
//
//};
