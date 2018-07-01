///
/// @file Include/Uefi/Protocol/MpService.h
///
/// Multiprocessor services protocol
///

#pragma once
#ifndef __MP_SERVICE_PROTOCOL_HEADER__
#define __MP_SERVICE_PROTOCOL_HEADER__

#include <Uefi.h>

// EFI_MP_SERVICES_PROTOCOL_GUID
/// The Multipreoccessor service protocol identifier
#define EFI_MP_SERVICES_PROTOCOL_GUID { 0x3fdda605, 0xa76e, 0x4f46, { 0xad, 0x29, 0x12, 0xf4, 0x53, 0x1b, 0x3d, 0x08 } }

// EFI_MP_SERVICES_PROTOCOL
/// Multiprocessor service protocol interface
typedef struct EFI_MP_SERVICES_PROTOCOL EFI_MP_SERVICES_PROTOCOL;

// END_OF_CPU_LIST
/// Terminator for a list of failed CPUs returned by StartAllAPs()
#define END_OF_CPU_LIST MAX_UINT32

// PROCESSOR_AS_BSP_BIT
/// Whether the processor is BSP or if not, an AP
#define PROCESSOR_AS_BSP_BIT EFI_BIT(0)
// PROCESSOR_ENABLED_BIT
/// Whether the processor is enabled or not
#define PROCESSOR_ENABLED_BIT EFI_BIT(1)
// PROCESSOR_HEALTH_STATUS_BIT
/// Whether the processor is healthy or not
#define PROCESSOR_HEALTH_STATUS_BIT  EFI_BIT(2)

// EFI_CPU_PHYSICAL_LOCATION
/// The pyhiscal location of a logical CPU
typedef struct EFI_CPU_PHYSICAL_LOCATION EFI_CPU_PHYSICAL_LOCATION;
struct EFI_CPU_PHYSICAL_LOCATION {

  // Package
  /// Zero-based physical package number that identifies the cartridge of the processor
  UINT32 Package;
  // Core
  /// Zero-based physical core number within package of the processor
  UINT32 Core;
  // Thread
  /// Zero-based logical thread number within core of the processor
  UINT32 Thread;

};
// EFI_PROCESSOR_INFORMATION
/// Logical CPU information
typedef struct EFI_PROCESSOR_INFORMATION EFI_PROCESSOR_INFORMATION;
struct EFI_PROCESSOR_INFORMATION {

  // ProcessorId
  /// The unique processor ID determined by system hardware
  UINT64                    ProcessorId;
  // StatusFlag
  /// Flags indicating the state of the processor
  UINT32                    StatusFlag;
  // Location
  /// The physical location of the processor
  EFI_CPU_PHYSICAL_LOCATION Location;

};

