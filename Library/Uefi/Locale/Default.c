///
/// @file Library/Uefi/Locale/Default.c
///
/// Default locale
///

#include "VirtualMemory.h"

// EFI_LOCALE_DEFAULT_SEPARATOR_SYMBOL
/// The default floating point separator symbol character
#define EFI_LOCALE_DEFAULT_SEPARATOR_SYMBOL '.'
// EFI_LOCALE_DEFAULT_EXPONENT_SYMBOL
/// The default exponent symbol character
#define EFI_LOCALE_DEFAULT_EXPONENT_SYMBOL 'e'
// EFI_LOCALE_DEFAULT_MONETARY_SYMBOL
/// The default monetary symbol character
#define EFI_LOCALE_DEFAULT_MONETARY_SYMBOL '$'

// mLatin1Handle
/// The handle of the installed encoding protocol
STATIC EFI_HANDLE mDefaultLocaleHandle = NULL;
// mNumber
/// This character buffer is for the longest representation of an integer in binary and the null terminator
STATIC CHAR8      mNumber[(sizeof(UINT64) << 3) + 1];

// gEfiLatin1EncodingProtocolGuid
/// The default locale protocol unique identifier
EFI_GUID gEfiDefaultLocaleProtocolGuid = EFI_DEFAULT_LOCALE_PROTOCOL_GUID;

// EfiDefaultLocale
/// Get the default locale protocol
/// @return The default locale protocol or NULL if there was an error
EFI_LOCALE_PROTOCOL *
EFIAPI
EfiDefaultLocale (
  VOID
) {
  EFI_LOCALE_PROTOCOL *Locale = NULL;
  if (EFI_ERROR(EfiLocateProtocol(&gEfiDefaultLocaleProtocolGuid, NULL, (VOID **)&Locale))) {
    return NULL;
  }
  return Locale;
}

