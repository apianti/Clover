///
/// @file Include/Uefi/Defaults.h
///
/// UEFI implementation defaults
///

#pragma once
#ifndef __UEFI_DEFAULTS__
#define __UEFI_DEFAULTS__

#include <Uefi/Version.h>

//
// Architecture defaults
//

// Check if architecture is already defined
#if !defined(EFI_ARCH_X64) && !defined(EFI_ARCH_IA32) && !defined(EFI_ARCH_IA64) && !defined(EFI_ARCH_ARM) && !defined(EFI_ARCH_AA64)
// Determine the architecture
# if defined(_MSC_VER)
// Get the architecture for MSVC
#  if defined(_M_AMD64) || defined(_M_X64)
#   define EFI_ARCH_X64
#  elif defined(_M_IX86)
#   define EFI_ARCH_IA32
#  elif defined(_M_IA64)
#   define EFI_ARCH_IA64
#  elif defined(_M_ARM)
#   define EFI_ARCH_ARM
#  elif defined(_M_ARM64)
#   define EFI_ARCH_AA64
#  endif
# elif defined(__GNUC__)
// Get the architecture for GCC
#  if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#   define EFI_ARCH_X64
#  elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#   define EFI_ARCH_IA32
#  elif defined(__ia64__) || defined(_IA64) || defined(__IA64__)
#   define EFI_ARCH_IA64
#  elif defined(__arm__) || defined(__thumb__)
#   define EFI_ARCH_ARM
#  elif defined(__aarch64__)
#   define EFI_ARCH_AA64
#  endif
# endif
// Check architecture is defined
# if !defined(EFI_ARCH_X64) && !defined(EFI_ARCH_IA32) && !defined(EFI_ARCH_IA64) && !defined(EFI_ARCH_ARM) && !defined(EFI_ARCH_AA64)
#  error Invalid architecture specified or unsupported architecture detected
# endif
#endif

//
// Firmware defaults
//

// EFI_DEFAULT_BOOT_PATH
/// The default path for boot file in a file system
#if !defined(EFI_DEFAULT_BOOT_PATH)
# define EFI_DEFAULT_BOOT_PATH L"\\EFI\\BOOT\\BOOT" EFI_PROJECT_ARCH L".efi"
#endif

//
// Library defaults
//

// EFI_PROJECT_NAME
/// The default project name
#if !defined(EFI_PROJECT_NAME)
# define EFI_PROJECT_NAME L"GUI"
#endif
// EFI_PROJECT_MAJOR_VERSION
/// The default project major version
#if !defined(EFI_PROJECT_MAJOR_VERSION)
# define EFI_PROJECT_MAJOR_VERSION 0
#endif
// EFI_PROJECT_MINOR_VERSION
/// The default project minor version
#if !defined(EFI_PROJECT_MINOR_VERSION)
# define EFI_PROJECT_MINOR_VERSION 1
#endif

//
// Memory defaults
//

// EFI_MEMORY_TYPE_DEFAULT_POOL
/// The default pool allocation type
#if !defined(EFI_MEMORY_TYPE_DEFAULT_POOL)
# define EFI_MEMORY_TYPE_DEFAULT_POOL EfiBootServicesData
#endif
// EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT
/// The default virtual allocation pool page count
#if !defined(EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT)
# define EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT 1
#endif
// EFI_MEMORY_PRINT_BUFFER_SIZE
/// The count of characters in the temporary print buffer for MemPrint
#if !defined(EFI_MEMORY_PRINT_BUFFER_SIZE)
# define EFI_MEMORY_PRINT_BUFFER_SIZE 256
#endif
// EFI_MEMORY_RECORD_PREVIEW_LENGTH
/// The size in length of the memory record preview string
#if !defined(EFI_MEMORY_RECORD_PREVIEW_LENGTH)
# define EFI_MEMORY_RECORD_PREVIEW_LENGTH 32
#endif
// EFI_MEMORY_PRINT_PREVIEW_SIZE
/// The size in bytes of the memory record preview data
#if !defined(EFI_MEMORY_PRINT_PREVIEW_SIZE)
# define EFI_MEMORY_PRINT_PREVIEW_SIZE 32
#endif
// EFI_MEMORY_MAP_DESCRIPTOR_COUNT
/// The count of descriptors in the temporary memory map
#if !defined(EFI_MEMORY_MAP_DESCRIPTOR_COUNT)
# define EFI_MEMORY_MAP_DESCRIPTOR_COUNT 256
#endif
// EFI_MEMORY_MAP_DESCRIPTOR_PAD_COUNT
/// The count of descriptors to pad memory map sizes
#if !defined(EFI_MEMORY_MAP_DESCRIPTOR_PAD_COUNT)
# define EFI_MEMORY_MAP_DESCRIPTOR_PAD_COUNT 8
#endif

//
// Encoding defaults
//

// EFI_ENCODING_DEFAULT_NAME
/// Default encoding (Latin-1/ASCII) in Latin-1/ASCII encoding
#if !defined(EFI_ENCODING_DEFAULT_NAME)
# define EFI_ENCODING_DEFAULT_NAME "Latin-1"
#endif
// EFI_ENCODING_VARIABLE_NAME
/// The current default encoding variable name in UCS-2 encoding
#if !defined(EFI_ENCODING_VARIABLE_NAME)
# define EFI_ENCODING_VARIABLE_NAME L"PlatformEncoding"
#endif

#endif // __UEFI_DEFAULTS__
