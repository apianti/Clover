///
/// @file Include/Uefi/Protocol/DevicePath.h
///
/// UEFI implementation universal graphics adapter draw protocol
///

#pragma once
#ifndef ___UEFI_UGA_DRAW_PROTOCOL_HEADER__
#define ___UEFI_UGA_DRAW_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_UGA_DRAW_PROTOCOL_GUID
/// The universal graphics adapter draw protocol unique identifier
#define EFI_UGA_DRAW_PROTOCOL_GUID { 0x982c298b, 0xf4fa, 0x41cb, {0xb8, 0x38, 0x77, 0xaa, 0x68, 0x8f, 0xb8, 0x39} }

// gEfiUgaDrawProtocolGuid
/// The universal graphics adapter draw protocol unique identifier
EXTERN EFI_GUID gEfiUgaDrawProtocolGuid;

// EFI_UGA_BLT_OPERATION
/// The universal graphics adapter protocol block transfer operation
typedef enum EFI_UGA_BLT_OPERATION EFI_UGA_BLT_OPERATION;
enum EFI_UGA_BLT_OPERATION {

  // EfiUgaVideoFill
  /// Write data from the BltBuffer pixel (SourceX, SourceY) directly to every pixel of the video display rectangle (DestinationX, DestinationY)
  ///  (DestinationX + Width, DestinationY + Height). Only one pixel will be used from the BltBuffer. Delta is NOT used
  EfiUgaVideoFill,
  // EfiUgaVideoToBltBuffer
  /// Read data from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) and place it in the BltBuffer rectangle (DestinationX, DestinationY )
  ///  (DestinationX + Width, DestinationY + Height). If DestinationX or DestinationY is not zero then Delta must be set to the length in bytes of a row in the BltBuffer
  EfiUgaVideoToBltBuffer,
  // EfiUgaBltBufferToVideo
  /// Write data from the BltBuffer rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) directly to the video display rectangle (DestinationX, DestinationY)
  ///  (DestinationX + Width, DestinationY + Height). If SourceX or SourceY is not zero then Delta must be set to the length in bytes of a row in the BltBuffer
  EfiUgaBltBufferToVideo,
  // EfiUgaVideoToVideo
  /// Copy from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) to the video display rectangle (X, Y) (X + Width, Y + Height).
  ///  The BltBuffer and Delta are not used in this mode. There is no limitation on the overlapping of the source and destination rectangles
  EfiUgaVideoToVideo,
  // EfiUgaBltMax
  /// All UGA Blt operations are below this value
  EfiUgaBltMax

};

// EFI_UGA_PIXEL
/// The universal graphics adapter protocol pixel
typedef struct EFI_UGA_PIXEL EFI_UGA_PIXEL;
struct EFI_UGA_PIXEL {

  // Blue
  /// The blue color component
  UINT8 Blue;
  // Green
  /// The green color component
  UINT8 Green;
  // Red
  /// The red color component
  UINT8 Red;
  // Reserved
  /// The reserved color component
  UINT8 Reserved;

};

// EFI_UGA_PIXEL_UNION
/// The universal graphics adapter protocol pixel
typedef union EFI_UGA_PIXEL_UNION EFI_UGA_PIXEL_UNION;
union EFI_UGA_PIXEL_UNION {

  // Pixel
  /// Pixel components
  EFI_UGA_PIXEL Pixel;
  // Raw
  /// Raw 32bit pixel
  UINT32        Raw;

};

// EFI_UGA_DRAW_PROTOCOL
/// The universal graphics adapter draw protocol
typedef struct EFI_UGA_DRAW_PROTOCOL EFI_UGA_DRAW_PROTOCOL;

// EFI_UGA_DRAW_PROTOCOL_GET_MODE
/// Return the current frame buffer geometry and display refresh rate
/// @param This                 The universal graphics adapter protocol interface
/// @param HorizontalResolution On output, the video horizontal resolution in pixels
/// @param VerticalResolution   On output, the video vertical resolution in pixels
/// @param ColorDepth           On output, the video color depth in bits per pixel
/// @param RefreshRate          On output, the video refresh rate in Hertz
/// @retval EFI_SUCCESS           Valid mode information was returned
/// @retval EFI_NOT_STARTED       A hardware error occurred trying to retrieve the video mode
/// @retval EFI_INVALID_PARAMETER HorizontalResolution, VerticalResolution, or RefreshRate is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_UGA_DRAW_PROTOCOL_GET_MODE) (
  IN  EFI_UGA_DRAW_PROTOCOL *This,
  OUT UINT32                *HorizontalResolution,
  OUT UINT32                *VerticalResolution,
  OUT UINT32                *ColorDepth,
  OUT UINT32                *RefreshRate
);

