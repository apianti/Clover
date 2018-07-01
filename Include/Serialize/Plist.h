///
/// @file Include/Serialize/Plist.h
///
/// PLIST serialization implementation
///

#pragma once
#ifndef __PLIST_HEADER__
#define __PLIST_HEADER__

#include <Serialize/Xml.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// PLIST_KEY
/// PLIST key
typedef struct PLIST_KEY PLIST_KEY;
// PLIST_TYPE
/// PLIST key value type
typedef enum PLIST_TYPE PLIST_TYPE;
// PLIST_VALUE
/// PLIST key value
typedef union PLIST_VALUE PLIST_VALUE;
// PLIST_PARSER
/// PLIST parser
typedef struct PLIST_PARSER PLIST_PARSER;

// PLIST_TYPE
/// PLIST key value type
enum PLIST_TYPE {

  // PlistTypeDictionary
  /// PLIST dictionary type
  PlistTypeDictionary = 0,
  // PlistTypeArray
  /// PLIST array type
  PlistTypeArray,
  // PlistTypeString
  /// PLIST string type
  PlistTypeString,
  // PlistTypeDate
  /// PLIST date type
  PlistTypeDate,
  // PlistTypeData
  /// PLIST data type
  PlistTypeData,
  // PlistTypeReal
  /// PLIST real type
  PlistTypeReal,
  // PlistTypeUnsigned
  /// PLIST unsigned type
  PlistTypeUnsigned,
  // PlistTypeInteger
  /// PLIST integer type
  PlistTypeInteger,
  // PlistTypeBoolean
  /// PLIST boolean type
  PlistTypeBoolean

};
// PLIST_VALUE
/// PLIST key value
union PLIST_VALUE {

  // Dictionary
  /// PLIST dictionary and array value
  PLIST_KEY   *Dictionary;
  // String
  /// PLIST string value
  CHAR16      *String;
  // Date
  /// PLIST date value
  EFI_TIME    *Date;
  // Data
  /// PLIST data value
  VOID        *Data;
  // Real
  /// PLIST real value
  FLOAT64      Real;
  // Unsigned
  /// PLIST unsigned value
  UINT64       Unsigned;
  // Integer
  /// PLIST integer value
  INT64        Integer;
  // Boolean
  /// PLIST boolean value
  BOOLEAN      Boolean;

};
// PLIST_KEY
/// PLIST key
struct PLIST_KEY {

  // Next
  /// PLIST next key
  PLIST_KEY   *Next;
  // Name
  /// PLIST key name
  CHAR16      *Name;
  // Size
  /// PLIST key value size in bytes
  UINTN        Size;
  // Value
  /// PLIST key value
  PLIST_VALUE  Value;
  // Type
  /// PLIST key value type
  PLIST_TYPE   Type;

};

// PlistCreate
/// Create a PLIST dictionary parser
/// @param Parser On output, the created PLIST parser
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or *Parser is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the PLIST parser
/// @retval EFI_SUCCESS           If the PLIST parser was created successfully 
EXTERN
EFI_STATUS
EFIAPI
PlistCreate (
  OUT PLIST_PARSER **Parser,
  IN  CONST CHAR16  *Source OPTIONAL
);
// PlistReset
/// Reset a PLIST parser for reuse
/// @param Parser The PLIST parser to reset
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the PLIST parser was reset successfully
EXTERN
EFI_STATUS
EFIAPI
PlistReset (
  IN OUT PLIST_PARSER *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
);
// PlistFree
/// Free a PLIST parser
/// @param Parser The PLIST parser to free
/// @retval EFI_SUCCESS           The PLIST parser was freed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistFree (
  IN PLIST_PARSER *Parser
);
// PlistDictionaryFree
/// Free a PLIST dictionary
/// @param Dictionary The PLIST dictionary to free
/// @retval EFI_SUCCESS           The PLIST dictionary was freed
/// @retval EFI_INVALID_PARAMETER If Dictionary is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistDictionaryFree (
  IN PLIST_KEY *Dictionary
);

// XmlParserFromPlistParser
/// Get the XML parser from a PLIST parser
/// @param Parser The PLIST parser of which to get the XML parser
/// @return The XML parser associated with the specified PLIST parser or NULL if there was an error
EXTERN
XML_PARSER *
EFIAPI
XmlParserFromPlistParser (
  IN PLIST_PARSER *Parser
);
// ParserFromPlistParser
/// Get the language parser from a PLIST parser
/// @param Parser The PLIST parser of which to get the language parser
/// @return The language parser associated with the specified PLIST parser or NULL if there was an error
EXTERN
LANG_PARSER *
EFIAPI
ParserFromPlistParser (
  IN PLIST_PARSER *Parser
);

