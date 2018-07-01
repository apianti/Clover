///
/// @file Library/Serialize/Svg.c
///
/// SVG serialization implementation
///

#include "SvgElements.h"

#include "Serialize/Type/svg-1.1.dtd.h"

// SVG
/// SVG image
struct SVG_IMAGE {

  // Reserved
  /// Reserved
  UINTN Reserved;

};
// SVG_PARSER
/// SVG parser
struct SVG_PARSER {

  // Parser
  /// The XML parser
  XML_PARSER *Parser;
  // Image
  /// The parsed SVG image
  SVG_IMAGE  *Image;

};

// mSvgElement
/// The SVG element
STATIC CONST SVG_ELEMENTS mSvgElement[] = {
  { ARRAY_COUNT(mSvgStructuralElements),  mSvgStructuralElements },
  { ARRAY_COUNT(mSvgAnimationElements),   mSvgAnimationElements },
  { ARRAY_COUNT(mSvgDescriptiveElements), mSvgDescriptiveElements },
  { ARRAY_COUNT(mSvgGradientElements),    mSvgGradientElements },
  { ARRAY_COUNT(mSvgShapeElements),       mSvgShapeElements },
  { ARRAY_COUNT(mSvgTextElements),        mSvgTextElements },
  { ARRAY_COUNT(mSvgElements),            mSvgElements },
};
// mSvgElementAttributes
/// The SVG element attributes
STATIC CONST SVG_ATTRIBUTES mSvgElementAttributes[] = {
  { ARRAY_COUNT(mSvgConditionalAttributes),   mSvgConditionalAttributes },
  { ARRAY_COUNT(mSvgCoreAttributes),          mSvgCoreAttributes },
  { ARRAY_COUNT(mSvgDocumentEventAttributes), mSvgDocumentEventAttributes },
  { ARRAY_COUNT(mSvgGraphicEventAttributes),  mSvgGraphicEventAttributes },
  { ARRAY_COUNT(mSvgPresentationAttributes),  mSvgPresentationAttributes },
  { ARRAY_COUNT(mSvgAttributes),              mSvgAttributes },
};
// mSvgAElement
/// The SVG a element
STATIC CONST SVG_ELEMENTS mSvgAElement[] = {
  { ARRAY_COUNT(mSvgStructuralElements),  mSvgStructuralElements },
  { ARRAY_COUNT(mSvgAnimationElements),   mSvgAnimationElements },
  { ARRAY_COUNT(mSvgDescriptiveElements), mSvgDescriptiveElements },
  { ARRAY_COUNT(mSvgGradientElements),    mSvgGradientElements },
  { ARRAY_COUNT(mSvgShapeElements),       mSvgShapeElements },
  { ARRAY_COUNT(mSvgTextElements),        mSvgTextElements },
  { ARRAY_COUNT(mSvgElements),            mSvgElements },
};
// mSvgAElementAttributes
/// The SVG a element attributes
STATIC CONST SVG_ATTRIBUTES mSvgAElementAttributes[] = {
  { ARRAY_COUNT(mSvgConditionalAttributes),   mSvgConditionalAttributes },
  { ARRAY_COUNT(mSvgCoreAttributes),          mSvgCoreAttributes },
  { ARRAY_COUNT(mSvgDocumentEventAttributes), mSvgDocumentEventAttributes },
  { ARRAY_COUNT(mSvgGraphicEventAttributes),  mSvgGraphicEventAttributes },
  { ARRAY_COUNT(mSvgPresentationAttributes),  mSvgPresentationAttributes },
  { ARRAY_COUNT(mSvgAAttributes),             mSvgAAttributes },
};

