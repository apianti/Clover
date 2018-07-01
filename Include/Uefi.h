///
/// @file Include/Uefi.h
///
/// UEFI implementation
///

#pragma once
#ifndef ___UEFI_HEADER__
#define ___UEFI_HEADER__

#include <Uefi/System.h>

#include <Uefi/Protocol/DevicePathToText.h>
#include <Uefi/Protocol/DevicePathFromText.h>
#include <Uefi/Protocol/DevicePathUtilities.h>
#include <Uefi/Protocol/File.h>
#include <Uefi/Protocol/LoadedImage.h>
#include <Uefi/Protocol/SerialIo.h>
#include <Uefi/Protocol/SimpleFileSystem.h>
#include <Uefi/Protocol/UnicodeCollation.h>

#include <Uefi/Language.h>
#include <Uefi/String.h>
#include <Uefi/Protocol/Locale.h>
#include <Uefi/Protocol/Encoding.h>
#include <Uefi/Print.h>
#include <Uefi/Protocol/Log.h>
#include <Uefi/Protocol/Configuration.h>
#include <Uefi/Protocol/Translation.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// gEfiImageHandle
/// The image handle for the current execution image
EXTERN EFI_HANDLE gEfiImageHandle;
// gEfiSystemTable
/// The system services table
EXTERN EFI_SYSTEM_TABLE *gEfiSystemTable;
// gEfiBootServices
/// The boot services table
EXTERN EFI_BOOT_SERVICES *gEfiBootServices;
// gEfiRuntimeServices
/// The runtime services table
EXTERN EFI_RUNTIME_SERVICES *gEfiRuntimeServices;
// gEfiConIn
/// The default console input
EXTERN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *gEfiConIn;
// gEfiConOut
/// The default console output
EXTERN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *gEfiConOut;

// EfiEntryPoint
/// UEFI entry point for execution
/// @return Whether the execution failed to start, fatally aborted or completed successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEntryPoint (
  VOID
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_HEADER__
