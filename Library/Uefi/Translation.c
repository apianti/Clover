///
/// @file Library/Uefi/Translation.c
///
/// TODO: Language translations
///

#include <Uefi.h>

// gEfiTranslationProtocolGuid
/// The language translation protocol unique identifier
EFI_GUID gEfiTranslationProtocolGuid = EFI_TRANSLATION_PROTOCOL_GUID;

// GetTranslationProtocol
/// Get the current translation protocol
STATIC
EFI_TRANSLATION_PROTOCOL *
EFIAPI
GetTranslationProtocol (
  VOID
) {
  EFI_TRANSLATION_PROTOCOL *Translation = NULL;
  // Check if there is already a translation protocol
  if (EFI_ERROR(EfiLocateProtocol(&gEfiTranslationProtocolGuid, NULL, (VOID **)&Translation))) {
    // Translation protocol not found
    return NULL;
  }
  // Return the translation protocol interface
  return Translation;
}

// EfiGetText
/// Get a translation for a language
/// @param Message The message for which to retrieve a translation
/// @return The message translation or the original message if there was no translation
CONST CHAR16 *
EFIAPI
EfiGetText (
  IN CONST CHAR16 *Message
) {
  return EfiGetTranslationText(NULL, Message);
}
// EfiGetTranslationText
/// Get a translation for a language
/// @param Language The language code for the translation or NULL for the current language
/// @param Message  The message for which to retrieve a translation
/// @return The message translation or the original message if there was no translation
CONST CHAR16 *
EFIAPI
EfiGetTranslationText (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message
) {
  CONST CHAR16 *Translation = Message;
  // Get the message translation
  if (EFI_ERROR(EfiGetTranslation(Language, Message, &Translation)) || (Translation == NULL)) {
    // Return the original message
    Translation = Message;
  }
  // Return the message translation
  return (Translation != NULL) ? Translation : L"";
}
// EfiGetTranslation
/// Get a translation for a language
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to retrieve a translation
/// @param Translation On output, the translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_NOT_FOUND         If the specified lanaguage could not be found
/// @retval EFI_NOT_FOUND         If the specified message translation was not found for the specified language
/// @retval EFI_SUCCESS           The specified message translation was returned for the specified language
EFI_STATUS
EFIAPI
EfiGetTranslation (
  IN  CONST CHAR8   *Language OPTIONAL,
  IN  CONST CHAR16  *Message,
  OUT CONST CHAR16 **Translation
) {
  // Get the translation protocol
  EFI_TRANSLATION_PROTOCOL *This = GetTranslationProtocol();
  if ((This == NULL) || (This->GetTranslation == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call the method
  return This->GetTranslation(This, Language, Message, Translation);
}
// EfiSetTranslation
/// Set a translation for a language
/// @param Language    The language code for the translation or NULL for the current language
/// @param Message     The message for which to set a translation
/// @param Translation The translation of the message in the specified language
/// @retval EFI_INVALID_PARAMETER If Message or Translation is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the message translation could not be allocated
/// @retval EFI_SUCCESS           The specified message translation was set for the specified language
EFI_STATUS
EFIAPI
EfiSetTranslation (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message,
  IN CONST CHAR16 *Translation
) {
  // Get the translation protocol
  EFI_TRANSLATION_PROTOCOL *This = GetTranslationProtocol();
  if ((This == NULL) || (This->SetTranslation == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call the method
  return This->SetTranslation(This, Language, Message, Translation);
}
// EfiRemoveTranslation
/// Remove translation(s) from a language
/// @param Language The language code for the translation or NULL for the current language
/// @param Message  The message tranlation to remove from the specified language or NULL to remove all message translations from the specified language
/// @retval EFI_NOT_FOUND If the specifed language could not be found
/// @retval EFI_SUCCESS   The specified message translation(s) were removed for the specified language
EFI_STATUS
EFIAPI
EfiRemoveTranslation (
  IN CONST CHAR8  *Language OPTIONAL,
  IN CONST CHAR16 *Message OPTIONAL
) {
  // Get the translation protocol
  EFI_TRANSLATION_PROTOCOL *This = GetTranslationProtocol();
  if ((This == NULL) || (This->RemoveTranslation == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call the method
  return This->RemoveTranslation(This, Language, Message);
}
// EfiRemoveTranslations
/// Remove all translations for a language (or all languages)
/// @param Language The language code or NULL for all languages
/// @retval EFI_SUCCESS All translations were removed for the specifed language(s)
EFI_STATUS
EFIAPI
EfiRemoveTranslations (
  IN CONST CHAR8 *Language OPTIONAL
) {
  // Get the translation protocol
  EFI_TRANSLATION_PROTOCOL *This = GetTranslationProtocol();
  if ((This == NULL) || (This->RemoveTranslations == NULL)) {
    return EFI_UNSUPPORTED;
  }
  // Call the method
  return This->RemoveTranslations(This, Language);
}
