//
/// @file Include/Uefi/String.h
///
/// UEFI implementation string utilities
///

#pragma once
#ifndef ___UEFI_STRING_HEADER__
#define ___UEFI_STRING_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// STR_LIST_NO_DUPLICATES
/// Do not allow duplicate strings in the list
#define STR_LIST_NO_DUPLICATES EFI_BIT(0)
// STR_LIST_CASE_INSENSITIVE
/// Compare strings case insensitively when checking for duplicates in the list, only valid with STR_LIST_NO_DUPLICATES
#define STR_LIST_CASE_INSENSITIVE EFI_BIT(1)
// STR_LIST_SORTED
/// The string list should be sorted
#define STR_LIST_SORTED EFI_BIT(2)
// STR_LIST_INCLUDE_EMPTY
/// The string list can contain empty strings (non-NULL)
#define STR_LIST_INCLUDE_EMPTY EFI_BIT(3)
// STR_SEARCH_CASE_INSENSITIVE
/// Compare strings case insensitively when searching for strings
#define STR_SEARCH_CASE_INSENSITIVE EFI_BIT(4)

// IsUnicodeCharacter
/// Check if character is valid unicode code point
/// @param Character The character to check is valid
/// @retval TRUE  If the character is a valid unicode code point
/// @retval FALSE If the character is invalid in unicode
EXTERN
BOOLEAN
EFIAPI
IsUnicodeCharacter (
  IN UINT32 Character
);
// IsUnicodePrintCharacter
/// Check if character is valid printable unicode code point
/// @param Character The character to check is valid
/// @retval TRUE  If the character is a valid printable unicode code point
/// @retval FALSE If the character is not printable in unicode
EXTERN
BOOLEAN
EFIAPI
IsUnicodePrintCharacter (
  IN UINT32 Character
);

// StrLen
/// Get the length of a character string
/// @param Str The character string
/// @return The count of characters in the character string
EXTERN
UINTN
EFIAPI
StrLen (
  CONST CHAR16 *Str
);
// StrSize
/// Get the size in bytes of a character string (including the null-terminator)
/// @param Str The character string of which to get the size
/// @return The size in bytes of the character string (including the null-terminator)
EXTERN
UINTN
EFIAPI
StrSize (
  CONST CHAR16 *Str
);

// StrDup
/// Duplicate a string
/// @param Str The string to duplicate
/// @return The duplicated string which needs freed or NULL if the string could not be duplicate
EXTERN
CHAR16 *
EFIAPI
StrDup (
  IN CONST CHAR16 *Str
);
// StrnDup
/// Duplicate a string
/// @param Str   The string to duplicate
/// @param Count The maximum count of characters to duplicate
/// @return The duplicated string which needs freed or NULL if the string could not be duplicate
EXTERN
CHAR16 *
EFIAPI
StrnDup (
  IN CONST CHAR16 *Str,
  IN UINTN         Count
);

// StrCmp
/// Performs a comparison of two strings
/// @param Str1 The first string to comapre
/// @param Str2 The second string to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
StrCmp (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2
);
// StrnCmp
/// Performs a comparison of two strings
/// @param Str1  The first string to comapre
/// @param Str2  The second string to compare
/// @param Count The maximum count of characters to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
StrnCmp (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2,
  IN UINTN         Count
);
// StriCmp
/// Performs a case-insensitive comparison of two strings
/// @param Str1 The first string to comapre
/// @param Str2 The second string to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
StriCmp (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2
);
// StrniCmp
/// Performs a case-insensitive comparison of two strings
/// @param Str1  The first string to comapre
/// @param Str2  The second string to compare
/// @param Count The maximum count of characters to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
StrniCmp (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2,
  IN UINTN         Count
);

// StriColl
/// Performs a case-insensitive collation of two strings
/// @param Str1 The first string to collate
/// @param Str2 The second string to collate
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
StriColl (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2
);
// MetaiMatch
/// Performs a case-insensitive comparison of a pattern string and a string
/// @param String  A pointer to a string
/// @param Pattern A pointer to a pattern string
/// @retval TRUE  Pattern was found in String
/// @retval FALSE Pattern was not found in String
EXTERN
BOOLEAN
EFIAPI
MetaiMatch (
  IN CONST CHAR16 *String,
  IN CONST CHAR16 *Pattern
);

// StrLwr
/// Converts all the characters in a string to lower case characters
/// @param Str A pointer to a string
EXTERN
VOID
EFIAPI
StrLwr (
  IN OUT CHAR16 *Str
);
// StrUpr
/// Converts all the characters in a string to upper case characters
/// @param Str A pointer to a string
EXTERN
VOID
EFIAPI
StrUpr (
  IN OUT CHAR16 *Str
);

