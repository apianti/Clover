///
/// @file Library/Uefi/Encoding/Utf8.c
///
/// UTF-8 character encoding
///

#include "VirtualMemory.h"

// mUtf8Handle
/// The handle of the installed encoding protocol
STATIC EFI_HANDLE mUtf8Handle = NULL;

// gEfiUtf8EncodingProtocolGuid
/// The UTF-8 character string encoding protocol unique identifier
EFI_GUID gEfiUtf8EncodingProtocolGuid = EFI_UTF8_ENCODING_PROTOCOL_GUID;

// EfiUtf8Encoding
/// Get the UTF-8 encoding protocol
/// @return The UTF-8 encoding protocol or NULL if there was an error
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiUtf8Encoding (
  VOID
) {
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  if (EFI_ERROR(EfiLocateProtocol(&gEfiUtf8EncodingProtocolGuid, NULL, (VOID **)&Encoding))) {
    return NULL;
  }
  return Encoding;
}

// Utf8DecodeCharacter
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
Utf8DecodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
) {
  EFI_STATUS   Status = EFI_SUCCESS;
  CONST CHAR8 *Utf8;
  UINTN        BufferSize;
  UINT32       Unit;
  UINT32       Char = 0;
  UINT8        Trailers = 0;
  // Check parameters
  if ((This == NULL) || (Buffer == NULL) || (Size == NULL) || (Character == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the buffer size
  BufferSize = *Size;
  // Get the string buffer
  Utf8 = (CONST CHAR8 *)Buffer;
  // Try to decode characters
  do {
    // Check there is buffer remaining to decode
    if (BufferSize < sizeof(CHAR8)) {
      return (Status == EFI_WARN_UNKNOWN_GLYPH) ? EFI_NO_MAPPING : EFI_NOT_FOUND;
    }
    // Get the character unit
    Unit = (UINT32)*Utf8++;
    // Decrease the size by a character
    BufferSize -= sizeof(CHAR8);
    // Check character is valid
    if (Unit == 0) {
      // Null terminator code point so break from loop
      Char = Unit;
      break;
    }
    // Check if single, leading, or trailing character unit
    if (EFI_BITS_ARE_SET(Unit, 0xC0)) {
      // Previous character units may be skipped here because of invalid encoding
      if (Trailers != 0) {
        Status = EFI_WARN_UNKNOWN_GLYPH;
      }
      // Leading character unit
      if (EFI_BIT_IS_UNSET(Unit, 6)) {
        // Two character sequence
        Trailers = 1;
        Char = (UINT32)EFI_BITFIELD(Unit, 0, 5);
      } else if (EFI_BIT_IS_UNSET(Unit, 5)) {
        // Three character sequence
        Trailers = 2;
        Char = (UINT32)EFI_BITFIELD(Unit, 0, 4);
      } else if (EFI_BIT_IS_UNSET(Unit, 4)) {
        // Four character sequence
        Trailers = 3;
        Char = (UINT32)EFI_BITFIELD(Unit, 0, 3);
      } else {
        // This character unit is skipped because of invalid encoding
        Status = EFI_WARN_UNKNOWN_GLYPH;
        Trailers = 0;
      }
      // Continue decoding character
      continue;
    } else if (EFI_BIT_IS_SET(Unit, 7)) {
      // Trailing character unit
      if (Trailers != 0) {
        // Decode the trailing character unit
        Char <<= 6;
        Char |= (UINT32)EFI_BITFIELD(Unit, 0, 6);
        // Keep decoding if more trailing character units are expected
        if (--Trailers != 0) {
          continue;
        }
      } else {
        // Trailing character unit but no leading character unit preceded so skip
        Status = EFI_WARN_UNKNOWN_GLYPH;
        continue;
      }
    } else {
      // Previous character units may be skipped here because of invalid encoding
      if (Trailers != 0) {
        Status = EFI_WARN_UNKNOWN_GLYPH;
        Trailers = 0;
      }
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
// Utf8EncodeCharacter
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
Utf8EncodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
) {
  EFI_STATUS  Status = EFI_NO_MAPPING;
  CHAR8      *Utf8;
  // Check parameters
  if ((This == NULL) || (Size == NULL) || !IsUnicodeCharacter(Character)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the string buffer
  Utf8 = (CHAR8 *)Buffer;
  // Determine the character units for this character code point
  if (Character <= 0x7F) {
    // One character unit code point
    if ((Buffer != NULL) && (*Size >= sizeof(CHAR8))) {
      *Utf8++ = (CHAR8)Character;
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set size of the encoded character
    *Size = sizeof(CHAR8);
  } else if (Character <= 0x7FF) {
    // Two character unit code point
    if ((Buffer != NULL) && (*Size >= (2 * sizeof(CHAR8)))) {
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 6, 10) | 0xC0);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 0, 5) | 0x80);
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set size of the encoded character
    *Size = (2 * sizeof(CHAR8));
  } else if (Character <= 0xFFFF) {
    // Three character unit code point
    if ((Buffer != NULL) && (*Size >= (3 * sizeof(CHAR8)))) {
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 12, 15) | 0xE0);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 6, 11) | 0x80);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 0, 5) | 0x80);
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set size of the encoded character
    *Size = (3 * sizeof(CHAR8));
  } else if (Character <= 0x10FFFF) {
    // Four character unit code point
    if ((Buffer != NULL) && (*Size >= (4 * sizeof(CHAR8)))) {
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 18, 20) | 0xF0);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 12, 17) | 0x80);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 6, 11) | 0x80);
      *Utf8++ = (CHAR8)(EFI_BITFIELD(Character, 0, 5) | 0x80);
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    // Set size of the encoded character
    *Size = (4 * sizeof(CHAR8));
  }
  return Status;
}

// Utf8EncodingUninstall
/// Uninstall UTF-8 encoding protocol
EFI_STATUS
EFIAPI
Utf8EncodingUninstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is installed
  if ((mUtf8Handle == NULL) || EFI_ERROR(EfiLocateProtocol(&gEfiUtf8EncodingProtocolGuid, NULL, (VOID **)&Encoding)) || (Encoding == NULL)) {
    return EFI_SUCCESS;
  }
  // Uninstall and free the protocol on error
  Status = EfiUninstallMultipleProtocolInterfaces(mUtf8Handle, &gEfiUtf8EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  EfiFreePool((VOID *)Encoding);
  return Status;
}
// Utf8EncodingInstall
/// Uninstall UTF-8 encoding protocol
EFI_STATUS
EFIAPI
Utf8EncodingInstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *Encoding = NULL;
  // Check if encoding protocol is already installed
  if (!EFI_ERROR(EfiLocateProtocol(&gEfiUtf8EncodingProtocolGuid, NULL, (VOID **)&Encoding)) && (Encoding != NULL)) {
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
  Encoding->Name = "UTF-8";
  Encoding->UnitSize = sizeof(CHAR8);
  Encoding->MaxUnits = 4;
  Encoding->DecodeCharacter = Utf8DecodeCharacter;
  Encoding->EncodeCharacter = Utf8EncodeCharacter;
  // Install the encoding protocol
  Status = EfiInstallMultipleProtocolInterfaces(&mUtf8Handle, &gEfiUtf8EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
  if (EFI_ERROR(Status)) {
    // Uninstall and free the protocol on error
    EfiUninstallMultipleProtocolInterfaces(mUtf8Handle, &gEfiUtf8EncodingProtocolGuid, Encoding, &gEfiEncodingProtocolGuid, Encoding, NULL);
    EfiFreePool((VOID *)Encoding);
  }
  return Status;
}
