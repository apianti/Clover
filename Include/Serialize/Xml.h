///
/// @file Include/Serialize/Xml.h
///
/// XML serialization implementation
///

#pragma once
#ifndef __XML_HEADER__
#define __XML_HEADER__

#include <Serialize/Parser.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// XML_ATTRIBUTE
/// XML document tree node attribute
typedef struct XML_ATTRIBUTE XML_ATTRIBUTE;
struct XML_ATTRIBUTE {

  // Name
  /// XML document tree node attribute name
  CHAR16 *Name;
  // Value
  /// XML document tree node attribute value
  CHAR16 *Value;

};
// XML_TREE
/// XML document tree node
typedef struct XML_TREE XML_TREE;
// XML_SCHEMA
/// XML document schema
typedef struct XML_SCHEMA XML_SCHEMA;
// XML_DOCUMENT
/// XML document
typedef struct XML_DOCUMENT XML_DOCUMENT;
// XML_PARSER
/// XML parser
typedef struct XML_PARSER XML_PARSER;

// XML_INSPECT
/// XML document tree inspection callback
/// @param Tree           The document tree node
/// @param Level          The level of generation of tree nodes, zero for the root
/// @param LevelIndex     The index of the tree node relative to the previous level
/// @param TagName        The tree node tag name
/// @param Value          The tree node value
/// @param AttributeCount The tree node attribute count
/// @param Attributes     The tree node attributes
/// @param ChildCount     The tree node child count
/// @param Children       The tree node children
/// @param Context        The context passed when inspection started
/// @retval TRUE  If the inspection should continue
/// @retval FALSE If the inspection should stop
typedef
BOOLEAN
(EFIAPI
*XML_INSPECT) (
  IN XML_TREE       *Tree,
  IN UINTN           Level,
  IN UINTN           LevelIndex,
  IN CHAR16         *TagName,
  IN CHAR16         *Value OPTIONAL,
  IN UINTN           AttributeCount,
  IN XML_ATTRIBUTE **Attributes OPTIONAL,
  IN UINTN           ChildCount,
  IN XML_TREE      **Children OPTIONAL,
  IN VOID           *Context OPTIONAL
);

// XmlCreate
/// Create an XML parser
/// @param Parser On output, the XML parser, which must be freed by XmlFree
/// @param Source The source being parsed
/// @return Whether the XML parser was created or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or *Parser is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the XML parser
/// @retval EFI_SUCCESS           If the XML parser was created successfully
EXTERN
EFI_STATUS
EFIAPI
XmlCreate (
  OUT XML_PARSER   **Parser,
  IN  CONST CHAR16  *Source OPTIONAL
);
// XmlReset
/// Reset an XML parser for reuse
/// @param Parser The XML parser
/// @param Source The source being parsed
/// @return Whether the XML parser was reset or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the XML parser was reset successfully
EXTERN
EFI_STATUS
EFIAPI
XmlReset (
  IN OUT XML_PARSER   *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
);
// XmlFree
/// Free an XML parser
/// @param Parser The XML parser to free
/// @return Whether the XML parser was freed or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the XML parser was freed successfully
EXTERN
EFI_STATUS
EFIAPI
XmlFree (
  IN XML_PARSER *Parser
);

// ParserFromXmlParser
/// Get the language parser from an XML parser
/// @param Parser The XML parser of which to get the language parser
/// @return The language parser associated with the specified XML parser or NULL if there was an error
EXTERN
LANG_PARSER *
EFIAPI
ParserFromXmlParser (
  IN XML_PARSER *Parser
);

