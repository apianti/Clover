///
/// @file Include/Uefi/Protocol/Configuration.h
///
/// Configuration database
///

#pragma once
#ifndef __UEFI_CONFIGURATION_HEADER__
#define __UEFI_CONFIGURATION_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// EFI_CONFIGURATION_PROTOCOL_GUID
/// The configuration protocol unique identifier
#define EFI_CONFIGURATION_PROTOCOL_GUID { 0xD4BABEA3, 0x9FCC, 0x4DA5, { 0x93, 0x4E, 0xA0, 0x3A, 0x1C, 0x3F, 0x7A, 0xA5 } }

// gEfiConfigurationProtocolGuid
/// The configuration protocol unique identifier
EXTERN EFI_GUID gEfiConfigurationProtocolGuid;

// EFI_CONFIGURATION_PROTOCOL
/// Configuration protocol
typedef struct EFI_CONFIGURATION_PROTOCOL EFI_CONFIGURATION_PROTOCOL;

// EFI_CONFIGURATION_KEY_TYPE
/// The type of configuration key
typedef enum EFI_CONFIGURATION_TYPE EFI_CONFIGURATION_TYPE;
enum EFI_CONFIGURATION_TYPE {

  // EfiConfigurationTypeList
  /// The key has children and no value
  EfiConfigurationTypeList = (1 << 0),
  // EfiConfigurationTypeArray
  /// The key has numbered children and no value
  EfiConfigurationTypeArray = (1 << 1),
  // EfiConfigurationTypeDATA
  /// The key value is unformatted binary data with a size in bytes
  EfiConfigurationTypeData = (1 << 2),
  // EfiConfigurationTypeDate
  /// The key value is a date and time
  EfiConfigurationTypeDate = (1 << 3),
  // EfiConfigurationTypeString
  /// The key value is a character string
  EfiConfigurationTypeString = (1 << 4),
  // EfiConfigurationTypeBoolean
  /// The key value is a boolean
  EfiConfigurationTypeBoolean = (1 << 5),
  // EfiConfigurationTypeUnsigned
  /// The key value is an unsigned integer
  EfiConfigurationTypeUnsigned = (1 << 6),
  // EfiConfigurationTypeInteger
  /// The key value is a signed integer
  EfiConfigurationTypeInteger = (1 << 7),
  // EfiConfigurationTypeFloat
  /// The key value is a floating point
  EfiConfigurationTypeFloat = (1 << 8)

};

// EFI_CONFIGURATION_CALLBACK
/// Configuration key enumeration callback
/// @param Key     The full key path identifier
/// @param Name    The key name
/// @param Type    The type of the key value
/// @param Size    The size in bytes of the key value
/// @param Value   The value of the key
/// @param Context The enumeration callback context
/// @return Whether to continue the enumeration or not
typedef
BOOLEAN
(EFIAPI
*EFI_CONFIGURATION_CALLBACK) (
  IN CONST CHAR16           *Key,
  IN CONST CHAR16           *Name,
  IN EFI_CONFIGURATION_TYPE  Type,
  IN UINTN                   Size,
  IN VOID                   *Value,
  IN VOID                   *Context
);

