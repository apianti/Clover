///
/// @file Library/Serialize/XmlStates.h
///
/// GUI XML states
///

#pragma once
#ifndef __GUI_XML_STATES_HEADER__
#define __GUI_XML_STATES_HEADER__

#include <Serialize/Xml.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// XML_DEBUG
/// Whether to debug XML parsing, the following values are valid:
/// 0 - No debug logging or release build
/// 1 - Debug logging
/// 2 - Verbose and debug logging
/// 3 - Extra verbose, verbose, and debug logging
#if !defined(XML_DEBUG)
# define XML_DEBUG 0
#endif

// XML_LOG
/// Debug logging for XML parsing
#if defined(EFI_DEBUG) && defined(XML_DEBUG) && (XML_DEBUG >= 1)
# define XML_LOG(...) LOG(__VA_ARGS__)
#else
# define XML_LOG(...)
#endif
// XML_VERBOSE
/// Verbose logging for XML parsing
#if defined(EFI_DEBUG) && defined(XML_DEBUG) && (XML_DEBUG >= 2)
# define XML_VERBOSE(...) VERBOSE(__VA_ARGS__)
#else
# define XML_VERBOSE(...)
#endif
// XML_WHISPER
/// Extra verbose logging for XML parsing
#if defined(EFI_DEBUG) && defined(XML_DEBUG) && (XML_DEBUG >= 3)
# define XML_WHISPER(...) WHISPER(__VA_ARGS__)
#else
# define XML_WHISPER(...)
#endif

// XML_OPTION_DISABLE_WHITESPACE_TRIMMING
/// Disable whitespace trimming to prevent multiple whitespace characters from being trimmed to a single space character
#define XML_OPTION_DISABLE_WHITESPACE_TRIMMING EFI_BIT(0)

// XML_SCHEMA_TYPE
/// XML document schema declaration type
typedef enum _XML_SCHEMA_TYPE XML_SCHEMA_TYPE;
enum _XML_SCHEMA_TYPE {

  XML_SCHEMA_TYPE_INTERNAL = 0,
  XML_SCHEMA_TYPE_SYSTEM,
  XML_SCHEMA_TYPE_PUBLIC,

};
// XML_STATE
/// XML parser state identifiers
typedef enum _XML_STATE XML_STATE;
enum _XML_STATE {

  XML_LANG_STATE_SIGNATURE = 0,
  XML_LANG_STATE_SIGNATURE_ATTRIBUTE,
  XML_LANG_STATE_SIGNATURE_ATTRIBUTE_VALUE,
  XML_LANG_STATE_TAG,
  XML_LANG_STATE_TAG_NAME,
  XML_LANG_STATE_TAG_ATTRIBUTE,
  XML_LANG_STATE_TAG_ATTRIBUTE_VALUE,
  XML_LANG_STATE_TAG_CLOSE,
  XML_LANG_STATE_TAG_END,
  XML_LANG_STATE_TAG_QUOTE,
  XML_LANG_STATE_TAG_DOUBLE_QUOTE,
  XML_LANG_STATE_QUOTE,
  XML_LANG_STATE_DOUBLE_QUOTE,
  XML_LANG_STATE_ENTITY,
  XML_LANG_STATE_COMMENT,
  XML_LANG_STATE_DOCUMENT_TAG,
  XML_LANG_STATE_DOCUMENT_NAME,
  XML_LANG_STATE_DOCUMENT_TYPE,
  XML_LANG_STATE_DOCUMENT_PUBLIC,
  XML_LANG_STATE_DOCUMENT_PUBLIC_IDENTIFIER,
  XML_LANG_STATE_DOCUMENT_PUBLIC_LOCATION,
  XML_LANG_STATE_DOCUMENT_SYSTEM,
  XML_LANG_STATE_DOCUMENT_SYSTEM_LOCATION,
  XML_LANG_STATE_DOCUMENT_INTERNAL,
  XML_LANG_STATE_DOCUMENT_ENTITY,
  XML_LANG_STATE_DOCUMENT_ENTITY_PARAMETER,
  XML_LANG_STATE_DOCUMENT_ENTITY_VALUE,
  XML_LANG_STATE_DOCUMENT_ENTITY_PUBLIC,
  XML_LANG_STATE_DOCUMENT_ENTITY_SYSTEM,
  XML_LANG_STATE_DOCUMENT_ELEMENT,
  XML_LANG_STATE_DOCUMENT_ELEMENT_ALIAS,
  XML_LANG_STATE_DOCUMENT_ELEMENT_OPEN,
  XML_LANG_STATE_DOCUMENT_ELEMENT_CLOSE,
  XML_LANG_STATE_DOCUMENT_ELEMENT_CONTENT,
  XML_LANG_STATE_DOCUMENT_ATTLIST,
  XML_LANG_STATE_DOCUMENT_ATTLIST_NAME,
  XML_LANG_STATE_DOCUMENT_ATTLIST_TYPE,
  XML_LANG_STATE_DOCUMENT_ATTLIST_ENUMERATION,
  XML_LANG_STATE_DOCUMENT_ATTLIST_PRESENCE,
  XML_LANG_STATE_DOCUMENT_ATTLIST_DEFAULT,
  XML_LANG_STATE_REMOVE_WHITESPACE,
};

