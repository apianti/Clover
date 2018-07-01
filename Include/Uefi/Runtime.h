///
/// @file Include/Uefi/Runtime.h
///
/// UEFI implementation runtime services table
///

#pragma once
#ifndef __UEFI_RUNTIME_SERVICES_HEADER__
#define __UEFI_RUNTIME_SERVICES_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//
// Time Services (EFI 1.0+)
//

// EFI_TIME_CAPABILITIES
/// The capabilities of the real time clock device
typedef struct EFI_TIME_CAPABILITIES EFI_TIME_CAPABILITIES;
struct EFI_TIME_CAPABILITIES {

  // Resolution
  /// Provides the reporting resolution of the real-time clock device in counts per second
  UINT32  Resolution;
  // Accuracy
  /// Provides the timekeeping accuracy of the real-time clock in an error rate of 1E-6 parts per million.
  ///  For a clock with an accuracy of 50 parts per million, the value in this field would be 50,000,000
  UINT32  Accuracy;
  // SetsToZero
  /// A TRUE indicates that a time set operation clears the device’s time below the Resolution reporting level.
  ///  A FALSE indicates that the state below the Resolution level of the device is not cleared when the time is set
  BOOLEAN SetsToZero;

};

// EFI_GET_TIME
/// Returns the current time and date information, and the time-keeping capabilities of the hardware platform
/// @param Time         On output, the current time
/// @param Capabilities On output, the real time clock device capabilities
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER Time is NULL
/// @retval EFI_DEVICE_ERROR      The time could not be retrieved due to a hardware error
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_TIME) (
  OUT EFI_TIME              *Time,
  OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
);
// EFI_SET_TIME
/// Sets the current local time and date information.
/// @param Time The time to set as current time
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The time could not be set due to a hardware error
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_TIME) (
  IN EFI_TIME *Time
);
// EFI_GET_WAKEUP_TIME
/// Returns the current wakeup alarm clock settings
/// @param Enabled Indicates if the alarm is currently enabled or disabled
/// @param Pending Indicates if the alarm signal is pending and requires acknowledgement
/// @param Time    The current alarm time
/// @retval EFI_SUCCESS           The alarm settings were returned
/// @retval EFI_INVALID_PARAMETER Enabled is NULL
/// @retval EFI_INVALID_PARAMETER Pending is NULL
/// @retval EFI_INVALID_PARAMETER Time is NULL
/// @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error
/// @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_WAKEUP_TIME) (
  OUT BOOLEAN  *Enabled,
  OUT BOOLEAN  *Pending,
  OUT EFI_TIME *Time
);
// EFI_SET_WAKEUP_TIME
/// Sets the system wakeup alarm clock time
/// @param Enable Enable or disable the wakeup alarm
/// @param Time   If Enable is TRUE, the time for which to set the wakeup alarm, may be NULL if Enable is FALSE
/// @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If Enable is FALSE, then the wakeup alarm was disabled
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error
/// @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_WAKEUP_TIME) (
  IN BOOLEAN   Enable,
  IN EFI_TIME *Time OPTIONAL
);

//
// Virtual Memory Services (EFI 1.0+)
//

// EFI_OPTIONAL_PTR
/// When converting a pointer with a new virtual memory map the address can be NULL
#define EFI_OPTIONAL_PTR 0x00000001

// EFI_SET_VIRTUAL_ADDRESS_MAP
/// Changes the runtime addressing mode of EFI firmware from physical to virtual
/// @param MemoryMapSize     The size in bytes of the virtual memory descriptors entries
/// @param DescriptorSize    The size in bytes of a virtual memory descriptor entry
/// @param DescriptorVersion The version of the memory descriptor entries
/// @param VirtualMap        The virtual memory descriptor entries which contain new virtual address mapping information for all runtime ranges
/// @retval EFI_SUCCESS           The virtual address map has been applied
/// @retval EFI_UNSUPPORTED       EFI firmware is not at runtime, or the EFI firmware is already in virtual address mapped mode
/// @retval EFI_INVALID_PARAMETER DescriptorSize or DescriptorVersion is invalid
/// @retval EFI_NO_MAPPING        A virtual address was not supplied for a range in the memory map that requires a mapping
/// @retval EFI_NOT_FOUND         A virtual address was supplied for an address that is not found in the memory map
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_VIRTUAL_ADDRESS_MAP) (
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN UINT32                 DescriptorVersion,
  IN EFI_MEMORY_DESCRIPTOR *VirtualMap
);
// EFI_CONVERT_POINTER
/// Determines the new virtual address that is to be used on subsequent memory accesses
/// @param DebugDisposition Supplies type information for the pointer being converted
/// @param Address          The address to be converted with the new virtual address mappings being applied
/// @retval EFI_SUCCESS           The address was modified
/// @retval EFI_NOT_FOUND         The address was not found to be part of the current memory map, this is normally fatal
/// @retval EFI_INVALID_PARAMETER Address is NULL
/// @retval EFI_INVALID_PARAMETER *Address is NULL and DebugDisposition does not have the EFI_OPTIONAL_PTR bit set
typedef
EFI_STATUS
(EFIAPI
*EFI_CONVERT_POINTER) (
  IN UINTN   DebugDisposition,
  IN VOID  **Address
);

//
// Variable Services (EFI 1.0+)
//

// EFI_GLOBAL_VARIABLE_GUID
/// The global variable vendor unique identifier
#define EFI_GLOBAL_VARIABLE_GUID { 0x8BE4DF61, 0x93CA, 0x11D2, { 0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C } }

