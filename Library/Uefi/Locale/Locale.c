///
/// @file Library/Uefi/Locale/Locale.c
///
/// Locale protocol
///

#include <Uefi.h>

// TODO: Write documentation for these flags
#define INTERNAL_PRINT_FLAG_WIDTH_AS_ARG EFI_BIT(0)
#define INTERNAL_PRINT_FLAG_PRECISION EFI_BIT(1)
#define INTERNAL_PRINT_FLAG_PRECISION_AS_ARG EFI_BIT(2)
#define INTERNAL_PRINT_FLAG_LEFT_JUSTIFY EFI_BIT(3)
#define INTERNAL_PRINT_FLAG_PAD_ZEROES EFI_BIT(4)
#define INTERNAL_PRINT_FLAG_PREFIX_SIGN EFI_BIT(5)
#define INTERNAL_PRINT_FLAG_PREFIX_HEXADECIMAL EFI_BIT(6)
#define INTERNAL_PRINT_FLAG_PREFIX_OCTAL EFI_BIT(7)
#define INTERNAL_PRINT_FLAG_PREFIX_BINARY EFI_BIT(8)
#define INTERNAL_PRINT_FLAG_SEPARATOR EFI_BIT(9)
#define INTERNAL_PRINT_FLAG_LONG EFI_BIT(10)
#define INTERNAL_PRINT_FLAG_SIGNED EFI_BIT(11)
#define INTERNAL_PRINT_FLAG_FLOAT EFI_BIT(12)
#define INTERNAL_PRINT_FLAG_SCIENTIFIC EFI_BIT(13)
#define INTERNAL_PRINT_FLAG_MONETARY EFI_BIT(14)
#define INTERNAL_PRINT_FLAG_HEXADECIMAL EFI_BIT(15)
#define INTERNAL_PRINT_FLAG_OCTAL EFI_BIT(16)
#define INTERNAL_PRINT_FLAG_BINARY EFI_BIT(17)
#define INTERNAL_PRINT_FLAG_UPPERCASE EFI_BIT(18)
#define INTERNAL_PRINT_FLAG_CHAR EFI_BIT(19)
#define INTERNAL_PRINT_FLAG_BUFFER EFI_BIT(20)
#define INTERNAL_PRINT_FLAG_GUID EFI_BIT(21)
#define INTERNAL_PRINT_FLAG_TIME EFI_BIT(22)
#define INTERNAL_PRINT_FLAG_DATE EFI_BIT(23)

// EfiLocaleProtocolGuid
/// The locale protocol unique identifier
EFI_GUID gEfiLocaleProtocolGuid = EFI_LOCALE_PROTOCOL_GUID;

// DefaultLocaleInstall
/// Install default locale protocol
EXTERN
EFI_STATUS
EFIAPI
DefaultLocaleInstall (
  VOID
);
// DefaultLocaleUninstall
/// Uninstall default locale protocol
EXTERN
EFI_STATUS
EFIAPI
DefaultLocaleUninstall (
  VOID
);

// EfiLocaleUninstall
/// Uninstall locale protocols
/// @retval EFI_SUCCESS The locale protocols were uninstalled
EFI_STATUS
EFIAPI
EfiLocaleUninstall (
  VOID
) {
  // Uninstall the default locale
  DefaultLocaleUninstall();
  return EFI_SUCCESS;
}
// EfiLocaleInstall
/// Install locale protocols
/// @retval EFI_SUCCESS The locale protocols were installed
EFI_STATUS
EFIAPI
EfiLocaleInstall (
  VOID
) {
  EFI_STATUS Status;
  // Install the default locale
  Status = DefaultLocaleInstall();
  return Status;
}

