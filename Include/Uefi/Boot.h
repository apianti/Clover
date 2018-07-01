///
/// @file Include/Uefi/Boot.h
///
/// UEFI implementation boot services table
///

#pragma once
#ifndef ___UEFI_BOOT_SERVICES_HEADER__
#define ___UEFI_BOOT_SERVICES_HEADER__

#include <Uefi/Protocol/DevicePath.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//
// Task Priority Services (EFI 1.0+)
//

// TPL_APPLICATION
/// This is the lowest priority level. It is the level of execution which occurs when
///  no event notifications are pending and which interacts with the user. User I/O
///  (and blocking on User I/O) can be performed at this level. The boot manager
///  executes at this level and passes control to other UEFI applications at this level
#define TPL_APPLICATION 4
// TPL_CALLBACK
/// Interrupts code executing below TPL_CALLBACK level Long term operations
///  (such as file system operations and disk I/O) can occur at this level
#define TPL_CALLBACK 8
// TPL_NOTIFY
/// Interrupts code executing below TPL_NOTIFY level Blocking is not allowed at this level.
///  Code executes to completion and returns. If code requires more processing, it needs to
///  to signal an event to wait to obtain control again at whatever level it requires. This level
///  is typically used to process low level IO to or from a device. (Firmware Interrupts) This
///  level is internal to the firmware. It is the level at which internal interrupts occur.
///  Code running at this level interrupts code running at the TPL_NOTIFY level (or lower levels).
///  If the interrupt requires extended time to complete, firmware signals another event (or events)
///  to perform the longer term operations so that other interrupts can occur
#define TPL_NOTIFY 16
// TPL_HIGH_LEVEL
/// Interrupts code executing below TPL_HIGH_LEVEL This is the highest priority level.
///  It is not interruptible (interrupts are disabled) and is used sparingly by firmware
///  to synchronize operations that need to be accessible from any priority level. For example,
///  it must be possible to signal events while executing at any priority level. Therefore,
///  firmware manipulates the internal event structure while at this priority level
#define TPL_HIGH_LEVEL 31

// EFI_RAISE_TPL
/// Raises a task’s priority level and returns its previous level
/// @param NewTpl The new task priority level that must be greater than or equal to the current task priority level
/// @return The previous task priority level
typedef
EFI_TPL
(EFIAPI
*EFI_RAISE_TPL) (
  IN EFI_TPL NewTpl
);
// EFI_RESTORE_TPL
/// Restores a task’s priority level to its previous value
/// @param OldTpl The previous task priority level to restore
typedef
VOID
(EFIAPI
*EFI_RESTORE_TPL) (
  IN EFI_TPL OldTpl
);

//
// Memory Services (EFI 1.0+)
//

// EFI_MEMORY_DESCRIPTOR_VERSION
/// Memory descriptor version
#define EFI_MEMORY_DESCRIPTOR_VERSION 1

// EFI_MEMORY_UC
/// Memory cacheability attribute: The memory region supports being configured as not cacheable
#define EFI_MEMORY_UC 0x0000000000000001
// EFI_MEMORY_WC
/// Memory cacheability attribute: The memory region supports being configured as write combining
#define EFI_MEMORY_WC 0x0000000000000002
// EFI_MEMORY_WT
/// Memory cacheability attribute: The memory region supports being configured as cacheable with
///  a “write through” policy. Writes that hit in the cache will also be written to main memory
#define EFI_MEMORY_WT 0x0000000000000004
// EFI_MEMORY_WB
/// Memory cacheability attribute: The memory region supports being configured as cacheable
///  with a “write back” policy. Reads and writes that hit in the cache do not propagate to
///  main memory. Dirty data is written back to main memory when a new cache line is allocated
#define EFI_MEMORY_WB 0x0000000000000008
// EFI_MEMORY_UCE
/// Memory cacheability attribute: The memory region supports being configured as
///  not cacheable, exported, and supports the “fetch and add” semaphore mechanism
#define EFI_MEMORY_UCE 0x0000000000000010
// EFI_MEMORY_WP
/// Physical memory protection attribute: The memory region supports being configured as write-protected
///  by system hardware. This is typically used as a cacheability attribute today. The memory region
///  supports being configured as cacheable with a "write protected" policy. Reads come from cache
///  lines when possible, and read misses cause cache fills. Writes are propagated to the system
///  bus and cause corresponding cache lines on all processors on the bus to be invalidated
#define EFI_MEMORY_WP 0x0000000000001000
// EFI_MEMORY_RP
/// Physical memory protection attribute: The memory region supports being configured as read-protected by system hardware
#define EFI_MEMORY_RP 0x0000000000002000
// EFI_MEMORY_XP
/// Physical memory protection attribute: The memory region supports being configured so it is protected by system hardware from executing code
#define EFI_MEMORY_XP 0x0000000000004000
// EFI_MEMORY_NV
///  Runtime memory attribute: The memory region refers to persistent memory
#define EFI_MEMORY_NV 0x0000000000008000
// EFI_MEMORY_MORE_RELIABLE
/// The memory region provides higher reliability relative to other memory in
///  the system. If all memory has the same reliability, then this bit is not used
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
// EFI_MEMORY_RO
/// Physical memory protection attribute: The memory region supports making this memory range read-only by system hardware
#define EFI_MEMORY_RO 0x0000000000020000
// EFI_MEMORY_RUNTIME
/// Runtime memory attribute: The memory region needs to be given a virtual mapping
#define EFI_MEMORY_RUNTIME 0x8000000000000000

// EFI_ALLOCATE_TYPE
/// The type of memory allocation
typedef enum EFI_ALLOCATE_TYPE EFI_ALLOCATE_TYPE;
enum EFI_ALLOCATE_TYPE {

  // AllocateAnyPages
  /// Allocate any available range of pages
  AllocateAnyPages,
  // AllocateMaxAddress
  /// Allocate any available range of pages whose uppermost address is less than or equal to the input address
  AllocateMaxAddress,
  // AllocateAddress
  /// Allocate pages at the input address
  AllocateAddress,
  // MaxAllocateType
  /// The count of allocation types
  MaxAllocateType

};
// EFI_MEMORY_TYPE
/// The type of memory
typedef enum EFI_MEMORY_TYPE EFI_MEMORY_TYPE;
enum EFI_MEMORY_TYPE {

  // EfiReservedMemoryType
  /// Not usable
  EfiReservedMemoryType,
  // EfiLoaderCode
  /// During boot services, the code portions of a loaded UEFI application
  ///  During runtime services, the UEFI OS Loader and/or OS may use this memory as they see fit
  EfiLoaderCode,
  // EfiLoaderData
  /// During boot services, the data portions of a loaded UEFI application and the default data allocation type used by a UEFI application to allocate pool memory
  ///  During runtime services, the UEFI OS Loader and/or OS may use this memory as they see fit
  EfiLoaderData,
  // EfiBootServicesCode
  /// During boot services, the code portions of a loaded UEFI Boot Service Driver
  ///  During runtime services, free memory available for general use
  EfiBootServicesCode,
  // EfiBootServicesData
  /// During boot services, the data portions of a loaded UEFI Boot Serve Driver, and the default data allocation type used by a UEFI Boot Service Driver to allocate pool memory
  ///  During runtime services, free memory available for general use
  EfiBootServicesData,
  // EfiRuntimeServicesCode
  /// During boot services, the code portions of a loaded UEFI Runtime Driver
  ///  During runtime services, the memory in this range is to be preserved by the UEFI OS loader and OS in the working and ACPI S1–S3 states
  EfiRuntimeServicesCode,
  // EfiRuntimeServicesData
  /// During boot services, the data portions of a loaded UEFI Runtime Driver and the default data allocation type used by a UEFI Runtime Driver to allocate pool memory
  ///  During runtime services, the memory in this range is to be preserved by the UEFI OS loader and OS in the working and ACPI S1–S3 states
  EfiRuntimeServicesData,
  // EfiConventionalMemory
  /// Free memory available for general use
  EfiConventionalMemory,
  // EfiUnusableMemory
  /// Memory that contains errors and is not to be used
  EfiUnusableMemory,
  // EfiACPIReclaimMemory
  /// During boot services, memory that holds the ACPI tables
  ///  During runtime services, this memory is to be preserved by the UEFI OS loader and OS until ACPI is enabled, then the memory in this range is available for general use
  EfiACPIReclaimMemory,
  // EfiACPIMemoryNVS
  /// During boot services, address space reserved for use by the firmware
  ///  During runtime services, this memory is to be preserved by the UEFI OS loader and OS in the working and ACPI S1–S3 states
  EfiACPIMemoryNVS,
  // EfiMemoryMappedIO
  /// During boot services, used by system firmware to request that a memory-mapped IO region be mapped by the OS to a virtual address so it can be accessed by runtime services
  ///  During runtime services, this memory is not used by the OS. All system memory-mapped IO information should come from ACPI tables
  EfiMemoryMappedIO,
  // EfiMemoryMappedIOPortSpace
  /// During boot services, system memory-mapped IO region that is used to translate memory cycles to IO cycles by the processor
  ///  During runtime services, this memory is not used by the OS. All system memory-mapped IO port space information should come from ACPI tables
  EfiMemoryMappedIOPortSpace,
  // EfiPalCode
  /// During boot services, address space reserved by the firmware for code that is part of the processor
  ///  During runtime services, memory is to be preserved by the UEFI OS loader and OS in the working and ACPI S1–S4 states.
  ///  This memory may also have other attributes that are defined by the processor implementation
  EfiPalCode,
  // EfiPersistentMemory
  /// Memory region that operates as EfiConventionalMemory but happens to also support byte-addressable non-volatility
  EfiPersistentMemory,
  // EfiMaxMemoryType
  /// The count of specification defined memory types
  EfiMaxMemoryType

};

// EFI_LOAD_OPTION
/// TODO: Boot#### and Driver#### entry load options
typedef struct EFI_LOAD_OPTION EFI_LOAD_OPTION;
struct EFI_LOAD_OPTION {

  // Attributes
  /// 
  UINT32 Attributes;
  // FilePathListLength
  /// 
  UINT16 FilePathListLength;
  // Description
  /// 
  // CHAR16 Description[];
  // FilePathList
  /// 
  // EFI_DEVICE_PATH_PROTOCOL FilePathList[];
  // OptionalData
  /// 
  // UINT8 OptionalData[];

};

// EFI_MEMORY_DESCRIPTOR
/// Memory map descriptor
typedef struct EFI_MEMORY_DESCRIPTOR EFI_MEMORY_DESCRIPTOR;
struct EFI_MEMORY_DESCRIPTOR {

  // Type
  /// The type of the memory region
  UINT64               Type;
  // PhysicalStart
  /// Physical address of the first byte in the memory region, must be
  ///  aligned on a 4 KiB boundary, and must not be above 0xFFFFFFFFFFFFF000
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  // VirtualStart
  /// Virtual address of the first byte in the memory region, must be
  ///  aligned on a 4 KiB boundary, and must not be above 0xFFFFFFFFFFFFF000
  EFI_VIRTUAL_ADDRESS  VirtualStart;
  // NumberOfPages
  /// Number of 4 KiB pages in the memory region, must not be 0, and must not be any value that would
  ///  represent a memory page with a start address, either physical or virtual, above 0xFFFFFFFFFFFFF000
  UINT64               NumberOfPages;
  // Attribute
  /// Attributes of the memory region that describe the bit mask of capabilities for that
  ///  memory region, and not necessarily the current settings for that memory region
  UINT64               Attribute;

};

// EFI_MEMORY_ATTRIBUTES_TABLE
/// Memory attributes table
typedef struct EFI_MEMORY_ATTRIBUTES_TABLE EFI_MEMORY_ATTRIBUTES_TABLE;
struct EFI_MEMORY_ATTRIBUTES_TABLE {

  // Version
  /// The version of this table
  UINT32 Version;
  // NumberOfEntries
  /// Count of memory descriptor entries
  UINT32 NumberOfEntries;
  // DescriptorSize
  /// Size in bytes of a memory descriptor entry
  UINT32 DescriptorSize;
  // Reserved
  /// Reserved bytes
  UINT32 Reserved;
  // Memory descriptor entries follow
  // EFI_MEMORY_DESCRIPTOR Entries[];

};

// EFI_ALLOCATE_PAGES
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
typedef
EFI_STATUS
(EFIAPI
*EFI_ALLOCATE_PAGES) (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory
);
// EFI_FREE_PAGES
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
typedef
EFI_STATUS
(EFIAPI
*EFI_FREE_PAGES) (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
);
// EFI_GET_MEMORY_MAP
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
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_MEMORY_MAP) (
  IN OUT UINTN                 *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
  OUT    UINTN                 *MapKey,
  OUT    UINTN                 *DescriptorSize,
  OUT    UINT32                *DescriptorVersion
);
// EFI_ALLOCATE_POOL
/// Allocates pool memory
/// @param PoolType The type of pool memory to allocate
/// @param Size     The size in bytes to allocate
/// @param Buffer   On output, the allocated pool memory
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_ALLOCATE_POOL) (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer
);
// EFI_FREE_POOL
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
typedef
EFI_STATUS
(EFIAPI
*EFI_FREE_POOL) (
  IN VOID *Buffer
);

