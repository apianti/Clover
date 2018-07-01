///
/// @file Library/Uefi/Configuration.c
///
/// Configuration database
///

#include <Uefi.h>

// EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE
/// The configuration protocol implementation signature
#define EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE 0x00004749464E4F43

// gEfiConfigurationProtocolGuid
/// The configuration protocol unique identifier
EFI_GUID gEfiConfigurationProtocolGuid = EFI_CONFIGURATION_PROTOCOL_GUID;

// EFI_CONFIGURATION_ACTION
/// Configuration key action
typedef enum EFI_CONFIGURATION_ACTION EFI_CONFIGURATION_ACTION;
enum EFI_CONFIGURATION_ACTION {

  // ConfigurationActionFind
  /// Find configuration key
  ConfigurationActionFind = 0,
  // ConfigurationActionCreate
  /// Create configuration key
  ConfigurationActionCreate,
  // ConfigurationActionRemove
  /// Remove configuration key
  ConfigurationActionRemove

};
// EFI_CONFIGURATION_KEY
/// Configuration key
typedef struct EFI_CONFIGURATION_KEY EFI_CONFIGURATION_KEY;
struct EFI_CONFIGURATION_KEY {

  // Next
  /// The next key
  EFI_CONFIGURATION_KEY  *Next;
  // Name
  /// The key name
  CHAR16                 *Name;
  // Type
  /// The key type
  EFI_CONFIGURATION_TYPE  Type;
  // Size
  /// The size in bytes of the key value or the count of keys in a list or array type
  UINTN                   Size;
  // Value
  /// The key value
  VOID                   *Value;

};
// EFI_CONFIGURATION_PROTOCOL_IMPL
/// Configuration protocol implementation
typedef struct EFI_CONFIGURATION_PROTOCOL_IMPL EFI_CONFIGURATION_PROTOCOL_IMPL;
struct EFI_CONFIGURATION_PROTOCOL_IMPL {

  // Protocol
  /// The configuration protocol
  EFI_CONFIGURATION_PROTOCOL  Protocol;

  // Signature
  /// The signature of this protocol, should be EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE
  UINT64                      Signature;
  // Root
  /// The root configuration key
  EFI_CONFIGURATION_KEY      *Root;

};

