///
/// @file Library/Uefi/Encoding/Latin1.c
///
/// Latin-1 character encoding
///

#include "VirtualMemory.h"

// mLatin1Handle
/// The handle of the installed encoding protocol
STATIC EFI_HANDLE mLatin1Handle = NULL;

// gEfiLatin1EncodingProtocolGuid
/// The Latin-1 character string encoding protocol unique identifier
EFI_GUID gEfiLatin1EncodingProtocolGuid = EFI_LATIN1_ENCODING_PROTOCOL_GUID;

// EfiLatin1Encoding
/// Get the Latin-1 encoding protocol
/// @return The Latin-1 encoding protocol or NULL if there was an error
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiLatin1Encoding (
  VOID
) {
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  if (EFI_ERROR(EfiLocateProtocol(&gEfiLatin1EncodingProtocolGuid, NULL, (VOID **)&Encoding))) {
    return NULL;
  }
  return Encoding;
}

// Latin1DecodeCharacter
/// Decode a character from an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The encoded character string buffer to decode
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to decode the character from the output string buffer
/// @param Character On output, the decoded Unicode character
/// @retval EFI_INVALID_PARAMETER  If This, Buffer, Size, or Character is NULL
/// @retval EFI_NOT_FOUND          There were no characters remaining to decode
/// @retval EFI_SUCCESS            The character was decoded successfully
STATIC
EFI_STATUS
EFIAPI
Latin1DecodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
) {
  // Check parameters
  if ((This == NULL) || (Buffer == NULL) || (Size == NULL) || (Character == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check there is any remaining character units
  if (*Size < sizeof(CHAR8)) {
    return EFI_NOT_FOUND;
  }
  // Every Latin-1 character unit is a valid one to one code point
  *Character = (UINT32)(*((CHAR8 *)Buffer));
  *Size = sizeof(CHAR8);
  return EFI_SUCCESS;
}
// Latin1EncodeCharacter
/// Encode a character into an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The character in the encoded string buffer
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to encode the character into the output string buffer
/// @param Character The Unicode character to encode
/// @retval EFI_INVALID_PARAMETER  If This or Size is NULL
/// @retval EFI_INVALID_PARAMETER  If Character is an invalid Unicode character but not zero (null-terminator)
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted string, *Size is updated with the required size
/// @retval EFI_NO_MAPPING         The character can not be encoded with this encoding
/// @retval EFI_SUCCESS            The character was encoded successfully
STATIC
EFI_STATUS
EFIAPI
Latin1EncodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
) {
  CHAR8 *Latin1;
  // Check parameters
  if ((This == NULL) || (Size == NULL) || !IsUnicodeCharacter(Character)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check the character can be encoded
  if (Character > 0xFF) {
    return EFI_NO_MAPPING;
  }
  // Check there the size of the buffer
  if ((Buffer == NULL) || (*Size < sizeof(CHAR8))) {
    // Return the needed size of the buffer
    *Size = sizeof(CHAR8);
    return EFI_BUFFER_TOO_SMALL;
  }
  // Encode the character into the string buffer
  Latin1 = (CHAR8 *)Buffer;
  *Latin1 = (CHAR8)Character;
  *Size = sizeof(CHAR8);
  return EFI_SUCCESS;
}

// Latin1EncodingUninstall
/// Uninstall Latin-1 encoding protocol
EFI_STATUS
EFIAPI
Latin1EncodingUninstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is installed
  if ((mLatin1Handle == NULL) || EFI_ERROR(EfiLocateProtocol(&gEfiLatin1EncodingProtocolGuid, NULL, (VOID **)&Encoding)) || (Encoding == NULL)) {
    return EFI_SUCCESS;
  }
  // Uninstall and free the protocol on error
  Status = EfiUninstallMultipleProtocolInterfaces(mLatin1Handle, &gEfiLatin1EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  EfiFreePool((VOID *)Encoding);
  return Status;
}
// Latin1EncodingInstall
/// Install Latin-1 encoding protocol
EFI_STATUS
EFIAPI
Latin1EncodingInstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is already installed
  if (!EFI_ERROR(EfiLocateProtocol(&gEfiLatin1EncodingProtocolGuid, NULL, (VOID **)&Encoding)) && (Encoding != NULL)) {
    return EFI_SUCCESS;
  }
  // Create the encoding protocol
  Encoding =
#if defined(EFI_MEMORY_VIRTUAL)
  (EFI_ENCODING_PROTOCOL *)EfiInternalAllocate(sizeof(EFI_ENCODING_PROTOCOL));
#else
  EfiAllocateByType(EFI_ENCODING_PROTOCOL);
#endif
  if (Encoding == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Setup encoding protocol
  Encoding->Name = "Latin-1";
  Encoding->UnitSize = sizeof(CHAR8);
  Encoding->MaxUnits = 1;
  Encoding->DecodeCharacter = Latin1DecodeCharacter;
  Encoding->EncodeCharacter = Latin1EncodeCharacter;
  // Install the encoding protocol
  Status = EfiInstallMultipleProtocolInterfaces(&mLatin1Handle, &gEfiLatin1EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  if (EFI_ERROR(Status)) {
    // Uninstall and free the protocol on error
    EfiUninstallMultipleProtocolInterfaces(mLatin1Handle, &gEfiLatin1EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
    EfiFreePool((VOID *)Encoding);
  }
  return Status;
}