// EfiGetLocale
/// Get a locale protocol interface
/// @param Name The name of the locale or NULL for the current locale
/// @return The locale protocol interface or NULL if not found
EFI_LOCALE_PROTOCOL *
EFIAPI
EfiGetLocale (
  IN CONST CHAR8 *Name OPTIONAL
) {
  EFI_LOCALE_PROTOCOL *Locale = NULL;
  CHAR8               *LocaleName = NULL;
  EFI_HANDLE          *Handles = NULL;
  UINTN                Count = 0;
  UINTN                Index;
  // Check parameters
  if ((Name == NULL) || (*Name == 0)) {
    // Get the current locale name
    if (EFI_ERROR(EfiGetGlobalVariableBuffer(L"PlatformLocale", NULL, NULL, &LocaleName)) || (LocaleName == NULL)) {
      return EfiDefaultLocale();
    }
    // Check the name is valid
    if (*LocaleName == 0) {
      EfiFreePool(LocaleName);
      return EfiDefaultLocale();
    }
    // Set the name to the current locale name
    Name = LocaleName;
  }
  // Find the locale protocol by name
  if (!EFI_ERROR(EfiLocateHandleBuffer(ByProtocol, &gEfiLocaleProtocolGuid, NULL, &Count, &Handles)) && (Handles != NULL)) {
    // On first pass try to exact match
    for (Index = 0; Index < Count; ++Index) {
      EFI_LOCALE_PROTOCOL *Protocol = NULL;
      // Get each locale protocol
      if (EFI_ERROR(EfiHandleProtocol(Handles[Index], &gEfiLocaleProtocolGuid, (VOID **)&Protocol))) {
        continue;
      }
      if (Protocol == NULL) {
        continue;
      }
      // Check if this locale name matches
      if (AsciiStriCmp(Name, Protocol->Name) == 0) {
        Locale = Protocol;
        break;
      }
    }
  }
  // Free current locale name if needed
  if (LocaleName != NULL) {
    EfiFreePool(LocaleName);
  }
  // Return the locale if found
  return Locale;
}
// EfiSetLocale
/// Set the current locale protocol interface
/// @param Name The name of the locale or NULL to restore the default locale
/// @retval EFI_NOT_FOUND The locale was not found
/// @retval EFI_SUCCESS   The locale was set
EFI_STATUS
EFIAPI
EfiSetLocale (
  IN CONST CHAR8 *Name OPTIONAL
) {
  return EfiSetVariable(L"PlatformLocale", &gEfiGlobalVariableGuid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS, AsciiStrSize(Name), (VOID *)Name);
}

// EfiLocalePrintSize
/// Get the size in bytes of a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The size in bytes of the formatted character string
UINTN
EFIAPI
EfiLocalePrintSize (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, FormatSize);
  Length = EfiLocaleVPrintSize(Locale, Encoding, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return Length;
}
// EfiLocaleVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The size in bytes of the formatted character string
UINTN
EFIAPI
EfiLocaleVPrintSize (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  IN VA_LIST                Marker
) {
  EFI_STATUS Status;
  UINTN      Size = 0;
  // Get size needed for buffer
  Status = EfiLocaleVSPrint(Locale, Encoding, NULL, &Size, FormatEncoding, Format, FormatSize, Marker);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return 0;
  }
  return Size;
}

