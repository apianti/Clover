///
/// @file Include/Uefi/Protocol/Encoding.h
///
/// Character string encoding
///

#pragma once
#ifndef __UEFI_ENCODING_HEADER__
#define __UEFI_ENCODING_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_ENCODING_PROTOCOL_GUID
/// The character string encoding protocol unique identifier
#define EFI_ENCODING_PROTOCOL_GUID { 0x0952A6AB, 0x38C1, 0x44A7, { 0x9B, 0xEE, 0xCA, 0xA1, 0xDE, 0x19, 0x7F, 0x27 } }

// EFI_LATIN1_ENCODING_PROTOCOL_GUID
/// The Latin-1 character string encoding protocol unique identifier
#define EFI_LATIN1_ENCODING_PROTOCOL_GUID { 0x366EE757, 0x4247, 0x4549, { 0x8C, 0x33, 0xC4, 0x54, 0x0F, 0x71, 0x03, 0xC9 } }
// EFI_UTF8_ENCODING_PROTOCOL_GUID
/// The UTF-8 character string encoding protocol unique identifier
#define EFI_UTF8_ENCODING_PROTOCOL_GUID { 0x66396A68, 0xD0A4, 0x40D2, { 0xAF, 0xB7, 0x57, 0xE0, 0xBE, 0x93, 0xBF, 0x63 } }
// EFI_UTF16_ENCODING_PROTOCOL_GUID
/// The UTF-16 character string encoding protocol unique identifier
#define EFI_UTF16_ENCODING_PROTOCOL_GUID { 0x72BCC62F, 0x496E, 0x44A2, { 0xA1, 0x9D, 0xB7, 0x52, 0x2F, 0xCC, 0xFD, 0xD8 } }
// EFI_UTF16_SWAPPED_ENCODING_PROTOCOL_GUID
/// The UTF-16 byte swapped character string encoding protocol unique identifier
#define EFI_UTF16_SWAPPED_ENCODING_PROTOCOL_GUID { 0x3C3C40B9, 0x1892, 0x403C, { 0x95, 0xD0, 0x8B, 0x7B, 0x1D, 0xC8, 0x1B, 0xE2 } }

// gEfiEncodingProtocolGuid
/// The character string encoding protocol unique identifier
EXTERN EFI_GUID gEfiEncodingProtocolGuid;

// gEfiLatin1EncodingProtocolGuid
/// The Latin-1 character string encoding protocol unique identifier
EXTERN EFI_GUID gEfiLatin1EncodingProtocolGuid;
// gEfiUtf8EncodingProtocolGuid
/// The UTF-8 character string encoding protocol unique identifier
EXTERN EFI_GUID gEfiUtf8EncodingProtocolGuid;
// gEfiUtf16EncodingProtocolGuid
/// The UTF-16 character string encoding protocol unique identifier
EXTERN EFI_GUID gEfiUtf16EncodingProtocolGuid;
// gEfiUtf16SwappedEncodingProtocolGuid
/// The UTF-16 byte swapped character string encoding protocol unique identifier
EXTERN EFI_GUID gEfiUtf16SwappedEncodingProtocolGuid;

// EFI_ENCODING_PROTOCOL
/// Character string encoding protocol
typedef struct EFI_ENCODING_PROTOCOL EFI_ENCODING_PROTOCOL;

// EFI_ENCODING_DECODE_CHAR
/// Decode a character from an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The encoded character string buffer to decode
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to decode the character from the output string buffer
/// @param Character On output, the decoded Unicode character
/// @retval EFI_INVALID_PARAMETER  If This, Buffer, Size, or Character is NULL
/// @retval EFI_NOT_FOUND          There were no characters remaining to decode
/// @retval EFI_NO_MAPPING         There were no valid characters decoded but some character code points were invalid and ignored
/// @retval EFI_WARN_UNKNOWN_GLYPH The character was converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            The character was decoded successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_ENCODING_DECODE_CHAR) (
  IN     EFI_ENCODING_PROTOCOL *This,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
);
// EFI_ENCODING_ENCODE_CHAR
/// Encode a character into an encoded character string
/// @param This      The encoding protocol interface
/// @param Buffer    The encoded character string buffer
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to encode the character into the output string buffer
/// @param Character The Unicode character to encode
/// @retval EFI_INVALID_PARAMETER  If This or Size is NULL
/// @retval EFI_INVALID_PARAMETER  If Character is an invalid Unicode character but not zero (null-terminator)
/// @retval EFI_BUFFER_TOO_SMALL   The specified output buffer size is not large enough to hold the converted string, *Size is updated with the required size
/// @retval EFI_NO_MAPPING         The character can not be encoded with this encoding
/// @retval EFI_SUCCESS            The character was encoded successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_ENCODING_ENCODE_CHAR) (
  IN     EFI_ENCODING_PROTOCOL *This,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
);

// EFI_ENCODING_PROTOCOL
/// Character string encoding protocol
struct EFI_ENCODING_PROTOCOL {

