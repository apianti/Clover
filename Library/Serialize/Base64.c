///
/// @file Library/Serialize/Base64.c
///
/// UEFI implementation Base64
///

#include <Uefi.h>

// mBase64Dictionaryionary
/// Base 64 dictionary
STATIC CHAR8 mBase64Dictionary[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// IsBase64Character
/// Check if character is Base64 character
/// @param Character The character to check is Base64
/// @retval TRUE  The character is Base64
/// @retval FALSE The character is not Base64
STATIC
BOOLEAN
EFIAPI
IsBase64Character (
  IN UINT32 Character
) {
  return (((Character >= 'A') && (Character <= 'Z')) ||
          ((Character >= 'a') && (Character <= 'z')) ||
          ((Character >= '0') && (Character <= '9')) ||
          (Character == '+') || (Character == '/') || (Character == '='));
}
// FromBase64Character
/// Get 6bit value from 8bit encoded character
/// @param Character The character to decode
/// @return The decoded 6bit value
STATIC
UINT32
EFIAPI
FromBase64Character (
  IN UINT32 Character
) {
  UINT32 Result = 0;
  if ((Character >= 'A') && (Character <= 'Z')) {
    Result = (Character - 'A');
  } else if ((Character >= 'a') && (Character <= 'z')) {
    Result = ((Character - 'a') + 26);
  } else if ((Character >= '0') && (Character <= '9')) {
    Result = ((Character - '0') + 52);
  } else if (Character == '+') {
    Result = 62;
  } else if (Character == '/') {
    Result = 63;
  }
  return Result;
}

// Base64Decode
/// Convert an encoded character string to a data buffer or a data buffer to an encoded character string
/// @param Encoding   The input encoding protocol interface
/// @param Input      The input encoded character string or data buffer
/// @param InputSize  The size in bytes of the input buffer
/// @param Output     The data output buffer
/// @param OutputSize On input, the size in bytes of the output buffer, on output, the size in bytes of the converted output buffer
/// @retval EFI_INVALID_PARAMETER  If Input or OutputSize is NULL
/// @retval EFI_INVALID_PARAMETER  If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted buffer, *OutputSize is updated with the required size
/// @retval EFI_SUCCESS            The buffer was converted successfully
EFI_STATUS
EFIAPI
Base64Decode (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     CONST VOID            *Input,
  IN     UINTN                  InputSize,
  OUT    VOID                  *Output OPTIONAL,
  IN OUT UINTN                 *OutputSize
) {
  EFI_STATUS Result = EFI_SUCCESS;
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      RemainingSize;
  UINTN      BufferSize;
  UINTN      ThisSize;
  UINTN      Count;
  UINT8     *Buffer = (UINT8 *)Output;
  UINT32     Units[4];
  // Set default encoding if needed
  if (Encoding == NULL) {
    Encoding = EfiDefaultEncoding();
  }
  // Check parameters
  if ((Encoding == NULL) || (Input == NULL) || (InputSize == 0) || (OutputSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the size of the buffer
  Count = 0;
  BufferSize = 0;
  Buffer = (UINT8 *)Input;
  RemainingSize = InputSize;
  // Count the length of the string
  while (RemainingSize >= Encoding->UnitSize) {
    ThisSize = RemainingSize;
    Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, Units);
    if (EFI_ERROR(Status)) {
      break;
    }
    // Advance the input buffer and increase output buffer size
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    BufferSize += ThisSize;
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Stop on null terminator
    if ((Units[Count] == 0) && (RemainingSize < Encoding->UnitSize)) {
      break;
    }
    // Check this is a valid base 64 character or skip
    if (!IsBase64Character(*Units)) {
      LOG(L"Unknown base64 character `%c` (0x%x)\n", *Units, *Units);
      Result = EFI_WARN_UNKNOWN_GLYPH;
      continue;
    }
    // Increment the base 64 count
    ++Count;
  }
  // Adjust the length of the base 64 to the count of bytes
  BufferSize = ((Count * 3) >> 2);
  // Get buffer size
  if ((Output == NULL) || (*OutputSize < (BufferSize * sizeof(UINT8)))) {
    *OutputSize = (BufferSize * sizeof(UINT8));
    return EFI_BUFFER_TOO_SMALL;
  }
  // Decode the base 64 in blocks, one block is four characters and three bytes when decoded
  Count = 0;
  Buffer = (UINT8 *)Output;
  RemainingSize = InputSize;
  while (RemainingSize >= Encoding->UnitSize) {
    EfiZeroMem(Units, sizeof(Units));
    // Decode the each unit character of this block
    Count = 0;
    while ((Count < 4) && (RemainingSize >= Encoding->UnitSize)) {
      ThisSize = RemainingSize;
      Status = EfiDecodeCharacter(Encoding, Input, &ThisSize, Units + Count);
      if (EFI_ERROR(Status)) {
        break;
      }
      // Advance the input buffer and increase output buffer size
      Input = ADDRESS_OFFSET(VOID, Input, ThisSize);
      BufferSize += ThisSize;
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Stop on null terminator
      if ((Units[Count] == 0) && (RemainingSize < Encoding->UnitSize)) {
        break;
      }
      // Check this is a valid base 64 character or skip
      if (!IsBase64Character(Units[Count])) {
        LOG(L"Unknown base64 character `%c` (0x%x)\n", Units[Count], Units[Count]);
        Result = EFI_WARN_UNKNOWN_GLYPH;
        continue;
      }
      // Continue to next unit in block after valid character
      ++Count;
    }
    if (EFI_ERROR(Status) || (Count != 4)) {
      break;
    }
    // Decode the block into the unit buffer
    if (Output != NULL) {
      *Buffer = (UINT8)EFI_BITFIELDSHIFTLEFT(FromBase64Character(Units[0]), 2, 0, 5);
      Units[1] = FromBase64Character(Units[1]);
      *Buffer++ |= (UINT8)EFI_BITFIELD(Units[1], 4, 5);
      *Buffer = (UINT8)EFI_BITFIELDSHIFTLEFT(Units[1], 4, 0, 3);
      Units[2] = FromBase64Character(Units[2]);
      *Buffer++ |= (UINT8)EFI_BITFIELD(Units[2], 2, 5);
      *Buffer = (UINT8)EFI_BITFIELDSHIFTLEFT(Units[2], 6, 0, 1);
      *Buffer++ |= (UINT8)EFI_BITFIELD(FromBase64Character(Units[3]), 0, 5);
    }
    Count = 0;
  }
  // If there are any characters left over then the input is malformed base 64
  if (!EFI_ERROR(Status) && (Count != 0)) {
    if ((Count == 2) || (Count == 3)) {
      // Output the last bytes
      *Buffer = (UINT8)EFI_BITFIELDSHIFTLEFT(Units[0], 2, 0, 5);
      *Buffer++ |= (UINT8)EFI_BITFIELD(Units[1], 4, 5);
      if (Count == 3) {
        *Buffer = (UINT8)EFI_BITFIELDSHIFTLEFT(Units[1], 4, 0, 3);
        *Buffer++ |= (UINT8)EFI_BITFIELD(Units[2], 2, 5);
      }
    } else {
      Status = EFI_ABORTED;
    }
  }
  // Return the output size
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Result = Status;
  } else if (EFI_ERROR(Status)) {
    return Status;
  }
  *OutputSize = (BufferSize * sizeof(UINT8));
  return Result;
}

// Base64Encode
/// Convert an encoded character string to a data buffer or a data buffer to an encoded character string
/// @param Encoding   The output encoding protocol interface
/// @param Input      The input encoded character string or data buffer
/// @param InputSize  The size in bytes of the input buffer
/// @param Output     The base 64 encoded output buffer
/// @param OutputSize On input, the size in bytes of the output buffer, on output, the size in bytes of the converted output buffer
/// @retval EFI_INVALID_PARAMETER  If Input or OutputSize is NULL
/// @retval EFI_INVALID_PARAMETER  If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted buffer, *OutputSize is updated with the required size
/// @retval EFI_SUCCESS            The buffer was converted successfully
EFI_STATUS
EFIAPI
Base64Encode (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     CONST VOID            *Input,
  IN     UINTN                  InputSize,
  OUT    VOID                  *Output OPTIONAL,
  IN OUT UINTN                 *OutputSize
) {
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      RemainingSize;
  UINTN      BufferSize;
  UINTN      ThisSize;
  UINTN      Index;
  UINT8     *Buffer = (UINT8 *)Input;
  UINT8      Units[4];
  // Set default encoding if needed
  if (Encoding == NULL) {
    Encoding = EfiDefaultEncoding();
  }
  // Check parameters
  if ((Encoding == NULL) || (Input == NULL) || (OutputSize == NULL) || (InputSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the size of the buffer
  BufferSize = 0;
  RemainingSize = *OutputSize;
  // Encode buffer by getting the individual units from the bytes
  while (InputSize >= (3 * sizeof(UINT8))) {
    EfiZeroMem(Units, sizeof(Units));
    // Set the first unit in this block
    Units[0] = (UINT32)EFI_BITFIELD(*Buffer, 2, 7);
    // The second unit is spread across the high part of the byte and the low part of the next
    Units[1] = (UINT32)EFI_BITFIELDSHIFTLEFT(*Buffer, 4, 0, 1);
    // Advance the input buffer and decrease the remaining input size
    ++Buffer;
    --InputSize;
    Units[1] |= (UINT32)EFI_BITFIELD(*Buffer, 4, 7);
    // The third unit in this character block is spread across two bytes like the second unit
    Units[2] = (UINT32)EFI_BITFIELDSHIFTLEFT(*Buffer, 2, 0, 3);
    // Advance the input buffer and decrease the remaining input size
    ++Buffer;
    --InputSize;
    Units[2] |= (UINT32)EFI_BITFIELD(*Buffer, 6, 7);
    // Set the last unit in this block
    Units[3] = (UINT32)EFI_BITFIELD(*Buffer, 0, 5);
    // Advance the input buffer and decrease the remaining input size
    ++Buffer;
    --InputSize;
    // Encode the units
    for (Index = 0; Index < ARRAY_COUNT(Units); ++Index) {
      ThisSize = RemainingSize;
      Units[Index] = mBase64Dictionary[Units[Index]];
      Status = EfiEncodeCharacter(Encoding, Output, &ThisSize, Units[Index]);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        break;
      }
      if (ThisSize == 0) {
        Status = EFI_ABORTED;
        break;
      }
      // Advance the output buffer and increase output buffer size
      if (Output != NULL) {
        Output = ADDRESS_OFFSET(VOID, Output, ThisSize);
      }
      BufferSize += ThisSize;
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
    }
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      break;
    }
  }
  // Encode any remaining bytes that do not fit into a block
  if ((InputSize != 0) && (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL))) {
    EfiZeroMem(Units, sizeof(Units));
    if (InputSize == sizeof(UINT8)) {
      // Set the first unit in this block
      Units[0] = (UINT32)EFI_BITFIELD(*Buffer, 2, 7);
      // The second unit is spread across the high part of the byte and the low part of the next
      Units[1] = (UINT32)EFI_BITFIELDSHIFTLEFT(*Buffer, 4, 0, 1);
      // Advance the input buffer and decrease the remaining input size
      ++Buffer;
      --InputSize;
      // The last two units will be padding characters
      Units[2] = '=';
      Units[3] = '=';
    } else if (InputSize == (2 * sizeof(UINT8))) {
      // Set the first unit in this block
      Units[0] = mBase64Dictionary[(UINT32)EFI_BITFIELD(*Buffer, 2, 7)];
      // The second unit is spread across the high part of the byte and the low part of the next
      Units[1] = (UINT32)EFI_BITFIELDSHIFTLEFT(*Buffer, 4, 0, 1);
      // Advance the input buffer and decrease the remaining input size
      ++Buffer;
      --InputSize;
      Units[1] = mBase64Dictionary[Units[1] | (UINT32)EFI_BITFIELD(*Buffer, 4, 7)];
      // The third unit in this character block is spread across two bytes like the second unit
      Units[2] = mBase64Dictionary[(UINT32)EFI_BITFIELDSHIFTLEFT(*Buffer, 2, 0, 3)];
      // Advance the input buffer and decrease the remaining input size
      ++Buffer;
      --InputSize;
      // The last unit will be padding character
      Units[3] = '=';
    }
    // Copy the units into the output
    for (Index = 0; Index < ARRAY_COUNT(Units); ++Index) {
      ThisSize = RemainingSize;
      Status = EfiEncodeCharacter(Encoding, Output, &ThisSize, Units[Index]);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        break;
      }
      if (ThisSize == 0) {
        Status = EFI_ABORTED;
        break;
      }
      // Advance the output buffer and increase output buffer size
      if (Output != NULL) {
        Output = ADDRESS_OFFSET(VOID, Output, ThisSize);
      }
      BufferSize += ThisSize;
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
    }
  }
  // Check to see if there was a problem encoding
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    ThisSize = RemainingSize;
    // Encode the null terminator
    Status = EfiEncodeCharacter(Encoding, Output, &ThisSize, 0);
    if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
      // Increase output buffer size
      BufferSize += ThisSize;
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
    } else if (ThisSize == 0) {
      Status = EFI_ABORTED;
    }
  }
  // Set output buffer size
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    if (OutputSize != NULL) {
      *OutputSize = BufferSize;
    }
  }
  return Status;
}

