///
/// @file Library/Uefi/VirtualMemory.c
///
/// UEFI implementation virtual memory management
///

#include "VirtualMemory.h"

#if defined(EFI_MEMORY_VIRTUAL)

// EFI_VIRTUAL_MEMORY_RECORD_FLAG_STATIC
/// The virtual memory record should not be freed when the pool region becomes available
#define EFI_VIRTUAL_MEMORY_RECORD_FLAG_STATIC EFI_BIT(0)

// mEfiVirtualMemoryProtocolGuid
/// Virtual memory protocol unique identifier
STATIC EFI_GUID mEfiVirtualMemoryProtocolGuid = { 0xCA34D2D9, 0xFC18, 0x44BC, { 0xB6, 0x96, 0x1E, 0xB3, 0x3F, 0xB8, 0xC4, 0x8F } };

// EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD
/// Virtual memory available record
typedef struct EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD;
struct EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD {

  // Next
  /// The next available memory record
  EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *Next;
  // Size
  /// The size in bytes of the available memory
  UINTN                                Size;

};
// EFI_VIRTUAL_MEMORY_RECORD
/// Virtual memory record
typedef struct EFI_VIRTUAL_MEMORY_RECORD EFI_VIRTUAL_MEMORY_RECORD;
struct EFI_VIRTUAL_MEMORY_RECORD {

  // Next
  /// The next memory pool record
  EFI_VIRTUAL_MEMORY_RECORD           *Next;
  // Size
  /// The size in bytes allocated for this pool record
  UINTN                                Size;
  // Type
  /// The memory type of this pool record
  EFI_MEMORY_TYPE                      Type;
  // Flags
  /// The virtual memory record flags
  UINT32                               Flags;
  // Available
  /// The available pool memory
  EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *Available;

};

// EFI_VIRTUAL_MEMORY_PAGE_RECORD
/// Virtual memory page allocation record
typedef struct EFI_VIRTUAL_MEMORY_PAGE_RECORD EFI_VIRTUAL_MEMORY_PAGE_RECORD;
struct EFI_VIRTUAL_MEMORY_PAGE_RECORD {

  // Next
  /// The next memory allocation record
  EFI_VIRTUAL_MEMORY_RECORD *Next;
  // Address
  /// The address of the allocation
  EFI_PHYSICAL_ADDRESS       Address;
  // Count
  /// The 4KiB page count of the allocation
  UINTN                      Count;
  // Type
  /// The type of the memory allocation
  UINTN                      Type;
  // ImageHandle
  /// The handle of the image that allocated the memory
  EFI_HANDLE                 ImageHandle;
  // Source
  /// The name of the source where the  memory was allocated
  CONST CHAR8               *Source;
  // LineNumber
  /// The line number of the source where the memory was allocated
  UINTN                      LineNumber;

};
// EFI_VIRTUAL_MEMORY_PAGE_RECORDS
/// Virtual memory page records
typedef struct EFI_VIRTUAL_MEMORY_PAGE_RECORDS EFI_VIRTUAL_MEMORY_PAGE_RECORDS;
struct EFI_VIRTUAL_MEMORY_PAGE_RECORDS {

  // Next
  /// The next virtual memory page records
  EFI_VIRTUAL_MEMORY_PAGE_RECORDS *Next;
  // Count
  /// The count of valid memory page allocation records
  UINTN                            Count;
  // Size
  /// The maximum count of memory page allocation records
  UINTN                            Size;
  // Records
  /// The memory page allocation records
  EFI_VIRTUAL_MEMORY_PAGE_RECORD   Records[1];

};
// EFI_VIRTUAL_MEMORY_POOL_RECORD
/// Virtual memory pool allocation record
typedef struct EFI_VIRTUAL_MEMORY_POOL_RECORD EFI_VIRTUAL_MEMORY_POOL_RECORD;
struct EFI_VIRTUAL_MEMORY_POOL_RECORD {

  // Next
  /// The next memory allocation record
  EFI_VIRTUAL_MEMORY_RECORD *Next;
  // Address
  /// The address of the allocation
  VOID                      *Address;
  // Size
  /// The size in bytes of the allocation
  UINTN                      Size;
  // Type
  /// The type of the memory allocation
  UINTN                      Type;
  // ImageHandle
  /// The handle of the image that allocated the memory
  EFI_HANDLE                 ImageHandle;
  // Source
  /// The name of the source where the  memory was allocated
  CONST CHAR8               *Source;
  // LineNumber
  /// The line number of the source where the memory was allocated
  UINTN                      LineNumber;

};
// EFI_VIRTUAL_MEMORY_POOL_RECORDS
/// Virtual memory pool records
typedef struct EFI_VIRTUAL_MEMORY_POOL_RECORDS EFI_VIRTUAL_MEMORY_POOL_RECORDS;
struct EFI_VIRTUAL_MEMORY_POOL_RECORDS {

  // Next
  /// The next virtual memory pool records
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *Next;
  // Count
  /// The count of valid memory pool allocation records
  UINTN                            Count;
  // Size
  /// The maximum count of memory pool allocation records
  UINTN                            Size;
  // Records
  /// The memory pool allocation records
  EFI_VIRTUAL_MEMORY_POOL_RECORD   Records[1];

};

// EFI_VIRTUAL_MEMORY_PROTOCOL
/// Virtual memory protocol
typedef struct EFI_VIRTUAL_MEMORY_PROTOCOL EFI_VIRTUAL_MEMORY_PROTOCOL;
struct EFI_VIRTUAL_MEMORY_PROTOCOL {

  //
  // These are the original methods from boot services, the boot service methods should be replaced with the stubs below
  //

  // AllocatePages
  /// Allocate memory pages from the firmware memory map
  EFI_ALLOCATE_PAGES AllocatePages;
  // FreePages
  /// Free memory pages allocated from the firmware memory map
  EFI_FREE_PAGES     FreePages;
  // AllocatePool
  /// Allocate pool memory from the firmware memory map
  EFI_ALLOCATE_POOL  AllocatePool;
  // FreePool
  /// Free pool memory from the firmware memory map
  EFI_FREE_POOL      FreePool;
  // GetMemoryMap
  /// Get the firmware memory map
  EFI_GET_MEMORY_MAP GetMemoryMap;

  //
  // This represents the virtual memory map
  //

  // Lock
  /// The virtual memory lock to preserve thread coherency
  EFI_LOCK                         Lock;
  // Records
  /// The virtual memory records
  EFI_VIRTUAL_MEMORY_RECORD       *Records;
  // PageRecords
  /// The virtual memory page records
  EFI_VIRTUAL_MEMORY_PAGE_RECORDS *PageRecords;
  // PoolRecords
  /// The virtual memory pool records
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *PoolRecords;

};

// VMemGetProtocol
/// Get the virtual memory protocol
/// @return The virtual memory protocol or NULL
STATIC
EFI_VIRTUAL_MEMORY_PROTOCOL *
EFIAPI
VMemGetProtocol (
  VOID
) {
  EFI_STATUS Status;
  EFI_VIRTUAL_MEMORY_PROTOCOL *VirtualMemory = NULL;
  // Locate the virtual memory protocol
  Status = EfiLocateProtocol(&mEfiVirtualMemoryProtocolGuid, NULL, (VOID **)&VirtualMemory);
  if (EFI_ERROR(Status)) {
    return NULL;
  }
  return VirtualMemory;
}

