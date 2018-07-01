///
/// @file Application/Gui/Gui.c
///
/// GUI application entry point
///

#include <Gui/Client.h>

#include <Gui/config.plist.h>

// EFI_PROJECT_CONFIGURATION_FILE
/// The default configuration file device path
#if !defined(EFI_PROJECT_CONFIGURATION_FILE)
# define EFI_PROJECT_CONFIGURATION_FILE L"\\EFI\\" EFI_PROJECT_NAME L"\\" EFI_PROJECT_NAME L".plist"
#endif

// EFI_GUI_BUILD_OPTIONS
/// The build options as a string
#if defined(EFI_DEBUG)
# if defined(EFI_MEMORY_VIRTUAL)
#  define EFI_GUI_BUILD_OPTIONS L" (Debug, Virtual Memory)"
# else
#  define EFI_GUI_BUILD_OPTIONS L" (Debug)"
# endif
#elif defined(EFI_MEMORY_VIRTUAL)
# define EFI_GUI_BUILD_OPTIONS L" (Virtual Memory)"
#else
# define EFI_GUI_BUILD_OPTIONS
#endif

// LOG_CPU_FEATURE
/// Log a cpu feature
/// @param CpuFeatures The CPU_FEATURES structure
/// @param Feature     The feature in the structure to log
#define LOG_CPU_FEATURE(CpuFeatures, Feature) VERBOSE(L"  %-38a %a\n", #Feature ":" , (CpuFeatures.Feature) ? "Yes" : "No")

// GuiServerStart
/// GUI server start
/// @param This The GUI server protocol
/// @retval EFI_SUCCESS The GUI started successfully
EXTERN
EFI_STATUS
EFIAPI
GuiServerStart (
  VOID
);
// GuiServerRun
/// GUI server run loop
/// @retval EFI_INVALID_PARAMETER If Server is NULL or not valid
/// @retval EFI_NOT_READY         If the server was not started
/// @retval EFI_SUCCESS           The run loop finished successfully
EXTERN
EFI_STATUS
EFIAPI
GuiServerRun (
  VOID
);
// GuiServerFinish
/// GUI server finish execution
/// @retval EFI_INVALID_PARAMETER If This is NULL
/// @retval EFI_SUCCESS           The GUI finished successfully
EXTERN
EFI_STATUS
EFIAPI
GuiServerFinish (
  VOID
);

