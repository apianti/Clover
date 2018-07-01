///
/// @file Library/Uefi/VirtualMemory.c
///
/// UEFI implementation virtual memory management
///

#pragma once
#ifndef __UEFI_VIRTUAL_MEMORY__
#define __UEFI_VIRTUAL_MEMORY__

#include <Uefi.h>

#if defined(EFI_MEMORY_VIRTUAL)

// MemPrint
/// Safely print a formatted character string to outputs
/// @param Outputs The outputs to use
/// @param Format  The format specifier string
/// @param ...     The arguments to format
EXTERN
VOID
MemPrint (
  IN UINT32        Outputs,
  IN CONST CHAR16 *Format,
  ...
);

// EfiVirtualMemoryInstall
/// Install virtual memory services
/// @retval EFI_SUCCESS The virtual memory services were installed
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualMemoryInstall (
  VOID
);
// EfiVirtualMemoryUninstall
/// Uninstall virtual memory services
/// @retval EFI_SUCCESS The virtual memory services were uninstalled
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualMemoryUninstall (
  VOID
);

// EfiGetMemoryTypeDescription
/// Get the memory type description
/// @param Type The memory type
/// @return The memory type description or NULL if not found
EXTERN
CONST CHAR16 *
EFIAPI
EfiGetMemoryTypeDescription (
  IN EFI_MEMORY_TYPE Type
);

// EfiPrintMemoryRecords
/// Print the memory allocation records
EXTERN
VOID
EFIAPI
EfiPrintMemoryRecords (
  VOID
);

// EfiFindHighestPages
/// Find the highest available memory region with the specified page count
/// @param Pages  The count of contiguous 4 KiB pages to find
/// @param Memory On input, the maximum address that the end of the entire region must be equal or below, or NULL for the maximum architecture address, on output, the address of the available region
/// @retval EFI_INVALID_PARAMETER Memory is NULL or Pages is zero
/// @retval EFI_NOT_FOUND         The requested pages could not be found
/// @retval EFI_SUCCESS           The requested pages were found
EFI_STATUS
EFIAPI
EfiFindHighestPages (
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory
);

// EfiVirtualAllocatePages
/// Allocates memory pages from the system
/// @param Type        The type of allocation to perform
/// @param MemoryType  The type of memory to allocate
/// @param Pages       The number of contiguous 4 KiB pages to allocate
/// @param Memory      On input, the way in which the address is used depends on the value of Type.
///                     On output, the address of the base of the page range that was allocated
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER Type is not AllocateAnyPages or AllocateMaxAddress or AllocateAddress
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
/// @retval EFI_NOT_FOUND         The requested pages could not be found
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory,
  IN     EFI_HANDLE            ImageHandle,
  IN     CONST CHAR8          *Source,
  IN     UINTN                 LineNumber
);
// EfiVirtualFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
);
// EfiVirtualAllocatePool
/// Allocates pool memory
/// @param PoolType    The type of pool memory to allocate
/// @param Size        The size in bytes to allocate
/// @param Buffer      On output, the allocated pool memory
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualAllocatePool (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer,
  IN  EFI_HANDLE        ImageHandle,
  IN  CONST CHAR8      *Source,
  IN  UINTN             LineNumber
);
// EfiVirtualFreePool
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EXTERN
EFI_STATUS
EFIAPI
EfiVirtualFreePool (
  IN VOID       *Buffer
);

// EfiInternalAllocatePages
/// Allocates memory pages from the system
/// @param Pages  The number of contiguous 4 KiB pages to allocate
/// @param Memory On input, the way in which the address is used depends on the value of Type.
///                On output, the address of the base of the page range that was allocated
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER Type is not AllocateAnyPages or AllocateMaxAddress or AllocateAddress
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
/// @retval EFI_NOT_FOUND         The requested pages could not be found
EXTERN
EFI_STATUS
EFIAPI
EfiInternalAllocatePages (
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory
);
// EfiInternalFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
EXTERN
EFI_STATUS
EFIAPI
EfiInternalFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
);
// EfiInternalAllocate
/// Add memory pool record to virtual memory map
/// @param Size    The size in bytes of the allocation
/// @return The pool allocation or NULL
EXTERN
VOID *
EFIAPI
EfiInternalAllocate (
  IN  UINTN Size
);
// EfiInternalAllocatePool
/// Add memory pool record to virtual memory map
/// @param Type    The type of memory to allocate
/// @param Size    The size in bytes of the allocation
/// @param Address On output, the address of the memory that was allocated
/// @retval EFI_SUCCESS           The record was added
/// @retval EFI_OUT_OF_RESOURCES  The record could not be allocated
/// @retval EFI_INVALID_PARAMETER Memory is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiInternalAllocatePool (
  IN  EFI_MEMORY_TYPE   Type,
  IN  UINTN             Size,
  OUT VOID            **Address
);
// EfiInternalFreePool
/// Remove memory pool record from virtual memory map
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EXTERN
EFI_STATUS
EFIAPI
EfiInternalFreePool (
  IN VOID *Buffer
);

#endif // EFI_MEMORY_VIRTUAL

#endif // __UEFI_VIRTUAL_MEMORY__
