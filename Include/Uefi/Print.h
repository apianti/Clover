///
/// @file Include/Uefi/Print.h
///
/// UEFI print to string and console utilities
///

#pragma once
#ifndef ___UEFI_PRINT_HEADER__
#define ___UEFI_PRINT_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//
// Unicode UTF-16 print helper functions
//

// EfiPrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiPrintSize (
  IN CONST CHAR16 *Format,
  ...
);
// EfiVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiVPrintSize (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
);

// EfiPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiPrint (
  IN CONST CHAR16 *Format,
  ...
);
// EfiVPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiVPrint (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
);

// EfiPoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR16 *
EFIAPI
EfiPoolPrint (
  IN CONST CHAR16 *Format,
  ...
);
// EfiPoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR16 *
EFIAPI
EfiPoolVPrint (
  IN CONST CHAR16 *Format,
  IN VA_LIST       Marker
);

// EfiSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EXTERN
EFI_STATUS
EFIAPI
EfiSPrint (
  OUT    CHAR16       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR16 *Format,
  ...
);
// EfiVSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiVSPrint (
  OUT    CHAR16       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR16 *Format,
  IN     VA_LIST       Marker
);

//
// Unicode UTF-8 print helper functions
//

// EfiUtf8PrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiUtf8PrintSize (
  IN CONST CHAR8 *Format,
  ...
);
// EfiUtf8VPrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiUtf8VPrintSize (
  IN CONST CHAR8 *Format,
  IN VA_LIST       Marker
);

// EfiUtf8Print
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiUtf8Print (
  IN CONST CHAR8 *Format,
  ...
);
// EfiUtf8VPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiUtf8VPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST       Marker
);

// EfiUtf8PoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR8 *
EFIAPI
EfiUtf8PoolPrint (
  IN CONST CHAR8 *Format,
  ...
);
// EfiUtf8PoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR8 *
EFIAPI
EfiUtf8PoolVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST       Marker
);

// EfiUtf8SPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EXTERN
EFI_STATUS
EFIAPI
EfiUtf8SPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR8 *Format,
  ...
);
// EfiUtf8VSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiUtf8VSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN        *Size,
  IN     CONST CHAR8 *Format,
  IN     VA_LIST       Marker
);

//
// Latin-1/ASCII print helper functions
//

// EfiPrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiAsciiPrintSize (
  IN CONST CHAR8 *Format,
  ...
);
// EfiVPrintSize
/// Get the size in bytes of a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The length of the formatted character string
EXTERN
UINTN
EFIAPI
EfiAsciiVPrintSize (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
);

// EfiAsciiPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param ...    The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiAsciiPrint (
  IN CONST CHAR8 *Format,
  ...
);
// EfiAsciiVPrint
/// Print a formatted character string to console
/// @param Format The format specifier string
/// @param Marker The arguments to format
EXTERN
EFI_STATUS
EFIAPI
EfiAsciiVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
);

// EfiAsciiPoolPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR8 *
EFIAPI
EfiAsciiPoolPrint (
  IN CONST CHAR8 *Format,
  ...
);
// EfiAsciiPoolVPrint
/// Allocate a formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @return The pool allocated formatted character string which must be freed or NULL
EXTERN
CHAR8 *
EFIAPI
EfiAsciiPoolVPrint (
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
);

// EfiAsciiSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param ...    The arguments to format
/// @return The number of characters printed
EXTERN
EFI_STATUS
EFIAPI
EfiAsciiSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  ...
);
// EfiAsciiVSPrint
/// Print a formatted character string to a character string buffer
/// @param Buffer On output, the formatted character string
/// @param Size   On input, the size in bytes of the formatted character string buffer, on output, the size in bytes of the formatted character string
/// @param Format The format specifier string
/// @param Marker The arguments to format
/// @retval EFI_SUCCESS          The formatted character string was printed
/// @retval EFI_BUFFER_TOO_SMALL The formatted character string buffer is too small, *Size has been updated with the required size
EXTERN
EFI_STATUS
EFIAPI
EfiAsciiVSPrint (
  OUT    CHAR8       *Buffer OPTIONAL,
  IN OUT UINTN       *Size,
  IN     CONST CHAR8 *Format,
  IN     VA_LIST      Marker
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_PRINT_HEADER__