// GuiCpuInformation
/// GUI CPU information
STATIC
VOID
GuiCpuInformation (
  VOID
) {
  CHAR16       *Description;
  UINTN         NumberOfLogicalProcessors;
  UINTN         NumberOfPhysicalProcessors;
  UINTN         NumberOfPackages;
  UINT16        CpuFamily;
  UINT8         CpuModel;
  UINT8         CpuStepping;
  CPU_FEATURES  CpuFeatures;
  // Log CPU information
  LOG(L"Virtual machine: %a\nArchitecture: " EFI_PROJECT_ARCH L"\n", EfiIsVirtualMachine() ? "Yes" : "No");
  LOG(L"Multiprocessing: %a\n", CpuIsMultiprocessor() ? "Yes" : "No");
  // Get the CPU description
  Description = CpuGetDescription();
  if (Description != NULL) {
    LOG(L"CPU: %s\n", Description);
    EfiFreePool(Description);
  } else {
    LOG(L"CPU: Unknown CPU\n");
  }
  // Get CPU family, model, and stepping
  CpuFamily = 0;
  CpuModel = 0;
  CpuStepping = 0;
  LOG(L"CPU signature: 0x%08x\n", CpuGetSignature(&CpuFamily, &CpuModel, &CpuStepping));
  LOG(L"CPU family: 0x%x\nCPU model: 0x%x\nCPU stepping: 0x%x\n", (UINTN)CpuFamily, (UINTN)CpuModel, (UINTN)CpuStepping);
  // Get CPU features
  EfiZero(CPU_FEATURES, &CpuFeatures);
  CpuGetFeatures(&CpuFeatures);
  // Get the number of logical processors
  NumberOfLogicalProcessors = 1;
  NumberOfPhysicalProcessors = 1;
  NumberOfPackages = 1;
  CpuGetNumberOfProcessors(&NumberOfLogicalProcessors, &NumberOfPhysicalProcessors, &NumberOfPackages);
  LOG(L"CPU logical processors: %u\nCPU physical processors: %u\nCPU packages: %u\n", NumberOfLogicalProcessors, NumberOfPhysicalProcessors, NumberOfPackages);
  // Verbose log CPU features
  VERBOSE(L"CPU features:\n");
#if defined(EFI_ARCH_IA32) || defined(EFI_ARCH_X64)

  // Log IA32 and X64 features
  LOG_CPU_FEATURE(CpuFeatures, FloatingPointUnit);
  LOG_CPU_FEATURE(CpuFeatures, VirtualModeExtensions);
  LOG_CPU_FEATURE(CpuFeatures, DebuggingExtensions);
  LOG_CPU_FEATURE(CpuFeatures, PageSizeExtension);
  LOG_CPU_FEATURE(CpuFeatures, TimeStampCounter);
  LOG_CPU_FEATURE(CpuFeatures, ModelSpecificRegisters);
  LOG_CPU_FEATURE(CpuFeatures, PhysicalAddressExtension);
  LOG_CPU_FEATURE(CpuFeatures, MachineCheckException);
  LOG_CPU_FEATURE(CpuFeatures, CompareAndSwap8);
  LOG_CPU_FEATURE(CpuFeatures, SystemInstructions);
  LOG_CPU_FEATURE(CpuFeatures, MemoryTypeRangeRegisters);
  LOG_CPU_FEATURE(CpuFeatures, PageGlobalEnable);
  LOG_CPU_FEATURE(CpuFeatures, MachineCheckArchitecture);
  LOG_CPU_FEATURE(CpuFeatures, ConditionalMove);
  LOG_CPU_FEATURE(CpuFeatures, PageAttributeTable);
  LOG_CPU_FEATURE(CpuFeatures, PageSizeExtension36);
  LOG_CPU_FEATURE(CpuFeatures, ProcessSerialNumber);
  LOG_CPU_FEATURE(CpuFeatures, FlushCacheLine);
  LOG_CPU_FEATURE(CpuFeatures, DebugStore);
  LOG_CPU_FEATURE(CpuFeatures, MsrForThermalAcpi);
  LOG_CPU_FEATURE(CpuFeatures, Mmx);
  LOG_CPU_FEATURE(CpuFeatures, FloatingPointSave);
  LOG_CPU_FEATURE(CpuFeatures, Sse);
  LOG_CPU_FEATURE(CpuFeatures, Sse2);
  LOG_CPU_FEATURE(CpuFeatures, SelfSnoop);
  LOG_CPU_FEATURE(CpuFeatures, Hyperthreading);
  LOG_CPU_FEATURE(CpuFeatures, ThermalMonitor);
  LOG_CPU_FEATURE(CpuFeatures, Ia64EmulateIa32);
  LOG_CPU_FEATURE(CpuFeatures, PendingBreakEnable);
  LOG_CPU_FEATURE(CpuFeatures, Sse3);
  LOG_CPU_FEATURE(CpuFeatures, CarrylessMultiply);
  LOG_CPU_FEATURE(CpuFeatures, DebugStore64);
  LOG_CPU_FEATURE(CpuFeatures, MonitorWait);
  LOG_CPU_FEATURE(CpuFeatures, DebugStoreCpl);
  LOG_CPU_FEATURE(CpuFeatures, VirtualMachineExtensions);
  LOG_CPU_FEATURE(CpuFeatures, SafeModeExtensions);
  LOG_CPU_FEATURE(CpuFeatures, EnhancedSpeedStep);
  LOG_CPU_FEATURE(CpuFeatures, ThermalMonitor2);
  LOG_CPU_FEATURE(CpuFeatures, Ssse3);
  LOG_CPU_FEATURE(CpuFeatures, ContextId);
  LOG_CPU_FEATURE(CpuFeatures, SiliconDebug);
  LOG_CPU_FEATURE(CpuFeatures, FusedMultiplyAdd);
  LOG_CPU_FEATURE(CpuFeatures, CompareAndSwap16);
  LOG_CPU_FEATURE(CpuFeatures, DisableTaskPriorityMessages);
  LOG_CPU_FEATURE(CpuFeatures, PerformanceMonitor);
  LOG_CPU_FEATURE(CpuFeatures, ProcessContextId);
  LOG_CPU_FEATURE(CpuFeatures, DirectCacheAccess);
  LOG_CPU_FEATURE(CpuFeatures, Sse4_1);
  LOG_CPU_FEATURE(CpuFeatures, Sse4_2);
  LOG_CPU_FEATURE(CpuFeatures, X2Apic);
  LOG_CPU_FEATURE(CpuFeatures, ByteSwapMove);
  LOG_CPU_FEATURE(CpuFeatures, CountBits);
  LOG_CPU_FEATURE(CpuFeatures, TimeStampCounterDeadline);
  LOG_CPU_FEATURE(CpuFeatures, Aes);
  LOG_CPU_FEATURE(CpuFeatures, SaveExtendedState);
  LOG_CPU_FEATURE(CpuFeatures, SaveExtendedStateEnabled);
  LOG_CPU_FEATURE(CpuFeatures, AdvancedVectorExtensions);
  LOG_CPU_FEATURE(CpuFeatures, HalfPrecisionFloatingPoint);
  LOG_CPU_FEATURE(CpuFeatures, RandomNumberGenerator);
  LOG_CPU_FEATURE(CpuFeatures, Hypervisor);
  LOG_CPU_FEATURE(CpuFeatures, ReadSegmentBase);
  LOG_CPU_FEATURE(CpuFeatures, Ia32TimeStampCounterAdjust);
  LOG_CPU_FEATURE(CpuFeatures, SoftwareGuardExtensions);
  LOG_CPU_FEATURE(CpuFeatures, BitManipulation);
  LOG_CPU_FEATURE(CpuFeatures, TransactionalSyncExtensions);
  LOG_CPU_FEATURE(CpuFeatures, AdvancedVectorExtensions2);
  LOG_CPU_FEATURE(CpuFeatures, SupervisorModeExecutionPrevention);
  LOG_CPU_FEATURE(CpuFeatures, BitManipulation2);
  LOG_CPU_FEATURE(CpuFeatures, EnhanceRepeatMove);
  LOG_CPU_FEATURE(CpuFeatures, InvalidateProcessContextId);
  LOG_CPU_FEATURE(CpuFeatures, TransactionalRestrictedSyncExtensions);
  LOG_CPU_FEATURE(CpuFeatures, PlatformQualityOfServiceMonitor);
  LOG_CPU_FEATURE(CpuFeatures, FloatingPointDeprecated);
  LOG_CPU_FEATURE(CpuFeatures, Mpx);
  LOG_CPU_FEATURE(CpuFeatures, PlatformQualityOfServiceEnforcement);
  LOG_CPU_FEATURE(CpuFeatures, Avx512);
  LOG_CPU_FEATURE(CpuFeatures, Avx512DoubleQuad);
  LOG_CPU_FEATURE(CpuFeatures, RandomSeed);
  LOG_CPU_FEATURE(CpuFeatures, Adx);
  LOG_CPU_FEATURE(CpuFeatures, SupervisorModeAccessPrevention);
  LOG_CPU_FEATURE(CpuFeatures, Avx512IntegerFusedMultiplyAdd);
  LOG_CPU_FEATURE(CpuFeatures, PCommit);
  LOG_CPU_FEATURE(CpuFeatures, FlushCacheLineOptimized);
  LOG_CPU_FEATURE(CpuFeatures, CacheLineWriteBack);
  LOG_CPU_FEATURE(CpuFeatures, ProcessorTrace);
  LOG_CPU_FEATURE(CpuFeatures, Avx512Prefetch);
  LOG_CPU_FEATURE(CpuFeatures, Avx512ExponentialAndReciprocal);
  LOG_CPU_FEATURE(CpuFeatures, Avx512ConflictDetection);
  LOG_CPU_FEATURE(CpuFeatures, Sha);
  LOG_CPU_FEATURE(CpuFeatures, Avx512ByteWord);
  LOG_CPU_FEATURE(CpuFeatures, Avx512VectorLength);
  LOG_CPU_FEATURE(CpuFeatures, PrefetchVectorDataIntoCaches);
  LOG_CPU_FEATURE(CpuFeatures, Avx512VectorBitManipulation);
  LOG_CPU_FEATURE(CpuFeatures, UserModePrevention);
  LOG_CPU_FEATURE(CpuFeatures, MemoryProtectionKeys);
  LOG_CPU_FEATURE(CpuFeatures, MemoryProtectKeysEnabled);
  LOG_CPU_FEATURE(CpuFeatures, Avx512VectorBitManipulation2);
  LOG_CPU_FEATURE(CpuFeatures, GaloisField);
  LOG_CPU_FEATURE(CpuFeatures, AesVector);
  LOG_CPU_FEATURE(CpuFeatures, Avx512CarrylessMultiply);
  LOG_CPU_FEATURE(CpuFeatures, Avx512NeuralNetwork);
  LOG_CPU_FEATURE(CpuFeatures, Avx512BitAlgorithms);
  LOG_CPU_FEATURE(CpuFeatures, Avx512PopulationDoubleQuad);
  VERBOSE(L"  %-38a 0x%02x\n", "MpxAddressWidthAdjust:", CpuFeatures.MpxAddressWidthAdjust);
  LOG_CPU_FEATURE(CpuFeatures, ReadProcessorId);
  LOG_CPU_FEATURE(CpuFeatures, SgxLaunchConfiguration);
  LOG_CPU_FEATURE(CpuFeatures, AvxNeuralNetwork);
  LOG_CPU_FEATURE(CpuFeatures, Avx512MultiplyAccumulation);
  LOG_CPU_FEATURE(CpuFeatures, PlatformConfiguration);
  LOG_CPU_FEATURE(CpuFeatures, IndirectBranchControl);
  LOG_CPU_FEATURE(CpuFeatures, IndirectBranchControlPredictor);
  LOG_CPU_FEATURE(CpuFeatures, ArchitectureCapabilities);
  LOG_CPU_FEATURE(CpuFeatures, SpeculativeStoreBypassDisable);
  LOG_CPU_FEATURE(CpuFeatures, MultiprocessorCapable);
  LOG_CPU_FEATURE(CpuFeatures, NoExecuteBit);
  LOG_CPU_FEATURE(CpuFeatures, ExtendedMmx);
  LOG_CPU_FEATURE(CpuFeatures, FloatingPointSaveOptimizations);
  LOG_CPU_FEATURE(CpuFeatures, GibibytePages);
  LOG_CPU_FEATURE(CpuFeatures, TimeStampCounterAndProcessorId);
  LOG_CPU_FEATURE(CpuFeatures, LongMode);
  LOG_CPU_FEATURE(CpuFeatures, ExtendedThreeDNow);
  LOG_CPU_FEATURE(CpuFeatures, ThreeDNow);
  LOG_CPU_FEATURE(CpuFeatures, LoadOrStoreFlagsFromLongMode);
  LOG_CPU_FEATURE(CpuFeatures, HyperthreadingUnavailable);
  LOG_CPU_FEATURE(CpuFeatures, SecureVirtualMachine);
  LOG_CPU_FEATURE(CpuFeatures, ExtendedApicSpace);
  LOG_CPU_FEATURE(CpuFeatures, Cr8Legacy);
  LOG_CPU_FEATURE(CpuFeatures, AdvancedBitManipulation);
  LOG_CPU_FEATURE(CpuFeatures, Sse4a);
  LOG_CPU_FEATURE(CpuFeatures, MisalignedSse);
  LOG_CPU_FEATURE(CpuFeatures, ThreeDNowPrefetch);
  LOG_CPU_FEATURE(CpuFeatures, OsVisibleWorkaround);
  LOG_CPU_FEATURE(CpuFeatures, InstructionBasedSampling);
  LOG_CPU_FEATURE(CpuFeatures, Xop);
  LOG_CPU_FEATURE(CpuFeatures, SecureInitJumpAndSetGlobalInterrupt);
  LOG_CPU_FEATURE(CpuFeatures, WatchdogTimer);
  LOG_CPU_FEATURE(CpuFeatures, LightWeightProfiling);
  LOG_CPU_FEATURE(CpuFeatures, FusedMultiplyAdd4);
  LOG_CPU_FEATURE(CpuFeatures, TranslationCacheExtension);
  LOG_CPU_FEATURE(CpuFeatures, NodeIdMsr);
  LOG_CPU_FEATURE(CpuFeatures, TrailingBitManipulation);
  LOG_CPU_FEATURE(CpuFeatures, TopologyExtensions);
  LOG_CPU_FEATURE(CpuFeatures, PerformanceCounter);
  LOG_CPU_FEATURE(CpuFeatures, PerformanceCounterNB);
  LOG_CPU_FEATURE(CpuFeatures, DataBreakpoint);
  LOG_CPU_FEATURE(CpuFeatures, PerformanceTimeStampCounter);
  LOG_CPU_FEATURE(CpuFeatures, PerformanceCounterL2I);

#elif defined(EFI_ARCH_ARM) || defined(EFI_ARCH_AA64)

  // TODO: Log CPU features for ARM and ARM64 architectures
  
#endif
}

