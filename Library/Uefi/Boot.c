///
/// @file Library/Uefi/Boot.c
///
/// UEFI boot services
///

#include <Uefi.h>

// gEfiEventGroupExitBootServicesGuid
/// The exit boot services event group identifier
EFI_GUID gEfiEventGroupExitBootServicesGuid = EFI_EVENT_GROUP_EXIT_BOOT_SERVICES;
// gEfiEventGroupVirtualAddressChangeGuid
/// The virtual address map changed event group identifier
EFI_GUID gEfiEventGroupVirtualAddressChangeGuid = EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE;
// gEfiEventGroupMemoryMapChangeGuid
/// The memory map changed event group identifier
EFI_GUID gEfiEventGroupMemoryMapChangeGuid = EFI_EVENT_GROUP_MEMORY_MAP_CHANGE;
// gEfiEventGroupReadyToBootGuid
/// The ready to boot event group identifier
EFI_GUID gEfiEventGroupReadyToBootGuid = EFI_EVENT_GROUP_READY_TO_BOOT;
// gEfiEventGroupResetSystemGuid
/// The reset system event group identifier
EFI_GUID gEfiEventGroupResetSystemGuid = EFI_EVENT_GROUP_RESET_SYSTEM;

// EfiRaiseTPL
/// Raises a task’s priority level and returns its previous level
/// @param NewTpl The new task priority level that must be greater than or equal to the current task priority level
/// @return The previous task priority level
EFI_TPL
EFIAPI
EfiRaiseTPL (
  IN EFI_TPL NewTpl
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->RaiseTPL == NULL)) {
    return TPL_APPLICATION;
  }
  return gEfiBootServices->RaiseTPL(NewTpl);
}
// EfiRestoreTPL
/// Restores a task’s priority level to its previous value
/// @param OldTpl The previous task priority level to restore
VOID
EFIAPI
EfiRestoreTPL (
  IN EFI_TPL OldTpl
) {
  if ((gEfiBootServices != NULL) && (gEfiBootServices->RestoreTPL != NULL)) {
    gEfiBootServices->RestoreTPL(OldTpl);
  }
}

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
EFI_STATUS
EFIAPI
EfiCreateEvent (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  VOID             *NotifyContext OPTIONAL,
  OUT EFI_EVENT        *Event
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->CreateEvent == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CreateEvent(Type, NotifyTpl, NotifyFunction, NotifyContext, Event);
}
// EfiSetTimer
/// Sets the type of timer and the trigger time for a timer event
/// @param Event       The timer event that is to be signaled at the specified time
/// @param Type        The type of time that is specified in TriggerTime
/// @param TriggerTime The number of 100ns units until the timer expires. If TriggerTime is 0, then if
///                     Type is TimerRelative the timer event will be signaled on the next timer tick,
///                     or if Type is TimerPeriodic the timer event will be signaled on every timer tick
/// @retval EFI_SUCCESS           The event has been set to be signaled at the requested time
/// @retval EFI_INVALID_PARAMETER Event or Type is not valid
EFI_STATUS
EFIAPI
EfiSetTimer (
  IN EFI_EVENT       Event,
  IN EFI_TIMER_DELAY Type,
  IN UINT64          TriggerTime
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->SetTimer == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->SetTimer(Event, Type, TriggerTime);
}
// EfiWaitForEvent
/// Stops execution until an event is signaled
/// @param NumberOfEvents The number of events on which to wait
/// @param Event          The events on which to wait
/// @param Index          On output, the index of the event which satisfied the wait condition
/// @retval EFI_SUCCESS           The event indicated by Index was signaled
/// @retval EFI_INVALID_PARAMETER NumberOfEvents is 0
/// @retval EFI_INVALID_PARAMETER The event indicated by Index is of type EVT_NOTIFY_SIGNAL
/// @retval EFI_UNSUPPORTED       The current TPL is not TPL_APPLICATION
EFI_STATUS
EFIAPI
EfiWaitForEvent (
  IN  UINTN      NumberOfEvents,
  IN  EFI_EVENT *Event,
  OUT UINTN     *Index
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->WaitForEvent == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->WaitForEvent(NumberOfEvents, Event, Index);
}
// EfiSignalEvent
/// Signals an event
/// @param Event The event to signal
/// @retval EFI_SUCCESS The event was signaled
EFI_STATUS
EFIAPI
EfiSignalEvent (
  IN EFI_EVENT Event
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->SignalEvent == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->SignalEvent(Event);
}
// EfiCloseEvent
/// Closes an event
/// @param Event The event to close
/// @retval EFI_SUCCESS The event has been closed
EFI_STATUS
EFIAPI
EfiCloseEvent (
  IN EFI_EVENT Event
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->CloseEvent == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CloseEvent(Event);
}
// EfiCheckEvent
/// Checks whether an event is in the signaled state
/// @param Event The event to check
/// @retval EFI_SUCCESS           The event is in the signaled state
/// @retval EFI_NOT_READY         The event is not in the signaled state
/// @retval EFI_INVALID_PARAMETER Event is of type EVT_NOTIFY_SIGNAL
EFI_STATUS
EFIAPI
EfiCheckEvent (
  IN EFI_EVENT Event
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->CheckEvent == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CheckEvent(Event);
}

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
EFI_STATUS
EFIAPI
EfiInstallProtocolInterface (
  IN OUT EFI_HANDLE         *Handle,
  IN     EFI_GUID           *Protocol,
  IN     EFI_INTERFACE_TYPE  InterfaceType,
  IN     VOID               *Interface OPTIONAL
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->InstallProtocolInterface == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->InstallProtocolInterface(Handle, Protocol, InterfaceType, Interface);
}
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
EFI_STATUS
EFIAPI
EfiReinstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *OldInterface,
  IN VOID       *NewInterface
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->ReinstallProtocolInterface == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->ReinstallProtocolInterface(Handle, Protocol, OldInterface, NewInterface);
}
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
EFI_STATUS
EFIAPI
EfiUninstallProtocolInterface (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN VOID       *Interface
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->UninstallProtocolInterface == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->UninstallProtocolInterface(Handle, Protocol, Interface);
}
// EfiInstallMultipleProtocolInterfaces
/// Installs one or more protocol interfaces into the boot services environment
/// @param Handle On input, the handle on which to install the new protocol interfaces, or if NULL, on output, a newly allocated handle
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were installed
/// @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in the handle database
/// @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols
/// @retval EFI_INVALID_PARAMETER Handle is NULL
/// @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle
EFI_STATUS
EFIAPI
EfiInstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE *Handle,
  ...
) {
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_GUID   *Guid;
  VOID       *Interface;
  VA_LIST     Marker;
  if (gEfiBootServices == NULL) {
    return EFI_UNSUPPORTED;
  }
  VA_START(Marker, Handle);
  // Get if the 1.10 interface installer is available
  if (gEfiBootServices->Hdr.Revision >= EFI_1_10_SYSTEM_TABLE_REVISION) {
    if (gEfiBootServices->InstallMultipleProtocolInterfaces != NULL) {
      // Install multiple protocols
      do {
        // Get the protocol identifier
        Guid = VA_ARG(Marker, EFI_GUID *);
        if (Guid == NULL) {
          break;
        }
        // Get the protocol interface
        Interface = VA_ARG(Marker, VOID *);
        // Install the protocol interface
        Status = gEfiBootServices->InstallMultipleProtocolInterfaces(Handle, Guid, Interface, NULL);
      } while (!EFI_ERROR(Status));
      VA_END(Marker);
      return Status;
    }
  }
  // Check if original interface installer is available
  if (gEfiBootServices->InstallProtocolInterface == NULL) {
    VA_END(Marker);
    return EFI_UNSUPPORTED;
  }
  // Install multiple protocols
  do {
    // Get the protocol identifier
    Guid = VA_ARG(Marker, EFI_GUID *);
    if (Guid == NULL) {
      Status = EFI_SUCCESS;
      break;
    }
    // Get the protocol interface
    Interface = VA_ARG(Marker, VOID *);
    // Install the protocol interface
    Status = gEfiBootServices->InstallProtocolInterface(Handle, Guid, EFI_NATIVE_INTERFACE, Interface);
  } while (!EFI_ERROR(Status));
  VA_END(Marker);
  return Status;
}
// EfiUninstallMultipleProtocolInterfaces
/// Removes one or more protocol interfaces into the boot services environment
/// @param Handle The handle from which to remove the protocol interfaces
/// @param ...    A NULL terminated variable argument list containing pairs of protocol GUIDs and protocol interfaces
/// @retval EFI_SUCCESS           All the protocol interfaces were removed
/// @retval EFI_INVALID_PARAMETER One of the protocol interfaces was not previously installed on Handle
EFI_STATUS
EFIAPI
EfiUninstallMultipleProtocolInterfaces (
  IN EFI_HANDLE Handle,
  ...
) {
  EFI_STATUS  Status;
  EFI_GUID   *Guid;
  VOID       *Interface;
  VA_LIST     Marker;
  if (gEfiBootServices == NULL) {
    return EFI_UNSUPPORTED;
  }
  VA_START(Marker, Handle);
  // Get if the 1.10 interface installer is available
  if (gEfiBootServices->Hdr.Revision >= EFI_1_10_SYSTEM_TABLE_REVISION) {
    if (gEfiBootServices->UninstallMultipleProtocolInterfaces != NULL) {
      // Install multiple protocols
      do {
        // Get the protocol identifier
        Guid = VA_ARG(Marker, EFI_GUID *);
        if (Guid == NULL) {
          Status = EFI_SUCCESS;
          break;
        }
        // Get the protocol interface
        Interface = VA_ARG(Marker, VOID *);
        // Install the protocol interface
        Status = gEfiBootServices->UninstallMultipleProtocolInterfaces(Handle, Guid, Interface, NULL);
      } while (!EFI_ERROR(Status));
      VA_END(Marker);
      return Status;
    }
  }
  // Check if original interface installer is available
  if (gEfiBootServices->UninstallProtocolInterface == NULL) {
    VA_END(Marker);
    return EFI_UNSUPPORTED;
  }
  // Install multiple protocols
  do {
    // Get the protocol identifier
    Guid = VA_ARG(Marker, EFI_GUID *);
    if (Guid == NULL) {
      Status = EFI_SUCCESS;
      break;
    }
    // Get the protocol interface
    Interface = VA_ARG(Marker, VOID *);
    // Install the protocol interface
    Status = gEfiBootServices->UninstallProtocolInterface(Handle, Guid, Interface);
  } while (!EFI_ERROR(Status));
  VA_END(Marker);
  return Status;
}
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
EFI_STATUS
EFIAPI
EfiHandleProtocol (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface
) {
  // Check boot services and handle protocol method is valid
  if ((gEfiBootServices == NULL) || (gEfiBootServices->HandleProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Handle protocol
  return gEfiBootServices->HandleProtocol(Handle, Protocol, Interface);
}
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
EFI_STATUS
EFIAPI
EfiRegisterProtocolNotify (
  IN  EFI_GUID   *Protocol,
  IN  EFI_EVENT   Event,
  OUT VOID      **Registration
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->RegisterProtocolNotify == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->RegisterProtocolNotify(Protocol, Event, Registration);
}
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
EFI_STATUS
EFIAPI
EfiLocateHandle (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID               *Protocol OPTIONAL,
  IN     VOID                   *SearchKey OPTIONAL,
  IN OUT UINTN                  *BufferSize,
  OUT    EFI_HANDLE             *Buffer
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->LocateHandle == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->LocateHandle(SearchType, Protocol, SearchKey, BufferSize, Buffer);
}
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
EFI_STATUS
EFIAPI
EfiLocateDevicePath (
  IN     EFI_GUID                  *Protocol,
  IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
  OUT    EFI_HANDLE                *Device
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->LocateDevicePath == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->LocateDevicePath(Protocol, DevicePath, Device);
}
// EfiInstallConfigurationTable
/// Adds, updates, or removes a configuration table entry from the EFI System Table
/// @param Guid  The configuration table unique identifier for the entry to add, update, or remove
/// @param Table The configuration table for the entry to add or update, may be NULL to remove
/// @retval EFI_SUCCESS           The (Guid, Table) pair was added, updated, or removed
/// @retval EFI_INVALID_PARAMETER Guid is NULL
/// @retval EFI_NOT_FOUND         An attempt was made to delete a nonexistent entry
/// @retval EFI_OUT_OF_RESOURCES  There is not enough memory available to complete the operation
EFI_STATUS
EFIAPI
EfiInstallConfigurationTable (
  IN EFI_GUID *Guid,
  IN VOID     *Table
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->InstallConfigurationTable == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->InstallConfigurationTable(Guid, Table);
}

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
EFI_STATUS
EFIAPI
EfiLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->LoadImage == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->LoadImage(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);
}
// EfiStartImage
/// Transfers control to a loaded image’s entry point
/// @param ImageHandle  Handle of image to be started
/// @param ExitDataSize On output, the size, in bytes, of ExitData. If ExitData is NULL, then this parameter is ignored and the contents of ExitDataSize are not modified
/// @param ExitData     On output, a data buffer that includes a Null-terminated string, optionally followed by additional binary data.
///                      The string is a description that the caller may use to further indicate the reason for the image’s exit
/// @retval EFI_INVALID_PARAMETER  ImageHandle is either an invalid image handle or the image has already been initialized with StartImage 
/// @retval EFI_SECURITY_VIOLATION The current platform policy specifies that the image should not be started
/// @retval "Exit code from image" Exit code from image
EFI_STATUS
EFIAPI
EfiStartImage (
  IN  EFI_HANDLE   ImageHandle,
  OUT UINTN       *ExitDataSize,
  OUT CHAR16     **ExitData OPTIONAL
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->StartImage == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->StartImage(ImageHandle, ExitDataSize, ExitData);
}
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
EFI_STATUS
EFIAPI
EfiExit (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_STATUS  ExitStatus,
  IN UINTN       ExitDataSize,
  IN CHAR16     *ExitData OPTIONAL
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->Exit == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->Exit(ImageHandle, ExitStatus, ExitDataSize, ExitData);
}
// EfiUnloadImage
/// Unloads an image
/// @param ImageHandle Handle that identifies the image to be unloaded
/// @retval EFI_SUCCESS                     The image has been unloaded
/// @retval EFI_UNSUPPORTED                 The image has been started, and does not support unload
/// @retval EFI_INVALID_PARAMETER           ImageHandle is not a valid image handle
/// @retval "Exit code from unload handler" Exit code from the image unload function
EFI_STATUS
EFIAPI
EfiUnloadImage (
  IN EFI_HANDLE ImageHandle
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->UnloadImage == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->UnloadImage(ImageHandle);
}
// EfiExitBootServices
/// Terminates all boot services
/// @param ImageHandle Handle that identifies the exiting image
/// @param MapKey      Key to the latest memory map
/// @retval EFI_SUCCESS           Boot services have been terminated
/// @retval EFI_INVALID_PARAMETER MapKey is incorrect
EFI_STATUS
EFIAPI
EfiExitBootServices (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->ExitBootServices == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->ExitBootServices(ImageHandle, MapKey);
}

// EfiGetNextMonotonicCount
/// Returns a monotonically increasing count for the platform.
/// @param Count On output, the monotonic count
/// @retval EFI_SUCCESS The next monotonic count was returned
/// @retval EFI_DEVICE_ERROR The device is not functioning properly
/// @retval EFI_INVALID_PARAMETER Count is NULL
EFI_STATUS
EFIAPI
EfiGetNextMonotonicCount (
  OUT UINT64 *Count
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->GetNextMonotonicCount == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->GetNextMonotonicCount(Count);
}
// EfiStall
/// Induces a fine-grained stall
/// @param Microseconds The number of microseconds to stall execution
/// @retval EFI_SUCCESS Execution was stalled at least the requested number of Microseconds
EFI_STATUS
EFIAPI
EfiStall (
  IN UINTN Microseconds
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->Stall == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->Stall(Microseconds);
}
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
EFI_STATUS
EFIAPI
EfiSetWatchdogTimer (
  IN UINTN   Timeout,
  IN UINT64  WatchdogCode,
  IN UINTN   DataSize,
  IN CHAR16 *WatchdogData OPTIONAL
) {
  if ((gEfiBootServices == NULL) || (gEfiBootServices->SetWatchdogTimer == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->SetWatchdogTimer(Timeout, WatchdogCode, DataSize, WatchdogData);
}

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
EFI_STATUS
EFIAPI
EfiConnectController (
  IN EFI_HANDLE                ControllerHandle,
  IN EFI_HANDLE               *DriverImageHandle OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL,
  IN BOOLEAN                   Recursive
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->ConnectController == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->ConnectController(ControllerHandle, DriverImageHandle, RemainingDevicePath, Recursive);
}
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
EFI_STATUS
EFIAPI
EfiDisconnectController (
  IN EFI_HANDLE ControllerHandle,
  IN EFI_HANDLE DriverImageHandle OPTIONAL,
  IN EFI_HANDLE ChildHandle OPTIONAL
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->DisconnectController == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->DisconnectController(ControllerHandle, DriverImageHandle, ChildHandle);
}

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
EFI_STATUS
EFIAPI
EfiOpenProtocol (
  IN  EFI_HANDLE   Handle,
  IN  EFI_GUID    *Protocol,
  OUT VOID       **Interface OPTIONAL,
  IN  EFI_HANDLE   AgentHandle,
  IN  EFI_HANDLE   ControllerHandle,
  IN  UINT32       Attributes
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->OpenProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->OpenProtocol(Handle, Protocol, Interface, AgentHandle, ControllerHandle, Attributes);
}
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
EFI_STATUS
EFIAPI
EfiCloseProtocol (
  IN EFI_HANDLE  Handle,
  IN EFI_GUID   *Protocol,
  IN EFI_HANDLE  AgentHandle,
  IN EFI_HANDLE  ControllerHandle
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->CloseProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CloseProtocol(Handle, Protocol, AgentHandle, ControllerHandle);
}
// EfiOpenProtocolInformation
/// Retrieves the list of agents that currently have a protocol interface opened
/// @param Handle      The handle for the protocol interface that is being queried.
/// @param Protocol    The published unique identifier of the protocol
/// @param EntryBuffer On output, an array of open protocol information entries, which must be freed
/// @param EntryCount  On output, the count of entries
/// @retval EFI_SUCCESS          The open protocol information was returned in EntryBuffer, and the count of entries was returned EntryCount
/// @retval EFI_NOT_FOUND        Handle does not support the protocol specified by Protocol
/// @retval EFI_OUT_OF_RESOURCES There are not enough resources available to allocate EntryBuffer
EFI_STATUS
EFIAPI
EfiOpenProtocolInformation (
  IN  EFI_HANDLE                            Handle,
  IN  EFI_GUID                             *Protocol,
  OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
  OUT UINTN                                *EntryCount
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->OpenProtocolInformation == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->OpenProtocolInformation(Handle, Protocol, EntryBuffer, EntryCount);
}

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
EFI_STATUS
EFIAPI
EfiProtocolsPerHandle (
  IN  EFI_HANDLE    Handle,
  OUT EFI_GUID   ***ProtocolBuffer,
  OUT UINTN        *ProtocolBufferCount
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->ProtocolsPerHandle == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->ProtocolsPerHandle(Handle, ProtocolBuffer, ProtocolBufferCount);
}
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
EFI_STATUS
EFIAPI
EfiLocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE   SearchType,
  IN     EFI_GUID                *Protocol OPTIONAL,
  IN     VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN                   *NoHandles,
  OUT    EFI_HANDLE             **Buffer
) {
  if (gEfiBootServices == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Check for support, if not pseudo provide support
  if (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) {
    EFI_STATUS  Status;
    UINTN       Size = 0;
    EFI_HANDLE *Handles = NULL;
    if (gEfiBootServices->LocateHandle == NULL) {
      return EFI_UNSUPPORTED;
    }
    if (NoHandles != NULL) {
      return EFI_INVALID_PARAMETER;
    }
    // Get the size of the handle buffer
    Status = gEfiBootServices->LocateHandle(SearchType, Protocol, SearchKey, &Size, Handles);
    if (Status != EFI_BUFFER_TOO_SMALL) {
      return Status;
    }
    if (Buffer != NULL) {
      // Allocate handle buffer
      Handles = (EFI_HANDLE *)EfiAllocate(Size);
      if (Handles == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      // Get the handle buffer
      Status = gEfiBootServices->LocateHandle(SearchType, Protocol, SearchKey, &Size, Handles);
      if (EFI_ERROR(Status)) {
        // Free the handles on error
        EfiFreePool(Handles);
      } else {
        // Return the handles
        *Buffer = Handles;
      }
    }
    // Set the number of handles returned
    if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
      *NoHandles = (Size / sizeof(EFI_HANDLE));
    }
    // Return the status code
    return Status;
  } else if (gEfiBootServices->LocateHandleBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->LocateHandleBuffer(SearchType, Protocol, SearchKey, NoHandles, Buffer);
}
// EfiLocateProtocol
/// Returns the first protocol instance that matches the given protocol
/// @param Protocol     Provides the protocol for which to search
/// @param Registration Protocol registration update key
/// @param Interface    On return, the first interface that matches Protocol and Registration
/// @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in Interface
/// @retval EFI_INVALID_PARAMETER Interface or Protocol is NULL
/// @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and Registration
EFI_STATUS
EFIAPI
EfiLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID     **Interface
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->LocateProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->LocateProtocol(Protocol, Registration, Interface);
}

// EfiCalculateCrc32
/// Computes and returns a 32-bit CRC for a data buffer.
/// @param Data     The buffer on which the 32-bit CRC is to be computed
/// @param DataSize The number of bytes in the buffer Data
/// @param Crc32    On output, the 32-bit CRC that was computed for the data buffer specified by Data and DataSize
/// @retval EFI_SUCCESS           The 32-bit CRC was computed for the data buffer and returned in Crc32
/// @retval EFI_INVALID_PARAMETER Data is NULL
/// @retval EFI_INVALID_PARAMETER Crc32 is NULL
/// @retval EFI_INVALID_PARAMETER DataSize is 0
EFI_STATUS
EFIAPI
EfiCalculateCrc32 (
  IN  VOID   *Data,
  IN  UINTN   DataSize,
  OUT UINT32 *Crc32
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_1_10_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->CalculateCrc32 == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CalculateCrc32(Data, DataSize, Crc32);
}

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
EFI_STATUS
EFIAPI
EfiCreateEventEx (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction OPTIONAL,
  IN  CONST VOID       *NotifyContext OPTIONAL,
  IN  CONST EFI_GUID   *EventGroup OPTIONAL,
  OUT EFI_EVENT        *Event
) {
  if ((gEfiBootServices == NULL) ||
      (gEfiBootServices->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) ||
      (gEfiBootServices->CreateEventEx == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return gEfiBootServices->CreateEventEx(Type, NotifyTpl, NotifyFunction, NotifyContext, EventGroup, Event);
}

// EfiLoadedImage
/// Get the loaded image protocol interface for an image
/// @param ImageHandle The image handle for which to get the loaded image protocol interface
/// @return The loaded image protocol interface
EFI_LOADED_IMAGE_PROTOCOL *
EFIAPI
EfiLoadedImage (
  IN EFI_HANDLE ImageHandle OPTIONAL
) {
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = NULL;
  // Attempt to use the loaded image protocol to use same device handle as this image
  if (EFI_ERROR(EfiHandleProtocol((ImageHandle == NULL) ? gEfiImageHandle : ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&LoadedImage))) {
    return NULL;
  }
  return LoadedImage;
}
