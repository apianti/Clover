///
/// @file Library/Uefi/Runtime.c
///
/// UEFI runtime services
///

#include <Uefi.h>

// mEfiIsRuntime
/// TODO: The boot services have exited and only runtime services are available
STATIC BOOLEAN mEfiIsRuntime = FALSE;

// gEfiGlobalVariableGuid
/// The global variable vendor unique identifier
EFI_GUID gEfiGlobalVariableGuid = EFI_GLOBAL_VARIABLE_GUID;

// EfiGetTime
/// Returns the current time and date information, and the time-keeping capabilities of the hardware platform
/// @param Time         On output, the current time
/// @param Capabilities On output, the real time clock device capabilities
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER Time is NULL
/// @retval EFI_DEVICE_ERROR      The time could not be retrieved due to a hardware error
EFI_STATUS
EFIAPI
EfiGetTime (
  OUT EFI_TIME              *Time,
  OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->GetTime == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->GetTime(Time, Capabilities);
}
// EfiSetTime
/// Sets the current local time and date information.
/// @param Time The time to set as current time
/// @retval EFI_SUCCESS           The operation completed successfully
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The time could not be set due to a hardware error
EFI_STATUS
EFIAPI
EfiSetTime (
  IN EFI_TIME *Time
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->SetTime == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->SetTime(Time);
}
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
EFI_STATUS
EFIAPI
EfiGetWakeupTime (
  OUT BOOLEAN  *Enabled,
  OUT BOOLEAN  *Pending,
  OUT EFI_TIME *Time
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->GetWakeupTime == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->GetWakeupTime(Enabled, Pending, Time);
}
// EfiSetWakeupTime
/// Sets the system wakeup alarm clock time
/// @param Enable Enable or disable the wakeup alarm
/// @param Time   If Enable is TRUE, the time for which to set the wakeup alarm, may be NULL if Enable is FALSE
/// @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If Enable is FALSE, then the wakeup alarm was disabled
/// @retval EFI_INVALID_PARAMETER A time field is out of range
/// @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error
/// @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this platform
EFI_STATUS
EFIAPI
EfiSetWakeupTime (
  IN BOOLEAN   Enable,
  IN EFI_TIME *Time OPTIONAL
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->SetWakeupTime == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->SetWakeupTime(Enable, Time);
}

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
EFI_STATUS
EFIAPI
EfiSetVirtualAddressMap (
  IN UINTN                  MemoryMapSize,
  IN UINTN                  DescriptorSize,
  IN UINT32                 DescriptorVersion,
  IN EFI_MEMORY_DESCRIPTOR *VirtualMap
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->SetVirtualAddressMap == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->SetVirtualAddressMap(MemoryMapSize, DescriptorSize, DescriptorVersion, VirtualMap);
}
// EfiConvertPointer
/// Determines the new virtual address that is to be used on subsequent memory accesses
/// @param DebugDisposition Supplies type information for the pointer being converted
/// @param Address          The address to be converted with the new virtual address mappings being applied
/// @retval EFI_SUCCESS           The address was modified
/// @retval EFI_NOT_FOUND         The address was not found to be part of the current memory map, this is normally fatal
/// @retval EFI_INVALID_PARAMETER Address is NULL
/// @retval EFI_INVALID_PARAMETER *Address is NULL and DebugDisposition does not have the EFI_OPTIONAL_PTR bit set
EFI_STATUS
EFIAPI
EfiConvertPointer (
  IN UINTN   DebugDisposition,
  IN VOID  **Address
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->ConvertPointer == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->ConvertPointer(DebugDisposition, Address);
}

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
EFI_STATUS
EFIAPI
EfiGetVariable (
  IN     CHAR16   *VariableName,
  IN     EFI_GUID *VendorGuid,
  OUT    UINT32   *Attributes OPTIONAL,
  IN OUT UINTN    *DataSize,
  OUT    VOID     *Data OPTIONAL
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->GetVariable == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->GetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}
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
EFI_STATUS
EFIAPI
EfiGetVariableBuffer (
  IN  CHAR16    *VariableName,
  IN  EFI_GUID  *VendorGuid,
  OUT UINT32    *Attributes OPTIONAL,
  OUT UINTN     *DataSize OPTIONAL,
  OUT VOID     **Data 
) {
  EFI_STATUS  Status;
  UINTN       Size = 0;
  VOID       *Buffer;
  // Check parameters
  if ((VariableName == NULL) || (*VariableName == 0) || (VendorGuid == NULL) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the size of the variable
  Status = EfiGetVariable(VariableName, VendorGuid, Attributes, &Size, NULL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    if (!EFI_ERROR(Status)) {
      Status = EFI_DEVICE_ERROR;
    }
    return Status;
  }
  // Check size is valid
  if (Size == 0) {
    return EFI_NOT_FOUND;
  }
  // Allocate new variable buffer
  Buffer = EfiAllocate(Size);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Get the variable
  Status = EfiGetVariable(VariableName, VendorGuid, Attributes, &Size, Buffer);
  if (EFI_ERROR(Status)) {
    EfiFreePool(Buffer);
  } else {
    // Return the data and size
    *Data = Buffer;
    if (DataSize != NULL) {
      *DataSize = Size;
    }
  }
  // Return the status code
  return Status;
}
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
EFI_STATUS
EFIAPI
EfiGetGlobalVariable (
  IN     CHAR16 *VariableName,
  OUT    UINT32 *Attributes OPTIONAL,
  IN OUT UINTN  *DataSize,
  OUT    VOID   *Data OPTIONAL
) {
  return EfiGetVariable(VariableName, &gEfiGlobalVariableGuid, Attributes, DataSize, Data);
}
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
EFI_STATUS
EFIAPI
EfiGetGlobalVariableBuffer (
  IN  CHAR16  *VariableName,
  OUT UINT32  *Attributes OPTIONAL,
  OUT UINTN   *DataSize OPTIONAL,
  OUT VOID   **Data
) {
  return EfiGetVariableBuffer(VariableName, &gEfiGlobalVariableGuid, Attributes, DataSize, Data);
}
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
EFI_STATUS
EFIAPI
EfiGetNextVariableName (
  IN OUT UINTN    *VariableNameSize,
  IN OUT CHAR16   *VariableName,
  IN OUT EFI_GUID *VendorGuid
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->GetNextVariableName == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->GetNextVariableName(VariableNameSize, VariableName, VendorGuid);
}
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
EFI_STATUS
EFIAPI
EfiSetVariable (
  IN CHAR16   *VariableName,
  IN EFI_GUID *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID     *Data
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->SetVariable == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->SetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
}
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
) {
  return EfiSetVariable(VariableName, &gEfiGlobalVariableGuid, Attributes, DataSize, Data);
}
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
EFI_STATUS
EFIAPI
EfiDeleteVariable (
  IN CHAR16   *VariableName,
  IN EFI_GUID *VendorGuid
) {
  return EfiSetVariable(VariableName, VendorGuid, 0, 0, NULL);
}
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
EFI_STATUS
EFIAPI
EfiDeleteGlobalVariable (
  IN CHAR16 *VariableName
) {
  return EfiDeleteVariable(VariableName, &gEfiGlobalVariableGuid);
}