// EFI_VARIABLE_NON_VOLATILE
/// Variable is non volatile and will retain a value across resets
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
// EFI_VARIABLE_BOOTSERVICE_ACCESS
/// Variable is only accessible in boot services
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
// EFI_VARIABLE_RUNTIME_ACCESS
/// Variable is only accessible in runtime services
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
// EFI_VARIABLE_HARDWARE_ERROR_RECORD
/// 
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008
//This attribute is identified by the mnemonic 'HR' elsewhere
//in this specification.
// EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS
/// DEPRECATED: Variable is secured with certificate
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
// EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
/// Variable is secured with certificate and authentication time
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
// EFI_VARIABLE_APPEND_WRITE
/// Append data to variable instead of replacing
#define EFI_VARIABLE_APPEND_WRITE 0x00000040
// EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS
/// Variable is secured with certificates and enhanced authentication
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS 0x00000080

// gEfiGlobalVariableGuid
/// The global variable vendor unique identifier
EXTERN EFI_GUID gEfiGlobalVariableGuid;

// EFI_GET_VARIABLE
/// Returns the value of a variable
/// @param VariableName The null-terminated Unicode string that is the name of the variable
/// @param VendorGuid   The unique identifier for the vendor
/// @param Attributes   On output, the attributes bitmask for the variable
/// @param DataSize     On input, the size in bytes of the variable data buffer, on output, the size of the variable data
/// @param Data         On output, the variable data
/// @retval EFI_SUCCESS            The function completed successfully
/// @retval EFI_NOT_FOUND          The variable was not found
/// @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result. DataSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER  VariableName is NULL
/// @retval EFI_INVALID_PARAMETER  VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER  DataSize is NULL
/// @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL
/// @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error
/// @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_VARIABLE) (
  IN     CHAR16   *VariableName,
  IN     EFI_GUID *VendorGuid,
  OUT    UINT32   *Attributes OPTIONAL,
  IN OUT UINTN    *DataSize,
  OUT    VOID     *Data OPTIONAL
);
// EFI_GET_NEXT_VARIABLE_NAME
/// Enumerates the current variable names
/// @param VariableNameSize The size in bytes of the variable name buffer
/// @param VariableName     On input, the previous variable name or an empty string to start, on output, the name of the variable
/// @param VendorGuid       On input, the previous vendor identifier (ignored on start), on output, the vendor identifier of the variable
/// @retval EFI_SUCCESS           The function completed successfully
/// @retval EFI_NOT_FOUND         The next variable was not found
/// @retval EFI_BUFFER_TOO_SMALL  The VariableNameSize is too small for the result, VariableNameSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER VariableNameSize is NULL
/// @retval EFI_INVALID_PARAMETER VariableName is NULL
/// @retval EFI_INVALID_PARAMETER VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER The input values of VariableName and VendorGuid are not a name and GUID of an existing variable
/// @retval EFI_INVALID_PARAMETER Null-terminator is not found in the first VariableNameSize bytes of the input VariableName buffer
/// @retval EFI_DEVICE_ERROR      The variable name could not be retrieved due to a hardware error
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_NEXT_VARIABLE_NAME) (
  IN OUT UINTN    *VariableNameSize,
  IN OUT CHAR16   *VariableName,
  IN OUT EFI_GUID *VendorGuid
);
// EFI_SET_VARIABLE
/// Sets the value of a variable
/// @param VariableName The name of the variable
/// @param VendorGuid A unique identifier for the vendor
/// @param Attributes Attributes bitmask to set for the variable
/// @param DataSize The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE, EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS,
///                  EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS, or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set,
///                  a size of zero causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is set, then a
///                  SetVariable() call with a DataSize of zero will not cause any change to the variable value (the timestamp
///                  associated with the variable may be updated however, even if no new data value is provided). In this case
///                  the DataSize will not be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated)
/// @param Data The contents for the variable
/// @retval EFI_SUCCESS The firmware has successfully stored the variable and its data as defined by the Attributes
/// @retval EFI_INVALID_PARAMETER An invalid combination of attribute bits, name, and GUID was supplied, or the DataSize exceeds the maximum allowed
/// @retval EFI_INVALID_PARAMETER VariableName is an empty string
/// @retval EFI_OUT_OF_RESOURCES Not enough storage is available to hold the variable and its data
/// @retval EFI_DEVICE_ERROR The variable could not be saved due to a hardware failure
/// @retval EFI_WRITE_PROTECTED The variable in question is read-only
/// @retval EFI_WRITE_PROTECTED The variable in question cannot be deleted
/// @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS
///                                 or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set, but the payload
///                                 does NOT pass the validation check carried out by the firmware
/// @retval EFI_NOT_FOUND The variable trying to be updated or deleted was not found
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_VARIABLE) (
  IN CHAR16   *VariableName,
  IN EFI_GUID *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID     *Data
);

//
// Miscellaneous Services (EFI 1.0+)
//

// EFI_RESET_TYPE
/// Type of platform reset
typedef enum EFI_RESET_TYPE EFI_RESET_TYPE;
enum EFI_RESET_TYPE {

 // EfiResetCold
 /// Causes a system-wide reset
 EfiResetCold,
 // EfiResetWarm
 /// Causes a system-wide initialization
 EfiResetWarm,
 // EfiResetShutdown
 /// Causes the system to enter a power state equivalent to the ACPI G2/S5 or G3 states
 EfiResetShutdown,
 // EfiResetPlatformSpecific
 /// Causes a vendor specific system-wide reset
 EfiResetPlatformSpecific

};

