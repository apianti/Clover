///
/// @file Include/Gui/Graphics.h
///
/// Graphics engine
///

#pragma once
#ifndef __GUI_GRAPHICS_HEADER__
#define __GUI_GRAPHICS_HEADER__

#include <Gui/Object.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus


// GUI_GRAPHICS_TYPE
/// Graphics screen type
typedef enum GUI_GRAPHICS_TYPE GUI_GRAPHICS_TYPE;
enum GUI_GRAPHICS_TYPE {

  // GuiGraphicsTypeClone
  /// Adapter outputs draw the same output image
  GuiGraphicsTypeClone = 0,
  // GuiGraphicsTypeExtend
  /// Adapter outputs draw a portion of the total output image
  GuiGraphicsTypeExtend,
  // GuiGraphicsTypeFirstOnly
  /// Only the first or default adapter output draws the output image
  GuiGraphicsTypeFirstOnly,

};
// GUI_GRAPHICS_DISPLAY
/// Graphics display
typedef struct GUI_GRAPHICS_DISPLAY GUI_GRAPHICS_DISPLAY;
// GUI_GRAPHICS
/// Graphics engine
typedef struct GUI_GRAPHICS GUI_GRAPHICS;


// GuiGraphicsStart
/// Start the graphics engine by enumerating the graphics adapters and initializing graphics displays
/// @param Graphics On output, the graphics engine
/// @retval EFI_INVALID_PARAMETER If Graphics is NULL or *Graphics is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If there was not enough resources to initialize the graphics engine
/// @retval EFI_SUCCESS           The graphics engine was started successfully
EXTERN
EFI_STATUS
EFIAPI
GuiGraphicsStart (
  OUT GUI_GRAPHICS **Graphics
);
// GuiGraphicsFinish
/// Finish the graphics engine
/// @param Graphics The graphics engine
/// @retval EFI_INVALID_PARAMETER If Graphics is NULL
/// @retval EFI_SUCCESS           The graphics engine was finished and can no longer be used
EXTERN
EFI_STATUS
EFIAPI
GuiGraphicsFinish (
  IN GUI_GRAPHICS *Graphics
);

// 

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __GUI_GRAPHICS_HEADER__