  // Name
  /// The encoding name
  CONST CHAR8                *Name;
  // UnitSize
  /// The size in bytes of the base (smallest) unit of the encoding
  UINTN                       UnitSize;
  // MaxUnits
  /// The maximum count of base units needed to encoded a character
  UINTN                       MaxUnits;

  // DecodeCharacter
  /// Decode a character from an encoded character string
  EFI_ENCODING_DECODE_CHAR    DecodeCharacter;
  // EncodeCharacter
  /// Encode a character into an encoded character string
  EFI_ENCODING_ENCODE_CHAR    EncodeCharacter;

};

// EfiGetEncoding
/// Get an encoding protocol interface
/// @param Name The name of the encoding or NULL for the current encoding
/// @return The encoding protocol interface or NULL if not found
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiGetEncoding (
  IN CONST CHAR8 *Name OPTIONAL
);
// EfiSetEncoding
/// Set the current encoding protocol interface
/// @param Name The name of the encoding or NULL to restore the default encoding
/// @retval EFI_NOT_FOUND The encoding was not found
/// @retval EFI_SUCCESS   The encoding was set
EXTERN
EFI_STATUS
EFIAPI
EfiSetEncoding (
  IN CONST CHAR8 *Name OPTIONAL
);

// EfiDecodeCharacter
/// Decode a character from an encoded character string
/// @param Encoding  The encoding protocol interface
/// @param Buffer    The encoded character string buffer to decode
/// @param Size      On input, the size in bytes of the output string buffer, on output, the size in bytes used to decode the character from the output string buffer
/// @param Character On output, the decoded Unicode character
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, Size, or Character is NULL
/// @retval EFI_NO_MAPPING         There were no valid characters decoded but some character code points were invalid and ignored
/// @retval EFI_NOT_FOUND          There were no characters remaining to decode
/// @retval EFI_WARN_UNKNOWN_GLYPH The string was converted but some character code points were invalid and ignored
/// @retval EFI_SUCCESS            The character was decoded successfully
EXTERN
EFI_STATUS
EFIAPI
EfiDecodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size,
  OUT    UINT32                *Character
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiEncodeCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT32                 Character
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiAppendCharacter (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Count,
  IN OUT UINTN                  *Size,
  IN     UINT32                  Character
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiFillCharacter (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINTN                  Count,
  IN     UINT32                 Character
);

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
);
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
EXTERN
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
);
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
EXTERN
INTN
EFIAPI
EfiEncodingICompare (
  IN EFI_ENCODING_PROTOCOL *Encoding1,
  IN VOID                  *Buffer1,
  IN UINTN                  BufferSize1,
  IN EFI_ENCODING_PROTOCOL *Encoding2,
  IN VOID                  *Buffer2,
  IN UINTN                  BufferSize2
);
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
EXTERN
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
);

// EfiEncodingStringLength
/// Get the count of character code points from an encoded character string
/// @param Encoding The encoding protocol interface
/// @param Buffer   The encoded character string buffer
/// @param Size     On input, the size in bytes of the string buffer, on output, the count in characters of the string buffer
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, or Size is NULL
/// @retval EFI_WARN_UNKNOWN_GLYPH The character was converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            The character was decoded successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingStringLength (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  IN     CONST VOID            *Buffer,
  IN OUT UINTN                 *Size
);
// EfiEncodingStringSize
/// Get the size of a null-terminated encoded character string
/// @param Encoding The encoding protocol interface
/// @param Buffer   The encoded character string buffer
/// @param Size     On output, the size of the encoded character string buffer
/// @retval EFI_INVALID_PARAMETER  If Encoding, Buffer, or Size is NULL
/// @retval EFI_WARN_UNKNOWN_GLYPH The characters were converted but some character units were invalid and ignored
/// @retval EFI_SUCCESS            All characters were decoded successfully and the size in bytes was returned in *Size
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingStringSize (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  IN  CONST VOID            *Buffer,
  OUT UINTN                 *Size
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingConvert (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN     VOID                  *Input,
  IN     UINTN                  InputSize
);
// EfiEncodingPoolConvert
/// Convert an encoded string to another encoding
/// @param Encoding       The output encoding or NULL for the current default encoding
/// @param Size           On output, the size in bytes 
/// @param InputEncoding  The input encoding or NULL for the current default encoding
/// @param Input          The input encoded string buffer to decode
/// @param InputSize      The size in bytes of the input encoded string buffer
/// @return The converted encoded string or NULL if there was an error
EXTERN
VOID *
EFIAPI
EfiEncodingPoolConvert (
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *InputEncoding OPTIONAL,
  IN  VOID                  *Input,
  IN  UINTN                  InputSize
);
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
);
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
);

