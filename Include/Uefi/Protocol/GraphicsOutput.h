///
/// @file Include/Uefi/Protocol/GraphicsOutput.h
///
/// UEFI implementation graphics output protocol
///

#pragma once
#ifndef ___UEFI_GRAPHICS_OUTPUT_PROTOCOL_HEADER__
#define ___UEFI_GRAPHICS_OUTPUT_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
/// Graphics output protocol unique identifier
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID { 0x9042A9DE, 0x23DC, 0x4A38, { 0x96, 0xFB, 0x7A, 0xDE, 0xD0, 0x80, 0x51, 0x6A } }

// gEfiGraphicsOutputProtocolGuid
/// Graphics output protocol unique identifier
EXTERN EFI_GUID gEfiGraphicsOutputProtocolGuid;

// EFI_GRAPHICS_PIXEL_FORMAT
/// Graphics output pixel format
typedef enum EFI_GRAPHICS_PIXEL_FORMAT EFI_GRAPHICS_PIXEL_FORMAT;
enum EFI_GRAPHICS_PIXEL_FORMAT {

  // PixelRedGreenBlueReserved8BitPerColor
  /// A pixel is 32-bits and byte zero represents red, byte one represents green, byte two represents
  ///  blue, and byte three is reserved. This is the definition for the physical frame buffer. The
  ///  byte values for the red, green, and blue components represent the color intensity. This
  ///  color intensity value range from a minimum intensity of 0 to maximum intensity of 255
  PixelRedGreenBlueReserved8BitPerColor,
  // PixelBlueGreenRedReserved8BitPerColor
  /// A pixel is 32-bits and byte zero represents blue, byte one represents green, byte two represents
  ///  red, and byte three is reserved. This is the definition for the physical frame buffer. The
  ///  byte values for the red, green, and blue components represent the color intensity. This
  ///  color intensity value range from a minimum intensity of 0 to maximum intensity of 255
  PixelBlueGreenRedReserved8BitPerColor,
  // PixelBitMask
  /// The pixel definition of the physical frame buffer is defined by a pixel mask
  PixelBitMask,
  // PixelBltOnly
  /// This mode does not support a physical frame buffer
  PixelBltOnly,
  // PixelFormatMax
  /// Valid pixel formats are less than this value
  PixelFormatMax

};

// EFI_PIXEL_BITMASK
/// Pixel color bitmask
typedef struct EFI_PIXEL_BITMASK EFI_PIXEL_BITMASK;
struct EFI_PIXEL_BITMASK {

  // RedMask
  /// The bits that represent the red component
  UINT32 RedMask;
  // GreenMask
  /// The bits that represent the green component
  UINT32 GreenMask;
  // BlueMask
  /// The bits that represent the blue component
  UINT32 BlueMask;
  // ReservedMask
  /// The bits that represent the reserved component
  UINT32 ReservedMask;

};

// EFI_GRAPHICS_OUTPUT_MODE_INFORMATION
/// Graphics output mode information
typedef struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;
struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {

  // Version
  /// The version of the graphics output mode information
  UINT32                    Version;
  // HorizontalResolution
  /// The size of video screen in pixels in the X dimension
  UINT32                    HorizontalResolution;
  // VerticalResolution
  /// The size of video screen in pixels in the Y dimension
  UINT32                    VerticalResolution;
  // PixelFormat
  /// The physical format of the pixel, PixelBltOnly implies that frame buffer is not available
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
  // PixelInformation
  /// The pixel color bitmask information
  EFI_PIXEL_BITMASK         PixelInformation;
  // PixelsPerScanLine
  /// Defines the number of pixel elements per video memory line. For performance reasons, or due
  ///  to hardware restrictions, scan lines may be padded to an amount of memory alignment. These
  ///  padding pixel elements are outside the area covered by HorizontalResolution and are not visible.
  ///  For direct frame buffer access, this number is used as a span between starts of pixel lines in
  ///  video memory. Based on the size of an individual pixel element and PixelsPerScanline, the offset
  ///  in video memory from pixel element (x, y) to pixel element (x, y+1) has to be calculated as
  ///  "sizeof( PixelElement ) * PixelsPerScanLine", not "sizeof( PixelElement ) * HorizontalResolution",
  ///  though in many cases those values can coincide. This value can depend on video hardware and mode
  ///  resolution. GOP implementation is responsible for providing accurate value for this field
  UINT32                    PixelsPerScanLine;

};

// EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE
/// Graphics output mode
typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {

  // MaxMode
  /// The maximum count of modes supported by the graphics output protocol interface
  UINT32                                MaxMode;
  // Mode
  /// Current mode of the graphics device
  UINT32                                Mode;
  // Info
  /// The graphics output mode information
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  // SizeOfInfo
  /// Size in bytes of the graphics output mode information
  UINTN                                 SizeOfInfo;
  // FrameBufferBase
  /// Base address of graphics linear frame buffer, offset zero in FrameBufferBase represents the top left pixel of the display
  EFI_PHYSICAL_ADDRESS                  FrameBufferBase;
  // FrameBufferSize
  /// The size in bytes of the graphics linear frame buffer
  UINTN                                 FrameBufferSize;

};

// EFI_GRAPHICS_OUTPUT_BLT_PIXEL
/// Blt pixel
typedef struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL EFI_GRAPHICS_OUTPUT_BLT_PIXEL;
struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL {

  // Blue
  /// The blue component
  UINT8 Blue;
  // Green
  /// The green component
  UINT8 Green;
  // Red
  /// The red component
  UINT8 Red;
  // Reserved
  /// The reserved component
  UINT8 Reserved;

};