//
// Event and Timer Services (EFI 1.0+)
//

// EVT_TIMER
/// The event is a timer event
#define EVT_TIMER 0x80000000
// EVT_RUNTIME
/// The event is allocated from runtime memory
#define EVT_RUNTIME 0x40000000
// EVT_NOTIFY_WAIT
/// If an event of this type is not already in the signaled state, then the event
///  notification function will be queued at the event TPL whenever waiting for the event
#define EVT_NOTIFY_WAIT 0x00000100
// EVT_NOTIFY_SIGNAL
/// The event notify function is queued whenever the event is signaled
#define EVT_NOTIFY_SIGNAL 0x00000200
// EVT_SIGNAL_EXIT_BOOT_SERVICES
/// The event is to be notified when boot services are exited
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
// EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
/// The event is to be notified when the virtual address map is set
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

// EFI_EVENT_GROUP_EXIT_BOOT_SERVICES
/// The event group is to be notified when boot services are exited
#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES { 0x27ABF055, 0xB1B8, 0x4C26, { 0x80, 0x48, 0x74, 0x8F, 0x37, 0xBA, 0xA2, 0xDF } }
// EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE
/// The event group is to be notified when the virtual address map changed
#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE { 0x13FA7698, 0xC831, 0x49C7, { 0x87, 0xEA, 0x8F, 0x43, 0xFC, 0xC2, 0x51, 0x96 } }
// EFI_EVENT_GROUP_MEMORY_MAP_CHANGE
/// The event group is notified when the memory map has changed
#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE { 0x78BEE926, 0x692F, 0x48FD, { 0x9E, 0xDB, 0x1, 0x42, 0x2E, 0xF0, 0xD7, 0xAB } }
// EFI_EVENT_GROUP_READY_TO_BOOT
/// The event group is notified when the Boot Manager is about to load and execute a boot option
#define EFI_EVENT_GROUP_READY_TO_BOOT { 0x7CE88FB3, 0x4BD7, 0x4679, { 0x87, 0xA8, 0xA8, 0xD8, 0xDE, 0xE5, 0xD, 0x2B } }
// EFI_EVENT_GROUP_RESET_SYSTEM
/// The event group is notified when the system is about to be reset
#define EFI_EVENT_GROUP_RESET_SYSTEM { 0x62DA6A56, 0x13FB, 0x485A, { 0xA8, 0xDA, 0xA3, 0xDD, 0x79, 0x12, 0xCB, 0x6B } }

// gEfiEventGroupExitBootServicesGuid
/// The exit boot services event group identifier
EXTERN EFI_GUID gEfiEventGroupExitBootServicesGuid;
// gEfiEventGroupVirtualAddressChangeGuid
/// The virtual address map changed event group identifier
EXTERN EFI_GUID gEfiEventGroupVirtualAddressChangeGuid;
// gEfiEventGroupMemoryMapChangeGuid
/// The memory map changed event group identifier
EXTERN EFI_GUID gEfiEventGroupMemoryMapChangeGuid;
// gEfiEventGroupReadyToBootGuid
/// The ready to boot event group identifier
EXTERN EFI_GUID gEfiEventGroupReadyToBootGuid;
// gEfiEventGroupResetSystemGuid
/// The reset system event group identifier
EXTERN EFI_GUID gEfiEventGroupResetSystemGuid;

// EFI_TIMER_DELAY
/// Timer event delay type
typedef enum EFI_TIMER_DELAY EFI_TIMER_DELAY;
enum EFI_TIMER_DELAY {

  // TimerCancel
  /// Cancel the timer event
  TimerCancel,
  // TimerPeriodic
  /// The timer event is signaled periodically from the current time
  TimerPeriodic,
  // TimerRelative
  /// The timer event is signaled once from the current time
  TimerRelative

};

// EFI_EVENT_NOTIFY
/// Event notification function
/// @param Event Event whose notification function is being invoked
/// @param Context The notification function context
typedef
VOID
(EFIAPI
*EFI_EVENT_NOTIFY) (
  IN EFI_EVENT  Event,
  IN VOID      *Context
);

// EFI_CREATE_EVENT
/// Creates an event
/// @param Type           The type of event to create and its mode and attributes
/// @param NotifyTpl      The task priority level of event notifications
/// @param NotifyFunction The event’s notification function
/// @param NotifyContext  The notification function context
/// @param Event          On output, the newly created event
/// @retval EFI_SUCCESS           The event structure was created
/// @retval EFI_INVALID_PARAMETER One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Type has an unsupported bit set
/// @retval EFI_INVALID_PARAMETER Type has both EVT_NOTIFY_SIGNAL and EVT_NOTIFY_WAIT set
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyFunction is NULL
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyTpl is not a supported TPL level
/// @retval EFI_OUT_OF_RESOURCES  The event could not be allocated
typedef
EFI_STATUS
(EFIAPI
*EFI_CREATE_EVENT) (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  VOID             *NotifyContext OPTIONAL,
  OUT EFI_EVENT        *Event
);
// EFI_SET_TIMER
/// Sets the type of timer and the trigger time for a timer event
/// @param Event       The timer event that is to be signaled at the specified time
/// @param Type        The type of time that is specified in TriggerTime
/// @param TriggerTime The number of 100ns units until the timer expires. If TriggerTime is 0, then if
///                     Type is TimerRelative the timer event will be signaled on the next timer tick,
///                     or if Type is TimerPeriodic the timer event will be signaled on every timer tick
/// @retval EFI_SUCCESS           The event has been set to be signaled at the requested time
/// @retval EFI_INVALID_PARAMETER Event or Type is not valid
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_TIMER) (
  IN EFI_EVENT       Event,
  IN EFI_TIMER_DELAY Type,
  IN UINT64          TriggerTime
);
// EFI_WAIT_FOR_EVENT
/// Stops execution until an event is signaled
/// @param NumberOfEvents The number of events on which to wait
/// @param Event          The events on which to wait
/// @param Index          On output, the index of the event which satisfied the wait condition
/// @retval EFI_SUCCESS           The event indicated by Index was signaled
/// @retval EFI_INVALID_PARAMETER NumberOfEvents is 0
/// @retval EFI_INVALID_PARAMETER The event indicated by Index is of type EVT_NOTIFY_SIGNAL
/// @retval EFI_UNSUPPORTED       The current TPL is not TPL_APPLICATION
typedef
EFI_STATUS
(EFIAPI
*EFI_WAIT_FOR_EVENT) (
  IN  UINTN      NumberOfEvents,
  IN  EFI_EVENT *Event,
  OUT UINTN     *Index
);
// EFI_SIGNAL_EVENT
/// Signals an event
/// @param Event The event to signal
/// @retval EFI_SUCCESS The event was signaled
typedef
EFI_STATUS
(EFIAPI
*EFI_SIGNAL_EVENT) (
  IN EFI_EVENT Event
);
// EFI_CLOSE_EVENT
/// Closes an event
/// @param Event The event to close
/// @retval EFI_SUCCESS The event has been closed
typedef
EFI_STATUS
(EFIAPI
*EFI_CLOSE_EVENT) (
  IN EFI_EVENT Event
);
// EFI_CHECK_EVENT
/// Checks whether an event is in the signaled state
/// @param Event The event to check
/// @retval EFI_SUCCESS           The event is in the signaled state
/// @retval EFI_NOT_READY         The event is not in the signaled state
/// @retval EFI_INVALID_PARAMETER Event is of type EVT_NOTIFY_SIGNAL
typedef
EFI_STATUS
(EFIAPI
*EFI_CHECK_EVENT) (
  IN EFI_EVENT Event
);

//
// Protocol Handler Services (EFI 1.0+)
//

// EFI_INTERFACE_TYPE
/// Protocol interface type
typedef enum EFI_INTERFACE_TYPE EFI_INTERFACE_TYPE;
enum EFI_INTERFACE_TYPE {

  // EFI_NATIVE_INTERFACE
  /// Native protocol interface
  EFI_NATIVE_INTERFACE

};

// EFI_LOCATE_SEARCH_TYPE
/// Protocol handle search type
typedef enum EFI_LOCATE_SEARCH_TYPE EFI_LOCATE_SEARCH_TYPE;
enum EFI_LOCATE_SEARCH_TYPE {

  // AllHandles
  /// Locate every handle
  AllHandles,
  // ByRegisterNotify
  /// Locate the next handle that is new for the specified registration, only one handle is returned
  ///  at a time, starting with the first, and the caller must loop until no more handles are returned
  ByRegisterNotify,
  // ByProtocol
  /// Locate all handles that support the specified protocol
  ByProtocol

};

// EFI_CONFIGURATION_TABLE
/// Configuration table
typedef struct EFI_CONFIGURATION_TABLE EFI_CONFIGURATION_TABLE;
struct EFI_CONFIGURATION_TABLE {

  // VendorGuid
  /// The 128-bit GUID value that uniquely identifies the system configuration table
  EFI_GUID  VendorGuid;
  // VendorTable
  /// The configuration table
  VOID     *VendorTable;

};

// EFI_INSTALL_PROTOCOL_INTERFACE
/// Installs a protocol interface on a device handle. If the handle does not
///  exist, it is created and added to the list of handles in the system
/// @param Handle        The handle on which the interface is to be installed, if *Handle
///                       is NULL on input, a new handle is created and returned on output
/// @param Protocol      The unique identifier of the protocol
/// @param InterfaceType The interface type
/// @param Interface     The protocol interface
/// @retval EFI_SUCCESS           The protocol interface was installed
/// @retval EFI_OUT_OF_RESOURCES  A new handle could not be allocated
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER InterfaceType is not EFI_NATIVE_INTERFACE
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle
typedef
EFI_STATUS
(EFIAPI
*EFI_INSTALL_PROTOCOL_INTERFACE) (
  IN OUT EFI_HANDLE         *Handle,
  IN     EFI_GUID           *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID               *Interface OPTIONAL
);
// EFI_REINSTALL_PROTOCOL_INTERFACE
/// Reinstalls a protocol interface on a device handle
/// @param Handle The handle on which the interface is to be reinstalled
/// @param Protocol The unique identifier of the protocol
/// @param OldInterface The old protocol interface
/// @param NewInterface The new protocol interface
/// @retval EFI_SUCCESS           The protocol interface was reinstalled
/// @retval EFI_NOT_FOUND         The OldInterface on the handle was not found
/// @retval EFI_ACCESS_DENIED     The protocol interface could not be reinstalled, because OldInterface is still being used by a driver that will not release it
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_REINSTALL_PROTOCOL_INTERFACE) (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *OldInterface,
  IN VOID       *NewInterface
);
// EFI_UNINSTALL_PROTOCOL_INTERFACE
/// Removes a protocol interface from a device handle
/// @param Handle    The handle on which the interface was installed
/// @param Protocol  The unique identifier of the protocol
/// @param Interface The protocol interface
/// @retval EFI_SUCCESS           The interface was removed
/// @retval EFI_NOT_FOUND         The interface was not found
/// @retval EFI_ACCESS_DENIED     The interface was not removed because the interface is still being used by a driver
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_UNINSTALL_PROTOCOL_INTERFACE) (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *Interface
);
// EFI_HANDLE_PROTOCOL
/// Queries a handle to determine if it supports a specified protocol
/// @param Handle    The handle being queried
/// @param Protocol  The unique identifier of the protocol
/// @param Interface On output, the protocol interface
/// @retval EFI_SUCCESS           The interface information for the specified protocol was returned
/// @retval EFI_UNSUPPORTED       The device does not support the specified protocol
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Interface is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_HANDLE_PROTOCOL) (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface
);
// EFI_REGISTER_PROTOCOL_NOTIFY
/// Creates an event that is to be signaled whenever an interface is installed for a specified protocol
/// @param Protocol     The unique identifier of the protocol for which the event is to be registered
/// @param Event        The event that is to be signaled whenever a protocol interface is registered for the specified
///                      protocol, the same EFI_EVENT may be used for multiple protocol notify registrations
/// @param Registration On output, the registration identifier used to retrieve the list of handles that have added a protocol interface
/// @retval EFI_SUCCESS           The notification event has been registered
/// @retval EFI_OUT_OF_RESOURCES  The notification event could not be allocated
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Registration is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_REGISTER_PROTOCOL_NOTIFY) (
  IN  EFI_GUID   *Protocol,
  IN  EFI_EVENT   Event,
  OUT VOID      **Registration
);
// EFI_LOCATE_HANDLE
/// Returns an array of handles that support a specified protocol
/// @param SearchType The search type
/// @param Protocol   The unique identifier of the protocol for which to search
/// @param SearchKey  The registration identifier to search for new handles
/// @param BufferSize On input, the size in bytes available for the handles array, on output, the size in bytes of the handles array
/// @param Buffer     On output, the located handles array
/// @retval EFI_SUCCESS           The array of handles was returned
/// @retval EFI_NOT_FOUND         No handles match the search
/// @retval EFI_BUFFER_TOO_SMALL  The BufferSize is too small for the result, BufferSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER SearchType is not a member of EFI_LOCATE_SEARCH_TYPE
/// @retval EFI_INVALID_PARAMETER SearchType is ByRegisterNotify and SearchKey is NULL
/// @retval EFI_INVALID_PARAMETER SearchType is ByProtocol and Protocol is NULL
/// @retval EFI_INVALID_PARAMETER One or more matches are found and BufferSize is NULL
/// @retval EFI_INVALID_PARAMETER BufferSize is large enough for the result and Buffer is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCATE_HANDLE) (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID               *Protocol OPTIONAL,
  IN     VOID                   *SearchKey OPTIONAL,
  IN OUT UINTN                  *BufferSize,
  OUT    EFI_HANDLE             *Buffer
);
// EFI_LOCATE_DEVICE_PATH
/// Locates the handle to a device on the device path that supports the specified protocol
/// @param Protocol   The protocol for which to search
/// @param DevicePath On input, the device path, on output, the remaining part of the device path
/// @param Device     On output, the device handle
/// @retval EFI_SUCCESS           The resulting handle was returned
/// @retval EFI_NOT_FOUND         No handles matched the search
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER DevicePath is NULL
/// @retval EFI_INVALID_PARAMETER A handle matched the search and Device is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCATE_DEVICE_PATH) (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT    EFI_HANDLE                *Device
);
// EFI_INSTALL_CONFIGURATION_TABLE
/// Adds, updates, or removes a configuration table entry from the EFI System Table
/// @param Guid  The configuration table unique identifier for the entry to add, update, or remove
/// @param Table The configuration table for the entry to add or update, may be NULL to remove
/// @retval EFI_SUCCESS           The (Guid, Table) pair was added, updated, or removed
/// @retval EFI_INVALID_PARAMETER Guid is NULL
/// @retval EFI_NOT_FOUND         An attempt was made to delete a nonexistent entry
/// @retval EFI_OUT_OF_RESOURCES  There is not enough memory available to complete the operation
typedef
EFI_STATUS
(EFIAPI
*EFI_INSTALL_CONFIGURATION_TABLE) (
  IN EFI_GUID *Guid,
  IN VOID     *Table
);