// EFI_GET_NEXT_HIGH_MONO_COUNT
/// Returns the next high 32 bits of the platform monotonic counter
/// @param HighCount On output, the next high 32 bits of the platform monotonic counter
/// @retval EFI_SUCCESS           The next high monotonic count was returned
/// @retval EFI_DEVICE_ERROR      The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER HighCount is NULL
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_NEXT_HIGH_MONO_COUNT) (
  OUT UINT32 *HighCount
);
// EFI_RESET_SYSTEM
/// Resets or shuts down the entire platform
/// @param ResetType   The type of reset to perform
/// @param ResetStatus The status code for the reset
/// @param DataSize    The size, in bytes, of of the reset data
/// @param ResetData   For a ResetType of EfiResetCold, EfiResetWarm, or EfiResetShutdown the data buffer starts
///                     with a null-terminated Unicode string, optionally followed by additional binary data.
///                     The string is a description that the caller may use to further indicate the reason for the
///                     system reset. ResetData is only valid if ResetStatus is something other than EFI_SUCCESS unless
///                     the ResetType is EfiResetPlatformSpecific where a minimum amount of ResetData is always required.
///                     For a ResetType of EfiResetPlatformSpecific the data buffer also starts with a null-terminated
///                     string that is followed by an EFI_GUID that describes the specific type of reset to perform
typedef
VOID
(EFIAPI
*EFI_RESET_SYSTEM) (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData OPTIONAL
);

//
// Capsule Services (UEFI 2.0+)
//

// CAPSULE_FLAGS_PERSIST_ACROSS_RESET
/// Capsule will be processed after reset
#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
// CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE
/// Capsule will be placed in configuration table in system table
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
// CAPSULE_FLAGS_INITIATE_RESET
/// Capsule will initiate a system-wide reset
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000

// EFI_CAPSULE_BLOCK_DESCRIPTOR
/// Capsule block descriptor
typedef struct EFI_CAPSULE_BLOCK_DESCRIPTOR EFI_CAPSULE_BLOCK_DESCRIPTOR;
struct EFI_CAPSULE_BLOCK_DESCRIPTOR {

  // Length
  /// Length in bytes of the data pointed to by DataBlock
  UINT64 Length;
  // Union
  /// If Length is 0 then ContinuationPointer is valid, otherwise DataBlock is valid
  union {

    // DataBlock
    /// Physical address of the data block
    EFI_PHYSICAL_ADDRESS DataBlock;
    // ContinuationPointer
    /// Physical address of another capsule block descriptor, if zero this entry represents the end of the list
    EFI_PHYSICAL_ADDRESS ContinuationPointer;

  } Union;

};

// EFI_CAPSULE_HEADER
/// Capsule header
typedef struct EFI_CAPSULE_HEADER EFI_CAPSULE_HEADER;
struct EFI_CAPSULE_HEADER {

  // CapsuleGuid
  /// The unique identifier that defines the contents of the capsule
  EFI_GUID CapsuleGuid;
  // HeaderSize
  /// The size in bytes of the capsule header
  UINT32   HeaderSize;
  // Flags
  /// Bitmask of capsule flags
  UINT32   Flags;
  // CapsuleImageSize
  /// The size in bytes of the capsule
  UINT32   CapsuleImageSize;

};

// EFI_UPDATE_CAPSULE
/// Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended
///  consumption, the firmware may process the capsule immediately. If the payload should persist across
///  a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must be passed into
///  ResetSystem() and will cause the capsule to be processed by the firmware as part of the reset process
/// @param CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules being passed into update
///                            capsule. Each capsule is assumed to stored in contiguous virtual memory. The capsules in
///                            the CapsuleHeaderArray must be the same capsules as the ScatterGatherList. The
///                            CapsuleHeaderArray must have the capsules in the same order as the ScatterGatherList.
/// @param CapsuleCount       The count of capsules in the capsule array
/// @param ScatterGatherList  Physical pointer to a set of capsule block descriptors in physical memory. The capsules in
///                            the ScatterGatherList must be in the same order as the CapsuleHeaderArray. This parameter
///                            is only referenced if the capsules are defined to persist across system reset
/// @retval EFI_SUCCESS           Valid capsule was passed. If CAPSULE_FLAGS_PERSIST_ACROSS_RESET is not set, the capsule has been successfully processed by the firmware
/// @retval EFI_INVALID_PARAMETER CapsuleSize , or an incompatible set of flags were set in the capsule header
/// @retval EFI_INVALID_PARAMETER CapsuleCount is 0
/// @retval EFI_DEVICE_ERROR      The capsule update was started, but failed due to a device error
/// @retval EFI_UNSUPPORTED       The capsule type is not supported on this platform
/// @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule
///                                is compatible with this platform but is not capable of being submitted or processed
///                                in runtime. The caller may resubmit the capsule prior to ExitBootServices()
/// @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates the
///                                capsule is compatible with this platform but there are insufficient resources to process
typedef
EFI_STATUS
(EFIAPI
*EFI_UPDATE_CAPSULE) (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
);
// EFI_QUERY_CAPSULE_CAPABILITIES
/// Returns if the capsule is supported
/// @param CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules being passed into update capsule. The capsules are assumed stored in contiguous virtual memory
/// @param CapsuleCount       The count of capsules in the capsule array
/// @param MaximumCapsuleSize On output, the maximum size in bytes that UpdateCapsule() can support as an argument to UpdateCapsule() via CapsuleHeaderArray and ScatterGatherList
/// @param ResetType          On output, the type of reset required for the capsule update
/// @retval EFI_SUCCESS Valid answer returned
/// @retval EFI_INVALID_PARAMETER MaximumCapsuleSize is NULL
/// @retval EFI_UNSUPPORTED The capsule type is not supported on this platform, and MaximumCapsuleSize and ResetType are undefined
/// @retval EFI_OUT_OF_RESOURCES When ExitBootServices() has been previously called this error indicates the capsule is
///                               compatible with this platform but is not capable of being submitted or processed in
///                               runtime. The caller may resubmit the capsule prior to ExitBootServices()
/// @retval EFI_OUT_OF_RESOURCES When ExitBootServices()has not been previously called then this error indicates the
///                               capsule is compatible with this platform but there are insufficient resources to process
typedef
EFI_STATUS
(EFIAPI
*EFI_QUERY_CAPSULE_CAPABILITIES) (
  IN  EFI_CAPSULE_HEADER **CapsuleHeaderArray,
  IN  UINTN                CapsuleCount,
  OUT UINT64              *MaximumCapsuleSize,
  OUT EFI_RESET_TYPE      *ResetType
);