// EFI_GRAPHICS_OUTPUT_BLT_OPERATION
typedef enum EFI_GRAPHICS_OUTPUT_BLT_OPERATION EFI_GRAPHICS_OUTPUT_BLT_OPERATION;
enum EFI_GRAPHICS_OUTPUT_BLT_OPERATION {

  // EfiBltVideoFill
  /// Write data from the BltBuffer pixel (0,0) directly to every pixel of the video display rectangle (DestinationX, DestinationY)
  ///  (DestinationX + Width, DestinationY + Height). Only one pixel will be used from the BltBuffer. Delta is NOT used
  EfiBltVideoFill,
  // EfiBltVideoToBltBuffer
  /// Read data from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) and place it in
  ///  the BltBuffer rectangle (DestinationX, DestinationY ) (DestinationX + Width, DestinationY + Height). If DestinationX
  ///  or DestinationY is not zero then Delta must be set to the length in bytes of a row in the BltBuffer
  EfiBltVideoToBltBuffer,
  // EfiBltBufferToVideo
  /// Write data from the BltBuffer rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) directly to the video
  ///  display rectangle (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height). If SourceX or
  ///  SourceY is not zero then Delta must be set to the length in bytes of a row in the BltBuffer
  EfiBltBufferToVideo,
  // EfiBltVideoToVideo
  /// Copy from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) to the video display
  ///  rectangle(DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height. The BltBuffer and Delta
  ///  are not used in this mode. There is no limitation on the overlapping of the source and destination rectangles
  EfiBltVideoToVideo,
  // EfiGraphicsOutputBltOperationMax
  /// Valid blt operations are below this value
  EfiGraphicsOutputBltOperationMax

};

// EFI_GRAPHICS_OUTPUT_PROTOCOL
/// Graphics output protocol
typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

// EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE
/// Returns information for an available graphics mode that the graphics device and the set of active video output devices supports
/// @param This       The graphics output protocol interface
/// @param ModeNumber The mode number for which to return information
/// @param SizeOfInfo On output, the size, in bytes, of the mode information
/// @param Info       On output, the mode information which must be freed
/// @retval EFI_SUCCESS           Valid mode information was returned
/// @retval EFI_DEVICE_ERROR      A hardware error occurred trying to retrieve the video mode
/// @retval EFI_INVALID_PARAMETER ModeNumber is not valid
typedef
EFI_STATUS
(EFIAPI
*EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
);

// EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE
/// Set the video device into the specified mode and clears the visible portions of the output display to black
/// @param This       The graphics output protocol interface
/// @param ModeNumber The graphics mode to set
/// @retval EFI_SUCCESS      The graphics mode specified by ModeNumber was selected
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  ModeNumber is not supported by this device
typedef
EFI_STATUS
(EFIAPI
*EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN UINT32                        ModeNumber
);

// EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT
/// Blt a rectangle of pixels on the graphics screen. Blt stands for BLock Transfer.
/// @param This         The graphics output protocol interface
/// @param BltBuffer    The data to transfer to the graphics screen. Size is at least Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
/// @param BltOperation The operation to perform when copying BltBuffer on to the graphics screen
/// @param SourceX      The X coordinate of the source for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param SourceY      The Y coordinate of the source for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param DestinationX The X coordinate of the destination for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param DestinationY The Y coordinate of the destination for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param Width        The width of a rectangle in the blt rectangle in pixels. Each pixel is represented by an EFI_GRAPHICS_OUTPUT_BLT_PIXEL element
/// @param Height       The height of a rectangle in the blt rectangle in pixels. Each pixel is represented by an EFI_GRAPHICS_OUTPUT_BLT_PIXEL element
/// @param Delta        Not used for EfiBltVideoFill or the EfiBltVideoToVideo operation. If a Delta of zero is used, the entire BltBuffer is being operated on.
///                      If a subrectangle of the BltBuffer is being used then Delta represents the number of bytes in a row of the BltBuffer
/// @retval EFI_SUCCESS           BltBuffer was drawn to the graphics screen
/// @retval EFI_INVALID_PARAMETER BltOperation is not valid
/// @retval EFI_DEVICE_ERROR      The device had an error and could not complete the request
typedef
EFI_STATUS
(EFIAPI
*EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
  IN     EFI_GRAPHICS_OUTPUT_PROTOCOL      *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer OPTIONAL,
  IN     EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN     UINTN                              SourceX,
  IN     UINTN                              SourceY,
  IN     UINTN                              DestinationX,
  IN     UINTN                              DestinationY,
  IN     UINTN                              Width,
  IN     UINTN                              Height,
  IN     UINTN                              Delta OPTIONAL
);

// EFI_GRAPHICS_OUTPUT_PROTOCOL
/// Graphics output protocol
struct EFI_GRAPHICS_OUTPUT_PROTOCOL {

  // QueryMode
  /// Returns information for an available graphics mode that the graphics device and the set of active video output devices supports
  EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE  QueryMode;
  // SetMode
  /// Set the video device into the specified mode and clears the visible portions of the output display to black
  EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE    SetMode;
  // Blt
  /// Software abstraction to draw on the video device’s frame buffer
  EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT         Blt;
  // Mode
  /// The graphics output mode
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE       *Mode;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_GRAPHICS_OUTPUT_PROTOCOL_HEADER__
