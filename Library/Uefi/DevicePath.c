///
/// @file Include/Uefi/Protocol/DevicePathFromText.h
///
/// UEFI implementation device path
///

#include <Uefi.h>

// gEfiDevicePathProtocolGuid
/// Device path protocol unique identifier
EFI_GUID gEfiDevicePathProtocolGuid = EFI_DEVICE_PATH_PROTOCOL_GUID;
// gEfiDevicePathToTextProtocolGuid
/// Device path to text protocol unique identifier
EFI_GUID gEfiDevicePathToTextProtocolGuid = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
// gEfiDevicePathFromTextProtocolGuid
/// Device path from text protocol unique identifier
EFI_GUID gEfiDevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
// gEfiDevicePathUtilitiesProtocolGuid
/// Device path utilities protocol unique identifier
EFI_GUID gEfiDevicePathUtilitiesProtocolGuid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;

// mDevicePathToText
/// The device path to text protocol interface
STATIC EFI_DEVICE_PATH_TO_TEXT_PROTOCOL   *mDevicePathToText;
// mDevicePathFromText
/// The device path from text protocol interface
STATIC EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *mDevicePathFromText;
// mDevicePathUtilities
/// The device path utilities protocol interface
STATIC EFI_DEVICE_PATH_UTILITIES_PROTOCOL *mDevicePathUtilities;

// EfiConvertTextToDeviceNode
/// Convert text to the binary representation of a device node
/// @param TextDeviceNode The text representation of a device node, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device node which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiConvertTextToDeviceNode (
  IN CONST CHAR16 *TextDeviceNode
) {
  // Check for device path from text protocol interface
  if (mDevicePathFromText == NULL) {
    // Locate device path from text protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathFromTextProtocolGuid, NULL, (VOID **)&mDevicePathFromText))) {
      return NULL;
    }
    // Check device path from text protocol interface is valid
    if (mDevicePathFromText == NULL) {
      return NULL;
    }
  }
  // Check device node from text method is valid
  if (mDevicePathFromText->ConvertTextToDeviceNode == NULL) {
    return NULL;
  }
  // Convert the text to device node
  return mDevicePathFromText->ConvertTextToDeviceNode(TextDeviceNode);
}
// EfiConvertTextToDevicePath
/// Convert a text to its binary device path representation
/// @param TextDevicePath The text representation of a device path, conversion starts with the first character and continues until the first non-device node character
/// @return The allocated device path which must be freed or NULL if TextDeviceNode is NULL or there was insufficient memory
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiConvertTextToDevicePath (
  IN CONST CHAR16 *TextDevicePath
) {
  // Check for device path from text protocol interface
  if (mDevicePathFromText == NULL) {
    // Locate device path from text protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathFromTextProtocolGuid, NULL, (VOID **)&mDevicePathFromText))) {
      return NULL;
    }
    // Check device path from text protocol interface is valid
    if (mDevicePathFromText == NULL) {
      return NULL;
    }
  }
  // Check device path from text method is valid
  if (mDevicePathFromText->ConvertTextToDeviceNode == NULL) {
    return NULL;
  }
  // Convert the text to device node
  return mDevicePathFromText->ConvertTextToDevicePath(TextDevicePath);
}

// EfiConvertDeviceNodeToText
/// Convert a device node to its text representation
/// @param DeviceNode     The device node to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
CHAR16 *
EFIAPI
EfiConvertDeviceNodeToText (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
) {
  // Check for device path to text protocol interface
  if (mDevicePathToText == NULL) {
    // Locate device path to text protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (VOID **)&mDevicePathToText))) {
      return NULL;
    }
    // Check device path to text protocol interface is valid
    if (mDevicePathToText == NULL) {
      return NULL;
    }
  }
  // Check device node to text method is valid
  if (mDevicePathToText->ConvertDeviceNodeToText == NULL) {
    return NULL;
  }
  // Convert the device node to text
  return mDevicePathToText->ConvertDeviceNodeToText(DeviceNode, DisplayOnly, AllowShortcuts);
}