// StrToLower
/// Duplicates all the characters in a string to lower case characters
/// @param Str A pointer to a string
/// @return A duplicate string with all lower case characters, which must be freed or NULL if there was an error
EXTERN
CHAR16 *
EFIAPI
StrToLower (
  IN CONST CHAR16 *Str
);
// StrToUpper
/// Duplicates all the characters in a string to upper case characters
/// @param Str A pointer to a string
/// @return A duplicate string with all upper case characters, which must be freed or NULL if there was an error
EXTERN
CHAR16 *
EFIAPI
StrToUpper (
  IN CONST CHAR16 *Str
);

// StrStr
/// Returns the first occurrence of a sub-string in a string
/// @param String       A pointer to a string
/// @param SearchString A pointer to a string for which to search
/// @return A pointer to the matched sub-string in String
/// @retval NULL If SearchString was not found in String
EXTERN
CHAR16 *
EFIAPI
StrStr (
  IN CHAR16       *String,
  IN CHAR16 *SearchString
);
// StriStr
/// Returns the first occurrence of a case-insensitive sub-string in a string
/// @param String       A pointer to a string
/// @param SearchString A pointer to a string for which to search
/// @return A pointer to the matched sub-string in String
/// @retval NULL If SearchString was not found in String
EXTERN
CHAR16 *
EFIAPI
StriStr (
  IN CHAR16       *String,
  IN CHAR16 *SearchString
);

// StrAppend
/// Append a character to a string
/// @param String    On input, the string to which to append or NULL to allocate a new string, on output, the possibly reallocated string, which needs freed
/// @param Count     On input, the length count, in characters, of the string, on output, the length count, in characters, of the string
/// @param Size      On input, the allocated size, in characters, of the string, on output, the allocated size, in characters, of the string
/// @param Character The character to append to the string
/// @return Whether the character was appended to the string or not
/// @retval EFI_INVALID_PARAMETER If String, *String, Count, or Size is NULL or Character is invalid
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string
/// @retval EFI_SUCCESS           If the character was appended to the string successfully
EXTERN
EFI_STATUS
EFIAPI
StrAppend (
  IN OUT CHAR16 **String,
  IN OUT UINTN   *Count,
  IN OUT UINTN   *Size,
  IN     UINT32   Character
);

// StrSplit
/// Split a string into a list
/// @param List    On output, the string list, which must be freed with StrListFree
/// @param Count   On output, the count of strings in the list
/// @param Str     The string to split
/// @param Split   The string that splits
/// @param Options The options for the string list
/// @return Whether the string was split or not
/// @retval EFI_INVALID_PARAMETER If List, Count, Str, or Split is NULL or *List is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string list
/// @retval EFI_NOT_FOUND         If the split string could not be found
/// @retval EFI_SUCCESS           If the string was split into a list successfully
EXTERN
EFI_STATUS
EFIAPI
StrSplit (
  OUT CHAR16 ***List,
  OUT UINTN    *Count,
  IN  CHAR16   *Str,
  IN  CHAR16   *Split,
  IN  UINTN     Options
);
// StrList
/// Append a string to a list
/// @param List    The list to append the string, which must be freed with StrListFree, *List may be NULL to start new list
/// @param Count   The current count of strings in the list
/// @param Str     The string to append to the list
/// @param Length  The maximum length of the string or zero to use the entire string
/// @param Options The options for adding the strings
/// @return Whether the string was added to the list or not
/// @retval EFI_INVALID_PARAMETER If List, Count, or Str is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the memory for the list could not be allocated
/// @retval EFI_ACCESS_DENIED     If STR_LIST_NO_DUPLICATES was used and the string already exists
/// @retval EFI_SUCCESS           If the string was added to the list successfully
EXTERN
EFI_STATUS
EFIAPI
StrList (
  IN OUT CHAR16 ***List,
  IN OUT UINTN    *Count,
  IN     CHAR16   *Str,
  IN     UINTN     Length,
  IN     UINTN     Options
);
// StrListDup
/// Duplicate a string list
/// @param Duplicate On output, the duplicated string list which must be free with StrListFree
/// @param Count     The count of strings
/// @param List      The string list to duplicate
/// @return Whether the string list was duplicated or not
/// @return EFI_INVALID_PARAMETER If Duplicate or List is NULL or *Duplicate is not NULL or Count is zero
/// @return EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @return EFI_SUCCESS           If the string list was duplicated successfully
EXTERN
EFI_STATUS
EFIAPI
StrListDup (
  OUT CHAR16 ***Duplicate,
  IN  UINTN     Count,
  IN  CHAR16  **List
);
// StrListFree
/// Free a string list
/// @param List  The string list to free
/// @param Count The count of strings in the list
/// @return Whether the string list was freed or not
/// @return EFI_INVALID_PARAMETER If List is NULL or Count is zero
/// @return EFI_SUCCESS           If the string list was freed successfully
EXTERN
EFI_STATUS
EFIAPI
StrListFree (
  IN CHAR16 **List,
  IN UINTN    Count
);