// EfiEncodingPrintSize
/// Get the size in bytes of a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param ...            The arguments to format
/// @return The size in bytes of the formatted character string
EXTERN
UINTN
EFIAPI
EfiEncodingPrintSize (
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  ...
);
// EfiEncodingVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Encoding       The encoding of the formatted character string
/// @param FormatEncoding The encoding of the format specifier string
/// @param Format         The format specifier string
/// @param FormatSize     The size in bytes of the format specifier string
/// @param Marker         The arguments to format
/// @return The size in bytes of the formatted character string
EXTERN
UINTN
EFIAPI
EfiEncodingVPrintSize (
  IN EFI_ENCODING_PROTOCOL *Encoding,
  IN EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN CONST VOID            *Format,
  IN UINTN                  FormatSize,
  IN VA_LIST                Marker
);

// EfiEncodingPoolPrint
/// Allocate a formatted character string
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
EfiEncodingPoolPrint (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  ...
);
// EfiEncodingPoolVPrint
/// Allocate a formatted character string
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
EfiEncodingPoolVPrint (
  IN  EFI_ENCODING_PROTOCOL *Encoding,
  OUT UINTN                 *Size OPTIONAL,
  IN  EFI_ENCODING_PROTOCOL *FormatEncoding,
  IN  CONST VOID            *Format,
  IN  UINTN                  FormatSize,
  VA_LIST                    Marker
);

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
EXTERN
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
);
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
EXTERN
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
);

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
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingPrintBoolean (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     BOOLEAN                Boolean,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
// EfiEncodingPrintUnsigned
/// Print an unsigned integer to a character string buffer
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
EfiEncodingPrintUnsigned (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINT64                 Unsigned,
  IN     UINTN                  Base,
  IN     UINTN                  Width,
  IN     UINTN                  Flags
);
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
);
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
EXTERN
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
);
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
EXTERN
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
);
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
EXTERN
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
);
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
EXTERN
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
);
// EfiEncodingPrintGuid
/// Print a GUID interface to a character string buffer
/// @param Encoding  The encoding of the formatted character string
/// @param Buffer    On output, the formatted character string
/// @param Size      On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Guid      The GUID interface
/// @param Uppercase Whether to use uppercase digits if available or not
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingPrintGuid (
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     CONST EFI_GUID        *Guid,
  IN     BOOLEAN                Uppercase
);

// EfiEncodingToBoolean
/// Convert a boolean formatted character string buffer to a boolean
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
EfiEncodingToBoolean (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT BOOLEAN                *Boolean
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToUnsigned (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT UINT64                 *Unsigned,
  IN     UINTN                   Base
);
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
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToInteger (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT INT64                  *Integer,
  IN     UINTN                   Base
);
// EfiEncodingToFloat
/// Convert a floating point formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToFloat (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
// EfiEncodingToScientific
/// Convert a scientific formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToScientific (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
// EfiEncodingToMonetary
/// Convert a monetarily formatted character string buffer to a floating point
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Float    On output, the converted floating point
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Float is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToMonetary (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT FLOAT64                *Float
);
// EfiEncodingToDateTime
/// Convert a date and/or time formatted character string buffer to a date and time
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param DateTime On output, the converted date and time
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToDateTime (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_TIME               *DateTime
);
// EfiEncodingToGuid
/// Convert a GUID formatted character string buffer to a GUID
/// @param Encoding The encoding of the formatted character string
/// @param Buffer   On input, the character buffer to convert, on output, the buffer position after conversion
/// @param Size     On input, the size of the input buffer in bytes, on output, the size of the remaining buffer in bytes
/// @param Guid     On output, the converted GUID
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Guid is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a GUID successfully
EXTERN
EFI_STATUS
EFIAPI
EfiEncodingToGuid (
  IN     EFI_ENCODING_PROTOCOL  *Encoding,
  IN OUT VOID                  **Buffer,
  IN OUT UINTN                  *Size OPTIONAL,
  IN OUT EFI_GUID               *Guid
);

// EfiDefaultEncoding
/// Get the default encoding protocol
/// @return The default encoding protocol or NULL if there was an error
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiDefaultEncoding (
  VOID
);
// EfiLatin1Encoding
/// Get the Latin-1 encoding protocol
/// @return The Latin-1 encoding protocol or NULL if there was an error
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiLatin1Encoding (
  VOID
);
// EfiUtf8Encoding
/// Get the UTF-8 encoding protocol
/// @return The UTF-8 encoding protocol or NULL if there was an error
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiUtf8Encoding (
  VOID
);
// EfiUtf16Encoding
/// Get the UTF-16 encoding protocol
/// @return The UTF-16 encoding protocol or NULL if there was an error
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiUtf16Encoding (
  VOID
);
// EfiUtf16SwappedEncoding
/// Get the UTF-16 byte swapped encoding protocol
/// @return The UTF-16 byte swapped encoding protocol or NULL if there was an error
EXTERN
EFI_ENCODING_PROTOCOL *
EFIAPI
EfiUtf16SwappedEncoding (
  VOID
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_ENCODING_HEADER__