// DefaultLocaleCompare
/// Compare two encoded character strings
/// @param This        The locale protocol
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first
STATIC
INTN
EFIAPI
DefaultLocaleCompare (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  EFI_STATUS Status1;
  EFI_STATUS Status2;
  UINTN      ThisSize;
  UINT32     Character1;
  UINT32     Character2;
  // Check parameters
  if ((This == NULL) || (Encoding1 == NULL) || (Encoding2 == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if buffers are empty
  if ((Buffer1 == NULL) || (BufferSize1 == 0)) {
    if ((Buffer2 == NULL) || (BufferSize2 == 0)) {
      return 0;
    }
    return -1;
  } else if ((Buffer2 == NULL) || (BufferSize2 == 0)) {
    return 1;
  }
  // Decode the characters from both encoded strings and compare them individually
  do {
    // Decode character from the first encoded string buffer
    ThisSize = BufferSize1;
    Status1 = EfiDecodeCharacter(Encoding1, Buffer1, &ThisSize, &Character1);
    if (!EFI_ERROR(Status1)) {
      BufferSize1 -= ThisSize;
      Buffer1 = ADDRESS_OFFSET(VOID, Buffer1, ThisSize);
    }
    // Decode character from the second encoded string buffer
    ThisSize = BufferSize2;
    Status2 = EfiDecodeCharacter(Encoding2, Buffer2, &ThisSize, &Character2);
    if (!EFI_ERROR(Status2)) {
      BufferSize2 -= ThisSize;
      Buffer2 = ADDRESS_OFFSET(VOID, Buffer2, ThisSize);
    }
    // If there was an error treat it as if the string ended
    if (EFI_ERROR(Status1)) {
      if (EFI_ERROR(Status2)) {
        return 0;
      }
      return -1;
    } else if (EFI_ERROR(Status2)) {
      return 1;
    }
  // Continue while the characters are the same and not the end of both
  } while ((Character1 == Character2) && (Character1 != 0));
  // Return the difference in the characters
  return (Character1 - Character2);
}
// DefaultLocaleNCompare
/// TODO: Compare two encoded character strings with a maximum count
/// @param This        The locale protocol
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
STATIC
INTN
EFIAPI
DefaultLocaleNCompare (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
  UNUSED_PARAMETER(This);
  UNUSED_PARAMETER(Encoding1);
  UNUSED_PARAMETER(Buffer1);
  UNUSED_PARAMETER(BufferSize1);
  UNUSED_PARAMETER(Encoding2);
  UNUSED_PARAMETER(Buffer2);
  UNUSED_PARAMETER(BufferSize2);
  UNUSED_PARAMETER(Count);
  return 0;
}
// DefaultLocaleICompare
/// TODO: Compare two encoded character strings case insensitively
/// @param This        The locale protocol
/// @param Encoding1   The encoding of the first character string
/// @param Buffer1     The first character string to compare
/// @param BufferSize1 The size of the first character string in bytes
/// @param Encoding2   The encoding of the second character string
/// @param Buffer2     The second character string to compare
/// @param BufferSize2 The size of the second character string in bytes
/// @retval 0  The encoded character strings are the same
/// @retval >0 The first encoded character string is lexigraphically before the second
/// @retval <0 The second encoded character string is lexigraphically before the first
STATIC
INTN
EFIAPI
DefaultLocaleICompare (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  UNUSED_PARAMETER(This);
  UNUSED_PARAMETER(Encoding1);
  UNUSED_PARAMETER(Buffer1);
  UNUSED_PARAMETER(BufferSize1);
  UNUSED_PARAMETER(Encoding2);
  UNUSED_PARAMETER(Buffer2);
  UNUSED_PARAMETER(BufferSize2);
  return 0;
}
// DefaultLocaleNICompare
/// TODO: Compare two encoded character strings case insensitively with a maximum count
/// @param This        The locale protocol
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
STATIC
INTN
EFIAPI
DefaultLocaleNICompare (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
  UNUSED_PARAMETER(This);
  UNUSED_PARAMETER(Encoding1);
  UNUSED_PARAMETER(Buffer1);
  UNUSED_PARAMETER(BufferSize1);
  UNUSED_PARAMETER(Encoding2);
  UNUSED_PARAMETER(Buffer2);
  UNUSED_PARAMETER(BufferSize2);
  UNUSED_PARAMETER(Count);
  return 0;
}

// DefaultLocalePrintBoolean
/// Print a boolean to a character string buffer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Boolean  The boolean
/// @param Width    The minimum count of characters to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintBoolean (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     BOOLEAN                Boolean,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  EFI_STATUS Status;
  UINTN      BufferSize;
  UNUSED_PARAMETER(Flags);
  // Check parameters
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Print the value to the buffer
  BufferSize = *Size;
  Status = EfiLocaleSPrint(This, Encoding, Buffer, &BufferSize, EfiLatin1Encoding(), "%*a", AsciiStrSize("%*a"), Width, Boolean ? "true" : "false");
  // Return the size if successful
  if (!EFI_ERROR(Status) || (Status == EFI_BUFFER_TOO_SMALL)) {
    *Size = BufferSize;
  }
  return Status;
}
// DefaultLocalePrintUnsigned
/// Print an unsigned integer to a character string buffer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Unsigned The unsigned integer
/// @param Base     The base of the unsigned integer (usually 10 for decimal)
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintUnsigned (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT64                 Unsigned,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  EFI_STATUS Status;
  UINTN      RemainingSize;
  UINTN      BufferSize = 0;
  UINTN      ThisSize;
  UINTN      Index = 0;
  UINT32     SignCharacter = 0;
  // Check size is valid pointer
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check the base is valid
  if ((Base < 2) || (Base > 36)) {
    Base = 10;
  }
  // Check for the sign character
  if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN)) {
    SignCharacter = '+';
    if (Width != 0) {
      --Width;
    }
  }
  // Zero is a special case
  if (Unsigned == 0) {
    mNumber[Index++] = '0';
  } else {
    CHAR8 BaseCharacter = EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE) ? 'A' : 'a';
    // Get the characters for the integer
    while (Unsigned != 0) {
      UINTN Digit = Unsigned % Base;
      if (Digit < 10) {
        Digit += '0';
      } else {
        Digit -= 10;
        Digit += BaseCharacter;
      }
      mNumber[Index++] = (CHAR8)Digit;
      Unsigned /= Base;
    }
  }
  // Get the remaining size of the buffer
  RemainingSize = *Size;
  // Right justify by pushing the prefix character for the remaining width
  if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
    // Set the minimum width
    if (Index < Width) {
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Adjust the width for right justification
      Width -= Index;
      // Pad the buffer with spaces for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, ' ');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
    // To prevent accidental justification later
    Width = 0;
  }
  // Check if the sign character needs encoded
  if (SignCharacter != NULL) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Encode the sign character
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, SignCharacter);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Check if zeros need encoded for right justification
  if (Index < Width) {
    // Adjust the width for right justification
    Width -= Index;
    if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY) &&
        EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      // Right justify by encoding the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, '0');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
      // To prevent accidental justification later
      Width = 0;
    }
  } else  {
    // To prevent accidental justification later
    Width = 0;
  }
  // Encode the integer into the buffer
  while (Index != 0) {
    // Encode the digit character
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, mNumber[--Index]);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Add left justification if needed
  if ((Width != 0) && EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Pad the buffer with the padding character for left justification
    Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES) ? '0' : ' ');
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Encode the null terminator
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Set the total size of the integer
  BufferSize += ThisSize;
  *Size = BufferSize;
  UNUSED_PARAMETER(This);
  return Status;
}
// DefaultLocalePrintInteger
/// Print a signed integer to a character string buffer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Integer  The unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintInteger (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     INT64                  Integer,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  EFI_STATUS Status;
  UINTN      RemainingSize;
  UINTN      BufferSize = 0;
  UINTN      ThisSize;
  UINTN      Index = 0;
  UINT32     SignCharacter = 0;
  // Check size is valid pointer
  if (Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check the base is valid
  if ((Base < 2) || (Base > 36)) {
    Base = 10;
  }
  // Check for the sign character
  if (Integer < 0LL) {
    Integer = -Integer;
    SignCharacter = '-';
  } else if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN)) {
    SignCharacter = '+';
  }
  if ((SignCharacter != 0) && (Width != 0)) {
    --Width;
  }
  // Zero is a special case
  if (Integer == 0) {
    mNumber[Index++] = '0';
  } else {
    CHAR8 BaseCharacter = EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE) ? 'A' : 'a';
    // Get the characters for the integer
    while (Integer != 0) {
      UINTN Digit = Integer % Base;
      if (Digit < 10) {
        Digit += '0';
      } else {
        Digit -= 10;
        Digit += BaseCharacter;
      }
      mNumber[Index++] = (CHAR8)Digit;
      Integer /= Base;
    }
  }
  // Get the remaining size of the buffer
  RemainingSize = *Size;
  // Right justify by pushing the prefix character for the remaining width
  if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
    // Set the minimum width
    if (Index < Width) {
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Adjust the width for right justification
      Width -= Index;
      // Pad the buffer with spaces for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, ' ');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
    // To prevent accidental justification later
    Width = 0;
  }
  // Check if the sign character needs encoded
  if (SignCharacter != NULL) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Encode the sign character
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, SignCharacter);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Check if zeros need encoded for right justification
  if (Index < Width) {
    // Adjust the width for right justification
    Width -= Index;
    if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY) &&
        EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      // Right justify by encoding the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, '0');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
      // To prevent accidental justification later
      Width = 0;
    }
  } else  {
    // To prevent accidental justification later
    Width = 0;
  }
  // Encode the integer into the buffer
  while (Index != 0) {
    // Encode the digit character
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, mNumber[--Index]);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Add left justification if needed
  if ((Width != 0) && EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Pad the buffer with the padding character for left justification
    Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES) ? '0' : ' ');
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Encode the null terminator
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Set the total size of the integer
  BufferSize += ThisSize;
  *Size = BufferSize;
  UNUSED_PARAMETER(This);
  return Status;
}