// VMemAllocateHighestPages
/// Allocates memory from the system in the highest available region
/// @param This       The virtual memory protocol
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
STATIC
EFI_STATUS
EFIAPI
VMemAllocateHighestPages (
  IN  EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN  EFI_MEMORY_TYPE              MemoryType,
  IN  UINTN                        Pages,
  OUT EFI_PHYSICAL_ADDRESS        *Memory
) {
  EFI_STATUS             Status;
  EFI_PHYSICAL_ADDRESS   HighestMemory = NULL;
  // Check parameters
  if ((This == NULL) || (This->AllocatePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if ((Memory == NULL) || (Pages == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find the highest memory region
  Status = EfiFindHighestPages(Pages, &HighestMemory);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Allocate the highest memory region
  Status = This->AllocatePages(AllocateAddress, MemoryType, Pages, &HighestMemory);
  if (!EFI_ERROR(Status)) {
    // Set the return address
    *Memory = HighestMemory;
  }
  return Status;
}

// VMemAllocatePoolRecord
/// Add memory pool record to virtual memory map
/// @param This        The virtual memory protocol
/// @param Type        The type of memory to allocate
/// @param Size        The size in bytes of the allocation
/// @param Address     On output, the address of the memory that was allocated
/// @retval EFI_SUCCESS           The record was added
/// @retval EFI_OUT_OF_RESOURCES  The record could not be allocated
/// @retval EFI_INVALID_PARAMETER Memory is NULL
STATIC
EFI_STATUS
EFIAPI
VMemAllocatePoolRecord (
  IN  EFI_VIRTUAL_MEMORY_PROTOCOL  *This,
  IN  EFI_MEMORY_TYPE               Type,
  IN  UINTN                         Size,
  OUT VOID                        **Address
) {
  EFI_STATUS                            Status = EFI_NOT_FOUND;
  EFI_VIRTUAL_MEMORY_RECORD            *Records;
  EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD  *Available;
  EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD **Found = NULL;
  UINTN                                 AvailableSize;
  // Check parameters
  if ((This == NULL) || (Address == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Adjust size to be multiple of available record
  AvailableSize = Size % sizeof(EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD);
  if (AvailableSize != 0) {
    AvailableSize = (sizeof(EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD) - AvailableSize);
    Size += AvailableSize;
  }
  // Traverse records for available pool space
  Records = This->Records;
  while (Records != NULL) {
    // Check this is the correct memory type and has available records
    if ((Records->Type == Type) && (Records->Available != NULL)) {
      // Get the first available record
      Available = Records->Available;
      // Check if the first available record has enough space for this allocation
      if (Available->Size >= Size) {
        // This record has enough space
        Found = &(Records->Available);
      } else {
        // Traverse the available pool records
        while (Available->Next != NULL) {
          // Check if this available record has enough space for this allocation
          if (Available->Next->Size >= Size) {
            // This record has enough space
            Found = &(Available->Next);
            break;
          }
          // Get the next available pool record
          Available = Available->Next;
        }
      }
      // Check if any available record was found that had enough space for this allocation
      if (Found != NULL) {
        // Get the available record to use for allocation
        Available = *Found;
        // Get the available size
        AvailableSize = Available->Size - Size;
        // Set allocation address
        *Address = (VOID *)Available;
        // Check if some weird memory offset happened and ignore the extra bytes, also when the record sizes match exactly
        if (AvailableSize < sizeof(EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD)) {
          // Set the next available record
          *Found = Available->Next;
        } else {
          EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *Next = Available->Next;
          // Set the available record to after the allocation
          Available = (EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *)(((UINT8 *)Available) + Size);
          Available->Next = Next;
          Available->Size = AvailableSize;
          // Replace the available record with the new one
          *Found = Available;
        }
        // Allocated the memory
        Status = EFI_SUCCESS;
        break;
      }
    }
    // Get the next pool record
    Records = Records->Next;
  }
  // Check if a pool record needs allocated
  if (Status == EFI_NOT_FOUND) {
    EFI_VIRTUAL_MEMORY_RECORD *Pool;
    EFI_PHYSICAL_ADDRESS       Memory = NULL;
    UINTN                      Count = EFI_SIZE_TO_PAGES(Size + sizeof(EFI_VIRTUAL_MEMORY_RECORD));
    // Allocate pages for the new pool record
    Status = VMemAllocateHighestPages(This, Type, Count, &Memory);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if (Memory == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Setup the pool record
    Pool = (EFI_VIRTUAL_MEMORY_RECORD *)(UINTN)Memory;
    Pool->Flags = 0;
    Pool->Type = Type;
    Pool->Size = EFI_PAGES_TO_SIZE(Count) - sizeof(EFI_VIRTUAL_MEMORY_RECORD);
    // Set the available record if any
    AvailableSize = Pool->Size - Size;
    // Check if some weird memory offset happened and ignore the extra bytes, also when the record sizes match exactly
    if (AvailableSize < sizeof(EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD)) {
      // No remaining memory in this pool
      Pool->Available = NULL;
    } else {
      // Set the available record for the remaining memory in this pool
      Available = (EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *)(UINTN)(Memory + Size + sizeof(EFI_VIRTUAL_MEMORY_RECORD));
      Available->Next = NULL;
      Available->Size = AvailableSize;
      Pool->Available = Available;
    }
    // Set the next pool record
    Pool->Next = This->Records;
    This->Records = Pool;
    // Set the address and return code
    *Address = (VOID *)(UINTN)(Memory + sizeof(EFI_VIRTUAL_MEMORY_RECORD));
    Status = EFI_SUCCESS;
  }
  // Zero the pool allocation memory
  if (!EFI_ERROR(Status)) {
    if ((*Address != NULL) && (Size != 0)) {
      EfiZeroMem(*Address, Size);
    }
  }
  return Status;
}
// VMemFreePool
/// Remove memory pool record from virtual memory map
/// @param This   The virtual memory protocol
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
STATIC
EFI_STATUS
EFIAPI
VMemFreePoolRecord (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN VOID                        *Buffer
) {
  EFI_STATUS                       Status = EFI_NOT_FOUND;
  EFI_VIRTUAL_MEMORY_RECORD       *Record;
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *PoolRecords;
  // Check parameters
  if ((This == NULL) || (This->FreePages == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there are any records
  PoolRecords = This->PoolRecords;
  if (PoolRecords == NULL) {
    return EFI_NOT_FOUND;
  }
  // Traverse the pool of records for the address
  while (PoolRecords != NULL) {
    UINTN Index = 0;
    // Traverse each record in this pool of records
    while (Index < PoolRecords->Count) {
      // Check if the address is in this record
      EFI_VIRTUAL_MEMORY_POOL_RECORD *PoolRecord = PoolRecords->Records + Index;
      if (Buffer == PoolRecord->Address) {
        // Traverse the allocation records
        Record = This->Records;
        while (Record != NULL) {
          // Check if the address has been allocated within this allocation record
          VOID *Ptr = (VOID *)(Record + 1);
          VOID *End = (VOID *)(((UINT8 *)Ptr) + Record->Size);
          if ((Buffer >= Ptr) && (Buffer < End)) {
            // The address was allocated within this record
            EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD **PAvailable;
            EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD  *Available;
            // Search available records for available space directly before or after this now available space
            PAvailable = &(Record->Available);
            while (*PAvailable != NULL) {
              Available = *PAvailable;
              // Check available record is directly before or after this now available space
              if (Buffer == (VOID *)(((UINT8 *)Available) + Available->Size)) {
                // This available space is directly after the available record so just increase the size
                Available->Size += PoolRecord->Size;
                break;
              } else if (End == (VOID *)Available) {
                // This available space is directly before the available record so replace the record
                EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *Replacement;
                Replacement = (EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *)Buffer;
                Replacement->Next = Available->Next;
                Replacement->Size = PoolRecord->Size + Available->Size;
                *PAvailable = Replacement;
                break;
              }
              // Get the next available record
              PAvailable = &(Available->Next);
            }
            // There was no available space directly before or after this now available space so add a new available record
            if (*PAvailable == NULL) {
              Available = (EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *)Buffer;
              // Add this available space back
              Available->Size = PoolRecord->Size;
              Available->Next = Record->Available;
              Record->Available = Available;
            }
            break;
          }
          // Get the next allocation records
          Record = Record->Next;
        }
        // Set success return code, should 
        Status = EFI_SUCCESS;
        break;
      }
      // Increase the index for the next pool record
      ++Index;
    }
    // Get the next pool of records
    PoolRecords = PoolRecords->Next;
  }
  // Cleanup any pool records
  Record = This->Records;
  if (Record != NULL) {
    while (Record->Next != NULL) {
      EFI_VIRTUAL_MEMORY_RECORD *Next = Record->Next;
      // Check if the available space is the same as the entire record
      if ((Next->Available != NULL) && (Next->Size == Next->Available->Size) &&
          EFI_BITS_ARE_UNSET(Next->Flags, EFI_VIRTUAL_MEMORY_RECORD_FLAG_STATIC)) {
        // Remove the record since the space is all available again
        Record->Next = Next->Next;
        Status = gEfiBootServices->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Next, EFI_SIZE_TO_PAGES(Next->Size + sizeof(EFI_VIRTUAL_MEMORY_RECORD)));
      } else {
        // Get the next pool records
        Record = Record->Next;
      }
    }
    // Check if the available space is the same as the entire root record
    Record = This->Records;
    if ((Record != NULL) && (Record->Available != NULL) && (Record->Size == Record->Available->Size) &&
        EFI_BITS_ARE_UNSET(Record->Flags, EFI_VIRTUAL_MEMORY_RECORD_FLAG_STATIC)) {
      // Remove the record since the space is all available again
      This->Records = Record->Next;
      Status = gEfiBootServices->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Record, EFI_SIZE_TO_PAGES(Record->Size + sizeof(EFI_VIRTUAL_MEMORY_RECORD)));
    }
  }
  // Return status code
  return Status;
}
// VMemSetPageRecord
/// Set memory page record information
/// @param This        The virtual memory page record
/// @param Address     The address of the memory that was allocated
/// @param Count       The 4KiB page count of the allocation
/// @param Type        The type of memory to allocate
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The record was set
/// @retval EFI_INVALID_PARAMETER Memory is NULL
STATIC
EFI_STATUS
EFIAPI
VMemSetPageRecord (
  IN EFI_VIRTUAL_MEMORY_PAGE_RECORD *This,
  IN EFI_PHYSICAL_ADDRESS            Address,
  IN UINTN                           Count,
  IN EFI_MEMORY_TYPE                 Type,
  IN EFI_HANDLE                      ImageHandle,
  IN CONST CHAR8                    *Source,
  IN UINTN                           LineNumber
) {
  // Check parameters
  if ((This == NULL) || (Address == NULL) || (Count == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Set page record
  This->Address = Address;
  This->Count = Count;
  This->Type = Type;
  This->ImageHandle = ImageHandle;
  This->Source = Source;
  This->LineNumber = LineNumber;
  return EFI_SUCCESS;
}
// VMemAddPageRecord
/// Add memory page record to virtual memory map
/// @param This        The virtual memory protocol
/// @param Address     The address of the memory that was allocated
/// @param Count       The 4KiB page count of the allocation
/// @param Type        The type of memory to allocate
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The record was added
/// @retval EFI_OUT_OF_RESOURCES  The record could not be allocated
/// @retval EFI_INVALID_PARAMETER Memory is NULL
STATIC
EFI_STATUS
EFIAPI
VMemAddPageRecord (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN EFI_PHYSICAL_ADDRESS         Address,
  IN UINTN                        Count,
  IN EFI_MEMORY_TYPE              Type,
  IN EFI_HANDLE                   ImageHandle,
  IN CONST CHAR8                 *Source,
  IN UINTN                        LineNumber
) {
  EFI_STATUS                       Status = EFI_NOT_FOUND;
  EFI_VIRTUAL_MEMORY_PAGE_RECORDS *Records;
  // Check parameters
  if ((This == NULL) || (Address == NULL) || (Count == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find an empty page allocation record slot
  Records = This->PageRecords;
  while (Records != NULL) {
    // Check if there is an empty allocation record slot
    if (Records->Count < Records->Size) {
      // Set the allocation record information
      Status = VMemSetPageRecord(Records->Records + Records->Count, Address, Count, Type, ImageHandle, Source, LineNumber);
      if (!EFI_ERROR(Status)) {
        ++(Records->Count);
      }
      break;
    }
    // Get the next record
    Records = Records->Next;
  }
  // Check if an empty allocation record slot was found
  if (Status == EFI_NOT_FOUND) {
    EFI_PHYSICAL_ADDRESS Memory = NULL;
    // Allocate pages for the new page record
    Status = VMemAllocateHighestPages(This, Type, EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT, &Memory);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if (Memory == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Setup the page record
    Records = (EFI_VIRTUAL_MEMORY_PAGE_RECORDS *)(UINTN)Memory;
    Records->Count = 1;
    Records->Size = ((EFI_PAGES_TO_SIZE(EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT) - OFFSET_OF(EFI_VIRTUAL_MEMORY_PAGE_RECORDS, Records)) / sizeof(EFI_VIRTUAL_MEMORY_PAGE_RECORD));
    // Set the page allocation record information
    Status = VMemSetPageRecord(Records->Records, Address, Count, Type, ImageHandle, Source, LineNumber);
    // Add the page record
    Records->Next = This->PageRecords;
    This->PageRecords = Records;
  }
  return Status;
}
// VMemRemovePageRecord
/// Remove memory page record from virtual memory map
/// @param This        The virtual memory protocol
/// @param Address     The address of the memory to be freed
/// @param Count       The 4KiB page count being freed
/// @param ImageHandle The handle of the image allocating the memory
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
STATIC
EFI_STATUS
EFIAPI
VMemRemovePageRecord (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN EFI_PHYSICAL_ADDRESS         Address,
  IN UINTN                        Count
) {
  EFI_STATUS                       Status = EFI_NOT_FOUND;
  EFI_VIRTUAL_MEMORY_PAGE_RECORDS *Records;
  EFI_VIRTUAL_MEMORY_PAGE_RECORDS *Next;
  // Check parameters
  if ((This == NULL) || (This->FreePages == NULL) || (Address == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there are any page records;
  if (This->PageRecords == NULL) {
    return EFI_NOT_FOUND;
  }
  // Find the page allocation record
  Records = This->PageRecords;
  while (Records != NULL) {
    UINTN Index = 0;
    // Traverse the memory allocation records in this page record
    while (Index < Records->Count) {
      EFI_VIRTUAL_MEMORY_PAGE_RECORD *Record = Records->Records + Index;
      // Check if the address matches
      if ((Address >= Record->Address) && (Address < (Record->Address + EFI_PAGES_TO_SIZE(Record->Count)))) {
        // Check if the count matches exactly
        if (Record->Count >= Count) {
          // Remove the memory allocation record
          --(Records->Count);
          EfiCopyArray(EFI_VIRTUAL_MEMORY_PAGE_RECORD, Record, Record + 1, Records->Count - Index);
        } else {
          // The memory allocation is either split or shifted
          if (Record->Address == Address) {
            // The allocation is shifted higher
            Record->Address += EFI_PAGES_TO_SIZE(Count);
          } else if ((Record->Address + EFI_PAGES_TO_SIZE(Record->Count - Count)) == Address) {
            // The allocation is shifted lower
            Record->Count -= Count;
          } else {
            // The allocation is split
            UINTN NewCount = EFI_SIZE_TO_PAGES(Address - Record->Address);
            // Add a new record for the other side of the split
            Status = VMemAddPageRecord(This, Address + EFI_PAGES_TO_SIZE(Count), Record->Count - (NewCount + Count), Record->Type, Record->ImageHandle, Record->Source, Record->LineNumber);
            // Adjust the record
            Record->Count = NewCount;
          }
        }
        // Set success status code, record was removed or modified
        Status = EFI_SUCCESS;
        break;
      }
      // Increment the index
      ++Index;
    }
    // Get the next page record
    Records = Records->Next;
  }
  // Remove any page records with zero count
  if (This->PageRecords != NULL) {
    // Traverse the page records
    Records = This->PageRecords;
    while (Records->Next != NULL) {
      // Get the next page record
      Next = Records->Next;
      // If this page record has no allocation records then remove the page record
      if (Next->Count == 0) {
        // Remove the page record
        Records->Next = Next->Next;
        // Free the page record memory
        This->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Next, EFI_SIZE_TO_PAGES(OFFSET_OF(EFI_VIRTUAL_MEMORY_PAGE_RECORDS, Records) + (Records->Size * sizeof(EFI_VIRTUAL_MEMORY_PAGE_RECORD))));
      } else {
        // Continue to the next page record
        Records = Next;
      }
    }
    // Check if the root page record needs removed
    if ((This->PageRecords != NULL) && (This->PageRecords->Count == 0)) {
      // Get the root page record and the next page record
      Records = This->PageRecords;
      Next = Records->Next;
      // Free the page record memory
      This->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Records, EFI_SIZE_TO_PAGES(OFFSET_OF(EFI_VIRTUAL_MEMORY_PAGE_RECORDS, Records) + (Records->Size * sizeof(EFI_VIRTUAL_MEMORY_PAGE_RECORD))));
      // Remove the page record
      This->PageRecords =  Next;
    }
  }
  return Status;
}
// VMemSetPoolRecord
/// Set memory pool record information
/// @param This        The virtual memory pool record
/// @param Address     The address of the memory that was allocated
/// @param Size        The size in bytes of the allocation
/// @param Type        The type of memory to allocate
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The record was set
/// @retval EFI_INVALID_PARAMETER Memory is NULL
STATIC
EFI_STATUS
EFIAPI
VMemSetPoolRecord (
  IN EFI_VIRTUAL_MEMORY_POOL_RECORD *This,
  IN VOID                           *Address,
  IN UINTN                           Size,
  IN EFI_MEMORY_TYPE                 Type,
  IN EFI_HANDLE                      ImageHandle,
  IN CONST CHAR8                    *Source,
  IN UINTN                           LineNumber
) {
  // Check parameters
  if ((This == NULL) || (Address == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Set pool record
  This->Address = Address;
  This->Size = Size;
  This->Type = Type;
  This->ImageHandle = ImageHandle;
  This->Source = Source;
  This->LineNumber = LineNumber;
  return EFI_SUCCESS;
}
// VMemAddPoolRecord
/// Add memory pool record to virtual memory map
/// @param This        The virtual memory protocol
/// @param Address     The address of the memory that was allocated
/// @param Size        The size in bytes of the allocation
/// @param Type        The type of memory to allocate
/// @param ImageHandle The handle of the image allocating the memory
/// @param Source      The source code name where the allocation occurs
/// @param LineNumber  The line number of the source code where the allocation occurs
/// @retval EFI_SUCCESS           The record was added
/// @retval EFI_OUT_OF_RESOURCES  The record could not be allocated
/// @retval EFI_INVALID_PARAMETER Memory is NULL
STATIC
EFI_STATUS
EFIAPI
VMemAddPoolRecord (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN VOID                        *Address,
  IN UINTN                        Size,
  IN EFI_MEMORY_TYPE              Type,
  IN EFI_HANDLE                   ImageHandle,
  IN CONST CHAR8                 *Source,
  IN UINTN                        LineNumber
) {
  EFI_STATUS                       Status = EFI_NOT_FOUND;
  EFI_PHYSICAL_ADDRESS             Memory = NULL;
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *Records;
  // Check parameters
  if ((This == NULL) || (Address == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find an empty pool allocation record slot
  Records = This->PoolRecords;
  while (Records != NULL) {
    // Check if there is an empty allocation record slot
    if (Records->Count < Records->Size) {
      // Set the allocation record information
      Status = VMemSetPoolRecord(Records->Records + Records->Count, Address, Size, Type, ImageHandle, Source, LineNumber);
      if (!EFI_ERROR(Status)) {
        ++(Records->Count);
      }
      break;
    }
    // Get the next record
    Records = Records->Next;
  }
  // Check if an empty allocation record slot was found
  if (Status == EFI_NOT_FOUND) {
    // Allocate pages for the new pool record
    Status = VMemAllocateHighestPages(This, Type, EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT, &Memory);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if (Memory == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Setup the pool record
    Records = (EFI_VIRTUAL_MEMORY_POOL_RECORDS *)(UINTN)Memory;
    Records->Count = 1;
    Records->Size = ((EFI_PAGES_TO_SIZE(EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT) - OFFSET_OF(EFI_VIRTUAL_MEMORY_POOL_RECORDS, Records)) / sizeof(EFI_VIRTUAL_MEMORY_POOL_RECORD));
    // Set the pool allocation record information
    Status = VMemSetPoolRecord(Records->Records, Address, Size, Type, ImageHandle, Source, LineNumber);
    // Add the pool record
    Records->Next = This->PoolRecords;
    This->PoolRecords = Records;
  }
  return Status;
}
// VMemRemovePoolRecord
/// Remove memory pool record from virtual memory map
/// @param This        The virtual memory protocol
/// @param Address     The address of the memory to be freed
/// @param ImageHandle The handle of the image allocating the memory
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
STATIC
EFI_STATUS
EFIAPI
VMemRemovePoolRecord (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN VOID                        *Address
) {
  EFI_STATUS                       Status = EFI_NOT_FOUND;
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *Records;
  EFI_VIRTUAL_MEMORY_POOL_RECORDS *Next;
  // Check parameters
  if ((This == NULL) || (This->FreePages == NULL) || (Address == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there are any pool records;
  if (This->PoolRecords == NULL) {
    return EFI_NOT_FOUND;
  }
  // Find the pool allocation record
  Records = This->PoolRecords;
  while (Records != NULL) {
    UINTN Index = 0;
    // Traverse the memory allocation records in this pool record
    while (Index < Records->Count) {
      // Check if the address matches
      EFI_VIRTUAL_MEMORY_POOL_RECORD *Record = Records->Records + Index;
      if (Record->Address == Address) {
        // Remove the memory allocation record
        --(Records->Count);
        EfiCopyArray(EFI_VIRTUAL_MEMORY_POOL_RECORD, Record, Record + 1, Records->Count - Index);
        Status = EFI_SUCCESS;
        break;
      }
      // Increment the index
      ++Index;
    }
    // Get the next pool record
    Records = Records->Next;
  }
  // Remove any pool records with zero count
  if (This->PoolRecords != NULL) {
    // Traverse the pool records
    Records = This->PoolRecords;
    while (Records->Next != NULL) {
      // Get the next pool record
      Next = Records->Next;
      // If this pool record has no allocation records then remove the pool record
      if (Next->Count == 0) {
        // Remove the pool record
        Records->Next = Next->Next;
        // Free the pool record memory
        This->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Next, EFI_SIZE_TO_PAGES(OFFSET_OF(EFI_VIRTUAL_MEMORY_POOL_RECORDS, Records) + (Records->Size * sizeof(EFI_VIRTUAL_MEMORY_POOL_RECORD))));
      } else {
        // Continue to the next pool record
        Records = Next;
      }
    }
    // Check if the root pool record needs removed
    if ((This->PoolRecords != NULL) && (This->PoolRecords->Count == 0)) {
      // Get the root pool record and the next pool record
      Records = This->PoolRecords;
      Next = Records->Next;
      // Free the pool record memory
      This->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Records, EFI_SIZE_TO_PAGES(OFFSET_OF(EFI_VIRTUAL_MEMORY_POOL_RECORDS, Records) + (Records->Size * sizeof(EFI_VIRTUAL_MEMORY_POOL_RECORD))));
      // Remove the pool record
      This->PoolRecords = Next;
    }
  }
  return Status;
}

// VMemAllocatePages
/// Allocates memory pages from the system
/// @param This        The virtual memory protocol
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
STATIC
EFI_STATUS
EFIAPI
VMemAllocatePages (
  IN     EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN     EFI_ALLOCATE_TYPE            Type,
  IN     EFI_MEMORY_TYPE              MemoryType,
  IN     UINTN                        Pages,
  IN OUT EFI_PHYSICAL_ADDRESS        *Memory,
  IN     EFI_HANDLE                   ImageHandle,
  IN     CONST CHAR8                 *Source,
  IN     UINTN                        LineNumber
) {
  EFI_STATUS Status;
  EFI_TPL    OldTpl;
  // Check parameters
  if ((This == NULL) || (This->AllocatePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if (Memory == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Protect memory
  OldTpl = EfiRaiseTPL(TPL_NOTIFY);
  // Take different actions depending on the method of allocation
  switch (Type) {
    default:
    case AllocateAnyPages:
      // Set the maximum to NULL to find the available highest pages
      *Memory = NULL;
    case AllocateMaxAddress:
      // Find the highest pages below the address specified
      Status = EfiFindHighestPages(Pages, Memory);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      // Change the type to allocate at the found address
      Type = AllocateAddress;

    case AllocateAddress:
      break;
  }
  // Allocate the pages
  Status = This->AllocatePages(Type, MemoryType, Pages, Memory);
  if (!EFI_ERROR(Status)) {
    // Add a page memory record
    Status = VMemAddPageRecord(This, *Memory, Pages, MemoryType, ImageHandle, Source, LineNumber);
  }
  // Restore old TPL
  EfiRestoreTPL(OldTpl);
  return Status;
}
// VMemFreePages
/// Frees memory pages
/// @param This   The virtual memory protocol
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
STATIC
EFI_STATUS
EFIAPI
VMemFreePages (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN EFI_PHYSICAL_ADDRESS         Memory,
  IN UINTN                        Pages
) {
  EFI_STATUS Status;
  EFI_TPL    OldTpl;
  if ((This == NULL) || (This->FreePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Protect memory
  OldTpl = EfiRaiseTPL(TPL_NOTIFY);
  // Lock the virtual memory
  EfiLock(&(This->Lock));
  // Free the pages
  Status = This->FreePages(Memory, Pages);
  if (!EFI_ERROR(Status)) {
    // Remove the page memory record
    Status = VMemRemovePageRecord(This, Memory, Pages);
    // A page may have been allocated before virtual memory was installed
    if (Status == EFI_NOT_FOUND) {
      Status = EFI_SUCCESS;
    }
  }
  // Unlock the virtal memory
  EfiUnlock(&(This->Lock));
  // Restore old TPL
  EfiRestoreTPL(OldTpl);
  return Status;
}
// VMemAllocatePool
/// Allocates pool memory
/// @param This        The virtual memory protocol
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
STATIC
EFI_STATUS
EFIAPI
VMemAllocatePool (
  IN  EFI_VIRTUAL_MEMORY_PROTOCOL  *This,
  IN  EFI_MEMORY_TYPE               PoolType,
  IN  UINTN                         Size,
  OUT VOID                        **Buffer,
  IN  EFI_HANDLE                    ImageHandle,
  IN  CONST CHAR8                  *Source,
  IN  UINTN                         LineNumber
) {
  EFI_STATUS Status;
  EFI_TPL    OldTpl;
  // Check parameters
  if ((This == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Protect memory
  OldTpl = EfiRaiseTPL(TPL_NOTIFY);
  // Lock the virtual memory
  EfiLock(&(This->Lock));
  // Allocate the pages
  Status = VMemAllocatePoolRecord(This, PoolType, Size, Buffer);
  if (!EFI_ERROR(Status)) {
    // Add a page memory record
    Status = VMemAddPoolRecord(This, *Buffer, Size, PoolType, ImageHandle, Source, LineNumber);
  }
  // Unlock the virtual memory
  EfiUnlock(&(This->Lock));
  // Restore old TPL
  EfiRestoreTPL(OldTpl);
  return Status;
}
// VMemFreePool
/// Returns pool memory to the system
/// @param This   The virtual memory protocol
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
STATIC
EFI_STATUS
EFIAPI
VMemFreePool (
  IN EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN VOID                        *Buffer
) {
  EFI_STATUS Status;
  EFI_TPL    OldTpl;
  if ((This == NULL) || (This->FreePool == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Protect memory
  OldTpl = EfiRaiseTPL(TPL_NOTIFY);
  // Lock the virtual memory
  EfiLock(&(This->Lock));
  // Free the pages
  Status = VMemFreePoolRecord(This, Buffer);
  if (!EFI_ERROR(Status)) {
    // Remove the page memory record
    Status = VMemRemovePoolRecord(This, Buffer);
  } else if (Status == EFI_NOT_FOUND) {
    // An allocation may have occurred before virtual memory installation
    Status = This->FreePool(Buffer);
  }
  // Unlock the virtual memory
  EfiUnlock(&(This->Lock));
  // Restore old TPL
  EfiRestoreTPL(OldTpl);
  return Status;
}
// VirtualGetMemoryMap
/// Returns the current memory map
/// @param This              The virtual memory protocol
/// @param MemoryMapSize     On input, the size in bytes of the memory descriptors buffer, on output, the size in bytes of the memory map descriptors
/// @param MemoryMap         On output, the current memory map descriptors
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @retval EFI_SUCCESS           The memory map was returned in the MemoryMap buffer
/// @retval EFI_BUFFER_TOO_SMALL  The MemoryMap buffer was too small, the current buffer size needed to hold the memory map is returned in MemoryMapSize
/// @retval EFI_INVALID_PARAMETER MemoryMapSize is NULL
/// @retval EFI_INVALID_PARAMETER The MemoryMap buffer is not too small and MemoryMap is NULL
STATIC
EFI_STATUS
EFIAPI
VMemGetMemoryMap (
  IN     EFI_VIRTUAL_MEMORY_PROTOCOL *This,
  IN OUT UINTN                       *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR       *MemoryMap,
  OUT    UINTN                       *MapKey,
  OUT    UINTN                       *DescriptorSize,
  OUT    UINT32                      *DescriptorVersion
) {
  EFI_STATUS             Status;
  EFI_MEMORY_DESCRIPTOR *Descriptor1;
  EFI_MEMORY_DESCRIPTOR *Descriptor2;
  UINTN                  AvailableMapSize;
  UINTN                  MapSize;
  UINTN                  DescSize;
  if ((This == NULL) || (This->GetMemoryMap == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if ((MemoryMapSize == NULL) || (DescriptorSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the available memory map size
  AvailableMapSize = *MemoryMapSize;
  // Get the memory map
  Status = This->GetMemoryMap(MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    // Increase by extra spaces so the function won't fail after allocation
    *MemoryMapSize += ((*DescriptorSize) * EFI_MEMORY_MAP_DESCRIPTOR_PAD_COUNT);
    return Status;
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Get the memory and descriptor sizes
  MapSize = *MemoryMapSize;
  DescSize = *DescriptorSize;
  // Sort and remove duplicate or invalid descriptors
  Descriptor1 = NULL;
  while (!EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor1))) {
    // If the number of pages is zero the descriptor is invalid
    if (Descriptor1->NumberOfPages == 0) {
      // Remove The descriptor
      EfiRemoveMemoryDescriptor(MemoryMap, &MapSize, DescSize, Descriptor1);
      // Get the previous descriptor
      if (EFI_ERROR(EfiGetPreviousMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor1))) {
        Descriptor1 = NULL;
      }
    } else {
      // Check if this is available memory below 1MB barrier
      if ((Descriptor1->Type == EfiConventionalMemory) && (Descriptor1->PhysicalStart < 0x100000)) {
        // Change this available memory to reserved memory below 1MB barrier
        Descriptor1->Type = EfiReservedMemoryType;
      }
      // Get the next descriptor after this and check it for 
      Descriptor2 = Descriptor1;
      while (!EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor2))) {
        // Check if this is available memory below 1MB barrier
        if ((Descriptor2->Type == EfiConventionalMemory) && (Descriptor2->PhysicalStart < 0x100000)) {
          // Change this available memory to reserved memory below 1MB barrier
          Descriptor2->Type = EfiReservedMemoryType;
        }
        // If the number of pages is zero the descriptor is invalid
        if (Descriptor1->PhysicalStart == Descriptor2->PhysicalStart) {
          // Try to determine how to best fix these overlapping regions
          if (Descriptor1->Type == Descriptor2->Type) {
            // Check which descriptor has most pages and use that number to merge
            if (Descriptor1->NumberOfPages < Descriptor2->NumberOfPages) {
              Descriptor1->NumberOfPages = Descriptor2->NumberOfPages;
            }
            Descriptor1->Attribute |= Descriptor2->Attribute;
            // Remove The descriptor
            EfiRemoveMemoryDescriptor(MemoryMap, &MapSize, DescSize, Descriptor2);
          } else {
            // TODO: Determine what to do with these overlapped regions of different types - error condition?
            continue;
          }
        } else if ((Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages)) == Descriptor2->PhysicalStart) {
          // Check if these two regions should be merged
          if (Descriptor1->Type == Descriptor2->Type) {
            // Merge these two regions
            Descriptor1->NumberOfPages += Descriptor2->NumberOfPages;
            Descriptor1->Attribute |= Descriptor2->Attribute;
            // Remove The descriptor
            EfiRemoveMemoryDescriptor(MemoryMap, &MapSize, DescSize, Descriptor2);
          } else {
            // Cannot merge these two regions so continue
            continue;
          }
        } else if (Descriptor1->PhysicalStart == (Descriptor2->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor2->NumberOfPages))) {
          // Check if these two regions should be merged and if not then swap the descriptors
          if (Descriptor1->Type == Descriptor2->Type) {
            // Merge these two regions
            Descriptor1->PhysicalStart = Descriptor2->PhysicalStart;
            Descriptor1->NumberOfPages += Descriptor2->NumberOfPages;
            Descriptor1->Attribute |= Descriptor2->Attribute;
            // Remove The descriptor
            EfiRemoveMemoryDescriptor(MemoryMap, &MapSize, DescSize, Descriptor2);
          } else {
            // Cannot merge these two regions so swap the descriptors
            EfiByteSwapMemoryDescriptor(MemoryMap, MapSize, DescSize, Descriptor1, Descriptor2);
          }
        } else if ((((Descriptor1->PhysicalStart < Descriptor2->PhysicalStart) &&
                    ((Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages)) > Descriptor2->PhysicalStart))) ||
                   (((Descriptor1->PhysicalStart > Descriptor2->PhysicalStart) &&
                    (Descriptor1->PhysicalStart < (Descriptor2->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor2->NumberOfPages)))))) {
          // Try to determine how to best fix these overlapping regions
          if (Descriptor1->Type == Descriptor2->Type) {
            // Merge these two regions
            if (Descriptor1->PhysicalStart > Descriptor2->PhysicalStart) {
              EFI_PHYSICAL_ADDRESS Address = Descriptor1->PhysicalStart;
              Descriptor1->PhysicalStart = Descriptor2->PhysicalStart;
              Descriptor2->PhysicalStart = Address;
            }
            Descriptor1->NumberOfPages = (Descriptor1->NumberOfPages + Descriptor2->NumberOfPages - (UINTN)(Descriptor2->PhysicalStart - Descriptor1->PhysicalStart));
            Descriptor1->Attribute |= Descriptor2->Attribute;
            // Remove The descriptor
            EfiRemoveMemoryDescriptor(MemoryMap, &MapSize, DescSize, Descriptor2);
          } else {
            // TODO: Determine what to do with these overlapped regions of different types - error condition?
            continue;
          }
        } else if (Descriptor1->PhysicalStart > Descriptor2->PhysicalStart) {
          // Swap the descriptors
          EfiByteSwapMemoryDescriptor(MemoryMap, MapSize, DescSize, Descriptor1, Descriptor2);
        } else {
          // This appears to be a skippable region for now
          continue;
        }
        // Get the previous descriptor to prevent skipping any regions
        if (EFI_ERROR(EfiGetPreviousMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor2))) {
          Descriptor2 = Descriptor1;
        }
      }
    }
  }
  // Get the available memory map size remaining
  AvailableMapSize -= MapSize;
  // Fill any gaps in the memory map
  Descriptor1 = NULL;
  while (!EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor1))) {
    // Get the next descriptor to determine if there is a gap
    Descriptor2 = Descriptor1;
    if (EFI_ERROR(EfiGetNextMemoryDescriptor(MemoryMap, MapSize, DescSize, &Descriptor2))) {
      break;
    }
    // Check if this region has a gap before the next region
    if ((Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages)) != Descriptor2->PhysicalStart) {
      // This region has a gap so try to fill it
      if (Descriptor1->Type == EfiReservedMemoryType) {
        // This reserved region can just be extended
        Descriptor1->NumberOfPages += EFI_SIZE_TO_PAGES(Descriptor2->PhysicalStart - (Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages)));
      } else {
        // A reserved memory descriptor needs to be inserted
        if (AvailableMapSize > DescSize) {
          UINTN NumberOfPages = EFI_SIZE_TO_PAGES(Descriptor2->PhysicalStart - (Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages)));
          // Shift the regions after this to make room to insert a descriptor
          EfiCopyMem((VOID*)(((UINT8 *)Descriptor2) + DescSize), (VOID *)Descriptor2, MapSize - (UINTN)(((UINT8 *)Descriptor2) - ((UINT8 *)MemoryMap)));
          // Insert a reserved memory descriptor for the region
          EfiZeroMem((VOID *)Descriptor2, DescSize);
          Descriptor2->Type = EfiReservedMemoryType;
          Descriptor2->PhysicalStart = Descriptor1->PhysicalStart + EFI_PAGES_TO_SIZE(Descriptor1->NumberOfPages);
          Descriptor2->NumberOfPages = NumberOfPages;
          // Decrease the available map size
          AvailableMapSize -= DescSize;
        } else if (!EFI_ERROR(Status)) {
          // There is not enough space left so if not already an error then set buffer too small
          Status = EFI_BUFFER_TOO_SMALL;
        }
        // Increase the map size
        MapSize += DescSize;
      }
    }
  }
  // Return the memory map size and status
  *MemoryMapSize = MapSize;
  return Status;
}

//
// These are just stubs for allocating from virtual memory map instead of directly from firmware memory map,
//  these replace the boot services methods to allocate memory according to the virtual memory rules
//

// VirtualAllocatePages
/// Allocates memory pages from the system
/// @param Type       The type of allocation to perform
/// @param MemoryType The type of memory to allocate
/// @param Pages      The number of contiguous 4 KiB pages to allocate
/// @param Memory     On input, the way in which the address is used depends on the value of Type.
///                    On output, the address of the base of the page range that was allocated
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER Type is not AllocateAnyPages or AllocateMaxAddress or AllocateAddress
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
/// @retval EFI_NOT_FOUND         The requested pages could not be found
STATIC
EFI_STATUS
EFIAPI
VirtualAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory
) {
  return VMemAllocatePages(VMemGetProtocol(), Type, MemoryType, Pages, Memory, NULL, NULL, 0);
}
// VirtualFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
STATIC
EFI_STATUS
EFIAPI
VirtualFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
) {
  return VMemFreePages(VMemGetProtocol(), Memory, Pages);
}
// VirtualAllocatePool
/// Allocates pool memory
/// @param PoolType The type of pool memory to allocate
/// @param Size     The size in bytes to allocate
/// @param Buffer   On output, the allocated pool memory
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
STATIC
EFI_STATUS
EFIAPI
VirtualAllocatePool (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer
) {
  return EfiInternalAllocatePool(PoolType, Size, Buffer);
}
// VirtualFreePool
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
STATIC
EFI_STATUS
EFIAPI
VirtualFreePool (
  IN VOID *Buffer
) {
  return EfiInternalFreePool(Buffer);
}
// VirtualGetMemoryMap
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
STATIC
EFI_STATUS
EFIAPI
VirtualGetMemoryMap (
  IN OUT UINTN                 *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
  OUT    UINTN                 *MapKey,
  OUT    UINTN                 *DescriptorSize,
  OUT    UINT32                *DescriptorVersion
) {
  return VMemGetMemoryMap(VMemGetProtocol(), MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
}

// EfiVirtualMemoryInstall
/// Install virtual memory services
/// @retval EFI_SUCCESS The virtual memory services were installed
EFI_STATUS
EFIAPI
EfiVirtualMemoryInstall (
  VOID
) {
  EFI_STATUS                           Status;
  UINT32                               Crc32;
  EFI_PHYSICAL_ADDRESS                 HighestMemory = NULL;
  EFI_VIRTUAL_MEMORY_RECORD           *Record;
  EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *Available;
  // Check protocol already installed
  EFI_VIRTUAL_MEMORY_PROTOCOL         *VirtualMemory = VMemGetProtocol();
  if (VirtualMemory != NULL) {
    return EFI_SUCCESS;
  }
  // Check parameters
  if ((gEfiBootServices == NULL) || (gEfiBootServices->AllocatePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Find the highest memory region
  Status = EfiFindHighestPages(EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT, &HighestMemory);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Allocate the highest memory region
  Status = gEfiBootServices->AllocatePages(AllocateAddress, EFI_MEMORY_TYPE_DEFAULT_POOL, EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT, &HighestMemory);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (HighestMemory == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set the virtual memory protocol to the region
  VirtualMemory = (EFI_VIRTUAL_MEMORY_PROTOCOL *)(UINTN)HighestMemory;
  // There are no allocation records yet
  VirtualMemory->PageRecords = NULL;
  VirtualMemory->PoolRecords = NULL;
  // Set the first memory record to the remaining space in the page(s)
  Record = (EFI_VIRTUAL_MEMORY_RECORD *)(UINTN)(HighestMemory + sizeof(EFI_VIRTUAL_MEMORY_PROTOCOL));
  Record->Next = NULL;
  Record->Size = EFI_PAGES_TO_SIZE(EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT) - sizeof(EFI_VIRTUAL_MEMORY_PROTOCOL) - sizeof(EFI_VIRTUAL_MEMORY_RECORD);
  Record->Type = EFI_MEMORY_TYPE_DEFAULT_POOL;
  Record->Flags = EFI_VIRTUAL_MEMORY_RECORD_FLAG_STATIC;
  VirtualMemory->Records = Record;
  //// Set the available space in the record to the remaining space in the page(s)
  Available = (EFI_VIRTUAL_MEMORY_AVAILABLE_RECORD *)(UINTN)(HighestMemory + sizeof(EFI_VIRTUAL_MEMORY_PROTOCOL) + sizeof(EFI_VIRTUAL_MEMORY_RECORD));
  Available->Next = NULL;
  Available->Size = Record->Size;
  Record->Available = Available;
  // Store old boot services memory methods
  Crc32 = gEfiBootServices->Hdr.CRC32;
  VirtualMemory->AllocatePages = gEfiBootServices->AllocatePages;
  VirtualMemory->AllocatePool = gEfiBootServices->AllocatePool;
  VirtualMemory->FreePages = gEfiBootServices->FreePages;
  VirtualMemory->FreePool = gEfiBootServices->FreePool;
  VirtualMemory->GetMemoryMap = gEfiBootServices->GetMemoryMap;
  // Install the protocol
  Status = EfiInstallMultipleProtocolInterfaces(&gEfiImageHandle, &mEfiVirtualMemoryProtocolGuid, (VOID *)VirtualMemory, NULL);
  if (!EFI_ERROR(Status)) {
    // Set boot services memory methods overrides
    gEfiBootServices->Hdr.CRC32 = 0;
    gEfiBootServices->FreePages = VirtualFreePages;
    gEfiBootServices->AllocatePages = VirtualAllocatePages;
    gEfiBootServices->FreePool  = VirtualFreePool;
    gEfiBootServices->AllocatePool = VirtualAllocatePool;
    gEfiBootServices->GetMemoryMap = VirtualGetMemoryMap;
    // Update the boot services table CRC32
    Status = EfiCalculateCrc32(gEfiBootServices, gEfiBootServices->Hdr.HeaderSize, &(gEfiBootServices->Hdr.CRC32));
  }
  if (EFI_ERROR(Status)) {
    // Restore the original boot services table on error
    gEfiBootServices->AllocatePages = VirtualMemory->AllocatePages;
    gEfiBootServices->AllocatePool = VirtualMemory->AllocatePool;
    gEfiBootServices->FreePages = VirtualMemory->FreePages;
    gEfiBootServices->FreePool  = VirtualMemory->FreePool;
    gEfiBootServices->GetMemoryMap = VirtualMemory->GetMemoryMap;
    gEfiBootServices->Hdr.CRC32 = Crc32;
    // Free the protocol memory
    EfiFreePages(HighestMemory, EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT);
  }
  // Return the status code
  return Status;
}
// EfiVirtualMemoryUninstall
/// Uninstall virtual memory services
/// @retval EFI_SUCCESS The virtual memory services were uninstalled
EFI_STATUS
EFIAPI
EfiVirtualMemoryUninstall (
  VOID
) {
  EFI_STATUS Status;
  // Get protocol
  EFI_VIRTUAL_MEMORY_PROTOCOL *VMem = VMemGetProtocol();
  // Check protocol is installed
  if (VMem == NULL) {
    return EFI_NOT_FOUND;
  }
  // Uninstall the protocol
  Status = EfiUninstallProtocolInterface(gEfiImageHandle, &mEfiVirtualMemoryProtocolGuid, (VOID *)VMem);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Set back original boot services memory methods
  gEfiBootServices->AllocatePages = VMem->AllocatePages;
  gEfiBootServices->AllocatePool = VMem->AllocatePool;
  gEfiBootServices->FreePages = VMem->FreePages;
  gEfiBootServices->FreePool  = VMem->FreePool;
  gEfiBootServices->GetMemoryMap = VMem->GetMemoryMap;
  // Update the boot services table CRC32
  gEfiBootServices->Hdr.CRC32 = 0;
  EfiCalculateCrc32(gEfiBootServices, gEfiBootServices->Hdr.HeaderSize, &(gEfiBootServices->Hdr.CRC32));
  // Free the pages
  return EfiFreePages((EFI_PHYSICAL_ADDRESS)(UINTN)VMem, EFI_MEMORY_VIRTUAL_DEFAULT_PAGE_COUNT);
}

// EfiPrintMemoryPageRecord
/// Print the memory page allocation records
STATIC
VOID
EFIAPI
EfiPrintMemoryPageRecord (
  IN EFI_VIRTUAL_MEMORY_PAGE_RECORD *Record
) {
  if (Record != NULL) {
    CONST CHAR16 *Description = EfiGetMemoryTypeDescription(Record->Type);
    if (Description != NULL) {
      LOG(L"%a(%u): Allocated 0x%x pages of type %s at 0x%p\n", (Record->Source != NULL) ? Record->Source : "Unknown source", Record->LineNumber, Record->Count, Description, Record->Address);
    } else {
      LOG(L"%a(%u): Allocated 0x%x pages of type 0x%x at 0x%p\n", (Record->Source != NULL) ? Record->Source : "Unknown source", Record->LineNumber, Record->Count, Record->Type, Record->Address);
    }
  }
}
// EfiPrintMemoryPoolRecord
/// Print the memory pool allocation records
STATIC
VOID
EFIAPI
EfiPrintMemoryPoolRecord (
  IN EFI_VIRTUAL_MEMORY_POOL_RECORD *Record
) {
  if (Record != NULL) {
    CONST CHAR16 *Description = EfiGetMemoryTypeDescription(Record->Type);
    UINTN         Index = 0;
    UINTN         Size;
    if (Description != NULL) {
      LOG(L"%a(%u): Allocated pool memory of type %s, 0x%x bytes at 0x%p\n", (Record->Source != NULL) ? Record->Source : "Unknown source", Record->LineNumber, Description, Record->Size, Record->Address);
    } else {
      LOG(L"%a(%u): Allocated pool memory of type 0x%x, 0x%x bytes at 0x%p\n", (Record->Source != NULL) ? Record->Source : "Unknown source", Record->LineNumber, Record->Type, Record->Size, Record->Address);
    }
    LOG(L"  <");
    Size = (Record->Size < EFI_MEMORY_PRINT_PREVIEW_SIZE) ? Record->Size : EFI_MEMORY_PRINT_PREVIEW_SIZE;
    while (Index < Size) {
      if (Index != 0) {
        LOG(L" ");
      }
      LOG(L"%02x", (UINTN)(*(((UINT8 *)(Record->Address)) + Index)));
      ++Index;
    }
    LOG(L">\n");
    LOG(L"  <%-.*s>\n", EFI_MEMORY_PRINT_PREVIEW_SIZE, Record->Address);
  }
}
// EfiPrintMemoryRecords
/// Print the memory allocation records
VOID
EFIAPI
EfiPrintMemoryRecords (
  VOID
) {
  // Get protocol
  EFI_VIRTUAL_MEMORY_PROTOCOL *VMem = VMemGetProtocol();
  if (VMem != NULL) {
    EFI_VIRTUAL_MEMORY_PAGE_RECORDS *PageRecords;
    EFI_VIRTUAL_MEMORY_POOL_RECORDS *PoolRecords;
    // Print page allocation records
    PageRecords = VMem->PageRecords;
    if (PageRecords != NULL) {
      UINTN Count = 0;
      UINTN Size = 0;
      LOG(L"Memory page records:\n");
      while (PageRecords != NULL) {
        UINTN Index = 0;
        while (Index < PageRecords->Count) {
          Size += EFI_PAGES_TO_SIZE(PageRecords->Records[Index].Count);
          EfiPrintMemoryPageRecord(PageRecords->Records + Index++);
        }
        Count += PageRecords->Count;
        PageRecords = PageRecords->Next;
      }
      LOG(L"Allocated %u memory page records, 0x%x total bytes\n", Count, Size);
    }
    // Print pool allocation records
    PoolRecords = VMem->PoolRecords;
    if (PoolRecords != NULL) {
      UINTN Count = 0;
      UINTN Size = 0;
      LOG(L"Memory pool records:\n");
      while (PoolRecords != NULL) {
        UINTN Index = 0;
        while (Index < PoolRecords->Count) {
          Size += PoolRecords->Records[Index].Size;
          EfiPrintMemoryPoolRecord(PoolRecords->Records + Index++);
        }
        Count += PoolRecords->Count;
        PoolRecords = PoolRecords->Next;
      }
      LOG(L"Allocated %u memory pool records, 0x%x total bytes\n", Count, Size);
    }
  }
}
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
) {
  return VMemAllocatePages(VMemGetProtocol(), Type, MemoryType, Pages, Memory, ImageHandle, Source, LineNumber);
}
// EfiVirtualFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
EFI_STATUS
EFIAPI
EfiVirtualFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
) {
  return VMemFreePages(VMemGetProtocol(), Memory, Pages);
}
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
EFI_STATUS
EFIAPI
EfiVirtualAllocatePool (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer,
  IN  EFI_HANDLE        ImageHandle,
  IN  CONST CHAR8      *Source,
  IN  UINTN             LineNumber
) {
  return VMemAllocatePool(VMemGetProtocol(), PoolType, Size, Buffer, ImageHandle, Source, LineNumber);
}
// EfiVirtualFreePool
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EFI_STATUS
EFIAPI
EfiVirtualFreePool (
  IN VOID *Buffer
) {
  return VMemFreePool(VMemGetProtocol(), Buffer);
}

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
) {
  EFI_VIRTUAL_MEMORY_PROTOCOL *VMem = VMemGetProtocol();
  if ((VMem == NULL) || (VMem->AllocatePages == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return VMem->AllocatePages(AllocateAnyPages, EFI_MEMORY_TYPE_DEFAULT_POOL, Pages, Memory);
}
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
) {
  return VMemFreePages(VMemGetProtocol(), Memory, Pages);
}
// EfiInternalAllocate
/// Add memory pool record to virtual memory map
/// @param Size    The size in bytes of the allocation
/// @return The pool allocation or NULL
VOID *
EFIAPI
EfiInternalAllocate (
  IN  UINTN Size
) {
  VOID *Address = NULL;
  if (EFI_ERROR(EfiInternalAllocatePool(EFI_MEMORY_TYPE_DEFAULT_POOL, Size, &Address))) {
    return NULL;
  }
  return Address;
}
// EfiInternalAllocatePool
/// Add memory pool record to virtual memory map
/// @param Type    The type of memory to allocate
/// @param Size    The size in bytes of the allocation
/// @param Address On output, the address of the memory that was allocated
/// @retval EFI_SUCCESS           The record was added
/// @retval EFI_OUT_OF_RESOURCES  The record could not be allocated
/// @retval EFI_INVALID_PARAMETER Memory is NULL
EFI_STATUS
EFIAPI
EfiInternalAllocatePool (
  IN  EFI_MEMORY_TYPE   Type,
  IN  UINTN             Size,
  OUT VOID            **Address
) {
  return VMemAllocatePoolRecord(VMemGetProtocol(), Type, Size, Address);
}
// EfiInternalFreePool
/// Remove memory pool record from virtual memory map
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EFI_STATUS
EFIAPI
EfiInternalFreePool (
  IN VOID *Buffer
) {
  return VMemFreePoolRecord(VMemGetProtocol(), Buffer);
}

#endif // EFI_MEMORY_VIRTUAL
