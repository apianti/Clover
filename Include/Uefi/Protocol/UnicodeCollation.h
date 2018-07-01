///
/// @file Include/Uefi/Protocol/UnicodeCollation.h
///
/// UEFI implementation unicode collation protocol
///

#pragma once
#ifndef ___UEFI_UNICODE_COLLATION_PROTOCOL_HEADER__
#define ___UEFI_UNICODE_COLLATION_PROTOCOL_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_UNICODE_COLLATION_PROTOCOL_GUID
/// The unicode collation protocol unique identifier
#define EFI_UNICODE_COLLATION_PROTOCOL_GUID { 0x1D85CD7F, 0xF43D, 0x11D2, { 0x9A, 0xC, 0x0, 0x90, 0x27, 0x3F, 0xC1, 0x4D } }
// EFI_UNICODE_COLLATION_PROTOCOL2_GUID
/// The unicode collation 2 protocol unique identifier
#define EFI_UNICODE_COLLATION_PROTOCOL2_GUID { 0xA4C751FC, 0x23AE, 0x4C3E, { 0x92, 0xE9, 0x49, 0x64, 0xCF, 0x63, 0xF3, 0x49 } }

// gEfiUnicodeCollationProtocolGuid
/// The unicode collation protocol unique identifier
EXTERN EFI_GUID gEfiUnicodeCollationProtocolGuid;
// gEfiUnicodeCollation2ProtocolGuid
/// The unicode collation 2 protocol unique identifier
EXTERN EFI_GUID gEfiUnicodeCollation2ProtocolGuid;

// EFI_UNICODE_COLLATION_PROTOCOL
/// The unicode collation protocol
typedef struct EFI_UNICODE_COLLATION_PROTOCOL EFI_UNICODE_COLLATION_PROTOCOL;

// EFI_UNICODE_COLLATION_STRICOLL
/// Performs a case-insensitive comparison of two null-terminated strings
/// @param This The unicode collation protocol interface
/// @param Str1 A null-terminated string
/// @param Str2 A null-terminated string
/// @retval 0 Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
typedef
INTN
(EFIAPI
*EFI_UNICODE_COLLATION_STRICOLL) (
  IN EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN CHAR16                         *Str1,
  IN CHAR16                         *Str2
);

// EFI_UNICODE_COLLATION_METAIMATCH
/// Performs a case-insensitive comparison of a null-terminated pattern string and a null-terminated string
/// @param This    The unicode collation protocol interface
/// @param String  A null-terminated string
/// @param Pattern The null-terminated pattern string
/// @retval TRUE  Pattern was found in String
/// @retval FALSE Pattern was not found in String
typedef
BOOLEAN
(EFIAPI
*EFI_UNICODE_COLLATION_METAIMATCH) (
  IN EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN CHAR16                         *String,
  IN CHAR16                         *Pattern
);

// EFI_UNICODE_COLLATION_STRLWR
/// Converts all the characters in a null-terminated string to lowercase characters
/// @param This    The unicode collation protocol interface
/// @param String  A null-terminated string
typedef
VOID
(EFIAPI
*EFI_UNICODE_COLLATION_STRLWR) (
  IN     EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN OUT CHAR16                         *String
);

// EFI_UNICODE_COLLATION_STRUPR
/// Converts all the characters in a null-terminated string to uppercase characters
/// @param This    The unicode collation protocol interface
/// @param String  A null-terminated string
typedef
VOID
(EFIAPI
*EFI_UNICODE_COLLATION_STRUPR) (
  IN     EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN OUT CHAR16                         *String
);

// EFI_UNICODE_COLLATION_FATTOSTR
/// Converts an 8.3 FAT file name in an OEM character set to a null-terminated string
/// @param This    The unicode collation protocol interface
/// @param FatSize The size of the string Fat in bytes
/// @param Fat     A null-terminated string that contains an 8.3 file name encoded using an 8-bit OEM character set
/// @param String  On output, a null-terminated string that must be allocated in advance to hold FatSize characters
typedef
VOID
(EFIAPI
*EFI_UNICODE_COLLATION_FATTOSTR) (
  IN  EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN  UINTN                           FatSize,
  IN  CHAR8                          *Fat,
  OUT CHAR16                         *String
);

// EFI_UNICODE_COLLATION_STRTOFAT
/// Converts a null-terminated string to legal characters in a FAT filename using an OEM character set
/// @param This    The unicode collation protocol interface
/// @param String  A null-terminated string
/// @param FatSize The size of the string Fat in bytes
/// @param Fat     On output, a null-terminated string that contains the converted version of String using legal FAT characters from an OEM character set
typedef
BOOLEAN
(EFIAPI
*EFI_UNICODE_COLLATION_STRTOFAT) (
  IN  EFI_UNICODE_COLLATION_PROTOCOL *This,
  IN  CHAR16                         *String,
  IN  UINTN                           FatSize,
  OUT CHAR8                          *Fat
);

// EFI_UNICODE_COLLATION_PROTOCOL
/// The unicode collation protocol
struct EFI_UNICODE_COLLATION_PROTOCOL {

  // StriColl
  /// Performs a case-insensitive comparison of two null-terminated strings
  EFI_UNICODE_COLLATION_STRICOLL    StriColl;
  // MetaiMatch
  /// Performs a case-insensitive comparison between a Nullterminated pattern string and a null-terminated string.
  ///  The pattern string can use the ‘?’ wildcard to match any character, and the ‘*’ wildcard to match any substring
  EFI_UNICODE_COLLATION_METAIMATCH  MetaiMatch;
  // StrLwr
  /// Converts all the characters in a null-terminated string to lowercase characters
  EFI_UNICODE_COLLATION_STRLWR      StrLwr;
  // StrUpr
  /// Converts all the characters in a null-terminated string to uppercase characters
  EFI_UNICODE_COLLATION_STRUPR      StrUpr;
  // FatToStr
  /// Converts an 8.3 FAT file name using an OEM character set to a null-terminated string
  EFI_UNICODE_COLLATION_FATTOSTR    FatToStr;
  // StrToFat
  /// Converts a null-terminated string to legal characters in a FAT filename using an OEM character set
  EFI_UNICODE_COLLATION_STRTOFAT    StrToFat;
  // SupportedLanguages
  /// A null-terminated ASCII string array that contains one or more language codes. When this field is used for UnicodeCollation2,
  ///  it is specified in RFC 4646 format. When it is used for UnicodeCollation, it is specified in ISO 639-2 format
  CHAR8                            *SupportedLanguages;

};

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_UNICODE_COLLATION_PROTOCOL_HEADER__