//
// Image Services (EFI 1.0+)
//

// EFI_IMAGE_LOAD
/// Loads an EFI image into memory
/// @param BootPolicy        If TRUE, indicates that the request originates from the boot manager, and that the boot manager is attempting to load the device path as a boot selection, ignored if SourceBuffer is not NULL
/// @param ParentImageHandle The calling image handle
/// @param DevicePath        The DeviceHandle specific file path from which the image is loaded
/// @param SourceBuffer      If not NULL, a pointer to the memory location containing a copy of the image to be loaded
/// @param SourceSize        The size in bytes of SourceBuffer, ignored if SourceBuffer is NULL
/// @param ImageHandle       On output, the image handle that is created when the image is successfully loaded
/// @retval EFI_SUCCESS            Image was loaded into memory correctly
/// @retval EFI_NOT_FOUND          Both SourceBuffer and DevicePath are NULL
/// @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER  ImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_UNSUPPORTED        The image type is not supported
/// @retval EFI_OUT_OF_RESOURCES   Image was not loaded due to insufficient resources
/// @retval EFI_LOAD_ERROR         Image was not loaded because the image format was corrupt or not understood
/// @retval EFI_DEVICE_ERROR       Image was not loaded because the device returned a read error
/// @retval EFI_ACCESS_DENIED      Image was not loaded because the platform policy prohibits the image from being loaded. NULL is returned in *ImageHandle
/// @retval EFI_SECURITY_VIOLATION Image was loaded and an ImageHandle was created with a valid EFI_LOADED_IMAGE_PROTOCOL, however, the current platform policy specifies that the image should not be started
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_LOAD) (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle
);
// EFI_IMAGE_START
/// Transfers control to a loaded image’s entry point
/// @param ImageHandle  Handle of image to be started
/// @param ExitDataSize On output, the size, in bytes, of ExitData. If ExitData is NULL, then this parameter is ignored and the contents of ExitDataSize are not modified
/// @param ExitData     On output, a data buffer that includes a Null-terminated string, optionally followed by additional binary data.
///                      The string is a description that the caller may use to further indicate the reason for the image’s exit
/// @retval EFI_INVALID_PARAMETER  ImageHandle is either an invalid image handle or the image has already been initialized with StartImage 
/// @retval EFI_SECURITY_VIOLATION The current platform policy specifies that the image should not be started
/// @retval "Exit code from image" Exit code from image
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_START) (
  IN  EFI_HANDLE   ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16     **ExitData OPTIONAL
);
// EFI_EXIT
/// Terminates a loaded EFI image and returns control to boot services
/// @param ImageHandle  The handle that identifies the image that was passed to the image on entry
/// @param ExitStatus   The exit status
/// @param ExitDataSize The size, in bytes, of the exit data, ignored if ExitStatus is EFI_SUCCESS
/// @param ExitData     The data buffer that includes a null-terminated string, optionally followed by additional binary data. The string
///                      is a description that the caller may use to further indicate the reason for the image’s exit. ExitData is only
///                      valid if ExitStatus is something other than EFI_SUCCESS. The exit data buffer must be allocated from pool memory
/// @retval EFI_SUCCESS           The image specified by ImageHandle was unloaded. This condition only occurs for images that have been loaded with LoadImage() but have not been started with StartImage()
/// @retval EFI_INVALID_PARAMETER The image specified by ImageHandle has been loaded and started with LoadImage() and StartImage(), but the image is not the currently executing image
typedef
EFI_STATUS
(EFIAPI
*EFI_EXIT) (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16     *ExitData OPTIONAL
);
// EFI_IMAGE_UNLOAD
/// Unloads an image
/// @param ImageHandle Handle that identifies the image to be unloaded
/// @retval EFI_SUCCESS                     The image has been unloaded
/// @retval EFI_UNSUPPORTED                 The image has been started, and does not support unload
/// @retval EFI_INVALID_PARAMETER           ImageHandle is not a valid image handle
/// @retval "Exit code from unload handler" Exit code from the image unload function
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_UNLOAD) (
  IN EFI_HANDLE ImageHandle
);
// EFI_EXIT_BOOT_SERVICES
/// Terminates all boot services
/// @param ImageHandle Handle that identifies the exiting image
/// @param MapKey      Key to the latest memory map
/// @retval EFI_SUCCESS           Boot services have been terminated
/// @retval EFI_INVALID_PARAMETER MapKey is incorrect
typedef
EFI_STATUS
(EFIAPI
*EFI_EXIT_BOOT_SERVICES) (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey
);

//
// Miscellaneous Services (EFI 1.0+)
//

// EFI_GET_NEXT_MONOTONIC_COUNT
/// Returns a monotonically increasing count for the platform.
/// @param Count On output, the monotonic count
/// @retval EFI_SUCCESS The next monotonic count was returned
/// @retval EFI_DEVICE_ERROR The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER Count is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_NEXT_MONOTONIC_COUNT) (
  OUT UINT64 *Count
);
// EFI_STALL
/// Induces a fine-grained stall
/// @param Microseconds The number of microseconds to stall execution
/// @retval EFI_SUCCESS Execution was stalled at least the requested number of Microseconds
typedef
EFI_STATUS
(EFIAPI
*EFI_STALL) (
  IN UINTN Microseconds
);
// EFI_SET_WATCHDOG_TIMER
/// Sets the system’s watchdog timer
/// @param Timeout      The number of seconds to set the watchdog timer, zero disables the timer
/// @param WatchdogCode The numeric code to log on the watchdog timer timeout. The firmware reserves
///                     codes 0x0000 to 0xFFFF. Loaders and operating systems may use other timeout codes
/// @param DataSize     The size, in bytes, of WatchdogData
/// @param WatchdogData A data buffer that includes a Null-terminated string, optionally followed
///                      by additional binary data, the string is a description that the call may
///                      use to further indicate the reason to be logged with a watchdog event
/// @retval EFI_SUCCESS           The timeout has been set
/// @retval EFI_INVALID_PARAMETER The supplied WatchdogCode is invalid
/// @retval EFI_UNSUPPORTED       The system does not have a watchdog timer
/// @retval EFI_DEVICE_ERROR      The watch dog timer could not be programmed due to a hardware error
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_WATCHDOG_TIMER) (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16 *WatchdogData OPTIONAL
);

//
// DriverSupport Services (EFI 1.1+)
//

// EFI_CONNECT_CONTROLLER
/// Connects one or more drivers to a controller
/// @param ControllerHandle    The handle of the controller to which driver(s) are to be connected
/// @param DriverImageHandle   An ordered list of handles that support the EFI_DRIVER_BINDING_PROTOCOL, terminated by a NULL handle value. These handles are candidates for the
///                             Driver Binding Protocol(s) that will manage the controller specified by ControllerHandle. This parameter is typically used to debug new drivers
/// @param RemainingDevicePath The device path that specifies a child of the controller specified by ControllerHandle. If it is NULL, then handles for all the children of ControllerHandle
///                             will be created. This parameter is passed unchanged to the Supported() and Start() services of the EFI_DRIVER_BINDING_PROTOCOL attached to ControllerHandle
/// @param Recursive           If TRUE, then ConnectController() is called recursively until the entire tree of controllers below the controller specified by
///                             ControllerHandle have been created, if FALSE, then the tree of controllers is only expanded one level
/// @retval EFI_SUCCESS            One or more drivers were connected to ControllerHandle
/// @retval EFI_SUCCESS            No drivers were connected to ControllerHandle, but RemainingDevicePath is not NULL, and it is an End Device Path Node
/// @retval EFI_INVALID_PARAMETER  ControllerHandle is NULL
/// @retval EFI_NOT_FOUND          There are no EFI_DRIVER_BINDING_PROTOCOL instances present in the system
/// @retval EFI_NOT_FOUND          No drivers were connected to ControllerHandle
/// @retval EFI_SECURITY_VIOLATION The user has no permission to start UEFI device drivers on the device path associated with the ControllerHandle or specified by the RemainingDevicePath
typedef
EFI_STATUS
(EFIAPI
*EFI_CONNECT_CONTROLLER) (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE               *DriverImageHandle OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL,
  IN BOOLEAN                   Recursive
);
// EFI_DISCONNECT_CONTROLLER
/// Disconnects one or more drivers from a controller
/// @param ControllerHandle  The handle of the controller from which driver(s) are to be disconnected
/// @param DriverImageHandle The driver to disconnect from ControllerHandle, if NULL, then all the drivers currently managing ControllerHandle are disconnected from ControllerHandle
/// @param ChildHandle       The handle of the child to destroy, if NULL, then all the children of ControllerHandle are destroyed before the drivers are disconnected from ControllerHandle
/// @retval EFI_SUCCESS           One or more drivers were disconnected from the controller
/// @retval EFI_SUCCESS           On entry, no drivers are managing ControllerHandle
/// @retval EFI_SUCCESS           DriverImageHandle is not NULL, and on entry DriverImageHandle is not managing ControllerHandle
/// @retval EFI_INVALID_PARAMETER ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER DriverImageHandle is not NULL, and it is not a valid EFI_HANDLE
/// @retval EFI_INVALID_PARAMETER ChildHandle is not NULL, and it is not a valid EFI_HANDLE
/// @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to disconnect any drivers from ControllerHandle
/// @retval EFI_DEVICE_ERROR      The controller could not be disconnected because of a device error
/// @retval EFI_INVALID_PARAMETER DriverImageHandle does not support the EFI_DRIVER_BINDING_PROTOCOL
typedef
EFI_STATUS
(EFIAPI
*EFI_DISCONNECT_CONTROLLER) (
  IN EFI_HANDLE ControllerHandle,
  IN EFI_HANDLE DriverImageHandle OPTIONAL,
  IN EFI_HANDLE ChildHandle OPTIONAL
);

//
// Open and Close Protocol Services (EFI 1.1+)
//

// EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
/// Used in the implementation of EFI_BOOT_SERVICES.HandleProtocol(), since EFI_BOOT_SERVICES.OpenProtocol() performs the same function
///  as HandleProtocol() with additional functionality, HandleProtocol() can simply call OpenProtocol() with this Attributes value
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
// EFI_OPEN_PROTOCOL_GET_PROTOCOL
/// Used by a driver to get a protocol interface from a handle. Care must be taken when using this open mode because the
///  driver that opens a protocol interface in this manner will not be informed if the protocol interface is uninstalled
///  or reinstalled. The caller is also not required to close the protocol interface with EFI_BOOT_SERVICES.CloseProtocol()
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
// EFI_OPEN_PROTOCOL_TEST_PROTOCOL
/// Used by a driver to test for the existence of a protocol interface on a handle. Interface is optional for this attribute value, so it is ignored, and the caller should only use the return status code. The caller is also not required to close the protocol interface with CloseProtocol()
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
// EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
/// Used by bus drivers to show that a protocol interface is being used by one of the child controllers of a bus. This information
/// is used by the boot service EFI_BOOT_SERVICES.ConnectController() to recursively connect all child controllers and by the
/// boot service EFI_BOOT_SERVICES.DisconnectController() to get the list of child controllers that a bus driver created
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
// EFI_OPEN_PROTOCOL_BY_DRIVER
/// Used by a driver to gain access to a protocol interface. When this mode is used, the driver’s Stop() function will be called
/// by EFI_BOOT_SERVICES.DisconnectController() if the protocol interface is reinstalled or uninstalled. Once a protocol interface
/// is opened by a driver with this attribute, no other drivers will be allowed to open the same protocol interface with the BY_DRIVER
/// attribute. BY_DRIVER|EXCLUSIVEUsed by a driver to gain exclusive access to a protocol interface. If any other drivers have the
/// protocol interface opened with an attribute of BY_DRIVER, then an attempt will be made to remove them with DisconnectController()
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
// EFI_OPEN_PROTOCOL_EXCLUSIVE
/// Used by applications to gain exclusive access to a protocol interface. If any drivers have the protocol interface opened
///  with an attribute of BY_DRIVER, then an attempt will be made to remove them by calling the driver’s Stop() function
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

// EFI_OPEN_PROTOCOL_INFORMATION_ENTRY
/// Open protocol information entry
typedef struct EFI_OPEN_PROTOCOL_INFORMATION_ENTRY EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;
struct EFI_OPEN_PROTOCOL_INFORMATION_ENTRY {

  // AgentHandle
  /// The handle for the agent that opened the protocol interface
  EFI_HANDLE AgentHandle;
  // ControllerHandle
  /// The handle for the controller used to open the protocol interface
  EFI_HANDLE ControllerHandle;
  // Attributes
  /// The attributes used to open the protocol interface
  UINT32     Attributes;
  // OpenCount
  /// The count of times that the protocol interface has been opened by the agent specified by ImageHandle, ControllerHandle, and Attributes
  UINT32     OpenCount;

};

// EFI_OPEN_PROTOCOL
/// Queries a handle to determine if it supports a specified protocol. If the protocol
///  is supported by the handle, it opens the protocol on behalf of the calling agent
/// @param Handle           The handle for the protocol interface that is being opened
/// @param Protocol         The published unique identifier of the protocol
/// @param Interface        Supplies the address where a pointer to the corresponding Protocol Interface is returned
/// @param AgentHandle      The handle of the agent that is opening the protocol interface specified by Protocol and Interface. For agents
///                          that follow the UEFI Driver Model, this parameter is the handle that contains the EFI_DRIVER_BINDING_PROTOCOL
///                          instance that is produced by the UEFI driver that is opening the protocol interface. For UEFI applications, this
///                          is the image handle of the UEFI application that is opening the protocol interface. For applications that use
///                          HandleProtocol() to open a protocol interface, this parameter is the image handle of the EFI firmware
/// @param ControllerHandle If the agent that is opening a protocol is a driver that follows the UEFI Driver Model, then this
///                          parameter is the controller handle that requires the protocol interface. If the agent does not
///                          follow the UEFI Driver Model, then this parameter is optional and may be NULL
/// @param Attributes       The open mode of the protocol interface specified by Handle and Protocol
/// @retval EFI_SUCCESS           An item was added to the open list for the protocol interface, and the protocol interface was returned in Interface
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Interface is NULL, and Attributes is not TEST_PROTOCOL
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_UNSUPPORTED       Handle does not support Protocol
/// @retval EFI_INVALID_PARAMETER Attributes is not a legal value
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER|EXCLUSIVE and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is EXCLUSIVE and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER|EXCLUSIVE and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and Handle is identical to ControllerHandle
typedef
EFI_STATUS
(EFIAPI
*EFI_OPEN_PROTOCOL) (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface OPTIONAL,
  IN  EFI_HANDLE   AgentHandle,
  IN  EFI_HANDLE   ControllerHandle,
  IN  UINT32       Attributes
);
// EFI_CLOSE_PROTOCOL
/// Closes a protocol on a handle that was opened
/// @param Handle           The handle for the protocol interface to close
/// @param Protocol         The published unique identifier of the protocol
/// @param AgentHandle      The handle of the agent that is closing the protocol interface. For agents that follow the UEFI Driver Model, this
///                          parameter is the handle that contains the EFI_DRIVER_BINDING_PROTOCOL instance that is produced by the UEFI driver
///                          that is opening the protocol interface. For UEFI applications, this is the image handle of the UEFI application
/// @param ControllerHandle If the agent that opened a protocol is a driver that follows the UEFI Driver Model, then this parameter is the controller handle that
///                          required the protocol interface. If the agent does not follow the UEFI Driver Model, then this parameter is optional and may be NULL
/// @retval EFI_SUCCESS           The protocol instance was closed
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER ControllerHandle is not NULL and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_NOT_FOUND         Handle does not support the protocol specified by Protocol
/// @retval EFI_NOT_FOUND         The protocol interface specified by Handle and Protocol is not currently open by AgentHandle and ControllerHandle
typedef
EFI_STATUS
(EFIAPI
*EFI_CLOSE_PROTOCOL) (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
);
// EFI_OPEN_PROTOCOL_INFORMATION
/// Retrieves the list of agents that currently have a protocol interface opened
/// @param Handle      The handle for the protocol interface that is being queried.
/// @param Protocol    The published unique identifier of the protocol
/// @param EntryBuffer On output, an array of open protocol information entries, which must be freed
/// @param EntryCount  On output, the count of entries
/// @retval EFI_SUCCESS          The open protocol information was returned in EntryBuffer, and the count of entries was returned EntryCount
/// @retval EFI_NOT_FOUND        Handle does not support the protocol specified by Protocol
/// @retval EFI_OUT_OF_RESOURCES There are not enough resources available to allocate EntryBuffer
typedef
EFI_STATUS
(EFIAPI
*EFI_OPEN_PROTOCOL_INFORMATION) (
  IN  EFI_HANDLE                            Handle,
  IN  EFI_GUID                             *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN                                *EntryCount
);

//
// Library Services (EFI 1.1+)
//

// EFI_PROTOCOLS_PER_HANDLE
/// Retrieves the list of protocol interface GUIDs that are installed on a handle in a buffer allocated from pool
/// @param Handle              The handle from which to retrieve the list of unique protocol identifiers
/// @param ProtocolBuffer      On output, the list of unique protocol identifiers installed on the handle, which must be freed
/// @param ProtocolBufferCount On output, the count of unique protocol identifiers
/// @retval EFI_SUCCESS           The list of unique protocol identifiers installed on the handle was returned in ProtocolBuffer.
///                                The number of protocol interface GUIDs was returned in ProtocolBufferCount
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER ProtocolBuffer is NULL
/// @retval EFI_INVALID_PARAMETER ProtocolBufferCount is NULL
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the results
typedef
EFI_STATUS
(EFIAPI
*EFI_PROTOCOLS_PER_HANDLE) (
  IN  EFI_HANDLE    Handle,
  OUT EFI_GUID   ***ProtocolBuffer,
  OUT UINTN        *ProtocolBufferCount
);
// EFI_LOCATE_HANDLE_BUFFER
/// Returns an array of handles that support the requested protocol in a buffer allocated from pool
/// @param SearchType Specifies which handle(s) are to be returned
/// @param Protocol   Provides the protocol by which to search, only valid for a SearchType of ByProtocol
/// @param SearchKey  Supplies the search key depending on the SearchType
/// @param NoHandles  On output, the count of handles returned in Buffer
/// @param Buffer     On output, the buffer to return the requested array of handles that support Protocol, which must be freed
/// @retval EFI_SUCCESS           The array of handles was returned in Buffer, and the number of handles in Buffer was returned in NoHandles
/// @retval EFI_INVALID_PARAMETER NoHandles is NULL
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
/// @retval EFI_NOT_FOUND         No handles match the search
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCATE_HANDLE_BUFFER) (
  IN     EFI_LOCATE_SEARCH_TYPE   SearchType,
  IN     EFI_GUID                *Protocol OPTIONAL,
  IN     VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN                   *NoHandles,
  OUT    EFI_HANDLE             **Buffer
);
// EFI_LOCATE_PROTOCOL
/// Returns the first protocol instance that matches the given protocol
/// @param Protocol     Provides the protocol for which to search
/// @param Registration Protocol registration update key
/// @param Interface    On return, the first interface that matches Protocol and Registration
/// @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in Interface
/// @retval EFI_INVALID_PARAMETER Interface or Protocol is NULL
/// @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and Registration
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCATE_PROTOCOL) (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID     **Interface
);
// EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
/// Installs one or more protocol interfaces into the boot services environment
/// @param Handle On input, the handle on which to install the new protocol interfaces, or if NULL, on output, a newly allocated handle
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were installed
/// @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in the handle database
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle
typedef
EFI_STATUS
(EFIAPI
*EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
  IN OUT EFI_HANDLE *Handle,
  ...
);
// EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
/// Removes one or more protocol interfaces into the boot services environment
/// @param Handle The handle from which to remove the protocol interfaces
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were removed
/// @retval EFI_INVALID_PARAMETER One of the protocol interfaces was not previously installed on Handle
typedef
EFI_STATUS
(EFIAPI
*EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
  IN EFI_HANDLE Handle,
  ...
);

//
// 32-bit CRC Services (EFI 1.1+)
//

// EFI_CALCULATE_CRC32
/// Computes and returns a 32-bit CRC for a data buffer.
/// @param Data     The buffer on which the 32-bit CRC is to be computed
/// @param DataSize The number of bytes in the buffer Data
/// @param Crc32    On output, the 32-bit CRC that was computed for the data buffer specified by Data and DataSize
/// @retval EFI_SUCCESS           The 32-bit CRC was computed for the data buffer and returned in Crc32
/// @retval EFI_INVALID_PARAMETER Data is NULL
/// @retval EFI_INVALID_PARAMETER Crc32 is NULL
/// @retval EFI_INVALID_PARAMETER DataSize is 0
typedef
EFI_STATUS
(EFIAPI
*EFI_CALCULATE_CRC32) (
  IN  VOID   *Data,
  IN  UINTN   DataSize,
  OUT UINT32 *Crc32
);

//
// Memory Utility Services (EFI 1.1+)
//

// EFI_COPY_MEM
/// Copy the contents of one buffer to another buffer
/// @param Destination The destination buffer of the memory copy
/// @param Source      The source buffer of the memory copy
/// @param Length      The size in bytes to copy from Source to Destination
typedef
VOID
(EFIAPI
*EFI_COPY_MEM) (
  IN VOID  *Destination,
  IN VOID  *Source,
  IN UINTN  Length
);
// EFI_SET_MEM
/// Fill a buffer with a specified value
/// @param Buffer The buffer to fill
/// @param Size   The size in bytes of the Buffer to fill
/// @param Value  The value with which to fill Buffer
typedef
VOID
(EFIAPI
*EFI_SET_MEM) (
  IN VOID  *Buffer,
  IN UINTN  Size,
  IN UINT8  Value
);

//
// Extended Event Services (UEFI 2.0+)
//

// EFI_CREATE_EVENT_EX
/// Creates an event in a group
/// @param Type           The type of event to create and its mode and attributes
/// @param NotifyTpl      The task priority level of event notifications
/// @param NotifyFunction The event’s notification function
/// @param NotifyContext  The notification function context
/// @param EventGroup     The unique identifier of the group to which this event belongs
/// @param Event          On output, the newly created event
/// @retval EFI_SUCCESS           The event structure was created
/// @retval EFI_INVALID_PARAMETER One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Type has an unsupported bit set
/// @retval EFI_INVALID_PARAMETER Type has both EVT_NOTIFY_SIGNAL and EVT_NOTIFY_WAIT set
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyFunction is NULL
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyTpl is not a supported TPL level
/// @retval EFI_OUT_OF_RESOURCES  The event could not be allocated
typedef
EFI_STATUS
(EFIAPI
*EFI_CREATE_EVENT_EX) (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  CONST VOID       *NotifyContext OPTIONAL,
  IN  CONST EFI_GUID   *EventGroup OPTIONAL,
  OUT EFI_EVENT        *Event
);

//
// Boot Services Table
//

// EFI_BOOT_SERVICES_REVISION
/// The UEFI specification version to which the boot services table complies
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

