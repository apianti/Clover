///
/// @file Library/Serialize/Plist.c
///
/// PLIST serialization implementation
///

#include <Serialize/Plist.h>
#include <Serialize/Base64.h>

#include "Serialize/Type/plist.dtd.h"

// PLIST_LIST
/// PLIST key list
typedef struct PLIST_LIST PLIST_LIST;
struct PLIST_LIST {

  // Next
  /// The next PLIST key on the stack or NULL if this is the bottom of the stack
  PLIST_LIST *Next;
  // Key
  /// The PLIST key
  PLIST_KEY  *Key;

};
// PLIST_PARSER
/// PLIST parser
struct PLIST_PARSER {

  // Parser
  /// The XML parser
  XML_PARSER *Parser;
  // Key
  /// The current key name
  CHAR16     *Key;
  // Dictionary
  /// The dictionary
  PLIST_KEY  *Dictionary;
  // Stack
  /// The key stack
  PLIST_LIST *Stack;

};

// PlistCreate
/// Create a PLIST dictionary parser
/// @param Parser On output, the created PLIST parser
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or *Parser is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the PLIST parser
/// @retval EFI_SUCCESS           If the PLIST parser was created successfully 
EFI_STATUS
EFIAPI
PlistCreate (
  OUT PLIST_PARSER **Parser,
  IN  CONST CHAR16  *Source OPTIONAL
) {
  EFI_STATUS    Status;
  PLIST_PARSER *Plist;
  // Check parameters
  if ((Parser == NULL) || (*Parser != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate the PLIST parser
  Plist = EfiAllocateByType(PLIST_PARSER);
  if (Plist == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Create the XML parser
  Status = XmlCreate(&(Plist->Parser), L"plist.dtd");
  if (!EFI_ERROR(Status)) {
    // Parse the PLIST DTD schema
    Status = XmlParse(Plist->Parser, NULL, plist_dtd, plist_dtd_len);
    if (!EFI_ERROR(Status)) {
      // Set the correct source
      Status = XmlSetParseSource(Plist->Parser, Source);
    }
  }
  // Check if there was an error
  if (EFI_ERROR(Status)) {
    PlistFree(Plist);
  } else {
    // Return the create language parser
    *Parser = Plist;
  }
  return Status;
}
// PlistReset
/// Reset a PLIST parser for reuse
/// @param Parser The PLIST parser to reset
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the PLIST parser was reset successfully
EFI_STATUS
EFIAPI
PlistReset (
  IN OUT PLIST_PARSER *Parser,
  IN     CONST CHAR16 *Source OPTIONAL
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free the dictionary
  if (Parser->Dictionary != NULL) {
    PlistDictionaryFree(Parser->Dictionary);
    Parser->Dictionary = NULL;
  }
  // Free the key
  if (Parser->Key != NULL) {
    EfiFreePool(Parser->Key);
    Parser->Key = NULL;
  }
  // Reset the XML parser
  return XmlReset(Parser->Parser, Source);
}
// PlistFree
/// Free a PLIST parser
/// @param Parser The PLIST parser to free
/// @retval EFI_SUCCESS           The PLIST parser was freed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
EFI_STATUS
EFIAPI
PlistFree (
  IN PLIST_PARSER *Parser
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Parser->Dictionary != NULL) {
    PlistDictionaryFree(Parser->Dictionary);
    Parser->Dictionary = NULL;
  }
  if (Parser->Key != NULL) {
    EfiFreePool(Parser->Key);
    Parser->Key = NULL;
  }
  while (Parser->Stack != NULL) {
    PLIST_LIST *Stack = Parser->Stack;
    Parser->Stack = Stack->Next;
    EfiFreePool(Stack);
  }
  if (Parser->Parser != NULL) {
    XmlFree(Parser->Parser);
    Parser->Parser = NULL;
  }
  EfiFreePool(Parser);
  return EFI_SUCCESS;
}

// PlistFreeValue
/// Free PLIST value
/// @param Type  The type of the value
/// @param Value The value to free
STATIC
VOID
EFIAPI
PlistFreeValue (
  IN PLIST_TYPE   Type,
  IN PLIST_VALUE *Value
) {
  // Check parameters
  if (Value != NULL) {
    // Determine type of value
    switch (Type) {
      case PlistTypeDictionary:
      case PlistTypeArray:
        // Free dictionary or array
        if (Value->Dictionary != NULL) {
          PlistDictionaryFree(Value->Dictionary);
        }
        break;

      case PlistTypeString:
        // Free string
        if (Value->String != NULL) {
          EfiFreePool(Value->String);
          Value->String = NULL;
        }
        break;

      case PlistTypeDate:
        // Free date
        if (Value->Date != NULL) {
          EfiFreePool(Value->Date);
          Value->Date = NULL;
        }
        break;

      case PlistTypeData:
        // Free data
        if (Value->Data != NULL) {
          EfiFreePool(Value->Data);
          Value->Data = NULL;
        }
        break;

      default:
        // The rest of the value types do not require freeing as they are sizeof(UINT64) or less
        break;
    }
  }
}
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
) {
  // Check parameters
  if (Dictionary == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free next key if needed
  if (Dictionary->Next != NULL) {
    PlistDictionaryFree(Dictionary->Next);
    Dictionary->Next = NULL;
  }
  // Free key name
  if (Dictionary->Name != NULL) {
    EfiFreePool(Dictionary->Name);
    Dictionary->Name = NULL;
  }
  // Free the key value
  PlistFreeValue(Dictionary->Type, &(Dictionary->Value));
  // Free key
  return EfiFreePool((VOID *)Dictionary);
}

// XmlParserFromPlistParser
/// Get the XML parser from a PLIST parser
/// @param Parser The PLIST parser of which to get the XML parser
/// @return The XML parser associated with the specified PLIST parser or NULL if there was an error
XML_PARSER *
EFIAPI
XmlParserFromPlistParser (
  IN PLIST_PARSER *Parser
) {
  if (Parser == NULL) {
    return NULL;
  }
  return Parser->Parser;
}
// ParserFromPlistParser
/// Get the language parser from a PLIST parser
/// @param Parser The PLIST parser of which to get the language parser
/// @return The language parser associated with the specified PLIST parser or NULL if there was an error
LANG_PARSER *
EFIAPI
ParserFromPlistParser (
  IN PLIST_PARSER *Parser
) {
  if (Parser == NULL) {
    return NULL;
  }
  return ParserFromXmlParser(Parser->Parser);
}

// PlistImportValue
/// Import a PLIST value into the dictionary
/// @param Type  The PLIST value type
/// @param Value The PLIST value to import
/// @param Size  The size in bytes of the PLIST value
/// @param Path  The current configuration path while walking and importing the PLIST dictionary
STATIC
EFI_STATUS
EFIAPI
PlistImportValue (
  IN PLIST_TYPE    Type,
  IN PLIST_VALUE  *Value,
  IN UINTN         Size,
  IN CONST CHAR16 *Path
) {
  EFI_STATUS Status = EFI_SUCCESS;
  if ((Value == NULL) || (Path == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Determine the type of the key
  switch (Type) {
    case PlistTypeDictionary:
    case PlistTypeArray: {
      // Dictionary key type
      PLIST_KEY *Dictionary = Value->Dictionary;
      while (Dictionary != NULL) {
        Status = PlistImportConfiguration(Dictionary, Path);
        if (EFI_ERROR(Status)) {
          break;
        }
        Dictionary = Dictionary->Next;
      }
      break;
    }

    case PlistTypeString:
      // String key type
      EfiConfigurationSetString(Path, Value->String);
      break;

    case PlistTypeDate:
      // Date key type
      EfiConfigurationSetDate(Path, Value->Date);
      break;

    case PlistTypeData:
      // Data key type
      EfiConfigurationSetData(Path, Value->Data, Size);
      break;

    case PlistTypeUnsigned:
      // Unsigned key type
      EfiConfigurationSetUnsigned(Path, Value->Unsigned);
      break;

    case PlistTypeInteger:
      // Integer key type
      EfiConfigurationSetInteger(Path, Value->Integer);
      break;

    case PlistTypeBoolean:
      // Boolean key type
      EfiConfigurationSetBoolean(Path, Value->Boolean);
      break;

    case PlistTypeReal:
      // Real key type
      EfiConfigurationSetFloat(Path, Value->Real);
      break;

    default:
      // Unusable or unknown type so just skip
      break;
  }
  return Status;
}
// PlistImportConfiguration
/// Import a PLIST dictionary into the configuration
/// @param Dictionary The PLIST dictionary to import
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was imported
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_INVALID_PARAMETER If Dictionary is NULL
EFI_STATUS
EFIAPI
PlistImportConfiguration (
  IN PLIST_KEY    *Dictionary,
  IN CONST CHAR16 *KeyPath OPTIONAL
) {
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR16     *Path;
  // Check parameters
  if (Dictionary == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the full path to this key
  Path = EfiPoolPrint(L"%s/%s", (KeyPath != NULL) ? KeyPath : L"", Dictionary->Name);
  if (Path == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Set value in configuration
  Status = PlistImportValue(Dictionary->Type, &(Dictionary->Value), Dictionary->Size, Path);
  // Free full key path and return status
  EfiFreePool(Path);
  return Status;
}

// PlistXmlAddKey
/// Add a new PLIST key to a dictionary
/// @param Parser The PLIST parser
/// @param Type   The type of the key
/// @return The added key or NULL if there was an error
STATIC
PLIST_KEY *
EFIAPI
PlistXmlAddKey (
  IN OUT PLIST_PARSER *Parser,
  IN     PLIST_TYPE    Type
) {
  PLIST_LIST *Stack;
  PLIST_KEY  *Key;
  // Check parameters
  if (Parser == NULL) {
    return NULL;
  }
  // Create a new key
  Stack = Parser->Stack;
  if (Stack == NULL) {
    if (Parser->Dictionary != NULL) {
      // There is no where to put the key
      return NULL;
    }
    // Create root key
    Key = EfiAllocateByType(PLIST_KEY);
    Parser->Dictionary = Key;
  } else if (Parser->Dictionary == NULL) {
    // Why is the stack not empty if the root key is empty?
    return NULL;
  } else if ((Stack->Key == NULL) || ((Stack->Key->Type != PlistTypeDictionary) && (Stack->Key->Type != PlistTypeArray))) {
    // Only a dictionary or an array can have children
    return NULL;
  } else {
    // Add the key as a child of the dictionary/array on top of the stack
    if (Stack->Key->Value.Dictionary == NULL) {
      // First child
      Key = EfiAllocateByType(PLIST_KEY);
      Stack->Key->Value.Dictionary = Key;
    } else {
      // Append child to end of list
      Key = Stack->Key->Value.Dictionary;
      while (Key->Next != NULL) {
        Key = Key->Next;
      }
      Key->Next = EfiAllocateByType(PLIST_KEY);
      Key = Key->Next;
    }
    // Set the key name
    Key->Name = Parser->Key;
    Parser->Key = NULL;
  }
  // Set the key type
  Key->Type = Type;
  return Key;
}
// PlistExportCallback
/// Configuration key enumeration callback
/// @param Key     The full key path identifier
/// @param Name    The key name
/// @param Type    The type of the key value
/// @param Size    The size in bytes of the key value
/// @param Value   The value of the key
/// @param Context The enumeration callback context
/// @return Whether to continue the enumeration or not
STATIC
BOOLEAN
EFIAPI
PlistExportCallback (
  IN CONST CHAR16           *Key,
  IN CONST CHAR16           *Name,
  IN EFI_CONFIGURATION_TYPE  Type,
  IN UINTN                   Size,
  IN VOID                   *Value,
  IN VOID                   *Context
) {
  PLIST_PARSER *Parser = (PLIST_PARSER *)Context;
  PLIST_KEY    *This;
  PLIST_KEY    *Ptr;
  PLIST_LIST   *List;
  CHAR16       *Path;
  PLIST_TYPE    PlistType;
  LOG(L"PlistExportCallback()\n");
  // Determine the type of the PLIST key from the configuration type
  switch (Type) {
    case EfiConfigurationTypeList:
      PlistType = PlistTypeDictionary;
      break;

    case EfiConfigurationTypeArray:
      PlistType = PlistTypeArray;
      break;

    case EfiConfigurationTypeData:
      PlistType = PlistTypeData;
      break;

    case EfiConfigurationTypeDate:
      PlistType = PlistTypeDate;
      break;

    case EfiConfigurationTypeString:
      PlistType = PlistTypeString;
      break;

    case EfiConfigurationTypeBoolean:
      PlistType = PlistTypeBoolean;
      break;

    case EfiConfigurationTypeUnsigned:
      PlistType = PlistTypeUnsigned;
      break;

    case EfiConfigurationTypeInteger:
      PlistType = PlistTypeInteger;
      break;

    case EfiConfigurationTypeFloat:
      PlistType = PlistTypeReal;
      break;

    default:
      // Unknown type
      return FALSE;
  }
  // Only set a key name for dictionary or leaf key types and not for an array
  if ((Parser->Stack != NULL) && (Parser->Stack->Key != NULL) && (StrLen(Name) != 0) &&
      (Parser->Stack->Key->Type != EfiConfigurationTypeArray)) {
    Parser->Key = StrDup(Name);
    if (Parser->Key == NULL) {
      return FALSE;
    }
  }
  // Add the dictionary using the same method as when parsing from XML
  This = PlistXmlAddKey(Parser, PlistType);
  if (This == NULL) {
    return FALSE;
  }
  // Determine the type of key as they hold their value differently
  switch (PlistType) {
    case PlistTypeDictionary:
    case PlistTypeArray:
      // Push the dictionary onto the stack
      List = EfiAllocateByType(PLIST_LIST);
      if (List == NULL) {
        return FALSE;
      }
      List->Key = This;
      List->Next = Parser->Stack;
      Parser->Stack = List;
      // Get the path of each child
      Ptr = (PLIST_KEY *)Value;
      while (Ptr != NULL) {
        // Create the path for this key
        Path = EfiPoolPrint(L"%s/%s", Key, Ptr->Name);
        if (Path == NULL) {
          return FALSE;
        }
        // Enumerate the configuration to export each child
        if (EFI_ERROR(EfiConfigurationEnumerate(Path, 0, PlistExportCallback, Parser, FALSE))) {
          EfiFreePool(Path);
          return FALSE;
        }
        // Free the path
        EfiFreePool(Path);
        // Get the next key
        Ptr = Ptr->Next;
      }
      // Remove the dictionary from the stack
      List = Parser->Stack;
      Parser->Stack = List->Next;
      EfiFreePool(List);
      break;

    case PlistTypeData:
      // Duplicate the data
      This->Size = Size;
      This->Value.Data = EfiDuplicate(Size, Value);
      break;

    case PlistTypeDate:
      // Duplicate the date
      This->Size = Size;
      This->Value.Date = (EFI_TIME *)EfiDuplicate(Size, Value);
      break;

    case PlistTypeString:
      // Duplicate the string
      This->Size = Size;
      This->Value.String = (CHAR16 *)EfiDuplicate(Size, Value);
      break;

    case PlistTypeReal:
      // Duplicate the integer
      This->Size = sizeof(FLOAT64);
      This->Value.Real = *((FLOAT64 *)Value);
      break;

    case PlistTypeUnsigned:
      // Duplicate the unsigned integer
      This->Size = sizeof(UINT64);
      This->Value.Unsigned = *((UINT64 *)Value);
      break;

    case PlistTypeInteger:
      // Duplicate the integer
      This->Size = sizeof(INT64);
      This->Value.Integer = *((INT64 *)Value);
      break;

    case PlistTypeBoolean:
      // Duplicate the integer
      This->Size = sizeof(BOOLEAN);
      This->Value.Boolean = *((BOOLEAN *)Value);
      break;

    default:
      // Unknown type
      return FALSE;
  }
  return TRUE;
}
// PlistExportConfiguration
/// Export the configuration into a PLIST dictionary
/// @param Dictionary On output, the exported configuration PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was exported
/// @retval EFI_INVALID_PARAMETER If Dictionary is NULL
EFI_STATUS
EFIAPI
PlistExportConfiguration (
  OUT PLIST_KEY **Dictionary
) {
  EFI_STATUS   Status;
  PLIST_PARSER Parser;
  // Check parameters
  if (Dictionary == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Setup the root key export
  Parser.Parser = NULL;
  Parser.Dictionary = NULL;
  Parser.Stack = NULL;
  Parser.Key = NULL;
  // Enumerate the configuration to export
  Status = EfiConfigurationEnumerate(NULL, 0, PlistExportCallback, &Parser, FALSE);
  // Remove any remaining stack items
  while (Parser.Stack != NULL) {
    PLIST_LIST *Stack = Parser.Stack;
    Parser.Stack = Stack->Next;
    EfiFreePool(Stack);
  }
  if (Parser.Key != NULL) {
    EfiFreePool(Parser.Key);
  }
  // Set dictionary and return status
  if (!EFI_ERROR(Status)) {
    *Dictionary = Parser.Dictionary;
  }
  return Status;
}

// PlistXmlInspector
/// Inspect XML document tree to build a PLIST dictionary
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
PlistXmlInspector (
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
  PLIST_PARSER *Parser;
  PLIST_LIST   *Stack;
  PLIST_KEY    *Key;
  UINTN         Index;
  UNUSED_PARAMETER(Tree);
  UNUSED_PARAMETER(Attributes);
  UNUSED_PARAMETER(AttributeCount);
  // Get the parser
  Parser = (PLIST_PARSER *)Context;
  if (Parser == NULL) {
    return FALSE;
  }
  // Only the PLIST tag can exist at the root
  if (Level == 0) {
    // Check this is PLIST document, must be only element at this level and have PLIST tag name
    if ((LevelIndex != 0) || (StriCmp(TagName, L"plist") != 0) || (ChildCount != 1) ||
        EFI_ERROR(XmlTreeInspect(Children[0], Level + 1, 0, PlistXmlInspector, Context, FALSE))) {
      return FALSE;
    }
    return TRUE;
  }
  // Check there is only one value for the PLIST dictionary
  if(Level == 1) {
    // Must be only element at this level
    if (LevelIndex != 0) {
      return FALSE;
    }
  } else if ((LevelIndex % 2) == 0) {
    // Key name
    if ((StriCmp(TagName, L"key") != 0) || (Value == NULL) || (*Value == 0) || (Parser->Key != NULL)) {
      return FALSE;
    }
    // Set the key name
    Parser->Key = StrDup(Value);
    if (Parser->Key == NULL) {
      return FALSE;
    }
    return TRUE;
  }
  // Create the key
  if (StriCmp(TagName, L"dict") == 0) {
    // Dictionary
    Key = PlistXmlAddKey(Parser, PlistTypeDictionary);
    if (Key == NULL) {
      return FALSE;
    }
    // Push key on to stack
    Stack = EfiAllocateByType(PLIST_LIST);
    if (Stack == NULL) {
      PlistDictionaryFree(Key);
      return FALSE;
    }
    Stack->Key = Key;
    // Push the key on to the stack
    Stack->Next = Parser->Stack;
    Parser->Stack = Stack;
    // Inspect children
    if (Children != NULL) {
      ++Level;
      for (Index = 0; Index < ChildCount; ++Index) {
        if (Children[Index] != NULL) {
          if (EFI_ERROR(XmlTreeInspect(Children[Index], Level, Index, PlistXmlInspector, Context, FALSE))) {
            return FALSE;
          }
        }
      }
      --Level;
    }
    // Remove key from stack
    Stack = Parser->Stack;
    Parser->Stack = Parser->Stack->Next;
    EfiFreePool(Stack);
  } else if (StriCmp(TagName, L"array") == 0) {
    // Array
    Key = PlistXmlAddKey(Parser, PlistTypeArray);
    if (Key == NULL) {
      return FALSE;
    }
    // Push key on to stack
    Stack = EfiAllocateByType(PLIST_LIST);
    if (Stack == NULL) {
      PlistDictionaryFree(Key);
      return NULL;
    }
    Stack->Key = Key;
    // Push the key on to the stack
    Stack->Next = Parser->Stack;
    Parser->Stack = Stack;
    // Inspect children
    if (Children != NULL) {
      ++Level;
      for (Index = 0; Index < ChildCount; ++Index) {
        if (Children[Index] != NULL) {
          if (EFI_ERROR(XmlTreeInspect(Children[Index], Level, Index << 1, PlistXmlInspector, Context, FALSE))) {
            return FALSE;
          }
        }
      }
      --Level;
    }
    // Remove key from stack
    Stack = Parser->Stack;
    Parser->Stack = Parser->Stack->Next;
    EfiFreePool(Stack);
  } else if (StriCmp(TagName, L"string") == 0) {
    // String
    Key = PlistXmlAddKey(Parser, PlistTypeString);
    if (Key == NULL) {
      return FALSE;
    }
    if ((Value != NULL) && (*Value != 0)) {
      Key->Size = StrSize(Value);
      Key->Value.String = StrDup(Value);
      if (Key->Value.String == NULL) {
        return FALSE;
      }
    }
  } else if (StriCmp(TagName, L"date") == 0) {
    // Date and/or time
    Key = PlistXmlAddKey(Parser, PlistTypeDate);
    if (Key == NULL) {
      return FALSE;
    }
    Key->Size = sizeof(EFI_TIME);
    // Parse dare time as YYYY '-' MM '-' DD 'T' HH ':' MM ':' SS 'Z'
    if ((Value != NULL) && (*Value != 0)) {
      if (EFI_ERROR(StrToDateTime(&Value, Key->Value.Date))) {
        return FALSE;
      }
    }
  } else if (StriCmp(TagName, L"data") == 0) {
    // Binary data encoded in base64
    Key = PlistXmlAddKey(Parser, PlistTypeData);
    if (Key == NULL) {
      return FALSE;
    }
    // Decode the base64 string to binary data
    Key->Size = 0;
    Key->Value.Data = NULL;
    if ((Value != NULL) && (*Value != 0)) {
      UINTN Size = StrSize(Value);
      if (Size != 0) {
        if (EFI_ERROR(Base64DecodeBuffer(EfiUtf16Encoding(), Value, Size, &(Key->Value.Data), &(Key->Size)))) {
          return FALSE;
        }
      }
    }
  } else if (StriCmp(TagName, L"real") == 0) {
    // Floating point real number
    Key = PlistXmlAddKey(Parser, PlistTypeReal);
    if (Key == NULL) {
      return FALSE;
    }
    Key->Size = sizeof(FLOAT64);
    if ((Value != NULL) && (*Value != 0)) {
      if (EFI_ERROR(StrToFloat(&Value, ADDRESS_OF(PLIST_KEY, Key, FLOAT64, Value.Real)))) {
        return FALSE;
      }
    }
  } else if (StriCmp(TagName, L"integer") == 0) {
    // Integer
    Key = PlistXmlAddKey(Parser, PlistTypeInteger);
    if (Key == NULL) {
      return FALSE;
    }
    Key->Size = sizeof(INT64);
    if ((Value != NULL) && (*Value != 0)) {
      if (EFI_ERROR(StrToInteger(&Value, ADDRESS_OF(PLIST_KEY, Key, INT64, Value.Integer), 10))) {
        return FALSE;
      }
    }
  } else if (StriCmp(TagName, L"true") == 0) {
    // Boolean true
    Key = PlistXmlAddKey(Parser, PlistTypeBoolean);
    if (Key == NULL) {
      return FALSE;
    }
    Key->Size = sizeof(BOOLEAN);
    Key->Value.Boolean = TRUE;
  } else if (StriCmp(TagName, L"false") == 0) {
    // Boolean false
    Key = PlistXmlAddKey(Parser, PlistTypeBoolean);
    if (Key == NULL) {
      return FALSE;
    }
    Key->Size = sizeof(BOOLEAN);
    Key->Value.Boolean = FALSE;
  } else {
    // Unknown type
    ParseError(ParserFromPlistParser(Parser), L"Unknown type `%s` used in PLIST", TagName);
    return FALSE;
  }
  return TRUE;
}
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
EFI_STATUS
EFIAPI
PlistParse (
  IN  PLIST_PARSER          *Parser,
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
// PlistParseFinish
/// Finish a PLIST dictionary
/// @param Parser     The PLIST parser
/// @param Dictionary On output, the parsed PLIST dictionary
/// @retval EFI_INVALID_PARAMETER If Parser or Dictionary is NULL or *Dictionary is not NULL
/// @retval EFI_SUCCESS           The parsed PLIST dictionary was returned successfully
EFI_STATUS
EFIAPI
PlistParseFinish (
  IN OUT PLIST_PARSER  *Parser,
  IN     PLIST_KEY    **Dictionary
) {
  EFI_STATUS Status;
  // Check parameters
  if ((Parser == NULL) || (Dictionary == NULL) || (*Dictionary != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Finish parsing the XML
  Status = XmlParseFinish(XmlParserFromPlistParser(Parser));
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Create the PLIST dictionary from the XML
  Status = XmlInspect(Parser->Parser, PlistXmlInspector, Parser, FALSE);
  // Return the dictionary
  if (!EFI_ERROR(Status)) {
    *Dictionary = Parser->Dictionary;
    Parser->Dictionary = NULL;
  }
  return Status;
}

// PlistKeyToXml - forward declaration
/// Convert a PLIST dictionary key to an XML string
/// @param Key      The PLIST dictionary key
/// @param Encoding The encoding for the XML string buffer
/// @param Buffer   On output, the XML string buffer
/// @param Size     On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @param Level    The level of indentation for the key
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Dictionary, Buffer, or Size is NULL
STATIC
EFI_STATUS
EFIAPI
PlistKeyToXml (
  IN     PLIST_KEY             *Key,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer,
  IN OUT UINTN                 *Size,
  IN     UINTN                  Level
);

// PlistValueToXml
/// Convert a PLIST dictionary value to an XML string
/// @param Type      The PLIST dictionary key value type
/// @param Value     The PLIST dictionary key value
/// @param ValueSize The size in bytes of the PLIST dictionary key value
/// @param Encoding  The encoding for the XML string buffer
/// @param Buffer    On output, the XML string buffer
/// @param Size      On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @param Level     The level of indentation for the key
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Dictionary, Buffer, or Size is NULL
STATIC
EFI_STATUS
EFIAPI
PlistValueToXml (
  IN     PLIST_TYPE             Type,
  IN     PLIST_VALUE           *Value,
  IN     UINTN                  ValueSize,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer,
  IN OUT UINTN                 *Size,
  IN     UINTN                  Level
) {
  STATIC CONST CHAR8 *DictHeaderFormat = "% *a<dict>\n";
  STATIC CONST CHAR8 *DictFooterFormat = "% *a</dict>\n";
  STATIC CONST CHAR8 *DictEmptyFormat = "% *a<dict />\n";
  STATIC CONST CHAR8 *ArrayHeaderFormat = "% *a<array>\n";
  STATIC CONST CHAR8 *ArrayFooterFormat = "% *a</array>\n";
  STATIC CONST CHAR8 *ArrayEmptyFormat = "% *a<array />\n";
  STATIC CONST CHAR8 *StringFormat = "% *a<string>%s</string>\n";
  STATIC CONST CHAR8 *DataFormat = "% *a<data>%a</data>\n";
  STATIC CONST CHAR8 *DateFormat = "% *a<date>%04.4u-%02.2u-%02.2uT%02.2u:%02.2u:%02.2uZ</date>\n";
  STATIC CONST CHAR8 *DateEmptyFormat = "% *a<date>0000-00-00T00:00:00Z</date>\n";
  STATIC CONST CHAR8 *RealFormat = "% *a<real>&f</real>\n";
  STATIC CONST CHAR8 *UnsignedFormat = "% *a<integer>%u</integer>\n";
  STATIC CONST CHAR8 *IntegerFormat = "% *a<integer>%d</integer>\n";
  STATIC CONST CHAR8 *TrueFormat = "% *a<true />\n";
  STATIC CONST CHAR8 *FalseFormat = "% *a<false />\n";
  EFI_STATUS             Status = EFI_INVALID_PARAMETER;
  EFI_ENCODING_PROTOCOL *FormatEncoding;
  UINTN                  BufferSize = 0;
  UINTN                  ThisSize;
  // Check parameters
  if ((Value == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get ASCII encoding
  FormatEncoding = EfiLatin1Encoding();
  if (FormatEncoding == NULL) {
    return EFI_UNSUPPORTED;
  }
  // Determine type of value
  ThisSize = *Size;
  switch (Type) {
    case PlistTypeDictionary:
      // Dictionary key type
      if (Value->Dictionary == NULL) {
        // Empty dictionary
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DictEmptyFormat, AsciiStrSize(DictEmptyFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        BufferSize += ThisSize;
      } else {
        // Dictionary has keys
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DictHeaderFormat, AsciiStrSize(DictHeaderFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        ThisSize -= Encoding->UnitSize;
        BufferSize += ThisSize;
        // Advance the buffer
        if (Buffer != NULL) {
          Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
        }
        // Output the dictionary
        if (BufferSize < *Size) {
          ThisSize = *Size - BufferSize;
        } else {
          ThisSize = 0;
        }
        Status = PlistKeyToXml(Value->Dictionary, Encoding, Buffer, &ThisSize, Level + 1);
        // Increase the buffer size
        BufferSize += ThisSize;
        // Advance the buffer
        if (Buffer != NULL) {
          Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
        }
        // Close the dictionary
        if (BufferSize < *Size) {
          ThisSize = *Size - BufferSize;
        } else {
          ThisSize = 0;
        }
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DictFooterFormat, AsciiStrSize(DictFooterFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        BufferSize += ThisSize;
      }
      break;

    case PlistTypeArray:
      // Array key type
      if (Value->Dictionary == NULL) {
        // Empty array
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, ArrayEmptyFormat, AsciiStrSize(ArrayEmptyFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        BufferSize += ThisSize;
      } else {
        PLIST_KEY *Array;
        // Array has values
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, ArrayHeaderFormat, AsciiStrSize(ArrayHeaderFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        ThisSize -= Encoding->UnitSize;
        BufferSize += ThisSize;
        // Advance the buffer
        if (Buffer != NULL) {
          Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
        }
        // Increase the level indentation before the array values
        ++Level;
        // Output the array
        for (Array = Value->Dictionary; Array != NULL; Array = Array->Next) {
          if (BufferSize < *Size) {
            ThisSize = *Size - BufferSize;
          } else {
            ThisSize = 0;
          }
          // Import each array value
          Status = PlistValueToXml(Array->Type, &(Array->Value), Array->Size, Encoding, Buffer, &ThisSize, Level);
          if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
            break;
          }
          // Increase the buffer size
          BufferSize += ThisSize;
          // Advance the buffer
          if (Buffer != NULL) {
            Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
          }
        }
        // Decrease the level indentation after the array values
        --Level;
        // Close the dictionary
        if (BufferSize < *Size) {
          ThisSize = *Size - BufferSize;
        } else {
          ThisSize = 0;
        }
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, ArrayFooterFormat, AsciiStrSize(ArrayFooterFormat), Level * 2, "");
        if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
          return Status;
        }
        // Increase the buffer size
        if (ThisSize < Encoding->UnitSize) {
          return EFI_INVALID_PARAMETER;
        }
        BufferSize += ThisSize;
      }
      break;

    case PlistTypeString:
      // String key type
      Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, StringFormat, AsciiStrSize(StringFormat), Level * 2, "", Value->String);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    case PlistTypeDate:
      // Date key type
      // Print the date and time according to ISO 8601
      if (Value->Date != NULL) {
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DateFormat, AsciiStrSize(DateFormat), Level * 2, "",
                                 Value->Date->Year, Value->Date->Month, Value->Date->Day, Value->Date->Hour, Value->Date->Minute, Value->Date->Second);
      } else {
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DateEmptyFormat, AsciiStrSize(DateEmptyFormat), Level * 2, "");
      }
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    case PlistTypeData: {
      // Data key type
      CHAR8 *Data = NULL;
      // Convert the data to base 64 encoded string
      BufferSize = 0;
      Base64EncodeBuffer(EfiLatin1Encoding(), Value->Data, ValueSize, &Data, &BufferSize);
      // Print the base 64 encoded data string
      ThisSize = *Size;
      Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, DataFormat, AsciiStrSize(DataFormat), Level * 2, "", (Data != NULL) ? Data : "");
      if (Data != NULL) {
        EfiFreePool(Data);
        Data = NULL;
      }
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;
    }

    case PlistTypeReal:
      // Real key type
      Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, RealFormat, AsciiStrSize(RealFormat), Level * 2, "", Value->Real);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    case PlistTypeUnsigned:
      // Unsigned key type
      Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, UnsignedFormat, AsciiStrSize(UnsignedFormat), Level * 2, "", Value->Unsigned);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    case PlistTypeInteger:
      // Integer key type
      Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, IntegerFormat, AsciiStrSize(IntegerFormat), Level * 2, "", Value->Integer);
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    case PlistTypeBoolean:
      // Boolean key type
      if (Value->Boolean) {
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, TrueFormat, AsciiStrSize(TrueFormat), Level * 2, "");
      } else {
        Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, FalseFormat, AsciiStrSize(FalseFormat), Level * 2, "");
      }
      if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
        return Status;
      }
      // Increase the buffer size
      if (ThisSize < Encoding->UnitSize) {
        return EFI_INVALID_PARAMETER;
      }
      BufferSize += ThisSize;
      break;

    default:
      // Unusable or unknown type so just skip
      break;
  }
  // Set and return size
  BufferSize -= Encoding->UnitSize;
  *Size = BufferSize;
  return Status;
}
// PlistKeyToXml
/// Convert a PLIST dictionary key to an XML string
/// @param Key      The PLIST dictionary key
/// @param Encoding The encoding for the XML string buffer
/// @param Buffer   On output, the XML string buffer
/// @param Size     On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @param Level    The level of indentation for the key
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Key or Size is NULL
STATIC
EFI_STATUS
EFIAPI
PlistKeyToXml (
  IN     PLIST_KEY             *Key,
  IN     EFI_ENCODING_PROTOCOL *Encoding,
  OUT    VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size,
  IN     UINTN                  Level
) {
  STATIC CONST CHAR8 *NameFormat = "% *a<key>%s<key>\n";
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN      BufferSize = 0;
  UINTN      RemainingSize;
  UINTN      ThisSize;
  // Check parameters
  if ((Key == NULL) || (Size == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the size of the buffer
  RemainingSize = *Size;
  ThisSize = RemainingSize;
  // Print the key name element to the XML buffer
  if (Key->Name != NULL) {
    ThisSize = RemainingSize;
    Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, EfiLatin1Encoding(), NameFormat, AsciiStrSize(NameFormat), Level * 2, "", Key->Name);
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
    ThisSize = RemainingSize;
  }
  // Convert the PLIST key value to XML string
  Status = PlistValueToXml(Key->Type, &(Key->Value), Key->Size, Encoding, Buffer, &ThisSize, Level);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  if (ThisSize < Encoding->UnitSize) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the next PLIST key
  if (Key->Next != NULL) {
    // Increase the buffer size
    BufferSize += ThisSize;
    if (ThisSize < RemainingSize) {
      RemainingSize -= ThisSize;
    } else {
      RemainingSize = 0;
    }
    // Advance the buffer
    if (Buffer != NULL) {
      Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    }
    // Convert the next PLIST key to XML string
    ThisSize = RemainingSize;
    Status = PlistKeyToXml(Key->Next, Encoding, Buffer, &ThisSize, Level);
    if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
      return Status;
    }
    if (ThisSize < Encoding->UnitSize) {
      return EFI_INVALID_PARAMETER;
    }
  }
  BufferSize += ThisSize;
  // Return buffer size and status
  *Size = BufferSize;
  return Status;
}
// PlistToXml
/// Convert a PLIST dictionary to an XML string
/// @param Dictionary The PLIST dictionary
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On input, the available size in bytes of the data buffer, on output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_BUFFER_TOO_SMALL  The buffer size was not enough and *Size has been updated with the needed size
/// @retval EFI_INVALID_PARAMETER If Dictionary or Size is NULL
EFI_STATUS
EFIAPI
PlistToXml (
  IN     PLIST_KEY             *Dictionary,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN OUT VOID                  *Buffer OPTIONAL,
  IN OUT UINTN                 *Size
) {
  STATIC CONST CHAR8 *HeaderFormat = "<?xml version=\"1.0\" encoding=\"%a\" ?>\n"
                                     "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                                     "<plist version=\"1.0\">\n";
  STATIC CONST CHAR8 *FooterFormat = "</plist>\n";
  EFI_STATUS             Status;
  EFI_ENCODING_PROTOCOL *FormatEncoding;
  UINTN                  BufferSize = 0;
  UINTN                  RemainingSize;
  UINTN                  ThisSize;
  // Check parameters
  if ((Dictionary == NULL) || (Size == NULL)) {
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
  // Convert the PLIST keys to XML
  ThisSize = RemainingSize;
  Status = PlistKeyToXml(Dictionary, Encoding, Buffer, &ThisSize, 0);
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
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
  // Close the document
  ThisSize = RemainingSize;
  Status = EfiLocaleSPrint(NULL, Encoding, Buffer, &ThisSize, FormatEncoding, FooterFormat, AsciiStrSize(FooterFormat));
  if (EFI_ERROR(Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }
  if (ThisSize < Encoding->UnitSize) {
    return EFI_INVALID_PARAMETER;
  }
  BufferSize += ThisSize;
  // Return the size and status
  *Size = BufferSize;
  return Status;
}
// PlistToXmlBuffer
/// Convert a PLIST dictionary to an UTF-16 XML string
/// @param Dictionary The PLIST dictionary
/// @param Encoding   The encoding for the XML string buffer
/// @param Buffer     On output, the XML string buffer
/// @param Size       On output, the size in bytes of the XML string buffer
/// @retval EFI_SUCCESS           The PLIST dictionary was converted to XML
/// @retval EFI_INVALID_PARAMETER If Dictionary or Buffer is NULL
EFI_STATUS
EFIAPI
PlistToXmlBuffer (
  IN  PLIST_KEY              *Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  OUT VOID                  **Buffer,
  OUT UINTN                  *Size OPTIONAL
) {
  EFI_STATUS  Status;
  UINTN       XmlSize = 0;
  VOID       *Xml = NULL;
  // Check parameters
  if ((Dictionary == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Encoding == NULL) {
    Encoding = EfiUtf16Encoding();
    if (Encoding == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  // Get the size needed for the XML string buffer
  Status = PlistToXml(Dictionary, Encoding, NULL, &XmlSize);
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
  // Convert the PLIST dictionary to XML string
  Status = PlistToXml(Dictionary, Encoding, Xml, &XmlSize);
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
EFI_STATUS
EFIAPI
PlistLoad (
  OUT PLIST_KEY             **Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  EFI_FILE_PROTOCOL      *Root,
  IN  CONST CHAR16           *Path OPTIONAL
) {
  EFI_STATUS         Status;
  PLIST_PARSER      *Parser = NULL;
  VOID              *Buffer = NULL;
  UINT64             Size = 0;
  // Check parameters
  if ((Dictionary == NULL) || (*Dictionary != NULL) || (Root == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Load the PLIST dictionary from file
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
  // Create the PLIST parser
  Status = PlistCreate(&Parser, Path);
  if (!EFI_ERROR(Status)) {
    // Parse the XML buffer for a PLIST dictionary
    Status = PlistParse(Parser, Encoding, Buffer, (UINTN)Size);
    if (!EFI_ERROR(Status)) {
      // Finish parsing and create the PLIST dictionary
      Status = PlistParseFinish(Parser, Dictionary);
    }
  }
  // Free the PLIST parser
  if (Parser != NULL) {
    PlistFree(Parser);
  }
  // Free the buffer
  EfiFreePool(Buffer);
  return Status;
}
// PlistLoadPath
/// Load a PLIST dictionary from file
/// @param Dictionary On output, the PLIST dictionary
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the PLIST file to load
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EFI_STATUS
EFIAPI
PlistLoadPath (
  OUT PLIST_KEY             **Dictionary,
  IN  EFI_ENCODING_PROTOCOL  *Encoding OPTIONAL,
  IN  CONST CHAR16           *DevicePath
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath;
  // Check parameters;
  if ((Dictionary == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_NOT_FOUND;
  }
  // Load device path and import configuration
  Status = PlistLoadDevicePath(Dictionary, Encoding, ActualDevicePath);
  // Free device path and return result
  EfiFreePool(ActualDevicePath);
  return Status;
}
// PlistLoadDevicePath
/// Load a PLIST dictionary from file
/// @param Dictionary On output, the PLIST dictionary
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the PLIST file to load
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EFI_STATUS
EFIAPI
PlistLoadDevicePath (
  OUT PLIST_KEY                **Dictionary,
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
        // Load the PLIST dictionary file
        Status = PlistLoad(Dictionary, Encoding, Root, FileName);
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
EFI_STATUS
EFIAPI
PlistSave (
  IN PLIST_KEY             *Dictionary,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
) {
  EFI_STATUS         Status;
  VOID              *Buffer = NULL;
  UINTN              Size = 0;
  // Check parameters
  if ((Dictionary == NULL) || (Root == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the PLIST dictionary to XML buffer
  Status = PlistToXmlBuffer(Dictionary, Encoding, &Buffer, &Size);
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
  // Save the PLIST dictionary to file
  Status = EfiFileSave(Root, Path, Size, Buffer);
  // Free the buffer and return the result
  EfiFreePool(Buffer);
  return Status;
}
// PlistSavePath
/// Save a PLIST dictionary to file
/// @param Dictionary The PLIST dictionary to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string of the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was saved
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EFI_STATUS
EFIAPI
PlistSavePath (
  IN PLIST_KEY             *Dictionary,
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *ActualDevicePath;
  // Check parameters;
  if ((Dictionary == NULL) || (DevicePath == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  ActualDevicePath = EfiConvertTextToDevicePath(DevicePath);
  if (ActualDevicePath == NULL) {
    return EFI_NOT_FOUND;
  }
  // Load device path and import configuration
  Status = PlistSaveDevicePath(Dictionary, Encoding, ActualDevicePath);
  // Free device path and return result
  EfiFreePool(ActualDevicePath);
  return Status;
}
// PlistSaveDevicePath
/// Save a PLIST dictionary to file
/// @param Dictionary The PLIST dictionary to save
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path of the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was saved
/// @retval EFI_ABORTED           The PLIST dictionary is invalid
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Dictionary or DevicePath is NULL
EFI_STATUS
EFIAPI
PlistSaveDevicePath (
  IN PLIST_KEY                *Dictionary,
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
        Status = PlistSave(Dictionary, Encoding, Root, FileName);
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
EFI_STATUS
EFIAPI
PlistLoadAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL,
  IN CONST CHAR16          *KeyPath OPTIONAL
) {
  EFI_STATUS  Status;
  PLIST_KEY  *Dictionary = NULL;
  // Check parameters
  if (Root == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Load the PLIST dictionary from XML file
  Status = PlistLoad(&Dictionary, Encoding, Root, Path);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Dictionary == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Import the configuration from a PLIST dictionary
  Status = PlistImportConfiguration(Dictionary, KeyPath);
  // Free PLIST dictionary
  PlistDictionaryFree(Dictionary);
  return Status;
}
// PlistLoadPathAndImportConfiguration
/// Load a PLIST dictionary from file and import into the configuration
/// @param Encoding   The encoding of the file
/// @param DevicePath The full device path string of the PLIST dictionary file to load
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded and imported
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EFI_STATUS
EFIAPI
PlistLoadPathAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath,
  IN CONST CHAR16          *KeyPath OPTIONAL
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *DP;
  // Check parameters;
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  DP = EfiConvertTextToDevicePath(DevicePath);
  if (DP == NULL) {
    return EFI_NOT_FOUND;
  }
  // Load device path and import configuration
  Status = PlistLoadDevicePathAndImportConfiguration(Encoding, DP, KeyPath);
  // Free device path and return result
  EfiFreePool(DP);
  return Status;
}
// PlistLoadDevicePathAndImportConfiguration
/// Load a PLIST dictionary from file and import into the configuration
/// @param Encoding   The encoding of the file
/// @param DevicePath The device path of the PLIST dictionary file to load
/// @param KeyPath    The optional key path to prepend to the imported PLIST dictionary
/// @retval EFI_SUCCESS           The PLIST dictionary was loaded and imported
/// @retval EFI_ABORTED           The PLIST file is invalid
/// @retval EFI_NOT_FOUND         The PLIST file was not found
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EFI_STATUS
EFIAPI
PlistLoadDevicePathAndImportConfiguration (
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST CHAR16             *KeyPath OPTIONAL
) {
  EFI_STATUS  Status;
  PLIST_KEY  *Dictionary = NULL;
  // Check parameters
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Load the PLIST dictionary from XML file
  Status = PlistLoadDevicePath(&Dictionary, Encoding, DevicePath);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Dictionary == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Import the configuration from a PLIST dictionary
  Status = PlistImportConfiguration(Dictionary, KeyPath);
  // Free PLIST dictionary
  PlistDictionaryFree(Dictionary);
  return Status;
}

// PlistExportConfigurationAndSave
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding The encoding for the file
/// @param Root     The root file protocol interface
/// @param Path     The path where to save the PLIST dictionary file or NULL if Root is already the PLIST file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If Root is NULL
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSave (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN EFI_FILE_PROTOCOL     *Root,
  IN CONST CHAR16          *Path OPTIONAL
) {
  EFI_STATUS  Status;
  PLIST_KEY  *Dictionary = NULL;
  // Check parameters
  if (Root == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Export the configuration to a PLIST dictionary
  Status = PlistExportConfiguration(&Dictionary);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Dictionary == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Save the PLIST dictionary to XML file
  Status = PlistSave(Dictionary, Encoding, Root, Path);
  // Free PLIST dictionary
  PlistDictionaryFree(Dictionary);
  return Status;
}
// PlistExportConfigurationAndSavePath
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding   The encoding for the file
/// @param DevicePath The full device path string where to save the PLIST dictionary file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSavePath (
  IN EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN CONST CHAR16          *DevicePath
) {
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *DP;
  // Check parameters;
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Convert the device path string to device path
  DP = EfiConvertTextToDevicePath(DevicePath);
  if (DP == NULL) {
    return EFI_NOT_FOUND;
  }
  // Export configuration and save device path
  Status = PlistExportConfigurationAndSaveDevicePath(Encoding, DP);
  // Free device path and return result
  EfiFreePool(DP);
  return Status;
}
// PlistExportConfigurationAndSaveDevicePath
/// Export the configuration to a PLIST dictionary and save to file
/// @param Encoding   The encoding for the file
/// @param DevicePath The device path where to save the PLIST dictionary file
/// @retval EFI_SUCCESS           The PLIST dictionary was exported and saved
/// @retval EFI_ACCESS_DENIED     The PLIST file could not be saved
/// @retval EFI_INVALID_PARAMETER If DevicePath is NULL
EFI_STATUS
EFIAPI
PlistExportConfigurationAndSaveDevicePath (
  IN EFI_ENCODING_PROTOCOL    *Encoding OPTIONAL,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
) {
  EFI_STATUS  Status;
  PLIST_KEY  *Dictionary = NULL;
  // Check parameters
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Export the configuration to a PLIST dictionary
  Status = PlistExportConfiguration(&Dictionary);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Dictionary == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Save the PLIST dictionary to XML file
  Status = PlistSaveDevicePath(Dictionary, Encoding, DevicePath);
  // Free PLIST dictionary
  PlistDictionaryFree(Dictionary);
  return Status;
}
