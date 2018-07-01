///
/// @file Library/Serialize/Xml.c
///
/// XML implementation
///

#include "XmlStates.h"

// XmlAttributeDuplicateMembers
/// @param Destination The destination XML document tree node attribute
/// @param Source      The source XML document tree node attribute
STATIC
VOID
EFIAPI
XmlAttributeDuplicateMembers (
  IN XML_ATTRIBUTE *Destination,
  IN XML_ATTRIBUTE *Source
) {
  if (Destination != NULL) {
    if (Destination->Name != NULL) {
      EfiFreePool(Destination->Name);
      Destination->Name = NULL;
    }
    if (Destination->Value != NULL) {
      EfiFreePool(Destination->Value);
      Destination->Value = NULL;
    }
    if (Source != NULL) {
      Destination->Name = StrDup(Source->Name);
      Destination->Value = StrDup(Source->Value);
    }
  }
}
// XmlAttributeFreeMembers
/// Free XML document tree node attribute
/// @param Attribute The XML document tree node attribute
STATIC
VOID
EFIAPI
XmlAttributeFreeMembers (
  IN XML_ATTRIBUTE *Attribute
) {
  if (Attribute != NULL) {
    if (Attribute->Name != NULL) {
      EfiFreePool(Attribute->Name);
      Attribute->Name = NULL;
    }
    if (Attribute->Value != NULL) {
      EfiFreePool(Attribute->Value);
      Attribute->Value = NULL;
    }
  }
}
// XmlAttributeFree
/// Free XML document tree node attribute
/// @param Attribute The XML document tree node attribute
VOID
EFIAPI
XmlAttributeFree (
  IN XML_ATTRIBUTE *Attribute
) {
  // Check parameters
  if (Attribute != NULL) {
    XmlAttributeFreeMembers(Attribute);
    EfiFreePool(Attribute);
  }
}
// XmlAttributeListFree
/// Free XML document tree node attribute
/// @param Attribute The XML document tree node attribute
STATIC
VOID
EFIAPI
XmlAttributeListFree (
  IN XML_LIST *Attribute
) {
  // Check parameters
  if (Attribute != NULL) {
    XmlAttributeFreeMembers(&(Attribute->Attribute));
    EfiFreePool(Attribute);
  }
}
// XmlTreeFree
/// Free XML document tree node
/// @param Tree The XML document tree node
STATIC
VOID
EFIAPI
XmlTreeFree (
  IN XML_TREE *Tree
) {
  if (Tree != NULL) {
    if (Tree->Name != NULL) {
      EfiFreePool(Tree->Name);
      Tree->Name = NULL;
    }
    if (Tree->Value != NULL) {
      EfiFreePool(Tree->Value);
      Tree->Value = NULL;
    }
    while (Tree->Attributes != NULL) {
      XML_LIST *Attribute = Tree->Attributes;
      Tree->Attributes = Attribute->Next;
      XmlAttributeListFree(Attribute);
    }
    if (Tree->Children != NULL) {
      XmlTreeFree(Tree->Children);
      Tree->Children = NULL;
    }
    if (Tree->Next != NULL) {
      XmlTreeFree(Tree->Next);
      Tree->Next = NULL;
    }
    EfiFreePool(Tree);
  }
}
// XmlEntityFree
/// Free XML schema entity
/// @param Entity The XML schema entity
STATIC
VOID
EFIAPI
XmlEntityFree (
  IN XML_ENTITY *Entity
) {
  if (Entity != NULL) {
    if (Entity->Name != NULL) {
      EfiFreePool(Entity->Name);
      Entity->Name = NULL;
    }
    if (Entity->Replacement != NULL) {
      EfiFreePool(Entity->Replacement);
      Entity->Replacement = NULL;
    }
    EfiFreePool(Entity);
  }
}
// XmlElementAttributeFree
/// Free XML schema element attribute
/// @param Attribute The XML schema element attribute
STATIC
VOID
EFIAPI
XmlElementAttributeFree (
  IN XML_ELEMENT_ATTRIBUTE *Attribute
) {
  if (Attribute != NULL) {
    if (Attribute->Name != NULL) {
      EfiFreePool(Attribute->Name);
      Attribute->Name = NULL;
    }
    EfiFreePool(Attribute);
  }
}
// XmlElementFree
/// Free XML schema element
/// @param Element The XML schema element
STATIC
VOID
EFIAPI
XmlElementFree (
  IN XML_ELEMENT *Element
) {
  if (Element != NULL) {
    if (Element->Name != NULL) {
      EfiFreePool(Element->Name);
      Element->Name = NULL;
    }
    while (Element->Attributes != NULL) {
      XML_ELEMENT_ATTRIBUTE *Attribute = Element->Attributes;
      Element->Attributes = Attribute->Next;
      XmlElementAttributeFree(Attribute);
    }
    EfiFreePool(Element);
  }
}
// XmlSchemaFree
/// Free XML schema
/// @param Schema The XML schema
STATIC
VOID
EFIAPI
XmlSchemaFree (
  IN XML_SCHEMA *Schema
) {
  if (Schema != NULL) {
    if (Schema->Name != NULL) {
      EfiFreePool(Schema->Name);
      Schema->Name = NULL;
    }
    if (Schema->Identifier != NULL) {
      EfiFreePool(Schema->Identifier);
      Schema->Identifier = NULL;
    }
    if (Schema->Location != NULL) {
      EfiFreePool(Schema->Location);
      Schema->Location = NULL;
    }
    while (Schema->Entities != NULL) {
      XML_ENTITY *Entity = Schema->Entities;
      Schema->Entities = Entity->Next;
      XmlEntityFree(Entity);
    }
    while (Schema->Elements != NULL) {
      XML_ELEMENT *Element = Schema->Elements;
      Schema->Elements = Element->Next;
      XmlElementFree(Element);
    }
    EfiFreePool(Schema);
  }
}
// XmlSchemaDuplicate
/// @param Schema The XML schema to duplicate
/// @return The duplicated XML schema
STATIC
XML_SCHEMA *
EFIAPI
XmlSchemaDuplicate (
  IN XML_SCHEMA *Schema
) {
  XML_SCHEMA *Duplicate;
  // Check parameters
  if (Schema == NULL) {
    return NULL;
  }
  Duplicate = EfiAllocateByType(XML_SCHEMA);
  if (Duplicate != NULL) {
    Duplicate->Type = Schema->Type;
    Duplicate->Name = StrDup(Schema->Name);
    Duplicate->Identifier = StrDup(Schema->Identifier);
    Duplicate->Location = StrDup(Schema->Location);
  }
  return Duplicate;
}
// XmlDocumentFree
/// Free XML document
/// @param Document The XML document
STATIC
VOID
EFIAPI
XmlDocumentFree (
  IN XML_DOCUMENT *Document
) {
  if (Document != NULL) {
    if (Document->Schema != NULL) {
      XmlSchemaFree(Document->Schema);
      Document->Schema = NULL;
    }
    if (Document->Tree != NULL) {
      XmlTreeFree(Document->Tree);
      Document->Tree = NULL;
    }
    while (Document->Attributes != NULL) {
      XML_LIST *Attribute = Document->Attributes;
      Document->Attributes = Attribute->Next;
      XmlAttributeListFree(Attribute);
    }
    EfiFreePool(Document);
  }
}