// EFI_BOOT_SERVICES_SIGNATURE
/// The UEFI boot services table signature
#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544F4F42

// EFI_BOOT_SERVICES
/// Boot services
typedef struct EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
struct EFI_BOOT_SERVICES {

  // Hdr
  /// The table header
  EFI_TABLE_HEADER Hdr;

  //
  // Task Priority Services (EFI 1.0+)
  //

  // RaiseTPL
  /// Raises the task priority level
  EFI_RAISE_TPL   RaiseTPL;
  // RestoreTPL
  /// Restores/lowers the task priority level
  EFI_RESTORE_TPL RestoreTPL;

  //
  // Memory Services (EFI 1.0+)
  //

  // AllocatePages
  /// Allocates pages of a particular type
  EFI_ALLOCATE_PAGES AllocatePages;
  // FreePages
  /// Frees allocated pages
  EFI_FREE_PAGES     FreePages;
  // GetMemoryMap
  /// Returns the current boot services memory map and memory map key
  EFI_GET_MEMORY_MAP GetMemoryMap;
  // AllocatePool
  /// Allocates a pool of a particular type
  EFI_ALLOCATE_POOL  AllocatePool;
  // FreePool
  /// Frees allocated pool
  EFI_FREE_POOL      FreePool;

  //
  // Event and Timer Services (EFI 1.0+)
  //

  // CreateEvent
  /// Creates a general-purpose event structure
  EFI_CREATE_EVENT   CreateEvent;
  // SetTimer
  /// Sets an event to be signaled at a particular time
  EFI_SET_TIMER      SetTimer;
  // WaitForEvent
  /// Stops execution until an event is signaled
  EFI_WAIT_FOR_EVENT WaitForEvent;
  // SignalEvent
  /// Signals an event
  EFI_SIGNAL_EVENT   SignalEvent;
  // CloseEvent
  /// Closes and frees an event structure
  EFI_CLOSE_EVENT    CloseEvent;
  // CheckEvent
  /// Checks whether an event is in the signaled state
  EFI_CHECK_EVENT    CheckEvent;

  //
  // Protocol Handler Services (EFI 1.0+)
  //

  // InstallProtocolInterface
  /// Installs a protocol interface on a device handle
  EFI_INSTALL_PROTOCOL_INTERFACE    InstallProtocolInterface;
  // ReinstallProtocolInterface
  /// Reinstalls a protocol interface on a device handle
  EFI_REINSTALL_PROTOCOL_INTERFACE  ReinstallProtocolInterface;
  // UninstallProtocolInterface
  /// Removes a protocol interface from a device handle
  EFI_UNINSTALL_PROTOCOL_INTERFACE  UninstallProtocolInterface;
  // HandleProtocol
  /// Queries a handle to determine if it supports a specified protocol
  EFI_HANDLE_PROTOCOL               HandleProtocol;
  // Reserved
  /// Reserved
  VOID                             *Reserved;
  // RegisterProtocolNotify
  /// Registers an event that is to be signaled whenever an interface is installed for a specified protocol
  EFI_REGISTER_PROTOCOL_NOTIFY      RegisterProtocolNotify;
  // LocateHandle
  /// Returns an array of handles that support a specified protocol
  EFI_LOCATE_HANDLE                 LocateHandle;
  // LocateDevicePath
  /// Locates all devices on a device path that support a specified protocol and returns the handle to the device that is closest to the path
  EFI_LOCATE_DEVICE_PATH            LocateDevicePath;
  // InstallConfigurationTable
  /// Adds, updates, or removes a configuration table
  EFI_INSTALL_CONFIGURATION_TABLE   InstallConfigurationTable;

  //
  // Image Services (EFI 1.0+)
  //

  // LoadImage
  /// Loads an EFI image into memory
  EFI_IMAGE_LOAD         LoadImage;
  // StartImage
  /// Transfers control to a loaded image’s entry point
  EFI_IMAGE_START        StartImage;
  // Exit
  /// Exit the image entry point
  EFI_EXIT               Exit;
  // UnloadImage
  /// Unloads an image
  EFI_IMAGE_UNLOAD       UnloadImage;
  // ExitBootServices
  /// Terminates boot services
  EFI_EXIT_BOOT_SERVICES ExitBootServices;

  //
  // Miscellaneous Services (EFI 1.0+)
  //

  // GetNextMonotonicCount
  /// Returns a monotonically increasing count for the platform
  EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
  // Stall
  /// Stalls the processor
  EFI_STALL                    Stall;
  // SetWatchdogTimer
  /// Resets and sets a watchdog timer used during boot services time
  EFI_SET_WATCHDOG_TIMER       SetWatchdogTimer;

  //
  // DriverSupport Services (EFI 1.1+)
  //

  // ConnectController
  /// Uses a set of precedence rules to find the best set of drivers to manage a controller
  EFI_CONNECT_CONTROLLER    ConnectController;
  // DisconnectController
  /// Informs a set of drivers to stop managing a controller
  EFI_DISCONNECT_CONTROLLER DisconnectController;

  //
  // Open and Close Protocol Services (EFI 1.1+)
  //

  // OpenProtocol
  /// Adds elements to the list of agents consuming a protocol interface
  EFI_OPEN_PROTOCOL             OpenProtocol;
  // CloseProtocol
  /// Removes elements from the list of agents consuming a protocol interface
  EFI_CLOSE_PROTOCOL            CloseProtocol;
  // OpenProtocolInformation
  /// Retrieve the list of agents that are currently consuming a protocol interface
  EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;

  //
  // Library Services (EFI 1.1+)
  //

  // ProtocolsPerHandle
  /// Retrieves an automatically allocated list of protocols installed on a handle
  EFI_PROTOCOLS_PER_HANDLE                   ProtocolsPerHandle;
  // LocateHandleBuffer
  /// Retrieves an automatically allocated list of handles from the handle database that meet the search criteria
  EFI_LOCATE_HANDLE_BUFFER                   LocateHandleBuffer;
  // LocateProtocol
  /// Finds the first handle in the handle database the supports the requested protocol
  EFI_LOCATE_PROTOCOL                        LocateProtocol;
  // InstallMultipleProtocolInterfaces
  /// Installs one or more protocol interfaces onto a handle
  EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES   InstallMultipleProtocolInterfaces;
  // UninstallMultipleProtocolInterfaces
  /// Uninstalls one or more protocol interfaces from a handle
  EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;

  //
  // 32-bit CRC Services (EFI 1.1+)
  //

  // CalculateCrc32
  /// Computes and returns a 32-bit CRC for a data buffer
  EFI_CALCULATE_CRC32 CalculateCrc32;

  //
  // Memory Utility Services (EFI 1.1+)
  //

  // CopyMem
  /// Copies the contents of one buffer to another buffer
  EFI_COPY_MEM CopyMem;
  // SetMem
  /// Copies the contents of one buffer to another buffer
  EFI_SET_MEM  SetMem;

  //
  // Extended Event Services (UEFI 2.0+)
  //

  // CreateEventEx
  /// Creates an event structure as part of an event group
  EFI_CREATE_EVENT_EX CreateEventEx;

};

// EfiRaiseTPL
/// Raises a task’s priority level and returns its previous level
/// @param NewTpl The new task priority level that must be greater than or equal to the current task priority level
/// @return The previous task priority level
EXTERN
EFI_TPL
EFIAPI
EfiRaiseTPL (
  IN EFI_TPL NewTpl
);
// EfiRestoreTPL
/// Restores a task’s priority level to its previous value
/// @param OldTpl The previous task priority level to restore
EXTERN
VOID
EFIAPI
EfiRestoreTPL (
  IN EFI_TPL OldTpl
);

