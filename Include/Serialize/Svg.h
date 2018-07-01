///
/// @file Include/Serialize/Svg.h
///
/// SVG serialization implementation
///

#pragma once
#ifndef __SVG_HEADER__
#define __SVG_HEADER__

#include <Serialize/Xml.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// SVG_IMAGE
/// SVG image
typedef struct SVG_IMAGE SVG_IMAGE;
// SVG_PARSER
/// SVG parser
typedef struct SVG_PARSER SVG_PARSER;

// SvgCreate
/// Create a SVG image parser
/// @param Parser On output, the created SVG parser
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or *Parser is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the SVG parser
/// @retval EFI_SUCCESS           If the SVG parser was created successfully 
EXTERN
EFI_STATUS
EFIAPI
SvgCreate (
  OUT SVG_PARSER   **Parser,
  IN  CONST CHAR16  *Source OPTIONAL
);
// SvgReset
/// Reset a SVG parser for reuse
/// @param Parser The SVG parser to reset
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the SVG parser was reset successfully
EXTERN
EFI_STATUS
EFIAPI
SvgReset (
  IN OUT SVG_PARSER   *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
);
// SvgFree
/// Free a SVG parser
/// @param Parser The SVG parser to free
/// @retval EFI_SUCCESS           The SVG parser was freed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgFree (
  IN SVG_PARSER *Parser
);
// SvgImageFree
/// Free a SVG image
/// @param Image The SVG image to free
/// @retval EFI_SUCCESS           The SVG image was freed
/// @retval EFI_INVALID_PARAMETER If Image is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgImageFree (
  IN SVG_IMAGE *Image
);

// XmlParserFromSvgParser
/// Get the XML parser from a SVG parser
/// @param Parser The SVG parser of which to get the XML parser
/// @return The XML parser associated with the specified SVG parser or NULL if there was an error
EXTERN
XML_PARSER *
EFIAPI
XmlParserFromSvgParser (
  IN SVG_PARSER *Parser
);
// ParserFromSvgParser
/// Get the language parser from a SVG parser
/// @param Parser The SVG parser of which to get the language parser
/// @return The language parser associated with the specified SVG parser or NULL if there was an error
EXTERN
LANG_PARSER *
EFIAPI
ParserFromSvgParser (
  IN SVG_PARSER *Parser
);

// SvgParse
/// Parse a data buffer into a SVG image
/// @param Parser     The SVG parser
/// @param Encoding   The encoding of the XML string buffer
/// @param Buffer     The XML string buffer to parse
/// @param Size       The size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The data buffer was parsed and the SVG image was created
/// @retval EFI_ABORTED           The data buffer was invalid for a SVG image
/// @retval EFI_INVALID_PARAMETER If Image, Parser, or Buffer is NULL
/// @retval EFI_INVALID_PARAMETER If *Image is not NULL
/// @retval EFI_INVALID_PARAMETER If Size is zero
EXTERN
EFI_STATUS
EFIAPI
SvgParse (
  IN  SVG_PARSER            *Parser,
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN  VOID                  *Buffer,
  IN  UINTN                  Size
);
// SvgParseFinish
/// Finish a SVG image
/// @param Parser The SVG parser
/// @param Image  On output, the parsed SVG image
/// @retval EFI_INVALID_PARAMETER If Parser or Image is NULL
/// @retval EFI_SUCCESS           The parsed SVG image was returned successfully
EXTERN
EFI_STATUS
EFIAPI
SvgParseFinish (
  IN OUT SVG_PARSER  *Parser,
  IN     SVG_IMAGE  **Image
);
// SvgParseXml
/// Parse an SVG image from an XML tree
/// @param Tree  The XML tree to parse as an SVG image
/// @param Image On output, the parsed SVG image
/// @retval EFI_INVALID_PARAMETER If Parser or Image is NULL
/// @retval EFI_SUCCESS           The parsed SVG image was returned successfully
EXTERN
EFI_STATUS
EFIAPI
SvgParseXml (
  IN OUT XML_TREE   *Tree,
  IN     SVG_IMAGE **Image
);