// mPrecisionRounders
/// Precision rounder values for best precision
STATIC FLOAT64 mPrecisionRounders[] = {
  0.05,
  0.005,
  0.0005,
  0.00005,
  0.000005,
  0.0000005,
  0.00000005,
  0.000000005,
  0.0000000005,
  0.00000000005,
  0.000000000005,
  0.0000000000005,
  0.00000000000005,
  0.000000000000005,
  0.0000000000000005
};

// DefaultLocalePrintFloat
/// Print a floating point number to a character string buffer
/// @param This      The locale protocol
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Float     The floating point value
/// @param Width     The minimum count of digits to print
/// @param Precision The maximum count of digits to print after the separator
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintFloat (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  EFI_STATUS   Status;
  UINT64       Integer = 0;
  UINTN        RemainingSize;
  UINTN        BufferSize = 0;
  UINTN        ThisSize;
  UINTN        Index = 0;
  CHAR8        SignCharacter = 0;
  BOOLEAN      Negative = FALSE;
  // Check parameters
  if ((This == NULL) || (Encoding == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Extract the floating point components
  Status = EfiFloat64Extract(Float, &Integer, &Float, &Negative);
  if (EFI_ERROR(Status)) {
    // Check for special case error statuses
    if (Status == EFI_NO_MAPPING) {
      // Not a number
      if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE)) {
        mNumber[Index++] = 'N';
        mNumber[Index++] = 'A';
        mNumber[Index++] = 'N';
      } else {
        mNumber[Index++] = 'n';
        mNumber[Index++] = 'a';
        mNumber[Index++] = 'n';
      }
      // No precision needed
      Precision = 0;
      // No sign needed or zeroes padding
      Negative = FALSE;
      Flags &= ~(EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES);
    } else if (Status == EFI_NOT_FOUND) {
      // Infinity
      if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE)) {
        mNumber[Index++] = 'F';
        mNumber[Index++] = 'N';
        mNumber[Index++] = 'I';
      } else {
        mNumber[Index++] = 'f';
        mNumber[Index++] = 'n';
        mNumber[Index++] = 'i';
      }
      // No precision needed
      Precision = 0;
      // No zeroes padding
      Flags &= ~EFI_LOCALE_PRINT_FLAG_PAD_ZEROES;
    } else {
      // Invalid format
      return Status;
    }
  } else {
    // Zero is a special case
    if (Integer == 0) {
      mNumber[Index++] = '0';
    } else {
      // Get the characters for the integer
      while (Integer != 0) {
        mNumber[Index++] = '0' + (CHAR8)(Integer % 10);
        Integer /= 10;
      }
    }
  }
  // Adjust width to include precision
  if (Precision != 0) {
    if (Width > Precision) {
      Width -= Precision;
    } else {
      Width = 0;
    }
    // Adjust width for floating point
    if (Width != 0) {
      --Width;
    }
  }
  // Check if integer is negative
  if (Negative) {
    SignCharacter = '-';
  } else if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN)) {
    // Sign prefix was requested for positive numbers also
    SignCharacter = '+';
  }
  // Decrease the width if there is a sign character
  if ((SignCharacter != 0) && (Width != 0)) {
    --Width;
  }
  // Get the remaining size
  RemainingSize = *Size;
  // Set the minimum width
  if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
    if (Index < Width) {
      // Right justify by pushing the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Adjust the width for right justification
      Width -= Index;
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, ' ');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
    // To prevent accidental justification later
    Width = 0;
  } 
  // Output the sign character if needed
  if (SignCharacter != 0) {
    // Right justify by pushing the prefix character for the remaining width
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Encode the sign character
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, SignCharacter);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Check if zeros need encoded for right justification
  if (Index < Width) {
    // Adjust the width for right justification
    Width -= Index;
    if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY) &&
        EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      // Right justify by encoding the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, '0');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
      // To prevent accidental justification later
      Width = 0;
    }
  } else  {
    // To prevent accidental justification later
    Width = 0;
  }
  // Encode the integer into the buffer
  while (Index != 0) {
    // Encode the digit character
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, mNumber[--Index]);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Output fractional part of decimal point if needed
  if (Precision != 0) {
    // Round with precision if possible
    if (Precision <= ARRAY_COUNT(mPrecisionRounders)) {
      Float += mPrecisionRounders[Precision];
    }
    // Encode decimal point separator
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, EFI_LOCALE_DEFAULT_SEPARATOR_SYMBOL);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
    // Encode precision for fractional part of floating point
    while (Precision-- != 0) {
      UINT32 Digit;
      // Get the next fraction digit
      Float *= 10;
      Digit = (UINT32)Float;
      Float -= (FLOAT64)Digit;
      // Encode the next fractional character
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, '0' + Digit);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
  }
  // Add left justification if needed
  if ((Width != 0) && EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Adjust the width for left justification
    Width -= Index;
    // Pad the buffer with spaces for left justification
    Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES) ? '0' : ' ');
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Encode the null terminator
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Return buffer size
  *Size = BufferSize;
  return Status;
}
// DefaultLocalePrintScientific
/// Print a scientific floating point number to a character string buffer
/// @param This         The locale protocol
/// @param Encoding     The encoding of the formatted character string
/// @param Buffer       On output, the formatted character string
/// @param Size         On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Float        The floating point value
/// @param Width        The minimum count of digits to print
/// @param Significance The maximum significant digits
/// @param Flags        The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintScientific (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Significance,
  IN     UINTN                  Flags
) {
  EFI_STATUS   Status;
  INT64        Exponent;
  UINTN        RemainingSize;
  UINTN        BufferSize;
  UINTN        ThisSize;
  UINTN        Length;
  UINTN        Options;
  // Check parameters
  if ((This == NULL) || (Encoding == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Set the intial buffer size to zero
  BufferSize = 0;
  // Normalize the floating point to scientific notation
  Exponent = 0;
  if (Float >= 0.0) {
    // Positive exponent
    while (Float >= 10.0) {
      Float /= 10.0;
      --Exponent;
    }
  } else {
    // Negative exponent
    while (Float <= -10.0) {
      Float /= 10.0;
      ++Exponent;
    }
  }
  // Get the exponent length
  if (Exponent == 0) {
    Length = 2;
  } else {
    INT64 Exponent2 = Exponent;
    for (Length = 1; Exponent2 != 0; ++Length) {
      Exponent2 /= 10;
    }
  }
  if (Length < Width) {
    Width -= Length;
  } else {
    Width = 0;
  }
  // Adjust the significance because one digit will come before the floating point separator
  if (Significance != 0) {
    --Significance;
  }
  // Get the width without the exponent
  Length = Width;
  if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      Options = EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES;
    } else {
      Options = EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY;
      Length = 0;
    }
  } else if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
    Options = EFI_LOCALE_PRINT_FLAG_PAD_ZEROES;
  } else {
    Options = 0;
  }
  // Get the remaining size
  RemainingSize = *Size;
  // Encode the floating point part of the scientific notation
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiLocalePrintFloat(NULL, Encoding, Buffer, &ThisSize, Float, Length, Significance, Options);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Remove the null terminator from encoding the floating point
  if (ThisSize > Encoding->UnitSize) {
    ThisSize -= Encoding->UnitSize;
  } else {
    ThisSize = 0;
  }
  // Update the buffer address and size
  if (Buffer != NULL) {
    // Get the length of the encoded string
    Length = 0;
    Status = EfiEncodingStringLength(Encoding, Buffer, &Length);
    // Update the buffer address
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
  } else {
    // Get the maximum possible length
    Length = (ThisSize / Encoding->UnitSize);
  }
  // Update the buffer size
  if (RemainingSize > ThisSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  // Adjust the width by length of the string
  if (Length < Width) {
    Width -= Length;
  } else {
    Width = 0;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Encode the exponent symbol character
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, EFI_LOCALE_DEFAULT_EXPONENT_SYMBOL);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Update the buffer address and size
  if (Buffer != NULL) {
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
  }
  if (RemainingSize > ThisSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Encode the floating point part of the scientific notation
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  // Get the flags to use when printing the exponent
  if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    // Left justified
    Options = EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY;
    if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      // Just print the exponent
      Length = 0;
    } else {
      // Need to pad the right with spaces after the exponent
      Length = Width + 1;
    }
  } else {
    // Right justified so just print the exponent
    Length = 0;
    Options = 0;
  }
  // Encoding the exponent should also encode the null terminator
  Status = EfiLocalePrintInteger(NULL, Encoding, Buffer, &ThisSize, Exponent, 10, Length, Options);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Update the buffer address and size
  if (Buffer != NULL) {
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
  }
  if (RemainingSize > ThisSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Return buffer size
  *Size = BufferSize;
  UNUSED_PARAMETER(Flags);
  return Status;
}
// DefaultLocalePrintMonetary
/// Print a monetary value to a character string buffer
/// @param This      The locale protocol
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Monetary  The monetary value
/// @param Width     The minimum count of digits to print
/// @param Precision The maximum count of digits to print after the separator
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintMonetary (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Monetary,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  EFI_STATUS   Status;
  UINT64       Integer = 0;
  UINTN        RemainingSize;
  UINTN        BufferSize = 0;
  UINTN        ThisSize;
  UINTN        Index = 0;
  CHAR8        SignCharacter = 0;
  BOOLEAN      Negative = FALSE;
  // Check parameters
  if ((This == NULL) || (Encoding == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Extract the floating point components
  Status = EfiFloat64Extract(Monetary, &Integer, &Monetary, &Negative);
  if (EFI_ERROR(Status)) {
    // Check for special case error statuses
    if (Status == EFI_NO_MAPPING) {
      // Not a number
      if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE)) {
        mNumber[Index++] = 'N';
        mNumber[Index++] = 'A';
        mNumber[Index++] = 'N';
      } else {
        mNumber[Index++] = 'n';
        mNumber[Index++] = 'a';
        mNumber[Index++] = 'n';
      }
      // No precision needed
      Precision = 0;
      // No sign needed or zeroes padding
      Negative = FALSE;
      Flags &= ~(EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES);
    } else if (Status == EFI_NOT_FOUND) {
      // Infinity
      if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_UPPERCASE)) {
        mNumber[Index++] = 'F';
        mNumber[Index++] = 'N';
        mNumber[Index++] = 'I';
      } else {
        mNumber[Index++] = 'f';
        mNumber[Index++] = 'n';
        mNumber[Index++] = 'i';
      }
      // No precision needed
      Precision = 0;
      // No zeroes padding
      Flags &= ~EFI_LOCALE_PRINT_FLAG_PAD_ZEROES;
    } else {
      // Invalid format
      return Status;
    }
  } else {
    // Zero is a special case
    if (Integer == 0) {
      mNumber[Index++] = '0';
    } else {
      // Get the characters for the integer
      while (Integer != 0) {
        mNumber[Index++] = '0' + (CHAR8)(Integer % 10);
        Integer /= 10;
      }
    }
  }
  // Adjust the width to remove the monetary symbol
  if (Width != 0) {
    --Width;
  }
  // Adjust width to include precision
  if (Precision != 0) {
    if (Width > Precision) {
      Width -= Precision;
    } else {
      Width = 0;
    }
    // Adjust the width to remove the monetary separator
    if (Width != 0) {
      --Width;
    }
  }
  // Check if integer is negative
  if (Negative) {
    SignCharacter = '-';
  } else if (EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN)) {
    // Sign prefix was requested for positive numbers also
    SignCharacter = '+';
  }
  // Decrease the width if there is a sign character
  if ((SignCharacter != 0) && (Width != 0)) {
    --Width;
  }
  // Get the remaining size
  RemainingSize = *Size;
  // Set the minimum width
  if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY | EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
    if (Index < Width) {
      // Right justify by pushing the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Adjust the width for right justification
      Width -= Index;
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, ' ');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
    // To prevent accidental justification later
    Width = 0;
  } 
  // Output the sign character if needed
  if (SignCharacter != 0) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Encode the sign character
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, SignCharacter);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Encode the monetary symbol character
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, EFI_LOCALE_DEFAULT_MONETARY_SYMBOL);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Update the buffer address and size
  if (Buffer != NULL) {
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
  }
  if (RemainingSize > ThisSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Check if zeros need encoded for right justification
  if (Index < Width) {
    // Adjust the width for right justification
    Width -= Index;
    if (EFI_BITS_ARE_UNSET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY) &&
        EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES)) {
      // Right justify by encoding the prefix character for the remaining width
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      // Pad the buffer with zeroes for right justification
      Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, '0');
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
      // To prevent accidental justification later
      Width = 0;
    }
  } else  {
    // To prevent accidental justification later
    Width = 0;
  }
  // Encode the integer into the buffer
  while (Index != 0) {
    // Encode the digit character
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, mNumber[--Index]);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Output fractional part of decimal point if needed
  if (Precision != 0) {
    // Round with precision if possible
    if (Precision <= ARRAY_COUNT(mPrecisionRounders)) {
      Monetary += mPrecisionRounders[Precision];
    }
    // Encode decimal point separator
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, EFI_LOCALE_DEFAULT_SEPARATOR_SYMBOL);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
    // Encode precision for fractional part of floating point
    while (Precision-- != 0) {
      UINT32 Digit;
      // Get the next fraction digit
      Monetary *= 10;
      Digit = (UINT32)Monetary;
      Monetary -= (FLOAT64)Digit;
      // Encode the next fractional character
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, '0' + Digit);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Update the buffer address and size
      if (Buffer != NULL) {
        Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
      }
      if (RemainingSize > ThisSize) {
        RemainingSize -= ThisSize;
      } else {
        RemainingSize = 0;
      }
      // Increase the buffer size
      BufferSize += ThisSize;
    }
  }
  // Add left justification if needed
  if ((Width != 0) && EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY)) {
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Adjust the width for left justification
    Width -= Index;
    // Pad the buffer with spaces for left justification
    Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Width, EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_PAD_ZEROES) ? '0' : ' ');
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Update the buffer address and size
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    if (RemainingSize > ThisSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Increase the buffer size
    BufferSize += ThisSize;
  }
  // Encode the null terminator
  if (RemainingSize > Encoding->UnitSize) {
    ThisSize = RemainingSize;
  } else {
    ThisSize = 0;
  }
  Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Increase the buffer size
  BufferSize += ThisSize;
  // Return buffer size
  *Size = BufferSize;
  return Status;
}
// DefaultLocaleDayOfWeekName
/// Get the default day of the week name
/// @param DayOfWeek The day of the week for which to get the name (0-6)
/// @return The day of the week name
STATIC
CONST CHAR16 *
EFIAPI
DefaultLocaleDayOfWeekName (
  IN UINTN DayOfWeek
) {
  STATIC CONST CHAR16 *DayNames[] = {
    L"Sunday",
    L"Monday",
    L"Tuesday",
    L"Wednesday",
    L"Thursday",
    L"Friday",
    L"Saturday"
  };
  // Check day is valid
  if (DayOfWeek < 7) {
    // Translate day name if needed
    return EfiGetText(DayNames[DayOfWeek]);
  }
  // What?
  return L"";
}
// DefaultLocaleDayName
/// Get the default day name
/// @param Year  The year
/// @param Month The month
/// @param Day   The day of the month for which to get the name (1-31)
/// @return The day name
STATIC
CONST CHAR16 *
EFIAPI
DefaultLocaleDayName (
  IN UINTN Year,
  IN UINTN Month,
  IN UINTN Day
) {
  // Check month
  if ((Month == 0) || (Month > 12)) {
    return L"";
  }
  // Check day
  switch (Day) {

  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
  case 28:
    // Valid day
    break;

  case 29:
    // Valid day for every month but february unless leap year
    if ((Month == 2) && ((Year % 4) == 0) && (((Year % 100) != 0) || ((Year % 400) == 0))) {
      return L"";
    }
    break;

  case 30:
    // Valid day for every month but february
    if (Month == 2) {
      return L"";
    }
    break;

  case 31:
    // Valid day for every month but february, april, june, september, november
    switch (Month) {

    case 2:
    case 4:
    case 6:
    case 9:
    case 11:
      return L"";

    }
    break;

  default:
    // Invalid day
    return L"";

  }
  // Return day of week name
  return DefaultLocaleDayOfWeekName(((Day += ((Month < 3) ? Year-- : Year - 2)), (((23 * Month) / 9) + Day + 4 + (Year / 4) - (Year / 100) + (Year / 400))) % 7);
}
// DefaultLocaleMonthName
/// Get the default month name
/// @param Month The month for which to get the name (1-12)
/// @return The month name
STATIC
CONST CHAR16 *
EFIAPI
DefaultLocaleMonthName (
  IN UINTN Month
) {
  STATIC CONST CHAR16 *MonthNames[] = {
    L"January",
    L"February",
    L"March",
    L"April",
    L"May",
    L"June",
    L"July",
    L"August",
    L"September",
    L"October",
    L"November",
    L"December"
  };
  // Check month is valid
  if ((Month >= 1) && (Month <= 12)) {
    // Translate month name if needed
    return EfiGetText(MonthNames[Month - 1]);
  }
  // What?
  return L"";
}
// DefaultLocalePrintDateTime
/// Print a time interface to a character string buffer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Time     The time interface
/// @param Type     The type of date and/or time printing method (EFI_LOCALE_PRINT_TIME, EFI_LOCALE_PRINT_DATE, EFI_LOCALE_PRINT_LONG_DATE)
/// @param Width    The minimum count of characters to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
STATIC
EFI_STATUS
EFIAPI
DefaultLocalePrintDateTime (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_TIME        *Time,
  IN     UINTN                  Type,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  CONST CHAR8  *Format;
  CONST CHAR16 *MonthName;
  CONST CHAR16 *DayName;
  UINTN         Length;
  UINTN         Hour;
  UINTN         Minute;
  UINTN         PaddingBefore = 0;
  UINTN         PaddingAfter = 0;
  UINTN        *Padding;
  // Check parameters
  if ((This == NULL) || (Encoding == NULL) || (Size == NULL) || (Time == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the timezone offset
  if ((Time->TimeZone >= -1440) && (Time->TimeZone <= 1440)) {
    Hour = Time->TimeZone / 60;
    if (Time->TimeZone < 0) {
      Minute = -Time->TimeZone % 60;
    } else {
      Minute = Time->TimeZone % 60;
    }
  } else {
    Hour = 0;
    Minute = 0;
  }
  // Get the padding to modify
  Padding = EFI_BITS_ANY_SET(Flags, EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY) ? &PaddingAfter : &PaddingBefore;
  // Determine the type of printing
  if (EFI_BITS_ANY_SET(Type, EFI_LOCALE_PRINT_TIME)) {
    if (EFI_BITS_ANY_SET(Type, EFI_LOCALE_PRINT_DATE)) {
      // Print date and time according to locale rules
      if (Time->TimeZone == 0) {
        // UTC timezone
        Format = "%*a%04u-%02u-%02u %02u:%02u:%02u.%03uUTC%*a";
        Length = 26;
      } else if ((Time->TimeZone >= -1440) && (Time->TimeZone <= 1440)) {
        // Timezone offset
        Format = "%*a%04u-%02u-%02u %02u:%02u:%02u.%03u%+02u:%02u%*a";
        Length = 29;
        if (Width > Length) {
          *Padding = Width - Length;
        }
        // Encode the date and time into the string buffer
        return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, Time->Month, Time->Day,
                               Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, Hour, Minute, PaddingAfter, "");
      } else {
        // Local timezone
        Format = "%*a%04u-%02u-%02u %02u:%02u:%02u.%03u%*a";
        Length = 23;
      }
      if (Width > Length) {
        *Padding = Width - Length;
      }
      // Encode the date and time into the string buffer
      return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, Time->Month, Time->Day,
                             Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, PaddingAfter, "");
    } else if (EFI_BITS_ANY_SET(Type, EFI_LOCALE_PRINT_LONG_DATE)) {
      // Print long date and time accoring to locale rules
      MonthName = DefaultLocaleMonthName(Time->Month);
      DayName = DefaultLocaleDayName(Time->Year, Time->Month, Time->Day);
      if (Time->TimeZone == 0) {
        // UTC timezone
        Format = "%*a%04u %s %02u %s %02u:%02u:%02u.%03uUTC%*a";
        Length = 25 + StrLen(MonthName) + StrLen(DayName);
      } else if ((Time->TimeZone >= -1440) && (Time->TimeZone <= 1440)) {
        // Timezone offset
        Format = "%*a%04u %s %02u %s %02u:%02u:%02u.%03u%+02u:%02u%*a";
        Length = 28 + StrLen(MonthName) + StrLen(DayName);
        if (Width > Length) {
          *Padding = Width - Length;
        }
        // Encode the date and time into the string buffer
        return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, MonthName, Time->Day, DayName,
                               Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, Hour, Minute, PaddingAfter, "");
      } else {
        // Local timezone
        Format = "%*a%04u %s %02u %s %02u:%02u:%02u.%03u%*a";
        Length = 22 + StrLen(MonthName) + StrLen(DayName);
      }
      if (Width > Length) {
        *Padding = Width - Length;
      }
      // Encode the date and time into the string buffer
      return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, MonthName, Time->Day, DayName,
                             Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, PaddingAfter, "");
    }
    // Print time only according to locale rules
    if (Time->TimeZone == 0) {
      // UTC timezone
      Format = "%*a%02u:%02u:%02u.%03uUTC%*a";
      Length = 15;
    } else if ((Time->TimeZone >= -1440) && (Time->TimeZone <= 1440)) {
      // Timezone offset
      Format = "%*a%02u:%02u:%02u.%03u%+02u:%02u%*a";
      Length = 18;
      if (Width > Length) {
        *Padding = Width - Length;
      }
      // Encode the time into the string buffer
      return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "",
                              Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, Hour, Minute, PaddingAfter, "");
    } else {
      // Local timezone
      Format = "%*a%02u:%02u:%02u.%03u%*a";
      Length = 12;
    }
    if (Width > Length) {
      *Padding = Width - Length;
    }
    // Encode the time into the string buffer
    return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "",
                            Time->Hour, Time->Minute, Time->Second, Time->Nanosecond / 1000000, PaddingAfter, "");
  } else if (EFI_BITS_ANY_SET(Type, EFI_LOCALE_PRINT_LONG_DATE)) {
    // Print long date only according to locale rules
    Format = "%*a%04u %s %02u %s%*a";
    MonthName = DefaultLocaleMonthName(Time->Month);
    DayName = DefaultLocaleDayName(Time->Year, Time->Month, Time->Day);
    Length = 10 + StrLen(MonthName) + StrLen(DayName);
    if (Width > Length) {
      *Padding = Width - Length;
    }
    return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, MonthName, Time->Day, DayName, PaddingAfter, "");
  } else if (EFI_BITS_ANY_SET(Type, EFI_LOCALE_PRINT_DATE)) {
    // Print date only according to locale rules
    Format = "%*a%04u-%02u-%02u%*a";
    Length = 10;
    if (Width > Length) {
      *Padding = Width - Length;
    }
    return EfiLocaleSPrint(This, Encoding, Buffer, Size, EfiLatin1Encoding(), Format, AsciiStrSize(Format), PaddingBefore, "", Time->Year, Time->Month, Time->Day, PaddingAfter, "");
  }
  // The printing method type was invalid
  return EFI_INVALID_PARAMETER;
}

