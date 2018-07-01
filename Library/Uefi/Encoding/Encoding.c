///
/// @file Library/Uefi/Encoding/Encoding.c
///
/// Character encoding
///

#include <Uefi.h>

// ENCODING_STRING_DEFAULT_SIZE
/// The default or increment size of a string in characters
#define ENCODING_STRING_DEFAULT_SIZE 16

// gEfiEncodingProtocolGuid
/// The character string encoding protocol unique identifier
EFI_GUID gEfiEncodingProtocolGuid = EFI_ENCODING_PROTOCOL_GUID;

// Latin1EncodingInstall
/// Install Latin-1 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Latin1EncodingInstall (
  VOID
);
// Latin1EncodingUninstall
/// Uninstall Latin-1 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Latin1EncodingUninstall (
  VOID
);

// Utf8EncodingInstall
/// Uninstall UTF-8 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf8EncodingInstall (
  VOID
);
// Utf8EncodingUninstall
/// Uninstall UTF-8 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf8EncodingUninstall (
  VOID
);

// Utf16EncodingInstall
/// Install UTF-16 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf16EncodingInstall (
  VOID
);
// Utf16EncodingUninstall
/// Uninstall UTF-16 encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf16EncodingUninstall (
  VOID
);

// Utf16SwappedEncodingInstall
/// Install UTF-16 byte swapped encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf16SwappedEncodingInstall (
  VOID
);
// Utf16SwappedEncodingUninstall
/// Uninstall UTF-16 byte swapped encoding protocol
EXTERN
EFI_STATUS
EFIAPI
Utf16SwappedEncodingUninstall (
  VOID
);

// EfiDefaultEncoding
/// Get the default encoding protocol
/// @return The default encoding protocol or NULL if there was an error
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiDefaultEncoding (
  VOID
) {
  return EfiUtf8Encoding();
}

// EfiEncodingUninstall
/// Uninstall encoding protocols
/// @retval EFI_SUCCESS The encoding protocols were uninstalled
EFI_STATUS
EFIAPI
EfiEncodingUninstall (
  VOID
) {
  // Uninstall Latin-1 encoding
  Latin1EncodingUninstall();
  // Uninstall UTF-8 encoding
  Utf8EncodingUninstall();
  // Uninstall UTF-16 encoding
  Utf16EncodingUninstall();
  // Uninstall UTF-16 byte swapped encoding
  Utf16SwappedEncodingUninstall();
  return EFI_SUCCESS;
}
// EfiEncodingInstall
/// Install encoding protocols
/// @retval EFI_SUCCESS The encoding protocols were installed
EFI_STATUS
EFIAPI
EfiEncodingInstall (
  VOID
) {
  EFI_STATUS Status;
  // Install Latin-1 encoding
  Status = Latin1EncodingInstall();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Install UTF-8 encoding
  Status = Utf8EncodingInstall();
  if (EFI_ERROR(Status)) {
    EfiEncodingUninstall();
    return Status;
  }
  // Install UTF-16 encoding
  Status = Utf16EncodingInstall();
  if (EFI_ERROR(Status)) {
    EfiEncodingUninstall();
    return Status;
  }
  // Install UTF-16 byte swapped encoding
  Status = Utf16SwappedEncodingInstall();
  if (EFI_ERROR(Status)) {
    EfiEncodingUninstall();
  }
  return Status;
}