// SvgToXml
/// Convert a SVG image to an XML string
/// @param Image    The SVG image
/// @param Encoding The encoding for the XML string buffer
/// @param Buffer   On output, the XML string buffer
/// @param Size      On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The SVG image was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Image or Size is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgToXml (
  IN     SVG_IMAGE             *Image,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size
);
// SvgToXmlBuffer
/// Convert a SVG image to an UTF-16 XML string
/// @param Image The SVG image
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The SVG image was converted to XML
/// @retval EFI_INVALID_PARAMETER If Image or Buffer is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgToXmlBuffer (
  IN  SVG_IMAGE              *Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  OUT VOID                  **Buffer,
  OUT UINTN                  *Size OPTIONAL
);

// SvgLoad
/// Load a SVG image from file
/// @param Image On output, the SVG image
/// @param Encoding   The encoding of the file
/// @param Root       The root file protocol interface
/// @param Path       The path to the SVG file to load or NULL if Root is already the SVG file
/// @retval EFI_SUCCESS           The SVG image was loaded
/// @retval EFI_ABORTED           The SVG file is invalid
/// @retval EFI_NOT_FOUND         The SVG file was not found
/// @retval EFI_INVALID_PARAMETER If Image or Root is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgLoad (
  OUT SVG_IMAGE             **Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  EFI_FILE_PROTOCOL      *Root,
  IN  CONST CHAR16           *Path OPTIONAL
);
// SvgLoadPath
/// Load a SVG image from file
/// @param Image On output, the SVG image
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the SVG file to load
/// @retval EFI_SUCCESS           The SVG image was loaded
/// @retval EFI_ABORTED           The SVG file is invalid
/// @retval EFI_NOT_FOUND         The SVG file was not found
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgLoadPath (
  OUT SVG_IMAGE             **Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST CHAR16           *DevicePath
);
// SvgLoadDevicePath
/// Load a SVG image from file
/// @param Image On output, the SVG image
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the SVG file to load
/// @retval EFI_SUCCESS           The SVG image was loaded
/// @retval EFI_ABORTED           The SVG file is invalid
/// @retval EFI_NOT_FOUND         The SVG file was not found
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgLoadDevicePath (
  OUT SVG_IMAGE                **Image,
  IN  EFI_ENCODING_PROTOCOL     *Encoding OPTIONAL,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
);

// SvgSave
/// Save a SVG image to file
/// @param Image The SVG image to save
/// @param Encoding   The encoding for the file
/// @param Root       The root file protocol interface
/// @param Path       The path where to save to the SVG file or NULL if Root is already the SVG file
/// @retval EFI_SUCCESS           The SVG image was saved
/// @retval EFI_ABORTED           The SVG image is invalid
/// @retval EFI_ACCESS_DENIED     The SVG file could not be saved
/// @retval EFI_INVALID_PARAMETER If Image or Root is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgSave (
  IN SVG_IMAGE             *Image,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
);
// SvgSavePath
/// Save a SVG image to file
/// @param Image The SVG image to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string of the SVG file
/// @retval EFI_SUCCESS           The SVG image was saved
/// @retval EFI_ABORTED           The SVG image is invalid
/// @retval EFI_ACCESS_DENIED     The SVG file could not be saved
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgSavePath (
  IN SVG_IMAGE             *Image,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
);
// SvgSaveDevicePath
/// Save a SVG image to file
/// @param Image The SVG image to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path of the SVG file
/// @retval EFI_SUCCESS           The SVG image was saved
/// @retval EFI_ABORTED           The SVG image is invalid
/// @retval EFI_ACCESS_DENIED     The SVG file could not be saved
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgSaveDevicePath (
  IN SVG_IMAGE                *Image,
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __SVG_HEADER__
