///
/// @file Include/Uefi/System.h
///
/// UEFI implementation system table
///

#pragma once
#ifndef ___UEFI_SYSTEM_TABLE_HEADER__
#define ___UEFI_SYSTEM_TABLE_HEADER__

#include <Uefi/Base.h>
#include <Uefi/Boot.h>
#include <Uefi/Lock.h>
#include <Uefi/Queue.h>
#include <Uefi/Runtime.h>
#include <Uefi/Intrinsics.h>

#include <Uefi/Protocol/SimpleTextOutput.h>
#include <Uefi/Protocol/SimpleTextInput.h>
#include <Uefi/Protocol/SimpleTextInputEx.h>
#include <Uefi/Protocol/SimplePointer.h>
#include <Uefi/Protocol/AbsolutePointer.h>
#include <Uefi/Protocol/GraphicsOutput.h>
#include <Uefi/Protocol/UgaDraw.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_SYSTEM_TABLE_REVISION
/// The UEFI specification version to which the system table complies
#define EFI_SYSTEM_TABLE_REVISION EFI_SPECIFICATION_VERSION

// EFI_SYSTEM_TABLE_SIGNATURE
/// The UEFI system table signature
#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249

// EFI_SYSTEM_TABLE
/// Contains pointers to the runtime and boot services tables
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
struct EFI_SYSTEM_TABLE {

 // Hdr
 /// The table header
 EFI_TABLE_HEADER                 Hdr;
 // FirmwareVendor
 /// The Unicode string that identifies the vendor that produces the system firmware for the platform
 CHAR16                          *FirmwareVendor;
 // FirmwareRevision
 /// The revision of the system firmware for the platform
 UINT32                           FirmwareRevision;
 // ConsoleInHandle
 /// The handle for the active console input device
 EFI_HANDLE                       ConsoleInHandle;
 // ConIn
 /// The simple text input interface associated with ConsoleInHandle
 EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
 // ConsoleOutHandle
 /// The handle for the active console output device
 EFI_HANDLE                       ConsoleOutHandle;
 // ConOut
 /// The simple text output interface associated with ConsoleOutHandle
 EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
 // StandardErrorHandle
 /// The handle for the active standard error console device
 EFI_HANDLE                       StandardErrorHandle;
 // StdErr
 /// The simple text output interface associated with StandardErrorHandle
 EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
 // RuntimeServices
 /// The runtime services table
 EFI_RUNTIME_SERVICES            *RuntimeServices;
 // BootServices
 /// The boot services table
 EFI_BOOT_SERVICES               *BootServices;
 // NumberOfTableEntries
 /// The number of system configuration tables
 UINTN                            NumberOfTableEntries;
 // ConfigurationTable
 /// The system configuration tables
 EFI_CONFIGURATION_TABLE         *ConfigurationTable;

};

// EFI_IMAGE_ENTRY_POINT
/// The main entry point for a UEFI image, the same for applications and drivers
/// @param ImageHandle The firmware allocated handle for the UEFI image
/// @param SystemTable The UEFI System Table
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_ENTRY_POINT) (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_SYSTEM_TABLE_HEADER__