// Base64DecodeBuffer
/// Convert an encoded character string to a data buffer or a data buffer to an encoded character string
/// @param Encoding   The input encoding protocol interface
/// @param Input      The input encoded character string or data buffer
/// @param InputSize  The size in bytes of the input buffer
/// @param Output     On output, the data output buffer which must be freed
/// @param OutputSize On output, the size in bytes of the converted output buffer
/// @retval EFI_INVALID_PARAMETER  If Input or Output is NULL or *Output is not NULL
/// @retval EFI_INVALID_PARAMETER  If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted buffer, *OutputSize is updated with the required size
/// @retval EFI_SUCCESS            The buffer was converted successfully
EFI_STATUS
EFIAPI
Base64DecodeBuffer (
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST VOID             *Input,
  IN  UINTN                   InputSize,
  OUT VOID                  **Output,
  OUT UINTN                  *OutputSize OPTIONAL
) {
  EFI_STATUS Status;
  UINTN      Size = 0;
  // Check parameters
  if ((Input == NULL) || (InputSize == 0) || (Output == NULL) || (*Output != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get size of output buffer size
  Status = Base64Decode(Encoding, Input, InputSize, NULL, &Size);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }
  if (Size != 0) {
    // Allocate buffer
    VOID *Buffer = EfiAllocate(Size);
    if (Buffer == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    // Decode buffer
    Status = Base64Decode(Encoding, Input, InputSize, Buffer, &Size);
    // Set buffer
    if (!EFI_ERROR(Status)) {
      *Output = Buffer;
    }
  } else if (!EFI_ERROR(Status)) {
    // No output buffer
    *Output = NULL;
  }
  // Return the output size
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    if (OutputSize != NULL) {
      *OutputSize = Size;
    }
  }
  return Status;
}
// Base64EncodeBuffer
/// Convert an encoded character string to a data buffer or a data buffer to an encoded character string
/// @param Encoding   The output encoding protocol interface
/// @param Input      The input encoded character string or data buffer
/// @param InputSize  The size in bytes of the input buffer
/// @param Output     On output, the base 64 encoded output buffer which must be freed
/// @param OutputSize On output, the size in bytes of the converted output buffer
/// @retval EFI_INVALID_PARAMETER  If Input or Output is NULL or *Output is not NULL
/// @retval EFI_INVALID_PARAMETER  If InputSize is zero
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted buffer, *OutputSize is updated with the required size
/// @retval EFI_SUCCESS            The buffer was converted successfully
EFI_STATUS
EFIAPI
Base64EncodeBuffer (
  IN  EFI_ENCODING_PROTOCOL  *Encoding,
  IN  CONST VOID             *Input,
  IN  UINTN                   InputSize,
  OUT VOID                  **Output,
  OUT UINTN                  *OutputSize OPTIONAL
) {
  EFI_STATUS Status;
  UINTN      Size = 0;
  // Check parameters
  if ((Input == NULL) || (InputSize == 0) || (Output == NULL) || (*Output != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get size of output buffer size
  Status = Base64Encode(Encoding, Input, InputSize, NULL, &Size);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }
  if (Size != 0) {
    // Allocate buffer
    VOID *Buffer = EfiAllocate(Size);
    if (Buffer == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    // Encode buffer
    Status = Base64Encode(Encoding, Input, InputSize, Buffer, &Size);
    // Set buffer
    if (!EFI_ERROR(Status)) {
      *Output = Buffer;
    }
  } else if (!EFI_ERROR(Status)) {
    // No output buffer
    *Output = NULL;
  }
  // Return the output size
  if (!EFI_ERROR(Status) || (Status != EFI_BUFFER_TOO_SMALL)) {
    if (OutputSize != NULL) {
      *OutputSize = Size;
    }
  }
  return Status;
}

// Base64FromString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
CHAR16 *
EFIAPI
Base64FromString (
  IN CONST CHAR16 *String
) {
  CHAR16 *Base64 = NULL;
  // Encode the string into base 64
  if ((String == NULL) || EFI_ERROR(Base64EncodeBuffer(EfiUtf16Encoding(), String, StrSize(String), &Base64, NULL))) {
    return NULL;
  }
  // Return the base 64 encoded string
  return Base64;
}
// Base64ToString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
CHAR16 *
EFIAPI
Base64ToString (
  IN CONST CHAR16 *Base64
) {
  CHAR16 *String = NULL;
  // Decode the string from base 64
  if ((Base64 == NULL) || EFI_ERROR(Base64DecodeBuffer(EfiUtf16Encoding(), Base64, StrSize(Base64), &String, NULL))) {
    return NULL;
  }
  // Return the decoded string
  return String;
}

// Base64FromAsciiString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
CHAR16 *
EFIAPI
Base64FromAsciiString (
  IN CONST CHAR8 *String
) {
  CHAR16 *Base64 = NULL;
  // Encode the string into base 64
  if ((String == NULL) || EFI_ERROR(Base64EncodeBuffer(EfiUtf16Encoding(), String, AsciiStrSize(String), &Base64, NULL))) {
    return NULL;
  }
  // Return the base 64 encoded string
  return Base64;
}
// Base64ToAsciiString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
CHAR8 *
EFIAPI
Base64ToAsciiString (
  IN CONST CHAR16 *Base64
) {
  CHAR8 *String = NULL;
  // Decode the string from base 64
  if ((Base64 == NULL) || EFI_ERROR(Base64DecodeBuffer(EfiUtf8Encoding(), Base64, StrSize(Base64), &String, NULL))) {
    return NULL;
  }
  // Return the decoded string
  return String;
}

// Base64AsciiFromString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
CHAR8 *
EFIAPI
Base64AsciiFromString (
  IN CONST CHAR16 *String
) {
  CHAR8 *Base64 = NULL;
  // Encode the string into base 64
  if ((String == NULL) || EFI_ERROR(Base64EncodeBuffer(EfiUtf8Encoding(), String, StrSize(String), &Base64, NULL))) {
    return NULL;
  }
  // Return the base 64 encoded string
  return Base64;
}
// Base64AsciiToString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
CHAR16 *
EFIAPI
Base64AsciiToString (
  IN CONST CHAR8 *Base64
) {
  CHAR16 *String = NULL;
  // Decode the string from base 64
  if ((Base64 == NULL) || EFI_ERROR(Base64DecodeBuffer(EfiUtf8Encoding(), Base64, AsciiStrSize(Base64), &String, NULL))) {
    return NULL;
  }
  // Return the decoded string
  return String;
}

// Base64AsciiFromAsciiString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
CHAR8 *
EFIAPI
Base64AsciiFromAsciiString (
  IN CONST CHAR8 *String
) {
  CHAR8 *Base64 = NULL;
  // Encode the string into base 64
  if ((String == NULL) || EFI_ERROR(Base64EncodeBuffer(EfiUtf8Encoding(), String, AsciiStrSize(String), &Base64, NULL))) {
    return NULL;
  }
  // Return the base 64 encoded string
  return Base64;
}
// Base64ToAsciiString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
CHAR8 *
EFIAPI
Base64AsciiToAsciiString (
  IN CONST CHAR8 *Base64
) {
  CHAR8 *String = NULL;
  // Decode the string from base 64
  if ((Base64 == NULL) || EFI_ERROR(Base64DecodeBuffer(EfiUtf8Encoding(), Base64, AsciiStrSize(Base64), &String, NULL))) {
    return NULL;
  }
  // Return the decoded string
  return String;
}
