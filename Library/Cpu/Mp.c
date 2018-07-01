///
/// @file Library/Cpu/Mp.c
///
/// Multiprocessor CPU features
///

#include <Uefi.h>

#include <Uefi/Protocol/MpService.h>

// gEfiMpServiceProtocolGuid
/// The multiprocessor services protocol identifier
EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;

// EfiMpServicesProtocol
/// Get the multiprocessor services protocol
/// @return The multiprocessor services protocol or NULL if there was an error
STATIC
EFI_MP_SERVICES_PROTOCOL *
EFIAPI
GetMpServicesProtocol (
  VOID
) {
  EFI_MP_SERVICES_PROTOCOL *Interface = NULL;
  // Locate the multiprocessor services protocol
  if (EFI_ERROR(EfiLocateProtocol(&gEfiMpServiceProtocolGuid, NULL, (VOID **)&Interface))) {
    Interface = NULL;
  }
  return Interface;
}

// EfiMpServicesGetNumberOfProcessors
/// Get the number of logical processors and the number that are currently enabled
/// @param NumberOfProcessors        On output, the total number of logical processors in the system
/// @param NumberOfEnabledProcessors On output, the number of enabled logical processors that exist in system
/// @retval EFI_INVALID_PARAMETER   NumberOfProcessors or NumberOfEnabledProcessors is NULL
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_SUCCESS             The number of logical processors and enabled logical processors was retrieved
EFI_STATUS
EFIAPI
EfiMpServicesGetNumberOfProcessors (
  OUT UINTN *NumberOfProcessors OPTIONAL,
  OUT UINTN *NumberOfEnabledProcessors OPTIONAL
) {
  // Get protocol
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  UINTN                     MpNumberOfProcessors = 1;
  UINTN                     MpNumberOfEnabledProcessors = 1;
  if ((MpServices == NULL) || (MpServices->GetNumberOfProcessors == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  Status = MpServices->GetNumberOfProcessors(MpServices, &MpNumberOfProcessors, &MpNumberOfEnabledProcessors);
  // Check that the results are sane
  if (MpNumberOfProcessors == 0) {
    MpNumberOfProcessors = 1;
  }
  if (MpNumberOfEnabledProcessors == 0) {
    MpNumberOfEnabledProcessors = 1;
  }
  if (MpNumberOfEnabledProcessors > MpNumberOfProcessors) {
    MpNumberOfEnabledProcessors = MpNumberOfProcessors;
  }
  // Return the values requested
  if (!EFI_ERROR(Status)) {
    if (NumberOfProcessors != NULL) {
      *NumberOfProcessors = MpNumberOfProcessors;
    }
    if (NumberOfEnabledProcessors != NULL) {
      *NumberOfEnabledProcessors = MpNumberOfEnabledProcessors;
    }
  }
  return Status;
}
// EfiMpServicesGetProcessorInfo
/// Get processor information
/// @param ProcessorNumber      The handle number of processor
/// @param ProcessorInformation On output, the processor information
/// @retval EFI_INVALID_PARAMETER If ProcessorInfoBuffer is NULL
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_NOT_FOUND         The processor with the handle specified by ProcessorNumber does not exist in the platform
/// @retval EFI_SUCCESS           The processor information was returned
EFI_STATUS
EFIAPI
EfiMpServicesGetProcessorInfo (
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION *ProcessorInformation
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->GetProcessorInfo == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->GetProcessorInfo(MpServices, ProcessorNumber, ProcessorInformation);
}
// EfiMpServicesStartupAllAPs
/// Execute a function on all enabled processors
/// @param Procedure             The procedure function to execute on the processors
/// @param SingleThread          Whether to execute the function specified one by one, in ascending order of processor handle number or execute the function specified on each processor simultaneously
/// @param WaitEvent             If NULL wait for all processors to finish or timeout, otherwise the event to be signaled when all processors finish or timeout
/// @param TimeoutInMicroseconds The time limit in microseconds before the processor procedures fail or zero for infinity
/// @param ProcedureArgument     The procedure parameter for the procedure function
/// @param FailedCpuList         On output, the list of identifiers of failed processors ending with END_OF_CPU_LIST or NULL if no processors failed to start
/// @retval EFI_INVALID_PARAMETER If Procedure is NULL
/// @retval EFI_UNSUPPORTED       A non-blocking mode request was made after the UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was signaled
/// @retval EFI_DEVICE_ERROR      Caller processor is AP
/// @retval EFI_NOT_STARTED       No enabled APs exist in the system
/// @retval EFI_NOT_READY         Any enabled APs are busy
/// @retval EFI_TIMEOUT           In blocking mode, the timeout expired before all enabled APs have finished
/// @retval EFI_SUCCESS           In blocking mode, all APs have finished before the timeout expired
/// @retval EFI_SUCCESS           In non-blocking mode, function has been dispatched to all enabled APs
EFI_STATUS
EFIAPI
EfiMpServicesStartupAllAPs (
  IN  EFI_AP_PROCEDURE   Procedure,
  IN  BOOLEAN            SingleThread,
  IN  EFI_EVENT          WaitEvent OPTIONAL,
  IN  UINTN              TimeoutInMicroseconds,
  IN  VOID              *ProcedureArgument OPTIONAL,
  OUT UINTN            **FailedCpuList OPTIONAL
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->StartupAllAPs == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->StartupAllAPs(MpServices, Procedure, SingleThread, WaitEvent, TimeoutInMicroseconds, ProcedureArgument, FailedCpuList);
}
// EfiMpServicesStartupThisAP
/// Execute a function on an enabled processor
/// @param Procedure             The function to be run on the designated processor
/// @param ProcessorNumber       The handle number of the processor on which to execute the specified function
/// @param WaitEvent             If NULL wait for the processor to finish or timeout, otherwise the event to be signaled when the processor finishes or times out
/// @param TimeoutInMicroseconds The time limit in microseconds before the processor procedure fails or zero for infinity
/// @param ProcedureArgument     The procedure parameter passed to the specified function
/// @param Finished              On output, whether the function finished or not
/// @retval EFI_INVALID_PARAMETER   Procedure is NULL
/// @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP or disabled AP
/// @retval EFI_UNSUPPORTED         A non-blocking mode request was made after the UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was signaled
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_TIMEOUT             In blocking mode, the timeout expired before the specified AP has finished
/// @retval EFI_NOT_READY           The specified AP is busy
/// @retval EFI_NOT_FOUND           The processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_SUCCESS             In blocking mode, specified AP finished before the timeout expires
/// @retval EFI_SUCCESS             In non-blocking mode, the function has been dispatched to specified AP
EFI_STATUS
EFIAPI
EfiMpServicesStartupThisAP (
  IN  EFI_AP_PROCEDURE  Procedure,
  IN  UINTN             ProcessorNumber,
  IN  EFI_EVENT         WaitEvent OPTIONAL,
  IN  UINTN             TimeoutInMicroseconds,
  IN  VOID             *ProcedureArgument OPTIONAL,
  OUT BOOLEAN          *Finished OPTIONAL
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->StartupThisAP == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->StartupThisAP(MpServices, Procedure, ProcessorNumber, WaitEvent, TimeoutInMicroseconds, ProcedureArgument, Finished);
}
// EfiMpServicesSwitchBSP
/// Switches the requested processor to be the boot services processor
/// @param ProcessorNumber The processer handle number to set as the new boot service processor
/// @param EnableOldBSP    Whether the old boot services processor should remain enabled or not
/// @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the current BSP or a disabled AP
/// @retval EFI_UNSUPPORTED         Switching the BSP cannot be completed prior to this service returning
/// @retval EFI_UNSUPPORTED         Switching the BSP is not supported
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_NOT_FOUND           The processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_NOT_READY           The specified AP is busy
/// @retval EFI_SUCCESS             BSP successfully switched
EFI_STATUS
EFIAPI
EfiMpServicesSwitchBSP ( 
  IN UINTN   ProcessorNumber,
  IN BOOLEAN EnableOldBSP
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->SwitchBSP == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->SwitchBSP(MpServices, ProcessorNumber, EnableOldBSP);
}
// EfiMpServicesEnableDisableAP
/// Enable or disable a processor
/// @param ProcessorNumber The processor handle number
/// @param EnableAP        Whether to enable or disable the processor
/// @param HealthFlag      On output, the health of the processor using PROCESSOR_HEALTH_STATUS_BIT
/// @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP
/// @retval EFI_UNSUPPORTED         Enabling or disabling an AP cannot be completed prior to this service returning
/// @retval EFI_UNSUPPORTED         Enabling or disabling an AP is not supported
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_NOT_FOUND           Processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_SUCCESS             The specified AP was enabled or disabled successfully
EFI_STATUS
EFIAPI
EfiMpServicesEnableDisableAP (
  IN  UINTN    ProcessorNumber,
  IN  BOOLEAN  EnableAP,
  OUT UINT32  *HealthFlag OPTIONAL
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->EnableDisableAP == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->EnableDisableAP(MpServices, ProcessorNumber, EnableAP, HealthFlag);
}
// EfiMpServicesWhoAmI
/// Get the handle number for the calling processor
/// @param ProcessorNumber On output, the current processor handle number
/// @retval EFI_INVALID_PARAMETER If ProcessorNumber is NULL
/// @retval EFI_SUCCESS           The current processor handle number was returned
EFI_STATUS
EFIAPI
EfiMpServicesWhoAmI (
  OUT UINTN *ProcessorNumber
) {
  // Get protocol
  EFI_MP_SERVICES_PROTOCOL *MpServices = GetMpServicesProtocol();
  if ((MpServices == NULL) || (MpServices->WhoAmI == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call protocol method
  return MpServices->WhoAmI(MpServices, ProcessorNumber);
}