//
// Variable Information Services (UEFI 2.0+)
//

// EFI_QUERY_VARIABLE_INFO
/// Returns information about the variable store
/// @param Attributes                   Bitmask of the attributes to specify the type of variables on which to return information
/// @param MaximumVariableStorageSize   On output, the maximum size in bytes of the storage space available for the variables with the attributes specified
/// @param RemainingVariableStorageSize On output, the remaining size in bytes of the storage space available for the variable with the attributes specified
/// @param MaximumVariableSize          On output, the maximum size in bytes of an individual variable with the attributes specified
/// @retval EFI_SUCCESS           The variable store information was returned
/// @retval EFI_INVALID_PARAMETER An invalid combination of attribute bits was supplied
/// @retval EFI_UNSUPPORTED       The attributes are not supported on this platform
typedef
EFI_STATUS
(EFIAPI
*EFI_QUERY_VARIABLE_INFO) (
  IN  UINT32  Attributes,
  OUT UINT64 *MaximumVariableStorageSize,
  OUT UINT64 *RemainingVariableStorageSize,
  OUT UINT64 *MaximumVariableSize
);

//
// Runtime Services Table
//

// EFI_RUNTIME_SERVICES_REVISION
/// The UEFI specification version to which the runtime services table complies
#define EFI_RUNTIME_SERVICES_REVISION EFI_SPECIFICATION_VERSION

// EFI_RUNTIME_SERVICES_SIGNATURE
/// The UEFI runtime services table signature
#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544E5552

// EFI_RUNTIME_SERVICES
/// Runtime services
typedef struct EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;
struct EFI_RUNTIME_SERVICES {

  // Hdr
  /// The table header
  EFI_TABLE_HEADER Hdr;

  //
  // Time Services (EFI 1.0+)
  //

  // GetTime
  /// Returns the current time and date, and the time-keeping capabilities of the platform
  EFI_GET_TIME        GetTime;
  // SetTime
  /// Sets the current local time and date information
  EFI_SET_TIME        SetTime;
  // GetWakeupTime
  /// Returns the current wakeup alarm clock setting
  EFI_GET_WAKEUP_TIME GetWakeupTime;
  // SetWakeupTime
  /// Sets the system wakeup alarm clock time
  EFI_SET_WAKEUP_TIME SetWakeupTime;

  //
  // Virtual Memory Services (EFI 1.0+)
  //

  // SetVirtualAddressMap
  /// Convert from physical addressing to virtual addressing
  EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
  // ConvertPointer
  /// Cconvert internal pointers when switching to virtual addressing
  EFI_CONVERT_POINTER         ConvertPointer;

  //
  // Variable Services (EFI 1.0+)
  //

  // GetVariable
  /// Returns the value of a variable
  EFI_GET_VARIABLE           GetVariable;
  // GetNextVariableName
  /// Enumerates the current variable names
  EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
  // SetVariable
  /// Sets the value of a variable
  EFI_SET_VARIABLE           SetVariable;

  //
  // Miscellaneous Services (EFI 1.0+)
  //

  // GetNextHighMonotonicCount
  /// Returns the next high 32 bits of the platform’s monotonic counter
  EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
  // ResetSystem
  /// Resets the entire platform
  EFI_RESET_SYSTEM             ResetSystem;

  //
  // Capsule Services (UEFI 2.0+)
  //

  // UpdateCapsule
  /// Passes capsules to the firmware with both virtual and physical mapping
  EFI_UPDATE_CAPSULE             UpdateCapsule;
  // QueryCapsuleCapabilities
  /// Returns if a capsule is supported
  EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;

  //
  // Variable Information Services (UEFI 2.0+)
  //

  // QueryVariableInfo
  /// Returns information about the variable store
  EFI_QUERY_VARIABLE_INFO QueryVariableInfo;

};

// EfiIsVirtualMachine
/// Detect virtual machine environment
/// @return Whether the environment is a virtual machine not
EXTERN
BOOLEAN
EFIAPI
EfiIsVirtualMachine (
  VOID
);

// EfiGetTime
/// Returns the current time and date information, and the time-keeping capabilities of the hardware platform
/// @param Time         On output, the current time
/// @param Capabilities On output, the real time clock device capabilities
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER Time is NULL
/// @retval EFI_DEVICE_ERROR      The time could not be retrieved due to a hardware error
EXTERN
EFI_STATUS
EFIAPI
EfiGetTime (
  OUT EFI_TIME              *Time,
  OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
);
// EfiSetTime
/// Sets the current local time and date information.
/// @param Time The time to set as current time
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The time could not be set due to a hardware error
EXTERN
EFI_STATUS
EFIAPI
EfiSetTime (
  IN EFI_TIME *Time
);
// EfiGetWakeupTime
/// Returns the current wakeup alarm clock settings
/// @param Enabled Indicates if the alarm is currently enabled or disabled
/// @param Pending Indicates if the alarm signal is pending and requires acknowledgement
/// @param Time    The current alarm time
/// @retval EFI_SUCCESS           The alarm settings were returned
/// @retval EFI_INVALID_PARAMETER Enabled is NULL
/// @retval EFI_INVALID_PARAMETER Pending is NULL
/// @retval EFI_INVALID_PARAMETER Time is NULL
/// @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error
/// @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform
EXTERN
EFI_STATUS
EFIAPI
EfiGetWakeupTime (
  OUT BOOLEAN  *Enabled,
  OUT BOOLEAN  *Pending,
  OUT EFI_TIME *Time
);
// EfiSetWakeupTime
/// Sets the system wakeup alarm clock time
/// @param Enable Enable or disable the wakeup alarm
/// @param Time   If Enable is TRUE, the time for which to set the wakeup alarm, may be NULL if Enable is FALSE
/// @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If Enable is FALSE, then the wakeup alarm was disabled
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error
/// @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform
EXTERN
EFI_STATUS
EFIAPI
EfiSetWakeupTime (
  IN BOOLEAN   Enable,
  IN EFI_TIME *Time OPTIONAL
);