// DefaultLocaleToBoolean
/// Convert a boolean formatted character string buffer to a boolean
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Boolean  On output, the converted boolean
/// @retval EFI_INVALID_PARAMETER If Locale, Encoding, Buffer, *Buffer, or Boolean is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to a boolean successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToBoolean (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT BOOLEAN                *Boolean
) {
  EFI_STATUS  Status = EFI_SUCCESS;
  VOID       *Ptr;
  UINTN       BufferSize;
  UINT64      Unsigned = 0;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (*Buffer == NULL) || (Boolean == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get buffer and buffer size
  Ptr = *Buffer;
  if (Size != NULL) {
    BufferSize = *Size;
  } else {
    BufferSize = 0;
  }
  // Compare the buffer to boolean true values
  if ((EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "true", AsciiStrSize("true")) == 0) ||
      (EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "on", AsciiStrSize("on")) == 0) ||
      (EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "yes", AsciiStrSize("yes")) == 0)) {
    // This is a true value
    *Boolean = TRUE;
  } else if ((EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "false", AsciiStrSize("false")) == 0) ||
             (EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "off", AsciiStrSize("off")) == 0) ||
             (EfiLocaleICompare(Locale, Encoding, Ptr, BufferSize, EfiLatin1Encoding(), "mo", AsciiStrSize("no")) == 0)) {
    // This is a false value
    *Boolean = FALSE;
  } else {
    // Attempt to convert from a hexadecimal
    Status = EfiLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Unsigned, 16);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    *Boolean = (Unsigned != 0) ? TRUE : FALSE;
  }
  // Return buffer address and buffer size
  *Buffer = Ptr;
  if (Size != NULL) {
    *Size = BufferSize;
  }
  return Status;
}
// DefaultLocaleToUnsigned
/// Convert an unsigned integer formatted character string buffer to an unsigned integer
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Unsigned On output, the converted unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If This, Encoding, Buffer, *Buffer, or Unsigned is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to an unsigned integer successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToUnsigned (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
) {
  EFI_STATUS  Status = EFI_NOT_FOUND;
  VOID       *Ptr;
  UINT64      Value;
  UINTN       BufferSize;
  UINTN       ThisSize;
  UINT32      Character;
  UNUSED_PARAMETER(Locale);
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (*Buffer == NULL) || (Unsigned == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check base is valid
  if (Base < 2) {
    Base = 2;
  } else if (Base > 36) {
    Base = 36;
  }
  // Get the buffer size
  Ptr = *Buffer;
  BufferSize = 0;
  if (Size != NULL) {
    BufferSize = *Size;
  }
  if (BufferSize == 0) {
    Status = EfiEncodingStringSize(Encoding, Ptr, &BufferSize);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  // Decode the value
  Value = 0;
  while (Encoding->UnitSize < BufferSize) {
    // Decode next character
    Character = 0;
    ThisSize = BufferSize;
    Status = EfiDecodeCharacter(Encoding, Ptr, &ThisSize, &Character);
    if (EFI_ERROR(Status)) {
      break;
    }
    // Get the value of the character
    if ((Character >= '0') && (Character <= '9')) {
      Character -= '0';
    } else if ((Character >= 'a') && (Character <= 'z')) {
      Character -= 'a';
      Character += 10;
    } else if ((Character >= 'A') && (Character <= 'Z')) {
      Character -= 'A';
      Character += 10;
    } else {
      // Invalid character
      Character = 0xFFFF;
    }
    // Check the character is valid
    if (Base <= Character) {
      break;
    }
    // Increase the value based on the digit
    Value *= Base;
    Value += Character;
    // Advance the buffer pointer and decrease the remaining size
    Ptr = ADDRESS_OFFSET(VOID, Ptr, ThisSize);
    BufferSize -= ThisSize;
  }
  // Check if no characters were consumed
  if (*Buffer == Ptr) {
    return EFI_NOT_FOUND;
  }
  // Return the buffer pointer, remaining size, and value
  if (!EFI_ERROR(Status)) {
    *Unsigned = Value;
    *Buffer = Ptr;
    if (Size != NULL) {
      *Size = BufferSize;
    }
  }
  return Status;
}
// DefaultLocaleToInteger
/// Convert a signed integer formatted character string buffer to an signed integer
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Integer  On output, the converted signed integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Integer is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to an unsigned integer successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToInteger (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
) {
  EFI_STATUS  Status;
  VOID       *Ptr;
  UINT64      Result;
  UINTN       BufferSize;
  UINTN       ThisSize;
  UINT32      Character;
  BOOLEAN     Negative;
  // Check parameters
  if ((Encoding == NULL) || (Buffer == NULL) || (*Buffer == NULL) || (Integer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Assume zero at first
  Result = 0;
  Negative = FALSE;
  // Decode the first character to check for negative sign
  Ptr = *Buffer;
  BufferSize = 0;
  if (Size != NULL) {
    BufferSize = *Size;
  }
  ThisSize = BufferSize;
  Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, &Character);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Character == '-') {
    Negative = TRUE;
    Ptr = ADDRESS_OFFSET(VOID, Ptr, ThisSize);
    BufferSize -= ThisSize;
  }
  // Convert the string to unsigned integer
  Status = DefaultLocaleToUnsigned(Locale, Encoding, &Ptr, &BufferSize, &Result, Base);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Set new address of buffer and return integer
  *Buffer = Ptr;
  if (Negative) {
    *Integer = -(INT64)Result;
  } else {
    *Integer = (INT64)Result;
  }
  return EFI_SUCCESS;
}
// DefaultLocaleToFloat
/// TODO: Convert a floating point formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToFloat (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  UNUSED_PARAMETER(Locale);
  UNUSED_PARAMETER(Encoding);
  UNUSED_PARAMETER(Buffer);
  UNUSED_PARAMETER(Size);
  UNUSED_PARAMETER(Float);
  return EFI_UNSUPPORTED;
}
// DefaultLocaleToScientific
/// TODO: Convert a scientific formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToScientific (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  UNUSED_PARAMETER(Locale);
  UNUSED_PARAMETER(Encoding);
  UNUSED_PARAMETER(Buffer);
  UNUSED_PARAMETER(Size);
  UNUSED_PARAMETER(Float);
  return EFI_UNSUPPORTED;
}
// DefaultLocaleToMonetary
/// TODO: Convert a monetarily formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToMonetary (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  UNUSED_PARAMETER(Locale);
  UNUSED_PARAMETER(Encoding);
  UNUSED_PARAMETER(Buffer);
  UNUSED_PARAMETER(Size);
  UNUSED_PARAMETER(Float);
  return EFI_UNSUPPORTED;
}
// DefaultLocaleToDateTime
/// TODO: Convert a date and/or time formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param DateTime On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
STATIC
EFI_STATUS
EFIAPI
DefaultLocaleToDateTime (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_TIME               *DateTime
) {
  UNUSED_PARAMETER(Locale);
  UNUSED_PARAMETER(Encoding);
  UNUSED_PARAMETER(Buffer);
  UNUSED_PARAMETER(Size);
  UNUSED_PARAMETER(DateTime);
  return EFI_UNSUPPORTED;
}