// XmlParse
/// Parse a buffer for XML and finish the XML document
/// @param Parser   An XML parser used to parse
/// @param Encoding The encoding of the buffer
/// @param Buffer   The buffer to parse
/// @param Size     The size, in bytes, of the buffer to parse
/// @return Whether the buffer was parsed or not
EXTERN
EFI_STATUS
EFIAPI
XmlParse (
  IN OUT XML_PARSER            *Parser,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     VOID                  *Buffer,
  IN     UINTN                  Size
);
// XmlParseNext
/// The next buffer to parse in parsing multiple buffers for XML
/// @param Parser   An XML parser used to parse
/// @param Encoding The encoding of the buffer
/// @param Buffer   The buffer to parse
/// @param Size     The size, in bytes, of the buffer to parse
/// @return Whether the buffer was parsed or not
EXTERN
EFI_STATUS
EFIAPI
XmlParseNext (
  IN OUT XML_PARSER            *Parser,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     VOID                  *Buffer,
  IN     UINTN                  Size
);
// XmlParseFinish
/// Finish parsing multiple buffers for XML and finish the XML document
/// @param Parser An XML parser to finish
/// @return Whether the XML document was finished or not
EXTERN
EFI_STATUS
EFIAPI
XmlParseFinish (
  IN OUT XML_PARSER *Parser
);

// XmlSetParseSource
/// Set the source being parsed
/// @param Parser The XML parser used for parsing
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the source could not be changed
/// @retval EFI_SUCCESS           The source was changed successfully
EXTERN
EFI_STATUS
EFIAPI
XmlSetParseSource (
  IN XML_PARSER   *Parser,
  IN CONST CHAR16 *Source OPTIONAL
);

// XmlInspect
/// Inspect the XML document tree
/// @param Parser    The XML parser
/// @param Inspector The inspection callback
/// @param Context   The context to pass to the inspection callback
/// @param Recursive Whether the inspection should include child nodes
/// @return Whether the inspection finished or not
/// @retval EFI_INVALID_PARAMETER If Parser or Inspector is NULL
/// @retval EFI_ABORTED           If inspection was aborted by the callback
/// @retval EFI_SUCCESS           If inspection finished
EXTERN
EFI_STATUS
EFIAPI
XmlInspect (
  IN XML_PARSER  *Parser,
  IN XML_INSPECT  Inspector OPTIONAL,
  IN VOID        *Context OPTIONAL,
  IN BOOLEAN      Recursive
);
// XmlDocumentInspect
/// Inspect the XML document tree
/// @param Document  The XML document
/// @param Inspector The inspection callback
/// @param Context   The context to pass to the inspection callback
/// @param Recursive Whether the inspection should include child nodes
/// @return Whether the inspection finished or not
/// @retval EFI_INVALID_PARAMETER If Document or Inspector is NULL
/// @retval EFI_ABORTED           If inspection was aborted by the callback
/// @retval EFI_SUCCESS           If inspection finished
EXTERN
EFI_STATUS
EFIAPI
XmlDocumentInspect (
  IN XML_DOCUMENT *Document,
  IN XML_INSPECT   Inspector OPTIONAL,
  IN VOID         *Context OPTIONAL,
  IN BOOLEAN       Recursive
);
// XmlTreeInspect
/// Inspect the XML document tree
/// @param Tree       The XML document tree node
/// @param Level      The level of generation of tree nodes, zero for the root
/// @param LevelIndex The index of the tree node relative to the previous level
/// @param Inspector  The inspection callback
/// @param Context    The context to pass to the inspection callback
/// @param Recursive Whether the inspection should include child nodes
/// @return Whether the inspection finished or not
/// @retval EFI_INVALID_PARAMETER If Tree or Inspector is NULL
/// @retval EFI_ABORTED           If inspection was aborted by the callback
/// @retval EFI_SUCCESS           If inspection finished
EXTERN
EFI_STATUS
EFIAPI
XmlTreeInspect (
  IN XML_TREE    *Tree,
  IN UINTN        Level,
  IN UINTN        LevelIndex,
  IN XML_INSPECT  Inspector OPTIONAL,
  IN VOID        *Context OPTIONAL,
  IN BOOLEAN      Recursive
);