// EFI_CONFIGURATION_EXISTS
/// Check key exists
/// @param This The configuration protocol interface
/// @param Key  The key path identifier
/// @retval EFI_INVALID_PARAMETER If This or Key is NULL
/// @retval EFI_NOT_FOUND         If the key does not exist
/// @retval EFI_SUCCESS           If the key exists
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_EXISTS) (
  IN EFI_CONFIGURATION_PROTOCOL *This,
  IN CONST CHAR16               *Key
);
// EFI_CONFIGURATION_GET_CHILDREN
/// Get child key path identifiers
/// @param This     The configuration protocol interface
/// @param Key      The key path identifier
/// @param Full     Whether to include the full key paths or just the key names
/// @param Count    On output, the count of child key path identifiers
/// @param Children On output, the child key path identifiers, which must be freed by StrListFree
/// @retval EFI_INVALID_PARAMETER If Key, Count, or Children is NULL
/// @retval EFI_OUT_OF_RESOURCES  The child names could not be allocated
/// @retval EFI_UNSUPPORTED       The key is not a list or array type
/// @retval EFI_SUCCESS           The child key names and count were returned successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_GET_CHILDREN) (
  IN  EFI_CONFIGURATION_PROTOCOL   *This,
  IN  CONST CHAR16                 *Key,
  IN  BOOLEAN                       Full,
  OUT UINTN                        *Count,
  OUT CHAR16                     ***Children
);
// EFI_CONFIGURATION_GET
/// Get a configuration value by key
/// @param This  The configuration protocol interface
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @param Type  On output, the type of the value
/// @retval EFI_INVALID_PARAMETER If This, Key, or Value are NULL
/// @retval EFI_SUCCESS           The value, size, and type of the key were returned successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_GET) (
  IN     EFI_CONFIGURATION_PROTOCOL  *This,
  IN     CONST CHAR16                *Key,
  IN     CONST VOID                 **Value,
  IN OUT UINTN                       *Size OPTIONAL,
  OUT    EFI_CONFIGURATION_TYPE      *Type OPTIONAL
);
// EFI_CONFIGURATION_SET
/// Set a configuration value by key
/// @param This  The configuration protocol interface
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @retval EFI_INVALID_PARAMETER If This, Key, or Value is NULL or Size is zero
/// @retval EFI_SUCCESS           The value, size, and type of the key were set successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_SET) (
  IN EFI_CONFIGURATION_PROTOCOL *This,
  IN CONST CHAR16               *Key,
  IN CONST VOID                 *Value,
  IN UINTN                       Size,
  IN EFI_CONFIGURATION_TYPE      Type
);
// EFI_CONFIGURATION_REMOVE
/// Remove key by path identifier
/// @param This The configuration protocol interface
/// @param Key  The key path identifier to remove
/// @retval EFI_INVALID_PARAMETER If This or Key is NULL
/// @retval EFI_SUCESS            The key was removed successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_REMOVE) (
  IN EFI_CONFIGURATION_PROTOCOL *This,
  IN CONST CHAR16               *Key
);
// EFI_CONFIGURATION_ENUMERATE
/// Enumerate keys
/// @param This            The configuration protocol interface
/// @param Key             The key path identifier to enumerate or NULL for root
/// @param TypeFilter      The bitmask of the types of keys to enumerate or zero for all
/// @param Callback        The enumeration callback
/// @param CallbackContext The context passed to each invocation of the enumeration callback
/// @param Recursive       Whether to recursively enumerate child keys
/// @retval EFI_INVALID_PARAMETER If This or Callback is NULL
/// @retval EFI_NOT_FOUND         If the key was not found
/// @retval EFI_ABORTED           If the callback returned FALSE indicating quit enumerating
/// @retval EFI_SUCCESS           If the specified configuration key was enumerated successfully
typedef
EFI_STATUS
(EFIAPI
*EFI_CONFIGURATION_ENUMERATE) (
  IN EFI_CONFIGURATION_PROTOCOL *This,
  IN CONST CHAR16               *Key OPTIONAL,
  IN UINTN                       TypeFilter OPTIONAL,
  IN EFI_CONFIGURATION_CALLBACK  Callback,
  IN VOID                       *CallbackContext OPTIONAL,
  IN BOOLEAN                     Recursive
);

// EFI_CONFIGURATION_PROTOCOL
/// Configuration protocol
struct EFI_CONFIGURATION_PROTOCOL {