// GuiImportConfiguration
/// Import the default and user configuration PLIST dictionaries
STATIC
VOID
EFIAPI
GuiImportConfiguration (
  VOID
) {
  EFI_STATUS    Status;
  PLIST_PARSER *Parser;
  PLIST_KEY    *Plist;
  CONST CHAR16 *Source;
  // Create the PLIST parser
  LOGDIV();
  LOG(L"Loading default configuration ... ");
  Parser = NULL;
  Status = PlistCreate(&Parser, L"config.plist");
  if (!EFI_ERROR(Status)) {
    // Parse the default configuration
    Plist = NULL;
    Status = PlistParse(Parser, NULL, config_plist, config_plist_len);
    if (!EFI_ERROR(Status)) {
      // Finish parsing the default configuration into the PLIST dictionary
      Status = PlistParseFinish(Parser, &Plist);
      if (!EFI_ERROR(Status)) {
        // Import the configuration from the PLIST
        LOG(L"%r\nImporting default configuration ... ", Status);
        Status = PlistImportConfiguration(Plist, NULL);
      }
    }
    LOG(L"%r\n", Status);
    if (Plist != NULL) {
      PlistDictionaryFree(Plist);
    }
  }
  if (Parser != NULL) {
    // Log parser messages
    LogParseMessages(ParserFromPlistParser(Parser), NULL);
  }
  // Get the configuration path (this will have been set by load options parsing if applicable)
  Source = EfiConfigurationGetString(L"/Configuration/Path", EFI_PROJECT_CONFIGURATION_FILE);
  if (Source != NULL) {
    LOG(L"Loading configuration: `%s` ... ", Source);
    // Reset the parser
    Status = PlistReset(Parser, Source);
    if (!EFI_ERROR(Status)) {
      // Check if configuration file exists
      Status = EfiFileExistsPath(Source);
      if (!EFI_ERROR(Status)) {
        VOID   *Buffer = NULL;
        UINT64  Size = 0;
        // Load configuration
        Status = EfiFileLoadPath(Source, &Size, &Buffer);
        if (!EFI_ERROR(Status)) {
          if (Buffer == NULL) {
            Status = EFI_OUT_OF_RESOURCES;
          } else {
            // Parse loaded configuration
            Plist = NULL;
            Status = PlistParse(Parser, NULL, Buffer, (UINTN)Size);
            // Free the configuration file buffer
            EfiFreePool(Buffer);
            if (!EFI_ERROR(Status)) {
              // Finish parsing the configuration into the PLIST dictionary
              Status = PlistParseFinish(Parser, &Plist);
              if (!EFI_ERROR(Status)) {
                // Import loaded configuration from PLIST
                LOG(L"%r\nImporting configuration ... ", Status);
                Status = PlistImportConfiguration(Plist, NULL);
              }
            }
            if (Plist != NULL) {
              PlistDictionaryFree(Plist);
            }
          }
        }
      }
    }
    LOG(L"%r\n", Status);
  }
  if (Parser != NULL) {
    // Log parser messages
    LogParseMessages(ParserFromPlistParser(Parser), NULL);
    // Free PLIST parser
    PlistFree(Parser);
  }
}
// GuiSetupLog
/// Setup logging for the GUI
STATIC
VOID
EFIAPI
GuiSetupLog (
  VOID
) {
  EFI_STATUS    Status;
  CONST CHAR16 *String;
  UINTN         Width;
  UINTN         Height;
  UINT32        Outputs;
  // Switch to best console mode
  LOGDIV();
  Width = (UINTN)EfiConfigurationGetUnsigned(L"/Boot/Console/Columns", 0);
  Height = (UINTN)EfiConfigurationGetUnsigned(L"/Boot/Console/Rows", 0);
  if ((Width != 0) || (Height != 0)) {
    LOG(L"Switch to console mode: %u x %u ... ", Width, Height);
    Status = EfiConOutSwitchToMode(Width, Height);
    LOG(L"%r\n", Status);
  } else {
    String = EfiConfigurationGetString(L"/Boot/Console", NULL);
    if ((String == NULL) || (*String == 0)) {
      // Only switch to best console mode if the user wants that
      if (EfiConfigurationGetBoolean(L"/Boot/Console", FALSE)) {
        LOG(L"Switch to best console mode ... ");
        Status = EfiConOutSwitchToBestMode();
        LOG(L"%r\n", Status);
      }
    } else {
      LOG(L"Switch to user console mode `%s` ... ", String);
      Status = EfiConOutSwitchToModeString(String);
      LOG(L"%r\n", Status);
    }
  }
  // Log the current console mode
  if (!EFI_ERROR(EfiConOutQueryMode(EfiConOutCurrentMode(), &Width, &Height))) {
    LOG(L"Console mode: %u x %u\n", Width, Height);
  }
  // Get the default outputs
  Outputs = EfiIsDebug() ? EFI_LOG_OUTPUT_ALL : EfiLogGetDefaultOutputs();
  // Set the new log outputs and flush
  EfiLogSetOutputs(Outputs, TRUE);
  // Log the outputs in use
  Outputs = EfiLogGetOutputs();
  LOG(L"Log outputs:");
  if (EFI_BITS_ARE_UNSET(Outputs, EFI_LOG_OUTPUT_ALL)) {
    LOG(L" None\n");
  } else {
    if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_CONSOLE)) {
      LOG(L" Console");
    }
    if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_FILE)) {
      LOG(L" File");
    }
    if (EFI_BITS_ANY_SET(Outputs, EFI_LOG_OUTPUT_SERIAL)) {
      LOG(L" Serial");
    }
    LOG(L"\r\n");
  }
  // Output log path if used
  String = EfiLogGetFile();
  if (String != NULL) {
    LOG(L"Log file path: `%s`\n", String);
  }