// EfiAllocateHighestPages
/// Allocates memory from the system in the highest available region
/// @param MemoryType The type of memory to allocate
/// @param Pages      The number of contiguous 4 KiB pages to allocate
/// @param Memory     On output, the address of the base of the page range that was allocated
/// @retval EFI_SUCCESS           The requested pages were allocated
/// @retval EFI_OUT_OF_RESOURCES  The pages could not be allocated
/// @retval EFI_INVALID_PARAMETER MemoryType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER MemoryType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Memory is NULL
#define EfiAllocateHighestPages(MemoryType, Pages, Memory) EfiDebugAllocateHighestPages(MemoryType, Pages, Memory, __FILE__, __LINE__)
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
EXTERN
EFI_STATUS
EFIAPI
EfiDebugAllocateHighestPages (
  IN  EFI_MEMORY_TYPE       MemoryType,
  IN  UINTN                 Pages,
  OUT EFI_PHYSICAL_ADDRESS *Memory,
  IN  CONST CHAR8          *Source,
  IN  UINTN                 LineNumber
);
// EfiAllocatePages
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
#define EfiAllocatePages(Type, MemoryType, Pages, Memory) EfiDebugAllocatePages(Type, MemoryType, Pages, Memory, __FILE__, __LINE__)
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
EXTERN
EFI_STATUS
EFIAPI
EfiDebugAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory,
  IN     CONST CHAR8          *Source,
  IN     UINTN                 LineNumber
);
// EfiFreePages
/// Frees memory pages
/// @param Memory The base physical address of the pages to be freed
/// @param Pages  The number of contiguous 4 KiB pages to free
/// @retval EFI_SUCCESS           The requested memory pages were freed
/// @retval EFI_NOT_FOUND         The requested memory pages were not allocated with AllocatePages()
/// @retval EFI_INVALID_PARAMETER Memory is not a page-aligned address or Pages is invalid
EXTERN
EFI_STATUS
EFIAPI
EfiFreePages (
  IN EFI_PHYSICAL_ADDRESS Memory,
  IN UINTN                Pages
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiGetMemoryMap (
  IN OUT UINTN                 *MemoryMapSize,
  IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
  OUT    UINTN                 *MapKey,
  OUT    UINTN                 *DescriptorSize,
  OUT    UINT32                *DescriptorVersion
);
// EfiGetMemoryMapBuffer
/// Allocates a buffer and returns the current memory map
/// @param MemoryMapSize     On output, the size in bytes of the memory map descriptors
/// @param MemoryMap         On output, the current memory map descriptors which was allocated and must be freed
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @retval EFI_SUCCESS           The memory map was returned in the MemoryMap buffer
/// @retval EFI_BUFFER_TOO_SMALL  The MemoryMap buffer was too small, the current buffer size needed to hold the memory map is returned in MemoryMapSize
/// @retval EFI_INVALID_PARAMETER MemoryMapSize is NULL
/// @retval EFI_INVALID_PARAMETER The MemoryMap buffer is not too small and MemoryMap is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiGetMemoryMapBuffer (
  OUT UINTN                  *MemoryMapSize,
  OUT EFI_MEMORY_DESCRIPTOR **MemoryMap,
  OUT UINTN                  *MapKey,
  OUT UINTN                  *DescriptorSize,
  OUT UINT32                 *DescriptorVersion
);
// EfiAllocateMemoryMap
/// Allocates a buffer and returns the current memory map
/// @param MemoryMapSize     On output, the size in bytes of the memory map descriptors
/// @param MapKey            On output, the key for the current memory map
/// @param DescriptorSize    On output, the size, in bytes, of an individual memory map descriptor
/// @param DescriptorVersion On output, the memory map descriptor version number
/// @return The current memory map descriptors which was allocated and must be freed or NULL
EXTERN
EFI_MEMORY_DESCRIPTOR *
EFIAPI
EfiAllocateMemoryMap (
  OUT UINTN  *MemoryMapSize,
  OUT UINTN  *MapKey,
  OUT UINTN  *DescriptorSize,
  OUT UINT32 *DescriptorVersion
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiGetNextMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  IN     UINTN                   MemoryMapSize,
  IN     UINTN                   DescriptorSize,
  IN OUT EFI_MEMORY_DESCRIPTOR **Descriptor
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiGetPreviousMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR  *MemoryMap,
  IN     UINTN                   MemoryMapSize,
  IN     UINTN                   DescriptorSize,
  IN OUT EFI_MEMORY_DESCRIPTOR **Descriptor
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiRemoveMemoryDescriptor (
  IN     EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN OUT UINTN                 *MemoryMapSize,
  IN     UINTN                  DescriptorSize,
  IN     EFI_MEMORY_DESCRIPTOR *Descriptor
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiByteSwapMemoryDescriptor (
  IN EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN EFI_MEMORY_DESCRIPTOR *Descriptor1,
  IN EFI_MEMORY_DESCRIPTOR *Descriptor2
);
// EfiPrintMemoryMap
/// Print the memory map descriptor information to the log
/// @param MemoryMap      The memory map descriptors
/// @param MemoryMapSize  The size in bytes of the memory map descriptors
/// @param DescriptorSize The size in bytes of an individual memory map descriptor
EXTERN
VOID
EFIAPI
EfiPrintMemoryMap (
  IN EFI_MEMORY_DESCRIPTOR *MemoryMap,
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize
);
// EfiPrintCurrentMemoryMap
/// Print the current memory map descriptor information to the log
EXTERN
VOID
EFIAPI
EfiPrintCurrentMemoryMap (
  VOID
);

// EfiAllocatePool
/// Allocates pool memory
/// @param PoolType The type of pool memory to allocate
/// @param Size     The size in bytes to allocate
/// @param Buffer   On output, the allocated pool memory
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
#define EfiAllocatePool(PoolType, Size, Buffer) EfiDebugAllocatePool(PoolType, Size, Buffer, __FILE__, __LINE__)
// EfiDebugAllocatePool
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
EXTERN
EFI_STATUS
EFIAPI
EfiDebugAllocatePool (
  IN  EFI_MEMORY_TYPE   PoolType,
  IN  UINTN             Size,
  OUT VOID            **Buffer,
  IN  CONST CHAR8      *Source,
  IN  UINTN             LineNumber
);
// EfiAllocate
/// Allocates from the default pool memory
/// @param Size The size in bytes to allocate
/// @return The allocated default pool memory
#define EfiAllocate(Size) EfiDebugAllocate((Size), __FILE__, __LINE__)
// EfiAllocateByType
/// Allocates from the default pool memory
/// @param TYPE The type of the memory to allocate
/// @return The allocated default pool memory
#define EfiAllocateByType(TYPE) (TYPE *)EfiAllocate(sizeof(TYPE))
// EfiAllocateArray
/// Allocates from the default pool memory
/// @param TYPE  The type of the memory to allocate
/// @param Count The count of items in the array
/// @return The allocated default pool memory
#define EfiAllocateArray(TYPE, Count) (TYPE *)EfiAllocate(sizeof(TYPE) * (Count))
// EfiDebugAllocate
/// Allocates from the default pool memory
/// @param Size       The size in bytes to allocate
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
EXTERN
VOID *
EFIAPI
EfiDebugAllocate (
  IN UINTN        Size,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
);
// EfiReallocatePool
/// Reallocates pool memory
/// @param PoolType The type of pool memory to allocate
/// @param Size     The size in bytes to allocate
/// @param OldSize  The size in bytes of the previous allocation to reallocate
/// @param Buffer   On input, the previously allocated memory or NULL, on output, the allocated pool memory
/// @retval EFI_SUCCESS           The requested number of bytes was allocated
/// @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated
/// @retval EFI_INVALID_PARAMETER PoolType is in the range EfiMaxMemoryType ... 0x6FFFFFFF
/// @retval EFI_INVALID_PARAMETER PoolType is EfiPersistentMemory
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
#define EfiReallocatePool(PoolType, Size, OldSize, Buffer) EfiDebugReallocatePool(PoolType, Size, OldSize, Buffer, __FILE__, __LINE__)
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
EXTERN
EFI_STATUS
EFIAPI
EfiDebugReallocatePool (
  IN     EFI_MEMORY_TYPE   PoolType,
  IN     UINTN             Size,
  IN     UINTN             OldSize,
  IN OUT VOID            **Buffer,
  IN     CONST CHAR8      *Source,
  IN     UINTN             LineNumber
);
// EfiReallocate
/// Reallocates from the default pool memory
/// @param Size The size in bytes to allocate
/// @param OldSize    The size in bytes of the previous allocation to reallocate
/// @param Buffer     The previously allocated memory or NULL
/// @return The allocated default pool memory
#define EfiReallocate(Size, OldSize, Buffer) EfiDebugReallocate(Size, OldSize, Buffer, __FILE__, __LINE__)
// EfiReallocateArray
/// Reallocates from the default pool memory
/// @param TYPE    The type of the memory to allocate
/// @param Size    The size in bytes to allocate
/// @param OldSize The size in bytes of the previous allocation to reallocate
/// @param Buffer  The previously allocated memory or NULL
/// @return The allocated default pool memory
#define EfiReallocateArray(TYPE, Size, OldSize, Buffer) (TYPE *)EfiReallocate(sizeof(TYPE) * (Size), sizeof(TYPE) * (OldSize), Buffer)
// EfiDebugReallocate
/// Reallocates from the default pool memory
/// @param Size       The size in bytes to allocate
/// @param OldSize    The size in bytes of the previous allocation to reallocate
/// @param Buffer     The previously allocated memory or NULL
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
EXTERN
VOID *
EFIAPI
EfiDebugReallocate (
  IN UINTN        Size,
  IN UINTN        OldSize,
  IN CONST VOID  *Buffer,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
);
// EfiDuplicate
/// Duplicates memory from the default pool memory
/// @param Size    The size in bytes to duplicate
/// @param Buffer  The previously allocated memory or NULL
/// @return The allocated default pool memory
#define EfiDuplicate(Size, Buffer) EfiDebugDuplicate(Size, Buffer, __FILE__, __LINE__)
// EfiDebugDuplicate
/// Duplicates memory from the default pool memory
/// @param Size       The size in bytes to duplicate
/// @param Buffer     The memory to duplicate
/// @param Source     The source code name where the allocation occurs
/// @param LineNumber The line number of the source code where the allocation occurs
/// @return The allocated default pool memory
EXTERN
VOID *
EFIAPI
EfiDebugDuplicate (
  IN UINTN        Size,
  IN CONST VOID  *Buffer,
  IN CONST CHAR8 *Source,
  IN UINTN        LineNumber
);
// EfiFreePool
/// Returns pool memory to the system
/// @param Buffer The pool memory allocation to free
/// @retval EFI_SUCCESS           The memory was returned to the system
/// @retval EFI_INVALID_PARAMETER Buffer was invalid
EXTERN
EFI_STATUS
EFIAPI
EfiFreePool (
  IN VOID *Buffer
);

// EfiCreateEvent
/// Creates an event
/// @param Type           The type of event to create and its mode and attributes
/// @param NotifyTpl      The task priority level of event notifications
/// @param NotifyFunction The event’s notification function
/// @param NotifyContext  The notification function context
/// @param Event          On output, the newly created event
/// @retval EFI_SUCCESS           The event structure was created
/// @retval EFI_INVALID_PARAMETER One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Type has an unsupported bit set
/// @retval EFI_INVALID_PARAMETER Type has both EVT_NOTIFY_SIGNAL and EVT_NOTIFY_WAIT set
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyFunction is NULL
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyTpl is not a supported TPL level
/// @retval EFI_OUT_OF_RESOURCES  The event could not be allocated
EXTERN
EFI_STATUS
EFIAPI
EfiCreateEvent (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  VOID             *NotifyContext OPTIONAL,
  OUT EFI_EVENT        *Event
);
// EfiSetTimer
/// Sets the type of timer and the trigger time for a timer event
/// @param Event       The timer event that is to be signaled at the specified time
/// @param Type        The type of time that is specified in TriggerTime
/// @param TriggerTime The number of 100ns units until the timer expires. If TriggerTime is 0, then if
///                     Type is TimerRelative the timer event will be signaled on the next timer tick,
///                     or if Type is TimerPeriodic the timer event will be signaled on every timer tick
/// @retval EFI_SUCCESS           The event has been set to be signaled at the requested time
/// @retval EFI_INVALID_PARAMETER Event or Type is not valid
EXTERN
EFI_STATUS
EFIAPI
EfiSetTimer (
  IN EFI_EVENT       Event,
  IN EFI_TIMER_DELAY Type,
  IN UINT64          TriggerTime
);
// EfiWaitForEvent
/// Stops execution until an event is signaled
/// @param NumberOfEvents The number of events on which to wait
/// @param Event          The events on which to wait
/// @param Index          On output, the index of the event which satisfied the wait condition
/// @retval EFI_SUCCESS           The event indicated by Index was signaled
/// @retval EFI_INVALID_PARAMETER NumberOfEvents is 0
/// @retval EFI_INVALID_PARAMETER The event indicated by Index is of type EVT_NOTIFY_SIGNAL
/// @retval EFI_UNSUPPORTED       The current TPL is not TPL_APPLICATION
EXTERN
EFI_STATUS
EFIAPI
EfiWaitForEvent (
  IN  UINTN      NumberOfEvents,
  IN  EFI_EVENT *Event,
  OUT UINTN     *Index
);
// EfiSignalEvent
/// Signals an event
/// @param Event The event to signal
/// @retval EFI_SUCCESS The event was signaled
EXTERN
EFI_STATUS
EFIAPI
EfiSignalEvent (
  IN EFI_EVENT Event
);
// EfiCloseEvent
/// Closes an event
/// @param Event The event to close
/// @retval EFI_SUCCESS The event has been closed
EXTERN
EFI_STATUS
EFIAPI
EfiCloseEvent (
  IN EFI_EVENT Event
);
// EfiCheckEvent
/// Checks whether an event is in the signaled state
/// @param Event The event to check
/// @retval EFI_SUCCESS           The event is in the signaled state
/// @retval EFI_NOT_READY         The event is not in the signaled state
/// @retval EFI_INVALID_PARAMETER Event is of type EVT_NOTIFY_SIGNAL
EXTERN
EFI_STATUS
EFIAPI
EfiCheckEvent (
  IN EFI_EVENT Event
);

// EfiInstallProtocolInterface
/// Installs a protocol interface on a device handle. If the handle does not
///  exist, it is created and added to the list of handles in the system
/// @param Handle        The handle on which the interface is to be installed, if *Handle
///                       is NULL on input, a new handle is created and returned on output
/// @param Protocol      The unique identifier of the protocol
/// @param InterfaceType The interface type
/// @param Interface     The protocol interface
/// @retval EFI_SUCCESS           The protocol interface was installed
/// @retval EFI_OUT_OF_RESOURCES  A new handle could not be allocated
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER InterfaceType is not EFI_NATIVE_INTERFACE
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle
EXTERN
EFI_STATUS
EFIAPI
EfiInstallProtocolInterface (
  IN OUT EFI_HANDLE         *Handle,
  IN     EFI_GUID           *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID               *Interface OPTIONAL
);
// EfiReinstallProtocolInterface
/// Reinstalls a protocol interface on a device handle
/// @param Handle The handle on which the interface is to be reinstalled
/// @param Protocol The unique identifier of the protocol
/// @param OldInterface The old protocol interface
/// @param NewInterface The new protocol interface
/// @retval EFI_SUCCESS           The protocol interface was reinstalled
/// @retval EFI_NOT_FOUND         The OldInterface on the handle was not found
/// @retval EFI_ACCESS_DENIED     The protocol interface could not be reinstalled, because OldInterface is still being used by a driver that will not release it
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiReinstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *OldInterface,
  IN VOID       *NewInterface
);
// EfiUninstallProtocolInterface
/// Removes a protocol interface from a device handle
/// @param Handle    The handle on which the interface was installed
/// @param Protocol  The unique identifier of the protocol
/// @param Interface The protocol interface
/// @retval EFI_SUCCESS           The interface was removed
/// @retval EFI_NOT_FOUND         The interface was not found
/// @retval EFI_ACCESS_DENIED     The interface was not removed because the interface is still being used by a driver
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiUninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *Interface
);
// EfiInstallMultipleProtocolInterfaces
/// Installs one or more protocol interfaces into the boot services environment
/// @param Handle On input, the handle on which to install the new protocol interfaces, or if NULL, on output, a newly allocated handle
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were installed
/// @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in the handle database
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle
EXTERN
EFI_STATUS
EFIAPI
EfiInstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE *Handle,
  ...
);
// EfiUninstallMultipleProtocolInterfaces
/// Removes one or more protocol interfaces into the boot services environment
/// @param Handle The handle from which to remove the protocol interfaces
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were removed
/// @retval EFI_INVALID_PARAMETER One of the protocol interfaces was not previously installed on Handle
EXTERN
EFI_STATUS
EFIAPI
EfiUninstallMultipleProtocolInterfaces (
  IN EFI_HANDLE Handle,
  ...
);
// EfiHandleProtocol
/// Queries a handle to determine if it supports a specified protocol
/// @param Handle    The handle being queried
/// @param Protocol  The unique identifier of the protocol
/// @param Interface On output, the protocol interface
/// @retval EFI_SUCCESS           The interface information for the specified protocol was returned
/// @retval EFI_UNSUPPORTED       The device does not support the specified protocol
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Interface is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiHandleProtocol (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface
);
// EfiRegisterProtocolNotify
/// Creates an event that is to be signaled whenever an interface is installed for a specified protocol
/// @param Protocol     The unique identifier of the protocol for which the event is to be registered
/// @param Event        The event that is to be signaled whenever a protocol interface is registered for the specified
///                      protocol, the same EFI_EVENT may be used for multiple protocol notify registrations
/// @param Registration On output, the registration identifier used to retrieve the list of handles that have added a protocol interface
/// @retval EFI_SUCCESS           The notification event has been registered
/// @retval EFI_OUT_OF_RESOURCES  The notification event could not be allocated
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Registration is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiRegisterProtocolNotify (
  IN  EFI_GUID   *Protocol,
  IN  EFI_EVENT   Event,
  OUT VOID      **Registration
);
// EfiLocateHandle
/// Returns an array of handles that support a specified protocol
/// @param SearchType The search type
/// @param Protocol   The unique identifier of the protocol for which to search
/// @param SearchKey  The registration identifier to search for new handles
/// @param BufferSize On input, the size in bytes available for the handles array, on output, the size in bytes of the handles array
/// @param Buffer     On output, the located handles array
/// @retval EFI_SUCCESS           The array of handles was returned
/// @retval EFI_NOT_FOUND         No handles match the search
/// @retval EFI_BUFFER_TOO_SMALL  The BufferSize is too small for the result, BufferSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER SearchType is not a member of EFI_LOCATE_SEARCH_TYPE
/// @retval EFI_INVALID_PARAMETER SearchType is ByRegisterNotify and SearchKey is NULL
/// @retval EFI_INVALID_PARAMETER SearchType is ByProtocol and Protocol is NULL
/// @retval EFI_INVALID_PARAMETER One or more matches are found and BufferSize is NULL
/// @retval EFI_INVALID_PARAMETER BufferSize is large enough for the result and Buffer is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiLocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID               *Protocol OPTIONAL,
  IN     VOID                   *SearchKey OPTIONAL,
  IN OUT UINTN                  *BufferSize,
  OUT    EFI_HANDLE             *Buffer
);
// EfiLocateDevicePath
/// Locates the handle to a device on the device path that supports the specified protocol
/// @param Protocol   The protocol for which to search
/// @param DevicePath On input, the device path, on output, the remaining part of the device path
/// @param Device     On output, the device handle
/// @retval EFI_SUCCESS           The resulting handle was returned
/// @retval EFI_NOT_FOUND         No handles matched the search
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER DevicePath is NULL
/// @retval EFI_INVALID_PARAMETER A handle matched the search and Device is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiLocateDevicePath (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT    EFI_HANDLE                *Device
);
// EfiInstallConfigurationTable
/// Adds, updates, or removes a configuration table entry from the EFI System Table
/// @param Guid  The configuration table unique identifier for the entry to add, update, or remove
/// @param Table The configuration table for the entry to add or update, may be NULL to remove
/// @retval EFI_SUCCESS           The (Guid, Table) pair was added, updated, or removed
/// @retval EFI_INVALID_PARAMETER Guid is NULL
/// @retval EFI_NOT_FOUND         An attempt was made to delete a nonexistent entry
/// @retval EFI_OUT_OF_RESOURCES  There is not enough memory available to complete the operation
EXTERN
EFI_STATUS
EFIAPI
EfiInstallConfigurationTable (
  IN EFI_GUID *Guid,
  IN VOID     *Table
);