// EfiLocalePoolPrint
/// Allocate a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param Size           On output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
VOID *
EFIAPI
EfiLocalePoolPrint (
  IN  EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  ...
) {
  VOID    *String;
  VA_LIST  Marker;
  VA_START(Marker, FormatSize);
  String = EfiLocalePoolVPrint(Locale, Encoding, Size, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return String;
}
// EfiLocalePoolVPrint
/// Allocate a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param Size           On output, the size in bytes of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
VOID *
EFIAPI
EfiLocalePoolVPrint (
  IN  EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  VA_LIST                    Marker
) {
  VOID  *String;
  // Get the size of the formatted string
  UINTN  BufferSize = EfiLocaleVPrintSize(Locale, Encoding, FormatEncoding, Format, FormatSize, Marker);
  if (BufferSize == 0) {
    return NULL;
  }
  // Allocate the string buffer
  String = EfiAllocate(BufferSize);
  if (String == NULL) {
    return NULL;
  }
  // Print the formatted string
  if (EFI_ERROR(EfiLocaleVSPrint(Locale, Encoding, String, &BufferSize, FormatEncoding, Format, FormatSize, Marker))) {
    // Cleanup on error
    EfiFreePool(String);
    return NULL;
  }
  if (Size != NULL) {
    *Size = BufferSize;
  }
  // Return the string
  return String;
}

// EfiLocaleSPrint
/// Print a formatted character string to a character string buffer
/// @param Locale         The locale to use in formatting or NULL for the current locale
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
EfiLocaleSPrint (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
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
  Status = EfiLocaleVSPrint(Locale, Encoding, Buffer, Size, FormatEncoding, Format, FormatSize, Marker);
  VA_END(Marker);
  return Status;
}
// EfiLocaleVSPrint
/// Print a formatted character string to a character string buffer
/// @param Locale         The locale to use in formatting or NULL for the current locale
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
EfiLocaleVSPrint (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN     CONST VOID            *Format,
  IN     UINTN                  FormatSize,
  IN     VA_LIST                Marker
) {
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  UINTN      BufferSize = 0;
  UINTN      RemainingSize;
  UINTN      ThisSize;
  UINTN      Precision = 0;
  UINTN      Width = 0;
  UINTN      Flags = 0;
  BOOLEAN    Argument = FALSE;
  BOOLEAN    CarriageReturn = FALSE;
  // Check parameters
  if ((Encoding == NULL) || (Size == NULL) || (FormatEncoding == NULL) || (Format == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the remaining size
  RemainingSize = *Size;
  // Get the current locale if needed
  if (Locale == NULL) {
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Parse the format buffer
  while (FormatSize >= FormatEncoding->UnitSize) {
    EFI_ENCODING_PROTOCOL *ArgumentEncoding = NULL;
    UINT64                 ArgumentValue = 0;
    VOID                  *ArgumentBuffer = NULL;
    UINTN                  ArgumentSize = 0;
    UINT32                 Character = 0;
    // Decode the next character from the format buffer
    ThisSize = FormatSize;
    Status = EfiDecodeCharacter(FormatEncoding, Format, &ThisSize, &Character);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    // Check to make sure that this is not a null terminator character
    if (Character == 0) {
      break;
    }
    // Decrease the format buffer size
    FormatSize -= ThisSize;
    // Advance the format buffer
    Format = ADDRESS_OFFSET(VOID, Format, ThisSize);
    // Check if processing an argument
    if (Argument) {
      // Argument processing
      switch (Character) {

        default:
          // Unknown character for argument so skip and return to normal processing
          Argument = FALSE;
          Precision = 0;
          Width = 0;
          Flags = 0;
          continue;

        //
        // Argument flags
        //

        case '-':
          // Left justify instead of right justify
          Flags |= INTERNAL_PRINT_FLAG_LEFT_JUSTIFY;
          continue;

        case ' ':
        case '\t':
        case '\r':
        case '\n':
          // Skip any whitespace
          continue;

        case '+':
          // Prefix integer sign (+ or -)
          Flags |= INTERNAL_PRINT_FLAG_PREFIX_SIGN;
          continue;

        case ',':
          // Separate integer digits into groups of three
          Flags |= INTERNAL_PRINT_FLAG_SEPARATOR;
          continue;

        case 'L':
        case 'l':
          // Force long integer instead of normal integer
          Flags |= INTERNAL_PRINT_FLAG_LONG;
          continue;

        case '0':

          // If no width or precision yet, then prefix zeroes
          if ((Width == 0) && EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION)) {
            // Pad with zeroes instead of spaces
            Flags |= INTERNAL_PRINT_FLAG_PAD_ZEROES;
          } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PRECISION)) { 
            // Precision
            Precision *= 10;
          } else {
            // Width
            Width *= 10;
          }
          continue;

        //
        // Width and precision
        //

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          // Width or precision
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PRECISION)) {
            // Precision
            Precision *= 10;
            Precision += (Character - '0');
          } else {
            // Width
            Width *= 10;
            Width += (Character - '0');
          }
          continue;

        case '*':
          // The width and/or precision are the arguments before this argument
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PRECISION)) {
            // Get the precision from the arguments
            if (EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION_AS_ARG)) {
              Precision = VA_ARG(Marker, UINTN);
              Flags |= INTERNAL_PRINT_FLAG_PRECISION_AS_ARG;
            }
          } else if (EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_WIDTH_AS_ARG)) {
            // Get the width from the arguments
            Width = VA_ARG(Marker, UINTN);
            Flags |= INTERNAL_PRINT_FLAG_WIDTH_AS_ARG;
          }
          continue;

        case '.':
          // Switch from width field parsing to precision field parsing
          Flags |= INTERNAL_PRINT_FLAG_PRECISION;
          continue;

        //
        // Argument type
        //

        case '%':
          // Percent sign
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= INTERNAL_PRINT_FLAG_CHAR;
          break;

        case 'c':
        case 'C':
          // Unicode character
          Character = VA_ARG(Marker, UINT32);
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= INTERNAL_PRINT_FLAG_CHAR;
          break;

        case 'p':
          // Lowercase pointer argument
          Flags |= INTERNAL_PRINT_FLAG_PREFIX_HEXADECIMAL;

        case 'h':
          // Lowercase hexadecimal with default integer width
          if (Width == 0) {
            if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
              Width = (sizeof(UINT64) << 1);
            } else {
              Width = (sizeof(UINTN) << 1);
            }
          }
          Flags |= INTERNAL_PRINT_FLAG_PAD_ZEROES;

        case 'x':
          // Lowercase hexadecimal
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = VA_ARG(Marker, UINT64);
          } else {
            ArgumentValue = (UINT64)VA_ARG(Marker, UINTN);
          }
          Flags &= ~INTERNAL_PRINT_FLAG_PREFIX_SIGN;
          Flags |= INTERNAL_PRINT_FLAG_HEXADECIMAL;
          break;

        case 'P':
          // Uppercase pointer argument
          Flags |= INTERNAL_PRINT_FLAG_PREFIX_HEXADECIMAL;

        case 'H':
          // Uppercase hexadecimal with default integer width
          if (Width == 0) {
            if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
              Width = (sizeof(UINT64) << 1);
            } else {
              Width = (sizeof(UINTN) << 1);
            }
          }
          Flags |= INTERNAL_PRINT_FLAG_PAD_ZEROES;

        case 'X':
          // Uppercase hexadecimal
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = VA_ARG(Marker, UINT64);
          } else {
            ArgumentValue = (UINT64)VA_ARG(Marker, UINTN);
          }
          Flags &= ~INTERNAL_PRINT_FLAG_PREFIX_SIGN;
          Flags |= (INTERNAL_PRINT_FLAG_UPPERCASE | INTERNAL_PRINT_FLAG_HEXADECIMAL);
          break;

        case 'O':
          // Octal integer argument with prefix
          Flags |= INTERNAL_PRINT_FLAG_PREFIX_OCTAL;

        case 'o':
          // Octal integer argument
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = VA_ARG(Marker, UINT64);
          } else {
            ArgumentValue = (UINT64)VA_ARG(Marker, UINTN);
          }
          Flags |= INTERNAL_PRINT_FLAG_OCTAL;
          break;

        case 'B':
          // Binary integer argument with prefix
          Flags |= INTERNAL_PRINT_FLAG_PREFIX_BINARY;

        case 'b':
          // Binary integer argument
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = VA_ARG(Marker, UINT64);
          } else {
            ArgumentValue = (UINT64)VA_ARG(Marker, UINTN);
          }
          Flags |= INTERNAL_PRINT_FLAG_BINARY;
          break;

        case 'd':
        case 'i':
          // Signed decimal integer argument
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = (UINT64)VA_ARG(Marker, INT64);
          } else {
            ArgumentValue = (UINT64)(INT64)VA_ARG(Marker, INTN);
          }
          Flags |= INTERNAL_PRINT_FLAG_SIGNED;
          break;

        case 'u':
          // Unsigned decimal integer argument
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LONG)) {
            ArgumentValue = VA_ARG(Marker, UINT64);
          } else {
            ArgumentValue = (UINT64)VA_ARG(Marker, UINTN);
          }
          break;

        case 'F':
          // Uppercase floating point
          Flags |= INTERNAL_PRINT_FLAG_UPPERCASE;

        case 'f':
          // Floating point
          ArgumentBuffer = (VOID *)VA_ADDRESS(Marker, FLOAT64);
          Flags |= INTERNAL_PRINT_FLAG_FLOAT;
          break;

        case 'E':
          // Uppercase scientific floating point
          Flags |= INTERNAL_PRINT_FLAG_UPPERCASE;

        case 'e':
          // Scientific floating point
          ArgumentBuffer = (VOID *)VA_ADDRESS(Marker, FLOAT64);
          Flags |= INTERNAL_PRINT_FLAG_SCIENTIFIC;
          break;

        case 'm':
          // Monetary floating point
          ArgumentBuffer = (VOID *)VA_ADDRESS(Marker, FLOAT64);
          Flags |= INTERNAL_PRINT_FLAG_MONETARY;
          break;

        case 'a':
          // Latin-1 string argument
          ArgumentEncoding = EfiLatin1Encoding();
          ArgumentBuffer = VA_ARG(Marker, VOID *);
          Status = EfiEncodingStringSize(ArgumentEncoding, ArgumentBuffer, &ArgumentSize);
          if (EFI_ERROR(Status)) {
            Argument = FALSE;
            continue;
          }
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN);
          Flags |= INTERNAL_PRINT_FLAG_BUFFER;
          break;

        case 'A':
          // UTF-8 string argument
          ArgumentEncoding = EfiUtf8Encoding();
          ArgumentBuffer = VA_ARG(Marker, VOID *);
          Status = EfiEncodingStringSize(ArgumentEncoding, ArgumentBuffer, &ArgumentSize);
          if (EFI_ERROR(Status)) {
            Argument = FALSE;
            continue;
          }
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN);
          Flags |= INTERNAL_PRINT_FLAG_BUFFER;
          break;

        case 's':
          // Unicode string argument
          ArgumentEncoding = EfiUtf16Encoding();
          ArgumentBuffer = VA_ARG(Marker, VOID *);
          Status = EfiEncodingStringSize(ArgumentEncoding, ArgumentBuffer, &ArgumentSize);
          if (EFI_ERROR(Status)) {
            Argument = FALSE;
            continue;
          }
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN);
          Flags |= INTERNAL_PRINT_FLAG_BUFFER;
          break;

        case 'S':
          // Encoding argument and encoded string argument
          ArgumentEncoding = VA_ARG(Marker, EFI_ENCODING_PROTOCOL *);
          ArgumentBuffer = VA_ARG(Marker, VOID *);
          Status = EfiEncodingStringSize(ArgumentEncoding, ArgumentBuffer, &ArgumentSize);
          if (EFI_ERROR(Status)) {
            Argument = FALSE;
            continue;
          }
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN);
          Flags |= INTERNAL_PRINT_FLAG_BUFFER;
          break;

        case 'r':
          // Status code argument
          ArgumentEncoding = EfiLatin1Encoding();
          ArgumentBuffer = (VOID *)AsciiStrFromStatus(VA_ARG(Marker, EFI_STATUS));
          Status = EfiEncodingStringSize(ArgumentEncoding, ArgumentBuffer, &ArgumentSize);
          if (EFI_ERROR(Status)) {
            Argument = FALSE;
            continue;
          }
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN);
          Flags |= INTERNAL_PRINT_FLAG_BUFFER;
          break;

        case 'G':
          // Uppercase GUID argument
          Flags |= INTERNAL_PRINT_FLAG_UPPERCASE;

        case 'g':
          // GUID argument
          ArgumentBuffer = (VOID *)VA_ARG(Marker, EFI_GUID *);
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= INTERNAL_PRINT_FLAG_GUID;
          break;

        case 't':
          // Time argument
          ArgumentBuffer = (VOID *)VA_ARG(Marker, EFI_TIME *);
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= INTERNAL_PRINT_FLAG_TIME;
          break;

        case 'D':
          // Date argument
          ArgumentBuffer = (VOID *)VA_ARG(Marker, EFI_TIME *);
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= INTERNAL_PRINT_FLAG_DATE;
          break;

        case 'T':
          // Date and time argument
          ArgumentBuffer = (VOID *)VA_ARG(Marker, EFI_TIME *);
          Flags &= ~(INTERNAL_PRINT_FLAG_PAD_ZEROES | INTERNAL_PRINT_FLAG_PREFIX_SIGN | INTERNAL_PRINT_FLAG_PRECISION);
          Flags |= (INTERNAL_PRINT_FLAG_DATE | INTERNAL_PRINT_FLAG_TIME);
          break;

      }
    } else if (Character == '\n') {
      // Translate LF to CR+LF if needed
      if (CarriageReturn) {
        CarriageReturn = FALSE;
      } else {
        if (RemainingSize > Encoding->UnitSize) {
          ThisSize = RemainingSize;
        } else {
          ThisSize = 0;
        }
        Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, '\r');
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
      }
    } else if (CarriageReturn) {
      // Translate CR to CR+LF if needed
      if (RemainingSize > Encoding->UnitSize) {
        ThisSize = RemainingSize;
      } else {
        ThisSize = 0;
      }
      Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, '\n');
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
      // Still remember carriage return for translation
      CarriageReturn = (Character == '\r') ? TRUE : FALSE;
    } else if (Character == '%') {
      // Switch to argument processing
      Argument = TRUE;
      Precision = 0;
      Width = 0;
      Flags = 0;
      continue;
    } else {
      // Remember carriage return for translation
      CarriageReturn = (Character == '\r') ? TRUE : FALSE;
    }
    // Set the remaining size for the argument or character in the buffer if any
    if (RemainingSize > Encoding->UnitSize) {
      ThisSize = RemainingSize;
    } else {
      ThisSize = 0;
    }
    // Check if there is an argument buffer
    if (Argument) {
      UINTN  Padding = 0;
      UINT32 PadCharacter = EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PAD_ZEROES) ? '0' : ' ';
      // Print argument
      if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_CHAR | INTERNAL_PRINT_FLAG_BUFFER | INTERNAL_PRINT_FLAG_GUID)) {
        // Character or buffer argument
        if (Width != 0) {
          UINTN Length = 0;
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_GUID)) {
            // The length of the GUID is 36 characters for 16 bytes of hexadecimal and four separators
            Length = 36;
          } else {
            if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_BUFFER)) {
              // Get the length of the argument buffer
              Length = ArgumentSize;
              Status = EfiEncodingStringLength(ArgumentEncoding, ArgumentBuffer, &Length);
              if (EFI_ERROR(Status)) {
                return Status;
              }
            } else {
              // Just one character
              Length = 1;
            }
            // Check the length is less than the precision
            if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PRECISION) && (Length > Precision)) {
              Length = Precision;
            }
          }
          // Get the padding size for the argument
          if (Width > Length) {
            Padding = Width - Length;
          }
        }
        // Pad for right justification
        if ((Padding != 0) && EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_LEFT_JUSTIFY)) {
          // Encode the padding character into the output buffer
          Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Padding, PadCharacter);
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
          if (RemainingSize > Encoding->UnitSize) {
            ThisSize = RemainingSize;
          } else {
            ThisSize = 0;
          }
        }
        // Encode the characters from the format buffer into the output buffer
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_CHAR)) {
          // Character buffer argument
          if (EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION) || (Precision != 0)) {
            Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, Character);
          }
        } else {
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_BUFFER)) {
            // String buffer argument
            if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PRECISION)) {
              if (Precision != 0) {
                // Convert the argument buffer into the output buffer with a maximum count
                Status = EfiEncodingConvertCount(Encoding, Buffer, &ThisSize, ArgumentEncoding, ArgumentBuffer, ArgumentSize, Precision);
              }
            } else {
              // Convert the entire argument buffer into the output buffer
              Status = EfiEncodingConvert(Encoding, Buffer, &ThisSize, ArgumentEncoding, ArgumentBuffer, ArgumentSize);
            }
          } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_GUID)) {
            // GUID argument - precision does not apply to this argument type!
            Status = EfiEncodingPrintGuid(Encoding, Buffer, &ThisSize, (EFI_GUID *)ArgumentBuffer, EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_UPPERCASE));
          }
          // Do not include the null terminator until the end of the output
          if (ThisSize >= Encoding->UnitSize) {
            ThisSize -= Encoding->UnitSize;
          }
        }
        // Pad for left justification
        if ((Padding != 0) && EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LEFT_JUSTIFY)) {
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
          // Encode the padding character into the output buffer
          if (RemainingSize > Encoding->UnitSize) {
            ThisSize = RemainingSize;
          } else {
            ThisSize = 0;
          }
          Status = EfiFillCharacter(Encoding, Buffer, &ThisSize, Padding, PadCharacter);
        }
      } else {
        UINTN Base;
        UINTN ArgumentFlags = 0;
        // Determine the argument print flags
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_UPPERCASE)) {
          ArgumentFlags |= EFI_LOCALE_PRINT_FLAG_UPPERCASE;
        }
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_LEFT_JUSTIFY)) {
          ArgumentFlags |= EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY;
        }
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PAD_ZEROES)) {
          ArgumentFlags |= EFI_LOCALE_PRINT_FLAG_PAD_ZEROES;
        }
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_PREFIX_SIGN)) {
          ArgumentFlags |= EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN;
        }
        // Determine the base of the argument
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_HEXADECIMAL)) {
          Base = 16;
        } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_OCTAL)) {
          Base = 8;
        } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_BINARY)) {
          Base = 2;
        } else {
          Base = 10;
        }
        // Encode the argument into the output buffer
        if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_TIME | INTERNAL_PRINT_FLAG_DATE)) {
          // Time and/or date argument
          UINTN Type = EFI_LOCALE_PRINT_TIME | EFI_LOCALE_PRINT_DATE;
          if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_TIME)) {
            if (EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_DATE)) {
              Type = EFI_LOCALE_PRINT_TIME;
            }
          } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_DATE)) {
            Type = EFI_LOCALE_PRINT_DATE;
          }
          Status = EfiLocalePrintDateTime(Locale, Encoding, Buffer, &ThisSize, (EFI_TIME *)ArgumentBuffer, Type, Width, ArgumentFlags);
        } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_FLOAT)) {
          // Floating point argument
          Status = EfiLocalePrintFloat(Locale, Encoding, Buffer, &ThisSize, *((FLOAT64 *)ArgumentBuffer), Width, EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION) ? EFI_LOCALE_PRINT_FLOAT_DEFAULT_PRECISION : Precision, ArgumentFlags);
        } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_SCIENTIFIC)) {
          // Floating point in scientific notation argument
          Status = EfiLocalePrintScientific(Locale, Encoding, Buffer, &ThisSize, *((FLOAT64 *)ArgumentBuffer), Width, EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION) ? EFI_LOCALE_PRINT_FLOAT_DEFAULT_PRECISION : Precision, ArgumentFlags);
        } else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_MONETARY)) {
          // Floating point in monetary notation argument
          Status = EfiLocalePrintMonetary(Locale, Encoding, Buffer, &ThisSize, *((FLOAT64 *)ArgumentBuffer), Width, EFI_BITS_ARE_UNSET(Flags, INTERNAL_PRINT_FLAG_PRECISION) ? EFI_LOCALE_PRINT_FLOAT_DEFAULT_PRECISION : Precision, ArgumentFlags);
        //} else if (EFI_BITS_ANY_SET(Flags, INTERNAL_PRINT_FLAG_SIGNED)) {
        //  // Signed integer argument
        //  Status = EfiLocalePrintInteger(Locale, Encoding, Buffer, &ThisSize, (INT64)ArgumentValue, Base, Width, ArgumentFlags);
        } else {
          // Unsigned integer argument
          Status = EfiLocalePrintUnsigned(Locale, Encoding, Buffer, &ThisSize, ArgumentValue, Base, Width, ArgumentFlags);
        }
        // Do not include the null terminator until the end of the output
        if (ThisSize >= Encoding->UnitSize) {
          ThisSize -= Encoding->UnitSize;
        }
      }
      // Switch back to normal processing
      Argument = FALSE;
      Precision = 0;
      Width = 0;
      Flags = 0;
    } else {
      // Encode the character from the format buffer into the output buffer
      Status = EfiEncodeCharacter(Encoding, Buffer, &ThisSize, Character);
    }
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
  }
  // Encode the null-terminator into the output buffer
  if (RemainingSize >= Encoding->UnitSize) {
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
  // Set the new buffer size
  *Size = BufferSize;
  return Status;
}