#if defined(EFI_DEBUG)
  // Wait for a key to be pressed to continue while debugging
  EfiConOutPrint(L"Press any key to continue ...\r\n");
  EfiConInWaitForAnyKey();
#endif
}

// EfiEntryPoint
/// The GUI entry point
EFI_STATUS
EFIAPI
EfiEntryPoint (
  VOID
) {
  EFI_STATUS  Status;
  EFI_TIME    Time;
  // Get the start time
  EfiZero(EFI_TIME, &Time);
  EfiGetTime(&Time, NULL);
  // Clear the console immediately
  EfiConOutClear(EFI_BACKGROUND_BLACK | EFI_WHITE, FALSE);
  // Log application information
  LOG(EFI_PROJECT_NAME L" Boot Manager" EFI_GUI_BUILD_OPTIONS L"\n");
  LOG(L"Started: %T\n", &Time);
  LOG(L"Version: " EFI_PROJECT_VERSION L"\nCommit: " EFI_PROJECT_VERSION_HASH L"\n");
  // Log firmware information
  if ((gEfiSystemTable == NULL) || (gEfiSystemTable->FirmwareVendor == NULL)) {
    LOG(L"Firmware: UNKNOWN\n");
  } else {
    LOG(L"Firmware: %s %u.%u\n", gEfiSystemTable->FirmwareVendor,
        EFI_BITFIELD(gEfiSystemTable->Hdr.Revision, 16, 31),
        EFI_BITFIELD(gEfiSystemTable->Hdr.Revision, 0, 15));
  }
  // Get CPU information
  GuiCpuInformation();
  // Import configuration
  GuiImportConfiguration();
  // Setup log
  GuiSetupLog();
  // Get the GUI arrival time
  LOGDIV();
  EfiGetTime(&Time, NULL);
  LOG(L"GUI: %T\n", &Time);
  // Start the GUI
  Status = GuiServerStart();
  // TODO: Create boot entries from NVRAM, configuration, and file system volumes

  // Run GUI main loop
  Status = GuiServerRun();
  // Finish using GUI
  LOGDIV();
  GuiServerFinish();
  // Get the finished time
  EfiGetTime(&Time, NULL);
  LOG(L"Finished: %T\n", &Time);
  LOGDIV();
  EfiGetTime(&Time, NULL);
  LOG(L"%a: %T\n", EFI_ERROR(Status) ? "Failed" : "Quit", &Time);
#if defined(EFI_DEBUG)
  // Wait for a key to be pressed to exit while debugging
  EfiConOutPrint(L"Press any key to exit ...\r\n");
  EfiConInWaitForAnyKey();
#endif
  LOGDIV();
  return Status;
}