// EfiGetEncoding
/// Get an encoding protocol interface
/// @param Name The name of the encoding or NULL for the current encoding
/// @return The encoding protocol interface or NULL if not found
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiGetEncoding (
  IN CONST CHAR8 *Name OPTIONAL
) {
  UINTN       Count = 0;
  EFI_HANDLE *Handles = NULL;
  CHAR8      *Default = NULL;
  // Check if requesting default encoding
  if ((Name == NULL) || (*Name == 0)) {
    Name = NULL;
    // Get default encoding
    if (!EFI_ERROR(EfiGetGlobalVariableBuffer(EFI_ENCODING_VARIABLE_NAME, NULL, NULL, &Default))) {
      Name = Default;
    }
  }
  // If unknown encoding set default encoding
  if ((Name == NULL) || (*Name == 0)) {
    Name = EFI_ENCODING_DEFAULT_NAME;
  }
  // If ASCII encoding set Latin-1 encoding
  if (AsciiStriCmp(Name, "ASCII") == 0) {
    Name = "Latin-1";
  }
  // Get the handles that have encoding protocol installed
  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiEncodingProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
    // Iterate through each handle to get the encoding protocols
    UINTN Index = 0;
    while (Index < Count) {
      // Check the handle is valid
      if (Handles[Index] != NULL) {
        // Get the encoding protocol from the handle
        EFI_ENCODING_PROTOCOL *Encoding = NULL;
        if (!EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiEncodingProtocolGuid, (VOID **)&Encoding))) {
          // Check the encoding and encoding name are valid, then compare encoding names for match
          if ((Encoding != NULL) && (Encoding->Name != NULL) && (AsciiStriCmp(Name, Encoding->Name) == 0)) {
            // Free the handles array
            EfiFreePool(Handles);
            // Free the default encoding name if needed
            if (Default != NULL) {
              EfiFreePool((VOID *)Default);
            }
            // Return the found encoding
            return Encoding;
          }
        }
      }
      // Advance the index to the next encoding handle
      ++Index;
    }
    // Free the handles array
    EfiFreePool(Handles);
  }
  // Free the default encoding name if needed
  if (Default != NULL) {
    EfiFreePool((VOID *)Default);
  }
  // Encoding protocol not found
  return NULL;
}
// EfiSetEncoding
/// Set the current encoding protocol interface
/// @param Name The name of the encoding or NULL to restore the default encoding
/// @retval EFI_NOT_FOUND The encoding was not found
/// @retval EFI_SUCCESS   The default encoding was set
EFI_STATUS
EFIAPI
EfiSetEncoding (
  IN CONST CHAR8 *Name OPTIONAL
) {
  EFI_ENCODING_PROTOCOL *Encoding;
  // Delete current default encoding
  EfiDeleteGlobalVariable(EFI_ENCODING_VARIABLE_NAME);
  // If unknown encoding set default encoding
  if ((Name == NULL) || (*Name == 0)) {
    Name = EFI_ENCODING_DEFAULT_NAME;
  }
  // If ASCII encoding set Latin-1 encoding
  if (AsciiStriCmp(Name, "ASCII") == 0) {
    Name = "Latin-1";
  }
  // Get the encoding by name
  Encoding = EfiGetEncoding(Name);
  if (Encoding == NULL) {
    return EFI_NOT_FOUND;
  }
  // Set the default encoding
  return EfiSetGlobalVariable(EFI_ENCODING_VARIABLE_NAME, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS, AsciiStrSize(Name), (VOID *)Name);
}

