///
/// @file Library/Uefi/Memory.c
///
/// UEFI implementation memory management
///

#include "VirtualMemory.h"

// mMemoryTypes
/// Memory type information
STATIC CONST CHAR16 *mMemoryTypes[] = {
  // EfiReservedMemoryType
  L"Reserved",
  // EfiLoaderCode
  L"Loader_Code",
  // EfiLoaderData
  L"Loader_Data",
  // EfiBootServicesCode
  L"BS_Code",
  // EfiBootServicesData
  L"BS_Data",
  // EfiRuntimeServicesCode
  L"RT_Code",
  // EfiRuntimeServicesData
  L"RT_Data",
  // EfiConventionalMemory
  L"Available",
  // EfiUnusableMemory
  L"Unusable",
  // EfiACPIReclaimMemory
  L"ACPI_Reclaim",
  // EfiACPIMemoryNVS
  L"ACPI_NVS",
  // EfiMemoryMappedIO
  L"MMIO",
  // EfiMemoryMappedIOPortSpace
  L"MMIO_Port",
  // EfiPalCode
  L"Pal_Code",
  // EfiPersistentMemory
  L"Persistent"
};

// EfiGetMemoryTypeDescription
/// Get the memory type description
/// @param Type The memory type
/// @return The memory type description or NULL if not found
CONST CHAR16 *
EFIAPI
EfiGetMemoryTypeDescription (
  IN EFI_MEMORY_TYPE Type
) {
  if (Type >= ARRAY_COUNT(mMemoryTypes)) {
    return NULL;
  }
  return mMemoryTypes[Type];
}
// EfiPrintMemoryMap
/// Print the memory map descriptor information to the log
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  The size in bytes of the memory map descriptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
VOID
EFIAPI
EfiPrintMemoryMap (
  IN EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize
) {
  EFI_MEMORY_DESCRIPTOR *Descriptor = NULL;
  UINTN                  Index = 0;
  // Check parameters
  if ((MemoryMap != NULL) && (MemoryMapSize != 0) && (DescriptorSize != 0) && (MemoryMapSize >= DescriptorSize)) {
    // Traverse the memory map
    LOG(L"Memory map has %u regions, map size is 0x%x bytes, region descriptor size is 0x%x bytes:\n", (MemoryMapSize / DescriptorSize), MemoryMapSize, DescriptorSize);
    LOGDIV();
    LOG(L"#   | Region type  | Page count         | Start address     -End address        | Virtual address    | Attributes\n");
    LOG(L"====|==============|====================|=======================================|====================|===================\n");
    while (!EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, MemoryMapSize, DescriptorSize, &Descriptor))) {
      // Get the memory type description
      CONST CHAR16 *Description = EfiGetMemoryTypeDescription(Descriptor->Type);
      // Log each descriptor
      if (Description != NULL) {
        LOG(L"%-3u | %-12s | 0x%016lx | 0x%016lx-0x%016lx | 0x%016lx | 0x%016lx\n", Index, Description, Descriptor->NumberOfPages,
            Descriptor->PhysicalStart, Descriptor->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor->NumberOfPages) - 1, Descriptor->VirtualStart, Descriptor->Attribute);
      } else {
        LOG(L"%-3u | 0x%08lx   | 0x%016lx | 0x%016x-0x%016lx | 0x%016lx | 0x%016lx\n", Index, Descriptor->Type, Descriptor->NumberOfPages,
            Descriptor->PhysicalStart, Descriptor->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor->NumberOfPages) - 1, Descriptor->VirtualStart, Descriptor->Attribute);
      }
      ++Index;
    }
    LOGDIV();
  }
}
// EfiPrintCurrentMemoryMap
/// Print the current memory map descriptor information to the log
VOID
EFIAPI
EfiPrintCurrentMemoryMap (
  VOID
) {
  STATIC EFI_MEMORY_DESCRIPTOR MemoryMap[EFI_MEMORY_MAP_DESCRIPTOR_COUNT];
  UINTN                        MapKey = 0;
  UINTN                        MemoryMapSize = sizeof(MemoryMap);
  UINTN                        DescriptorSize = 0;
  UINT32                       DescriptorVersion = 0;
  if (!EFI_ERROR(EfiGetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion))) {
    EfiPrintMemoryMap(MemoryMap, MemoryMapSize, DescriptorSize);
  }
}

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
) {
  STATIC EFI_MEMORY_DESCRIPTOR  MemoryMap[EFI_MEMORY_MAP_DESCRIPTOR_COUNT];
  EFI_STATUS                    Status;
  EFI_PHYSICAL_ADDRESS          Maximum;
  EFI_PHYSICAL_ADDRESS          Highest = NULL;
  EFI_MEMORY_DESCRIPTOR        *Descriptor = NULL;
  UINTN                         Size = sizeof(MemoryMap);
  UINTN                         MapKey = 0;
  UINTN                         DescSize = 0;
  UINT32                        DescVersion = 0;
  // Check parameters
  if ((Memory == NULL) || (Pages == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the maximum address but only allocate memory below the 4GB as some firmwares may use mixed IA32/X64 architecture,
  //  in this case allocating above a maximum UINT32 can result in incorrect behavior especially within firmware methods
  Maximum = (*Memory != NULL) ? *Memory : 
#if defined(EFI_MEMORY_VIRTUAL_FIRMWARE_SAFE)
  (EFI_PHYSICAL_ADDRESS)(~(UINT32)0);
#else
  (EFI_PHYSICAL_ADDRESS)(~(UINTN)0);
#endif
  // Get memory map
  Status = EfiGetMemoryMap(&Size, MemoryMap, &MapKey, &DescSize, &DescVersion);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check descriptor size
  if (DescSize == 0) {
    DescSize = sizeof(EFI_MEMORY_DESCRIPTOR);
  }
  // Check map size
  if ((Size == 0) || ((Size % DescSize) != 0)) {
    return EFI_BAD_BUFFER_SIZE;
  }
  // Find highest available region in memory map
  while (!EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, Size, DescSize, &Descriptor))) {
    // Find free memory region that has enough pages and is higher than the current highest address
    if ((Descriptor->Type == EfiConventionalMemory) && (Descriptor->PhysicalStart > Highest) && (Descriptor->NumberOfPages >= Pages)) {
      // Check region is below maximum address
      EFI_PHYSICAL_ADDRESS Address = Descriptor->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor->NumberOfPages - Pages);
      if ((Address + EFI_PAGES_TO_SIZE(Pages)) <= Maximum) {
        // Adjust the highest address to be the highest part of this free region
        Highest = Address;
      }
    }
  }
  // Check highest memory address
  if (Highest == NULL) {
    return EFI_NOT_FOUND;
  }
  // Return highest available region address and status code
  *Memory = Highest;
  return EFI_SUCCESS;
}
// EfiDebugAllocateHighestPages
/// Allocates memory from the system in the highest available region
/// @param MemoryType The type of memory to allocate
/// @param Pages      The number of contiguous 4 KiB pages to allocate
/// @param Memory     On output, the address of the base of the page range that was allocated
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
/// @retval EFI_NOT_FOUND         The requested pages could not be found
EFI_STATUS
EFIAPI
EfiDebugAllocateHighestPages (
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 Pages,
  OUT EFI_PHYSICAL_ADDRESS *Memory,
  IN  CONST CHAR8          *Source,
  IN  UINTN                 LineNumber
) {
  EFI_STATUS             Status;
  EFI_PHYSICAL_ADDRESS   HighestMemory = NULL;
  // Check parameters
  if ((Memory == NULL) || (Pages == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find the highest memory region
  Status = EfiFindHighestPages(Pages, &HighestMemory);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Allocate the highest memory region
  Status = EfiDebugAllocatePages(AllocateAddress, MemoryType, Pages, &HighestMemory, Source, LineNumber);
  if (!EFI_ERROR(Status)) {
    // Set the return address
    *Memory = HighestMemory;
  }
  return Status;
}
// EfiDebugAllocatePages
/// Allocates memory pages from the system
/// @param Type       The type of allocation to perform
/// @param MemoryType The type of memory to allocate
/// @param Pages      The number of contiguous 4 KiB pages to allocate
/// @param Memory     On input, the way in which the address is used depends on the value of Type.
///                    On output, the address of the base of the page range that was allocated
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER Type is not AllocateAnyPages or AllocateMaxAddress or AllocateAddress
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
/// @retval EFI_NOT_FOUND         The requested pages could not be found
EFI_STATUS
EFIAPI
EfiDebugAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory,
  IN     CONST CHAR8          *Source,
  IN     UINTN                 LineNumber
) {
#if defined(EFI_MEMORY_VIRTUAL)
  return EfiVirtualAllocatePages(Type, MemoryType, Pages, Memory, gEfiImageHandle, Source, LineNumber);
#else
  UNUSED_PARAMETER(Source);
  UNUSED_PARAMETER(LineNumber);
  if ((gEfiBootServices == NULL) || (gEfiBootServices->AllocatePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->AllocatePages(Type, MemoryType, Pages, Memory);
#endif
}
// EfiFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
EFI_STATUS
EFIAPI
EfiFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
) {
#if defined(EFI_MEMORY_VIRTUAL)
  return EfiVirtualFreePages(Memory, Pages);
#else
  if ((gEfiBootServices == NULL) || (gEfiBootServices->FreePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->FreePages(Memory, Pages);
#endif
}

// EfiAllocatePool
/// Allocates pool memory
/// @param PoolType   The type of pool memory to allocate
/// @param Size       The size in bytes to allocate
/// @param Buffer     On output, the allocated pool memory
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
EFI_STATUS
EFIAPI
EfiDebugAllocatePool (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer,
  IN  CONST CHAR8      *Source,
  IN  UINTN             LineNumber
) {
#if defined(EFI_MEMORY_VIRTUAL)
  // Allocate the memory zeroed from the virtual pool
  return EfiVirtualAllocatePool(PoolType, Size, Buffer, gEfiImageHandle, Source, LineNumber);
#else
  EFI_STATUS Status;
  UNUSED_PARAMETER(Source);
  UNUSED_PARAMETER(LineNumber);
  // Check memory boot services
  if ((gEfiBootServices == NULL) || (gEfiBootServices->AllocatePool == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Allocate the memory from boot services
  Status = gEfiBootServices->AllocatePool(PoolType, Size, Buffer);
  // Zero the pool allocation memory
  if (!EFI_ERROR(Status)) {
    EfiZeroMem(*Buffer, Size);
  }
  return Status;
#endif
}
// EfiAllocate
/// Allocates from the default pool memory
/// @param Size       The size in bytes to allocate
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
VOID *
EFIAPI
EfiDebugAllocate (
  IN UINTN        Size,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
) {
  VOID *Buffer = NULL;
  // Allocate pool memory
  if (!EFI_ERROR(EfiDebugAllocatePool(EFI_MEMORY_TYPE_DEFAULT_POOL, Size, &Buffer, Source, LineNumber))) {
    return Buffer;
  }
  return NULL;
}
// EfiDebugReallocatePool
/// Reallocates pool memory
/// @param PoolType   The type of pool memory to allocate
/// @param Size       The size in bytes to allocate
/// @param OldSize    The size in bytes of the previous allocation to reallocate
/// @param Buffer     On input, the previously allocated memory or NULL, on output, the allocated pool memory
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
EFI_STATUS
EFIAPI
EfiDebugReallocatePool (
  IN     EFI_MEMORY_TYPE   PoolType,
  IN     UINTN             Size,
  IN     UINTN             OldSize,
  IN OUT VOID            **Buffer,
  IN     CONST CHAR8      *Source,
  IN     UINTN             LineNumber
) {
  EFI_STATUS  Status;
  VOID       *Result = NULL;
  if ((Size == 0) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate new buffer
  Status = EfiDebugAllocatePool(PoolType, Size, &Result, Source, LineNumber);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Result == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Copy and free old buffer if needed
  if (*Buffer != NULL) {
    // Copy buffer if needed
    if (Size < OldSize) {
      EfiCopyMem(Result, *Buffer, Size);
    } else if (OldSize != 0) {
      EfiCopyMem(Result, *Buffer, OldSize);
    }
    // Free the old buffer
    EfiFreePool(*Buffer);
  }
  // Set the newly allocated buffer
  *Buffer = Result;
  return EFI_SUCCESS;
}
// EfiDebugReallocate
/// Reallocates from the default pool memory
/// @param Size       The size in bytes to allocate
/// @param OldSize    The size in bytes of the previous allocation to reallocate
/// @param Buffer     The previously allocated memory or NULL
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
VOID *
EFIAPI
EfiDebugReallocate (
  IN UINTN        Size,
  IN UINTN        OldSize,
  IN CONST VOID  *Buffer OPTIONAL,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
) {
  if (EFI_ERROR(EfiDebugReallocatePool(EFI_MEMORY_TYPE_DEFAULT_POOL, Size, OldSize, (VOID **)&Buffer, Source, LineNumber))) {
    return NULL;
  }
  return (VOID *)Buffer;
}
// EfiDebugDuplicate
/// Duplicates memory from the default pool memory
/// @param Size       The size in bytes to duplicate
/// @param Buffer     The memory to duplicate
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
VOID *
EFIAPI
EfiDebugDuplicate (
  IN UINTN        Size,
  IN CONST VOID  *Buffer,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
) {
  VOID *Duplicate;
  if ((Size == 0) || (Buffer == NULL)) {
    return NULL;
  }
  Duplicate = EfiDebugAllocate(Size, Source, LineNumber);
  if (Duplicate != NULL) {
    EfiCopyMem(Duplicate, Buffer, Size);
  }
  return Duplicate;
}
// EfiFreePool
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EFI_STATUS
EFIAPI
EfiFreePool (
  IN VOID *Buffer
) {
#if defined(EFI_MEMORY_VIRTUAL)
  return EfiVirtualFreePool(Buffer);
#else
  if ((gEfiBootServices == NULL) || (gEfiBootServices->FreePool == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->FreePool(Buffer);
#endif
}

// EfiGetMemoryMap
/// Returns the current memory map
/// @param MemoryMapSize     On input, the size in bytes of the memory descriptors buffer, on output, the size in bytes of the memory map descriptors
/// @param MemoryMap         On output, the current memory map descriptors
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @retval EFI_SUCCESS           The memory map was returned in the MemoryMap buffer
/// @retval EFI_BUFFER_TOO_SMALL  The MemoryMap buffer was too small, the current buffer size needed to hold the memory map is returned in MemoryMapSize
/// @retval EFI_INVALID_PARAMETER MemoryMapSize is NULL
/// @retval EFI_INVALID_PARAMETER The MemoryMap buffer is not too small and MemoryMap is NULL
EFI_STATUS
EFIAPI
EfiGetMemoryMap (
  IN OUT UINTN                 *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
  OUT    UINTN                 *MapKey,
  OUT    UINTN                 *DescriptorSize,
  OUT    UINT32                *DescriptorVersion
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->GetMemoryMap == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->GetMemoryMap(MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
}
// EfiGetMemoryMapBuffer
/// Allocates a buffer and returns the current memory map
/// @param MemoryMapSize     On output, the size in bytes of the memory map descriptors
/// @param MemoryMap         On output, the current memory map descriptors which must be freed
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @retval EFI_SUCCESS           The memory map was returned in the MemoryMap buffer
/// @retval EFI_BUFFER_TOO_SMALL  The MemoryMap buffer was too small, the current buffer size needed to hold the memory map is returned in MemoryMapSize
/// @retval EFI_INVALID_PARAMETER MemoryMapSize is NULL
/// @retval EFI_INVALID_PARAMETER The MemoryMap buffer is not too small and MemoryMap is NULL
EFI_STATUS
EFIAPI
EfiGetMemoryMapBuffer (
  OUT UINTN                  *MemoryMapSize,
  OUT EFI_MEMORY_DESCRIPTOR **MemoryMap,
  OUT UINTN                  *MapKey,
  OUT UINTN                  *DescriptorSize,
  OUT UINT32                 *DescriptorVersion
) {
  EFI_MEMORY_DESCRIPTOR *Map;
  EFI_STATUS             Status;
  UINTN                  Size = 0;
  // Get the memory map size
  Status = EfiGetMemoryMap(&Size, NULL, MapKey, DescriptorSize, DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    if ((Size == 0) || (*DescriptorSize == 0)) {
      return EFI_NOT_FOUND;
    }
    // Allocate the memory map
    Map = (EFI_MEMORY_DESCRIPTOR *)EfiAllocate(Size);
    if (Map == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Get the memory map
    Status = EfiGetMemoryMap(&Size, Map, MapKey, DescriptorSize, DescriptorVersion);
    if (EFI_ERROR(Status)) {
      // Free the allocated memory map
      EfiFreePool(Map);
    } else {
      // Return the allocated memory map
      *MemoryMap = Map;
      *MemoryMapSize = Size;
    }
  }
  return Status;
}
// EfiAllocateMemoryMap
/// Allocates a buffer and returns the current memory map
/// @param MemoryMapSize     On output, the size in bytes of the memory map descriptors
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @return The current memory map descriptors which was allocated and must be freed or NULL
EFI_MEMORY_DESCRIPTOR *
EFIAPI
EfiAllocateMemoryMap (
  OUT UINTN  *MemoryMapSize,
  OUT UINTN  *MapKey,
  OUT UINTN  *DescriptorSize,
  OUT UINT32 *DescriptorVersion
) {
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  // Allocate the memory map buffer
  EFI_STATUS Status = EfiGetMemoryMapBuffer(MemoryMapSize, &MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
  if (EFI_ERROR(Status)) {
    return NULL;
  }
  return MemoryMap;
}
// EfiGetNextMemoryDescriptor
/// Gets the next memory descriptor
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  The size in bytes of the memory map descriptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
/// @param Descriptor     On input, the current descriptor or NULL to start from beginning, on output, the next descriptor
/// @retval EFI_INVALID_PARAMETER If MemoryMap or Descriptor is NULL
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize or DescriptorSize is zero
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize < DescriptorSize
/// @retval EFI_NOT_FOUND         There were no more descriptors in the memory map
/// @retval EFI_SUCCESS           The next descriptor was returned in *Descriptor
EFI_STATUS
EFIAPI
EfiGetNextMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  IN     UINTN                   MemoryMapSize,
  IN     UINTN                   DescriptorSize,
  IN OUT EFI_MEMORY_DESCRIPTOR **Descriptor
) {
  UINT8 *Memory;
  UINT8 *This;
  // Check parameters
  if ((MemoryMap == NULL) || (Descriptor == NULL) || (MemoryMapSize == 0) ||
      (DescriptorSize == 0) || (MemoryMapSize < DescriptorSize)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the memory map base
  Memory = (UINT8 *)MemoryMap;
  // Get the current descriptor
  This = (UINT8 *)(*Descriptor);
  // Check if starting from beginning
  if (This == NULL) {
    *Descriptor = MemoryMap;
    return EFI_SUCCESS;
  }
  // Check the current descriptor is within bounds of memory map
  if (This < Memory) {
    return EFI_NOT_FOUND;
  }
  // Decrease the memory memory map size by one descriptor
  MemoryMapSize -= DescriptorSize;
  // Check the next descriptor is within bounds of memory map
  if (This >= (Memory + MemoryMapSize)) {
    return EFI_NOT_FOUND;
  }
  // Return the next descriptor
  *Descriptor = (EFI_MEMORY_DESCRIPTOR *)(This + DescriptorSize);
  return EFI_SUCCESS;
}
// EfiGetPreviousMemoryDescriptor
/// Gets the previous memory descriptor
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  The size in bytes of the memory map descriptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
/// @param Descriptor     On input, the current descriptor or NULL to start from end, on output, the previous descriptor
/// @retval EFI_INVALID_PARAMETER If MemoryMap or Descriptor is NULL
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize or DescriptorSize is zero
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize < DescriptorSize
/// @retval EFI_NOT_FOUND         There were no more descriptors in the memory map
/// @retval EFI_SUCCESS           The previous descriptor was returned in *Descriptor
EFI_STATUS
EFIAPI
EfiGetPreviousMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  IN     UINTN                   MemoryMapSize,
  IN     UINTN                   DescriptorSize,
  IN OUT EFI_MEMORY_DESCRIPTOR **Descriptor
) {
  UINT8 *Memory;
  UINT8 *This;
  // Check parameters
  if ((MemoryMap == NULL) || (Descriptor == NULL) || (MemoryMapSize == 0) ||
      (DescriptorSize == 0) || (MemoryMapSize < DescriptorSize)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the memory map base
  Memory = (UINT8 *)MemoryMap;
  // Get the current descriptor
  This = (UINT8 *)(*Descriptor);
  if (This == NULL) {
    *Descriptor = (EFI_MEMORY_DESCRIPTOR *)(Memory + (MemoryMapSize - DescriptorSize));
    return EFI_SUCCESS;
  }
  // Check the current descriptor is within bounds of memory map
  if (This < (Memory + DescriptorSize)) {
    return EFI_NOT_FOUND;
  }
  // Decrease the map size by one descriptor
  MemoryMapSize -= DescriptorSize;
  // Check the next descriptor is within bounds of memory map
  if (This > (Memory + MemoryMapSize)) {
    return EFI_NOT_FOUND;
  }
  // Return the previous descriptor
  *Descriptor = (EFI_MEMORY_DESCRIPTOR *)(This - DescriptorSize);
  return EFI_SUCCESS;
}
// EfiRemoveMemoryDescriptor
/// Remove a memory descriptor
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  On input, the size in bytes of the memory map descriptors, On output, the new size in bytes of the memory map desciptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
/// @param Descriptor     The descriptor to remove
/// @retval EFI_INVALID_PARAMETER If MemoryMap or Descriptor is NULL
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize or DescriptorSize is zero
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize < DescriptorSize
/// @retval EFI_NOT_FOUND         The descriptor was not in the memory map
/// @retval EFI_SUCCESS           The descriptor was removed and the new memory map size in bytes was placed in *MemoryMapSize
EFI_STATUS
EFIAPI
EfiRemoveMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN OUT UINTN                 *MemoryMapSize,
  IN     UINTN                  DescriptorSize,
  IN     EFI_MEMORY_DESCRIPTOR *Descriptor
) {
  UINTN  Size;
  UINTN  MapSize;
  UINT8 *Memory;
  UINT8 *This;
  // Check parameters
  if ((MemoryMap == NULL) || (MemoryMapSize == NULL) || (Descriptor == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get map size
  MapSize = *MemoryMapSize;
  // Check map size
  if ((MapSize == 0) || (DescriptorSize == 0) || (MapSize < DescriptorSize)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the memory map base
  Memory = (UINT8 *)MemoryMap;
  // Get the current descriptor
  This = (UINT8 *)Descriptor;
  // Check the current descriptor is within bounds of memory map
  if (This < Memory) {
    return EFI_NOT_FOUND;
  }
  // Decrease the map size by one descriptor
  MapSize -= DescriptorSize;
  // Check the next descriptor is within bounds of memory map
  if (This > (Memory + MapSize)) {
    return EFI_NOT_FOUND;
  }
  // Get the size of the memory map below the current descriptor
  Size = (UINTN)(This - Memory);
  // Get the size that needs to be copied
  Size = MapSize - Size;
  if (Size != 0) {
    // Remove The descriptor
    EfiCopyMem((VOID *)This, (VOID *)(This + DescriptorSize), Size);
  }
  // Return new memory map size and success
  *MemoryMapSize = MapSize;
  return EFI_SUCCESS;
}
// EfiByteSwapMemoryDescriptor
/// Swap memory descriptors
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  The size in bytes of the memory map descriptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
/// @param Descriptor1    The first descriptor to swap
/// @param Descriptor2    The second descriptor to swap
/// @retval EFI_INVALID_PARAMETER If MemoryMap or Descriptor1 or Descriptor2 is NULL
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize or DescriptorSize is zero
/// @retval EFI_INVALID_PARAMETER If MemoryMapSize < DescriptorSize
/// @retval EFI_NOT_FOUND         The descriptors were not in the memory map
/// @retval EFI_SUCCESS           The descriptors were swapped
EFI_STATUS
EFIAPI
EfiByteSwapMemoryDescriptor (
  IN EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN EFI_MEMORY_DESCRIPTOR *Descriptor1,
  IN EFI_MEMORY_DESCRIPTOR *Descriptor2
) {
  UINT8                 *Memory;
  UINT8                 *This1;
  UINT8                 *This2;
  // Use two descriptors for temp storage to prevent larger descriptor size from overflowing buffer
  EFI_MEMORY_DESCRIPTOR  Temp[2];
  // Check parameters
  if ((MemoryMap == NULL) || (Descriptor1 == NULL) || (Descriptor2 == NULL) ||
      (MemoryMapSize == 0) || (DescriptorSize == 0) || (MemoryMapSize < DescriptorSize)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the memory map base
  Memory = (UINT8 *)MemoryMap;
  // Get the descriptors
  This1 = (UINT8 *)Descriptor1;
  This2 = (UINT8 *)Descriptor2;
  // Check the current descriptor is within bounds of memory map
  if ((This1 < Memory) || (This2 < Memory)) {
    return EFI_NOT_FOUND;
  }
  // Check the next descriptor is within bounds of memory map
  Memory += MemoryMapSize;
  if ((This1 >= Memory) || (This2 >= Memory)) {
    return EFI_NOT_FOUND;
  }
  // Swap these two descriptors to sort
  EfiCopyMem((VOID *)Temp, (VOID *)This2, DescriptorSize);
  EfiCopyMem((VOID *)This2, (VOID *)This1, DescriptorSize);
  EfiCopyMem((VOID *)This1, (VOID *)Temp, DescriptorSize);
  return EFI_SUCCESS;
}

// EfiCopyMem
/// Copy the contents of one buffer to another buffer
/// @param Destination The destination buffer of the memory copy
/// @param Source      The source buffer of the memory copy
/// @param Length      The size in bytes to copy from Source to Destination
VOID
EFIAPI
EfiCopyMem (
  IN VOID       *Destination,
  IN CONST VOID *Source,
  IN UINTN       Length
) {
  if ((gEfiBootServices != NULL) &&
      (gEfiBootServices->Hdr.Revision >= EFI_1_10_SYSTEM_TABLE_REVISION) &&
      (gEfiBootServices->CopyMem != NULL) && (Length != 0)) {
    gEfiBootServices->CopyMem(Destination, (VOID *)Source, Length);
  }
}
// EfiSetMem
/// Fill a buffer with a specified value
/// @param Buffer The buffer to fill
/// @param Size   The size in bytes of the Buffer to fill
/// @param Value  The value with which to fill Buffer
VOID
EFIAPI
EfiSetMem (
  IN VOID  *Buffer,
  IN UINTN  Size,
  IN UINT8  Value
) {
  if ((gEfiBootServices != NULL) &&
      (gEfiBootServices->Hdr.Revision >= EFI_1_10_SYSTEM_TABLE_REVISION) &&
      (gEfiBootServices->SetMem != NULL) &&
      (Buffer != NULL) && (Size != 0)) {
    gEfiBootServices->SetMem(Buffer, Size, Value);
  }
}
// EfiZeroMem
/// Zero a buffer
/// @param Buffer The buffer to zero
/// @param Size   The size in bytes of the buffer
VOID
EFIAPI
EfiZeroMem (
  IN VOID  *Buffer,
  IN UINTN  Size
) {
  EfiSetMem(Buffer, Size, 0);
}