// StrToBoolean
/// Convert a string to boolean
/// @param Str     On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Boolean On output, the converted boolean value
/// @retval EFI_INVALID_PARAMETER If Boolean is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
StrToBoolean (
  IN OUT CONST CHAR16 **Str,
  OUT    BOOLEAN       *Boolean
);
// StrToUnsigned
/// Convert a string to unsigned
/// @param Str      On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Unsigned On output, the converted unsigned value
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If Unsigned is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
StrToUnsigned (
  IN OUT CONST CHAR16 **Str,
  OUT    UINT64        *Unsigned,
  IN     UINTN          Base
);
// StrToInteger
/// Convert a string to integer
/// @param Str     On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Integer On output, the converted integer value
/// @param Base    The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If Integer is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
StrToInteger (
  IN OUT CONST CHAR16 **Str,
  OUT    INT64         *Integer,
  IN     UINTN          Base
);
// StrToFloat
/// Convert a string to a floating point
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Float On output, the converted float value
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
StrToFloat (
  IN OUT CONST CHAR16 **Str,
  OUT    FLOAT64       *Float
);
// StrToDateTime
/// Convert a date and/or time formatted character string buffer to a date and time
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param DateTime On output, the converted date and time
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
StrToDateTime (
  IN OUT CONST CHAR16 **Str,
  IN OUT EFI_TIME      *DateTime
);
// StrToGuid
/// Convert a GUID formatted character string buffer to a GUID
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Guid  On output, the converted GUID
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Guid is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a GUID successfully
EXTERN
EFI_STATUS
EFIAPI
StrToGuid (
  IN OUT CONST CHAR16 **Str,
  IN OUT EFI_GUID      *Guid
);

// StrFromStatus
/// Convert a status code to string
/// @param Status The status code to convert
/// @return The string for the status code
EXTERN
CONST CHAR16 *
EFIAPI
StrFromStatus (
  EFI_STATUS Status
);

// StrToAscii
/// Convert UTF-16 to UTF-8
/// @param String The UTF-16 string to convert
/// @return The converted UTF-8 string
EXTERN
CHAR8 *
EFIAPI
StrToAscii (
  IN CONST CHAR16 *String
);
// StrFromAscii
/// Convert UTF-16 to UTF-8
/// @param String The UTF-16 string to convert
/// @return The converted UTF-8 string
EXTERN
CHAR16 *
EFIAPI
StrFromAscii (
  IN CONST CHAR8 *String
);

// AsciiStrLen
/// Get the length of a character string
/// @param Str The character string
/// @return The count of characters in the character string
EXTERN
UINTN
EFIAPI
AsciiStrLen (
  CONST CHAR8 *Str
);
// AsciiStrSize
/// Get the size in bytes of a character string (including the null-terminator)
/// @param Str The character string of which to get the size
/// @return The size in bytes of the character string (including the null-terminator)
EXTERN
UINTN
EFIAPI
AsciiStrSize (
  CONST CHAR8 *Str
);

// AsciiStrDup
/// Duplicate a string
/// @param Str The string to duplicate
/// @return The duplicated string which needs freed or NULL if the string could not be duplicate
EXTERN
CHAR8 *
EFIAPI
AsciiStrDup (
  IN CONST CHAR8 *Str
);
// AsciiStrnDup
/// Duplicate a string
/// @param Str   The string to duplicate
/// @param Count The maximum count of characters to duplicate
/// @return The duplicated string which needs freed or NULL if the string could not be duplicate
EXTERN
CHAR8 *
EFIAPI
AsciiStrnDup (
  IN CONST CHAR8 *Str,
  IN UINTN        Count
);