//
// Virtual Memory Services (EFI 1.0+)
//

// EfiSetVirtualAddressMap
/// Changes the runtime addressing mode of EFI firmware from physical to virtual
/// @param MemoryMapSize     The size in bytes of the virtual memory descriptors entries
/// @param DescriptorSize    The size in bytes of a virtual memory descriptor entry
/// @param DescriptorVersion The version of the memory descriptor entries
/// @param VirtualMap        The virtual memory descriptor entries which contain new virtual address mapping information for all runtime ranges
/// @retval EFI_SUCCESS           The virtual address map has been applied
/// @retval EFI_UNSUPPORTED       EFI firmware is not at runtime, or the EFI firmware is already in virtual address mapped mode
/// @retval EFI_INVALID_PARAMETER DescriptorSize or DescriptorVersion is invalid
/// @retval EFI_NO_MAPPING        A virtual address was not supplied for a range in the memory map that requires a mapping
/// @retval EFI_NOT_FOUND         A virtual address was supplied for an address that is not found in the memory map
EXTERN
EFI_STATUS
EFIAPI
EfiSetVirtualAddressMap (
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN UINT32                 DescriptorVersion,
  IN EFI_MEMORY_DESCRIPTOR *VirtualMap
);
// EfiConvertPointer
/// Determines the new virtual address that is to be used on subsequent memory accesses
/// @param DebugDisposition Supplies type information for the pointer being converted
/// @param Address          The address to be converted with the new virtual address mappings being applied
/// @retval EFI_SUCCESS           The address was modified
/// @retval EFI_NOT_FOUND         The address was not found to be part of the current memory map, this is normally fatal
/// @retval EFI_INVALID_PARAMETER Address is NULL
/// @retval EFI_INVALID_PARAMETER *Address is NULL and DebugDisposition does not have the EFI_OPTIONAL_PTR bit set
EXTERN
EFI_STATUS
EFIAPI
EfiConvertPointer (
  IN UINTN   DebugDisposition,
  IN VOID  **Address
);

//
// Variable Services (EFI 1.0+)
//

