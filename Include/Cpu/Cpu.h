///
/// @file Include/Cpu/Cpu.h
///
/// CPU features
///

#pragma once
#ifndef __CPU_HEADER__
#define __CPU_HEADER__

#include <Uefi.h>

#include <Uefi/Protocol/MpService.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// CPU_TYPE
/// The type of CPU
typedef enum CPU_TYPE CPU_TYPE;
enum CPU_TYPE {

  // CpuTypeUnknown
  /// Unknown CPU type
  CpuTypeUnknown = 0,

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)
  
  // CpuTypeIntel
  /// Intel CPU type
  CpuTypeIntel,
  // CpuTypeAmd
  /// AMD CPU type
  CpuTypeAmd

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: ARM and ARM64 CPU types

#endif

};
// CPU_FEATURES
/// CPU features
typedef struct CPU_FEATURES CPU_FEATURES;
struct CPU_FEATURES {

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  // FloatingPointUnit
  /// Onboard floating point unit
  BOOLEAN FloatingPointUnit;
  // VirtualModeExtensions
  /// Virtual 8086 mode extensions
  BOOLEAN VirtualModeExtensions;
  // DebuggingExtensions
  /// Debugging extensions
  BOOLEAN DebuggingExtensions;
  // PageSizeExtension
  /// Pafe size extensions
  BOOLEAN PageSizeExtension;
  // TimeStampCounter
  /// Time stamp counter present
  BOOLEAN TimeStampCounter;
  // ModelSpecificRegisters
  /// Model specific registers
  BOOLEAN ModelSpecificRegisters;
  // PhysicalAddressExtension
  /// Physical address extension
  BOOLEAN PhysicalAddressExtension;
  // MachineCheckException
  /// Machine check exception
  BOOLEAN MachineCheckException;
  // CompareAndSwap8
  /// Compare and exchange 8bit value
  BOOLEAN CompareAndSwap8;
  // OnboardApic
  /// Onboard APIC
  BOOLEAN OnboardApic;
  // SystemInstructions
  /// System SYSENTER and SYSEXIT instructions
  BOOLEAN SystemInstructions;
  // MemoryTypeRangeRegisters
  /// Memory type range registers
  BOOLEAN MemoryTypeRangeRegisters;
  // PageGlobalEnable
  /// Page global enable
  BOOLEAN PageGlobalEnable;
  // MachineCheckArchitecture
  /// Machine check architecture
  BOOLEAN MachineCheckArchitecture;
  // ConditionalMove
  /// Conditional move instructions
  BOOLEAN ConditionalMove;
  // PageAttributeTable
  /// Page attribute table
  BOOLEAN PageAttributeTable;
  // PageSizeExtensions36
  /// Page size extension 36bit
  BOOLEAN PageSizeExtension36;
  // ProcessorSerialNumber
  /// Processor serial number
  BOOLEAN ProcessSerialNumber;
  // FlushCacheLine
  /// Flush cache line CLFLUSH
  BOOLEAN FlushCacheLine;
  // DebugStore
  /// Debug store
  BOOLEAN DebugStore;
  // MsrForThermalAcpi
  /// Thermal control MSRs for ACPI
  BOOLEAN MsrForThermalAcpi;
  // Mmx
  /// MMX instructions
  BOOLEAN Mmx;
  // FloatingPointSave
  /// Floating point save FXSAVE/FXRESTORE instructions
  BOOLEAN FloatingPointSave;
  // Sse
  /// SSE instructions
  BOOLEAN Sse;
  // Sse2
  /// SSE2 instrucitons
  BOOLEAN Sse2;
  // SelfSnoop
  /// Self snooping cache supported
  BOOLEAN SelfSnoop;
  // Hyperthreading
  /// Hyperthreading, each physical processor has multiple logical processors
  BOOLEAN Hyperthreading;
  // ThermalMonitor
  /// Thermal monitoring
  BOOLEAN ThermalMonitor;
  // Ia64EmulateIa32
  /// IA64 supports IA32 emulation
  BOOLEAN Ia64EmulateIa32;
  // PendingBreakEnable
  /// Pending break enable wakeup support
  BOOLEAN PendingBreakEnable;
  // Sse3
  /// SSE3 instructions
  BOOLEAN Sse3;
  // CarrylessMultiply
  /// Carryless multiply PCLMULQDQ instruction 
  BOOLEAN CarrylessMultiply;
  // DebugStore64
  /// Debug store 64bit
  BOOLEAN DebugStore64;
  // MonitorWait
  /// MONITOR and MWAIT instructions
  BOOLEAN MonitorWait;
  // DebugStoreCpl
  /// Debug store CPL qualifies
  BOOLEAN DebugStoreCpl;
  // VirtualMachineExtensions
  /// VIrtual machine extensions VMX
  BOOLEAN VirtualMachineExtensions;
  // SafeModeExtensions
  /// Safe mode extensions
  BOOLEAN SafeModeExtensions;
  // EnhancedSpeedStep
  /// Enhanced SpeedStep supported
  BOOLEAN EnhancedSpeedStep;
  // ThermalMonitor2
  /// Thermal monitor 2
  BOOLEAN ThermalMonitor2;
  // Ssse3
  /// SSSE3 Instructions
  BOOLEAN Ssse3;
  // ContextId
  /// L1 context identifier
  BOOLEAN ContextId;
  // SiliconDebug
  /// Silicon debug BOOLEANerface
  BOOLEAN SiliconDebug;
  // FusedMultiplyAdd
  /// Fused multiply-add (FMA3)
  BOOLEAN FusedMultiplyAdd;
  // CompareAndSwap16
  /// Compare and exchange 16bit
  BOOLEAN CompareAndSwap16;
  // DisableTaskPriorityMessages
  /// Support for disabling task priority messages
  BOOLEAN DisableTaskPriorityMessages;
  // PerformanceMonitor
  /// Performance monitoring and debugging capability
  BOOLEAN PerformanceMonitor;
  // ProcessContextId
  /// Process context identifiers
  BOOLEAN ProcessContextId;
  // DirectCacheAccess
  /// Direct cache access for DMA writes
  BOOLEAN DirectCacheAccess;
  // Sse4_1
  /// SSE4.1 instructions
  BOOLEAN Sse4_1;
  // Sse4_2
  /// SSE4.2 instructions
  BOOLEAN Sse4_2;
  // X2Apic
  /// X2APIC support
  BOOLEAN X2Apic;
  // ByteSwapMove
  /// Move data after byte swapping
  BOOLEAN ByteSwapMove;
  // CountBits
  /// Count bits POPCNT instruction
  BOOLEAN CountBits;
  // TimeStampCounterDeadline
  /// Time stamp counter one-shot deadline
  BOOLEAN TimeStampCounterDeadline;
  // Aes
  /// AES instruction set
  BOOLEAN Aes;
  // SaveExtendedState
  /// Save extended state XSAVE/XRESTOR/XSETBV/XGETBV instructions
  BOOLEAN SaveExtendedState;
  // SaveExtendedStateEnabled
  /// Save extended state enabled by OS
  BOOLEAN SaveExtendedStateEnabled;
  // AdvanceVectorExtensions
  /// Advanced vector extensions
  BOOLEAN AdvancedVectorExtensions;
  // HalfPrecisionFloatingPoint
  /// Half precision floating point F16C support
  BOOLEAN HalfPrecisionFloatingPoint;
  // RandomNumberGenerator
  /// Random number generator RDRAND instruction
  BOOLEAN RandomNumberGenerator;
  // Hypervisor
  /// Running on hypervisor
  BOOLEAN Hypervisor;
  // ReadSegmentBase
  /// Read FS/GS segment base
  BOOLEAN ReadSegmentBase;
  // Ia32TimeStampCounterAdjust
  /// IA32_TSC_ADJUST
  BOOLEAN Ia32TimeStampCounterAdjust;
  // SoftwareGuardExtensions
  /// Software guard extensions
  BOOLEAN SoftwareGuardExtensions;
  // BitManipulation
  /// Bit manipulation instruction set
  BOOLEAN BitManipulation;
  // TransactionalSyncExtensions
  /// Transactional synchronization HLE extensions
  BOOLEAN TransactionalSyncExtensions;
  // AdvancedVectorExtensions2
  /// Advanced vector extensions AVX2 instructions
  BOOLEAN AdvancedVectorExtensions2;
  // SupervisorModeExecutionPrevention
  /// Supervisor mode execution prevention
  BOOLEAN SupervisorModeExecutionPrevention;
  // BitManipulation2
  /// Bit manipulation instruction set 2
  BOOLEAN BitManipulation2;
  // EnhanceRepeatMove
  /// Enhance repeat move REP MOVSB/STOSB instructions
  BOOLEAN EnhanceRepeatMove;
  // InvalidateProcessContextId
  /// Invalidate process context identifier INVPCID instruction
  BOOLEAN InvalidateProcessContextId;
  // TransactionalRestrictedSyncExtensions
  /// Transactional synchronization RTM extensions
  BOOLEAN TransactionalRestrictedSyncExtensions;
  // PlatformQualityOfServiceMonitor
  /// Platform quality of service monitoring
  BOOLEAN PlatformQualityOfServiceMonitor;
  // FloatingPointDeprecated
  /// Floating point CS/DS deprecated
  BOOLEAN FloatingPointDeprecated;
  // Mpx
  /// Intel memory protection extensions
  BOOLEAN Mpx;
  // PlatformQualityOfServiceEnforcement
  /// Platform quality of service enforcement
  BOOLEAN PlatformQualityOfServiceEnforcement;
  // Avx512
  /// Advanced vector extensions 512bit
  BOOLEAN Avx512;
  // Avx512DoubleQuad
  /// Advanced vector extensions 512bit for doubleword and quadword
  BOOLEAN Avx512DoubleQuad;
  // RandomSeed
  /// Rnadom seed generator RDSEED instruction
  BOOLEAN RandomSeed;
  // Adx
  /// Intel ADX (Multi-Precision Add-Carry Instruction Extensions)
  BOOLEAN Adx;
  // SupervisorModeAccessPrevention
  /// Supervisor mode access prevention
  BOOLEAN SupervisorModeAccessPrevention;
  // Avx512IntegerFusedMultiplyAdd
  /// Advanced vector extensions 512bit Integer Fused Multiply-Add Instructions
  BOOLEAN Avx512IntegerFusedMultiplyAdd;
  // PCommit
  /// PCOMMIT instruction
  BOOLEAN PCommit;
  // FlushCacheLineOptimized
  /// CLFLUSHOPT instruction
  BOOLEAN FlushCacheLineOptimized;
  // CacheLineWriteBack
  /// CLWB instruction
  BOOLEAN CacheLineWriteBack;
  // ProcessorTrace
  // Intel Processor Trace
  BOOLEAN ProcessorTrace;
  // Avx512Prefetch
  /// Advanced vector extensions 512bit prefetch instructions
  BOOLEAN Avx512Prefetch;
  // AvxExponentialAndReciprocal
  /// Advanced vector extensions 512bit Exponential and Reciprocal Instructions
  BOOLEAN Avx512ExponentialAndReciprocal;
  // Avx512ConflictDetection
  /// Advanced vector extensions 512bit Conflict Detection Instructions
  BOOLEAN Avx512ConflictDetection;
  // Sha
  // Intel SHA extensions
  BOOLEAN Sha;
  // Avx512ByteWord
  /// AVX-512 Byte and Word Instructions
  BOOLEAN Avx512ByteWord;
  // Avx512VectorLength
  /// AVX-512 Vector Length Extensions
  BOOLEAN Avx512VectorLength;
  // PrefetchVectorDataIntoCaches
  /// Prefetch vector data BOOLEANo caches PREFETCHWT1 instruction
  BOOLEAN PrefetchVectorDataIntoCaches;
  // Avx512VectorBitManipulation
  /// AVX-512 Vector Bit Manipulation Instructions
  BOOLEAN Avx512VectorBitManipulation;
  // UserModePrevention
  /// User-mode Instruction Prevention
  BOOLEAN UserModePrevention;
  // MemoryProtectionKeys
  /// Memory protection keys for user-mode pages
  BOOLEAN MemoryProtectionKeys;
  // MemoryProtectKeysEnabled
  /// PKU enabled by OS
  BOOLEAN MemoryProtectKeysEnabled;
  // Avx512VectorBitManipulation2
  /// AVX-512 Vector Bit Manipulation Instructions 2
  BOOLEAN Avx512VectorBitManipulation2;
  // GaloisField
  /// Galois field instructions
  BOOLEAN GaloisField;
  // AesVector
  /// Vector AES instruction set (VEX-256/EVEX)
  BOOLEAN AesVector;
  // Avx512CarrylessMultiply
  /// CLMUL instruction set (VEX-256/EVEX)
  BOOLEAN Avx512CarrylessMultiply;
  // AvxNeuralNetwork
  /// AVX-512 vector neural network instructions
  BOOLEAN Avx512NeuralNetwork;
  // Avx512BitAlgorithms
  /// AVX-512 bit manipulation algorithms instructions
  BOOLEAN Avx512BitAlgorithms;
  // Avx512PopulationDoubleQuad
  /// AVX-512 vector population count double and quad-word
  BOOLEAN Avx512PopulationDoubleQuad;
  // MpxAddressWidthAdjust
  /// The value of userspace MPX Address-Width Adjust used by the BNDLDX and BNDSTX Intel MPX instructions in 64-bit mode
  UINT8   MpxAddressWidthAdjust;
  // ReadProcessorId
  /// Read Processor ID
  BOOLEAN ReadProcessorId;
  // SgxLaunchConfiguration
  /// SGX Launch Configuration
  BOOLEAN SgxLaunchConfiguration;
  // Avx512NeuralNetwork
  /// AVX-512 4-register Neural Network Instructions
  BOOLEAN AvxNeuralNetwork;
  // Avx512MultiplyAccumulation
  /// AVX-512 4-register Multiply Accumulation Single precision
  BOOLEAN Avx512MultiplyAccumulation;
  // PlatformConfiguration
  /// Platform configuration (Memory Encryption Technologies Instructions)
  BOOLEAN PlatformConfiguration;
  // IndirectBranchControl
  /// Indirect Branch Control (IBC)	Speculation Control: Indirect Branch Restricted Speculation (IBRS) and Indirect Branch Prediction Barrier (IBPB)
  BOOLEAN IndirectBranchControl;
  // IndirectBranchControlPredictor
  /// Indirect Branch Control (IBC)	Single Thread Indirect Branch Predictor (STIBP
  BOOLEAN IndirectBranchControlPredictor;
  // ArchitectureCapabilities
  /// IA32_ARCH_CAPABILITIES MSR support
  BOOLEAN ArchitectureCapabilities;
  // SpeculativeStoreBypassDisable
  /// Speculative store bypass disable (SSBD) as mitigation for speculative store bypass
  BOOLEAN SpeculativeStoreBypassDisable;
  // MultiprocessorCapable
  /// Multiprocessor capable
  BOOLEAN MultiprocessorCapable;
  // NoExecuteBit
  /// No execute bit
  BOOLEAN NoExecuteBit;
  // ExtendedMmx
  /// Extended MMX instructions
  BOOLEAN ExtendedMmx;
  // FloatingPointSaveOptimizations
  /// Floating point save and restore optimizations
  BOOLEAN FloatingPointSaveOptimizations;
  // GibibytePages
  /// Gibibyte pages
  BOOLEAN GibibytePages;
  // TimeStampCounterAndProcessorId
  /// Time stamp counter and processor identifier RDTSCP instruction
  BOOLEAN TimeStampCounterAndProcessorId;
  // LongMode
  /// Long mode support
  BOOLEAN LongMode;
  // ExtendedThreeDNow
  /// Extended 3DNow! instructions
  BOOLEAN ExtendedThreeDNow;
  // ThreeDNow
  /// 3DNow! instructions
  BOOLEAN ThreeDNow;
  // LoadOrStoreFlagsFromLongMode
  /// LAHF/SAHF in long mode
  BOOLEAN LoadOrStoreFlagsFromLongMode;
  // HyperthreadingUnavailable
  /// Hyperthreading not available
  BOOLEAN HyperthreadingUnavailable;
  // SecureVirtualMachine
  /// Secure virtual machine
  BOOLEAN SecureVirtualMachine;
  // ExtendedApicSpace
  /// Extended APIC space
  BOOLEAN ExtendedApicSpace;
  // Cr8Legacy
  /// CR8 in 32-bit mode
  BOOLEAN Cr8Legacy;
  // AdvancedBitManipulation
  /// Advanced bit manipulation (lzcnt and popcnt)
  BOOLEAN AdvancedBitManipulation;
  // Sse4a
  /// SSE4a instructions
  BOOLEAN Sse4a;
  // MisalignedSse
  /// Misaligned SSE mode
  BOOLEAN MisalignedSse;
  // ThreeDNowPrefetch
  /// 3DNow! prefetch	PREFETCH and PREFETCHW instructions
  BOOLEAN ThreeDNowPrefetch;
  // OsVisibileWorkaround
  /// OS Visible Workaround
  BOOLEAN OsVisibleWorkaround;
  // InstructionBaseSampling
  /// Instruction based sampling
  BOOLEAN InstructionBasedSampling;
  // Xop
  ///XOP instruction set
  BOOLEAN Xop;
  // SecureInitJumpAndSetGlobalInterrupt
  /// Secure init jump with attestation verification and set global interrupt flag SKINIT/STGI instructions
  BOOLEAN SecureInitJumpAndSetGlobalInterrupt;
  // WatchdogTimer
  ///	Watchdog timer
  BOOLEAN WatchdogTimer;
  // LightWeightProfiling
  /// Light weight profiling
  BOOLEAN LightWeightProfiling;
  // FusedMultiplyAdd4
  /// 4 operands fused multiply-add
  BOOLEAN FusedMultiplyAdd4;
  // TranslationCacheExtension
  /// Translation cache extension
  BOOLEAN TranslationCacheExtension;
  // NodeIdMsr
  /// NodeID MSR
  BOOLEAN NodeIdMsr;
  // TrailingBitManipulation
  /// Trailing Bit Manipulation
  BOOLEAN TrailingBitManipulation;
  // TopologyExtensions
  /// Topology wxtensions
  BOOLEAN TopologyExtensions;
  // PerformanceCounter
  /// Core performance counter extensions
  BOOLEAN PerformanceCounter;
  // PerformanceCounterNB
  /// NB performance counter extensions
  BOOLEAN PerformanceCounterNB;
  // DataBreakpoint
  /// Data breakpoint extensions
  BOOLEAN DataBreakpoint;
  // PerformanceTimeStampCounter
  ///	Performance TSC
  BOOLEAN PerformanceTimeStampCounter;
  // PerformanceCounterL2I
  /// L2I performance counter extensions
  BOOLEAN PerformanceCounterL2I;

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: ARM and ARM64 CPU features
  /// Reserved
  UINTN Reserved;

#endif

};