// EfiLoadImage
/// Loads an EFI image into memory
/// @param BootPolicy        If TRUE, indicates that the request originates from the boot manager, and that the boot manager is attempting to load the device path as a boot selection, ignored if SourceBuffer is not NULL
/// @param ParentImageHandle The calling image handle
/// @param DevicePath        The DeviceHandle specific file path from which the image is loaded
/// @param SourceBuffer      If not NULL, a pointer to the memory location containing a copy of the image to be loaded
/// @param SourceSize        The size in bytes of SourceBuffer, ignored if SourceBuffer is NULL
/// @param ImageHandle       On output, the image handle that is created when the image is successfully loaded
/// @retval EFI_SUCCESS            Image was loaded into memory correctly
/// @retval EFI_NOT_FOUND          Both SourceBuffer and DevicePath are NULL
/// @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER  ImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_INVALID_PARAMETER  ParentImageHandle is NULL
/// @retval EFI_UNSUPPORTED        The image type is not supported
/// @retval EFI_OUT_OF_RESOURCES   Image was not loaded due to insufficient resources
/// @retval EFI_LOAD_ERROR         Image was not loaded because the image format was corrupt or not understood
/// @retval EFI_DEVICE_ERROR       Image was not loaded because the device returned a read error
/// @retval EFI_ACCESS_DENIED      Image was not loaded because the platform policy prohibits the image from being loaded. NULL is returned in *ImageHandle
/// @retval EFI_SECURITY_VIOLATION Image was loaded and an ImageHandle was created with a valid EFI_LOADED_IMAGE_PROTOCOL, however, the current platform policy specifies that the image should not be started
EXTERN
EFI_STATUS
EFIAPI
EfiLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle
);
// EfiStartImage
/// Transfers control to a loaded image’s entry point
/// @param ImageHandle  Handle of image to be started
/// @param ExitDataSize On output, the size, in bytes, of ExitData. If ExitData is NULL, then this parameter is ignored and the contents of ExitDataSize are not modified
/// @param ExitData     On output, a data buffer that includes a Null-terminated string, optionally followed by additional binary data.
///                      The string is a description that the caller may use to further indicate the reason for the image’s exit
/// @retval EFI_INVALID_PARAMETER  ImageHandle is either an invalid image handle or the image has already been initialized with StartImage 
/// @retval EFI_SECURITY_VIOLATION The current platform policy specifies that the image should not be started
/// @retval "Exit code from image" Exit code from image
EXTERN
EFI_STATUS
EFIAPI
EfiStartImage (
  IN  EFI_HANDLE   ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16     **ExitData OPTIONAL
);
// EfiExit
/// Terminates a loaded EFI image and returns control to boot services
/// @param ImageHandle  The handle that identifies the image that was passed to the image on entry
/// @param ExitStatus   The exit status
/// @param ExitDataSize The size, in bytes, of the exit data, ignored if ExitStatus is EFI_SUCCESS
/// @param ExitData     The data buffer that includes a null-terminated string, optionally followed by additional binary data. The string
///                      is a description that the caller may use to further indicate the reason for the image’s exit. ExitData is only
///                      valid if ExitStatus is something other than EFI_SUCCESS. The exit data buffer must be allocated from pool memory
/// @retval EFI_SUCCESS           The image specified by ImageHandle was unloaded. This condition only occurs for images that have been loaded with LoadImage() but have not been started with StartImage()
/// @retval EFI_INVALID_PARAMETER The image specified by ImageHandle has been loaded and started with LoadImage() and StartImage(), but the image is not the currently executing image
EXTERN
EFI_STATUS
EFIAPI
EfiExit (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16     *ExitData OPTIONAL
);
// EfiUnloadImage
/// Unloads an image
/// @param ImageHandle Handle that identifies the image to be unloaded
/// @retval EFI_SUCCESS                     The image has been unloaded
/// @retval EFI_UNSUPPORTED                 The image has been started, and does not support unload
/// @retval EFI_INVALID_PARAMETER           ImageHandle is not a valid image handle
/// @retval "Exit code from unload handler" Exit code from the image unload function
EXTERN
EFI_STATUS
EFIAPI
EfiUnloadImage (
  IN EFI_HANDLE ImageHandle
);
// EfiExitBootServices
/// Terminates all boot services
/// @param ImageHandle Handle that identifies the exiting image
/// @param MapKey      Key to the latest memory map
/// @retval EFI_SUCCESS           Boot services have been terminated
/// @retval EFI_INVALID_PARAMETER MapKey is incorrect
EXTERN
EFI_STATUS
EFIAPI
EfiExitBootServices (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey
);

// EFI_IMAGE_LOAD_CALLBACK
/// Image load callback
/// @param DevicePath The device path of the image to load
/// @param Data       The data used when registering the callback
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_LOAD_CALLBACK) (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN VOID                     *Data OPTIONAL
);
// EFI_IMAGE_CALLBACK
/// Image callback
/// @param ImageHandle The image handle
/// @param Data        The data used when registering the callback
typedef
EFI_STATUS
(EFIAPI
*EFI_IMAGE_CALLBACK) (
  IN EFI_HANDLE  ImageHandle,
  IN VOID       *Data OPTIONAL
);

// EfiImageLoadCallback
/// Register an image load callback
/// @param Callback     The callback function to call when an image is being loaded
/// @param CallbackData The data to pass to the callback function
EXTERN
EFI_STATUS
EFIAPI
EfiImageLoadCallback (
  IN EFI_IMAGE_LOAD_CALLBACK  Callback,
  IN VOID                    *CallbackData OPTIONAL
);
// EfiImageStartCallback
/// Register an image start callback
/// @param Callback     The callback function to call when an image is started
/// @param CallbackData The data to pass to the callback function
EXTERN
EFI_STATUS
EFIAPI
EfiImageStartCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
);
// EfiImageExitCallback
/// Register an image exit callback
/// @param Callback     The callback function to call when this image is exited
/// @param CallbackData The data to pass to the callback function
EXTERN
EFI_STATUS
EFIAPI
EfiImageExitCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
);
// EfiImageUnloadCallback
/// Register an image unload callback for this image
/// @param Callback     The callback function to call when this image is unloaded
/// @param CallbackData The data to pass to the callback function
EXTERN
EFI_STATUS
EFIAPI
EfiImageUnloadCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
);
// EfiExitBootServicesCallback
/// Register an exit boot services callback
/// @param Callback     The callback function to call when exit boot services is called
/// @param CallbackData The data to pass to the callback function
EXTERN
EFI_STATUS
EFIAPI
EfiExitBootServicesCallback (
  IN EFI_IMAGE_CALLBACK  Callback,
  IN VOID               *CallbackData OPTIONAL
);

// EfiGetNextMonotonicCount
/// Returns a monotonically increasing count for the platform.
/// @param Count On output, the monotonic count
/// @retval EFI_SUCCESS The next monotonic count was returned
/// @retval EFI_DEVICE_ERROR The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER Count is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiGetNextMonotonicCount (
  OUT UINT64 *Count
);
// EfiStall
/// Induces a fine-grained stall
/// @param Microseconds The number of microseconds to stall execution
/// @retval EFI_SUCCESS Execution was stalled at least the requested number of Microseconds
EXTERN
EFI_STATUS
EFIAPI
EfiStall (
  IN UINTN Microseconds
);
// EfiSetWatchdogTimer
/// Sets the system’s watchdog timer
/// @param Timeout      The number of seconds to set the watchdog timer, zero disables the timer
/// @param WatchdogCode The numeric code to log on the watchdog timer timeout. The firmware reserves
///                     codes 0x0000 to 0xFFFF. Loaders and operating systems may use other timeout codes
/// @param DataSize     The size, in bytes, of WatchdogData
/// @param WatchdogData A data buffer that includes a Null-terminated string, optionally followed
///                      by additional binary data, the string is a description that the call may
///                      use to further indicate the reason to be logged with a watchdog event
/// @retval EFI_SUCCESS           The timeout has been set
/// @retval EFI_INVALID_PARAMETER The supplied WatchdogCode is invalid
/// @retval EFI_UNSUPPORTED       The system does not have a watchdog timer
/// @retval EFI_DEVICE_ERROR      The watch dog timer could not be programmed due to a hardware error
EXTERN
EFI_STATUS
EFIAPI
EfiSetWatchdogTimer (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16 *WatchdogData OPTIONAL
);

// EfiConnectController
/// Connects one or more drivers to a controller
/// @param ControllerHandle    The handle of the controller to which driver(s) are to be connected
/// @param DriverImageHandle   An ordered list of handles that support the EFI_DRIVER_BINDING_PROTOCOL, terminated by a NULL handle value. These handles are candidates for the
///                             Driver Binding Protocol(s) that will manage the controller specified by ControllerHandle. This parameter is typically used to debug new drivers
/// @param RemainingDevicePath The device path that specifies a child of the controller specified by ControllerHandle. If it is NULL, then handles for all the children of ControllerHandle
///                             will be created. This parameter is passed unchanged to the Supported() and Start() services of the EFI_DRIVER_BINDING_PROTOCOL attached to ControllerHandle
/// @param Recursive           If TRUE, then ConnectController() is called recursively until the entire tree of controllers below the controller specified by
///                             ControllerHandle have been created, if FALSE, then the tree of controllers is only expanded one level
/// @retval EFI_SUCCESS            One or more drivers were connected to ControllerHandle
/// @retval EFI_SUCCESS            No drivers were connected to ControllerHandle, but RemainingDevicePath is not NULL, and it is an End Device Path Node
/// @retval EFI_INVALID_PARAMETER  ControllerHandle is NULL
/// @retval EFI_NOT_FOUND          There are no EFI_DRIVER_BINDING_PROTOCOL instances present in the system
/// @retval EFI_NOT_FOUND          No drivers were connected to ControllerHandle
/// @retval EFI_SECURITY_VIOLATION The user has no permission to start UEFI device drivers on the device path associated with the ControllerHandle or specified by the RemainingDevicePath
EXTERN
EFI_STATUS
EFIAPI
EfiConnectController (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE               *DriverImageHandle OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL,
  IN BOOLEAN                   Recursive
);
// EfiDisconnectController
/// Disconnects one or more drivers from a controller
/// @param ControllerHandle  The handle of the controller from which driver(s) are to be disconnected
/// @param DriverImageHandle The driver to disconnect from ControllerHandle, if NULL, then all the drivers currently managing ControllerHandle are disconnected from ControllerHandle
/// @param ChildHandle       The handle of the child to destroy, if NULL, then all the children of ControllerHandle are destroyed before the drivers are disconnected from ControllerHandle
/// @retval EFI_SUCCESS           One or more drivers were disconnected from the controller
/// @retval EFI_SUCCESS           On entry, no drivers are managing ControllerHandle
/// @retval EFI_SUCCESS           DriverImageHandle is not NULL, and on entry DriverImageHandle is not managing ControllerHandle
/// @retval EFI_INVALID_PARAMETER ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER DriverImageHandle is not NULL, and it is not a valid EFI_HANDLE
/// @retval EFI_INVALID_PARAMETER ChildHandle is not NULL, and it is not a valid EFI_HANDLE
/// @retval EFI_OUT_OF_RESOURCES  There are not enough resources available to disconnect any drivers from ControllerHandle
/// @retval EFI_DEVICE_ERROR      The controller could not be disconnected because of a device error
/// @retval EFI_INVALID_PARAMETER DriverImageHandle does not support the EFI_DRIVER_BINDING_PROTOCOL
EXTERN
EFI_STATUS
EFIAPI
EfiDisconnectController (
  IN EFI_HANDLE ControllerHandle,
  IN EFI_HANDLE DriverImageHandle OPTIONAL,
  IN EFI_HANDLE ChildHandle OPTIONAL
);