// SvgElementCompare
/// Compare an element name to a set of callbacks
/// @param Element      The element name to check
/// @param ElementCount The count of elements to check
/// @param Elements     The elements to check
/// @return The element callback or NULL
STATIC
SVG_ELEMENT_CALLBACK
EFIAPI
SvgElementCompare (
  IN CONST CHAR16      *Element,
  IN UINTN              ElementCount,
  IN CONST SVG_ELEMENT *Elements
) {
  UINTN Index;
  // Check parameters
  if ((Element == NULL) || (Elements == NULL) || (ElementCount == 0)) {
    return NULL;
  }
  // Check the core attributes
  for (Index = 0; Index < ElementCount; ++Index) {
    INTN Comparison = StriCmp(Element, Elements[Index].Name);
    if (Comparison == 0) {
      // Return this attribute callback
      return Elements[Index].Callback;
    }
    // The attribute lists should be sorted
    if (Comparison < 0) {
      break;
    }
  }
  return NULL;
}
// SvgElementsCompare
/// Compare an element name to a set of elements
/// @param Parser       The SVG parser
/// @param Element      The element name to check
/// @param ElementCount The count of elements to check
/// @param Elements     The elements to check
/// @return Whether the element exists in the collection or not
STATIC
BOOLEAN
EFIAPI
SvgElementsCompare (
  IN SVG_PARSER          *Parser,
  IN XML_TREE            *Element,
  IN UINTN                ElementCount,
  IN CONST SVG_ELEMENTS  *Elements
) {
  UINTN                  Index;
  UINTN                  Index2;
  UINTN                  ChildCount;
  XML_TREE             **Children;
  SVG_ELEMENT_CALLBACK   Callback;
  // Check parameters
  if ((Parser == NULL) || (Elements == NULL) ||
      (Elements == NULL) || (ElementCount == 0)) {
    return NULL;
  }
  // Get the children
  ChildCount = 0;
  Children = NULL;
  if (EFI_ERROR(XmlTreeGetChildren(Element, &Children, &ChildCount))) {
    return FALSE;
  }
  if (Children == NULL) {
    if (ChildCount = 0) {
      return TRUE;
    }
    return FALSE;
  }
  // Check the children
  for (Index = 0; Index < ChildCount; ++Index) {
    // Get the element name
    if (Children[Index] != NULL) {
      CHAR16 *Child = NULL;
      if (EFI_ERROR(XmlTreeGetTag(Children[Index], &Child)) || (Child == NULL)) {
        return FALSE;
      }
      for (Index2 = 0; Index2 < ElementCount; ++Index2) {
        // Check the child element
        Callback = SvgElementCompare(Child, Elements[Index2].Count, Elements[Index2].Elements);
        if (Callback != NULL) {
          // Call element callback
          if (!Callback(Parser, Children[Index])) {
            return FALSE;
          }
          break;
        }
      }
      if (Index2 >= ElementCount) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
// SvgAttributeCompare
/// Compare an attribute name to a set of callbacks
/// @param Attribute      The attribute name to check
/// @param AttributeCount The count of attributes to check
/// @param Attributes     The attributes to check
/// @return The attribute callback or NULL
STATIC
SVG_ATTRIBUTE_CALLBACK
EFIAPI
SvgAttributeCompare (
  IN CONST CHAR16        *Attribute,
  IN UINTN                AttributeCount,
  IN CONST SVG_ATTRIBUTE *Attributes
) {
  UINTN Index;
  // Check parameters
  if ((Attribute == NULL) || (Attributes == NULL) || (AttributeCount == 0)) {
    return NULL;
  }
  // Check the core attributes
  for (Index = 0; Index < AttributeCount; ++Index) {
    INTN Comparison = StriCmp(Attribute, Attributes[Index].Name);
    if (Comparison == 0) {
      // Return this attribute callback
      return Attributes[Index].Callback;
    }
    // The attribute lists should be sorted
    if (Comparison < 0) {
      break;
    }
  }
  return NULL;
}
// SvgAttributesCompare
/// Compare an element's attributes to a set of attributes
/// @param Parser         The SVG parser
/// @param Element        The element whose attributes to check
/// @param AttributeCount The count of attributes to check
/// @param Attributes     The attributes to check
/// @return Whether the attribute exists in the collection or not
STATIC
BOOLEAN
EFIAPI
SvgAttributesCompare (
  IN SVG_PARSER            *Parser,
  IN XML_TREE              *Element,
  IN UINTN                  AttributeCount,
  IN CONST SVG_ATTRIBUTES  *Attributes
) {
  UINTN                    Index;
  UINTN                    Index2;
  UINTN                    SvgAttributeCount;
  XML_ATTRIBUTE          **SvgAttributes;
  SVG_ATTRIBUTE_CALLBACK   Callback;
  // Check parameters
  if ((Parser == NULL) || (Element == NULL) ||
      (Attributes == NULL) || (AttributeCount == 0)) {
    return FALSE;
  }
  // Get the attributes
  SvgAttributes = NULL;
  SvgAttributeCount = 0;
  if (EFI_ERROR(XmlTreeGetAttributes(Element, &SvgAttributes, &SvgAttributeCount))) {
    return FALSE;
  }
  if (SvgAttributes == NULL) {
    if (SvgAttributeCount = 0) {
      return TRUE;
    }
    return FALSE;
  }
  // Check the attributes
  for (Index = 0; Index < SvgAttributeCount; ++Index) {
    for (Index2 = 0; Index2 < AttributeCount; ++Index2) {
      Callback = SvgAttributeCompare(SvgAttributes[Index]->Name, Attributes[Index2].Count, Attributes[Index2].Attributes);
      if (Callback != NULL) {
        // Call attribute callback
        if (!Callback(Parser, SvgAttributes[Index])) {
          return FALSE;
        }
        break;
      }
    }
    if (Index2 >= AttributeCount) {
      return FALSE;
    }
  }
  return TRUE;
}
// SvgContainerElementCallback
/// Modify the SVG image using an SVG element
/// @param Parser  The SVG parser
/// @param Element The SVG element name
/// @return Whether the SVG image element is valid or not
//STATIC
//BOOLEAN
//EFIAPI
//SvgContainerElementCallback (
//  IN SVG_PARSER *Parser,
//  IN XML_TREE   *Element
//) {
//  // Check parameters
//  if ((Parser == NULL) || (Element == NULL)) {
//    return FALSE;
//  }
//  // Check the attributes
//  if (!SvgAttributesCompare(Parser, Element, ARRAY_COUNT(mSvgContainerElementAttributes), mSvgContainerElementAttributes)) {
//    return FALSE;
//  }
//  // Check the child elements
//  if (!SvgElementsCompare(Parser, Element, ARRAY_COUNT(mSvgContainerElement), mSvgContainerElement)) {
//    return FALSE;
//  }
//  return TRUE;
//}
// SvgElementCallback
/// Modify the SVG image using an SVG element
/// @param Parser  The SVG parser
/// @param Element The SVG element name
/// @return Whether the SVG image element is valid or not
STATIC
BOOLEAN
EFIAPI
SvgElementCallback (
  IN SVG_PARSER *Parser,
  IN XML_TREE   *Element
) {
  // Check parameters
  if ((Parser == NULL) || (Element == NULL)) {
    return FALSE;
  }
  // TODO: Create SVG element

  // Check the attributes
  if (!SvgAttributesCompare(Parser, Element, ARRAY_COUNT(mSvgElementAttributes), mSvgElementAttributes)) {
    return FALSE;
  }
  // Check the child elements
  if (!SvgElementsCompare(Parser, Element, ARRAY_COUNT(mSvgElement), mSvgElement)) {
    return FALSE;
  }
  return TRUE;
}
// SvgXmlInspector
/// Inspect XML document tree to build a SVG image
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
STATIC
BOOLEAN
EFIAPI
SvgXmlInspector (
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
  SVG_PARSER *Parser;
  UNUSED_PARAMETER(Value);
  UNUSED_PARAMETER(AttributeCount);
  UNUSED_PARAMETER(Attributes);
  UNUSED_PARAMETER(ChildCount);
  UNUSED_PARAMETER(Children);
  // Get the parser
  Parser = (SVG_PARSER *)Context;
  if (Parser == NULL) {
    return FALSE;
  }
  // Check this is an SVG element
  if (StriCmp(TagName, L"svg") != 0) {
    return FALSE;
  }
  // Only one tag can exist at the root
  if ((Level == 0) && (LevelIndex != 0)) {
    return FALSE;
  }
  // Parse the SVG element
  return SvgElementCallback(Parser, Tree);
}
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
EFI_STATUS
EFIAPI
SvgParse (
  IN  SVG_PARSER            *Parser,
  IN  EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN  VOID                  *Buffer,
  IN  UINTN                  Size
) {
  // Check parameters
  if ((Parser == NULL) || (Parser->Parser == NULL) || (Buffer == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Parse the XML from the buffer
  return XmlParse(Parser->Parser, Encoding, Buffer, Size);
}
// SvgParseFinish
/// Finish a SVG image
/// @param Parser     The SVG parser
/// @param Image On output, the parsed SVG image
/// @retval EFI_INVALID_PARAMETER If Parser or Image is NULL or *Image is not NULL
/// @retval EFI_SUCCESS           The parsed SVG image was returned successfully
EFI_STATUS
EFIAPI
SvgParseFinish (
  IN OUT SVG_PARSER  *Parser,
  IN     SVG_IMAGE  **Image
) {
  EFI_STATUS Status;
  // Check parameters
  if ((Parser == NULL) || (Image == NULL) || (*Image != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Finish parsing the XML
  Status = XmlParseFinish(XmlParserFromSvgParser(Parser));
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Create the SVG image from the XML
  Status = XmlInspect(Parser->Parser, SvgXmlInspector, Parser, FALSE);
  // Return the image
  if (!EFI_ERROR(Status)) {
    *Image = Parser->Image;
    Parser->Image = NULL;
  }
  return Status;
}
// SvgParseXml
/// Parse an SVG image from an XML tree
/// @param Tree  The XML tree to parse as an SVG image
/// @param Image On output, the parsed SVG image
/// @retval EFI_INVALID_PARAMETER If Parser or Image is NULL
/// @retval EFI_SUCCESS           The parsed SVG image was returned successfully
EFI_STATUS
EFIAPI
SvgParseXml (
  IN OUT XML_TREE   *Tree,
  IN     SVG_IMAGE **Image
) {
  EFI_STATUS  Status;
  SVG_PARSER *Parser;
  // Check parameters
  if ((Tree == NULL) || (Image == NULL) || (*Image != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Create a temporary parser
  Parser = NULL;
  Status = SvgCreate(&Parser, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Parser == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Create the SVG image from the XML
  Status = XmlTreeInspect(Tree, 0, 0, SvgXmlInspector, Parser, FALSE);
  // Return the image
  if (!EFI_ERROR(Status)) {
    *Image = Parser->Image;
    Parser->Image = NULL;
  }
  // Free the parser
  SvgFree(Parser);
  return Status;
}

// SvgToXml
/// Convert a SVG image to an XML string
/// @param Image The SVG image
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The SVG image was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Image or Size is NULL
EFI_STATUS
EFIAPI
SvgToXml (
  IN     SVG_IMAGE             *Image,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size
) {
  STATIC CONST CHAR8 *HeaderFormat = "<?xml version=\"1.0\" encoding=\"%a\" ?>\n"
                                     "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *FormatEncoding;
  UINTN                  BufferSize = 0;
  UINTN                  RemainingSize;
  UINTN                  ThisSize;
  // Check parameters
  if ((Image == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Encoding == NULL) {
    Encoding = EfiUtf16Encoding();
    if (Encoding == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Get the format encoding
  FormatEncoding = EfiLatin1Encoding();
  // Create the XML document tag
  RemainingSize = *Size;
  ThisSize = RemainingSize;
  Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, HeaderFormat, AsciiStrSize(HeaderFormat), Encoding->Name);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  // Increase the buffer size
  if (ThisSize < Encoding->UnitSize) {
    return EFI_INVALID_PARAMETER;
  }
  ThisSize -= Encoding->UnitSize;
  if (ThisSize < RemainingSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  BufferSize += ThisSize;
  // Advance the buffer
  if (Buffer != NULL) {
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
  }
  // Convert the SVG images to XML
  ThisSize = RemainingSize;
  Status = EFI_UNSUPPORTED; // TODO: SvgToXml(Image, Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  if (ThisSize < RemainingSize) {
    RemainingSize -= ThisSize;
  } else {
    RemainingSize = 0;
  }
  BufferSize += ThisSize;
  // Return the size and status
  *Size = BufferSize;
  return Status;
}
// SvgToXmlBuffer
/// Convert a SVG image to an UTF-16 XML string
/// @param Image The SVG image
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The SVG image was converted to XML
/// @retval EFI_INVALID_PARAMETER If Image or Buffer is NULL
EFI_STATUS
EFIAPI
SvgToXmlBuffer (
  IN  SVG_IMAGE              *Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  OUT VOID                  **Buffer,
  OUT UINTN                  *Size OPTIONAL
) {
  EFI_STATUS  Status;
  UINTN       XmlSize = 0;
  VOID       *Xml = NULL;
  // Check parameters
  if ((Image == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Encoding == NULL) {
    Encoding = EfiUtf16Encoding();
    if (Encoding == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Get the size needed for the XML string buffer
  Status = SvgToXml(Image, Encoding, NULL, &XmlSize);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  if (XmlSize == 0) {
    return EFI_NOT_FOUND;
  }
  // Allocate the XML string buffer
  Xml = EfiAllocate(XmlSize);
  if (Xml == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Convert the SVG image to XML string
  Status = SvgToXml(Image, Encoding, Xml, &XmlSize);
  if (EFI_ERROR(Status)) {
    EfiFreePool(Xml);
  } else {
    *Buffer = Xml;
    if (Size != NULL) {
      *Size = XmlSize;
    }
  }
  return Status;
}

// SvgCreate
/// Create a SVG image parser
/// @param Parser On output, the created SVG parser
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or *Parser is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the SVG parser
/// @retval EFI_SUCCESS           If the SVG parser was created successfully 
EFI_STATUS
EFIAPI
SvgCreate (
  OUT SVG_PARSER   **Parser,
  IN  CONST CHAR16  *Source OPTIONAL
) {
  EFI_STATUS  Status;
  SVG_PARSER *Svg;
  // Check parameters
  if ((Parser == NULL) || (*Parser != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate the SVG parser
  Svg = EfiAllocateByType(SVG_PARSER);
  if (Svg == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Create the XML parser
  Status = XmlCreate(&(Svg->Parser), L"svg-1.1.dtd");
  if (!EFI_ERROR(Status)) {
    // Parse the SVG DTD schema
    Status = XmlParse(Svg->Parser, NULL, svg_1_1_dtd, svg_1_1_dtd_len);
    if (!EFI_ERROR(Status)) {
      // Set the correct source
      Status = XmlSetParseSource(Svg->Parser, Source);
    }
  }
  // Check if there was an error
  if (EFI_ERROR(Status)) {
    SvgFree(Svg);
  } else {
    // Return the create language parser
    *Parser = Svg;
  }
  return Status;
}
// SvgReset
/// TODO: Reset a SVG parser for reuse
/// @param Parser The SVG parser to reset
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the SVG parser was reset successfully
EFI_STATUS
EFIAPI
SvgReset (
  IN OUT SVG_PARSER   *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free image
  if (Parser->Image != NULL) {
    SvgImageFree(Parser->Image);
    Parser->Image = NULL;
  }
  // Reset the XML parser
  return XmlReset(Parser->Parser, Source);
}
// SvgFree
/// Free a SVG parser
/// @param Parser The SVG parser to free
/// @retval EFI_SUCCESS           The SVG parser was freed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
EFI_STATUS
EFIAPI
SvgFree (
  IN SVG_PARSER *Parser
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Parser->Image != NULL) {
    SvgImageFree(Parser->Image);
    Parser->Image = NULL;
  }
  EfiFreePool(Parser);
  return EFI_SUCCESS;
}
// SvgImageFree
/// TODO: Free a SVG image
/// @param Image The SVG image to free
/// @retval EFI_SUCCESS           The SVG image was freed
/// @retval EFI_INVALID_PARAMETER If Image is NULL
EXTERN
EFI_STATUS
EFIAPI
SvgImageFree (
  IN SVG_IMAGE *Image
) {
  // Check parameters
  if (Image == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free image
  return EfiFreePool((VOID *)Image);
}

// XmlParserFromSvgParser
/// Get the XML parser from a SVG parser
/// @param Parser The SVG parser of which to get the XML parser
/// @return The XML parser associated with the specified SVG parser or NULL if there was an error
XML_PARSER *
EFIAPI
XmlParserFromSvgParser (
  IN SVG_PARSER *Parser
) {
  if (Parser == NULL) {
    return NULL;
  }
  return Parser->Parser;
}
// ParserFromSvgParser
/// Get the language parser from a SVG parser
/// @param Parser The SVG parser of which to get the language parser
/// @return The language parser associated with the specified SVG parser or NULL if there was an error
LANG_PARSER *
EFIAPI
ParserFromSvgParser (
  IN SVG_PARSER *Parser
) {
  if (Parser == NULL) {
    return NULL;
  }
  return ParserFromXmlParser(Parser->Parser);
}

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
EFI_STATUS
EFIAPI
SvgLoad (
  OUT SVG_IMAGE             **Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  EFI_FILE_PROTOCOL      *Root,
  IN  CONST CHAR16           *Path OPTIONAL
) {
  EFI_STATUS         Status;
  SVG_PARSER      *Parser = NULL;
  VOID              *Buffer = NULL;
  UINT64             Size = 0;
  // Check parameters
  if ((Image == NULL) || (*Image != NULL) || (Root == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Load the SVG image from file
  Status = EfiFileLoad(Root, Path, &Size, &Buffer);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (Size == 0) {
    EfiFreePool(Buffer);
    return EFI_OUT_OF_RESOURCES;
  }
  // Create the SVG parser
  Status = SvgCreate(&Parser, Path);
  if (!EFI_ERROR(Status)) {
    // Parse the XML buffer for a SVG image
    Status = SvgParse(Parser, Encoding, Buffer, (UINTN)Size);
    if (!EFI_ERROR(Status)) {
      // Finish parsing and create the SVG image
      Status = SvgParseFinish(Parser, Image);
    }
  }
  // Free the SVG parser
  if (Parser != NULL) {
    SvgFree(Parser);
  }
  // Free the buffer
  EfiFreePool(Buffer);
  return Status;
}
// SvgLoadPath
/// Load a SVG image from file
/// @param Image On output, the SVG image
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the SVG file to load
/// @retval EFI_SUCCESS           The SVG image was loaded
/// @retval EFI_ABORTED           The SVG file is invalid
/// @retval EFI_NOT_FOUND         The SVG file was not found
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EFI_STATUS
EFIAPI
SvgLoadPath (
  OUT SVG_IMAGE             **Image,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST CHAR16           *DevicePath
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath;
  // Check parameters;
  if ((Image == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_NOT_FOUND;
  }
  // Load device path and import configuration
  Status = SvgLoadDevicePath(Image, Encoding, ActualDevicePath);
  // Free device path and return result
  EfiFreePool(ActualDevicePath);
  return Status;
}
// SvgLoadDevicePath
/// Load a SVG image from file
/// @param Image On output, the SVG image
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the SVG file to load
/// @retval EFI_SUCCESS           The SVG image was loaded
/// @retval EFI_ABORTED           The SVG file is invalid
/// @retval EFI_NOT_FOUND         The SVG file was not found
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EFI_STATUS
EFIAPI
SvgLoadDevicePath (
  OUT SVG_IMAGE          **Image,
  IN  EFI_ENCODING_PROTOCOL     *Encoding OPTIONAL,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
) {
  EFI_FILE_PROTOCOL *Root = NULL;
  EFI_HANDLE         Handle = NULL;
  CHAR16            *FileName = NULL;
  // Get the device handle and file path
  EFI_STATUS Status = EfiFileLocateDevicePath(&DevicePath, &Handle, &FileName);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check device handle and file path are valid
  if (FileName == NULL) {
    Status = (Handle == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
  } else if (Handle == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (!EFI_ERROR(Status)) {
    // Open the root directory
    Status = EfiFileOpenRootByHandle(Handle, &Root);
    if (!EFI_ERROR(Status)) {
      if (Root == NULL) {
        Status = EFI_NOT_FOUND;
      } else {
        // Load the SVG image file
        Status = SvgLoad(Image, Encoding, Root, FileName);
      }
    }
  }
  // Cleanup root and file path
  if (Root != NULL) {
    EfiFileClose(Root);
  }
  if (FileName != NULL) {
    EfiFreePool(FileName);
  }
  return Status;
}

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
EFI_STATUS
EFIAPI
SvgSave (
  IN SVG_IMAGE             *Image,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
) {
  EFI_STATUS         Status;
  VOID              *Buffer = NULL;
  UINTN              Size = 0;
  // Check parameters
  if ((Image == NULL) || (Root == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the SVG image to XML buffer
  Status = SvgToXmlBuffer(Image, Encoding, &Buffer, &Size);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (Size == 0) {
    EfiFreePool(Buffer);
    return EFI_OUT_OF_RESOURCES;
  }
  // Save the SVG image to file
  Status = EfiFileSave(Root, Path, Size, Buffer);
  // Free the buffer and return the result
  EfiFreePool(Buffer);
  return Status;
}
// SvgSavePath
/// Save a SVG image to file
/// @param Image The SVG image to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string of the SVG file
/// @retval EFI_SUCCESS           The SVG image was saved
/// @retval EFI_ABORTED           The SVG image is invalid
/// @retval EFI_ACCESS_DENIED     The SVG file could not be saved
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EFI_STATUS
EFIAPI
SvgSavePath (
  IN SVG_IMAGE             *Image,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath;
  // Check parameters;
  if ((Image == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_NOT_FOUND;
  }
  // Load device path and import configuration
  Status = SvgSaveDevicePath(Image, Encoding, ActualDevicePath);
  // Free device path and return result
  EfiFreePool(ActualDevicePath);
  return Status;
}
// SvgSaveDevicePath
/// Save a SVG image to file
/// @param Image The SVG image to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path of the SVG file
/// @retval EFI_SUCCESS           The SVG image was saved
/// @retval EFI_ABORTED           The SVG image is invalid
/// @retval EFI_ACCESS_DENIED     The SVG file could not be saved
/// @retval EFI_INVALID_PARAMETER If Image or DevicePath is NULL
EFI_STATUS
EFIAPI
SvgSaveDevicePath (
  IN SVG_IMAGE                *Image,
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  EFI_FILE_PROTOCOL *Root = NULL;
  EFI_HANDLE         Handle = NULL;
  CHAR16            *FileName = NULL;
  // Get the device handle and file path
  EFI_STATUS Status = EfiFileLocateDevicePath(&DevicePath, &Handle, &FileName);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check device handle and file path are valid
  if (FileName == NULL) {
    Status = (Handle == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
  } else if (Handle == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (!EFI_ERROR(Status)) {
    // Open the root directory
    Status = EfiFileOpenRootByHandle(Handle, &Root);
    if (!EFI_ERROR(Status)) {
      if (Root == NULL) {
        Status = EFI_NOT_FOUND;
      } else {
        // Save the file
        Status = SvgSave(Image, Encoding, Root, FileName);
      }
    }
  }
  // Cleanup root and file path
  if (Root != NULL) {
    EfiFileClose(Root);
  }
  if (FileName != NULL) {
    EfiFreePool(FileName);
  }
  return Status;
}
