///
/// @file Library/Uefi/Language.c
///
/// UEFI implementation language utilities
///

#include <Uefi.h>

// DEFAULT_LANGUAGE
/// The default language
#define DEFAULT_LANGUAGE "en"

// EfiGetBestLanguage
/// Select the best matching language from a set of supported languages
/// @param SupportedLanguages A null-terminated string that contains a set of language codes in the format specified by Iso639Language
/// @param Iso639Language     If TRUE, then all language codes are assumed to be in ISO 639-2 format,
///                            if FALSE, then all language codes are assumed to be in RFC 4646 language format
/// @param ...                A NULL-terminated variable argument list specifying the preferred order of the language to select
/// @return The best matching language which must be freed
CHAR8 *
EFIAPI
EfiGetBestLanguage (
  IN CONST CHAR8  *SupportedLanguages, 
  IN BOOLEAN       Iso639Language,
  ...
) {
  VA_LIST       Args;
  CHAR8        *Language;
  UINTN         CompareLength;
  UINTN         LanguageLength;
  CONST CHAR8  *Supported;

  // Check parameters
  if (SupportedLanguages == NULL) {
    return NULL;
  }
  // Get best language from language codes list
  VA_START(Args, Iso639Language);
  while ((Language = VA_ARG(Args, CHAR8 *)) != NULL) {
    // Default to ISO 639-2 mode
    CompareLength = 3;
    LanguageLength = AsciiStrLen(Language);
    if (LanguageLength > 3) {
      LanguageLength = 3;
    }
    // If in RFC 4646 mode, then determine the length of the first RFC 4646 language code in Language
    if (!Iso639Language) {
      for (LanguageLength = 0; ((Language[LanguageLength] != 0) && (Language[LanguageLength] != ';')); ++LanguageLength);
    }
    // Trim back the length of Language used until it is empty
    while (LanguageLength > 0) {
      // Loop through all language codes in SupportedLanguages
      for (Supported = SupportedLanguages; *Supported != 0; Supported += CompareLength) {
        // In RFC 4646 mode, then Loop through all language codes in SupportedLanguages
        if (!Iso639Language) {
          // Skip ';' characters in Supported
          for (; ((*Supported != 0) && (*Supported == ';')); ++Supported);
          // Determine the length of the next language code in Supported
          for (CompareLength = 0; ((Supported[CompareLength] != 0) && (Supported[CompareLength] != ';')); ++CompareLength);
          // If Language is longer than the Supported, then skip to the next language
          if (LanguageLength > CompareLength) {
            continue;
          }
        }
        // See if the first LanguageLength characters in Supported match Language
        if (AsciiStrnCmp(Supported, Language, LanguageLength) == 0) {
          VA_END(Args);
          // Allocate, copy, and return the best matching language code from SupportedLanguages
          return AsciiStrnDup(Supported, CompareLength);
        }
      }
      if (Iso639Language) {
        // If ISO 639 mode, then each language can only be tested once
        LanguageLength = 0;
      } else {
        // If RFC 4646 mode, then trim Language from the right to the next '-' character 
        for (--LanguageLength; ((LanguageLength > 0) && (Language[LanguageLength] != '-')); --LanguageLength);
      }
    }
  }
  VA_END(Args);
  // No matches were found 
  return NULL;
}
// EfiIsSupportedLanguage
/// Check whether the language is supported from a set of supported languages
/// @param SupportedLanguages A null-terminated string that contains a set of language codes in the format specified by Iso639Language
/// @param Iso639Language     If TRUE, then all language codes are assumed to be in ISO 639-2 format,
///                            if FALSE, then all language codes are assumed to be in RFC 4646 language format
/// @param ... A NULL-terminated variable argument list specifying the preferred order of the language to select
/// @return Whether the language is supported or not
BOOLEAN
EFIAPI
EfiIsSupportedLanguage (
  IN CONST CHAR8  *SupportedLanguages, 
  IN BOOLEAN       Iso639Language,
  ...
) {
  VA_LIST       Args;
  CHAR8        *Language;
  UINTN         CompareLength;
  UINTN         LanguageLength;
  CONST CHAR8  *Supported;

  // Check parameters
  if (SupportedLanguages == NULL) {
    return NULL;
  }
  // Get best language from language codes list
  VA_START(Args, Iso639Language);
  while ((Language = VA_ARG(Args, CHAR8 *)) != NULL) {
    // Default to ISO 639-2 mode
    CompareLength = 3;
    LanguageLength = AsciiStrLen(Language);
    if (LanguageLength > 3) {
      LanguageLength = 3;
    }
    // If in RFC 4646 mode, then determine the length of the first RFC 4646 language code in Language
    if (!Iso639Language) {
      for (LanguageLength = 0; ((Language[LanguageLength] != 0) && (Language[LanguageLength] != ';')); ++LanguageLength);
    }
    // Trim back the length of Language used until it is empty
    while (LanguageLength > 0) {
      // Loop through all language codes in SupportedLanguages
      for (Supported = SupportedLanguages; *Supported != 0; Supported += CompareLength) {
        // In RFC 4646 mode, then Loop through all language codes in SupportedLanguages
        if (!Iso639Language) {
          // Skip ';' characters in Supported
          for (; ((*Supported != 0) && (*Supported == ';')); ++Supported);
          // Determine the length of the next language code in Supported
          for (CompareLength = 0; ((Supported[CompareLength] != 0) && (Supported[CompareLength] != ';')); ++CompareLength);
          // If Language is longer than the Supported, then skip to the next language
          if (LanguageLength > CompareLength) {
            continue;
          }
        }
        // See if the first LanguageLength characters in Supported match Language
        if (AsciiStrnCmp(Supported, Language, LanguageLength) == 0) {
          VA_END(Args);
          // Allocate, copy, and return the best matching language code from SupportedLanguages
          return TRUE;
        }
      }
      if (Iso639Language) {
        // If ISO 639 mode, then each language can only be tested once
        LanguageLength = 0;
      } else {
        // If RFC 4646 mode, then trim Language from the right to the next '-' character 
        for (--LanguageLength; ((LanguageLength > 0) && (Language[LanguageLength] != '-')); --LanguageLength);
      }
    }
  }
  VA_END(Args);
  // No matches were found 
  return FALSE;
}