// ConfigurationFinishKey
/// Finish using a configuration key by freeing all child resources
/// @param Key      The configuration key to finish
/// @param KeepName Whether to keep the name allocated or free the name
STATIC
VOID
EFIAPI
ConfigurationFinishKey (
  IN EFI_CONFIGURATION_KEY *Key,
  IN BOOLEAN                KeepName
) {
  if (Key != NULL) {
    // Free the key name
    if (!KeepName && (Key->Name != NULL)) {
      EfiFreePool(Key->Name);
      Key->Name = NULL;
    }
    // Free the value
    if (Key->Value != NULL) {
      // Check if list or array to finish child keys as well
      if ((Key->Type == EfiConfigurationTypeList) ||
          (Key->Type == EfiConfigurationTypeArray)) {
        // Get the child key list
        EFI_CONFIGURATION_KEY *List = (EFI_CONFIGURATION_KEY *)(Key->Value);
        // Finish and free child keys
        while (List != NULL) {
          // Get the next key
          EFI_CONFIGURATION_KEY *Next = List->Next;
          // Finish the key
          ConfigurationFinishKey(List, FALSE);
          // Free the key
          EfiFreePool(List);
          // Set the next key
          List = Next;
        }
      } else {
        // Free value
        EfiFreePool(Key->Value);
      }
      Key->Value = NULL;
    }
  }
}
// ConfigurationFind
/// Find a configuration key by key path identifier
/// @param Root   On input, the key to use as root, on output, the key with the specified key path identifier
/// @param Key    The key path identifier
/// @param Action The action to take for the configuration key
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_NOT_FOUND         If the key was not found
/// @retval EFI_SUCCESS           The key was found and returned successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationFind (
  IN OUT EFI_CONFIGURATION_KEY    **Root,
  IN     CONST CHAR16              *Key,
  IN     EFI_CONFIGURATION_ACTION   Action
) {
  EFI_CONFIGURATION_KEY *This;
  EFI_CONFIGURATION_KEY *List;
  CONST CHAR16          *Name;
  UINTN                  Length;
  // Check if root is valid and key is not null
  if (Root == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if the root exists
  This = *Root;
  if (This == NULL) {
    if ((Key == NULL) || (*Key == 0)) {
      // Check if it should be created
      if (Action == ConfigurationActionCreate) {
        // Allocate a new configuration key
        This = EfiAllocateByType(EFI_CONFIGURATION_KEY);
        if (This == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        // Set the root and return success
        *Root = This;
        return EFI_SUCCESS;
      } else if (Action == ConfigurationActionRemove) {
        // No root so nothing to remove
        return EFI_SUCCESS;
      }
    }
    // No root so the key can't be found
    return EFI_NOT_FOUND;
  }
  // Remove any leading separators
  if (Key != NULL) {
    while ((*Key == '/') || (*Key == '\\')) {
      ++Key;
    }
  }
  // No actual key path just return root key
  if ((Key == NULL) || (*Key == 0)) {
    // Check if remove action
    if (Action == ConfigurationActionRemove) {
      // Remove the root key
      ConfigurationFinishKey(This, FALSE);
      EfiFreePool(This);
      *Root = NULL;
    }
    return EFI_SUCCESS;
  }
  // Traverse the key path identifier to find the key
  while (*Key != 0) {
    CONST CHAR16 *Ptr;
    UINT64        Index = 0;
    // Get the next key name
    Name = Key;
    while ((*Key != 0) && (*Key != '/') && (*Key != '\\')) {
      ++Key;
    }
    // Get the length of the key name
    Length = (Key - Name);
    // Get the array index
    Ptr = Name;
    if (EFI_ERROR(StrToUnsigned(&Ptr, &Index, 10)) || (Ptr != Key)) {
      // Failed to get an unsigned because the key was not an unsigned so default to zero
      Index = 0;
    }
    // Remove any trailing path separators
    while ((*Key == '/') || (*Key == '\\')) {
      ++Key;
    }
    // Check the key type to make sure this is a list or an array
    if (This->Type == EfiConfigurationTypeList) {
      // Get the list of child keys
      List = (EFI_CONFIGURATION_KEY *)(This->Value);
      if (List != NULL) {
        // Traverse the keys to find the key with the name
        if ((List->Name != NULL) && (StrniCmp(List->Name, Name, Length) == 0)) {
          // Check if removing the key
          if (*Key == 0) {
            if (Action == ConfigurationActionRemove) {
              // Set the next key to replace this key
              This->Value = List->Next;
              // Finish and free the key
              ConfigurationFinishKey(List, FALSE);
              EfiFreePool(List);
              // Decrement list count
              --(This->Size);
              // Return no key and success
              *Root = NULL;
            } else {
              // Return key and success
              *Root = List;
            }
            return EFI_SUCCESS;
          }
          // Set new this key
          This = List;
          continue;
        }
        // Traverse the rest of the keys to find the key with the name
        while (List->Next != NULL) {
          if ((List->Next->Name != NULL) && (StrniCmp(List->Next->Name, Name, Length) == 0)) {
            // Check if removing the key
            if (*Key == 0) {
              if (Action == ConfigurationActionRemove) {
                EFI_CONFIGURATION_KEY *Next = List->Next;
                // Set the next key to replace this key
                List->Next = Next->Next;
                // Finish and free the key
                ConfigurationFinishKey(Next, FALSE);
                EfiFreePool(Next);
                // Decrement list count
                --(This->Size);
                // Return no key and success
                *Root = NULL;
              } else {
                // Set new this key
                *Root = List->Next;
              }
              return EFI_SUCCESS;
            }
            // Found a match stop looking
            break;
          }
          List = List->Next;
        }
        // Check if the key was found
        if (List->Next != NULL) {
          This = List->Next;
          continue;
        }
      }
    } else if (This->Type == EfiConfigurationTypeArray) {
      if (Ptr != Key) {
        // Reset the key pointer back to the name for the next child key
        Key = Name;
      }
      // Get the list of child keys
      List = (EFI_CONFIGURATION_KEY *)(This->Value);
      if (List != NULL) {
        // Remove the array index
        if ((*Key == 0) && (Action == ConfigurationActionRemove)) {
          // Remove Key
          if (Index == 0) {
            // Set next key to replace this key
            This->Value = List->Next;
            // Finish and free the key
            ConfigurationFinishKey(List, FALSE);
            EfiFreePool(List);
          } else {
            // Get key by index
            UINT64 Index2 = Index;
            while ((List->Next != NULL) && (--Index2 != 0)) {
              List = List->Next;
            }
            // Remove key if found
            if (List->Next != NULL) {
              // Set next key to replace this key
              This = List->Next;
              List->Next = This->Next;
              // Finish and free the key
              ConfigurationFinishKey(This, FALSE);
              EfiFreePool(This);
            }
          }
          // Return no key and success
          *Root = NULL;
          return EFI_SUCCESS;
        } else {
          // Change current child key to this key
          UINT64 Index2 = Index;
          while ((List != NULL) && (Index2 != 0)) {
            --Index2;
            List = List->Next;
          }
          // If the index is valid set the this key
          if (List != NULL) {
            if (*Key == 0) {
              *Root = List;
              return EFI_SUCCESS;
            }
            This = List;
            continue;
          }
        }
      }
      // If creating set name to null so its not created
      Name = NULL;
    } else if (Action == ConfigurationActionCreate) {
      // Appear to be overwriting existing key with different type
      This->Size = 0;
      // Remove previous value to overwrite
      if (This->Value != NULL) {
        EfiFreePool(This->Value);
        This->Value = NULL;
      }
      // Choose array type if only an index was the name
      This->Type = ((Index != 0) || (Ptr == Key)) ? EfiConfigurationTypeArray : EfiConfigurationTypeList;
    }
    // Child key not found, only continue if creating keys
    if (Action != ConfigurationActionCreate) {
      break;
    }
    // Create the child key
    List = EfiAllocateByType(EFI_CONFIGURATION_KEY);
    if (List == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    // No next key
    List->Next = NULL;
    // Set no value
    List->Size = 0;
    List->Value = NULL;
    // Duplicate key name
    List->Name = StrnDup(Name, Length);
    // Choose array type if only an index was the name
    List->Type = ((Index != 0) || (Ptr == Key)) ? EfiConfigurationTypeArray : EfiConfigurationTypeList;
    // Add the child to the list
    if (This->Value == NULL) {
      // First child
      This->Size = 1;
      This->Value = List;
    } else {
      EFI_CONFIGURATION_KEY *ListPtr = (EFI_CONFIGURATION_KEY *)(This->Value);
      // Insert child into list
      ++(This->Size);
      // Check if this child should be inserted at beginning of list
      if (StriCmp(List->Name, ListPtr->Name) < 0) {
        // Inserted at beginning
        List->Next = ListPtr;
        This->Value = (VOID *)(List);
      } else {
        // Determine where the child should be inserted
        while ((ListPtr->Next != NULL) && (StriCmp(List->Name, ListPtr->Next->Name) > 0)) {
          ListPtr = ListPtr->Next;
        }
        // Insert the child
        List->Next = ListPtr->Next;
        ListPtr->Next = List;
      }
    }
    // Check if this is the key requested
    if (*Key == 0) {
      *Root = List;
      return EFI_SUCCESS;
    }
    // Set the this key
    This = List;
  }
  // Check if the key was found
  if (This != *Root) {
    *Root = This;
    return EFI_SUCCESS;
  }
  // The key was not found
  if (Action == ConfigurationActionRemove) {
    *Root = NULL;
    return EFI_SUCCESS;
  }
  return EFI_NOT_FOUND;
}

// ConfigurationExists
/// Check key exists
/// @param This The configuration protocol interface
/// @param Key  The key path identifier
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_NOT_FOUND         If the key does not exist
/// @retval EFI_SUCCESS           The key exists
STATIC
EFI_STATUS
EFIAPI
ConfigurationExists (
  IN EFI_CONFIGURATION_PROTOCOL_IMPL *This,
  IN CONST CHAR16                    *Key
) {
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Protect the implementation root key
  Root = This->Root;
  // Find the key if it exists
  return ConfigurationFind(&Root, Key, ConfigurationActionFind);
}
// ConfigurationGetChildren
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
STATIC
EFI_STATUS
EFIAPI
ConfigurationGetChildren (
  IN  EFI_CONFIGURATION_PROTOCOL_IMPL   *This,
  IN  CONST CHAR16                      *Key,
  IN  BOOLEAN                            Full,
  OUT UINTN                             *Count,
  OUT CHAR16                          ***Children
) {
  EFI_STATUS             Status;
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (Count == NULL) || (Children == NULL) ||
      (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Protect the root key
  Root = This->Root;
  // Get the key
  Status = ConfigurationFind(&Root, Key, ConfigurationActionFind);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check if key type is list or array
  if ((Root->Type != EfiConfigurationTypeList) && (Root->Type != EfiConfigurationTypeArray)) {
    // Any other type doesn't support children
    return EFI_UNSUPPORTED;
  }
  // Set an empty list to begin
  *Count = 0;
  *Children = NULL;
  // Check if empty list or array
  if (Root->Value != NULL) {
    // Create the child key path identifiers
    CHAR16 *Name;
    // Get the children key list
    EFI_CONFIGURATION_KEY *List = (EFI_CONFIGURATION_KEY *)Root->Value;
    if (Root->Type == EfiConfigurationTypeList) {
      while (List != NULL) {
        if (List->Name != NULL) {
          // Create the key name
          if (Full) {
            // Full key path identifier
            Name = EfiPoolPrint(L"%s/%s", Key, List->Name);
          } else {
            // Key name only
            Name = EfiPoolPrint(L"%s", List->Name);
          }
          if (Name == NULL) {
            return EFI_OUT_OF_RESOURCES;
          }
          // Add the key path identifier or name to the list
          Status = StrList(Children, Count, Name, 0, STR_LIST_NO_DUPLICATES | STR_LIST_CASE_INSENSITIVE | STR_LIST_SORTED);
          EfiFreePool(Name);
          if (EFI_ERROR(Status)) {
            return Status;
          }
        }
        List = List->Next;
      }
    } else if (Root->Type == EfiConfigurationTypeArray) {
      UINTN Index = 0;
      // Type array children should have no name and are named by index
      while (List != NULL) {
        // Create the key name
        if (Full) {
          // Full key path identifier
          Name = EfiPoolPrint(L"%s/%u", Key, Index);
        } else {
          // Key name only
          Name = EfiPoolPrint(L"%u", Index);
        }
        if (Name == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        // Add the key path identifier or name to the list
        Status = StrList(Children, Count, Name, 0, STR_LIST_NO_DUPLICATES | STR_LIST_CASE_INSENSITIVE | STR_LIST_SORTED);
        EfiFreePool(Name);
        if (EFI_ERROR(Status)) {
          return Status;
        }
        // Advance the index
        ++Index;
        List = List->Next;
      }
    }
  }
  return EFI_SUCCESS;
}
// ConfigurationGet
/// Get a configuration value by key
/// @param This  The configuration protocol interface
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @param Type  On output, the type of the value
/// @retval EFI_INVALID_PARAMETER If Key or Value is NULL
/// @retval EFI_SUCCESS           The value, size, and type of the key were returned successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationGet (
  IN     EFI_CONFIGURATION_PROTOCOL_IMPL  *This,
  IN     CONST CHAR16                     *Key,
  IN     CONST VOID                      **Value,
  IN OUT UINTN                            *Size OPTIONAL,
  OUT    EFI_CONFIGURATION_TYPE           *Type OPTIONAL
) {
  EFI_STATUS             Status;
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Protect the root key
  Root = This->Root;
  // Find the key
  Status = ConfigurationFind(&Root, Key, ConfigurationActionFind);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Return the value
  *Value = Root->Value;
  // Return the type if needed
  if (Type != NULL) {
    *Type = Root->Type;
  }
  // Return the size if needed
  if (Size != NULL) {
    *Size = Root->Size;
  }
  // Return the value
  return Status;
}
// ConfigurationCheckType
/// Check key type is valid
/// @param Type The type or type filter to check is valid
/// @param Size The size in bytes of the type
/// @return Whether the key type is valid or not
STATIC
BOOLEAN
EFIAPI
ConfigurationCheckType (
  IN EFI_CONFIGURATION_TYPE Type,
  IN UINTN                  Size
) {
  // Determine the data type and if the size is valid
  if (Type == EfiConfigurationTypeData) {
    // Any size is acceptable for data type
    return (Size > 0);
  } else if (Type == EfiConfigurationTypeString) {
    // Any size that is a multiple of sizeof(CHAR16) is acceptable for string type
    return ((Size > 0) && ((Size % sizeof(CHAR16)) == 0));
  } else if (Type == EfiConfigurationTypeBoolean) {
    // Must be the size of a boolean
    return (Size == sizeof(BOOLEAN));
  } else if (Type == EfiConfigurationTypeUnsigned) {
    // Must be the size of an unsigned
    return (Size == sizeof(UINT64));
  } else if (Type == EfiConfigurationTypeInteger) {
    // Must be the size of an integer
    return (Size == sizeof(INT64));
  } else if (Type == EfiConfigurationTypeFloat) {
    // Must be the size of a float
    return (Size == sizeof(FLOAT64));
  }
  // List, array, or unknown data type - not valid
  return FALSE;
}
// ConfigurationSet
/// Set a configuration value by key
/// @param This  The configuration protocol interface
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @param Type  The configuration key value type
/// @retval EFI_INVALID_PARAMETER If Key, or Value is NULL or Size is zero
/// @retval EFI_SUCCESS           The value  of the key was set successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationSet (
  IN EFI_CONFIGURATION_PROTOCOL_IMPL *This,
  IN CONST CHAR16                    *Key,
  IN CONST VOID                      *Value,
  IN UINTN                            Size,
  IN EFI_CONFIGURATION_TYPE           Type
) {
  EFI_STATUS             Status;
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check type and size are valid
  if (!ConfigurationCheckType(Type, Size)) {
    return EFI_INVALID_PARAMETER;
  }
  // Create the root key if needed
  Status = ConfigurationFind(&(This->Root), NULL, ConfigurationActionCreate);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Protect the root key
  Root = This->Root;
  // Find or create the key
  Status = ConfigurationFind(&Root, Key, ConfigurationActionCreate);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Finish the key if the different type
  if (Root->Type != Type) {
    ConfigurationFinishKey(Root, TRUE);
  }
  // Set key value
  Root->Type = Type;
  Root->Size = Size;
  Root->Value = EfiDuplicate(Size, Value);
  return EFI_SUCCESS;
}
// ConfigurationRemove
/// Remove key by path identifier
/// @param This The configuration protocol interface
/// @param Key  The key path identifier to remove
/// @retval EFI_INVALID_PARAMETER If This or Key is NULL
/// @retval EFI_SUCESS            The key was removed successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationRemove (
  IN EFI_CONFIGURATION_PROTOCOL_IMPL *This,
  IN CONST CHAR16                    *Key
) {
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return FALSE;
  }
  // Protect the root key
  Root = This->Root;
  return ConfigurationFind(&Root, Key, ConfigurationActionRemove);
}
// ConfigurationEnumerateKey
/// Enumerate key
/// @param This            The configuration key
/// @param Key             The full key path identifier
/// @param TypeFilter      The bitmask of the types of keys to enumerate or zero for all
/// @param Callback        The enumeration callback
/// @param CallbackContext The context passed to each invocation of the enumeration callback
/// @param Recursive       Whether to recursively enumerate child keys
/// @retval EFI_INVALID_PARAMETER If This, Key, or Callback is NULL
/// @retval EFI_OUT_OF_RESOURCES  Could not enumerate configuration keys because there are not enough resources
/// @retval EFI_NOT_FOUND         The key was not found
/// @retval EFI_SUCCESS           The specified configuration key was enumerated successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationEnumerateKey (
  IN EFI_CONFIGURATION_KEY      *This,
  IN CONST CHAR16               *Key,
  IN UINTN                       TypeFilter OPTIONAL,
  IN EFI_CONFIGURATION_CALLBACK  Callback,
  IN VOID                       *CallbackContext OPTIONAL,
  IN BOOLEAN                     Recursive
) {
  EFI_STATUS Status = EFI_SUCCESS;
  // Check parameters
  if ((This == NULL) || (Key == NULL) || (Callback == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Callback if this key is empty
  if ((This->Name != NULL) || ((This->Size != 0) && (This->Value != NULL))) {
    // Callback for each key
    if (!Callback(Key, (This->Name == NULL) ? L"" : This->Name, This->Type, This->Size, This->Value, CallbackContext)) {
      Status = EFI_ABORTED;
    }
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check if recursive
  if (Recursive) {
    // Check if key type is list or array
    if (This->Type == EfiConfigurationTypeList) {
      // Traverse list child keys
      EFI_CONFIGURATION_KEY *List = (EFI_CONFIGURATION_KEY *)(This->Value);
      while (List != NULL) {
        // Get child key path identifier
        CHAR16 *ChildKey = EfiPoolPrint(L"%s/%s", Key, List->Name);
        if (ChildKey == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        // Enumerate child keys
        Status = ConfigurationEnumerateKey(List, ChildKey, TypeFilter, Callback, CallbackContext, Recursive);
        EfiFreePool(ChildKey);
        if (EFI_ERROR(Status)) {
          return Status;
        }
        // Get next key in list
        List = List->Next;
      }
    } else if (This->Type == EfiConfigurationTypeArray) {
      // Traverse array child keys
      UINTN                  Index = 0;
      EFI_CONFIGURATION_KEY *List = (EFI_CONFIGURATION_KEY *)(This->Value);
      while (List != NULL) {
        // Get child key path identifier
        CHAR16 *ChildKey = EfiPoolPrint(L"%s/%u", Key, Index++);
        if (ChildKey == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        // Enumerate child keys
        Status = ConfigurationEnumerateKey(List, ChildKey, TypeFilter, Callback, CallbackContext, Recursive);
        EfiFreePool(ChildKey);
        if (EFI_ERROR(Status)) {
          return Status;
        }
        // Get next key in list
        List = List->Next;
      }
    }
  }
  return Status;
}
// ConfigurationEnumerate
/// Enumerate keys
/// @param This            The configuration protocol interface
/// @param Key             The key path identifier to enumerate or NULL for root
/// @param TypeFilter      The bitmask of the types of keys to enumerate or zero for all
/// @param Callback        The enumeration callback
/// @param CallbackContext The context passed to each invocation of the enumeration callback
/// @param Recursive       Whether to recursively enumerate child keys
/// @retval EFI_INVALID_PARAMETER If This or Callback is NULL
/// @retval EFI_NOT_FOUND         The key was not found
/// @retval EFI_ABORTED           The callback returned FALSE indicating quit enumerating
/// @retval EFI_SUCCESS           The specified configuration key was enumerated successfully
STATIC
EFI_STATUS
EFIAPI
ConfigurationEnumerate (
  IN EFI_CONFIGURATION_PROTOCOL_IMPL *This,
  IN CONST CHAR16                    *Key OPTIONAL,
  IN UINTN                            TypeFilter OPTIONAL,
  IN EFI_CONFIGURATION_CALLBACK       Callback,
  IN VOID                            *CallbackContext OPTIONAL,
  IN BOOLEAN                          Recursive
) {
  EFI_STATUS             Status;
  EFI_CONFIGURATION_KEY *Root;
  // Check this is correct protocol implementation
  if ((This == NULL) || (This->Signature != EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find the key
  Root = This->Root;
  Status = ConfigurationFind(&Root, Key, ConfigurationActionFind);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Root == NULL) {
    return EFI_NOT_FOUND;
  }
  // Enumerate the keys
  return ConfigurationEnumerateKey(Root, (Key != NULL) ? Key : L"", TypeFilter, Callback, CallbackContext, Recursive);
}

// ConfigurationGetProtocol
/// Get the current configuration protocol
/// @return The current configuration protocol interface
STATIC
EFI_CONFIGURATION_PROTOCOL *
EFIAPI
ConfigurationGetProtocol (
  VOID
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = NULL;
  // Check if there is already a logging protocol
  if (EFI_ERROR(EfiLocateProtocol(&gEfiConfigurationProtocolGuid, NULL, (VOID **)&Configuration))) {
    return NULL;
  }
  // Return the configuration protocol interface
  return Configuration;
}

// EfiConfigurationInstall
/// Install the configuration protocol
/// @retval EFI_SUCCESS The configuration protocol was installed
EFI_STATUS
EFIAPI
EfiConfigurationInstall (
  VOID
) {
  EFI_STATUS                       Status;
  EFI_CONFIGURATION_PROTOCOL_IMPL *Impl;
  // Check if there is already a configuration protocol
  EFI_CONFIGURATION_PROTOCOL      *Configuration;
  Configuration = ConfigurationGetProtocol();
  if (Configuration != NULL) {
    return EFI_SUCCESS;
  }
  // Allocate the configuration protocol implementation interface
  Impl = EfiAllocateByType(EFI_CONFIGURATION_PROTOCOL_IMPL);
  if (Impl == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Setup the configuration protocol interface
  Impl->Protocol.Exists = (EFI_CONFIGURATION_EXISTS)ConfigurationExists;
  Impl->Protocol.GetChildren = (EFI_CONFIGURATION_GET_CHILDREN)ConfigurationGetChildren;
  Impl->Protocol.Get = (EFI_CONFIGURATION_GET)ConfigurationGet;
  Impl->Protocol.Set = (EFI_CONFIGURATION_SET)ConfigurationSet;
  Impl->Protocol.Remove = (EFI_CONFIGURATION_REMOVE)ConfigurationRemove;
  Impl->Protocol.Enumerate = (EFI_CONFIGURATION_ENUMERATE)ConfigurationEnumerate;
  // Setup the configuration protocol implementation interface
  Impl->Signature = EFI_CONFIGURATION_PROTOCOL_IMPL_SIGNATURE;
  Impl->Root = NULL;
  // Create the root key if needed
  Status = ConfigurationFind(&(Impl->Root), NULL, ConfigurationActionCreate);
  if (!EFI_ERROR(Status)) {
    // Install the configuration protocol
    Status = EfiInstallMultipleProtocolInterfaces(&gEfiImageHandle, &gEfiConfigurationProtocolGuid, Impl, NULL);
  }
  if (EFI_ERROR(Status)) {
    // Free the configuration protocol interface since there was an error
    EfiFreePool(Impl);
  }
  // Return the status code
  return Status;
}
// EfiConfigurationUninstall
/// Install the configuration protocol
/// @retval EFI_SUCCESS The configuration protocol was installed
EFI_STATUS
EFIAPI
EfiConfigurationUninstall (
  VOID
) {
  EFI_STATUS Status;
  // Get protocol
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  // Check protocol is installed
  if (Configuration == NULL) {
    return EFI_NOT_FOUND;
  }
  // Uninstall the protocol
  Status = EfiUninstallProtocolInterface(gEfiImageHandle, &gEfiConfigurationProtocolGuid, (VOID *)Configuration);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Clear the configuration
  if (Configuration->Remove != NULL) {
    Configuration->Remove(Configuration, NULL);
  }
  // Free the configuration protocol
  return EfiFreePool((VOID *)Configuration);
}

// EfiConfigurationExists
/// Check key exists
/// @param Key The key path identifier
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_NOT_FOUND         If the key does not exist
/// @retval EFI_SUCCESS           The key exists
EFI_STATUS
EFIAPI
EfiConfigurationExists (
  IN CONST CHAR16 *Key
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->Exists == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  return Configuration->Exists(Configuration, Key);
}

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
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->GetChildren == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return Configuration->GetChildren(Configuration, Key, Full, Count, Children);
}

// EfiConfigurationGet
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @param Type  On output, the type of the value
/// @retval EFI_INVALID_PARAMETER If Key or Value is NULL
/// @retval EFI_SUCCESS           The value, size, and type of the key were returned successfully
EFI_STATUS
EFIAPI
EfiConfigurationGet (
  IN     CONST CHAR16            *Key,
  IN     CONST VOID             **Value,
  IN OUT UINTN                   *Size OPTIONAL,
  OUT    EFI_CONFIGURATION_TYPE  *Type OPTIONAL
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->Get == NULL)) {
    return NULL;
  }
  return Configuration->Get(Configuration, Key, Value, Size, Type);
}
// EfiConfigurationGetData
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  On input, the size of the default value, on output, the size in bytes of the value
/// @retval EFI_INVALID_PARAMETER If Key or Value is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
CONST VOID *
EFIAPI
EfiConfigurationGetData (
  IN     CONST CHAR16 *Key,
  IN     CONST VOID   *Value OPTIONAL,
  IN OUT UINTN        *Size OPTIONAL
) {
  CONST VOID             *Data = NULL;
  EFI_CONFIGURATION_TYPE  Type = 0;
  UINTN                   ValueSize = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, &Data, &ValueSize, &Type))) {
    return Value;
  }
  // Check to make sure the value is actually a string
  if ((Data == NULL) || (ValueSize == 0) || (Type != EfiConfigurationTypeData)) {
    // Return the default value
    return Value;
  }
  // Return the key value
  if (Size != NULL) {
    *Size = ValueSize;
  }
  return Data;
}
// EfiConfigurationGetDate
/// Set a configuration value by key
/// @param Key  The key path identifier
/// @param Date The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Date is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
CONST EFI_TIME *
EFIAPI
EfiConfigurationGetDate (
  IN CONST CHAR16   *Key,
  IN CONST EFI_TIME *Date
) {
  CONST EFI_TIME         *Time = NULL;
  EFI_CONFIGURATION_TYPE  Type = 0;
  UINTN                   ValueSize = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, (VOID **)&Time, &ValueSize, &Type))) {
    return Date;
  }
  // Check to make sure the value is actually a string
  if ((Time == NULL) || (ValueSize != sizeof(EFI_TIME)) || (Type != EfiConfigurationTypeDate)) {
    // Return the default value
    return Date;
  }
  // Return the key value
  return Time;
}
// EfiConfigurationGetString
/// Get a configuration value by key
/// @param Key    The key path identifier
/// @param String The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key or String is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
CONST CHAR16 *
EFIAPI
EfiConfigurationGetString (
  IN CONST CHAR16 *Key,
  IN CONST CHAR16 *String
) {
  CONST CHAR16           *Value = NULL;
  UINTN                   Size = StrSize(String);
  EFI_CONFIGURATION_TYPE  Type = 0;
  EFI_STATUS Status = EfiConfigurationGet(Key, (VOID **)&Value, &Size, &Type);
  // Get the key value
  if (EFI_ERROR(Status)) {
    return String;
  }
  // Check to make sure the value is actually a string
  if ((Value == NULL) || (Size <= sizeof(CHAR16)) || (Type != EfiConfigurationTypeString)) {
    // Return the default value
    return String;
  }
  // Return the key value
  return Value;
}
// EfiConfigurationGetBoolean
/// Get a configuration value by key
/// @param Key     The key path identifier
/// @param Boolean The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
BOOLEAN
EFIAPI
EfiConfigurationGetBoolean (
  IN CONST CHAR16 *Key,
  IN BOOLEAN       Boolean
) {
  BOOLEAN                *Value = NULL;
  UINTN                   Size = sizeof(BOOLEAN);
  EFI_CONFIGURATION_TYPE  Type = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, (VOID **)&Value, &Size, &Type))) {
    return Boolean;
  }
  // Check to make sure the value is actually a boolean
  if ((Value == NULL) || (Size != sizeof(BOOLEAN)) || (Type != EfiConfigurationTypeBoolean)) {
    // Return the default value
    return Boolean;
  }
  // Return the key value
  return *Value;
}
// EfiConfigurationGetUnsigned
/// Get a configuration value by key
/// @param Key      The key path identifier
/// @param Unsigned The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
UINT64
EFIAPI
EfiConfigurationGetUnsigned (
  IN CONST CHAR16 *Key,
  IN UINT64        Unsigned
) {
  UINT64                 *Value = NULL;
  UINTN                   Size = sizeof(UINT64);
  EFI_CONFIGURATION_TYPE  Type = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, (VOID *)&Value, &Size, &Type))) {
    return Unsigned;
  }
  // Check to make sure the value is actually an unsigned
  if ((Value == NULL) || (Size != sizeof(UINT64)) || ((Type != EfiConfigurationTypeUnsigned) && (Type != EfiConfigurationTypeInteger))) {
    // Return the default value
    return Unsigned;
  }
  // Return the key value
  return *Value;
}
// EfiConfigurationGetInteger
/// Get a configuration value by key
/// @param Key     The key path identifier
/// @param Integer The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
INT64
EFIAPI
EfiConfigurationGetInteger (
  IN CONST CHAR16 *Key,
  IN INT64         Integer
) {
  INT64                  *Value = NULL;
  UINTN                   Size = sizeof(INT64);
  EFI_CONFIGURATION_TYPE  Type = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, (VOID **)&Value, &Size, &Type))) {
    return Integer;
  }
  // Check to make sure the value is actually an integer
  if ((Value == NULL) || (Size != sizeof(INT64)) || ((Type != EfiConfigurationTypeUnsigned) && (Type != EfiConfigurationTypeInteger))) {
    // Return the default value
    return Integer;
  }
  // Return the key value
  return *Value;
}