// XmlSchemaCreate
/// Create an XML parser document schema
/// @param Schema On output, the XML document schema, which must be freed by XmlSchemaFree
/// @param Name   The name of the document schema root element
/// @return Whether the XML document schema was created or not
/// @retval EFI_INVALID_PARAMETER If Schema is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the XML parser
/// @retval EFI_SUCCESS           If the XML document schema was created successfully
EFI_STATUS
EFIAPI
XmlSchemaCreate (
  IN XML_SCHEMA **Schema,
  IN CHAR16      *Name OPTIONAL
) {
  XML_SCHEMA *Sch;
  // Check parameters
  if (Schema == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there is a schema already
  Sch = *Schema;
  if (Sch == NULL) {
    // Allocate document schema
    Sch = EfiAllocateByType(XML_SCHEMA);
    if (Sch == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Set the document schema defaults
    Sch->Elements = NULL;
    Sch->Attributes = NULL;
    Sch->Entities = NULL;
    Sch->Identifier = NULL;
    Sch->Location = NULL;
    Sch->Type = XML_SCHEMA_TYPE_INTERNAL;
  } else {
    // Only allow the name to be set if not already
    if (Sch->Name != NULL) {
      return EFI_SUCCESS;
    }
  }
  if ((Name != NULL) && (*Name != L'\0')) {
    Sch->Name = StrDup(Name);
    if (Sch->Name == NULL) {
      EfiFreePool(Sch);
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    Sch->Name = NULL;
  }
  // Return the created document
  *Schema = Sch;
  return EFI_SUCCESS;
}

// XmlDocumentCreate
/// Create an XML parser document
/// @param Document On output, the XML document, which must be freed by XmlDocumentFree
/// @param Name     The name of the document schema root element
/// @return Whether the XML document was created or not
/// @retval EFI_INVALID_PARAMETER If Document is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the XML parser
/// @retval EFI_SUCCESS           If the XML document was created successfully
EFI_STATUS
EFIAPI
XmlDocumentCreate (
  IN XML_DOCUMENT **Document,
  IN CHAR16        *Name OPTIONAL
) {
  EFI_STATUS    Status;
  XML_DOCUMENT *Doc;
  // Check parameters
  if (Document == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if the document already exists
  Doc = *Document;
  if (Doc == NULL) {
    // Allocate document
    Doc = EfiAllocateByType(XML_DOCUMENT);
    if (Doc == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    // Set the document defaults
    Doc->Tree = NULL;
    Doc->Schema = NULL;
  }
  // Create the document schema if needed
  Status = XmlSchemaCreate(&(Doc->Schema), Name);
  if (EFI_ERROR(Status)) {
    EfiFreePool(Doc);
    return Status;
  }
  // Return the created document
  *Document = Doc;
  return Status;
}

// XmlReset
/// Reset an XML parser for reuse
/// @param Parser The XML parser
/// @param Source The source being parsed
/// @return Whether the XML parser was reset or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the XML parser was reset successfully
EFI_STATUS
EFIAPI
XmlReset (
  IN OUT XML_PARSER   *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
) {
  EFI_STATUS Status = EFI_SUCCESS;
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Remove the document contents
  Parser->Element = NULL;
  Parser->Attribute = NULL;
  Parser->Encoding = NULL;
  Parser->Entity = NULL;
  Parser->Options = 0;
  while (Parser->Stack != NULL) {
    XML_STACK *Stack = Parser->Stack;
    Parser->Stack = Stack->Previous;
    EfiFreePool(Stack);
  }
  if (Parser->Document != NULL) {
    // Store the current schema
    XML_SCHEMA *Schema = Parser->Document->Schema;
    Parser->Document->Schema = NULL;
    // Free the current document without the schema
    XmlDocumentFree(Parser->Document);
    // Create a new document
    Parser->Document = NULL;
    Status = XmlDocumentCreate(&(Parser->Document), NULL);
    if (!EFI_ERROR(Status) && (Schema != NULL)) {
      // Free the empty schema created with the document if needed
      if (Parser->Document->Schema != NULL) {
        XmlSchemaFree(Parser->Document->Schema);
      }
      // Restore the document schema
      Parser->Document->Schema = Schema;
    }
  }
  // Reset the language parser
  if (!EFI_ERROR(Status)) {
    Status = ResetParser(Parser->Parser, XML_LANG_STATE_SIGNATURE, Source);
  }
  return Status;
}
// XmlFree
/// Free an XML parser
/// @param Parser The XML parser to free
/// @return Whether the XML parser was freed or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the XML parser was freed successfully
EFI_STATUS
EFIAPI
XmlFree (
  IN XML_PARSER *Parser
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Parser->Document != NULL) {
    XmlDocumentFree(Parser->Document);
    Parser->Document = NULL;
  }
  while (Parser->Stack != NULL) {
    XML_STACK *Stack = Parser->Stack;
    Parser->Stack = Stack->Previous;
    EfiFreePool(Stack);
  }
  if (Parser->Parser != NULL) {
    FreeParser(Parser->Parser);
    Parser->Parser = NULL;
  }
  EfiFreePool(Parser);
  return EFI_SUCCESS;
}

// ParserFromXmlParser
/// Get the language parser from an XML parser
/// @param Parser The XML parser of which to get the language parser
/// @return The language parser associated with the specified XML parser or NULL if there was an error
LANG_PARSER *
EFIAPI
ParserFromXmlParser (
  IN XML_PARSER *Parser
) {
  if (Parser == NULL) {
    return NULL;
  }
  return Parser->Parser;
}

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
) {
  EFI_STATUS Status;
  // Parse the buffer
  Status = XmlParseNext(Parser, Encoding, Buffer, Size);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Finish XML document
  return XmlParseFinish(Parser);
}
// XmlDetectEncoding
/// Detect the encoding
/// @param Parser An XML parser
/// @param Buffer The buffer to parse
/// @param Size   The size, in bytes, of the buffer to parse
/// @return Whether the encoding was detected or not
STATIC
EFI_STATUS
EFIAPI
XmlDetectEncoding (
  IN OUT XML_PARSER  *Parser,
  IN     VOID        *Buffer,
  IN     UINTN        Size
) {
  // Check parameters
  if ((Parser == NULL) || (Buffer == NULL) || (Size <= sizeof(UINT32))) {
    return EFI_INVALID_PARAMETER;
  }
  // Check for byte order
  switch (*((UINT32 *)Buffer)) {

    //
    // Unicode detection
    //
    case 0x003CFEFF:
      // BOM followed by <
    case 0x0020FEFF:
      // BOM followed by space
    case 0x000DFEFF:
      // BOM followed by carriage return
    case 0x000AFEFF:
      // BOM followed by line feed
    case 0x003F003C:
      // <?
    case 0x003C0020:
      // Space followed by <
    case 0x003C000D:
      // Carriage return followed by <
    case 0x003C000A:
      // Line feed followed by <
    case 0x00200020:
      // Two spaces
    case 0x0020000D:
      // Carriage return followed by space
    case 0x0020000A:
      // Line feed followed by space
    case 0x000D0020:
      // Space followed by carriage return
    case 0x000D000D:
      // Two carriage returns
    case 0x000D000A:
      // Line feed followed by carriage return
    case 0x000A0020:
      // Space followed by line feed
    case 0x000A000D:
      // Carriage return followed by line feed
    case 0x000A000A:
      // Two line feeds

      // UTF-16 encoding
      Parser->Encoding = EfiUtf16Encoding();
      break;

    //
    // Unicode with swapped byte order detection
    //
    case 0x3C00FFFE:
      // BOM followed by <
    case 0x2000FFFE:
      // BOM followed by space
    case 0x0D00FFFE:
      // BOM followed by carriage return
    case 0x0A00FFFE:
      // BOM followed by line feed
    case 0x3F003C00:
      // <?
    case 0x3C002000:
      // Space followed by <
    case 0x3C000D00:
      // Carriage return followed by <
    case 0x3C000A00:
      // Line feed followed by <
    case 0x20002000:
      // Two spaces
    case 0x20000D00:
      // Carriage return followed by space
    case 0x20000A00:
      // Line feed followed by space
    case 0x0D002000:
      // Space followed by carriage return
    case 0x0D000D00:
      // Two carriage returns
    case 0x0D000A00:
      // Line feed followed by carriage return
    case 0x0A002000:
      // Space followed by line feed
    case 0x0A000D00:
      // Carriage return followed by line feed
    case 0x0A000A00:
      // Two line feeds

      // UTF-16 byte swapped encoding
      Parser->Encoding = EfiUtf16SwappedEncoding();
      break;

    default:
      // Unknown so assume UTF-8 unless the document specifies otherwise
      Parser->Encoding = EfiUtf8Encoding();
      break;
  }
  return (Parser->Encoding == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
}
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
) {
  EFI_STATUS Status;
  // Check parameters
  if ((Parser == NULL) || (Buffer == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Create the XML document
  if (Parser->Document == NULL) {
    Status = XmlDocumentCreate(&(Parser->Document), NULL);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  // Check if there is an encoding
  if (Encoding == NULL) {
    // First try to get previous encoding from the parser
    Encoding = Parser->Encoding;
    if (Encoding == NULL) {
      // Try to determine encoding
      Status = XmlDetectEncoding(Parser, Buffer, Size);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      // Get the detected encoding if any
      Encoding = Parser->Encoding;
      if (Encoding == NULL) {
        return EFI_NOT_FOUND;
      }
    }
  }
  // Parse the buffer
  return Parse(Parser->Parser, Encoding, Buffer, Size, 0, Parser);
}
// XmlParseFinish
/// Finish parsing multiple buffers for XML and finish the XML document
/// @param Parser An XML parser to finish
/// @return Whether the XML document was finished or not
EFI_STATUS
EFIAPI
XmlParseFinish (
  IN OUT XML_PARSER *Parser
) {
  // Check parameters
  if ((Parser == NULL) || (Parser->Document == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Finishing parsing for any partially complete token
  return ParseFinish(Parser->Parser, Parser);
}

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
) {
  UINTN Index;
  UNUSED_PARAMETER(Tree);
  UNUSED_PARAMETER(LevelIndex);
  UNUSED_PARAMETER(Context);
  // Log XML
  LOG(L"% *a<%s", Level * 2, "", TagName);
  if ((Attributes != NULL) && (AttributeCount != 0)) {
    for (Index = 0; Index < AttributeCount; ++Index) {
      if ((Attributes[Index] != NULL) && (Attributes[Index]->Name != NULL)) {
        if (Attributes[Index]->Value != NULL) {
          LOG(L" %s=\"%s\"", Attributes[Index]->Name, Attributes[Index]->Value);
        } else {
          LOG(L" %s=", Attributes[Index]->Name);
        }
      }
    }
  }
  if ((Children != NULL) && (ChildCount != 0)) {
    if (Value != NULL) {
      LOG(L">\n% *a%s\n", Level * 2, "", Value);
    } else {
      LOG(L">\n");
    }
    ++Level;
    for (Index = 0; Index < ChildCount; ++Index) {
      // Inspect each child
      if (EFI_ERROR(XmlTreeInspect(Children[Index], Level, Index, XmlDefaultInspector, Context, FALSE))) {
        return FALSE;
      }
    }
    --Level;
    LOG(L"% *a</%s>\n", Level * 2, "", TagName);
  } else if (Value != NULL) {
    LOG(L">%s</%s>\n", Value, TagName);
  } else {
    LOG(L" />\n");
  }
  return TRUE;
}

// XmlSetParseSource
/// Set the source being parsed
/// @param Parser The XML parser used for parsing
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the source could not be changed
/// @retval EFI_SUCCESS           The source was changed successfully
EFI_STATUS
EFIAPI
XmlSetParseSource (
  IN XML_PARSER   *Parser,
  IN CONST CHAR16 *Source OPTIONAL
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Set the parser source
  return SetParseSource(Parser->Parser, Source);
}

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
EFI_STATUS
EFIAPI
XmlInspect (
  IN XML_PARSER  *Parser,
  IN XML_INSPECT  Inspector OPTIONAL,
  IN VOID        *Context,
  IN BOOLEAN      Recursive
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((Inspector == NULL) || (Inspector == XmlDefaultInspector)) {
    Inspector = XmlDefaultInspector;
    Recursive = FALSE;
  }
  return XmlDocumentInspect(Parser->Document, Inspector, Context, Recursive);
}
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
EFI_STATUS
EFIAPI
XmlDocumentInspect (
  IN XML_DOCUMENT *Document,
  IN XML_INSPECT   Inspector OPTIONAL,
  IN VOID         *Context,
  IN BOOLEAN       Recursive
) {
  // Check parameters
  if (Document == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((Inspector == NULL) || (Inspector == XmlDefaultInspector)) {
    Inspector = XmlDefaultInspector;
    Recursive = FALSE;
  }
  return XmlTreeInspect(Document->Tree, 0, 0, Inspector, Context, Recursive);
}
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
EFI_STATUS
EFIAPI
XmlTreeInspect (
  IN XML_TREE    *Tree,
  IN UINTN        Level,
  IN UINTN        LevelIndex,
  IN XML_INSPECT  Inspector OPTIONAL,
  IN VOID        *Context OPTIONAL,
  IN BOOLEAN      Recursive
) {
  BOOLEAN         Success;
  XML_TREE      **Children;
  XML_ATTRIBUTE **Attributes;
  UINTN           ChildCount;
  UINTN           AttributeCount;
  // Check parameters
  if (Tree == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if ((Inspector == NULL) || (Inspector == XmlDefaultInspector)) {
    Inspector = XmlDefaultInspector;
    Recursive = FALSE;
  }
  // Get attributes
  Attributes = NULL;
  AttributeCount = 0;
  XmlTreeGetAttributes(Tree, &Attributes, &AttributeCount);
  // Get children
  Children = NULL;
  ChildCount = 0;
  XmlTreeGetChildren(Tree, &Children, &ChildCount);
  // Inspection callback
  Success = Inspector(Tree, Level++, LevelIndex, Tree->Name, Tree->Value, AttributeCount, Attributes, ChildCount, Children, Context);
  if (Attributes != NULL) {
    EfiFreePool(Attributes);
  }
  if (Children != NULL) {
    EfiFreePool(Children);
  }
  if (!Success) {
    return EFI_ABORTED;
  }
  // Check if recursive inspection
  if (Recursive) {
    UINTN     Index = 0;
    XML_TREE *Child = Tree->Children;
    while (Child != NULL) {
      // Inspect each child
      EFI_STATUS Status = XmlTreeInspect(Child, Level, Index++, Inspector, Context, Recursive);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      Child = Child->Next;
    }
  }
  return EFI_SUCCESS;
}

// XmlGetDocument
/// Get XML document
/// @param Parser   An XML parser
/// @param Document On output, the XML document
/// @return Whether the XML document was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Document is NULL
/// @retval EFI_SUCCESS           If the XML document was retrieved successfully
EFI_STATUS
EFIAPI
XmlGetDocument (
  IN  XML_PARSER    *Parser,
  OUT XML_DOCUMENT **Document
) {
  // Check parameters
  if ((Parser == NULL) || (Document == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Document = Parser->Document;
  return EFI_SUCCESS;
}
// XmlGetSchema
/// Get XML document schema
/// @param Parser An XML parser
/// @param Schema On output, the XML document schema
/// @return Whether the XML document schema was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document schema was retrieved successfully
EFI_STATUS
EFIAPI
XmlGetSchema (
  IN  XML_PARSER  *Parser,
  OUT XML_SCHEMA **Schema
) {
  // Check parameters
  if ((Parser == NULL) || (Schema == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  return XmlDocumentGetSchema(Parser->Document, Schema);
}
// XmlGetTree
/// Get XML document tree root node
/// @param Parser An XML parser
/// @param Tree   On output, the XML document tree root node
/// @return Whether the XML document tree root node was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document tree root node was retrieved successfully
EFI_STATUS
EFIAPI
XmlGetTree (
  IN  XML_PARSER  *Parser,
  OUT XML_TREE   **Tree
) {
  // Check parameters
  if ((Parser == NULL) || (Tree == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  return XmlDocumentGetTree(Parser->Document, Tree);
}

// XmlDocumentGetSchema
/// Get XML document schema
/// @param Document An XML document
/// @param Schema   On output, the XML document schema
/// @return Whether the XML document schema was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Document or Schema is NULL
/// @retval EFI_SUCCESS           If the XML document schema was retrieved successfully
EFI_STATUS
EFIAPI
XmlDocumentGetSchema (
  IN  XML_DOCUMENT  *Document,
  OUT XML_SCHEMA   **Schema
) {
  // Check parameters
  if ((Document == NULL) || (Schema == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Schema = Document->Schema;
  return EFI_SUCCESS;
}
// XmlDocumentSetSchema
/// Set XML document schema
/// @param Document An XML document
/// @param Schema   The XML document schema to set
/// @return Whether the XML document schema was set or not
/// @retval EFI_INVALID_PARAMETER If Document or Schema is NULL
/// @retval EFI_SUCCESS           If the XML document schema was set successfully
EFI_STATUS
EFIAPI
XmlDocumentSetSchema (
  IN OUT XML_DOCUMENT *Document,
  IN     XML_SCHEMA   *Schema
) {
  if ((Document == NULL) || (Schema == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Document->Schema != NULL) {
    XmlSchemaFree(Document->Schema);
  }
  Document->Schema = XmlSchemaDuplicate(Schema);
  return EFI_SUCCESS;
}
// XmlDocumentGetTree
/// Get XML document tree root node
/// @param Document An XML document
/// @param Tree     On output, the XML document tree root node
/// @return Whether the XML document tree root node was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Document or Tree is NULL
/// @retval EFI_SUCCESS           If the XML document tree root node was retrieved successfully
EFI_STATUS
EFIAPI
XmlDocumentGetTree (
  IN  XML_DOCUMENT  *Document,
  OUT XML_TREE     **Tree
) {
  // Check parameters
  if ((Document == NULL) || (Tree == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Tree = Document->Tree;
  return EFI_SUCCESS;
}

// XmlTreeGetTag
/// Get XML document tree node tag name
/// @param Tree An XML document tree
/// @param Tag  On output, the XML document tree tag name
/// @return Whether the XML document tree tag name was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree or Tag is NULL
/// @retval EFI_SUCCESS           If the XML document tree tag name was retrieved successfully
EFI_STATUS
EFIAPI
XmlTreeGetTag (
  IN  XML_TREE  *Tree,
  OUT CHAR16   **Tag
) {
  // Check parameters
  if ((Tree == NULL) || (Tag == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Tag = Tree->Name;
  return EFI_SUCCESS;
}
// XmlTreeSetTag
/// Set XML document tree node tag name
/// @param Tree An XML document tree
/// @param Tag  The XML document tree tag name to set
/// @return Whether the XML document tree tag name was set or not
/// @retval EFI_INVALID_PARAMETER If Tree or Tag is NULL
/// @retval EFI_SUCCESS           If the XML document tree tag name was set successfully
EFI_STATUS
EFIAPI
XmlTreeSetTag (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Tag
) {
  // Check parameters
  if ((Tree == NULL) || (Tag == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Tree->Name != NULL) {
    EfiFreePool(Tree->Name);
  }
  Tree->Name = StrDup(Tag);
  return EFI_SUCCESS;
}
// XmlTreeGetValue
/// Get XML document tree node value
/// @param Tree  An XML document tree
/// @param Value On output, the XML document tree value
/// @return Whether the XML document tree node value was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree or Value is NULL
/// @retval EFI_SUCCESS           If the XML document tree node value was retrieved successfully
EFI_STATUS
EFIAPI
XmlTreeGetValue (
  IN  XML_TREE  *Tree,
  OUT CHAR16   **Value
) {
  // Check parameters
  if ((Tree == NULL) || (Value == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Value = Tree->Value;
  return EFI_SUCCESS;
}
// XmlTreeSetValue
/// Set XML document tree node value
/// @param Tree  An XML document tree
/// @param Value The XML document tree node value to set
/// @return Whether the XML document tree node value was set or not
/// @retval EFI_INVALID_PARAMETER If Tree or Value is NULL
/// @retval EFI_SUCCESS           If the XML document tree node value was set successfully
EFI_STATUS
EFIAPI
XmlTreeSetValue (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Value
) {
  // Check parameters
  if ((Tree == NULL) || (Value == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Tree->Value != NULL) {
    EfiFreePool(Tree->Value);
  }
  Tree->Value = StrDup(Value);
  return EFI_SUCCESS;
}
// XmlTreeHasChildren
/// Check if XML document tree node has child nodes
/// @param Tree An XML document tree node
/// @return Whether the XML document tree node has child nodes or not
/// @retval TRUE  If the tree node has children
/// @retval FALSE If the tree node does not have children
BOOLEAN
EFIAPI
XmlTreeHasChildren (
  IN  XML_TREE *Tree
) {
  return ((Tree != NULL) && (Tree->Children != NULL));
}
// XmlTreeGetChildren
/// Get XML document tree node child nodes
/// @param Tree     An XML document tree
/// @param Children On output, the XML document tree node child nodes, which must be freed
/// @param Count    On output, the count of children
/// @return Whether the XML document tree child nodes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Children, or Count is NULL or *Children is not NULL
/// @retval EFI_NOT_FOUND         If there are no XML document tree child nodes
/// @retval EFI_SUCCESS           If the XML document tree child nodes were retrieved successfully
EFI_STATUS
EFIAPI
XmlTreeGetChildren (
  IN  XML_TREE   *Tree,
  OUT XML_TREE ***Children,
  OUT UINTN      *Count
) {
  XML_TREE **ChildList;
  XML_TREE  *List;
  UINTN      ChildCount;
  UINTN      ListCount;
  // Check parameters
  if ((Tree == NULL) || (Children == NULL) || (*Children != NULL) || (Count == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Count the child nodes
  ListCount = 0;
  for (List = Tree->Children; List != NULL; List = List->Next) {
    ++ListCount;
  }
  // If there are no children, return not found
  if (ListCount == 0) {
    return EFI_NOT_FOUND;
  }
  // Allocate new list
  ChildList = EfiAllocateArray(XML_TREE *, ListCount);
  if (ChildList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Copy child nodes list
  ChildCount = 0;
  List = Tree->Children;
  while ((List != NULL) && (ChildCount < ListCount)) {
    ChildList[ChildCount++] = List;
    List = List->Next;
  }
  // Return the child nodes list
  *Children = ChildList;
  *Count = ChildCount;
  return EFI_SUCCESS;
}
// XmlTreeGetAttributes
/// Get XML document tree node attributes
/// @param Tree       An XML document tree
/// @param Attributes On output, the XML document tree node attributes, which must be freed
/// @param Count      On output, the count of attributes
/// @return Whether the XML document tree attributes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Attributes, or Count is NULL or *Attributes is not NULL
/// @retval EFI_NOT_FOUND         If there are no XML document tree attibutes
/// @retval EFI_SUCCESS           If the XML document tree attributes were retrieved successfully
EFI_STATUS
EFIAPI
XmlTreeGetAttributes (
  IN  XML_TREE        *Tree,
  OUT XML_ATTRIBUTE ***Attributes,
  OUT UINTN           *Count
) {
  XML_ATTRIBUTE **AttributeList;
  XML_LIST       *List;
  UINTN           AttributeCount;
  UINTN           ListCount;
  // Check parameters
  if ((Tree == NULL) || (Attributes == NULL) || (*Attributes != NULL) || (Count == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Count the attributes
  ListCount = 0;
  for (List = Tree->Attributes; List != NULL; List = List->Next) {
    ++ListCount;
  }
  // If there are no attributes, return not found
  if (ListCount == 0) {
    return EFI_NOT_FOUND;
  }
  // Allocate new list
  AttributeList = EfiAllocateArray(XML_ATTRIBUTE *, ListCount);
  if (AttributeList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Copy attributes list
  AttributeCount = 0;
  List = Tree->Attributes;
  while ((List != NULL) && (AttributeCount < ListCount)) {
    AttributeList[AttributeCount++] = &(List->Attribute);
    List = List->Next;
  }
  // Return the attributes list
  *Attributes = AttributeList;
  *Count = AttributeCount;
  return EFI_SUCCESS;
}
// XmlTreeGetAttribute
/// Get an XML document tree node attribute
/// @param Tree      An XML document tree
/// @param Name      The XML document tree node attribute name
/// @param Attribute On output, the XML document tree attribute
/// @return Whether the XML document tree attribute was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Tree, Name, or Attribute is NULL
/// @retval EFI_NOT_FOUND         If the XML document tree attibute was not found
/// @retval EFI_SUCCESS           If the XML document tree attribute was retrieved successfully
EFI_STATUS
EFIAPI
XmlTreeGetAttribute (
  IN  XML_TREE       *Tree,
  IN  CHAR16         *Name,
  OUT XML_ATTRIBUTE **Attribute
) {
  XML_LIST *List;
  // Check parameters
  if ((Tree == NULL) || (Name == NULL) || (Attribute == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Search for attribute
  for (List = Tree->Attributes; List != NULL; List = List->Next) {
    if ((List->Attribute.Name != NULL) && (StrCmp(Name, List->Attribute.Name) == 0)) {
      *Attribute = &(List->Attribute);
      return EFI_SUCCESS;
    }
  }
  // Not found
  return EFI_NOT_FOUND;
}
// XmlTreeSetAttribute
/// Set an XML document tree node attribute
/// @param Tree      An XML document tree
/// @param Name      The XML document tree node attribute name
/// @param Attribute The XML document tree attribute to set
/// @return Whether the XML document tree attribute was set or not
/// @retval EFI_INVALID_PARAMETER If Tree, Name, Attribute, or Attribute->Name is NULL
/// @retval EFI_SUCCESS           If the XML document tree attribute was set successfully
EFI_STATUS
EFIAPI
XmlTreeSetAttribute (
  IN OUT XML_TREE      *Tree,
  IN     CHAR16        *Name,
  IN     XML_ATTRIBUTE *Attribute
) {
  XML_LIST *List = NULL;
  // Check parameters
  if ((Tree == NULL) || (Name == NULL) || (Attribute == NULL) || (Attribute->Name == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Search for attribute
  for (List = Tree->Attributes; List != NULL; List = List->Next) {
    if ((List->Attribute.Name != NULL) && (StrCmp(Name, List->Attribute.Name) == 0)) {
      XmlAttributeDuplicateMembers(&(List->Attribute), Attribute);
      return EFI_SUCCESS;
    }
  }
  // Allocate a new attribute
  if (List == NULL) {
    if (Tree->Attributes == NULL) {
      List = Tree->Attributes = EfiAllocateByType(XML_LIST);
    } else {
      for (List = Tree->Attributes; List->Next != NULL; List = List->Next);
      List = List->Next = EfiAllocateByType(XML_LIST);
    }
  }
  // Check the attribute was allocate
  if (List == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Return the new attribute
  List->Next = NULL;
  XmlAttributeDuplicateMembers(&(List->Attribute), Attribute);
  return EFI_SUCCESS;
}
// XmlTreeRemoveAttribute
/// Remove an attribute from XML document tree node
/// @param Tree An XML document tree
/// @param Name The name of the XML document tree node attribute to remove
/// @return Whether the XML document tree attribute was removed or not
/// @retval EFI_INVALID_PARAMETER If Tree or Name is NULL
/// @retval EFI_SUCCESS           If the XML document tree attribute was removed successfully
EFI_STATUS
EFIAPI
XmlTreeRemoveAttribute (
  IN OUT XML_TREE *Tree,
  IN     CHAR16   *Name
) {
  XML_LIST *Attribute;
  // Check parameters
  if ((Tree == NULL) || (Name == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Tree->Attributes != NULL) {
    // Check if first attribute
    if (StrCmp(Name, Tree->Attributes->Attribute.Name) == 0) {
      Attribute = Tree->Attributes;
      Tree->Attributes = Attribute->Next;
      XmlAttributeListFree(Attribute);
      return EFI_SUCCESS;
    } else {
      // Check rest of attributes
      for (Attribute = Tree->Attributes; Attribute->Next != NULL; Attribute = Attribute->Next) {
        if ((Attribute->Next->Attribute.Name!= NULL) && (StrCmp(Name, Attribute->Next->Attribute.Name) == 0)) {
          return EFI_SUCCESS;
        }
      }
    }
  }
  // Not found
  return EFI_NOT_FOUND;
}

// XmlIsValidName
/// Check if token is valid tag or attribute name
/// @param Token On input, the token to check is valid name, on output, the position in the token that invalidates the name or NULL if the token is a valid name
/// @retval EFI_INVALID_PARAMETER If Token is NULL or *Token is NULL
/// @retval EFI_NO_MAPPING        If the token is not a valid name, *Token is updated with the position of the invalid character
/// @retval EFI_SUCCESS           If the token is a valid name, *Token is NULL
EFI_STATUS
EFIAPI
XmlIsValidName (
  IN OUT CHAR16 **Token
) {
  CHAR16 *Name;
  CHAR16  Char;
  // Check parameters
  if ((Token == NULL) || (*Token == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check the token is not empty and begins with a letter or underscore
  Name = *Token;
  if (((Char = *Name) == L'\0') ||
      (((Char < L'a') || (Char > 'z')) &&
       ((Char < L'A') || (Char > 'Z')) &&
       (Char != L'_'))) {
    // Invalid name starting character
    return EFI_NO_MAPPING;
  }
  // Check if token begins with any casing of XML
  if (StrniCmp(Name, L"xml", 3) == 0) {
    // Invalid name starting with XML
    return EFI_NO_MAPPING;
  }
  // Check token contains only letters, digits, hyphens, underscores, or periods
  while ((Char = *(++Name)) != L'\0') {
    if (((Char < L'a') || (Char > L'z')) &&
        ((Char < L'A') || (Char > L'Z')) &&
        ((Char < L'0') || (Char > L'9')) &&
        (Char != L'-') && (Char != L'_') && (Char != L'.')) {
      // Invalid name character
      *Token = Name;
      return EFI_NO_MAPPING;
    }
  }
  // Valid name
  *Token = NULL;
  return EFI_SUCCESS;
}

// XmlLibInitialize
/// XML library initialize use
/// @return Whether the XML library initialized successfully or not
/// @retval EFI_SUCCESS The XML library successfully initialized
EFI_STATUS
EFIAPI
XmlLibInitialize (
  VOID
) {
  return EFI_SUCCESS;
}

// XmlLibFinish
/// XML library finish use
/// @return Whether the XML library finished successfully or not
/// @retval EFI_SUCCESS The XML library successfully finished
EFI_STATUS
EFIAPI
XmlLibFinish (
  VOID
) {
  return EFI_SUCCESS;
}