// EfiOpenProtocol
/// Queries a handle to determine if it supports a specified protocol. If the protocol
///  is supported by the handle, it opens the protocol on behalf of the calling agent
/// @param Handle           The handle for the protocol interface that is being opened
/// @param Protocol         The published unique identifier of the protocol
/// @param Interface        Supplies the address where a pointer to the corresponding Protocol Interface is returned
/// @param AgentHandle      The handle of the agent that is opening the protocol interface specified by Protocol and Interface. For agents
///                          that follow the UEFI Driver Model, this parameter is the handle that contains the EFI_DRIVER_BINDING_PROTOCOL
///                          instance that is produced by the UEFI driver that is opening the protocol interface. For UEFI applications, this
///                          is the image handle of the UEFI application that is opening the protocol interface. For applications that use
///                          HandleProtocol() to open a protocol interface, this parameter is the image handle of the EFI firmware
/// @param ControllerHandle If the agent that is opening a protocol is a driver that follows the UEFI Driver Model, then this
///                          parameter is the controller handle that requires the protocol interface. If the agent does not
///                          follow the UEFI Driver Model, then this parameter is optional and may be NULL
/// @param Attributes       The open mode of the protocol interface specified by Handle and Protocol
/// @retval EFI_SUCCESS           An item was added to the open list for the protocol interface, and the protocol interface was returned in Interface
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_INVALID_PARAMETER Interface is NULL, and Attributes is not TEST_PROTOCOL
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_UNSUPPORTED       Handle does not support Protocol
/// @retval EFI_INVALID_PARAMETER Attributes is not a legal value
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER|EXCLUSIVE and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is EXCLUSIVE and AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_DRIVER|EXCLUSIVE and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Attributes is BY_CHILD_CONTROLLER and Handle is identical to ControllerHandle
EXTERN
EFI_STATUS
EFIAPI
EfiOpenProtocol (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface OPTIONAL,
  IN  EFI_HANDLE   AgentHandle,
  IN  EFI_HANDLE   ControllerHandle,
  IN  UINT32       Attributes
);
// EfiCloseProtocol
/// Closes a protocol on a handle that was opened
/// @param Handle           The handle for the protocol interface to close
/// @param Protocol         The published unique identifier of the protocol
/// @param AgentHandle      The handle of the agent that is closing the protocol interface. For agents that follow the UEFI Driver Model, this
///                          parameter is the handle that contains the EFI_DRIVER_BINDING_PROTOCOL instance that is produced by the UEFI driver
///                          that is opening the protocol interface. For UEFI applications, this is the image handle of the UEFI application
/// @param ControllerHandle If the agent that opened a protocol is a driver that follows the UEFI Driver Model, then this parameter is the controller handle that
///                          required the protocol interface. If the agent does not follow the UEFI Driver Model, then this parameter is optional and may be NULL
/// @retval EFI_SUCCESS           The protocol instance was closed
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER AgentHandle is NULL
/// @retval EFI_INVALID_PARAMETER ControllerHandle is not NULL and ControllerHandle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is NULL
/// @retval EFI_NOT_FOUND         Handle does not support the protocol specified by Protocol
/// @retval EFI_NOT_FOUND         The protocol interface specified by Handle and Protocol is not currently open by AgentHandle and ControllerHandle
EXTERN
EFI_STATUS
EFIAPI
EfiCloseProtocol (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
);
// EfiOpenProtocolInformation
/// Retrieves the list of agents that currently have a protocol interface opened
/// @param Handle      The handle for the protocol interface that is being queried.
/// @param Protocol    The published unique identifier of the protocol
/// @param EntryBuffer On output, an array of open protocol information entries, which must be freed
/// @param EntryCount  On output, the count of entries
/// @retval EFI_SUCCESS          The open protocol information was returned in EntryBuffer, and the count of entries was returned EntryCount
/// @retval EFI_NOT_FOUND        Handle does not support the protocol specified by Protocol
/// @retval EFI_OUT_OF_RESOURCES There are not enough resources available to allocate EntryBuffer
EXTERN
EFI_STATUS
EFIAPI
EfiOpenProtocolInformation (
  IN  EFI_HANDLE                            Handle,
  IN  EFI_GUID                             *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN                                *EntryCount
);

// EfiProtocolsPerHandle
/// Retrieves the list of protocol interface GUIDs that are installed on a handle in a buffer allocated from pool
/// @param Handle              The handle from which to retrieve the list of unique protocol identifiers
/// @param ProtocolBuffer      On output, the list of unique protocol identifiers installed on the handle, which must be freed
/// @param ProtocolBufferCount On output, the count of unique protocol identifiers
/// @retval EFI_SUCCESS           The list of unique protocol identifiers installed on the handle was returned in ProtocolBuffer.
///                                The number of protocol interface GUIDs was returned in ProtocolBufferCount
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER ProtocolBuffer is NULL
/// @retval EFI_INVALID_PARAMETER ProtocolBufferCount is NULL
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the results
EXTERN
EFI_STATUS
EFIAPI
EfiProtocolsPerHandle (
  IN  EFI_HANDLE    Handle,
  OUT EFI_GUID   ***ProtocolBuffer,
  OUT UINTN        *ProtocolBufferCount
);
// EfiLocateHandleBuffer
/// Returns an array of handles that support the requested protocol in a buffer allocated from pool
/// @param SearchType Specifies which handle(s) are to be returned
/// @param Protocol   Provides the protocol by which to search, only valid for a SearchType of ByProtocol
/// @param SearchKey  Supplies the search key depending on the SearchType
/// @param NoHandles  On output, the count of handles returned in Buffer
/// @param Buffer     On output, the buffer to return the requested array of handles that support Protocol, which must be freed
/// @retval EFI_SUCCESS           The array of handles was returned in Buffer, and the number of handles in Buffer was returned in NoHandles
/// @retval EFI_INVALID_PARAMETER NoHandles is NULL
/// @retval EFI_INVALID_PARAMETER Buffer is NULL
/// @retval EFI_NOT_FOUND         No handles match the search
/// @retval EFI_OUT_OF_RESOURCES  There is not enough pool memory to store the matching results
EXTERN
EFI_STATUS
EFIAPI
EfiLocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE   SearchType,
  IN     EFI_GUID                *Protocol OPTIONAL,
  IN     VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN                   *NoHandles,
  OUT    EFI_HANDLE             **Buffer
);
// EfiLocateProtocol
/// Returns the first protocol instance that matches the given protocol
/// @param Protocol     Provides the protocol for which to search
/// @param Registration Protocol registration update key
/// @param Interface    On return, the first interface that matches Protocol and Registration
/// @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in Interface
/// @retval EFI_INVALID_PARAMETER Interface or Protocol is NULL
/// @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and Registration
EXTERN
EFI_STATUS
EFIAPI
EfiLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID     **Interface
);

// EfiCalculateCrc32
/// Computes and returns a 32-bit CRC for a data buffer.
/// @param Data     The buffer on which the 32-bit CRC is to be computed
/// @param DataSize The number of bytes in the buffer Data
/// @param Crc32    On output, the 32-bit CRC that was computed for the data buffer specified by Data and DataSize
/// @retval EFI_SUCCESS           The 32-bit CRC was computed for the data buffer and returned in Crc32
/// @retval EFI_INVALID_PARAMETER Data is NULL
/// @retval EFI_INVALID_PARAMETER Crc32 is NULL
/// @retval EFI_INVALID_PARAMETER DataSize is 0
EXTERN
EFI_STATUS
EFIAPI
EfiCalculateCrc32 (
  IN  VOID   *Data,
  IN  UINTN   DataSize,
  OUT UINT32 *Crc32
);

// EfiCopy
/// Copy the contents of one array to another
/// @param TYPE        The type of the array items
/// @param Destination The destination array of the memory copy
/// @param Source      The source array of the memory copy
#define EfiCopy(TYPE, Destination, Source) EfiCopyMem((VOID *)(Destination), (VOID *)(Source), sizeof(TYPE))
// EfiCopyArray
/// Copy the contents of one array to another
/// @param TYPE        The type of the array items
/// @param Destination The destination array of the memory copy
/// @param Source      The source array of the memory copy
/// @param Length      The count of items to copy from Source to Destination
#define EfiCopyArray(TYPE, Destination, Source, Length) EfiCopyMem((VOID *)(Destination), (VOID *)(Source), (Length) * sizeof(TYPE))
// EfiCopyMem
/// Copy the contents of one buffer to another buffer
/// @param Destination The destination buffer of the memory copy
/// @param Source      The source buffer of the memory copy
/// @param Length      The size in bytes to copy from Source to Destination
EXTERN
VOID
EFIAPI
EfiCopyMem (
  IN VOID       *Destination,
  IN CONST VOID *Source,
  IN UINTN       Length
);
// EfiSetMem
/// Fill a buffer with a specified value
/// @param Buffer The buffer to fill
/// @param Size   The size in bytes of the Buffer to fill
/// @param Value  The value with which to fill Buffer
EXTERN
VOID
EFIAPI
EfiSetMem (
  IN VOID  *Buffer,
  IN UINTN  Size,
  IN UINT8  Value
);
// EfiZero
/// Zero a type object
/// @param TYPE  The type of the array items
/// @param Object The type object to zero
#define EfiZero(TYPE, Object) EfiZeroMem(Object, sizeof(TYPE))
// EfiZeroArray
/// Zero an array
/// @param TYPE  The type of the array items
/// @param Array The array to zero
/// @param Count The count of items to zero
#define EfiZeroArray(TYPE, Array, Count) EfiZeroMem(Array, (Count) * sizeof(TYPE))
// EfiZeroMem
/// Zero a buffer
/// @param Buffer The buffer to zero
/// @param Size   The size in bytes of the buffer
EXTERN
VOID
EFIAPI
EfiZeroMem (
  IN VOID  *Buffer,
  IN UINTN  Size
);

// EfiCreateEventEx
/// Creates an event in a group
/// @param Type           The type of event to create and its mode and attributes
/// @param NotifyTpl      The task priority level of event notifications
/// @param NotifyFunction The event’s notification function
/// @param NotifyContext  The notification function context
/// @param EventGroup     The unique identifier of the group to which this event belongs
/// @param Event          On output, the newly created event
/// @retval EFI_SUCCESS           The event structure was created
/// @retval EFI_INVALID_PARAMETER One of the parameters has an invalid value
/// @retval EFI_INVALID_PARAMETER Event is NULL
/// @retval EFI_INVALID_PARAMETER Type has an unsupported bit set
/// @retval EFI_INVALID_PARAMETER Type has both EVT_NOTIFY_SIGNAL and EVT_NOTIFY_WAIT set
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyFunction is NULL
/// @retval EFI_INVALID_PARAMETER Type has either EVT_NOTIFY_SIGNAL or EVT_NOTIFY_WAIT set and NotifyTpl is not a supported TPL level
/// @retval EFI_OUT_OF_RESOURCES  The event could not be allocated
EXTERN
EFI_STATUS
EFIAPI
EfiCreateEventEx (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  CONST VOID       *NotifyContext OPTIONAL,
  IN  CONST EFI_GUID   *EventGroup OPTIONAL,
  OUT EFI_EVENT        *Event
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_BOOT_SERVICES_HEADER__
