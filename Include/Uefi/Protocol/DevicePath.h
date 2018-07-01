///
/// @file Include/Uefi/Protocol/DevicePath.h
///
/// UEFI implementation device path protocol
///

#pragma once
#ifndef ___UEFI_DEVICE_PATH_PROTOCOL_HEADER__
#define ___UEFI_DEVICE_PATH_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_DEVICE_PATH_PROTOCOL_GUID
/// Device path protocol unique identifier
#define EFI_DEVICE_PATH_PROTOCOL_GUID { 0x09576E91, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

#define HARDWARE_DEVICE_PATH        0x01  
#define ACPI_DEVICE_PATH            0x02  
#define MESSAGING_DEVICE_PATH       0x03
#define MEDIA_DEVICE_PATH           0x04
#define BBS_DEVICE_PATH             0x05
#define END_DEVICE_PATH_TYPE        0x7f

#define  HW_PCI_DP                  0x01  
#define  HW_PCCARD_DP               0x02
#define  HW_MEMMAP_DP               0x03
#define  HW_VENDOR_DP               0x04
#define  HW_CONTROLLER_DP           0x05

#define  ACPI_DP                    0x01  
#define  ACPI_EXTENDED_DP           0x02
#define  ACPI_ADR_DP                0x03

#define  MSG_ATAPI_DP               0x01
#define  MSG_SCSI_DP                0x02
#define  MSG_FIBRECHANNEL_DP        0x03
#define  MSG_1394_DP                0x04
#define  MSG_USB_DP                 0x05
#define  MSG_I2O_DP                 0x06
#define  MSG_INFINIBAND_DP          0x09
#define  MSG_VENDOR_DP              0x0a
#define  MSG_MAC_ADDR_DP            0x0b
#define  MSG_IPv4_DP                0x0c
#define  MSG_IPv6_DP                0x0d
#define  MSG_UART_DP                0x0e
#define  MSG_USB_CLASS_DP           0x0f
#define  MSG_USB_WWID_DP            0x10
#define  MSG_DEVICE_LOGICAL_UNIT_DP 0x11
#define  MSG_SATA_DP                0x12
#define  MSG_ISCSI_DP               0x13

#define  MEDIA_HARDDRIVE_DP         0x01
#define  MEDIA_CDROM_DP             0x02
#define  MEDIA_VENDOR_DP            0x03
#define  MEDIA_FILEPATH_DP          0x04
#define  MEDIA_PROTOCOL_DP          0x05

#define  BBS_BBS_DP                 0x01

#define END_ENTIRE_DEVICE_PATH_SUBTYPE   0xFF
#define END_INSTANCE_DEVICE_PATH_SUBTYPE 0x01

// gEfiDevicePathProtocolGuid
/// Device path protocol unique identifier
EXTERN EFI_GUID gEfiDevicePathProtocolGuid;

#pragma pack(1)

// EFI_DEVICE_PATH_PROTOCOL
/// Device path protocol
typedef struct EFI_DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH_PROTOCOL;
struct EFI_DEVICE_PATH_PROTOCOL {

  // Type
  /// The device path part type
  UINT8 Type;
  // SubType
  /// The device path part sub type which depends on the type
  UINT8 SubType;
  // Length
  /// The size in bytes of this part of the device path
  UINT8 Length[2];

};

#pragma pack()

// EfiDevicePathFromHandle
/// Get the device path from a handle
/// @param Handle The handle for which to get the device path
/// @return The device path or NULL if there is no device path for the handle or the device path could not be allocated
EXTERN
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
EfiDevicePathFromHandle (
  IN EFI_HANDLE Handle
);
// EfiDevicePathTextFromHandle
/// Get the device path text from a handle
/// @param Handle         The handle for which to get the device path text
/// @param DisplayOnly    Indicates the shorter text representation of the display node can be used instead of the longer text representation, where applicable
/// @param AllowShortcuts Indicates the shortcut forms of text representation for a device node can be used, where applicable
/// @return The device path text or NULL if there is no device path for the handle or the device path text could not be allocated
EXTERN
CHAR16 *
EFIAPI
EfiDevicePathTextFromHandle (
  IN EFI_HANDLE Handle,
  IN BOOLEAN    DisplayOnly,
  IN BOOLEAN    AllowShortcuts
);
// EfiDevicePathTextFromHandle
/// Get the device path text from a handle
/// @param Handle The handle for which to get the device path text
/// @return The device path text or NULL if there is no device path for the handle or the device path text could not be allocated
EXTERN
CHAR16 *
EFIAPI
EfiDevicePathFullTextFromHandle (
  IN EFI_HANDLE Handle
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_DEVICE_PATH_PROTOCOL_HEADER__