// EfiGetVariable
/// Returns the value of a variable
/// @param VariableName The null-terminated Unicode string that is the name of the variable
/// @param VendorGuid   The unique identifier for the vendor
/// @param Attributes   On output, the attributes bitmask for the variable
/// @param DataSize     On input, the size in bytes of the variable data buffer, on output, the size of the variable data
/// @param Data         On output, the variable data
/// @retval EFI_SUCCESS            The function completed successfully
/// @retval EFI_NOT_FOUND          The variable was not found
/// @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result. DataSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER  VariableName is NULL
/// @retval EFI_INVALID_PARAMETER  VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER  DataSize is NULL
/// @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL
/// @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error
/// @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure
EXTERN
EFI_STATUS
EFIAPI
EfiGetVariable (
  IN     CHAR16   *VariableName,
  IN     EFI_GUID *VendorGuid,
  OUT    UINT32   *Attributes OPTIONAL,
  IN OUT UINTN    *DataSize,
  OUT    VOID     *Data OPTIONAL
);
// EfiGetVariableBuffer
/// Returns a duplicate of the value of a variable
/// @param VariableName The null-terminated Unicode string that is the name of the variable
/// @param VendorGuid   The unique identifier for the vendor
/// @param Attributes   On output, the attributes bitmask for the variable
/// @param DataSize     On output, the size of the variable data
/// @param Data         On output, the variable data which must be freed
/// @retval EFI_SUCCESS            The function completed successfully
/// @retval EFI_NOT_FOUND          The variable was not found
/// @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result. DataSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER  VariableName is NULL
/// @retval EFI_INVALID_PARAMETER  VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL
/// @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error
/// @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure
EXTERN
EFI_STATUS
EFIAPI
EfiGetVariableBuffer (
  IN  CHAR16    *VariableName,
  IN  EFI_GUID  *VendorGuid,
  OUT UINT32    *Attributes OPTIONAL,
  OUT UINTN     *DataSize OPTIONAL,
  OUT VOID     **Data
);
// EfiGetGlobalVariable
/// Returns the value of a global vendor variable
/// @param VariableName The null-terminated Unicode string that is the name of the variable
/// @param Attributes   On output, the attributes bitmask for the variable
/// @param DataSize     On input, the size in bytes of the variable data buffer, on output, the size of the variable data
/// @param Data         On output, the variable data
/// @retval EFI_SUCCESS            The function completed successfully
/// @retval EFI_NOT_FOUND          The variable was not found
/// @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result. DataSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER  VariableName is NULL
/// @retval EFI_INVALID_PARAMETER  VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER  DataSize is NULL
/// @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL
/// @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error
/// @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure
EXTERN
EFI_STATUS
EFIAPI
EfiGetGlobalVariable (
  IN     CHAR16 *VariableName,
  OUT    UINT32 *Attributes OPTIONAL,
  IN OUT UINTN  *DataSize,
  OUT    VOID   *Data OPTIONAL
);
// EfiGetGlobalVariableBuffer
/// Returns a duplicate of the value of a global vendor variable
/// @param VariableName The null-terminated Unicode string that is the name of the variable
/// @param Attributes   On output, the attributes bitmask for the variable
/// @param DataSize     On output, the size of the variable data
/// @param Data         On output, the variable data which must be freed
/// @retval EFI_SUCCESS            The function completed successfully
/// @retval EFI_NOT_FOUND          The variable was not found
/// @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result. DataSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER  VariableName is NULL
/// @retval EFI_INVALID_PARAMETER  VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER  DataSize is NULL
/// @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL
/// @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error
/// @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure
EXTERN
EFI_STATUS
EFIAPI
EfiGetGlobalVariableBuffer (
  IN  CHAR16  *VariableName,
  OUT UINT32  *Attributes OPTIONAL,
  OUT UINTN   *DataSize OPTIONAL,
  OUT VOID   **Data
);
// EfiGetNextVariableName
/// Enumerates the current variable names
/// @param VariableNameSize The size in bytes of the variable name buffer
/// @param VariableName     On input, the previous variable name or an empty string to start, on output, the name of the variable
/// @param VendorGuid       On input, the previous vendor identifier (ignored on start), on output, the vendor identifier of the variable
/// @retval EFI_SUCCESS           The function completed successfully
/// @retval EFI_NOT_FOUND         The next variable was not found
/// @retval EFI_BUFFER_TOO_SMALL  The VariableNameSize is too small for the result, VariableNameSize has been updated with the size needed to complete the request
/// @retval EFI_INVALID_PARAMETER VariableNameSize is NULL
/// @retval EFI_INVALID_PARAMETER VariableName is NULL
/// @retval EFI_INVALID_PARAMETER VendorGuid is NULL
/// @retval EFI_INVALID_PARAMETER The input values of VariableName and VendorGuid are not a name and GUID of an existing variable
/// @retval EFI_INVALID_PARAMETER Null-terminator is not found in the first VariableNameSize bytes of the input VariableName buffer
/// @retval EFI_DEVICE_ERROR      The variable name could not be retrieved due to a hardware error
EXTERN
EFI_STATUS
EFIAPI
EfiGetNextVariableName (
  IN OUT UINTN    *VariableNameSize,
  IN OUT CHAR16   *VariableName,
  IN OUT EFI_GUID *VendorGuid
);
// EfiSetVariable
/// Sets the value of a variable
/// @param VariableName The name of the variable
/// @param VendorGuid   A unique identifier for the vendor
/// @param Attributes   Attributes bitmask to set for the variable
/// @param DataSize     The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE, EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS,
///                      EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS, or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set,
///                      a size of zero causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is set, then a
///                      SetVariable() call with a DataSize of zero will not cause any change to the variable value (the timestamp
///                      associated with the variable may be updated however, even if no new data value is provided). In this case
///                      the DataSize will not be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated)
/// @param Data The contents for the variable
/// @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as defined by the Attributes
/// @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the DataSize exceeds the maximum allowed
/// @retval EFI_INVALID_PARAMETER  VariableName is an empty string
/// @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data
/// @retval EFI_DEVICE_ERROR       The variable could not be saved due to a hardware failure
/// @retval EFI_WRITE_PROTECTED    The variable in question is read-only
/// @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted
/// @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS
///                                 or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set, but the payload
///                                 does NOT pass the validation check carried out by the firmware
/// @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found
EXTERN
EFI_STATUS
EFIAPI
EfiSetVariable (
  IN CHAR16   *VariableName,
  IN EFI_GUID *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID     *Data
);
// EfiSetGlobalVariable
/// Sets the value of a global vendor variable
/// @param VariableName The name of the variable
/// @param Attributes   Attributes bitmask to set for the variable
/// @param DataSize     The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE, EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS,
///                      EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS, or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set,
///                      a size of zero causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is set, then a
///                      SetVariable() call with a DataSize of zero will not cause any change to the variable value (the timestamp
///                      associated with the variable may be updated however, even if no new data value is provided). In this case
///                      the DataSize will not be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated)
/// @param Data         The contents for the variable
/// @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as defined by the Attributes
/// @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the DataSize exceeds the maximum allowed
/// @retval EFI_INVALID_PARAMETER  VariableName is an empty string
/// @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data
/// @retval EFI_DEVICE_ERROR       The variable could not be saved due to a hardware failure
/// @retval EFI_WRITE_PROTECTED    The variable in question is read-only
/// @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted
/// @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS
///                                 or EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set, but the payload
///                                 does NOT pass the validation check carried out by the firmware
/// @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found
EXTERN
EFI_STATUS
EFIAPI
EfiSetGlobalVariable (
  IN CHAR16   *VariableName,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID     *Data
);
// EfiDeleteVariable
/// Deletes the value of a variable
/// @param VariableName The name of the variable
/// @param VendorGuid   A unique identifier for the vendor
/// @retval EFI_SUCCESS            The firmware has successfully deleted the variable and its data
/// @retval EFI_INVALID_PARAMETER  An invalid combination of name, and GUID was supplied
/// @retval EFI_INVALID_PARAMETER  VariableName is an empty string
/// @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data
/// @retval EFI_DEVICE_ERROR       The variable could not be saved due to a hardware failure
/// @retval EFI_WRITE_PROTECTED    The variable in question is read-only
/// @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted
EXTERN
EFI_STATUS
EFIAPI
EfiDeleteVariable (
  IN CHAR16   *VariableName,
  IN EFI_GUID *VendorGuid
);
// EfiDeleteGlobalVariable
/// Deletes the value of a global vendor variable
/// @param VariableName The name of the variable
/// @retval EFI_SUCCESS            The firmware has successfully deleted the variable and its data
/// @retval EFI_INVALID_PARAMETER  An invalid combination of name, and GUID was supplied
/// @retval EFI_INVALID_PARAMETER  VariableName is an empty string
/// @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data
/// @retval EFI_DEVICE_ERROR       The variable could not be saved due to a hardware failure
/// @retval EFI_WRITE_PROTECTED    The variable in question is read-only
/// @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted
EXTERN
EFI_STATUS
EFIAPI
EfiDeleteGlobalVariable (
  IN CHAR16 *VariableName
);