// EfiDecodeCharacter
/// Decode a character from an encoded character string
/// @param Encoding  The encoding protocol interface
/// @param Buffer    The encoded character string buffer
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to decode the character from the output string buffer
/// @param Character On output, the decoded Unicode character
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, Size, or Character is NULL
/// @retval EFI_NO_MAPPING         There were no valid characters decoded but some character code points were invalid and ignored
/// @retval EFI_NOT_FOUND          There were no characters remaining to decode
/// @retval EFI_WARN_UNKNOWN_GLYPH The string was converted but some character code points were invalid and ignored
/// @retval EFI_SUCCESS            The character was decoded successfully
EFI_STATUS
EFIAPI
EfiDecodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
) {
  EFI_STATUS Status;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (Size == NULL) || (Character == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Encoding->DecodeCharacter == NULL) {
    return EFI_UNSUPPORTED;
  }
  Status = Encoding->DecodeCharacter(Encoding, Buffer, Size, Character);
  if (!EFI_ERROR(Status) && (Character == 0)) {
    return EFI_NOT_FOUND;
  }
  return Status;
}
// EfiEncodeCharacter
/// Encode a character into an encoded character string
/// @param Encoding  The encoding protocol interface
/// @param Buffer    The encoded string buffer in which to encode the character
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to encode the character into the output string buffer
/// @param Character The Unicode character to encode
/// @retval EFI_INVALID_PARAMETER  If Encoding or Size is NULL
/// @retval EFI_INVALID_PARAMETER  If Character is an invalid Unicode character but not zero (null-terminator)
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted string, *Size is updated with the required size
/// @retval EFI_NO_MAPPING         The character can not be encoded with this encoding
/// @retval EFI_SUCCESS            The character was encoded successfully
EFI_STATUS
EFIAPI
EfiEncodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
) {
  // Check parameters
  if ((Encoding == NULL) || (Size == NULL) || !IsUnicodeCharacter(Character)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Encoding->EncodeCharacter == NULL) {
    return EFI_UNSUPPORTED;
  }
  return Encoding->EncodeCharacter(Encoding, Buffer, Size, Character);
}
// EfiAppendCharacter
/// Append a character to an encoded character string
/// @param Encoding  The encoding protocol interface
/// @param Buffer    On input, the string to which to append or NULL to allocate a new string, on output, the possibly reallocated string, which needs freed
/// @param Count     On input, the length count, in characters, of the string, on output, the length count, in characters, of the string
/// @param Size      On input, the allocated size, in characters, of the string, on output, the allocated size, in characters, of the string
/// @param Character The character to append to the string
/// @return Whether the character was appended to the string or not
/// @retval EFI_INVALID_PARAMETER If Buffer, Count, or Size is NULL or Character is invalid or *Count >= *Size
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string
/// @retval EFI_SUCCESS           If the character was appended to the string successfully
EFI_STATUS
EFIAPI
EfiAppendCharacter (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Count,
  IN OUT UINTN                  *Size,
  IN     UINT32                  Character
) {
  EFI_STATUS  Status;
  VOID       *Ptr;
  UINTN       BufferSize;
  UINTN       ThisSize;
  UINTN       ThisCount;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (Count == NULL) ||
      (Size == NULL) || !IsUnicodeCharacter(Character)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the string information
  Ptr = *Buffer;
  ThisCount = *Count;
  BufferSize = *Size;
  // Check if there is space in the buffer for the character
  if ((Ptr == NULL) || (BufferSize == 0) || (BufferSize <= (ThisCount + (2 * Encoding->MaxUnits)))) {
    // The buffer needs (re)allocated
    UINTN NewSize = BufferSize;
    do {
      NewSize += ENCODING_STRING_DEFAULT_SIZE;
    } while (NewSize <= (ThisCount + (2 * Encoding->MaxUnits)));
    // Allocate or reallocate the buffer
    if (Ptr != NULL) {
      Ptr = (CHAR16 *)EfiReallocate(NewSize * Encoding->UnitSize, BufferSize * Encoding->UnitSize, Ptr);
    } else {
      Ptr = (CHAR16 *)EfiAllocate(NewSize * Encoding->UnitSize);
    }
    // Set the new buffer
    *Buffer = Ptr;
    if (Ptr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Set the new buffer size
    BufferSize = NewSize;
    *Size = BufferSize;
  }
  // Encode the character
  Ptr = ADDRESS_OFFSET(CHAR16, Ptr, ThisCount * Encoding->UnitSize);
  if (Character != 0) {
    ThisSize = ((BufferSize - ThisCount) * Encoding->UnitSize);
    Status = EfiEncodeCharacter(Encoding, Ptr, &ThisSize, Character);
    if (EFI_ERROR(Status)) {
      // Something went wrong
      return Status;
    }
    // Advance the string pointer
    Ptr = ADDRESS_OFFSET(VOID, Ptr, ThisSize);
    // Increase the character count
    ThisCount += (ThisSize / Encoding->UnitSize);
    *Count = ThisCount;
  }
  // Append null terminator
  ThisSize = ((BufferSize - ThisCount) * Encoding->UnitSize);
  return EfiEncodeCharacter(Encoding, Ptr, &ThisSize, 0);
}
// EfiFillCharacter
/// Fill an encoded character string with a character
/// @param Encoding  The encoding protocol interface
/// @param Buffer    The encoded string buffer in which to fill with a character
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to encode the characters into the output string buffer
/// @param Count     The count of characters to encode into theoutput string buffer
/// @param Character The Unicode character to use to fill the encoded character string
/// @retval EFI_INVALID_PARAMETER  If Encoding or Size is NULL
/// @retval EFI_INVALID_PARAMETER  If Character is an invalid Unicode character but not zero (null-terminator)
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted string, *Size is updated with the required size
/// @retval EFI_NO_MAPPING         The character can not be encoded with this encoding
/// @retval EFI_SUCCESS            The character was encoded successfully
EFI_STATUS
EFIAPI
EfiFillCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINTN                  Count,
  IN     UINT32                 Character
) {
  EFI_STATUS Status;
  UINTN      BufferSize = 0;
  UINTN      RemainingSize;
  UINTN      ThisSize;
  // Check parameters
  if ((Encoding == NULL) || (Size == NULL) || (Count == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the remaining size
  RemainingSize = *Size;
  // Fill the output buffer with count of pad characters
  do {
    // Encode the padding character into the output buffer
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, Character);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Decrease remaining size
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
    // Advance the buffer
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
  // Check there are more characters to fill
  } while (--Count != 0);
  // Return size of output buffer
  *Size = BufferSize;
  return Status;
}

// EfiEncodingCompare
/// Compare two encoded character strings
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first

EXTERN
INTN
EFIAPI
EfiEncodingCompare (
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  return EfiLocaleCompare(NULL, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2);
}
// EfiEncodingNCompare
/// Compare two encoded character strings with a maximum count
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @param Count       The maximum count  of characters to compare
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first
INTN
EFIAPI
EfiEncodingNCompare (
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
    return EfiLocaleNCompare(NULL, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2, Count);
}
// EfiEncodingICompare
/// Compare two encoded character strings case insensitively
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first
INTN
EFIAPI
EfiEncodingICompare (
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  return EfiLocaleICompare(NULL, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2);
}
// EfiEncodingNICompare
/// Compare two encoded character strings case insensitively with a maximum count
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @param Count       The maximum count  of characters to compare
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first
INTN
EFIAPI
EfiEncodingNICompare (
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
  return EfiLocaleNICompare(NULL, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2, Count);
}

// EfiEncodingStringLength
/// Get the count of character code points from an encoded character string
/// @param Encoding The encoding protocol interface
/// @param Buffer   The encoded character string buffer
/// @param Size     On input, the size in bytes of the string buffer, on output, the count in characters of the string buffer
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, or Size is NULL
/// @retval EFI_WARN_UNKNOWN_GLYPH The characters were converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            All characters were decoded successfully and the count of characters was returned in *Size
EFI_STATUS
EFIAPI
EfiEncodingStringLength (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size
) {
  EFI_STATUS Result = EFI_SUCCESS;
  UINTN      Count = 0;
  UINTN      BufferSize;
  UINT32     Character;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the buffer size
  BufferSize = *Size;
  if (BufferSize != 0) {
    // Count the characters with (assumed) known size
    do {
      EFI_STATUS Status;
      UINTN      ThisSize = BufferSize;
      // Decode each character
      Character = 0;
      Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, &Character);
      // Increase character count (do this before breaking the loop because the null-terminator is subtracted below)
      ++Count;
      // Check the status of decoding the character
      if (EFI_ERROR(Status)) {
        // Return character count assuming the error is because end of string even though that does not
        //  matter because otherwise the string appears malformed and truncating is probably best
        break;
      }
      // Change result status to warn any character units were skipped if needed
      if (Result == EFI_SUCCESS) {
        Result = Status;
      }
      // Advance the buffer
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      // Decrease the remaining buffer size
      BufferSize -= ThisSize;
    // Done counting if reached null-terminator
    } while (Character != 0);
  } else {
    // Count the characters with unknown size so get the max units size in bytes (two max
    //  character units characters size in bytes should be enough to find one valid character)
    UINTN MaxUnitsSize = 2 * Encoding->UnitSize * Encoding->MaxUnits;
    // Get the count of bytes for each of the characters
    do {
      EFI_STATUS Status;
      UINTN      ThisSize = MaxUnitsSize;
      // Decode each character
      Character = 0;
      Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, &Character);
      // Increase character count (do this before breaking the loop because the null-terminator is subtracted below)
      ++Count;
      // Check the status of decoding the character
      if (EFI_ERROR(Status)) {
        // Return string size assuming the error is because end of string even though that does not
        //  matter because otherwise the string appears malformed and truncating is probably best
        break;
      }
      // Change result status to warn any character units were skipped if needed
      if (Result == EFI_SUCCESS) {
        Result = Status;
      }
      // Advance the buffer
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      // Done counting if reached null-terminator
    } while (Character != 0);
  }
  // Return character count
  *Size = Count - 1;
  return Result;
}
// EfiEncodingStringSize
/// Get the size of a null-terminated encoded character string
/// @param Encoding The encoding protocol interface
/// @param Buffer   The encoded character string buffer
/// @param Size     On output, the size of the encoded character string buffer
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, or Size is NULL
/// @retval EFI_WARN_UNKNOWN_GLYPH The characters were converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            All characters were decoded successfully and the size in bytes was returned in *Size
EFI_STATUS
EFIAPI
EfiEncodingStringSize (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  IN  CONST VOID            *Buffer,
  OUT UINTN                 *Size
) {
  EFI_STATUS Result = EFI_SUCCESS;
  UINTN      BufferSize = 0;
  UINTN      MaxUnitsSize;
  UINT32     Character;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the max units size in bytes (two max character units characters size in bytes should be enough to find one valid character)
  MaxUnitsSize = 2 * Encoding->UnitSize * Encoding->MaxUnits;
  // Get the count of bytes for each of the characters
  do {
    EFI_STATUS Status;
    UINTN      ThisSize = MaxUnitsSize;
    // Decode each character
    Character = 0;
    Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, &Character);
    // Check the status of decoding the character
    if (EFI_ERROR(Status)) {
      // Return string size assuming the error is because end of string even though that does not
      //  matter because otherwise the string appears malformed and truncating is probably best
      break;
    }
    // Change result status to warn any character units were skipped if needed
    if (Result == EFI_SUCCESS) {
      Result = Status;
    }
    // Advance the buffer
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    // Increase buffer size
    BufferSize += ThisSize;
  // Done counting if reached null-terminator
  } while (Character != 0);
  // Return buffer size in bytes
  *Size = BufferSize;
  return Result;
}

