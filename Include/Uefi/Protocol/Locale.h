///
/// @file Include/Uefi/Protocol/Locale.h
///
/// Locale protocol
///

#pragma once
#ifndef __UEFI_LOCALE_HEADER__
#define __UEFI_LOCALE_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_LOCALE_PROTOCOL_GUID
/// The locale protocol unique identifier
#define EFI_LOCALE_PROTOCOL_GUID { 0xE8878EA9, 0xC47C, 0x47FA, { 0x9C, 0xD0, 0x66, 0xB7, 0xD6, 0x98, 0x78, 0x05 } }

// EFI_DEFAULT_LOCALE_PROTOCOL_GUID
/// The default locale protocol unique identifier
#define EFI_DEFAULT_LOCALE_PROTOCOL_GUID { 0x6C9E79FA, 0x9AD3, 0x41D3, { 0x90, 0x42, 0x04, 0xF1, 0xD0, 0x72, 0x96, 0x1B } }

// EfiLocaleProtocolGuid
/// The locale protocol unique identifier
EXTERN EFI_GUID gEfiLocaleProtocolGuid;

// EfiDefaultLocaleProtocolGuid
/// The default locale protocol unique identifier
EXTERN EFI_GUID gEfiDefaultLocaleProtocolGuid;

// EFI_ENCODING_PROTOCOL
/// Character string encoding protocol
typedef struct EFI_ENCODING_PROTOCOL EFI_ENCODING_PROTOCOL;

// EFI_LOCALE_PROTOCOL
/// Locale protocol
typedef struct EFI_LOCALE_PROTOCOL EFI_LOCALE_PROTOCOL;

// EFI_LOCALE_PRINT_TIME
/// Print time according to locale rules
#define EFI_LOCALE_PRINT_TIME EFI_BIT(0)
// EFI_LOCALE_PRINT_DATE
/// Print date according to locale rules
#define EFI_LOCALE_PRINT_DATE EFI_BIT(1)
// EFI_LOCALE_PRINT_LONG_DATE
/// Print long date accoring to locale rules
#define EFI_LOCALE_PRINT_LONG_DATE EFI_BIT(2)

// EFI_LOCALE_PRINT_FLOAT_DEFAULT_PRECISION
/// The default print precision for floating point values
#define EFI_LOCALE_PRINT_FLOAT_DEFAULT_PRECISION 6

// EFI_LOCALE_PRINT_FLAG_UPPERCASE
/// Print uppercase characters instead of lowercase if possible
#define EFI_LOCALE_PRINT_FLAG_UPPERCASE EFI_BIT(0)
// EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY
/// Left justify to the width instead of right justification if possible
#define EFI_LOCALE_PRINT_FLAG_LEFT_JUSTIFY EFI_BIT(1)
// EFI_LOCALE_PRINT_FLAG_PAD_ZEROES
/// Pad zeroes instead of spaces when padding width if possible
#define EFI_LOCALE_PRINT_FLAG_PAD_ZEROES EFI_BIT(2)
// EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN
/// Prefix the positive or negative sign if possible
#define EFI_LOCALE_PRINT_FLAG_PREFIX_SIGN EFI_BIT(3)

// EFI_LOCALE_COMPARE
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
typedef
INTN
(EFIAPI
*EFI_LOCALE_COMPARE) (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
);
// EFI_LOCALE_NCOMPARE
/// Compare two encoded character strings with a maximum count
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
typedef
INTN
(EFIAPI
*EFI_LOCALE_NCOMPARE) (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
 );
 // EFI_LOCALE_ICOMPARE
/// Compare two encoded character strings case insensitively
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
typedef
INTN
(EFIAPI
*EFI_LOCALE_ICOMPARE) (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
);
// EFI_LOCALE_NICOMPARE
/// Compare two encoded character strings case insensitively with a maximum count
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
typedef
INTN
(EFIAPI
*EFI_LOCALE_NICOMPARE) (
  IN EFI_LOCALE_PROTOCOL   *This,
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2,
  IN UINTN                  Count
 );

