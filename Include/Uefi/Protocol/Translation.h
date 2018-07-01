///
/// @file Include/Uefi/Protocol/Translation.h
///
/// Language translations
///

#pragma once
#ifndef __UEFI_TRANSLATION_HEADER__
#define __UEFI_TRANSLATION_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_TRANSLATION_PROTOCOL_GUID
/// The language translation protocol unique identifier
#define EFI_TRANSLATION_PROTOCOL_GUID { 0x8457BBE4, 0x3B78, 0x4DB5, { 0xAB, 0xA0, 0xDC, 0xED, 0x08, 0x76, 0x9A, 0x13 } }

// _
/// A macro simplification for EfiGetText
/// @param Message The message for which to retrieve a translation
/// @return The message translation or the original message if there was no translation
#define _(Message) EfiGetText(Message)

// gEfiTranslationProtocolGuid
/// The language translation protocol unique identifier
EXTERN EFI_GUID gEfiTranslationProtocolGuid;

// EFI_TRANSLATION_PROTOCOL
/// Language translation protocol
typedef struct EFI_TRANSLATION_PROTOCOL EFI_TRANSLATION_PROTOCOL;

// EFI_GET_TRANSLATION
/// Get a translation for a language
/// @param This        The language translation protocol
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to retrieve a translation
/// @param Translation On output, the translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_NOT_FOUND         If the specified lanaguage could not be found
/// @retval EFI_NOT_FOUND         If the specified message translation was not found for the specified language
/// @retval EFI_SUCCESS           The specified message translation was returned for the specified language
typedef
EFI_STATUS
(EFIAPI
*EFI_GET_TRANSLATION) (
  IN EFI_TRANSLATION_PROTOCOL  *This,
  IN  CONST CHAR8              *Language OPTIONAL,
  IN  CONST CHAR16             *Message,
  OUT CONST CHAR16            **Translation
);
// EFI_SET_TRANSLATION
/// Set a translation for a language
/// @param This        The language translation protocol
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to set a translation
/// @param Translation The translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the message translation could not be allocated
/// @retval EFI_SUCCESS           The specified message translation was set for the specified language
typedef
EFI_STATUS
(EFIAPI
*EFI_SET_TRANSLATION) (
  IN EFI_TRANSLATION_PROTOCOL *This,
  IN CONST CHAR8              *Language OPTIONAL,
  IN CONST CHAR16             *Message,
  IN CONST CHAR16             *Translation
);
// EFI_REMOVE_TRANSLATION
/// Remove translation(s) from a language
/// @param This     The language translation protocol
/// @param Language The language code for the translation or NULL for the current language
/// @param Message  The message tranlation to remove from the specified language or NULL to remove all message translations from the specified language
/// @retval EFI_NOT_FOUND If the specifed language could not be found
/// @retval EFI_SUCCESS   The specified message translation(s) were removed for the specified language
typedef
EFI_STATUS
(EFIAPI
*EFI_REMOVE_TRANSLATION) (
  IN EFI_TRANSLATION_PROTOCOL *This,
  IN CONST CHAR8              *Language OPTIONAL,
  IN CONST CHAR16             *Message OPTIONAL
);
// EFI_REMOVE_TRANSLATIONS
/// Remove all translations for a language (or all languages)
/// @param This     The language translation protocol
/// @param Language The language code or NULL for all languages
/// @retval EFI_SUCCESS All translations were removed for the specifed language(s)
typedef
EFI_STATUS
(EFIAPI
*EFI_REMOVE_TRANSLATIONS) (
  IN EFI_TRANSLATION_PROTOCOL *This,
  IN CONST CHAR8              *Language OPTIONAL
);

// EFI_TRANSLATION_PROTOCOL
/// Language translation protocol
struct EFI_TRANSLATION_PROTOCOL {

  // GetTranslation
  /// Get a translation for a language
  EFI_GET_TRANSLATION     GetTranslation;
  // SetTranslation
  /// Set a translation for a language
  EFI_SET_TRANSLATION     SetTranslation;
  // RemoveTranslation
  /// Remove translation(s) from a language
  EFI_REMOVE_TRANSLATION  RemoveTranslation;
  // RemoveTranslations
  /// Remove all translations for a language (or all languages)
  EFI_REMOVE_TRANSLATIONS RemoveTranslations;

};

// EfiGetText
/// Get a translation for a language
/// @param Message The message for which to retrieve a translation
/// @return The message translation or the original message if there was no translation
EXTERN
CONST CHAR16 *
EFIAPI
EfiGetText (
  IN CONST CHAR16 *Message
);
// EfiGetTranslationText
/// Get a translation for a language
/// @param Language The language code for the translation or NULL for the current language
/// @param Message  The message for which to retrieve a translation
/// @return The message translation or the original message if there was no translation
EXTERN
CONST CHAR16 *
EFIAPI
EfiGetTranslationText (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message
);
// EfiGetTranslation
/// Get a translation for a language
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to retrieve a translation
/// @param Translation On output, the translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_NOT_FOUND         If the specified lanaguage could not be found
/// @retval EFI_NOT_FOUND         If the specified message translation was not found for the specified language
/// @retval EFI_SUCCESS           The specified message translation was returned for the specified language
EXTERN
EFI_STATUS
EFIAPI
EfiGetTranslation (
  IN  CONST CHAR8   *Language OPTIONAL,
  IN  CONST CHAR16  *Message,
  OUT CONST CHAR16 **Translation
);
// EfiSetTranslation
/// Set a translation for a language
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to set a translation
/// @param Translation The translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the message translation could not be allocated
/// @retval EFI_SUCCESS           The specified message translation was set for the specified language
EXTERN
EFI_STATUS
EFIAPI
EfiSetTranslation (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message,
  IN CONST CHAR16 *Translation
);
// EfiRemoveTranslation
/// Remove translation(s) from a language
/// @param Language The language code for the translation or NULL for the current language
/// @param Message  The message tranlation to remove from the specified language or NULL to remove all message translations from the specified language
/// @retval EFI_NOT_FOUND If the specifed language could not be found
/// @retval EFI_SUCCESS   The specified message translation(s) were removed for the specified language
EXTERN
EFI_STATUS
EFIAPI
EfiRemoveTranslation (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message OPTIONAL
);
// EfiRemoveTranslations
/// Remove all translations for a language (or all languages)
/// @param Language The language code or NULL for all languages
/// @retval EFI_SUCCESS All translations were removed for the specifed language(s)
EXTERN
EFI_STATUS
EFIAPI
EfiRemoveTranslations (
  IN CONST CHAR8 *Language OPTIONAL
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_TRANSLATION_HEADER__