// XML_LIST
/// XML document tree node attribute list
typedef struct XML_LIST XML_LIST;
struct XML_LIST {

  // Next
  /// The next attribute in the list
  XML_LIST      *Next;
  // Attribute
  /// The XML document tree node attribute
  XML_ATTRIBUTE  Attribute;

};
// XML_STACK
/// XML document tree stack
typedef struct XML_STACK XML_STACK;
struct XML_STACK {

  // Previous
  /// The previous tree in the stack
  XML_STACK *Previous;
  // Tree
  /// The XML document tree node
  XML_TREE  *Tree;

};

// XML_ENTITY_TYPE
/// XML document schema entity type
typedef enum XML_ENTITY_TYPE XML_ENTITY_TYPE;
enum XML_ENTITY_TYPE {

  // XML_ENTITY_TYPE_GENERAL
  /// XML general entity
  XML_ENTITY_TYPE_GENERAL = 0,
  // XML_ENTITY_TYPE_PARAMETER
  /// XML parameter enity
  XML_ENTITY_TYPE_PARAMETER,
  // XML_ENITY_TYPE_PUBLIC
  /// XML public entity
  XML_ENITY_TYPE_PUBLIC,
  // XML_ENITY_TYPE_PUBLIC_PARAMETER
  /// XML public parameter entity
  XML_ENITY_TYPE_PUBLIC_PARAMETER,
  // XML_ENITY_TYPE_SYSTEM
  /// XML system entity
  XML_ENITY_TYPE_SYSTEM,
  // XML_ENITY_TYPE_SYSTEM_PARAMETER
  /// XML system parameter entity
  XML_ENITY_TYPE_SYSTEM_PARAMETER,

};
// XML_ENTITY
/// XML document schema entity
typedef struct XML_ENTITY XML_ENTITY;
struct XML_ENTITY {

  // Next
  /// The next entity in the list
  XML_ENTITY      *Next;
  // Name
  /// The name of the entity
  CHAR16          *Name;
  // Replacement
  /// The entity replacement text
  CHAR16          *Replacement;
  // Type
  /// The entity type
  XML_ENTITY_TYPE  Type;

};
// XML_ELEMENT_ATTRIBUTE
/// XML document schema element attribute
typedef struct XML_ELEMENT_ATTRIBUTE XML_ELEMENT_ATTRIBUTE;
struct XML_ELEMENT_ATTRIBUTE {

  // Next
  /// The next attribute in the list
  XML_ELEMENT_ATTRIBUTE *Next;
  // Name
  /// The name of the attribute
  CHAR16                *Name;
  // TODO: Other stuff with attribute

};
// XML_ELEMENT_CHILD
/// XML document schema element child
typedef struct XML_ELEMENT_CHILD XML_ELEMENT_CHILD;
struct XML_ELEMENT_CHILD {