// EFI_UGA_DRAW_PROTOCOL_SET_MODE
/// Set the video device into the specified mode and clears the output display to black
/// @param This                 The universal graphics adapter protocol interface
/// @param HorizontalResolution The video horizontal resolution in pixels
/// @param VerticalResolution   The video vertical resolution in pixels
/// @param ColorDepth           The video color depth in bits per pixel
/// @param RefreshRate          The video refresh rate in Hertz
/// @retval EFI_SUCCESS      Graphics mode was changed
/// @retval EFI_DEVICE_ERROR The device had an error and could not complete the request
/// @retval EFI_UNSUPPORTED  HorizontalResolution, VerticalResolution, or RefreshRate is not supported
typedef
EFI_STATUS
(EFIAPI
*EFI_UGA_DRAW_PROTOCOL_SET_MODE) (
  IN EFI_UGA_DRAW_PROTOCOL *This,
  IN UINT32                 HorizontalResolution,
  IN UINT32                 VerticalResolution,
  IN UINT32                 ColorDepth,
  IN UINT32                 RefreshRate
);

// EFI_UGA_DRAW_PROTOCOL_BLT
/// Block transfer a rectangle of pixels on the graphics screen
/// @param This                 The universal graphics adapter protocol interface
/// @param BltBuffer    The data to transfer to the graphics screen. Size is at least Width * Height * sizeof(EFI_UGA_PIXEL)
/// @param BltOperation The operation to perform when copying BltBuffer on to the graphics screen
/// @param SourceX      The X coordinate of the source for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param SourceY      The Y coordinate of the source for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param DestinationX The X coordinate of the destination for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param DestinationY The Y coordinate of the destination for the BltOperation. The origin of the screen is 0, 0 and that is the upper left-hand corner of the screen
/// @param Width        The width of a rectangle in the blt rectangle in pixels
/// @param Height       The height of a rectangle in the blt rectangle in pixels
/// @param Delta        Not used for EfiUgaVideoFill or the EfiUgaVideoToVideo operation. If a Delta of zero is used, the entire BltBuffer is being operated on.
///                      If a subrectangle of the BltBuffer is being used then Delta represents the number of bytes in a row of the BltBuffer
/// @retval EFI_SUCCESS           BltBuffer was drawn to the graphics screen
/// @retval EFI_INVALID_PARAMETER BltOperation is not valid
/// @retval EFI_DEVICE_ERROR      The device had an error and could not complete the request
typedef
EFI_STATUS
(EFIAPI
*EFI_UGA_DRAW_PROTOCOL_BLT) (
  IN  EFI_UGA_DRAW_PROTOCOL *This,
  IN  EFI_UGA_PIXEL         *BltBuffer, OPTIONAL
  IN  EFI_UGA_BLT_OPERATION  BltOperation,
  IN  UINTN                  SourceX,
  IN  UINTN                  SourceY,
  IN  UINTN                  DestinationX,
  IN  UINTN                  DestinationY,
  IN  UINTN                  Width,
  IN  UINTN                  Height,
  IN  UINTN                  Delta         OPTIONAL
);

// EFI_UGA_DRAW_PROTOCOL
/// The universal graphics adapter draw protocol
struct EFI_UGA_DRAW_PROTOCOL {

  // GetMode
  /// Returns information about the geometry and configuration of the graphics controller’s current frame buffer configuration
  EFI_UGA_DRAW_PROTOCOL_GET_MODE GetMode;
  // SetMode
  /// Set the graphics device into a given mode and clears the frame buffer to black
  EFI_UGA_DRAW_PROTOCOL_SET_MODE SetMode;
  // Blt
  /// Draw on the video device’s frame buffer
  EFI_UGA_DRAW_PROTOCOL_BLT      Blt;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_UGA_DRAW_PROTOCOL_HEADER__
