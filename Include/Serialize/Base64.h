//
/// @file Include/Serialize/Parser.h
///
/// GUI language parser implementation
///

#pragma once
#ifndef __GUI_PARSER_HEADER__
#define __GUI_PARSER_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

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
EXTERN
EFI_STATUS
EFIAPI
Base64Decode (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     CONST VOID            *Input,
  IN     UINTN                  InputSize,
  OUT    VOID                  *Output OPTIONAL,
  IN OUT UINTN                 *OutputSize
);
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
EXTERN
EFI_STATUS
EFIAPI
Base64Encode (
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     CONST VOID            *Input,
  IN     UINTN                  InputSize,
  OUT    VOID                  *Output OPTIONAL,
  IN OUT UINTN                 *OutputSize
);

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
EXTERN
EFI_STATUS
EFIAPI
Base64DecodeBuffer (
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST VOID             *Input,
  IN  UINTN                   InputSize,
  OUT VOID                  **Output,
  OUT UINTN                  *OutputSize OPTIONAL
);
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
EXTERN
EFI_STATUS
EFIAPI
Base64EncodeBuffer (
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST VOID             *Input,
  IN  UINTN                   InputSize,
  OUT VOID                  **Output,
  OUT UINTN                  *OutputSize OPTIONAL
);

// Base64FromString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
EXTERN
CHAR16 *
EFIAPI
Base64FromString (
  IN CONST CHAR16 *String
);
// Base64ToString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
EXTERN
CHAR16 *
EFIAPI
Base64ToString (
  IN CONST CHAR16 *Base64
);
// Base64FromAsciiString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
EXTERN
CHAR16 *
EFIAPI
Base64FromAsciiString (
  IN CONST CHAR8 *String
);
// Base64ToAsciiString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
EXTERN
CHAR8 *
EFIAPI
Base64ToAsciiString (
  IN CONST CHAR16 *Base64
);

// Base64AsciiFromString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
EXTERN
CHAR8 *
EFIAPI
Base64AsciiFromString (
  IN CONST CHAR16 *String
);
// Base64AsciiToString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
EXTERN
CHAR16 *
EFIAPI
Base64AsciiToString (
  IN CONST CHAR8 *Base64
);

// Base64AsciiFromAsciiString
/// Encode a string into a base 64 string
/// @param String The string to encode into base 64
/// @return The base 64 encoded string which must be freed
EXTERN
CHAR8 *
EFIAPI
Base64AsciiFromAsciiString (
  IN CONST CHAR8 *String
);
// Base64ToAsciiString
/// Decode a string from a base 64 string
/// @param Base64 The base 64 string to decode
/// @return The decoded string which must be freed
EXTERN
CHAR8 *
EFIAPI
Base64AsciiToAsciiString (
  IN CONST CHAR8 *Base64
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __GUI_PARSER_HEADER__
