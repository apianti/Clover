//
/// @file Include/Uefi/Language.h
///
/// UEFI implementation language utilities
///

#pragma once
#ifndef ___UEFI_LANGUAGE_HEADER__
#define ___UEFI_LANGUAGE_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EfiGetBestLanguage
/// Select the best matching language from a set of supported languages
/// @param SupportedLanguages A null-terminated string that contains a set of language codes in the format specified by Iso639Language
/// @param Iso639Language     If TRUE, then all language codes are assumed to be in ISO 639-2 format,
///                            if FALSE, then all language codes are assumed to be in RFC 4646 language format
/// @param ... A NULL-terminated variable argument list specifying the preferred order of the language to select
/// @return The best matching language which must be freed
EXTERN
CHAR8 *
EFIAPI
EfiGetBestLanguage (
  IN CONST CHAR8 *SupportedLanguages, 
  IN BOOLEAN      Iso639Language,
  ...
);
// EfiIsSupportedLanguage
/// Check whether the language is supported from a set of supported languages
/// @param SupportedLanguages A null-terminated string that contains a set of language codes in the format specified by Iso639Language
/// @param Iso639Language     If TRUE, then all language codes are assumed to be in ISO 639-2 format,
///                            if FALSE, then all language codes are assumed to be in RFC 4646 language format
/// @param ... A NULL-terminated variable argument list specifying the preferred order of the language to select
/// @return Whether the language is supported or not
EXTERN
BOOLEAN
EFIAPI
EfiIsSupportedLanguage (
  IN CONST CHAR8  *SupportedLanguages, 
  IN BOOLEAN       Iso639Language,
  ...
);
// EfiSetLanguage
/// Set the language
/// @param Language The language code to discover collation protocol or NULL for default english
/// @return Whether the language protocol was changed or not
/// @retval EFI_NOT_FOUND If the language did not match a collation protocol
/// @retval EFI_SUCCESS   If the language was changed
EXTERN
EFI_STATUS
EFIAPI
EfiSetLanguage (
  IN CONST CHAR8 *Language OPTIONAL
);
// EfiGetLanguage
/// Get the language
/// @param Language On output, the language code used to discover collation protocol, which must be freed
/// @return Whether the language was returned or not
/// @retval EFI_INVALID_PARAMETER If Language is NULL or *Language is not NULL
/// @retval EFI_OUT_OF_RESOURCES If memory could not be allocated for the string
/// @retval EFI_SUCCESS          If the language was returned successfully
EXTERN
EFI_STATUS
EFIAPI
EfiGetLanguage (
  OUT CHAR8 **Language
);
// EfiGetLanguages
/// Get the available languages
/// @param Languages On output, languages available for collation
/// @param Count     On output, the count of languages
/// @return Whether the languages available for collation were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Languages or Count is NULL or *Languages is not NULL
/// @retval EFI_NOT_FOUND         If no collation protocol with valid language support could be found
/// @retval EFI_SUCCESS           If the languages were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
EfiGetLanguages (
  OUT CHAR8 ***Languages,
  OUT UINTN   *Count
);

// EfiGetLanguageCollationProtocol
/// Get a collation protocol by language
/// @param Language The language of the collation protocol or NULL for the current language collation prtocol
/// @return The collation protocol for the specified language or NULL if not found
EXTERN
EFI_UNICODE_COLLATION_PROTOCOL *
EFIAPI
EfiGetLanguageCollationProtocol (
  IN CONST CHAR8 *Language OPTIONAL
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_LANGUAGE_HEADER__