// EfiLocaleCompare
/// Compare two encoded character strings
/// @param Locale      The locale to use in formatting or NULL for the current locale
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
EfiLocaleCompare (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->Compare == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->Compare(Locale, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2);
}
// EfiLocaleNCompare
/// Compare two encoded character strings with a maximum count
/// @param Locale      The locale to use in formatting or NULL for the current locale
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
EXTERN
INTN
EFIAPI
EfiLocaleNCompare (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->NCompare == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->NCompare(Locale, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2, Count);
}
// EfiLocaleICompare
/// Compare two encoded character strings case insensitively
/// @param Locale      The locale to use in formatting or NULL for the current locale
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
EfiLocaleICompare (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ICompare == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ICompare(Locale, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2);
}
// EfiLocaleNICompare
/// Compare two encoded character strings case insensitively with a maximum count
/// @param Locale      The locale to use in formatting or NULL for the current locale
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
EXTERN
INTN
EFIAPI
EfiLocaleNICompare (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->NICompare == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->NICompare(Locale, Encoding1, Buffer1, BufferSize1, Encoding2, Buffer2, BufferSize2, Count);
}

// EfiLocalePrintBoolean
/// Print a boolean to a character string buffer
/// @param Locale   The locale to use in formatting or NULL for the current locale
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
EfiLocalePrintBoolean (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     BOOLEAN                Boolean,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintBoolean == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintBoolean(Locale, Encoding, Buffer, Size, Boolean, Width, Flags);
}
// EfiLocalePrintUnsigned
/// Print an unsigned integer to a character string buffer
/// @param Locale   The locale to use in formatting or NULL for the current locale
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
EfiLocalePrintUnsigned (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT64                 Unsigned,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintUnsigned == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintUnsigned(Locale, Encoding, Buffer, Size, Unsigned, Base, Width, Flags);
}
// EfiLocalePrintInteger
/// Print an unsigned integer to a character string buffer
/// @param Locale   The locale to use in formatting or NULL for the current locale
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Integer  The unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiLocalePrintInteger (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     INT64                  Integer,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintInteger == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintInteger(Locale, Encoding, Buffer, Size, Integer, Base, Width, Flags);
}
// EfiLocalePrintFloat
/// Print a floating point number to a character string buffer
/// @param Locale    The locale to use in formatting or NULL for the current locale
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
EfiLocalePrintFloat (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintFloat == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintFloat(Locale, Encoding, Buffer, Size, Float, Width, Precision, Flags);
}
// EfiLocalePrintScientific
/// Print a scientific floating point number to a character string buffer
/// @param Locale       The locale to use in formatting or NULL for the current locale
/// @param Encoding     The encoding of the formatted character string
/// @param Buffer       On output, the formatted character string
/// @param Size         On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Float        The floating point value
/// @param Width        The minimum count of digits to print
/// @param Significance The maximum significant digits
/// @param Flags        The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiLocalePrintScientific (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Significance,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintScientific == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintScientific(Locale, Encoding, Buffer, Size, Float, Width, Significance, Flags);
}
// EfiLocalePrintMonetary
/// Print a monetary value to a character string buffer
/// @param Locale    The locale to use in formatting or NULL for the current locale
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
EfiLocalePrintMonetary (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Monetary,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintMonetary == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintMonetary(Locale, Encoding, Buffer, Size, Monetary, Width, Precision, Flags);
}
// EfiLocalePrintDateTime
/// Print a time interface to a character string buffer
/// @param Locale   The locale to use in formatting or NULL for the current locale
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Time     The time interface
/// @param Type     The type of date and/or time printing method (EFI_LOCALE_PRINT_TIME, EFI_LOCALE_PRINT_DATE, EFI_LOCALE_PRINT_LONG_DATE)
/// @param Width    The minimum count of characters to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiLocalePrintDateTime (
  IN     EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_TIME        *Time,
  IN     UINTN                  Type,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->PrintDateTime == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->PrintDateTime(Locale, Encoding, Buffer, Size, Time, Type, Width, Flags);
}