// EfiDevicePathToText
/// Convert a device path to its text representation
/// @param DevicePath     The device path to be converted
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The allocated text representation of the device node data which must be freed or NULL if DeviceNode was NULL or there was insufficient memory
CHAR16 *
EFIAPI
EfiDevicePathToText (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN BOOLEAN                         DisplayOnly,
  IN BOOLEAN                         AllowShortcuts
) {
  // Check for device path to text protocol interface
  if (mDevicePathToText == NULL) {
    // Locate device path to text protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (VOID **)&mDevicePathToText))) {
      return NULL;
    }
    // Check device path to text protocol interface is valid
    if (mDevicePathToText == NULL) {
      return NULL;
    }
  }
  // Check device path to text method is valid
  if (mDevicePathToText->ConvertDevicePathToText == NULL) {
    return NULL;
  }
  // Convert the device node to text
  return mDevicePathToText->ConvertDevicePathToText(DevicePath, DisplayOnly, AllowShortcuts);
}

// EfiGetDevicePathSize
/// Returns the size of the device path, in bytes
/// @param DevicePath The device path of which to get the size
/// @return The size of the device path in bytes
UINTN
EFIAPI
EfiGetDevicePathSize (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return 0;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return 0;
    }
  }
  // Check get device path size method is valid
  if (mDevicePathUtilities->GetDevicePathSize == NULL) {
    return 0;
  }
  // Get the device path size
  return mDevicePathUtilities->GetDevicePathSize(DevicePath);
}

// EfiDuplicateDevicePath
/// Create a duplicate of the specified path
/// @param DevicePath The device path of which to duplicate
/// @return The duplicate device path which must be freed or NULL if the device path could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiDuplicateDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if duplicate device path method is valid
  if (mDevicePathUtilities->DuplicateDevicePath == NULL) {
    return NULL;
  }
  // Get the duplicate device path
  return mDevicePathUtilities->DuplicateDevicePath(DevicePath);
}

// EfiAppendDevicePath
/// Create a new path by appending the second device path to the first
/// @param Src1 The first device path
/// @param Src2 The second device path
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *Src2
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if append device path method is valid
  if (mDevicePathUtilities->AppendDevicePath == NULL) {
    return NULL;
  }
  // Append device path
  return mDevicePathUtilities->AppendDevicePath(Src1, Src2);
}

// EfiAppendDeviceNode
/// Creates a new path by appending the device node to the device path
/// @param DevicePath The device path
/// @param DeviceNode The device node
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDeviceNode (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if append device node method is valid
  if (mDevicePathUtilities->AppendDeviceNode == NULL) {
    return NULL;
  }
  // Append device node
  return mDevicePathUtilities->AppendDeviceNode(DevicePath, DeviceNode);
}

// EfiAppendDevicePathInstance
/// Creates a new path by appending the specified device path instance to the specified device path
/// @param DevicePath         The device path
/// @param DevicePathInstance The device path instance
/// @return The newly created device path which must be freed or NULL if the device path could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiAppendDevicePathInstance (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath OPTIONAL,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if append device path instance method is valid
  if (mDevicePathUtilities->AppendDevicePathInstance == NULL) {
    return NULL;
  }
  // Append device path instance
  return mDevicePathUtilities->AppendDevicePathInstance(DevicePath, DevicePathInstance);
}

// EfiGetNextDevicePathInstance
/// Creates a copy of the current device path instance and returns a pointer to the next device path instance
/// @param DevicePathInstance     On input, the current device path instance, on output, the next device path instance or NULL if there are no more device path instances in the device path
/// @param DevicePathInstanceSize On output, the size in bytes of the created device path instance
/// @return The newly created device path instance which must be freed or NULL if the device path instance could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiGetNextDevicePathInstance (
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePathInstance,
  OUT    UINTN                     *DevicePathInstanceSize OPTIONAL
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface and check device path utilities protocol interface is valid
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      mDevicePathUtilities = NULL;
    }
  }
  // Check if get next device path instance method is valid
  if ((mDevicePathUtilities == NULL) || (mDevicePathUtilities->GetNextDevicePathInstance == NULL)) {
    // Attempt to return correct end case
    if (DevicePathInstance != NULL) {
      *DevicePathInstance = NULL;
    }
    if (DevicePathInstanceSize != NULL) {
      *DevicePathInstanceSize = 0;
    }
    return NULL;
  }
  // Get next device path instance
  return mDevicePathUtilities->GetNextDevicePathInstance(DevicePathInstance, DevicePathInstanceSize);
}