// CpuIsMultiprocessor
/// Check whether there is more than one processor available
EXTERN
BOOLEAN
EFIAPI
CpuIsMultiprocessor (
  VOID
);

// CpuGetType
/// Get the CPU type
/// @return The CPU type
EXTERN
CPU_TYPE
EFIAPI
CpuGetType (
  VOID
);
// CpuGetSignature
/// Get the CPU signature
/// @param Family   On output, the CPU family
/// @param Model    On output, the CPU model
/// @param Stepping On output, the CPU stepping
/// @return The CPUID signature
EXTERN
UINT32
EFIAPI
CpuGetSignature (
  OUT UINT16 *Family OPTIONAL,
  OUT UINT8  *Model OPTIONAL,
  OUT UINT8  *Stepping OPTIONAL
);
// CpuGetFamily
/// Get the CPU family
/// @return The CPU family
EXTERN
UINT16
EFIAPI
CpuGetFamily (
  VOID
);
// CpuGetModel
/// Get the CPU model
/// @return The CPU model
EXTERN
UINT8
EFIAPI
CpuGetModel (
  VOID
);
// CpuGetStepping
/// Get the CPU stepping
/// @return The CPU stepping
EXTERN
UINT8
EFIAPI
CpuGetStepping (
  VOID
);
// CpuGetDescription
/// Get CPU description
/// @return The description of the CPU which must be freed
EXTERN
CHAR16 *
EFIAPI
CpuGetDescription (
  VOID
);
// CpuGetFeatures
/// Get CPU features
/// @param Features On output, the CPU features
/// @retval EFI_INVALID_PARAMETER If Features is NULL
/// @retval EFI_SUCCESS           If the CPU features were returned successfully
EXTERN
EFI_STATUS
EFIAPI
CpuGetFeatures (
  OUT CPU_FEATURES *Features
);
// CpuGetNumberOfProcessors
/// Get the number of processors
/// @param NumberOfLogicalProcessors  On output, the number of logical processors
/// @param NumberOfPhysicalProcessors On output, the number of physical processors
/// @param NumberOfPackages           On output, the number of packages
/// @retval EFI_INVALID_PARAMETER If NumberOfLogicalProcessors, NumberOfPhysicalProcessors, and NumberOfPackages are all NULL
/// @retval EFI_SUCCEESS          If the number of processors was returned successfully
EXTERN
EFI_STATUS
EFIAPI
CpuGetNumberOfProcessors (
  OUT UINTN *NumberOfLogicalProcessors OPTIONAL,
  OUT UINTN *NumberOfPhysicalProcessors OPTIONAL,
  OUT UINTN *NumberOfPackages OPTIONAL
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __CPU_HEADER__