//
// Miscellaneous Services (EFI 1.0+)
//

// EfiGetNextHighMonotonicCount
/// Returns the next high 32 bits of the platform monotonic counter
/// @param HighCount On output, the next high 32 bits of the platform monotonic counter
/// @retval EFI_SUCCESS           The next high monotonic count was returned
/// @retval EFI_DEVICE_ERROR      The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER HighCount is NULL
EFI_STATUS
EFIAPI
EfiGetNextHighMonotonicCount (
  OUT UINT32 *HighCount
) {
  if ((gEfiRuntimeServices == NULL) || (gEfiRuntimeServices->GetNextHighMonotonicCount == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->GetNextHighMonotonicCount(HighCount);
}
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
VOID
EFIAPI
EfiResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData OPTIONAL
) {
  if ((gEfiRuntimeServices != NULL) && (gEfiRuntimeServices->ResetSystem != NULL)) {
    gEfiRuntimeServices->ResetSystem(ResetType, ResetStatus, DataSize, ResetData);
  }
}

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
EFI_STATUS
EFIAPI
EfiUpdateCapsule (
  IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList OPTIONAL
) {
  if ((gEfiRuntimeServices == NULL) ||
      (gEfiRuntimeServices->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) ||
      (gEfiRuntimeServices->UpdateCapsule == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->UpdateCapsule(CapsuleHeaderArray, CapsuleCount, ScatterGatherList);
}
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
EFI_STATUS
EFIAPI
EfiQueryCapsuleCapabilities (
  IN  EFI_CAPSULE_HEADER **CapsuleHeaderArray,
  IN  UINTN                CapsuleCount,
  OUT UINT64              *MaximumCapsuleSize,
  OUT EFI_RESET_TYPE      *ResetType
) {
  if ((gEfiRuntimeServices == NULL) ||
      (gEfiRuntimeServices->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) ||
      (gEfiRuntimeServices->QueryCapsuleCapabilities == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->QueryCapsuleCapabilities(CapsuleHeaderArray, CapsuleCount, MaximumCapsuleSize, ResetType);
}

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
EFI_STATUS
EFIAPI
EfiQueryVariableInfo (
  IN  UINT32  Attributes,
  OUT UINT64 *MaximumVariableStorageSize,
  OUT UINT64 *RemainingVariableStorageSize,
  OUT UINT64 *MaximumVariableSize
) {
  if ((gEfiRuntimeServices == NULL) ||
      (gEfiRuntimeServices->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) ||
      (gEfiRuntimeServices->QueryVariableInfo == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiRuntimeServices->QueryVariableInfo(Attributes, MaximumVariableStorageSize, RemainingVariableStorageSize, MaximumVariableSize);
}

// EfiIsRuntime
/// Indicates only runtime services are available and boot services are not
/// @return Whether this is runtime only
BOOLEAN
EFIAPI
EfiIsRuntime (
  VOID
) {
#if defined(EFI_RUNTIME_DRIVER)
  // When at runtime this variable has been updated when exit boot services occurs
  return mEfiIsRuntime;
#else
  return FALSE;
#endif
}

// EfiIsDebug
/// Indicates whether this is a debug build
/// @return Whether this is a debug build
BOOLEAN
EFIAPI
EfiIsDebug (
  VOID
) {
  // At runtime just return whether this was a debug build or not
  if (EfiIsRuntime()) {
#if defined(EFI_DEBUG)
    return TRUE;
#else
    return FALSE;
#endif
  }
  // Return the configuration value for debug during boot
  return EfiConfigurationGetBoolean(L"/Boot/Debug",
#if defined(EFI_DEBUG)
  TRUE
#else
  FALSE
#endif
  );
}

// FLOAT_BITS
/// Helper type to extract bits from floating point values
typedef union FLOAT_BITS FLOAT_BITS;
union FLOAT_BITS {

  // Float32
  /// 4-byte floating point value
  FLOAT32 Float32;
  // Float64
  /// 8-byte floating point value
  FLOAT64 Float64;
  // Bits
  /// Floating point value bits
  UINT64  Bits;

};

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
EFI_STATUS
EFIAPI
EfiFloat32Extract (
  IN  FLOAT32  Float,
  OUT UINT32  *Integer OPTIONAL,
  OUT FLOAT32 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
) {
  EFI_STATUS Status = EFI_SUCCESS;
  FLOAT_BITS Bits;
  INT32      FieldExponent;
  UINT32     FieldFraction;
  UINT32     FieldInteger;
  BOOLEAN    IsNegative;
  // Check parameters
  if ((Integer == NULL) && (Fraction == NULL) && (Negative == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the floating point bits
  Bits.Float32 = Float;
  // Get the floating point fractional mantessa
  FieldFraction = (UINT64)EFI_BITFIELD(Bits.Bits, 0, 22);
  // Get the floating point exponent
  FieldExponent = (INT64)EFI_BITFIELD(Bits.Bits, 23, 30);
  // Get whether the floating point is negative
  IsNegative = (BOOLEAN)EFI_BITS_ANY_SET(Bits.Bits, EFI_BIT(31));
  // Adjust the exponent accordingly
  if (FieldExponent == 255) {
    if (FieldFraction != 0) {
      // Not a number
      Status = EFI_NO_MAPPING;
    } else {
      // Infinity
      Status = EFI_NOT_FOUND;
    }
  } else if (FieldExponent == 0) {
    // Check if zero or denormalized
    if (FieldFraction != 0) {
      FieldExponent = -126;
    }
  } else {
    // Convert the exponent and fraction to correct values
    FieldExponent -= 127;
    FieldFraction |= EFI_BIT(23);
  }
  // Check whether the floating point value is valid
  if (EFI_ERROR(Status)) {
    if ((Status == EFI_NOT_FOUND) && (Negative != NULL)) {
      *Negative = IsNegative;
    }
  } else {
    // Calculate the floating point components
    if (FieldExponent >= 23) {
      // Integer component only
      FieldInteger = (FieldFraction << (FieldExponent - 23));
      FieldFraction = 0;
      FieldExponent = 0;
    } else if (FieldExponent >= 0) {
      // Integer and fractional component
      INT32 Adjust = 23 - FieldExponent;
      FieldInteger = (FieldFraction >> Adjust);
      FieldFraction &= EFI_BITMASK(0, Adjust - 1);
      if (FieldFraction == 0) {
        FieldExponent = -127;
      } else {
        while (EFI_BITS_ARE_UNSET(FieldFraction, EFI_BIT(23))) {
          FieldFraction <<= 1;
          --FieldExponent;
        }
      }
    } else {
      // No integer component
      FieldInteger = 0;
    }
    // Return component values if needed
    if (Integer != NULL) {
      *Integer = FieldInteger;
    }
    if (Fraction != NULL) {
      // Convert the leftover fractional part
      Bits.Bits = EFI_BITMASKFIELD(0, FieldExponent + 127, 23, 30);
      Bits.Bits |= EFI_BITFIELD(FieldFraction, 0, 22);
      *Fraction = Bits.Float32;
    }
    if (Negative != NULL) {
      *Negative = IsNegative;
    }
  }
  return Status;
}
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
EFI_STATUS
EFIAPI
EfiFloat32Extract64 (
  IN  FLOAT32  Float,
  OUT UINT64  *Integer OPTIONAL,
  OUT FLOAT64 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
) {
  EFI_STATUS Status = EFI_SUCCESS;
  FLOAT_BITS Bits;
  INT64      FieldExponent;
  UINT64     FieldFraction;
  UINT64     FieldInteger;
  BOOLEAN    IsNegative;
  // Check parameters
  if ((Integer == NULL) && (Fraction == NULL) && (Negative == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the floating point bits
  Bits.Float32 = Float;
  // Get the floating point fractional mantessa
  FieldFraction = (UINT64)EFI_BITFIELD(Bits.Bits, 0, 22);
  // Get the floating point exponent
  FieldExponent = (INT64)EFI_BITFIELD(Bits.Bits, 23, 30);
  // Get whether the floating point is negative
  IsNegative = (BOOLEAN)EFI_BITS_ANY_SET(Bits.Bits, EFI_BIT(31));
  // Adjust the exponent accordingly
  if (FieldExponent == 255) {
    if (FieldFraction != 0) {
      // Not a number
      Status = EFI_NO_MAPPING;
    } else {
      // Infinity
      Status = EFI_NOT_FOUND;
    }
  } else if (FieldExponent == 0) {
    // Check if zero or denormalized
    if (FieldFraction != 0) {
      FieldExponent = -126;
    }
  } else {
    // Convert the exponent and fraction to correct values
    FieldExponent -= 127;
    FieldFraction |= EFI_BIT(23);
  }
  // Check whether the floating point value is valid
  if (EFI_ERROR(Status)) {
    if ((Status == EFI_NOT_FOUND) && (Negative != NULL)) {
      *Negative = IsNegative;
    }
  } else {
    // Calculate the floating point components
    if (FieldExponent >= 23) {
      // Integer component only
      FieldInteger = (FieldFraction << (FieldExponent - 23));
      FieldFraction = 0;
      FieldExponent = 0;
    } else if (FieldExponent >= 0) {
      // Integer and fractional component
      INT64 Adjust = 23 - FieldExponent;
      FieldInteger = (FieldFraction >> Adjust);
      FieldFraction &= EFI_BITMASK(0, Adjust - 1);
      if (FieldFraction == 0) {
        FieldExponent = -1023;
      } else {
        while (EFI_BITS_ARE_UNSET(FieldFraction, EFI_BIT(52))) {
          FieldFraction <<= 1;
          --FieldExponent;
        }
      }
    } else {
      // No integer component
      FieldInteger = 0;
    }
    // Return component values if needed
    if (Integer != NULL) {
      *Integer = FieldInteger;
    }
    if (Fraction != NULL) {
      // Convert the leftover fractional part
      Bits.Bits = EFI_BITMASKFIELD(0, FieldExponent + 1023, 52, 62);
      Bits.Bits |= EFI_BITFIELD(FieldFraction, 0, 51);
      *Fraction = Bits.Float64;
    }
    if (Negative != NULL) {
      *Negative = IsNegative;
    }
  }
  return Status;
}
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
EFI_STATUS
EFIAPI
EfiFloat64Extract (
  IN  FLOAT64  Float,
  OUT UINT64  *Integer OPTIONAL,
  OUT FLOAT64 *Fraction OPTIONAL,
  OUT BOOLEAN *Negative OPTIONAL
) {
  EFI_STATUS Status = EFI_SUCCESS;
  FLOAT_BITS Bits;
  INT64      FieldExponent;
  UINT64     FieldFraction;
  UINT64     FieldInteger;
  BOOLEAN    IsNegative;
  // Check parameters
  if ((Integer == NULL) && (Fraction == NULL) && (Negative == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the floating point bits
  Bits.Float64 = Float;
  // Get the floating point fractional mantessa
  FieldFraction = (UINT64)EFI_BITFIELD(Bits.Bits, 0, 51);
  // Get the floating point exponent
  FieldExponent = (INT64)EFI_BITFIELD(Bits.Bits, 52, 62);
  // Get whether the floating point is negative
  IsNegative = EFI_BITS_ANY_SET(Bits.Bits, EFI_BIT(63)) ? TRUE : FALSE;
  // Adjust the exponent accordingly
  if (FieldExponent == 2047) {
    if (FieldFraction != 0) {
      // Not a number
      Status = EFI_NO_MAPPING;
    } else {
      // Infinity
      Status = EFI_NOT_FOUND;
    }
  } else if (FieldExponent == 0) {
    // Check if zero or denormalized
    if (FieldFraction != 0) {
      FieldExponent = -1022;
    }
  } else {
    // Convert the exponent and fraction to correct values
    FieldExponent -= 1023;
    FieldFraction |= EFI_BIT(52);
  }
  // Check whether the floating point value is valid
  if (EFI_ERROR(Status)) {
    if ((Status == EFI_NOT_FOUND) && (Negative != NULL)) {
      *Negative = IsNegative;
    }
  } else {
    // Calculate the floating point components
    if (FieldExponent >= 52) {
      // Integer component only
      FieldInteger = (FieldFraction << (FieldExponent - 52));
      FieldFraction = 0;
      FieldExponent = 0;
    } else if (FieldExponent >= 0) {
      // Integer and fractional component
      INT64 Adjust = 52 - FieldExponent;
      FieldInteger = (FieldFraction >> Adjust);
      FieldFraction &= EFI_BITMASK(0, Adjust - 1);
      if (FieldFraction == 0) {
        FieldExponent = -1023;
      } else {
        while (EFI_BITS_ARE_UNSET(FieldFraction, EFI_BIT(52))) {
          FieldFraction <<= 1;
          --FieldExponent;
        }
      }
    } else {
      // No integer component
      FieldInteger = 0;
    }
    // Return component values if needed
    if (Integer != NULL) {
      *Integer = FieldInteger;
    }
    if (Fraction != NULL) {
      // Convert the leftover fractional part
      Bits.Bits = EFI_BITMASKFIELD(0, FieldExponent + 1023, 52, 62);
      Bits.Bits |= EFI_BITFIELD(FieldFraction, 0, 51);
      *Fraction = Bits.Float64;
    }
    if (Negative != NULL) {
      *Negative = IsNegative;
    }
  }
  return Status;
}
// EfiFloat32Encode
/// Encode a floating point from components
/// @param Value    The value to encode into a floating point
/// @param Exponent The binary exponent of the floating point
/// @param Negative Whether the floating point is negative or not
/// @param Float    On output, the floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           If the floating point was encoded successfully
EFI_STATUS
EFIAPI
EfiFloat32Encode (
  IN  UINT32   Value,
  IN  INT32    Exponent,
  IN  BOOLEAN  Negative,
  OUT FLOAT32 *Float
) {
  FLOAT_BITS Bits;
  // Check the parameters
  if (Float == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the highest set bit to maximize precision
  while (EFI_BITS_ARE_UNSET(Value, EFI_BIT(31))) {
    Value <<= 1;
  }
  Value >>= 8;
  // Set the exponent component
  Bits.Bits = EFI_BITMASKFIELD(0, Exponent + 127, 23, 30);
  // Set the fractional component
  Bits.Bits |= EFI_BITFIELD(Value, 0, 22);
  // Set negative component
  if (Negative) {
    Bits.Bits |= EFI_BIT(31);
  }
  // Return the float and success;
  *Float = Bits.Float32;
  return EFI_SUCCESS;
}
// EfiFloat64Encode
/// Encode a floating point from components
/// @param Value    The value to encode into a floating point
/// @param Exponent The binary exponent of the floating point
/// @param Negative Whether the floating point is negative or not
/// @param Float    On output, the floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           If the floating point was encoded successfully
EFI_STATUS
EFIAPI
EfiFloat64Encode (
  IN  UINT64   Value,
  IN  INT64    Exponent,
  IN  BOOLEAN  Negative,
  OUT FLOAT64 *Float
) {
  FLOAT_BITS Bits;
  // Check the parameters
  if (Float == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the highest set bit to maximize precision
  while (EFI_BITS_ARE_UNSET(Value, EFI_BIT(63))) {
    Value <<= 1;
  }
  Value >>= 11;
  // Set the exponent component
  Bits.Bits = EFI_BITMASKFIELD(0, Exponent + 1023, 52, 62);
  // Set the fractional component
  Bits.Bits |= EFI_BITFIELD(Value, 0, 51);
  // Set negative component
  if (Negative) {
    Bits.Bits |= EFI_BIT(63);
  }
  // Return the float and success;
  *Float = Bits.Float64;
  return EFI_SUCCESS;
}
// EfiFloat32Normalize
/// Normalize a floating point
/// @param Float On input, the floating point to normalize, on output, the normalized floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           The floating point was normalized
EFI_STATUS
EFIAPI
EfiFloat32Normalize (
  IN OUT FLOAT32 *Float
) {
  FLOAT_BITS Bits;
  UINT32     Value;
  INT32      Exponent;
  // Check parameters
  if (Float == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the floating point components
  Bits.Float32 = *Float;
  Value = (UINT32)EFI_BITFIELD(Bits.Bits, 0, 22);
  Exponent = ((INT32)EFI_BITFIELD(Bits.Bits, 23, 30)) - 127;
  // Normalize the components
  if (Value != 0) {
    while (!EFI_BIT_IS_UNSET(Value, 22)) {
      Value <<= 1;
      --Exponent;
    }
  }
  // Reencode the float point with the normalized values
  return EfiFloat32Encode(Value, Exponent, EFI_BIT_IS_SET(Bits.Bits, 31) ? TRUE : FALSE, Float);
}
// EfiFloat64Normalize
/// Normalize a floating point
/// @param Float On input, the floating point to normalize, on output, the normalized floating point
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_SUCCESS           The floating point was normalized
EFI_STATUS
EFIAPI
EfiFloat64Normalize (
  IN OUT FLOAT64 *Float
) {
  FLOAT_BITS Bits;
  UINT64     Value;
  INT64      Exponent;
  // Check parameters
  if (Float == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the floating point components
  Bits.Float64 = *Float;
  Value = (UINT64)EFI_BITFIELD(Bits.Bits, 0, 51);
  Exponent = ((INT64)EFI_BITFIELD(Bits.Bits, 52, 62)) - 1023;
  // Normalize the components
  if (Value != 0) {
    while (!EFI_BIT_IS_UNSET(Value, 51)) {
      Value <<= 1;
      --Exponent;
    }
  }
  // Reencode the float point with the normalized values
  return EfiFloat64Encode(Value, Exponent, EFI_BIT_IS_SET(Bits.Bits, 63) ? TRUE : FALSE, Float);
}

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

// EfiCpuidIntrinsic
/// Call CPUID instruction
/// @param Function    The function to execute
/// @param SubFunction The sub function to execute
/// @param Eax         On output, the register returned
/// @param Ebx         On output, the register returned
/// @param Ecx         On output, the register returned
/// @param Edx         On output, the register returned
//EXTERN
//VOID
//EFIAPI
//EfiCpuidIntrinsic (
//  IN  UINT32  Function,
//  IN  UINT32  SubFunction,
//  OUT UINT32 *Eax OPTIONAL,
//  OUT UINT32 *Ebx OPTIONAL,
//  OUT UINT32 *Ecx OPTIONAL,
//  OUT UINT32 *Edx OPTIONAL
//);
// EfiCpuid
/// Call CPUID instruction
/// @param Function The function to execute
/// @param Eax      On output, the register returned
/// @param Ebx      On output, the register returned
/// @param Ecx      On output, the register returned
/// @param Edx      On output, the register returned
VOID
EFIAPI
EfiCpuid (
  IN  UINT32  Function,
  OUT UINT32 *Eax OPTIONAL,
  OUT UINT32 *Ebx OPTIONAL,
  OUT UINT32 *Ecx OPTIONAL,
  OUT UINT32 *Edx OPTIONAL
) {
  EfiCpuidEx(Function, 0, Eax, Ebx, Ecx, Edx);
}

#endif // EFI_ARCH_IA32 || EFI_ARCH_X64