//
// Miscellaneous Services (EFI 1.0+)
//

// EfiGetNextHighMonotonicCount
/// Returns the next high 32 bits of the platform monotonic counter
/// @param HighCount On output, the next high 32 bits of the platform monotonic counter
/// @retval EFI_SUCCESS           The next high monotonic count was returned
/// @retval EFI_DEVICE_ERROR      The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER HighCount is NULL
EXTERN
EFI_STATUS
EFIAPI
EfiGetNextHighMonotonicCount (
  OUT UINT32 *HighCount
);
// EfiResetSystem
/// Resets or shuts down the entire platform
/// @param ResetType   The type of reset to perform
/// @param ResetStatus The status code for the reset
/// @param DataSize    The size, in bytes, of of the reset data
/// @param ResetData   For a ResetType of EfiResetCold, EfiResetWarm, or EfiResetShutdown the data buffer starts
///                     with a null-terminated Unicode string, optionally followed by additional binary data.
///                     The string is a description that the caller may use to further indicate the reason for the
///                     system reset. ResetData is only valid if ResetStatus is something other than EFI_SUCCESS unless
///                     the ResetType is EfiResetPlatformSpecific where a minimum amount of ResetData is always required.
///                     For a ResetType of EfiResetPlatformSpecific the data buffer also starts with a null-terminated
///                     string that is followed by an EFI_GUID that describes the specific type of reset to perform
EXTERN
VOID
EFIAPI
EfiResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData OPTIONAL
);

//
// Capsule Services (UEFI 2.0+)
//

// EfiUpdateCapsule
/// Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended
///  consumption, the firmware may process the capsule immediately. If the payload should persist across
///  a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must be passed into
///  ResetSystem() and will cause the capsule to be processed by the firmware as part of the reset process
/// @param CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules being passed into update
///                            capsule. Each capsule is assumed to stored in contiguous virtual memory. The capsules in
///                            the CapsuleHeaderArray must be the same capsules as the ScatterGatherList. The
///                            CapsuleHeaderArray must have the capsules in the same order as the ScatterGatherList.
/// @param CapsuleCount       The count of capsules in the capsule array
/// @param ScatterGatherList  Physical pointer to a set of capsule block descriptors in physical memory. The capsules in
///                            the ScatterGatherList must be in the same order as the CapsuleHeaderArray. This parameter
///                            is only referenced if the capsules are defined to persist across system reset
/// @retval EFI_SUCCESS           Valid capsule was passed. If CAPSULE_FLAGS_PERSIST_ACROSS_RESET is not set, the capsule has been successfully processed by the firmware
/// @retval EFI_INVALID_PARAMETER CapsuleSize , or an incompatible set of flags were set in the capsule header
/// @retval EFI_INVALID_PARAMETER CapsuleCount is 0
/// @retval EFI_DEVICE_ERROR      The capsule update was started, but failed due to a device error
/// @retval EFI_UNSUPPORTED       The capsule type is not supported on this platform
/// @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule
///                                is compatible with this platform but is not capable of being submitted or processed
///                                in runtime. The caller may resubmit the capsule prior to ExitBootServices()
/// @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates the
///                                capsule is compatible with this platform but there are insufficient resources to process
EXTERN
EFI_STATUS
EFIAPI
EfiUpdateCapsule (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
);
// EfiQueryCapsuleCapabilities
/// Returns if the capsule is supported
/// @param CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules being passed into update capsule. The capsules are assumed stored in contiguous virtual memory
/// @param CapsuleCount       The count of capsules in the capsule array
/// @param MaximumCapsuleSize On output, the maximum size in bytes that UpdateCapsule() can support as an argument to UpdateCapsule() via CapsuleHeaderArray and ScatterGatherList
/// @param ResetType          On output, the type of reset required for the capsule update
/// @retval EFI_SUCCESS Valid answer returned
/// @retval EFI_INVALID_PARAMETER MaximumCapsuleSize is NULL
/// @retval EFI_UNSUPPORTED The capsule type is not supported on this platform, and MaximumCapsuleSize and ResetType are undefined
/// @retval EFI_OUT_OF_RESOURCES When ExitBootServices() has been previously called this error indicates the capsule is
///                               compatible with this platform but is not capable of being submitted or processed in
///                               runtime. The caller may resubmit the capsule prior to ExitBootServices()
/// @retval EFI_OUT_OF_RESOURCES When ExitBootServices()has not been previously called then this error indicates the
///                               capsule is compatible with this platform but there are insufficient resources to process
EXTERN
EFI_STATUS
EFIAPI
EfiQueryCapsuleCapabilities (
  IN  EFI_CAPSULE_HEADER **CapsuleHeaderArray,
  IN  UINTN                CapsuleCount,
  OUT UINT64              *MaximumCapsuleSize,
  OUT EFI_RESET_TYPE      *ResetType
);