// XmlDefaultInspector
/// Default XML document tree inspection callback that logs inspection
/// @param Tree           The document tree node
/// @param Level          The level of generation of tree nodes, zero for the root
/// @param LevelIndex     The index of the tree node relative to the previous level
/// @param TagName        The tree node tag name
/// @param Value          The tree node value
/// @param AttributeCount The tree node attribute count
/// @param Attributes     The tree node attributes
/// @param ChildCount     The tree node child count
/// @param Children       The tree node children
/// @param Context        The context passed when inspection started
/// @retval TRUE  If the inspection should continue
/// @retval FALSE If the inspection should stop
EXTERN
BOOLEAN
EFIAPI
XmlDefaultInspector (
  IN XML_TREE       *Tree,
  IN UINTN           Level,
  IN UINTN           LevelIndex,
  IN CHAR16         *TagName,
  IN CHAR16         *Value OPTIONAL,
  IN UINTN           AttributeCount,
  IN XML_ATTRIBUTE **Attributes OPTIONAL,
  IN UINTN           ChildCount,
  IN XML_TREE      **Children OPTIONAL,
  IN VOID           *Context OPTIONAL
);

// XmlGetDocument
/// Get XML document
/// @param Parser   An XML parser
/// @param Document On output, the XML document
/// @return Whether the XML document was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Document is NULL
/// @retval EFI_SUCCESS           If the XML document was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlGetDocument (
  IN  XML_PARSER    *Parser,
  OUT XML_DOCUMENT **Document
);
// XmlGetSchema
/// Get XML document schema
/// @param Parser An XML parser
/// @param Schema On output, the XML document schema
/// @return Whether the XML document schema was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document schema was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlGetSchema (
  IN  XML_PARSER  *Parser,
  OUT XML_SCHEMA **Schema
);
// XmlGetTree
/// Get XML document tree root node
/// @param Parser An XML parser
/// @param Tree   On output, the XML document tree root node
/// @return Whether the XML document tree root node was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document tree root node was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlGetTree (
  IN  XML_PARSER  *Parser,
  OUT XML_TREE   **Tree
);

// XmlDocumentGetSchema
/// Get XML document schema
/// @param Document An XML document
/// @param Schema   On output, the XML document schema
/// @return Whether the XML document schema was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Document or Schema is NULL
/// @retval EFI_SUCCESS           If the XML document schema was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlDocumentGetSchema (
  IN  XML_DOCUMENT  *Document,
  OUT XML_SCHEMA   **Schema
);
// XmlDocumentSetSchema
/// Set XML document schema
/// @param Document An XML document
/// @param Schema   The XML document schema to set
/// @return Whether the XML document schema was set or not
/// @retval EFI_INVALID_PARAMETER If Document or Schema is NULL
/// @retval EFI_SUCCESS           If the XML document schema was set successfully
EXTERN
EFI_STATUS
EFIAPI
XmlDocumentSetSchema (
  IN OUT XML_DOCUMENT *Document,
  IN     XML_SCHEMA   *Schema
);
// XmlDocumentGetTree
/// Get XML document tree root node
/// @param Document An XML document
/// @param Tree     On output, the XML document tree root node
/// @return Whether the XML document tree root node was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Document or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document tree root node was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlDocumentGetTree (
  IN  XML_DOCUMENT  *Document,
  OUT XML_TREE     **Tree
);