// EfiConfigurationGetFloat
/// Get a configuration value by key
/// @param Key   The key path identifier
/// @param Float The default value to return in the key does not exist
/// @retval EFI_INVALID_PARAMETER If Key is NULL
/// @retval EFI_SUCCESS           The value of the key was returned successfully
FLOAT64
EFIAPI
EfiConfigurationGetFloat (
  IN CONST CHAR16 *Key,
  IN FLOAT64       Float
) {
  FLOAT64                *Value = NULL;
  UINTN                   Size = sizeof(INT64);
  EFI_CONFIGURATION_TYPE  Type = 0;
  // Get the key value
  if (EFI_ERROR(EfiConfigurationGet(Key, (VOID **)&Value, &Size, &Type))) {
    return Float;
  }
  // Check to make sure the value is actually an integer
  if ((Value == NULL) || (Size != sizeof(INT64)) || (Type != EfiConfigurationTypeFloat)) {
    // Return the default value
    return Float;
  }
  // Return the key value
  return *Value;
}

// EfiConfigurationSet
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @param Type  The configuration key value type
/// @retval EFI_INVALID_PARAMETER If Key, or Value is NULL or Size is zero
/// @retval EFI_SUCCESS           The value  of the key was set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSet (
  IN CONST CHAR16           *Key,
  IN CONST VOID             *Value,
  IN UINTN                   Size,
  IN EFI_CONFIGURATION_TYPE  Type
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->Set == NULL)) {
    return FALSE;
  }
  return Configuration->Set(Configuration, Key, Value, Size, Type);
}
// EfiConfigurationSetData
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Value The default value to return if the key does not exist
/// @param Size  The size in bytes of the value
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetData (
  IN CONST CHAR16 *Key,
  IN CONST VOID   *Value,
  IN UINTN         Size
) {
  return EfiConfigurationSet(Key, Value, Size, EfiConfigurationTypeData);
}
// EfiConfigurationSetDate
/// Set a configuration value by key
/// @param Key  The key path identifier
/// @param Time The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetDate (
  IN CONST CHAR16 *Key,
  IN EFI_TIME     *Time
) {
  return EfiConfigurationSet(Key, Time, sizeof(EFI_TIME), EfiConfigurationTypeDate);
}
// EfiConfigurationSetString
/// Set a configuration value by key
/// @param Key    The key path identifier
/// @param String The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetString (
  IN CONST CHAR16 *Key,
  IN CONST CHAR16 *String
) {
  return EfiConfigurationSet(Key, String, StrSize(String), EfiConfigurationTypeString);
}
// EfiConfigurationSetBoolean
/// Set a configuration value by key
/// @param Key     The key path identifier
/// @param Boolean The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetBoolean (
  IN CONST CHAR16 *Key,
  IN BOOLEAN       Boolean
) {
  return EfiConfigurationSet(Key, &Boolean, sizeof(BOOLEAN), EfiConfigurationTypeBoolean);
}
// EfiConfigurationSetUnsigned
/// Set a configuration value by key
/// @param Key      The key path identifier
/// @param Unsigned The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetUnsigned (
  IN CONST CHAR16 *Key,
  IN UINT64        Unsigned
) {
  return EfiConfigurationSet(Key, &Unsigned, sizeof(UINT64), EfiConfigurationTypeUnsigned);
}
// EfiConfigurationSetInteger
/// Set a configuration value by key
/// @param Key     The key path identifier
/// @param Integer The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetInteger (
  IN CONST CHAR16 *Key,
  IN INT64         Integer
) {
  return EfiConfigurationSet(Key, &Integer, sizeof(INT64), EfiConfigurationTypeInteger);
}
// EfiConfigurationSetFloat
/// Set a configuration value by key
/// @param Key   The key path identifier
/// @param Float The value to set for the key
/// @retval EFI_INVALID_PARAMETER If Key or Time is NULL
/// @retval EFI_SUCCESS           The value of the key wwas set successfully
EFI_STATUS
EFIAPI
EfiConfigurationSetFloat (
  IN CONST CHAR16 *Key,
  IN FLOAT64       Float
) {
  return EfiConfigurationSet(Key, &Float, sizeof(FLOAT64), EfiConfigurationTypeFloat);
}