// EFI_AP_PROCEDURE
/// Application processor function
/// @param ProcedureArgument The procedure argument passed at the processor start
typedef
VOID
(EFIAPI *EFI_AP_PROCEDURE) (
  IN VOID *ProcedureArgument
);
// EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS
/// Get the number of logical processors and the number that are currently enabled
/// @param This                      The multiprocessor services protocol
/// @param NumberOfProcessors        On output, the total number of logical processors in the system
/// @param NumberOfEnabledProcessors On output, the number of enabled logical processors that exist in system
/// @retval EFI_INVALID_PARAMETER NumberOfProcessors or NumberOfEnabledProcessors is NULL
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_SUCCESS           The number of logical processors and enabled logical processors was retrieved
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS) (
  IN  EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                    *NumberOfProcessors,
  OUT UINTN                    *NumberOfEnabledProcessors
);
// EFI_MP_SERVICES_GET_PROCESSOR_INFO
/// Get processor information
/// @param This                The multiprocessor services protocol
/// @param ProcessorNumber     The handle number of processor
/// @param ProcessorInfomation On output, the processor information
/// @retval EFI_INVALID_PARAMETER ProcessorInfoBuffer is NULL
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_NOT_FOUND         The processor with the handle specified by ProcessorNumber does not exist in the platform
/// @retval EFI_SUCCESS           The processor information was returned
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_GET_PROCESSOR_INFO) (
  IN  EFI_MP_SERVICES_PROTOCOL  *This,
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION *ProcessorInformation
);
// EFI_MP_SERVICES_STARTUP_ALL_APS
/// Execute a function on all enabled processors
/// @param This                  The multiprocessor services protocol
/// @param Procedure             The procedure function to execute on the processors
/// @param SingleThread          Whether to execute the function specified one by one, in ascending order of processor handle number or execute the function specified on each processor simultaneously
/// @param WaitEvent             If NULL wait for all processors to finish or timeout, otherwise the event to be signaled when all processors finish or timeout
/// @param TimeoutInMicroseconds The time limit in microseconds before the processor procedures fail or zero for infinity
/// @param ProcedureArgument     The procedure parameter for the procedure function
/// @param FailedCpuList         On output, the list of identifiers of failed processors ending with END_OF_CPU_LIST or NULL if no processors failed to start
/// @retval EFI_INVALID_PARAMETER Procedure is NULL
/// @retval EFI_UNSUPPORTED       A non-blocking mode request was made after the UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was signaled
/// @retval EFI_DEVICE_ERROR      Caller processor is AP
/// @retval EFI_NOT_STARTED       No enabled APs exist in the system
/// @retval EFI_NOT_READY         Any enabled APs are busy
/// @retval EFI_TIMEOUT           In blocking mode, the timeout expired before all enabled APs have finished
/// @retval EFI_SUCCESS           In blocking mode, all APs have finished before the timeout expired
/// @retval EFI_SUCCESS           In non-blocking mode, function has been dispatched to all enabled APs
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_STARTUP_ALL_APS) (
  IN  EFI_MP_SERVICES_PROTOCOL  *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  BOOLEAN                    SingleThread,
  IN  EFI_EVENT                  WaitEvent OPTIONAL,
  IN  UINTN                      TimeoutInMicroseconds,
  IN  VOID                      *ProcedureArgument OPTIONAL,
  OUT UINTN                    **FailedCpuList OPTIONAL
);
// EFI_MP_SERVICES_STARTUP_THIS_AP
/// Execute a function on an enabled processor
/// @param This                  The multiprocessor services protocol
/// @param Procedure             The function to be run on the designated processor
/// @param ProcessorNumber       The handle number of the processor on which to execute the specified function
/// @param WaitEvent             If NULL wait for the processor to finish or timeout, otherwise the event to be signaled when the processor finishes or times out
/// @param TimeoutInMicrosecsond The time limit in microseconds before the processor procedure fails or zero for infinity
/// @param ProcedureArgument     The procedure parameter passed to the specified function
/// @param Finished              On output, whether the function finished or not
/// @retval EFI_INVALID_PARAMETER Procedure is NULL
/// @retval EFI_INVALID_PARAMETER ProcessorNumber specifies the BSP or disabled AP
/// @retval EFI_UNSUPPORTED       A non-blocking mode request was made after the UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was signaled
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_TIMEOUT           In blocking mode, the timeout expired before the specified AP has finished
/// @retval EFI_NOT_READY         The specified AP is busy
/// @retval EFI_NOT_FOUND         The processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_SUCCESS           In blocking mode, specified AP finished before the timeout expires
/// @retval EFI_SUCCESS           In non-blocking mode, the function has been dispatched to specified AP
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_STARTUP_THIS_AP) (
  IN  EFI_MP_SERVICES_PROTOCOL *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  EFI_EVENT                 WaitEvent OPTIONAL,
  IN  UINTN                     TimeoutInMicroseconds,
  IN  VOID                     *ProcedureArgument OPTIONAL,
  OUT BOOLEAN                  *Finished OPTIONAL
);
// EFI_MP_SERVICES_SWITCH_BSP
/// Switches the requested processor to be the boot services processor
/// @param This            The multiprocessor services protocol
/// @param ProcessorNumber The processer handle number to set as the new boot service processor
/// @param EnableOldBSP    Whether the old boot services processor should remain enabled or not
/// @retval EFI_INVALID_PARAMETER ProcessorNumber specifies the current BSP or a disabled AP
/// @retval EFI_UNSUPPORTED       Switching the BSP cannot be completed prior to this service returning
/// @retval EFI_UNSUPPORTED       Switching the BSP is not supported
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_NOT_FOUND         The processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_NOT_READY         The specified AP is busy
/// @retval EFI_SUCCESS           BSP successfully switched
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_SWITCH_BSP) (
  IN EFI_MP_SERVICES_PROTOCOL *This,
  IN UINTN                     ProcessorNumber,
  IN BOOLEAN                   EnableOldBSP
);
// EFI_MP_SERVICES_ENABLEDISABLEAP
/// Enable or disable a processor
/// @param This            The multiprocessor services protocol
/// @param ProcessorNumber The processor handle number
/// @param EnableAP        Whether to enable or disable the processor
/// @param HealthFlag      On output, the health of the processor using PROCESSOR_HEALTH_STATUS_BIT
/// @retval EFI_INVALID_PARAMETER ProcessorNumber specifies the BSP
/// @retval EFI_UNSUPPORTED       Enabling or disabling an AP cannot be completed prior to this service returning
/// @retval EFI_UNSUPPORTED       Enabling or disabling an AP is not supported
/// @retval EFI_DEVICE_ERROR      The calling processor is an AP
/// @retval EFI_NOT_FOUND         Processor with the handle specified by ProcessorNumber does not exist
/// @retval EFI_SUCCESS           The specified AP was enabled or disabled successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_ENABLEDISABLEAP) (
  IN  EFI_MP_SERVICES_PROTOCOL *This,
  IN  UINTN                     ProcessorNumber,
  IN  BOOLEAN                   EnableAP,
  OUT UINT32                   *HealthFlag OPTIONAL
);
// EFI_MP_SERVICES_WHOAMI
/// Get the handle number for the calling processor
/// @param This            The multiprocessor services procotol
/// @param ProcessorNumber On output, the current processor handle number
/// @retval EFI_INVALID_PARAMETER If ProcessorNumber is NULL
/// @retval EFI_SUCCESS           The current processor handle number was returned
typedef
EFI_STATUS
(EFIAPI
*EFI_MP_SERVICES_WHOAMI) (
  IN  EFI_MP_SERVICES_PROTOCOL *This,
  OUT UINTN                    *ProcessorNumber
);