  // Reserverd
  /// Reserved
  UINTN Reserved;

};
// XML_ELEMENT
/// XML document schema element
typedef struct XML_ELEMENT XML_ELEMENT;
struct XML_ELEMENT {

  // Next
  /// The next element in the tree
  XML_ELEMENT           *Next;
  // Children
  /// This element's children
  XML_ELEMENT_CHILD     *Children;
  // Attributes
  /// The attributes of the element
  XML_ELEMENT_ATTRIBUTE *Attributes;
  // Name
  /// The name of the element
  CHAR16                *Name;
  // TODO: Other stuff with attribute

};

// XML_TREE
/// XML document tree node
struct XML_TREE {

  // Next
  /// The next tree node
  XML_TREE *Next;
  // Children
  /// The child nodes
  XML_TREE *Children;
  // Name
  /// The tag name
  CHAR16   *Name;
  // Attributes
  /// List of attributes
  XML_LIST *Attributes;
  // Value
  /// Value
  CHAR16   *Value;

};
// XML_SCHEMA
/// XML document schema
struct XML_SCHEMA {

  // Name
  /// XML document schema type name (DOCTYPE)
  CHAR16                *Name;
  // Type
  /// XML document scheme type
  XML_SCHEMA_TYPE        Type;
  // Identifier
  /// Unique identifier for type
  CHAR16                *Identifier;
  // Location
  /// Schema location
  CHAR16                *Location;
  // Attributes
  /// XML document schema attributes
  XML_ELEMENT_ATTRIBUTE *Attributes;
  // Entities
  /// XML document schema entities
  XML_ENTITY            *Entities;
  // Elements
  /// XML document schema element tree
  XML_ELEMENT           *Elements;

};
// XML_DOCUMENT
/// XML document
struct XML_DOCUMENT {

  // Schema
  /// XML document schema
  XML_SCHEMA *Schema;
  // Attributes
  /// XML document attributes
  XML_LIST   *Attributes;
  // Tree
  /// XML document tree root node
  XML_TREE   *Tree;

};
// XML_PARSER
/// XML parser
struct XML_PARSER {

  // Parser
  /// The language parser
  LANG_PARSER           *Parser;
  // Document
  /// XML document
  XML_DOCUMENT          *Document;
  // Stack
  /// XML document tree stack
  XML_STACK             *Stack;
  // Entity
  /// XML document schema entity present only during parsing of entity declaration
  XML_ENTITY            *Entity;
  // Element
  /// XML document schema element present only during parsing of element and attribute list declarations
  XML_ELEMENT           *Element;
  // Attribute
  /// XML document schema element attribute present only during parsing of attribute list declarations
  XML_ELEMENT_ATTRIBUTE *Attribute;
  // Encoding
  /// The detected encoding, if detection took place
  EFI_ENCODING_PROTOCOL *Encoding;
  // Optiona
  /// The parser options
  UINTN                  Options;

};

// XmlDocumentCreate
/// Create an XML parser document
/// @param Document On output, the XML document, which must be freed by XmlDocumentFree
/// @param Name     The name of the document schema root element
/// @return Whether the XML document was created or not
/// @retval EFI_INVALID_PARAMETER If Document is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the XML parser
/// @retval EFI_SUCCESS           If the XML document was created successfully
EXTERN
EFI_STATUS
EFIAPI
XmlDocumentCreate (
  IN XML_DOCUMENT **Document,
  IN CHAR16        *Name OPTIONAL
);

// XmlSchemaCreate
/// Create an XML parser document schema
/// @param Schema On output, the XML document schema, which must be freed by XmlSchemaFree
/// @param Name   The name of the document schema root element
/// @return Whether the XML document schema was created or not
/// @retval EFI_INVALID_PARAMETER If Schema is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the XML parser
/// @retval EFI_SUCCESS           If the XML document schema was created successfully
EXTERN
EFI_STATUS
EFIAPI
XmlSchemaCreate (
  IN XML_SCHEMA **Schema,
  IN CHAR16      *Name OPTIONAL
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __GUI_XML_STATES_HEADER__
