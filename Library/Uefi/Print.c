///
/// @file Library/Uefi/Print.c
///
/// UEFI print to string and console utilities
///

#include <Uefi.h>

//
// Unicode UTF-16 print helper functions
//

// EfiPrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiPrintSize (
  IN CONST CHAR16 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiVPrintSize(Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiVPrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiVPrintSize (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf16Encoding();
  return EfiLocaleVPrintSize(NULL, Encoding, Encoding, Format, StrSize(Format), Marker);
}

// EfiPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiPrint (
  IN CONST CHAR16 *Format,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Marker;
  VA_START(Marker, Format);
  Status = EfiVPrint(Format, Marker);
  VA_END(Marker);
  return Status;
}
// EfiVPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiVPrint (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
) {
  EFI_STATUS  Status;
  // Create formatted character string
  CHAR16     *String = EfiPoolVPrint(Format, Marker);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Print to console
  Status = EfiConOutPrint(String);
  // Free string
  EfiFreePool((VOID *)String);
  // Return status
  return Status;
}

// EfiPoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR16 *
EFIAPI
EfiPoolPrint (
  IN CONST CHAR16 *Format,
  ...
) {
  CHAR16  *String;
  VA_LIST  Marker;
  VA_START(Marker, Format);
  String = EfiPoolVPrint(Format, Marker);
  VA_END(Marker);
  return String;
}
// EfiPoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR16 *
EFIAPI
EfiPoolVPrint (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf16Encoding();
  return EfiLocalePoolVPrint(NULL, Encoding, NULL, Encoding, (VOID  *)Format, StrSize(Format), Marker);
}

// EfiSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiSPrint (
  OUT    CHAR16       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR16 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiVSPrint(Buffer, Size, Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiVSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiVSPrint (
  OUT    CHAR16       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR16 *Format,
  IN     VA_LIST       Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf16Encoding();
  return EfiLocaleVSPrint(NULL, Encoding, (VOID *)Buffer, Size, Encoding, (VOID *)Format, StrSize(Format), Marker);
}

//
// Unicode UTF-8 print helper functions
//

// EfiUtf8PrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiUtf8PrintSize (
  IN CONST CHAR8 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiUtf8VPrintSize(Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiUtf8VPrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiUtf8VPrintSize (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf8Encoding();
  return EfiLocaleVPrintSize(NULL, Encoding, Encoding, Format, AsciiStrSize(Format), Marker);
}

// EfiUtf8Print
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiUtf8Print (
  IN CONST CHAR8 *Format,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Marker;
  VA_START(Marker, Format);
  Status = EfiUtf8VPrint(Format, Marker);
  VA_END(Marker);
  return Status;
}
// EfiUtf8VPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiUtf8VPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_STATUS  Status;
  // Create formatted character string
  CHAR16     *String = (CHAR16 *)EfiEncodingPoolVPrint(EfiUtf16Encoding(), NULL, EfiUtf8Encoding(), Format, AsciiStrSize(Format), Marker);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Print to console
  Status = EfiConOutPrint(String);
  // Free string
  EfiFreePool((VOID *)String);
  // Return status
  return Status;
}

// EfiUtf8PoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR8 *
EFIAPI
EfiUtf8PoolPrint (
  IN CONST CHAR8 *Format,
  ...
) {
  CHAR8   *String;
  VA_LIST  Marker;
  VA_START(Marker, Format);
  String = EfiUtf8PoolVPrint(Format, Marker);
  VA_END(Marker);
  return String;
}
// EfiUtf8PoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR8 *
EFIAPI
EfiUtf8PoolVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf8Encoding();
  return EfiLocalePoolVPrint(NULL, Encoding, NULL, Encoding, (VOID  *)Format, AsciiStrSize(Format), Marker);
}

// EfiUtf8SPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiUtf8SPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiUtf8VSPrint(Buffer, Size, Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiUtf8VSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiUtf8VSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  IN     VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiUtf8Encoding();
  return EfiLocaleVSPrint(NULL, Encoding, (VOID *)Buffer, Size, Encoding, (VOID *)Format, AsciiStrSize(Format), Marker);
}

//
// Latin-1/ASCII print helper functions
//

// EfiAsciiPrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiAsciiPrintSize (
  IN CONST CHAR8 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiAsciiVPrintSize(Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiAsciiVPrintSize
/// Get the length of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
UINTN
EFIAPI
EfiAsciiVPrintSize (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiLatin1Encoding();
  return EfiLocaleVPrintSize(NULL, Encoding, Encoding, Format, AsciiStrSize(Format), Marker);
}

// EfiAsciiPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiAsciiPrint (
  IN CONST CHAR8 *Format,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Marker;
  VA_START(Marker, Format);
  Status = EfiAsciiVPrint(Format, Marker);
  VA_END(Marker);
  return Status;
}
// EfiAsciiVPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The number of characters printed
UINTN
EFIAPI
EfiAsciiVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_STATUS  Status;
  // Create formatted character string
  CHAR16    *String = (CHAR16 *)EfiEncodingPoolVPrint(EfiUtf16Encoding(), NULL, EfiLatin1Encoding(), Format, AsciiStrSize(Format), Marker);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Print to console
  Status = EfiConOutPrint(String);
  // Free string
  EfiFreePool((VOID *)String);
  // Return status
  return Status;
}

// EfiAsciiPoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR8 *
EFIAPI
EfiAsciiPoolPrint (
  IN CONST CHAR8 *Format,
  ...
) {
  CHAR8  *String;
  VA_LIST  Marker;
  VA_START(Marker, Format);
  String = EfiAsciiPoolVPrint(Format, Marker);
  VA_END(Marker);
  return String;
}
// EfiAsciiPoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
CHAR8 *
EFIAPI
EfiAsciiPoolVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiLatin1Encoding();
  return EfiLocalePoolVPrint(NULL, Encoding, NULL, Encoding, (VOID  *)Format, AsciiStrSize(Format), Marker);
}

// EfiAsciiSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EFI_STATUS
EFIAPI
EfiAsciiSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  ...
) {
  UINTN   Length;
  VA_LIST Marker;
  VA_START(Marker, Format);
  Length = EfiAsciiVSPrint(Buffer, Size, Format, Marker);
  VA_END(Marker);
  return Length;
}
// EfiAsciiVSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EFI_STATUS
EFIAPI
EfiAsciiVSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  IN     VA_LIST      Marker
) {
  EFI_ENCODING_PROTOCOL *Encoding = EfiLatin1Encoding();
  return EfiLocaleVSPrint(NULL, Encoding, (VOID *)Buffer, Size, Encoding, (VOID *)Format, AsciiStrSize(Format), Marker);
}
