///
/// @file Library/Uefi/Encoding/Utf16.c
///
/// UTF-16 character encoding
///

#include "VirtualMemory.h"

// mUtf16Handle
/// The handle of the installed encoding protocol
STATIC EFI_HANDLE mUtf16Handle = NULL;

// gEfiUtf16EncodingProtocolGuid
/// The UTF-16 character string encoding protocol unique identifier
EFI_GUID gEfiUtf16EncodingProtocolGuid = EFI_UTF16_ENCODING_PROTOCOL_GUID;

// EfiUtf16Encoding
/// Get the UTF-16 encoding protocol
/// @return The UTF-16 encoding protocol or NULL if there was an error
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiUtf16Encoding (
  VOID
) {
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  if (EFI_ERROR(EfiLocateProtocol(&gEfiUtf16EncodingProtocolGuid, NULL, (VOID **)&Encoding))) {
    return NULL;
  }
  return Encoding;
}

// Utf16DecodeCharacter
/// Decode a character from an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The encoded character string buffer to decode
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to decode the character from the output string buffer
/// @param Character On output, the decoded Unicode character
/// @retval EFI_INVALID_PARAMETER  If This, Buffer, Size, or Character is NULL
/// @retval EFI_NO_MAPPING         There were no valid characters decoded but some character code points were invalid and ignored
/// @retval EFI_NOT_FOUND          There were no characters remaining to decode
/// @retval EFI_WARN_UNKNOWN_GLYPH The character was converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            The character was decoded successfully
STATIC
EFI_STATUS
EFIAPI
Utf16DecodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
) {
  EFI_STATUS    Status = EFI_SUCCESS;
  CONST CHAR16 *Utf16;
  UINTN         BufferSize;
  UINT32        Unit;
  UINT32        Char = 0;
  BOOLEAN       Surrogate = FALSE;
  // Check parameters
  if ((This == NULL) || (Buffer == NULL) || (Size == NULL) || (Character == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the buffer size
  BufferSize = *Size;
  // Get the string buffer
  Utf16 = (CONST CHAR16 *)Buffer;
  // Try to decode characters
  do {
    // Check there is buffer remaining to decode
    if (BufferSize < sizeof(CHAR16)) {
      return (Status == EFI_WARN_UNKNOWN_GLYPH) ? EFI_NO_MAPPING : EFI_NOT_FOUND;
    }
    // Get the character unit
    Unit = (UINT32)*Utf16++;
    // Decrease the size by a character
    BufferSize -= sizeof(CHAR16);
    // Check character is valid
    if (Unit == 0) {
      // Null terminator code point so break from loop
      Char = Unit;
      break;
    }
    // Check if surrogate pair or character units
    if (Surrogate) {
      if ((Unit >= 0xDC00) && (Unit <= 0xDFFF)) {
        // Low/trailing surrogate
        Char <<= 10;
        Char |= EFI_BITFIELD(Unit - 0xDC00, 0, 9);
      } else if ((Char >= 0xD800) && (Char <= 0xDBFF)) {
        // High/leading surrogate but expected low/trailing surrogate so skip previous surrogate
        Status = EFI_WARN_UNKNOWN_GLYPH;
        Char = EFI_BITFIELD(Unit - 0xD800, 0, 9);
        continue;
      } else {
        // Previous high/leading surrogate character unit is being skipped
        Status = EFI_WARN_UNKNOWN_GLYPH;
        Char = Unit;
      }
      // No longer decoding a surrogate pair
      Surrogate = FALSE;
    } else if ((Char >= 0xD800) && (Char <= 0xDBFF)) {
      // High/leading surrogate
      Char = EFI_BITFIELD(Unit - 0xD800, 0, 9);
      // Need to decode next character as surrogate pair
      Surrogate = TRUE;
      continue;
    } else if ((Unit >= 0xDC00) && (Unit <= 0xDFFF)) {
      // Low/trailing surrogate but no previous high/leading surrogate so skip
      Status = EFI_WARN_UNKNOWN_GLYPH;
      continue;
    } else {
      // Single character unit code point
      Char = Unit;
    }
  // Check the decoded character is valid
  } while (!IsUnicodeCharacter(Char));
  // Return the character
  *Character = Char;
  // Return the size of the buffer and status
  *Size -= BufferSize;
  return Status;
}
// Utf16EncodeCharacter
/// Encode a character into an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The character in the encoded string buffer
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to encode the character into the output string buffer
/// @param Character The Unicode character to encode
/// @retval EFI_INVALID_PARAMETER  If This, Buffer, or Size is NULL
/// @retval EFI_INVALID_PARAMETER  If Character is an invalid Unicode character but not zero (null-terminator)
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted string, *Size is updated with the required size
/// @retval EFI_NO_MAPPING         The character can not be encoded with this encoding
/// @retval EFI_SUCCESS            The character was encoded successfully
STATIC
EFI_STATUS
EFIAPI
Utf16EncodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
) {
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR16     *Utf16;
  // Check parameters
  if ((This == NULL) || (Size == NULL) || !IsUnicodeCharacter(Character)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the string buffer
  Utf16 = (CHAR16 *)Buffer;
  // Check if the character requires surrogate pair to encode
  if (Character > 0xFFFF) {
    // Encode the character into the buffer
    if ((Utf16 != NULL) && (*Size >= (2 * sizeof(CHAR16)))) {
      // Get the surrogate pair bits to encode
      Character -= 0x10000;
      // Encode the high/leading surrogate
      *Utf16++ = (CHAR16)(0xD800 + EFI_BITFIELD(Character, 10, 19));
      // Encode the low/trailing surrogate
      *Utf16 = (CHAR16)(0xDC00 + EFI_BITFIELD(Character, 0, 9));
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set the size of two character units written
    *Size = (2 * sizeof(CHAR16));
  } else {
    // Encode the character into the buffer
    if ((Utf16 != NULL) && (*Size >= sizeof(CHAR16))) {
      *Utf16 = (CHAR16)Character;
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set the size of one character unit written
    *Size = sizeof(CHAR16);
  }
  return Status;
}

// Utf16EncodingUninstall
/// Uninstall UTF-16 encoding protocol
EFI_STATUS
EFIAPI
Utf16EncodingUninstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is installed
  if ((mUtf16Handle == NULL) || EFI_ERROR(EfiLocateProtocol(&gEfiUtf16EncodingProtocolGuid, NULL, (VOID **)&Encoding)) || (Encoding == NULL)) {
    return EFI_SUCCESS;
  }
  // Uninstall and free the protocol on error
  Status = EfiUninstallMultipleProtocolInterfaces(mUtf16Handle, &gEfiUtf16EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  EfiFreePool((VOID *)Encoding);
  return Status;
}
// Utf16EncodingInstall
/// Install UTF-16 encoding protocol
EFI_STATUS
EFIAPI
Utf16EncodingInstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is already installed
  if (!EFI_ERROR(EfiLocateProtocol(&gEfiUtf16EncodingProtocolGuid, NULL, (VOID **)&Encoding)) && (Encoding != NULL)) {
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
  Encoding->Name = "UTF-16";
  Encoding->UnitSize = sizeof(CHAR16);
  Encoding->MaxUnits = 2;
  Encoding->DecodeCharacter = Utf16DecodeCharacter;
  Encoding->EncodeCharacter = Utf16EncodeCharacter;
  // Install the encoding protocol
  Status = EfiInstallMultipleProtocolInterfaces(&mUtf16Handle, &gEfiUtf16EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  if (EFI_ERROR(Status)) {
    // Uninstall and free the protocol on error
    EfiUninstallMultipleProtocolInterfaces(mUtf16Handle, &gEfiUtf16EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
    EfiFreePool((VOID *)Encoding);
  }
  return Status;
}