//
// Variable Information Services (UEFI 2.0+)
//

// EfiQueryVariableInfo
/// Returns information about the variable store
/// @param Attributes                   Bitmask of the attributes to specify the type of variables on which to return information
/// @param MaximumVariableStorageSize   On output, the maximum size in bytes of the storage space available for the variables with the attributes specified
/// @param RemainingVariableStorageSize On output, the remaining size in bytes of the storage space available for the variable with the attributes specified
/// @param MaximumVariableSize          On output, the maximum size in bytes of an individual variable with the attributes specified
/// @retval EFI_SUCCESS           The variable store information was returned
/// @retval EFI_INVALID_PARAMETER An invalid combination of attribute bits was supplied
/// @retval EFI_UNSUPPORTED       The attributes are not supported on this platform
EXTERN
EFI_STATUS
EFIAPI
EfiQueryVariableInfo (
  IN  UINT32  Attributes,
  OUT UINT64 *MaximumVariableStorageSize,
  OUT UINT64 *RemainingVariableStorageSize,
  OUT UINT64 *MaximumVariableSize
);

// EfiIsRuntime
/// Indicates only runtime services are available and boot services are not
/// @return Whether this is runtime only
EXTERN
BOOLEAN
EFIAPI
EfiIsRuntime (
  VOID
);
// EfiIsDebug
/// Indicates whether this is a debug build
/// @return Whether this is a debug build
EXTERN
BOOLEAN
EFIAPI
EfiIsDebug (
  VOID
);

// EfiFloat32Extract
/// Extract the components from a floating point
/// @param Float    The floating point value
/// @param Integer  On output, the integer component of the floating point
/// @param Fraction On output, the fractional component of the floating point
/// @param Negative On output, whether the floating point is negative or not
/// @retval EFI_INVALID_PARAMETER If Integer, Fraction, and Negative are NULL
/// @retval EFI_NO_MAPPING        The floating point value is not a valid number
/// @retval EFI_NOT_FOUND         The floating point value is positive or negative infinity, *Negative is populated with the sign information
/// @retval EFI_SUCCESS           The floating point components were extracted
EXTERN
EFI_STATUS
EFIAPI
EfiFloat32Extract (
  IN  FLOAT32  Float,
  OUT UINT32  *Integer OPTIONAL,
  OUT FLOAT32 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
);
// EfiFloat32Extract64
/// Extract the components from a floating point
/// @param Float    The floating point value
/// @param Integer  On output, the integer component of the floating point
/// @param Fraction On output, the fractional component of the floating point
/// @param Negative On output, whether the floating point is negative or not
/// @retval EFI_INVALID_PARAMETER If Integer, Fraction, and Negative are NULL
/// @retval EFI_NO_MAPPING        The floating point value is not a valid number
/// @retval EFI_NOT_FOUND         The floating point value is positive or negative infinity, *Negative is populated with the sign information
/// @retval EFI_SUCCESS           The floating point components were extracted
EXTERN
EFI_STATUS
EFIAPI
EfiFloat32Extract64 (
  IN  FLOAT32  Float,
  OUT UINT64  *Integer OPTIONAL,
  OUT FLOAT64 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
);
// EfiFloat64Extract
/// Extract the components from a floating point
/// @param Float    The floating point value
/// @param Integer  On output, the integer component of the floating point
/// @param Fraction On output, the fractional component of the floating point
/// @param Negative On output, whether the floating point is negative or not
/// @retval EFI_INVALID_PARAMETER If Integer, Fraction, and Negative are NULL
/// @retval EFI_NO_MAPPING        The floating point value is not a valid number
/// @retval EFI_NOT_FOUND         The floating point value is positive or negative infinity, *Negative is populated with the sign information
/// @retval EFI_SUCCESS           The floating point components were extracted
EXTERN
EFI_STATUS
EFIAPI
EfiFloat64Extract (
  IN  FLOAT64  Float,
  OUT UINT64  *Integer OPTIONAL,
  OUT FLOAT64 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
);
// EfiFloat32Encode
/// Encode a floating point from components
/// @param Value    The value to encode into a floating point
/// @param Exponent The binary exponent of the floating point
/// @param Negative Whether the floating point is negative or not
/// @param Float    On output, the encoded floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS The floating point components were encoded
EXTERN
EFI_STATUS
EFIAPI
EfiFloat32Encode (
  IN  UINT32   Value,
  IN  INT32    Exponent,
  IN  BOOLEAN  Negative,
  OUT FLOAT32 *Float
);
// EfiFloat64Encode
/// Encode a floating point from components
/// @param Value    The value to encode into a floating point
/// @param Exponent The binary exponent of the floating point
/// @param Negative Whether the floating point is negative or not
/// @param Float    On output, the encoded floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS The floating point components were encoded
EXTERN
EFI_STATUS
EFIAPI
EfiFloat64Encode (
  IN  UINT64   Value,
  IN  INT64    Exponent,
  IN  BOOLEAN  Negative,
  OUT FLOAT64 *Float
);
// EfiFloat32Normalize
/// Normalize a floating point
/// @param Float On input, the floating point to normalize, on output, the normalized floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           The floating point was normalized
EXTERN
EFI_STATUS
EFIAPI
EfiFloat32Normalize (
  IN OUT FLOAT32 *Float
);
// EfiFloat64Normalize
/// Normalize a floating point
/// @param Float On input, the floating point to normalize, on output, the normalized floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           The floating point was normalized
EXTERN
EFI_STATUS
EFIAPI
EfiFloat64Normalize (
  IN OUT FLOAT64 *Float
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_RUNTIME_SERVICES_HEADER__