// EfiSetLanguage
/// Set the language
/// @param Language The language code to discover collation protocol or NULL for default english
/// @return Whether the language protocol was changed or not
/// @retval EFI_NOT_FOUND If the language did not match a collation protocol
/// @retval EFI_SUCCESS   If the language was changed
EFI_STATUS
EFIAPI
EfiSetLanguage (
  IN CONST CHAR8 *Language OPTIONAL
) {
  EFI_STATUS   Status;
  UINT32       Options = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
  CHAR8       *PlatformLanguages = NULL;
  CHAR8       *LanguageCode;
  // Get the language to set
  if ((Language == NULL) || (AsciiStrLen(Language) == 0)) {
    Language = DEFAULT_LANGUAGE;
  }
  // Get the supported platform languages
  Status = EfiGetGlobalVariable(L"PlatformLangCodes", NULL, NULL, (VOID **)&PlatformLanguages);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if(PlatformLanguages == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Get the best language code
  LanguageCode = EfiGetBestLanguage(PlatformLanguages, FALSE, Language, NULL);
  EfiFreePool(PlatformLanguages);
  if (LanguageCode == NULL) {
    return EFI_NOT_FOUND;
  }
  // Set platform language
  Status = EfiSetVariable(L"PlatformLang", &gEfiGlobalVariableGuid, Options, AsciiStrSize(LanguageCode), (VOID *)LanguageCode);
  EfiFreePool(LanguageCode);
  return Status;
}
// EfiGetLanguage
/// Get the language
/// @param Language On output, the language code used to discover collation protocol, which must be freed
/// @return Whether the language was returned or not
/// @retval EFI_INVALID_PARAMETER If Language is NULL or *Language is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string
/// @retval EFI_SUCCESS           If the language was returned successfully
EFI_STATUS
EFIAPI
EfiGetLanguage (
  OUT CHAR8 **Language
) {
  CHAR8 *PlatformLanguage = NULL;
  if ((Language == NULL) || (*Language != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the language from the platform
  if (!EFI_ERROR(EfiGetGlobalVariableBuffer(L"PlatformLang", NULL, NULL, (VOID **)&PlatformLanguage)) && (PlatformLanguage != NULL)) {
    *Language = PlatformLanguage;
  } else {
    // Get the default language
    *Language = AsciiStrDup(DEFAULT_LANGUAGE);
  }
  return (*Language == NULL) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
}
// EfiGetLanguages
/// Get the available languages
/// @param Languages On output, languages available for collation
/// @param Count     On output, the count of languages
/// @return Whether the languages available for collation were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Languages or Count is NULL or *Languages is not NULL
/// @retval EFI_NOT_FOUND         If no collation protocol with valid language support could be found
/// @retval EFI_SUCCESS           If the languages were retrieved successfully
EFI_STATUS
EFIAPI
EfiGetLanguages (
  OUT CHAR8 ***Languages,
  OUT UINTN   *Count
) {
  EFI_STATUS   Status = EFI_SUCCESS;
  CHAR8      **AvailableLanguages = NULL;
  CHAR8       *PlatformLanguages = NULL;
  UINTN        LanguageCount = 0;
  if ((Languages == NULL) || (*Languages != NULL) || (Count == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the languages from the platform
  if (!EFI_ERROR(EfiGetGlobalVariableBuffer(L"PlatformLangCodes", NULL, NULL, (VOID **)&PlatformLanguages)) && (PlatformLanguages != NULL)) {
    Status = AsciiStrSplit(&AvailableLanguages, &LanguageCount, PlatformLanguages, ";", STR_LIST_NO_DUPLICATES | STR_LIST_CASE_INSENSITIVE | STR_LIST_SORTED);
    EfiFreePool(PlatformLanguages);
  }
  if (!EFI_ERROR(Status)) {
    // Get the default language
    Status = AsciiStrList(&AvailableLanguages, &LanguageCount, DEFAULT_LANGUAGE, 0, STR_LIST_NO_DUPLICATES | STR_LIST_CASE_INSENSITIVE | STR_LIST_SORTED);
    // Check if default language already exists in list
    if (Status == EFI_ACCESS_DENIED) {
      Status = EFI_SUCCESS;
    }
  }
  // Return languages
  *Languages = AvailableLanguages;
  *Count = LanguageCount;
  return Status;
}

// EfiGetLanguageCollationProtocol
/// Get a collation protocol by language
/// @param Language The language of the collation protocol or NULL for the current language collation prtocol
/// @return The collation protocol for the specified language or NULL if not found
EFI_UNICODE_COLLATION_PROTOCOL *
EFIAPI
EfiGetLanguageCollationProtocol (
  IN CONST CHAR8 *Language OPTIONAL
) {
  EFI_STATUS                      Status;
  EFI_UNICODE_COLLATION_PROTOCOL *Collation;
  EFI_HANDLE                     *Handles = NULL;
  CHAR8                          *LanguageRequest = NULL;
  UINTN                           Count = 0;
  UINTN                           Index;
  // Search for collation protocol
  if ((Language == NULL) || (AsciiStrLen(Language) == 0)) {
    EfiGetLanguage(&LanguageRequest);
  } else {
    LanguageRequest = AsciiStrDup(Language);
  }
  if (LanguageRequest == NULL) {
    return NULL;
  }
  // Locate collation protocols that give language codes in RFC 4646
  Status = EfiLocateHandleBuffer(ByProtocol, &gEfiUnicodeCollation2ProtocolGuid, NULL, &Count, &Handles);
  if (!EFI_ERROR(Status) && (Handles != NULL)) {
    // On first pass try to exact match
    for (Index = 0; Index < Count; ++Index) {
      // Get each collation protocol
      Collation = NULL;
      if (EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiUnicodeCollation2ProtocolGuid, (VOID **)&Collation))) {
        continue;
      }
      // Check the collation protocol has valid language support
      if ((Collation == NULL) || (Collation->SupportedLanguages == NULL)) {
        continue;
      }
      // Check if this protocol supports this language
      if (EfiIsSupportedLanguage(Collation->SupportedLanguages, FALSE, LanguageRequest, NULL)) {
        // Return collation protocol
        EfiFreePool(LanguageRequest);
        return Collation;
      }
    }
  }
  // Locate collation protocols that give language codes in ISO 639-2
  Status = EfiLocateHandleBuffer(ByProtocol, &gEfiUnicodeCollationProtocolGuid, NULL, &Count, &Handles);
  if (!EFI_ERROR(Status) && (Handles != NULL)) {
    // On first pass try to exact match
    for (Index = 0; Index < Count; ++Index) {
      // Get each collation protocol
      Collation = NULL;
      if (EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiUnicodeCollationProtocolGuid, (VOID **)&Collation))) {
        continue;
      }
      // Check the collation protocol has valid language support
      if ((Collation == NULL) || (Collation->SupportedLanguages == NULL)) {
        continue;
      }
      // Check if this protocol supports this language
      if (EfiIsSupportedLanguage(Collation->SupportedLanguages, TRUE, LanguageRequest, NULL)) {
        // Return collation protocol
        EfiFreePool(LanguageRequest);
        return Collation;
      }
    }
  }
  // Not found
  EfiFreePool(LanguageRequest);
  return NULL;
}