// DefaultLocaleUninstall
/// Uninstall default locale protocol
EFI_STATUS
EFIAPI
DefaultLocaleUninstall (
  VOID
) {
  EFI_STATUS             Status;
  EFI_LOCALE_PROTOCOL *Locale = NULL;
  // Check if encoding protocol is installed
  if ((mDefaultLocaleHandle == NULL) || EFI_ERROR(EfiLocateProtocol(&gEfiDefaultLocaleProtocolGuid, NULL, (VOID **)&Locale)) || (Locale == NULL)) {
    return EFI_SUCCESS;
  }
  // Uninstall and free the protocol on error
  Status = EfiUninstallMultipleProtocolInterfaces(mDefaultLocaleHandle, &gEfiDefaultLocaleProtocolGuid, Locale, &gEfiLocaleProtocolGuid, Locale, NULL);
  EfiFreePool((VOID *)Locale);
  return Status;
}
// DefaultLocaleInstall
/// Install default locale protocol
EFI_STATUS
EFIAPI
DefaultLocaleInstall (
  VOID
) {
  EFI_STATUS           Status;
  EFI_LOCALE_PROTOCOL *Locale = NULL;
  // Check if encoding protocol is already installed
  if (!EFI_ERROR(EfiLocateProtocol(&gEfiDefaultLocaleProtocolGuid, NULL, (VOID **)&Locale)) && (Locale != NULL)) {
    return EFI_SUCCESS;
  }
  // Create the encoding protocol
  Locale =
#if defined(EFI_MEMORY_VIRTUAL)
  (EFI_LOCALE_PROTOCOL *)EfiInternalAllocate(sizeof(EFI_LOCALE_PROTOCOL));
#else
  EfiAllocateByType(EFI_LOCALE_PROTOCOL);
#endif
  if (Locale == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Setup encoding protocol
  Locale->Name = "Default";
  Locale->Compare = DefaultLocaleCompare;
  Locale->NCompare = DefaultLocaleNCompare;
  Locale->ICompare = DefaultLocaleICompare;
  Locale->NICompare = DefaultLocaleNICompare;
  Locale->PrintBoolean = DefaultLocalePrintBoolean;
  Locale->ToBoolean = DefaultLocaleToBoolean;
  Locale->PrintUnsigned = DefaultLocalePrintUnsigned;
  Locale->ToUnsigned = DefaultLocaleToUnsigned;
  Locale->PrintInteger = DefaultLocalePrintInteger;
  Locale->ToInteger = DefaultLocaleToInteger;
  Locale->PrintFloat = DefaultLocalePrintFloat;
  Locale->ToFloat = DefaultLocaleToFloat;
  Locale->PrintScientific = DefaultLocalePrintScientific;
  Locale->ToScientific = DefaultLocaleToScientific;
  Locale->PrintMonetary = DefaultLocalePrintMonetary;
  Locale->ToMonetary = DefaultLocaleToMonetary;
  Locale->PrintDateTime = DefaultLocalePrintDateTime;
  Locale->ToDateTime = DefaultLocaleToDateTime;
  // Install the encoding protocol
  Status = EfiInstallMultipleProtocolInterfaces(&mDefaultLocaleHandle, &gEfiDefaultLocaleProtocolGuid, Locale, &gEfiLocaleProtocolGuid, Locale, NULL);
  if (EFI_ERROR(Status)) {
    // Uninstall and free the protocol on error
    EfiUninstallMultipleProtocolInterfaces(mDefaultLocaleHandle, &gEfiDefaultLocaleProtocolGuid, Locale, &gEfiLocaleProtocolGuid, Locale, NULL);
    EfiFreePool((VOID *)Locale);
  }
  return Status;
}