// EFI_LOCALE_PRINT_BOOLEAN
/// Print a boolean to a character string buffer
/// @param This      The locale protocol
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Boolean   The boolean
/// @param Width     The minimum count of characters to print
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_BOOLEAN) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     BOOLEAN                Boolean,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_BOOLEAN
/// Convert a boolean formatted character string buffer to a boolean
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Boolean  On output, the converted boolean
/// @retval EFI_INVALID_PARAMETER If This, Encoding, Buffer, *Buffer, or Boolean is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to a boolean successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_BOOLEAN) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT BOOLEAN                *Boolean
);
// EFI_LOCALE_PRINT_UNSIGNED
/// Print an unsigned integer to a character string buffer
/// @param This      The locale protocol
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Unsigned  The unsigned integer
/// @param Base      The base of the unsigned integer (usually 10 for decimal)
/// @param Width     The minimum count of digits to print
/// @param Flags     The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_UNSIGNED) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT64                 Unsigned,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_UNSIGNED
/// Convert an unsigned integer formatted character string buffer to an unsigned integer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Unsigned On output, the converted unsigned integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If This, Encoding, Buffer, *Buffer, Size, or Unsigned is NULL
/// @retval EFI_NOT_FOUND         If the buffer is not valid for conversion
/// @retval EFI_SUCCESS           If the buffer was converted to an unsigned integer successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_UNSIGNED) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
);
// EFI_LOCALE_PRINT_INTEGER
/// Print an unsigned integer to a character string buffer
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
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_INTEGER) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     INT64                  Integer,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_INTEGER
/// Convert a signed integer formatted character string buffer to an signed integer
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Integer  On output, the converted signed integer
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, Size, or Integer is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to an unsigned integer successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_INTEGER) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
);
// EFI_LOCALE_PRINT_FLOAT
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
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_FLOAT) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_FLOAT
/// Convert a floating point formatted character string buffer to a floating point
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, Size, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_FLOAT) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT FLOAT64                *Float
);
// EFI_LOCALE_PRINT_SCIENTIFIC
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
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_SCIENTIFIC) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Float,
  IN     UINTN                  Width,
  IN     UINTN                  Significance,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_SCIENTIFIC
/// Convert a scientific formatted character string buffer to a floating point
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, Size, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_SCIENTIFIC) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT FLOAT64                *Float
);
// EFI_LOCALE_PRINT_MONETARY
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
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_MONETARY) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     FLOAT64                Monetary,
  IN     UINTN                  Width,
  IN     UINTN                  Precision,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_MONETARY
/// Convert a monetarily formatted character string buffer to a floating point
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, Size, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_MONETARY) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT FLOAT64                *Float
);
// EFI_LOCALE_PRINT_DATE_TIME
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
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_PRINT_DATE_TIME) (
  IN     EFI_LOCALE_PROTOCOL   *This,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_TIME        *Time,
  IN     UINTN                  Type,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
// EFI_LOCALE_TO_DATE_TIME
/// Convert a date and/or time formatted character string buffer to a floating point
/// @param This     The locale protocol
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param DateTime On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, Size, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_LOCALE_TO_DATE_TIME) (
  IN     EFI_LOCALE_PROTOCOL    *This,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size,
  IN OUT EFI_TIME               *DateTime
);

// EFI_LOCALE_PROTOCOL
/// Locale protocol
struct EFI_LOCALE_PROTOCOL {

  // Name
  /// The locale name
  CONST CHAR8 *Name;