// EfiLocaleToBoolean
/// Convert a boolean formatted character string buffer to a boolean
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Boolean  On output, the converted boolean
/// @retval EFI_INVALID_PARAMETER If Locale, Encoding, Buffer, *Buffer, or Boolean is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to a boolean successfully
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToBoolean (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT BOOLEAN                *Boolean
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToBoolean == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToBoolean(Locale, Encoding, Buffer, Size, Boolean);
}
// EfiLocaleToUnsigned
/// Convert an unsigned integer formatted character string buffer to an unsigned integer
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Unsigned On output, the converted unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If Locale, Encoding, Buffer, *Buffer, or Unsigned is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to an unsigned integer successfully
EFI_STATUS
EFIAPI
EfiLocaleToUnsigned (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToUnsigned == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToUnsigned(Locale, Encoding, Buffer, Size, Unsigned, Base);
}
// EfiLocaleToInteger
/// Convert a signed integer formatted character string buffer to an signed integer
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Integer  On output, the converted signed integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER  If Locale, Encoding, Buffer, *Buffer, or Integer is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to an unsigned integer successfully
EFI_STATUS
EFIAPI
EfiLocaleToInteger (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToInteger == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToInteger(Locale, Encoding, Buffer, Size, Integer, Base);
}
// EfiLocaleToFloat
/// Convert a floating point formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If Locale, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiLocaleToFloat (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToFloat == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToFloat(Locale, Encoding, Buffer, Size, Float);
}
// EfiLocaleToScientific
/// Convert a scientific formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If Locale, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiLocaleToScientific (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToScientific == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToScientific(Locale, Encoding, Buffer, Size, Float);
}
// EfiLocaleToMonetary
/// Convert a monetarily formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If Locale, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiLocaleToMonetary (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToMonetary == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToMonetary(Locale, Encoding, Buffer, Size, Float);
}
// EfiLocaleToDateTime
/// Convert a date and/or time formatted character string buffer to a floating point
/// @param Locale   The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param DateTime On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If Locale, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EFI_STATUS
EFIAPI
EfiLocaleToDateTime (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_TIME               *DateTime
) {
  // Check for locale
  if (Locale == NULL) {
    // Get current locale
    Locale = EfiGetLocale(NULL);
    if (Locale == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Check for method
  if (Locale->ToDateTime == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Call print method
  return Locale->ToDateTime(Locale, Encoding, Buffer, Size, DateTime);
}