// EFI_MP_SERVICE_PROTOCOL
/// The Multiprocessor service protocol
struct EFI_MP_SERVICES_PROTOCOL {

  // GetNumberOfProcessors
  /// Gets the number of logical processors and the number of enabled logical processors in the system
  EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS  GetNumberOfProcessors;
  // GetProcessorInfo
  /// Gets detailed information on the requested processor at the instant this call is made
  EFI_MP_SERVICES_GET_PROCESSOR_INFO        GetProcessorInfo;
  // StartupAllAPs
  /// Starts up all the enabled APs in the system to run the function provided by the caller
  EFI_MP_SERVICES_STARTUP_ALL_APS           StartupAllAPs;
  // StartupThisAP
  /// Starts up the requested AP to run the function provided by the caller
  EFI_MP_SERVICES_STARTUP_THIS_AP           StartupThisAP;
  // SwitchBSP
  /// Switches the requested AP to be the BSP from that point onward, this changes the BSP for all purposes
  EFI_MP_SERVICES_SWITCH_BSP                SwitchBSP;
  // EnableDisableAP
  /// Enables and disables the given AP from that point onward
  EFI_MP_SERVICES_ENABLEDISABLEAP           EnableDisableAP;
  // WhoAmI
  /// Gets the handle number of the caller processor
  EFI_MP_SERVICES_WHOAMI                    WhoAmI;

};

// gEfiMpServiceProtocolGuid
/// The multiprocessor services protocol identifier
EXTERN EFI_GUID gEfiMpServiceProtocolGuid;

// EfiMpServicesGetNumberOfProcessors
/// Get the number of logical processors and the number that are currently enabled
/// @param NumberOfProcessors        On output, the total number of logical processors in the system
/// @param NumberOfEnabledProcessors On output, the number of enabled logical processors that exist in system
/// @retval EFI_INVALID_PARAMETER   NumberOfProcessors or NumberOfEnabledProcessors is NULL
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_SUCCESS             The number of logical processors and enabled logical processors was retrieved
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesGetNumberOfProcessors (
  OUT UINTN *NumberOfProcessors OPTIONAL,
  OUT UINTN *NumberOfEnabledProcessors OPTIONAL
);
// EfiMpServicesGetProcessorInfo
/// Get processor information
/// @param ProcessorNumber      The handle number of processor
/// @param ProcessorInformation On output, the processor information
/// @retval EFI_INVALID_PARAMETER   If ProcessorInformation is NULL
/// @retval EFI_DEVICE_ERROR        The calling processor is an AP
/// @retval EFI_NOT_FOUND           The processor with the handle specified by ProcessorNumber does not exist in the platform
/// @retval EFI_SUCCESS             The processor information was returned
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesGetProcessorInfo (
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION *ProcessorInformation
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesStartupAllAPs (
  IN  EFI_AP_PROCEDURE   Procedure,
  IN  BOOLEAN            SingleThread,
  IN  EFI_EVENT          WaitEvent OPTIONAL,
  IN  UINTN              TimeoutInMicroseconds,
  IN  VOID              *ProcedureArgument OPTIONAL,
  OUT UINTN            **FailedCpuList OPTIONAL
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesStartupThisAP (
  IN  EFI_AP_PROCEDURE  Procedure,
  IN  UINTN             ProcessorNumber,
  IN  EFI_EVENT         WaitEvent OPTIONAL,
  IN  UINTN             TimeoutInMicroseconds,
  IN  VOID             *ProcedureArgument OPTIONAL,
  OUT BOOLEAN          *Finished OPTIONAL
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesSwitchBSP ( 
  IN UINTN   ProcessorNumber,
  IN BOOLEAN EnableOldBSP
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesEnableDisableAP (
  IN  UINTN    ProcessorNumber,
  IN  BOOLEAN  EnableAP,
  OUT UINT32  *HealthFlag OPTIONAL
);
// EfiMpServicesWhoAmI
/// Get the handle number for the calling processor
/// @param ProcessorNumber On output, the current processor handle number
/// @retval EFI_INVALID_PARAMETER If ProcessorNumber is NULL
/// @retval EFI_SUCCESS           The current processor handle number was returned
EXTERN
EFI_STATUS
EFIAPI
EfiMpServicesWhoAmI (
  OUT UINTN *ProcessorNumber
);

#endif // __MP_SERVICE_PROTOCOL_HEADER__