// AsciiStrCmp
/// Performs a comparison of two strings
/// @param Str1 The first string to comapre
/// @param Str2 The second string to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
AsciiStrCmp (
  IN CONST CHAR8 *Str1,
  IN CONST CHAR8 *Str2
);
// AsciiStrnCmp
/// Performs a comparison of two strings
/// @param Str1  The first string to comapre
/// @param Str2  The second string to compare
/// @param Count The maximum count of characters to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
AsciiStrnCmp (
  IN CONST CHAR8 *Str1,
  IN CONST CHAR8 *Str2,
  IN UINTN        Count
);
// AsciiStriCmp
/// Performs a case-insensitive comparison of two strings
/// @param Str1 The first string to comapre
/// @param Str2 The second string to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
AsciiStriCmp (
  IN CONST CHAR8 *Str1,
  IN CONST CHAR8 *Str2
);
// AsciiStrniCmp
/// Performs a case-insensitive comparison of two strings
/// @param Str1  The first string to comapre
/// @param Str2  The second string to compare
/// @param Count The maximum count of characters to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
AsciiStrniCmp (
  IN CONST CHAR8 *Str1,
  IN CONST CHAR8 *Str2,
  IN UINTN        Count
);

// AsciiStriColl
/// Performs a case-insensitive comparison of two strings
/// @param Str1 The first string to comapre
/// @param Str2 The second string to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
EXTERN
INTN
EFIAPI
AsciiStriColl (
  IN CONST CHAR8 *Str1,
  IN CONST CHAR8 *Str2
);
// AsciiMetaiMatch
/// Performs a case-insensitive comparison of a pattern string and a string
/// @param String  A pointer to a string
/// @param Pattern A pointer to a pattern string
/// @retval TRUE  Pattern was found in String
/// @retval FALSE Pattern was not found in String
EXTERN
BOOLEAN
EFIAPI
AsciiMetaiMatch (
  IN CONST CHAR8 *String,
  IN CONST CHAR8 *Pattern
);

// AsciiLwr
/// Converts all the characters in a string to lower case characters
/// @param Str A pointer to a string
EXTERN
VOID
EFIAPI
AsciiLwr (
  IN OUT CHAR8 *Str
);
// AsciiUpr
/// Converts all the characters in a string to upper case characters
/// @param Str A pointer to a string
EXTERN
VOID
EFIAPI
AsciiUpr (
  IN OUT CHAR8 *Str
);

// AsciiToLower
/// Duplicates all the characters in a string to lower case characters
/// @param Str A pointer to a string
/// @return A duplicate string with all lower case characters, which must be freed or NULL if there was an error
EXTERN
CHAR8 *
EFIAPI
AsciiToLower (
  IN CONST CHAR8 *Str
);
// AsciiToUpper
/// Duplicates all the characters in a string to upper case characters
/// @param Str A pointer to a string
/// @return A duplicate string with all upper case characters, which must be freed or NULL if there was an error
EXTERN
CHAR8 *
EFIAPI
AsciiToUpper (
  IN CONST CHAR8 *Str
);

// AsciiStrStr
/// Returns the first occurrence of a sub-string in a string
/// @param String       A pointer to a string
/// @param SearchString A pointer to a string for which to search
/// @return A pointer to the matched sub-string in String
/// @retval NULL If SearchString was not found in String
EXTERN
CHAR8 *
EFIAPI
AsciiStrStr (
  IN CHAR8 *String,
  IN CHAR8 *SearchString
);
// AsciiStriStr
/// Returns the first occurrence of a case-insensitive sub-string in a string
/// @param String       A pointer to a string
/// @param SearchString A pointer to a string for which to search
/// @return A pointer to the matched sub-string in String
/// @retval NULL If SearchString was not found in String
EXTERN
CHAR8 *
EFIAPI
AsciiStriStr (
  IN CHAR8 *String,
  IN CHAR8 *SearchString
);

// AsciiStrAppend
/// Append a character to a string
/// @param String    On input, the string to which to append or NULL to allocate a new string, on output, the possibly reallocated string, which needs freed
/// @param Count     On input, the length count, in characters, of the string, on output, the length count, in characters, of the string
/// @param Size      On input, the allocated size, in characters, of the string, on output, the allocated size, in characters, of the string
/// @param Character The character to append to the string
/// @return Whether the character was appended to the string or not
/// @retval EFI_INVALID_PARAMETER If String, *String, Count, or Size is NULL or Character is invalid
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string
/// @retval EFI_SUCCESS           If the character was appended to the string successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrAppend (
  IN OUT CHAR16 **String,
  IN OUT UINTN   *Count,
  IN OUT UINTN   *Size,
  IN     UINT32   Character
);