// EfiIsDevicePathMultiInstance
/// Returns whether a device path is multi-instance
/// @param DevicePath The device path to check is multi-instance
/// @return Whether the device path is multi-instance or not
BOOLEAN
EFIAPI
EfiIsDevicePathMultiInstance (
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if is device path multi-instance method is valid
  if (mDevicePathUtilities->IsDevicePathMultiInstance == NULL) {
    return NULL;
  }
  // Is device path multi-instance
  return mDevicePathUtilities->IsDevicePathMultiInstance(DevicePath);
}

// EfiCreateDeviceNode
/// Creates a device node
/// @param NodeType    The device node type for the new device node
/// @param NodeSubType The device node sub-type for the new device node
/// @param NodeLength  The length of the device node for the new device node
/// @return The newly created device node which must be freed or NULL if the device node could not be allocated or NodeLength is less than the size of the device path header
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiCreateDeviceNode (
  IN UINT8  NodeType,
  IN UINT8  NodeSubType,
  IN UINT16 NodeLength
) {
  // Check for device path utilities protocol interface
  if (mDevicePathUtilities == NULL) {
    // Locate device path utilites protocol interface
    if (EFI_ERROR(EfiLocateProtocol(&gEfiDevicePathUtilitiesProtocolGuid, NULL, (VOID **)&mDevicePathUtilities))) {
      return NULL;
    }
    // Check device path utilities protocol interface is valid
    if (mDevicePathUtilities == NULL) {
      return NULL;
    }
  }
  // Check if create device node method is valid
  if (mDevicePathUtilities->CreateDeviceNode == NULL) {
    return NULL;
  }
  // Create device node
  return mDevicePathUtilities->CreateDeviceNode(NodeType, NodeSubType, NodeLength);
}

// EfiDevicePathFromHandle
/// Get the device path from a handle
/// @param Handle The handle for which to get the device path
/// @return The device path or NULL if there is no device path for the handle or the device path could not be allocated
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiDevicePathFromHandle (
  IN EFI_HANDLE Handle
) {
  EFI_DEVICE_PATH_PROTOCOL *DevicePath = NULL;
  // Get the device path protocol interface from the handle
  if (EFI_ERROR(EfiHandleProtocol(Handle, &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath))) {
    // Assume the device path was not found...
    return NULL;
  }
  // Return the device path
  return DevicePath;
}
// EfiDevicePathTextFromHandle
/// Get the device path text from a handle
/// @param Handle The handle for which to get the device path text
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The device path text or NULL if there is no device path for the handle or the device path text could not be allocated
CHAR16 *
EFIAPI
EfiDevicePathTextFromHandle (
  IN EFI_HANDLE Handle,
  IN BOOLEAN    DisplayOnly,
  IN BOOLEAN    AllowShortcuts
) {
  // Get the device path from the handle
  EFI_DEVICE_PATH_PROTOCOL *DevicePath = EfiDevicePathFromHandle(Handle);
  if (DevicePath == NULL) {
    return NULL;
  }
  // Convert device path to text
  return EfiDevicePathToText(DevicePath, DisplayOnly, AllowShortcuts);
}
// EfiDevicePathTextFromHandle
/// Get the device path text from a handle
/// @param Handle The handle for which to get the device path text
/// @return The device path text or NULL if there is no device path for the handle or the device path text could not be allocated
CHAR16 *
EFIAPI
EfiDevicePathFullTextFromHandle (
  IN EFI_HANDLE Handle
) {
  return EfiDevicePathTextFromHandle(Handle, FALSE, FALSE);
}