// XmlTreeGetTag
/// Get XML document tree node tag name
/// @param Tree An XML document tree
/// @param Tag  On output, the XML document tree tag name
/// @return Whether the XML document tree tag name was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree or Tag is NULL
/// @retval EFI_SUCCESS           If the XML document tree tag name was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeGetTag (
  IN  XML_TREE  *Tree,
  OUT CHAR16   **Tag
);
// XmlTreeSetTag
/// Set XML document tree node tag name
/// @param Tree An XML document tree
/// @param Tag  The XML document tree tag name to set
/// @return Whether the XML document tree tag name was set or not
/// @retval EFI_INVALID_PARAMETER If Tree or Tag is NULL
/// @retval EFI_SUCCESS           If the XML document tree tag name was set successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeSetTag (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Tag
);
// XmlTreeGetValue
/// Get XML document tree node value
/// @param Tree  An XML document tree
/// @param Value On output, the XML document tree value
/// @return Whether the XML document tree node value was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree or Value is NULL
/// @retval EFI_SUCCESS           If the XML document tree node value was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeGetValue (
  IN  XML_TREE  *Tree,
  OUT CHAR16   **Value
);
// XmlTreeSetValue
/// Set XML document tree node value
/// @param Tree  An XML document tree
/// @param Value The XML document tree node value to set
/// @return Whether the XML document tree node value was set or not
/// @retval EFI_INVALID_PARAMETER If Tree or Value is NULL
/// @retval EFI_SUCCESS           If the XML document tree node value was set successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeSetValue (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Value
);
// XmlTreeHasChildren
/// Check if XML document tree node has child nodes
/// @param Tree An XML document tree node
/// @return Whether the XML document tree node has child nodes or not
/// @retval TRUE  If the tree node has children
/// @retval FALSE If the tree node does not have children
EXTERN
BOOLEAN
EFIAPI
XmlTreeHasChildren (
  IN  XML_TREE *Tree
);
// XmlTreeGetChildren
/// Get XML document tree node child nodes
/// @param Tree     An XML document tree
/// @param Children On output, the XML document tree node child nodes, which must be freed
/// @param Count    On output, the count of children
/// @return Whether the XML document tree child nodes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Children, or Count is NULL or *Children is not NULL
/// @retval EFI_NOT_FOUND         If there are no XML document tree child nodes
/// @retval EFI_SUCCESS           If the XML document tree child nodes were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeGetChildren (
  IN  XML_TREE   *Tree,
  OUT XML_TREE ***Children,
  OUT UINTN      *Count
);
// XmlTreeGetAttributes
/// Get XML document tree node attributes
/// @param Tree       An XML document tree
/// @param Attributes On output, the XML document tree node attributes, which must be freed
/// @param Count      On output, the count of attributes
/// @return Whether the XML document tree attributes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Attributes, or Count is NULL or *Attributes is not NULL
/// @retval EFI_NOT_FOUND         If there are no XML document tree attibutes
/// @retval EFI_SUCCESS           If the XML document tree attributes were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeGetAttributes (
  IN  XML_TREE        *Tree,
  OUT XML_ATTRIBUTE ***Attributes,
  OUT UINTN           *Count
);
// XmlTreeGetAttribute
/// Get an XML document tree node attribute
/// @param Tree      An XML document tree
/// @param Name      The XML document tree node attribute name
/// @param Attribute On output, the XML document tree attribute
/// @return Whether the XML document tree attribute was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Name, or Attribute is NULL
/// @retval EFI_NOT_FOUND         If the XML document tree attibute was not found
/// @retval EFI_SUCCESS           If the XML document tree attribute was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeGetAttribute (
  IN  XML_TREE       *Tree,
  IN  CHAR16         *Name,
  OUT XML_ATTRIBUTE **Attribute
);
// XmlTreeSetAttribute
/// Set an XML document tree node attribute
/// @param Tree      An XML document tree
/// @param Name      The XML document tree node attribute name
/// @param Attribute The XML document tree attribute to set
/// @return Whether the XML document tree attribute was set or not
/// @retval EFI_INVALID_PARAMETER If Tree, Name, or Attribute is NULL
/// @retval EFI_SUCCESS           If the XML document tree attribute was set successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeSetAttribute (
  IN OUT XML_TREE      *Tree,
  IN     CHAR16        *Name,
  IN     XML_ATTRIBUTE *Attribute
);
// XmlTreeRemoveAttribute
/// Remove an attribute from XML document tree node
/// @param Tree An XML document tree
/// @param Name The name of the XML document tree node attribute to remove
/// @return Whether the XML document tree attribute was removed or not
/// @retval EFI_INVALID_PARAMETER If Tree or Name is NULL
/// @retval EFI_SUCCESS           If the XML document tree attribute was removed successfully
EXTERN
EFI_STATUS
EFIAPI
XmlTreeRemoveAttribute (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Name
);

// XmlIsValidName
/// Check if token is valid tag or attribute name
/// @param Token On input, the token to check is valid name, on output, the position in the token that invalidates the name or NULL if the token is a valid name
/// @retval EFI_INVALID_PARAMETER If Token is NULL or *Token is NULL
/// @retval EFI_NO_MAPPING        If the token is not a valid name, *Token is updated with the position of the invalid character
/// @retval EFI_SUCCESS           If the token is a valid name, *Token is NULL
EXTERN
EFI_STATUS
EFIAPI
XmlIsValidName (
  IN OUT CHAR16 **Token
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __XML_HEADER__