// EfiEncodingConvert
/// Convert an encoded string to another encoding
/// @param Encoding       The output encoding or NULL for the current default encoding
/// @param Buffer         On output, the output encoded string buffer
/// @param Size           On input, the size in bytes of the output encoded string buffer, on output, the size in bytes 
/// @param InputEncoding  The input encoding or NULL for the current default encoding
/// @param Input          The input encoded string buffer to decode
/// @param InputSize      The size in bytes of the input encoded string buffer
/// @retval EFI_INVALID_PARAMETER If Size or Input is NULL
/// @retval EFI_INVALID_PARAMETER If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL  The specified output buffer size is not large enough to hold the converted string, *OutputSize is updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingConvert (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN     VOID                  *Input,
  IN     UINTN                  InputSize
) {
  EFI_STATUS Status = EFI_NOT_FOUND;
  UINTN      RemainingSize;
  UINTN      BufferSize = 0;
  UINTN      ThisSize;
  UINT32     Character;
  // Check parameters
  if ((Input == NULL) || (InputSize == 0) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get default encodings if needed
  if (InputEncoding == NULL) {
    InputEncoding = EfiGetEncoding(NULL);
  } else if (Encoding == NULL) {
    Encoding = EfiGetEncoding(NULL);
  }
  // Check encodings are valid
  if ((InputEncoding == NULL) || (Encoding == NULL)) {
    return EFI_NOT_FOUND;
  }
  // Get the total size
  RemainingSize = *Size;
  // Convert the character buffers
  while (InputSize >= InputEncoding->UnitSize) {
    ThisSize = InputSize;
    Character = 0;
    // Decode the next input buffer character
    Status = EfiDecodeCharacter(InputEncoding, Input, &ThisSize, &Character);
    if (EFI_ERROR(Status)) {
      break;
    }
    // Decrease input size
    InputSize -= ThisSize;
    // Advance the input
    Input = ADDRESS_OFFSET(VOID, Input, ThisSize);
    // Check character is valid
    if ((Character != 0) && !IsUnicodeCharacter(Character)) {
      // Skip this character as it is invalid
      continue;
    }
    // Encode the character into the output buffer
    if (RemainingSize >= Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, Character);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      break;
    }
    // Increase Buffer size
    BufferSize += ThisSize;
    // Advance the Buffer if needed
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    // Break if character is null terminator
    if (Character == 0) {
      break;
    }
  }
  // Check if input ended before null terminator
  if (Status == EFI_NOT_FOUND) {
    // Encode the null terminator into the output buffer
    if (RemainingSize >= Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Increase buffer size
    BufferSize += ThisSize;
  }
  // Set Buffer size if needed
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    *Size = BufferSize;
  }
  return Status;
}
// EfiEncodingPoolConvert
/// Convert an encoded string to another encoding
/// @param Encoding       The output encoding or NULL for the current default encoding
/// @param Size           On output, the size in bytes 
/// @param InputEncoding  The input encoding or NULL for the current default encoding
/// @param Input          The input encoded string buffer to decode
/// @param InputSize      The size in bytes of the input encoded string buffer
/// @return The converted encoded string or NULL if there was an error
VOID *
EFIAPI
EfiEncodingPoolConvert (
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN  VOID                  *Input,
  IN  UINTN                  InputSize
) {
  VOID  *Buffer;
  UINTN  BufferSize = 0;
  // Get the buffer size
  if ((EfiEncodingConvert(Encoding, NULL, &BufferSize, InputEncoding, Input, InputSize) != EFI_BUFFER_TOO_SMALL) || (BufferSize == 0)) {
    return NULL;
  }
  // Allocate the buffer
  Buffer = EfiAllocate(BufferSize);
  if (Buffer == NULL) {
    return NULL;
  }
  // Convert the string
  if (EFI_ERROR(EfiEncodingConvert(Encoding, Buffer, &BufferSize, InputEncoding, Input, InputSize))) {
    EfiFreePool(Buffer);
    return NULL;
  }
  // Return the buffer
  if (Size != NULL) {
    *Size = BufferSize;
  }
  return Buffer;
}
// EfiEncodingConvertCount
/// Convert an encoded string to another encoding
/// @param Encoding       The output encoding or NULL for the current default encoding
/// @param Buffer         On output, the output encoded string buffer
/// @param Size           On input, the size in bytes of the output encoded string buffer, on output, the size in bytes 
/// @param InputEncoding  The input encoding or NULL for the current default encoding
/// @param Input          The input encoded string buffer to decode
/// @param InputSize      The size in bytes of the input encoded string buffer
/// @param Count          The maximum count of characters to convert
/// @retval EFI_INVALID_PARAMETER If Size or Input is NULL
/// @retval EFI_INVALID_PARAMETER If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL  The specified output buffer size is not large enough to hold the converted string, *OutputSize is updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingConvertCount (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN     VOID                  *Input,
  IN     UINTN                  InputSize,
  IN     UINTN                  Count
) {
  EFI_STATUS Status = EFI_NOT_FOUND;
  UINTN      RemainingSize;
  UINTN      BufferSize = 0;
  UINTN      ThisSize;
  UINT32     Character;
  // Check parameters
  if ((Input == NULL) || (InputSize == 0) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get default encodings if needed
  if (InputEncoding == NULL) {
    InputEncoding = EfiGetEncoding(NULL);
  } else if (Encoding == NULL) {
    Encoding = EfiGetEncoding(NULL);
  }
  // Check encodings are valid
  if ((InputEncoding == NULL) || (Encoding == NULL) || (Count == 0)) {
    return EFI_NOT_FOUND;
  }
  // Get the total size
  RemainingSize = *Size;
  // Convert the character buffers
  while ((Count-- != 0) && (InputSize >= InputEncoding->UnitSize)) {
    ThisSize = InputSize;
    Character = 0;
    // Decode the next input buffer character
    Status = EfiDecodeCharacter(InputEncoding, Input, &ThisSize, &Character);
    if (EFI_ERROR(Status)) {
      break;
    }
    // Decrease input size
    InputSize -= ThisSize;
    // Advance the input
    Input = ADDRESS_OFFSET(VOID, Input, ThisSize);
    // Check character is valid
    if ((Character != 0) && !IsUnicodeCharacter(Character)) {
      // Skip this character as it is invalid
      continue;
    }
    // Encode the character into the output buffer
    if (RemainingSize >= Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, Character);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      break;
    }
    // Increase Buffer size
    BufferSize += ThisSize;
    // Advance the Buffer if needed
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    // Break if character is null terminator
    if (Character == 0) {
      break;
    }
  }
  // Check if input ended before null terminator
  if (Status == EFI_NOT_FOUND) {
    // Encode the null terminator into the output buffer
    if (RemainingSize >= Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Increase buffer size
    BufferSize += ThisSize;
  }
  // Set Buffer size if needed
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    *Size = BufferSize;
  }
  return Status;
}
// EfiEncodingPoolConvertCount
/// Convert an encoded string to another encoding
/// @param Encoding       The output encoding or NULL for the current default encoding
/// @param Size           On output, the size in bytes 
/// @param InputEncoding  The input encoding or NULL for the current default encoding
/// @param Input          The input encoded string buffer to decode
/// @param InputSize      The size in bytes of the input encoded string buffer
/// @param Count          The maximum count of characters to convert
/// @return The converted encoded string or NULL if there was an error
EXTERN
VOID *
EFIAPI
EfiEncodingPoolConvertCount (
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN  VOID                  *Input,
  IN  UINTN                  InputSize,
  IN  UINTN                  Count
) {
  VOID  *Buffer;
  UINTN  BufferSize = 0;
  // Get the buffer size
  if ((EfiEncodingConvertCount(Encoding, NULL, &BufferSize, InputEncoding, Input, InputSize, Count) != EFI_BUFFER_TOO_SMALL) || (BufferSize == 0)) {
    return NULL;
  }
  // Allocate the buffer
  Buffer = EfiAllocate(BufferSize);
  if (Buffer == NULL) {
    return NULL;
  }
  // Convert the string
  if (EFI_ERROR(EfiEncodingConvertCount(Encoding, Buffer, &BufferSize, InputEncoding, Input, InputSize, Count))) {
    EfiFreePool(Buffer);
    return NULL;
  }
  // Return the buffer
  if (Size != NULL) {
    *Size = BufferSize;
  }
  return Buffer;
}