  // Exists
  /// Check key exists
  EFI_CONFIGURATION_EXISTS       Exists;
  // GetChildren
  /// Get child key path identifiers
  EFI_CONFIGURATION_GET_CHILDREN GetChildren;
  // Get
  /// Get a configuration value by key
  EFI_CONFIGURATION_GET          Get;
  // Set
  /// Set a configuration value by key
  EFI_CONFIGURATION_SET          Set;
  // Remove
  /// Remove key by path identifier
  EFI_CONFIGURATION_REMOVE       Remove;
  // Enumerate
  /// Enumerate keys
  EFI_CONFIGURATION_ENUMERATE    Enumerate;

};

// EfiConfigurationExists
/// Check key exists
/// @param Key The key path identifier
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_NOT_FOUND         If the key does not exist
/// @retval EFI_SUCCESS           The key exists
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationExists (
  IN CONST CHAR16 *Key
);

// EfiConfigurationGetChildren
/// Get child key path identifiers
/// @param Key      The key path identifier
/// @param Full     Whether to include the full key paths or just the key names
/// @param Count    On output, the count of child key path identifiers
/// @param Children On output, the child key path identifiers, which must be freed by StrListFree, or NULL
/// @retval EFI_INVALID_PARAMETER If Key, Count, or Children is NULL
/// @retval EFI_OUT_OF_RESOURCES  The child names could not be allocated
/// @retval EFI_UNSUPPORTED       The key is not a list or array type
/// @retval EFI_SUCCESS           The child key names and count were returned successfully
EFI_STATUS
EFIAPI
EfiConfigurationGetChildren (
  IN  CONST CHAR16   *Key,
  IN  BOOLEAN         Full,
  OUT UINTN          *Count,
  OUT CHAR16       ***Children
);

// EfiConfigurationGet
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @param Type  On output, the type of the value
/// @retval EFI_INVALID_PARAMETER If Key or Value is NULL
/// @retval EFI_SUCCESS           The value, size, and type of the key were returned successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationGet (
  IN     CONST CHAR16            *Key,
  IN     CONST VOID             **Value,
  IN OUT UINTN                   *Size OPTIONAL,
  OUT    EFI_CONFIGURATION_TYPE  *Type OPTIONAL
);
// EfiConfigurationGetData
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @retval EFI_INVALID_PARAMETER If Key or Value is NULL
/// @retval EFI_SUCCESS           The value, size, and type of the key were returned successfully
EXTERN
CONST VOID *
EFIAPI
EfiConfigurationGetData (
  IN     CONST CHAR16           *Key,
  IN     CONST VOID             *Value OPTIONAL,
  IN OUT UINTN                  *Size OPTIONAL
);
// EfiConfigurationGetDate
/// Set a configuration value by key
/// @param Key  The key path identifier
/// @param Date The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Date is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
CONST EFI_TIME *
EFIAPI
EfiConfigurationGetDate (
  IN CONST CHAR16   *Key,
  IN CONST EFI_TIME *Date
);
// EfiConfigurationGetString
/// Get a configuration value by key
/// @param Key    The key path identifier
/// @param String The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key or String is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
CONST CHAR16 *
EFIAPI
EfiConfigurationGetString (
  IN CONST CHAR16 *Key,
  IN CONST CHAR16 *String
);
// EfiConfigurationGetBoolean
/// Get a configuration value by key
/// @param Key     The key path identifier
/// @param Boolean The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
BOOLEAN
EFIAPI
EfiConfigurationGetBoolean (
  IN CONST CHAR16 *Key,
  IN BOOLEAN       Boolean
);
// EfiConfigurationGetUnsigned
/// Get a configuration value by key
/// @param Key      The key path identifier
/// @param Unsigned The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
UINT64
EFIAPI
EfiConfigurationGetUnsigned (
  IN CONST CHAR16 *Key,
  IN UINT64        Unsigned
);
// EfiConfigurationGetInteger
/// Get a configuration value by key
/// @param Key     The key path identifier
/// @param Integer The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
INT64
EFIAPI
EfiConfigurationGetInteger (
  IN CONST CHAR16 *Key,
  IN INT64         Integer
);
// EfiConfigurationGetFloat
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Float The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key or Float is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
EXTERN
FLOAT64
EFIAPI
EfiConfigurationGetFloat (
  IN CONST CHAR16 *Key,
  IN FLOAT64       Float
);