// AsciiStrSplit
/// Split a string into a list
/// @param List    On output, the string list, which must be freed with AsciiStrListFree
/// @param Count   On output, the count of strings in the list
/// @param Str     The string to split
/// @param Split   The string that splits
/// @param Options The options for the string list
/// @return Whether the string was split or not
/// @retval EFI_INVALID_PARAMETER If List, Count, Str, or Split is NULL or *List is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the string list
/// @retval EFI_NOT_FOUND         If the split string could not be found
/// @retval EFI_SUCCESS           If the string was split into a list successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrSplit (
  OUT CHAR8 ***List,
  OUT UINTN   *Count,
  IN  CHAR8   *Str,
  IN  CHAR8   *Split,
  IN  UINTN    Options
);
// AsciiStrList
/// Append a string to a list
/// @param List    The list to append the string, which must be freed with AsciiStrListFree, *List may be NULL to start new list
/// @param Count   The current count of strings in the list
/// @param Str     The string to append to the list
/// @param Length  The maximum length of the string or zero to use the entire string
/// @param Options The options for adding the strings
/// @return Whether the string was added to the list or not
/// @retval EFI_INVALID_PARAMETER If List, Count, or Str is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the memory for the list could not be allocated
/// @retval EFI_ACCESS_DENIED     If STR_LIST_NO_DUPLICATES was used and the string already exists
/// @retval EFI_SUCCESS           If the string was added to the list successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrList (
  IN OUT CHAR8 ***List,
  IN OUT UINTN   *Count,
  IN     CHAR8   *Str,
  IN     UINTN    Length,
  IN     UINTN    Options
);
// AsciiStrListFree
/// Free a string list
/// @param List  The string list to free
/// @param Count The count of strings in the list
/// @return Whether the string list was freed or not
/// @return EFI_INVALID_PARAMETER If List is NULL or Count is zero
/// @return EFI_SUCCESS           If the string list was freed successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrListFree (
  IN CHAR8 **List,
  IN UINTN   Count
);

// AsciiStrToBoolean
/// Convert a string to boolean
/// @param Str     On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Boolean On output, the converted boolean value
/// @retval EFI_INVALID_PARAMETER If Str, *Str, or Boolean is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToBoolean (
  IN OUT CONST CHAR8 **Str,
  OUT    BOOLEAN      *Boolean
);
// AsciiStrToUnsigned
/// Convert a string to unsigned
/// @param Str      On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Unsigned On output, the converted unsigned value
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If Unsigned is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToUnsigned (
  IN OUT CONST CHAR8 **Str,
  OUT    UINT64       *Unsigned,
  IN     UINTN         Base
);
// AsciiStrToInteger
/// Convert a string to integer
/// @param Str     On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Integer On output, the converted integer value
/// @param Base     The base of the integer (usually 10 for decimal)
/// @retval EFI_INVALID_PARAMETER If Integer is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToInteger (
  IN OUT CONST CHAR8 **Str,
  OUT    INT64        *Integer,
  IN     UINTN         Base
);
// AsciiStrToFloat
/// Convert a string to a floating point
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Float On output, the converted float value
/// @retval EFI_INVALID_PARAMETER If Float is NULL
/// @retval EFI_NOT_FOUND         The string did not contain any valid characters to convert
/// @retval EFI_SUCCESS           The string was converted and the string pointer was updated to point after any characters used in converting
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToFloat (
  IN OUT CONST CHAR8 **Str,
  OUT    FLOAT64      *Float
);
// AsciiStrToDateTime
/// Convert a date and/or time formatted character string buffer to a date and time
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param DateTime On output, the converted date and time
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or DateTime is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a floating point successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToDateTime (
  IN OUT CONST CHAR16 **Str,
  IN OUT EFI_TIME      *DateTime
);
// AsciiStrToGuid
/// Convert a GUID formatted character string buffer to a GUID
/// @param Str   On input, the address of the position in the string to start converting, on output, the address of the new position in the string after converting
/// @param Guid  On output, the converted GUID
/// @retval EFI_INVALID_PARAMETER  If This, Encoding, Buffer, *Buffer, or Guid is NULL
/// @retval EFI_NOT_FOUND          If the buffer is not valid for conversion
/// @retval EFI_SUCCESS            If the buffer was converted to a GUID successfully
EXTERN
EFI_STATUS
EFIAPI
AsciiStrToGuid (
  IN OUT CONST CHAR16 **Str,
  IN OUT EFI_GUID      *Guid
);

// AsciiStrFromStatus
/// Convert a status code to string
/// @param Status The status code to convert
/// @return The string for the status code
EXTERN
CONST CHAR8 *
EFIAPI
AsciiStrFromStatus (
  EFI_STATUS Status
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // ___UEFI_STRING_HEADER__