  // Compare
  /// Compare two encoded character strings
  EFI_LOCALE_COMPARE          Compare;
  // NCompare
  /// Compare two encoded character strings with a maximum count
  EFI_LOCALE_NCOMPARE         NCompare;
  // ICompare
  /// Compare two encoded character strings case insensitively
  EFI_LOCALE_ICOMPARE         ICompare;
  // NICompare
  /// Compare two encoded character strings case insensitively with a maximum count;
  EFI_LOCALE_NICOMPARE        NICompare;
  // ToBoolean
  /// Print a boolean to a character string buffer
  EFI_LOCALE_PRINT_BOOLEAN    PrintBoolean;
  // ToBoolean
  /// Convert a character string buffer to a boolean
  EFI_LOCALE_TO_BOOLEAN       ToBoolean;
  // PrintUnsigned
  /// Print an unsigned integer to a character string buffer
  EFI_LOCALE_PRINT_UNSIGNED   PrintUnsigned;
  // ToUnsigned
  /// Convert a character string buffer to an unsigned integer
  EFI_LOCALE_TO_UNSIGNED      ToUnsigned;
  // PrintInteger
  /// Print a signed integer to a character string buffer
  EFI_LOCALE_PRINT_INTEGER    PrintInteger;
  // ToInteger
  /// Convert a character string buffer to a signed integer
  EFI_LOCALE_TO_INTEGER       ToInteger;
  // PrintFloat
  /// Print a floating point number to a character string buffer
  EFI_LOCALE_PRINT_FLOAT      PrintFloat;
  // ToFloat
  /// Convert a character string buffer to a floating point
  EFI_LOCALE_TO_FLOAT         ToFloat;
  // PrintScientific
  /// Print a scientific floating point number to a character string buffer
  EFI_LOCALE_PRINT_SCIENTIFIC PrintScientific;
  // ToScientific
  /// Convert a character string buffer to a scientific floating point
  EFI_LOCALE_TO_SCIENTIFIC    ToScientific;
  // PrintMonetary
  /// Print a monetary value to a character string buffer
  EFI_LOCALE_PRINT_MONETARY   PrintMonetary;
  // ToMonetary
  /// Convert a character string buffer to a monetary value
  EFI_LOCALE_TO_MONETARY      ToMonetary;
  // PrintDateTime
  /// Print a time interface to a character string buffer
  EFI_LOCALE_PRINT_DATE_TIME  PrintDateTime;
  // ToDateTime
  /// Convert a character string buffer to a time interface
  EFI_LOCALE_TO_DATE_TIME     ToDateTime;

};

// EfiGetLocale
/// Get an locale protocol interface
/// @param Name The name of the locale or NULL for the current locale
/// @return The locale protocol interface or NULL if not found
EXTERN
EFI_LOCALE_PROTOCOL *
EFIAPI
EfiGetLocale (
  IN CONST CHAR8 *Name OPTIONAL
);
// EfiSetLocale
/// Set the current locale protocol interface
/// @param Name The name of the locale or NULL to restore the default locale
/// @retval EFI_NOT_FOUND The locale was not found
/// @retval EFI_SUCCESS   The locale was set
EXTERN
EFI_STATUS
EFIAPI
EfiSetLocale (
  IN CONST CHAR8 *Name OPTIONAL
);

// EfiLocalePrintSize
/// Get the size in bytes of a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The size in bytes of the formatted character string
EXTERN
UINTN
EFIAPI
EfiLocalePrintSize (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  ...
);
// EfiLocaleVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Locale         The locale to use in formatting or NULL for the current locale
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The size in bytes of the formatted character string
EXTERN
UINTN
EFIAPI
EfiLocaleVPrintSize (
  IN EFI_LOCALE_PROTOCOL   *Locale OPTIONAL,
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  IN VA_LIST                Marker
);

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
EXTERN
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
);
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
EXTERN
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
);

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
EXTERN
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
);
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
EXTERN
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
);

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
EXTERN
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
);
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
 );
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
);
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
 );

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
EXTERN
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
);
// EfiLocalePrintUnsigned
/// Print an unsigned integer to a character string buffer
/// @param Locale   The locale to use in formatting or NULL for the current locale
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On output, the formatted character string
/// @param Size     On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Base     The base of the unsigned integer (usually 10 for decimal)
/// @param Unsigned The unsigned integer
/// @param Width    The minimum count of digits to print
/// @param Flags    The print flags
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
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
);
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
EXTERN
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
);
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
EXTERN
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
);
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
);
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
EXTERN
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
);
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
);

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
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToUnsigned (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToInteger (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToFloat (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToScientific (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToMonetary (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiLocaleToDateTime (
  IN     EFI_LOCALE_PROTOCOL    *Locale OPTIONAL,
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_TIME               *DateTime
);

// EfiDefaultLocale
/// Get the default locale protocol
/// @return The default locale protocol or NULL if there was an error
EXTERN
EFI_LOCALE_PROTOCOL *
EFIAPI
EfiDefaultLocale (
  VOID
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_LOCALE_HEADER__