// EfiConfigurationRemove
/// Remove key by path identifier
/// @param Key The key path identifier to remove
/// @retval EFI_INVALID_PARAMETER If This or Key is NULL
/// @retval EFI_SUCESS            The key was removed successfully
EFI_STATUS
EFIAPI
EfiConfigurationRemove (
  IN CONST CHAR16 *Key
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->Remove == NULL)) {
    return FALSE;
  }
  return Configuration->Remove(Configuration, Key);
}
// EfiConfigurationClear
/// Remove all keys from the configuration
/// @retval EFI_SUCESS The configuration was cleared successfully
EFI_STATUS
EFIAPI
EfiConfigurationClear (
  VOID
) {
  return EfiConfigurationRemove(NULL);
}

// EfiConfigurationEnumerate
/// Enumerate keys
/// @param Key             The key path identifier to enumerate or NULL for root
/// @param TypeFilter      The bitmask of the types of keys to enumerate or zero for all
/// @param Callback        The enumeration callback
/// @param CallbackContext The context passed to each invocation of the enumeration callback
/// @param Recursive       Whether to recursively enumerate child keys
/// @retval EFI_INVALID_PARAMETER If This or Callback is NULL
/// @retval EFI_NOT_FOUND         If the key was not found
/// @retval EFI_ABORTED           If the callback returned FALSE indicating quit enumerating
/// @retval EFI_SUCCESS           If the specified configuration key was enumerated successfully
EFI_STATUS
EFIAPI
EfiConfigurationEnumerate (
  IN CONST CHAR16               *Key OPTIONAL,
  IN UINTN                       TypeFilter OPTIONAL,
  IN EFI_CONFIGURATION_CALLBACK  Callback,
  IN VOID                       *CallbackContext OPTIONAL,
  IN BOOLEAN                     Recursive
) {
  EFI_CONFIGURATION_PROTOCOL *Configuration = ConfigurationGetProtocol();
  if ((Configuration == NULL) || (Configuration->Enumerate == NULL)) {
    return EFI_UNSUPPORTED;
  }
  return Configuration->Enumerate(Configuration, Key, TypeFilter, Callback, CallbackContext, Recursive);
}