// EfiEncodingPrintSize
/// Get the size in bytes of a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The size in bytes of the formatted character string
UINTN
EFIAPI
EfiEncodingPrintSize (
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  ...
) {
  UINTN   Result;
  VA_LIST Marker;
  VA_START(Marker, FormatSize);
  Result = EfiEncodingVPrintSize(Encoding, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return Result;
}
// EfiEncodingVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The size in bytes of the formatted character string
UINTN
EFIAPI
EfiEncodingVPrintSize (
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  IN VA_LIST                Marker
) {
  return EfiLocaleVPrintSize(NULL, Encoding, FormatEncoding, Format, FormatSize, Marker);
}

// EfiEncodingPoolPrint
/// Allocate a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param Size           On output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
VOID *
EFIAPI
EfiEncodingPoolPrint (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  ...
) {
  VOID    *Result;
  VA_LIST  Marker;
  VA_START(Marker, FormatSize);
  Result = EfiLocalePoolVPrint(NULL, Encoding, Size, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return Result;
}
// EfiEncodingPoolVPrint
/// Allocate a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param Size           On output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
VOID *
EFIAPI
EfiEncodingPoolVPrint (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  VA_LIST                    Marker
) {
  return EfiLocalePoolVPrint(NULL, Encoding, Size, FormatEncoding, Format, FormatSize, Marker);
}

// EfiEncodingSPrint
/// Print a formatted character string to a character string buffer
/// @param Encoding       The encoding of the formatted character string
/// @param Buffer         On output, the formatted character string
/// @param Size           On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiEncodingSPrint (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN     CONST VOID            *Format,
  IN     UINTN                  FormatSize,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Marker;
  VA_START(Marker, FormatSize);
  Status = EfiEncodingVSPrint(Encoding, Buffer, Size, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return Status;
}
// EfiEncodingVSPrint
/// Print a formatted character string to a character string buffer
/// @param Encoding       The encoding of the formatted character string
/// @param Buffer         On output, the formatted character string
/// @param Size           On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingVSPrint (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN     CONST VOID            *Format,
  IN     UINTN                  FormatSize,
  IN     VA_LIST                Marker
) {
  return EfiLocaleVSPrint(NULL, Encoding, Buffer, Size, FormatEncoding, Format, FormatSize, Marker);
}

// EfiEncodingPrintBoolean
/// Print a boolean to a character string buffer
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Boolean  The boolean
/// @param Width    The minimum count of characters to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintBoolean (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     BOOLEAN                Boolean,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintBoolean(NULL, Encoding, Buffer, Size, Boolean, Width, Flags);
}
// EfiEncodingPrintUnsigned
/// Print an unsigned integer to a character string buffer
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Unsigned The unsigned integer
/// @param Base     The base of the unsigned integer (usually 10 for decimal)
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintUnsigned (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT64                 Unsigned,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintUnsigned(NULL, Encoding, Buffer, Size, Unsigned, Base, Width, Flags);
}
// EfiEncodingPrintInteger
/// Print an unsigned integer to a character string buffer
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Integer  The unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingPrintInteger (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     INT64                  Integer,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintInteger(NULL, Encoding, Buffer, Size, Integer, Base, Width, Flags);
}
// EfiEncodingPrintFloat
/// Print a floating point number to a character string buffer
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Float     The floating point value
/// @param Width     The minimum count of digits to print
/// @param Precision The maximum count of digits to print after the separator
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintFloat (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintFloat(NULL, Encoding, Buffer, Size, Float, Width, Precision, Flags);
}
// EfiEncodingPrintScientific
/// Print a scientific floating point number to a character string buffer
/// @param Encoding     The encoding of the formatted character string
/// @param Buffer       On output, the formatted character string
/// @param Size         On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Float        The floating point value
/// @param Width        The minimum count of digits to print
/// @param Significance The maximum significant digits
/// @param Flags        The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintScientific (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Significance,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintScientific(NULL, Encoding, Buffer, Size, Float, Width, Significance, Flags);
}
// EfiEncodingPrintMonetary
/// Print a monetary value to a character string buffer
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Monetary  The monetary value
/// @param Width     The minimum count of digits to print
/// @param Precision The maximum count of digits to print after the separator
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintMonetary (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Monetary,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintMonetary(NULL, Encoding, Buffer, Size, Monetary, Width, Precision, Flags);
}
// EfiEncodingPrintDateTime
/// Print a time interface to a character string buffer as a per ISO 8601
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Time     The time interface
/// @param Type     The type of date and/or time printing method (EFI_LOCALE_PRINT_TIME, EFI_LOCALE_PRINT_DATE, EFI_LOCALE_PRINT_LONG_DATE)
/// @param Width    The minimum count of characters to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintDateTime (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_TIME        *Time,
  IN     UINTN                  Type,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  return EfiLocalePrintDateTime(NULL, Encoding, Buffer, Size, Time, Type, Width, Flags);
}
// EfiEncodingPrintGuid
/// Print a GUID interface to a character string buffer
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Guid      The GUID interface
/// @param Uppercase Whether to use uppercase digits if available or not
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiEncodingPrintGuid (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_GUID        *Guid,
  IN     BOOLEAN                Uppercase
) {
  CONST CHAR8 *Format;
  // Check parameters
  if (Guid == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Format = Uppercase ? "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" : "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
  return EfiLocaleSPrint(NULL, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format),
                         Guid->Data1, Guid->Data2, Guid->Data3,
                         Guid->Data4[0], Guid->Data4[1], Guid->Data4[2], Guid->Data4[3],
                         Guid->Data4[4], Guid->Data4[5], Guid->Data4[6], Guid->Data4[7]);
}