// EfiConfigurationSet
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @param Type  The configuration value type
/// @retval EFI_INVALID_PARAMETER If This, Key, or Value is NULL or Size is zero
/// @retval EFI_SUCCESS           The value, size, and type of the key were set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSet (
  IN CONST CHAR16           *Key,
  IN CONST VOID             *Value,
  IN UINTN                   Size,
  IN EFI_CONFIGURATION_TYPE  Type
);
// EfiConfigurationSetData
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @retval EFI_INVALID_PARAMETER If Key, or Value is NULL or Size is zero
/// @retval EFI_SUCCESS           The value  of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetData (
  IN CONST CHAR16           *Key,
  IN CONST VOID             *Value,
  IN UINTN                   Size
);
// EfiConfigurationSetDate
/// Set a configuration value by key
/// @param Key  The key path identifier
/// @param Time The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetDate (
  IN CONST CHAR16 *Key,
  IN EFI_TIME     *Time
);
// EfiConfigurationSetString
/// Set a configuration value by key
/// @param Key    The key path identifier
/// @param String The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or String is NULL
/// @retval EFI_SUCCESS           The value of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetString (
  IN CONST CHAR16 *Key,
  IN CONST CHAR16 *String
);
// EfiConfigurationSetBoolean
/// Set a configuration value by key
/// @param Key     The key path identifier
/// @param Boolean The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetBoolean (
  IN CONST CHAR16 *Key,
  IN BOOLEAN       Boolean
);
// EfiConfigurationSetUnsigned
/// Set a configuration value by key
/// @param Key      The key path identifier
/// @param Unsigned The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetUnsigned (
  IN CONST CHAR16 *Key,
  IN UINT64        Unsigned
);
// EfiConfigurationSetInteger
/// Set a configuration value by key
/// @param Key     The key path identifier
/// @param Integer The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetInteger (
  IN CONST CHAR16 *Key,
  IN INT64         Integer
);
// EfiConfigurationSetFloat
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Float The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was set successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationSetFloat (
  IN CONST CHAR16 *Key,
  IN FLOAT64       Float
);

// EfiConfigurationRemove
/// Remove key by path identifier
/// @param Key The key path identifier to remove
/// @retval EFI_INVALID_PARAMETER If This or Key is NULL
/// @retval EFI_SUCESS            The key was removed successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationRemove (
  IN CONST CHAR16 *Key
);
// EfiConfigurationClear
/// Remove all keys from the configuration
/// @retval EFI_SUCESS The configuration was cleared successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationClear (
  VOID
);

// EfiConfigurationEnumerate
/// Enumerate keys
/// @param Key             The key path identifier to enumerate or NULL for root
/// @param TypeFilter      The bitmask of the types of keys to enumerate or zero for all
/// @param Callback        The enumeration callback
/// @param CallbackContext The context passed to each invocation of the enumeration callback
/// @param Recursive       Whether to recursively enumerate child keys
/// @retval EFI_INVALID_PARAMETER If This or Callback is NULL
/// @retval EFI_NOT_FOUND         The key was not found
/// @retval EFI_ABORTED           The callback returned FALSE indicating quit enumerating
/// @retval EFI_SUCCESS           The specified configuration key was enumerated successfully
EXTERN
EFI_STATUS
EFIAPI
EfiConfigurationEnumerate (
  IN CONST CHAR16               *Key OPTIONAL,
  IN UINTN                       TypeFilter OPTIONAL,
  IN EFI_CONFIGURATION_CALLBACK  Callback,
  IN VOID                       *CallbackContext OPTIONAL,
  IN BOOLEAN                     Recursive
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __UEFI_CONFIGURATION_HEADER__
