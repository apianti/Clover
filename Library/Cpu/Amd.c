///
/// @file Library/Cpu/Amd.c
///
/// AMD specific CPU features
///

#include "Amd.h"

#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

// AmdCpuGetFeatures
/// Get CPU features
/// @param Features On output, the CPU features
/// @retval EFI_INVALID_PARAMETER If Features is NULL
/// @retval EFI_SUCCESS           If the CPU features were returned successfully
EFI_STATUS
EFIAPI
AmdCpuGetFeatures (
  OUT CPU_FEATURES *Features
) {
  UINT32 *Ptr;
  UINT32  MaxFunction = 0;
  UINT32  Ebx = 0;
  UINT32  Ecx = 0;
  UINT32  Edx = 0;
  // Check parameters
  if (Features == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the max CPUID function
  EfiCpuid(0, &MaxFunction, NULL, NULL, NULL);
  // Zero the CPU features
  Ptr = (UINT32 *)Features;
  EfiZero(CPU_FEATURES, Features);
  // Get the CPU features
  if (MaxFunction >= 1) {
    // Modify the signature for practical use
    EfiCpuid(1, NULL, NULL, &Ecx, &Edx);
    // Features from EDX
    Features->FloatingPointUnit = EFI_BIT_IS_SET(Edx, 0);
    Features->VirtualModeExtensions = EFI_BIT_IS_SET(Edx, 1);
    Features->DebuggingExtensions = EFI_BIT_IS_SET(Edx, 2);
    Features->PageSizeExtension = EFI_BIT_IS_SET(Edx, 3);
    Features->TimeStampCounter = EFI_BIT_IS_SET(Edx, 4);
    Features->ModelSpecificRegisters = EFI_BIT_IS_SET(Edx, 5);
    Features->PhysicalAddressExtension = EFI_BIT_IS_SET(Edx, 6);
    Features->MachineCheckException = EFI_BIT_IS_SET(Edx, 7);
    Features->CompareAndSwap8 = EFI_BIT_IS_SET(Edx, 8);
    Features->OnboardApic = EFI_BIT_IS_SET(Edx, 9);
    Features->SystemInstructions = EFI_BIT_IS_SET(Edx, 11);
    Features->MemoryTypeRangeRegisters = EFI_BIT_IS_SET(Edx, 12);
    Features->PageGlobalEnable = EFI_BIT_IS_SET(Edx, 13);
    Features->MachineCheckArchitecture = EFI_BIT_IS_SET(Edx, 14);
    Features->ConditionalMove = EFI_BIT_IS_SET(Edx, 15);
    Features->PageAttributeTable = EFI_BIT_IS_SET(Edx, 16);
    Features->PageSizeExtension36 = EFI_BIT_IS_SET(Edx, 17);
    Features->ProcessSerialNumber = EFI_BIT_IS_SET(Edx, 18);
    Features->FlushCacheLine = EFI_BIT_IS_SET(Edx, 19);
    Features->DebugStore = EFI_BIT_IS_SET(Edx, 21);
    Features->MsrForThermalAcpi = EFI_BIT_IS_SET(Edx, 22);
    Features->Mmx = EFI_BIT_IS_SET(Edx, 23);
    Features->FloatingPointSave = EFI_BIT_IS_SET(Edx, 24);
    Features->Sse = EFI_BIT_IS_SET(Edx, 25);
    Features->Sse2 = EFI_BIT_IS_SET(Edx, 26);
    Features->SelfSnoop = EFI_BIT_IS_SET(Edx, 27);
    Features->Hyperthreading = EFI_BIT_IS_SET(Edx, 28);
    Features->ThermalMonitor = EFI_BIT_IS_SET(Edx, 29);
    Features->Ia64EmulateIa32 = EFI_BIT_IS_SET(Edx, 30);
    Features->PendingBreakEnable = EFI_BIT_IS_SET(Edx, 31);
    // Features from ECX
    Features->Sse3 = EFI_BIT_IS_SET(Edx, 0);
    Features->CarrylessMultiply = EFI_BIT_IS_SET(Ecx, 1);
    Features->DebugStore64 = EFI_BIT_IS_SET(Ecx, 2);
    Features->MonitorWait = EFI_BIT_IS_SET(Ecx, 3);
    Features->DebugStoreCpl = EFI_BIT_IS_SET(Ecx, 4);
    Features->VirtualMachineExtensions = EFI_BIT_IS_SET(Ecx, 5);
    Features->SafeModeExtensions = EFI_BIT_IS_SET(Ecx, 6);
    Features->EnhancedSpeedStep = EFI_BIT_IS_SET(Ecx, 7);
    Features->ThermalMonitor2 = EFI_BIT_IS_SET(Ecx, 8);
    Features->Ssse3 = EFI_BIT_IS_SET(Ecx, 9);
    Features->ContextId = EFI_BIT_IS_SET(Ecx, 10);
    Features->SiliconDebug = EFI_BIT_IS_SET(Ecx, 11);
    Features->FusedMultiplyAdd = EFI_BIT_IS_SET(Ecx, 12);
    Features->CompareAndSwap16 = EFI_BIT_IS_SET(Ecx, 13);
    Features->DisableTaskPriorityMessages = EFI_BIT_IS_SET(Ecx, 14);
    Features->PerformanceMonitor = EFI_BIT_IS_SET(Ecx, 15);
    Features->ProcessContextId = EFI_BIT_IS_SET(Ecx, 17);
    Features->DirectCacheAccess = EFI_BIT_IS_SET(Ecx, 18);
    Features->Sse4_1 = EFI_BIT_IS_SET(Ecx, 19);
    Features->Sse4_2 = EFI_BIT_IS_SET(Ecx, 20);
    Features->X2Apic = EFI_BIT_IS_SET(Ecx, 21);
    Features->ByteSwapMove = EFI_BIT_IS_SET(Ecx, 22);
    Features->CountBits = EFI_BIT_IS_SET(Ecx, 23);
    Features->TimeStampCounterDeadline = EFI_BIT_IS_SET(Ecx, 24);
    Features->Aes = EFI_BIT_IS_SET(Ecx, 25);
    Features->SaveExtendedState = EFI_BIT_IS_SET(Ecx, 26);
    Features->SaveExtendedStateEnabled = EFI_BIT_IS_SET(Ecx, 27);
    Features->AdvancedVectorExtensions = EFI_BIT_IS_SET(Ecx, 28);
    Features->HalfPrecisionFloatingPoint = EFI_BIT_IS_SET(Ecx, 29);
    Features->RandomNumberGenerator = EFI_BIT_IS_SET(Ecx, 30);
    Features->Hypervisor = EFI_BIT_IS_SET(Ecx, 31);
    // Get the CPU extended features
    if (MaxFunction >= 7) {
      EfiCpuid(7, NULL, &Ebx, &Ecx, &Edx);
      // Features from EBX
      Features->ReadSegmentBase = EFI_BIT_IS_SET(Ebx, 0);
      Features->Ia32TimeStampCounterAdjust = EFI_BIT_IS_SET(Ebx, 1);
      Features->SoftwareGuardExtensions = EFI_BIT_IS_SET(Ebx, 2);
      Features->BitManipulation = EFI_BIT_IS_SET(Ebx, 3);
      Features->TransactionalSyncExtensions = EFI_BIT_IS_SET(Ebx, 4);
      Features->AdvancedVectorExtensions2 = EFI_BIT_IS_SET(Ebx, 5);
      Features->SupervisorModeExecutionPrevention = EFI_BIT_IS_SET(Ebx, 7);
      Features->BitManipulation2 = EFI_BIT_IS_SET(Ebx, 8);
      Features->EnhanceRepeatMove = EFI_BIT_IS_SET(Ebx, 9);
      Features->InvalidateProcessContextId = EFI_BIT_IS_SET(Ebx, 10);
      Features->TransactionalRestrictedSyncExtensions = EFI_BIT_IS_SET(Ebx, 11);
      Features->PlatformQualityOfServiceMonitor = EFI_BIT_IS_SET(Ebx, 12);
      Features->FloatingPointDeprecated = EFI_BIT_IS_SET(Ebx, 13);
      Features->Mpx = EFI_BIT_IS_SET(Ebx, 14);
      Features->PlatformQualityOfServiceEnforcement = EFI_BIT_IS_SET(Ebx, 15);
      Features->Avx512 = EFI_BIT_IS_SET(Ebx, 16);
      Features->Avx512DoubleQuad = EFI_BIT_IS_SET(Ebx, 17);
      Features->RandomSeed = EFI_BIT_IS_SET(Ebx, 18);
      Features->Adx = EFI_BIT_IS_SET(Ebx, 19);
      Features->SupervisorModeAccessPrevention = EFI_BIT_IS_SET(Ebx, 20);
      Features->Avx512IntegerFusedMultiplyAdd = EFI_BIT_IS_SET(Ebx, 21);
      Features->PCommit = EFI_BIT_IS_SET(Ebx, 22);
      Features->FlushCacheLineOptimized = EFI_BIT_IS_SET(Ebx, 23);
      Features->CacheLineWriteBack = EFI_BIT_IS_SET(Ebx, 24);
      Features->ProcessorTrace = EFI_BIT_IS_SET(Ebx, 25);
      Features->Avx512Prefetch = EFI_BIT_IS_SET(Ebx, 26);
      Features->Avx512ExponentialAndReciprocal = EFI_BIT_IS_SET(Ebx, 27);
      Features->Avx512ConflictDetection = EFI_BIT_IS_SET(Ebx, 28);
      Features->Sha = EFI_BIT_IS_SET(Ebx, 29);
      Features->Avx512ByteWord = EFI_BIT_IS_SET(Ebx, 30);
      Features->Avx512VectorLength = EFI_BIT_IS_SET(Ebx, 31);
      // Features from ECX
      Features->PrefetchVectorDataIntoCaches = EFI_BIT_IS_SET(Ecx, 0);
      Features->Avx512VectorBitManipulation = EFI_BIT_IS_SET(Ecx, 1);
      Features->UserModePrevention = EFI_BIT_IS_SET(Ecx, 2);
      Features->MemoryProtectionKeys = EFI_BIT_IS_SET(Ecx, 3);
      Features->MemoryProtectKeysEnabled = EFI_BIT_IS_SET(Ecx, 4);
      Features->Avx512VectorBitManipulation2 = EFI_BIT_IS_SET(Ecx, 6);
      Features->GaloisField = EFI_BIT_IS_SET(Ecx, 8);
      Features->AesVector = EFI_BIT_IS_SET(Ecx, 9);
      Features->Avx512CarrylessMultiply = EFI_BIT_IS_SET(Ecx, 10);
      Features->Avx512NeuralNetwork = EFI_BIT_IS_SET(Ecx, 11);
      Features->Avx512BitAlgorithms = EFI_BIT_IS_SET(Ecx, 12);
      Features->Avx512PopulationDoubleQuad = EFI_BIT_IS_SET(Ecx, 14);
      Features->MpxAddressWidthAdjust = (UINT8)EFI_BITFIELD(Ecx, 17, 21);
      Features->ReadProcessorId = EFI_BIT_IS_SET(Ecx, 22);
      Features->SgxLaunchConfiguration = EFI_BIT_IS_SET(Ecx, 30);
      // Features from EDX
      Features->AvxNeuralNetwork = EFI_BIT_IS_SET(Edx, 2);
      Features->Avx512MultiplyAccumulation = EFI_BIT_IS_SET(Edx, 3);
      Features->PlatformConfiguration = EFI_BIT_IS_SET(Edx, 18);
      Features->IndirectBranchControl = EFI_BIT_IS_SET(Edx, 26);
      Features->IndirectBranchControlPredictor = EFI_BIT_IS_SET(Edx, 27);
      Features->ArchitectureCapabilities = EFI_BIT_IS_SET(Edx, 29);
      Features->SpeculativeStoreBypassDisable = EFI_BIT_IS_SET(Edx, 31);
    }
  }
  // Get the max CPUID extended function
  EfiCpuid(0x80000000, &MaxFunction, NULL, NULL, NULL);
  if (MaxFunction >= 0x80000001) {
    // Get the CPU extended features
    EfiCpuid(0x80000001, NULL, NULL, &Ecx, &Edx);
    // Features from EDX
    Features->FloatingPointUnit |= EFI_BIT_IS_SET(Edx, 0);
    Features->VirtualModeExtensions |= EFI_BIT_IS_SET(Edx, 1);
    Features->DebuggingExtensions |= EFI_BIT_IS_SET(Edx, 2);
    Features->PageSizeExtension |= EFI_BIT_IS_SET(Edx, 3);
    Features->TimeStampCounter |= EFI_BIT_IS_SET(Edx, 4);
    Features->ModelSpecificRegisters |= EFI_BIT_IS_SET(Edx, 5);
    Features->PhysicalAddressExtension |= EFI_BIT_IS_SET(Edx, 6);
    Features->MachineCheckException |= EFI_BIT_IS_SET(Edx, 7);
    Features->CompareAndSwap8 |= EFI_BIT_IS_SET(Edx, 8);
    Features->OnboardApic |= EFI_BIT_IS_SET(Edx, 9);
    Features->SystemInstructions |= EFI_BIT_IS_SET(Edx, 11);
    Features->MemoryTypeRangeRegisters |= EFI_BIT_IS_SET(Edx, 12);
    Features->PageGlobalEnable |= EFI_BIT_IS_SET(Edx, 13);
    Features->MachineCheckArchitecture |= EFI_BIT_IS_SET(Edx, 14);
    Features->ConditionalMove |= EFI_BIT_IS_SET(Edx, 15);
    Features->PageAttributeTable |= EFI_BIT_IS_SET(Edx, 16);
    Features->PageSizeExtension36 |= EFI_BIT_IS_SET(Edx, 17);
    Features->MultiprocessorCapable = EFI_BIT_IS_SET(Edx, 19);
    Features->NoExecuteBit = EFI_BIT_IS_SET(Edx, 20);
    Features->ExtendedMmx = EFI_BIT_IS_SET(Edx, 22);
    Features->Mmx |= EFI_BIT_IS_SET(Edx, 23);
    Features->FloatingPointSave |= EFI_BIT_IS_SET(Edx, 24);
    Features->FloatingPointSaveOptimizations = EFI_BIT_IS_SET(Edx, 25);
    Features->GibibytePages = EFI_BIT_IS_SET(Edx, 26);
    Features->TimeStampCounterAndProcessorId = EFI_BIT_IS_SET(Edx, 27);
    Features->LongMode = EFI_BIT_IS_SET(Edx, 29);
    Features->ExtendedThreeDNow = EFI_BIT_IS_SET(Edx, 30);
    Features->ThreeDNow = EFI_BIT_IS_SET(Edx, 31);
    // Features from ECX
    Features->LoadOrStoreFlagsFromLongMode = EFI_BIT_IS_SET(Ecx, 0);
    Features->HyperthreadingUnavailable = EFI_BIT_IS_SET(Ecx, 1);
    Features->SecureVirtualMachine = EFI_BIT_IS_SET(Ecx, 2);
    Features->ExtendedApicSpace = EFI_BIT_IS_SET(Ecx, 3);
    Features->Cr8Legacy = EFI_BIT_IS_SET(Ecx, 4);
    Features->AdvancedBitManipulation = EFI_BIT_IS_SET(Ecx, 5);
    Features->Sse4a = EFI_BIT_IS_SET(Ecx, 6);
    Features->MisalignedSse = EFI_BIT_IS_SET(Ecx, 7);
    Features->ThreeDNowPrefetch = EFI_BIT_IS_SET(Ecx, 8);
    Features->OsVisibleWorkaround = EFI_BIT_IS_SET(Ecx, 9);
    Features->InstructionBasedSampling = EFI_BIT_IS_SET(Ecx, 10);
    Features->Xop = EFI_BIT_IS_SET(Ecx, 11);
    Features->SecureInitJumpAndSetGlobalInterrupt = EFI_BIT_IS_SET(Ecx, 12);
    Features->WatchdogTimer = EFI_BIT_IS_SET(Ecx, 13);
    Features->LightWeightProfiling = EFI_BIT_IS_SET(Ecx, 15);
    Features->FusedMultiplyAdd4 = EFI_BIT_IS_SET(Ecx, 16);
    Features->TranslationCacheExtension = EFI_BIT_IS_SET(Ecx, 17);
    Features->NodeIdMsr = EFI_BIT_IS_SET(Ecx, 19);
    Features->TrailingBitManipulation = EFI_BIT_IS_SET(Ecx, 21);
    Features->TopologyExtensions = EFI_BIT_IS_SET(Ecx, 22);
    Features->PerformanceCounter = EFI_BIT_IS_SET(Ecx, 23);
    Features->PerformanceCounterNB = EFI_BIT_IS_SET(Ecx, 24);
    Features->DataBreakpoint = EFI_BIT_IS_SET(Ecx, 26);
    Features->PerformanceTimeStampCounter = EFI_BIT_IS_SET(Ecx, 27);
    Features->PerformanceCounterL2I = EFI_BIT_IS_SET(Ecx, 28);
  }
  return EFI_SUCCESS;
}
// AmdCpuGetNumberOfProcessors
/// TODO: Get the number of processors
/// @param NumberOfLogicalProcessors  On output, the number of logical processors
/// @param NumberOfPhysicalProcessors On output, the number of physical processors
/// @retval EFI_INVALID_PARAMETER If NumberOfLogicalProcessors, NumberOfPhysicalProcessors, or NumberOfPackages is NULL
/// @retval EFI_SUCCEESS          Tthe number of processors was returned successfully
EFI_STATUS
EFIAPI
AmdCpuGetNumberOfProcessors (
  OUT UINTN *NumberOfLogicalProcessors,
  OUT UINTN *NumberOfPhysicalProcessors,
  OUT UINTN *NumberOfPackages
) {
  // Check parameters
  if ((NumberOfLogicalProcessors == NULL) || (NumberOfPhysicalProcessors == NULL) || (NumberOfPackages == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  return EFI_UNSUPPORTED;
}

#endif