// EfiEncodingToBoolean
/// Convert a boolean formatted character string buffer to a boolean
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Boolean  On output, the converted boolean
/// @retval EFI_INVALID_PARAMETER If Locale, Encoding, Buffer, *Buffer, or Boolean is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to a boolean successfully
EFI_STATUS
EFIAPI
EfiEncodingToBoolean (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT BOOLEAN                *Boolean
) {
  return EfiLocaleToBoolean(NULL, Encoding, Buffer, Size, Boolean);
}
// EfiEncodingToUnsigned
/// Convert an unsigned integer formatted character string buffer to an unsigned integer
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Unsigned On output, the converted unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If This, Encoding, Buffer, *Buffer, or Unsigned is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to an unsigned integer successfully
EFI_STATUS
EFIAPI
EfiEncodingToUnsigned (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
) {
  return EfiLocaleToUnsigned(NULL, Encoding, Buffer, Size, Unsigned, Base);
}
// EfiEncodingToInteger
/// Convert a signed integer formatted character string buffer to an signed integer
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Integer  On output, the converted signed integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer,  or Integer is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to an unsigned integer successfully
EFI_STATUS
EFIAPI
EfiEncodingToInteger (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
) {
  return EfiLocaleToInteger(NULL, Encoding, Buffer, Size, Integer, Base);
}
// EfiEncodingToFloat
/// Convert a floating point formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiEncodingToFloat (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  return EfiLocaleToFloat(NULL, Encoding, Buffer, Size, Float);
}
// EfiEncodingToScientific
/// Convert a scientific formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiEncodingToScientific (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  return EfiLocaleToScientific(NULL, Encoding, Buffer, Size, Float);
}
// EfiEncodingToMonetary
/// Convert a monetarily formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiEncodingToMonetary (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  return EfiLocaleToMonetary(NULL, Encoding, Buffer, Size, Float);
}
// EfiEncodingToDateTime
/// Convert a date and/or time formatted character string buffer to a date and time
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param DateTime On output, the converted date and time
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiEncodingToDateTime (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_TIME               *DateTime
) {
  return EfiLocaleToDateTime(NULL, Encoding, Buffer, Size, DateTime);
}
// EfiEncodingToGuid
/// Convert a GUID formatted character string buffer to a GUID
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Guid     On output, the converted GUID
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Guid is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a GUID successfully
EFI_STATUS
EFIAPI
EfiEncodingToGuid (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_GUID               *Guid
) {
  EFI_STATUS           Status;
  EFI_GUID             Result;
  EFI_LOCALE_PROTOCOL *Locale;
  VOID                *Ptr;
  UINT64               Data;
  UINTN                BufferSize;
  UINTN                ThisSize;
  UINT32               Character;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (*Buffer == NULL) || (Guid == NULL)) {
   return EFI_INVALID_PARAMETER;
  }
  // Zero the result GUID
  EfiZero(EFI_GUID, &Result);
  // Get the default locale
  Locale = EfiDefaultLocale();
  // Get the first part of the GUID
  Ptr = *Buffer;
  BufferSize = *Size;
  Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Data, 16);
  if (!EFI_ERROR(Status)) {
    Result.Data1 = (UINT32)Data;
    // The next character should be a dash
    Character = 0;
    ThisSize = BufferSize;
    Status = EfiDecodeCharacter(Encoding, Ptr, &ThisSize, &Character);
    if (!EFI_ERROR(Status)) {
      if (Character == '-') {
        // Advance the buffer and reduce the remaining size
        BufferSize -= ThisSize;
        Ptr = ADDRESS_OFFSET(VOID *, Ptr, ThisSize);
        // Get the second part of the GUID
        Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Data, 16);
        if (!EFI_ERROR(Status)) {
          Result.Data2 = (UINT16)Data;
          // The next character should be a dash
          Character = 0;
          ThisSize = BufferSize;
          Status = EfiDecodeCharacter(Encoding, Ptr, &ThisSize, &Character);
          if (!EFI_ERROR(Status)) {
            if (Character == '-') {
              // Advance the buffer and reduce the remaining size
              BufferSize -= ThisSize;
              Ptr = ADDRESS_OFFSET(VOID *, Ptr, ThisSize);
              // Get the third part of the GUID
              Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Data, 16);
              if (!EFI_ERROR(Status)) {
                Result.Data3 = (UINT16)Data;
                // The next character should be a dash
                Character = 0;
                ThisSize = BufferSize;
                Status = EfiDecodeCharacter(Encoding, Ptr, &ThisSize, &Character);
                if (!EFI_ERROR(Status)) {
                  if (Character == '-') {
                    // Advance the buffer and reduce the remaining size
                    BufferSize -= ThisSize;
                    Ptr = ADDRESS_OFFSET(VOID *, Ptr, ThisSize);
                    // Get the fourth part of the GUID
                    Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Data, 16);
                    if (!EFI_ERROR(Status)) {
                      Result.Data4[0] = (UINT8)EFI_BITFIELD(Data, 8, 15);
                      Result.Data4[1] = (UINT8)EFI_BITFIELD(Data, 0, 7);
                      // The next character should be a dash
                      Character = 0;
                      ThisSize = BufferSize;
                      Status = EfiDecodeCharacter(Encoding, Ptr, &ThisSize, &Character);
                      if (!EFI_ERROR(Status)) {
                        if (Character == '-') {
                          // Advance the buffer and reduce the remaining size
                          BufferSize -= ThisSize;
                          Ptr = ADDRESS_OFFSET(VOID *, Ptr, ThisSize);
                          // Get the fifth part of the GUID
                          Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Data, 16);
                          if (!EFI_ERROR(Status)) {
                            Result.Data4[2] = (UINT8)EFI_BITFIELD(Data, 40, 48);
                            Result.Data4[3] = (UINT8)EFI_BITFIELD(Data, 32, 39);
                            Result.Data4[4] = (UINT8)EFI_BITFIELD(Data, 24, 31);
                            Result.Data4[5] = (UINT8)EFI_BITFIELD(Data, 16, 23);
                            Result.Data4[6] = (UINT8)EFI_BITFIELD(Data, 8, 15);
                            Result.Data4[7] = (UINT8)EFI_BITFIELD(Data, 0, 7);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  // If the string was not valid to decode the GUID just keep the zeros that were originally in result
  if (Status == EFI_NOT_FOUND) {
    Status = EFI_SUCCESS;
  }
  if (!EFI_ERROR(Status)) {
    // Set the remaining buffer and remaining buffer size
    *Buffer = Ptr;
    *Size = BufferSize;
    // Copy the GUID and return success
    EfiCopy(EFI_GUID, Guid, &Result);
  }
  return Status;
}