// PlistParse
/// Parse a data buffer into a PLIST dictionary
/// @param Parser     The PLIST parser
/// @param Encoding   The encoding of the XML string buffer
/// @param Buffer     The XML string buffer to parse
/// @param Size       The size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The data buffer was parsed and the PLIST dictionary was created
/// @retval EFI_ABORTED           The data buffer was invalid for a PLIST dictionary
/// @retval EFI_INVALID_PARAMETER If Dictionary, Parser, or Buffer is NULL
/// @retval EFI_INVALID_PARAMETER If *Dictionary is not NULL
/// @retval EFI_INVALID_PARAMETER If Size is zero
EXTERN
EFI_STATUS
EFIAPI
PlistParse (
  IN  PLIST_PARSER          *Parser,
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN  VOID                  *Buffer,
  IN  UINTN                  Size
);
// PlistParseFinish
/// Finish a PLIST dictionary
/// @param Parser     The PLIST parser
/// @param Dictionary On output, the parsed PLIST dictionary
/// @retval EFI_INVALID_PARAMETER If Parser or Dictionary is NULL
/// @retval EFI_SUCCESS           The parsed PLIST dictionary was returned successfully
EXTERN
EFI_STATUS
EFIAPI
PlistParseFinish (
  IN OUT PLIST_PARSER  *Parser,
  IN     PLIST_KEY    **Dictionary
);

// PlistToXml
/// Convert a PLIST dictionary to an XML string
/// @param Dictionary The PLIST dictionary
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Dictionary or Size is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistToXml (
  IN     PLIST_KEY             *Dictionary,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size
);
// PlistToXmlBuffer
/// Convert a PLIST dictionary to an UTF-16 XML string
/// @param Dictionary The PLIST dictionary
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_INVALID_PARAMETER If Dictionary or Buffer is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistToXmlBuffer (
  IN  PLIST_KEY              *Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  OUT VOID                  **Buffer,
  OUT UINTN                  *Size OPTIONAL
);

// PlistImportConfiguration
/// Import a PLIST dictionary into the configuration
/// @param Dictionary The PLIST dictionary to import
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was imported
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_INVALID_PARAMETER If Dictionary is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistImportConfiguration (
  IN PLIST_KEY    *Dictionary,
  IN CONST CHAR16 *KeyPath OPTIONAL
);
// PlistExportConfiguration
/// Export the configuration into a PLIST dictionary
/// @param Dictionary On output, the exported configuration PLIST dictionary which must be freed with PlistFree
/// @retval EFI_SUCCESS           The PLIST dictionary was exported
/// @retval EFI_INVALID_PARAMETER If Dictionary is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistExportConfiguration (
  OUT PLIST_KEY **Dictionary
);

// PlistLoad
/// Load a PLIST dictionary from file
/// @param Dictionary On output, the PLIST dictionary
/// @param Encoding   The encoding of the file
/// @param Root       The root file protocol interface
/// @param Path       The path to the PLIST file to load or NULL if Root is already the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Dictionary or Root is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoad (
  OUT PLIST_KEY             **Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  EFI_FILE_PROTOCOL      *Root,
  IN  CONST CHAR16           *Path OPTIONAL
);
// PlistLoadPath
/// Load a PLIST dictionary from file
/// @param Dictionary On output, the PLIST dictionary
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the PLIST file to load
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoadPath (
  OUT PLIST_KEY             **Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST CHAR16           *DevicePath
);
// PlistLoadDevicePath
/// Load a PLIST dictionary from file
/// @param Dictionary On output, the PLIST dictionary
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the PLIST file to load
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoadDevicePath (
  OUT PLIST_KEY                **Dictionary,
  IN  EFI_ENCODING_PROTOCOL     *Encoding OPTIONAL,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
);

// PlistSave
/// Save a PLIST dictionary to file
/// @param Dictionary The PLIST dictionary to save
/// @param Encoding   The encoding for the file
/// @param Root       The root file protocol interface
/// @param Path       The path where to save to the PLIST file or NULL if Root is already the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was saved
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Dictionary or Root is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistSave (
  IN PLIST_KEY             *Dictionary,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
);
// PlistSavePath
/// Save a PLIST dictionary to file
/// @param Dictionary The PLIST dictionary to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string of the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was saved
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistSavePath (
  IN PLIST_KEY             *Dictionary,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
);
// PlistSaveDevicePath
/// Save a PLIST dictionary to file
/// @param Dictionary The PLIST dictionary to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path of the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was saved
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistSaveDevicePath (
  IN PLIST_KEY                *Dictionary,
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

// PlistLoadAndImportConfiguration
/// Load a PLIST dictionary from file and import into the configuration
/// @param Encoding The encoding of the file
/// @param Root     The root file protocol interface
/// @param Path     The path to the PLIST dictionary file to load or NULL if Root is already the PLIST file
/// @param KeyPath  The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded and imported
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Root is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoadAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL,
  IN CONST CHAR16          *KeyPath OPTIONAL
);
// PlistLoadPathAndImportConfiguration
/// Load a PLIST dictionary from file and import into the configuration
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the PLIST dictionary file to load
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded and imported
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoadPathAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath,
  IN CONST CHAR16          *KeyPath OPTIONAL
);
// PlistLoadDevicePathAndImportConfiguration
/// Load a PLIST dictionary from file and import into the configuration
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the PLIST dictionary file to load
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded and imported
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistLoadDevicePathAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST CHAR16             *KeyPath OPTIONAL
);

// PlistExportConfigurationAndSave
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding The encoding for the file
/// @param Root     The root file protocol interface
/// @param Path     The path where to save the PLIST dictionary file or NULL if Root is already the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Root is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSave (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
);
// PlistExportConfigurationAndSavePath
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string where to save the PLIST dictionary file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSavePath (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
);
// PlistExportConfigurationAndSaveDevicePath
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path where to save the PLIST dictionary file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSaveDevicePath (
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __PLIST_HEADER__
