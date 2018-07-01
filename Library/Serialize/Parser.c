///
/// @file Library/Serialize/Parser.c
///
/// GUI language parser implementation
///

#include <Serialize/Parser.h>

// LANG_PREVIOUS_STATE_STACK_SIZE
/// The default size for the previous state stack
#define LANG_PREVIOUS_STATE_STACK_SIZE 8

// LANG_MESSAGE_FORMAT
/// The default language message format specifier
#define LANG_MESSAGE_FORMAT L"%s(%u,%u): %s%s"

// LANG_MESSAGE_ERROR
/// Language error message
#define LANG_MESSAGE_ERROR EFI_BIT(0)
// LANG_MESSAGE_WARNING
/// Language warning message
#define LANG_MESSAGE_WARNING EFI_BIT(1)

// LANG_DEBUG
/// Whether to debug language parsing, the following values are valid:
/// 0 - No debug logging or release build
/// 1 - Debug logging
/// 2 - Verbose and debug logging
/// 3 - Extra verbose, verbose, and debug logging
#if !defined(LANG_DEBUG)
# define LANG_DEBUG 0
#endif

// LANG_LOG
/// Debug logging for language parsing
#if defined(EFI_DEBUG) && defined(LANG_DEBUG) && (LANG_DEBUG >= 1)
# define LANG_LOG(...) LOG(__VA_ARGS__)
#else
# define LANG_LOG(...)
#endif
// LANG_VERBOSE
/// Verbose logging for language parsing
#if defined(EFI_DEBUG) && defined(LANG_DEBUG) && (LANG_DEBUG >= 2)
# define LANG_VERBOSE(...) VERBOSE(__VA_ARGS__)
#else
# define LANG_VERBOSE(...)
#endif
// LANG_WHISPER
/// Extra verbose logging for language parsing
#if defined(EFI_DEBUG) && defined(LANG_DEBUG) && (LANG_DEBUG >= 3)
# define LANG_WHISPER(...) WHISPER(__VA_ARGS__)
#else
# define LANG_WHISPER(...)
#endif

// LANG_MESSAGE
/// Language message
typedef struct LANG_MESSAGE LANG_MESSAGE;
struct LANG_MESSAGE {

  // Next
  /// The next message
  LANG_MESSAGE *Next;
  // Flags
  /// The message flags
  UINTN         Flags;
  // LineNumber
  /// The line number of the parsed source where the message was generated
  UINTN         LineNumber;
  // LineOffset
  /// The offset of the line of the parsed source where the message was generated
  UINTN         LineOffset;
  // Source
  /// The source being parsed when the message occured
  CHAR16       *Source;
  // Message
  /// The message
  CHAR16       *Message;

};
// LANG_RULE
/// Language state rule
struct LANG_RULE {

  // Next
  /// The next language rule
  LANG_RULE     *Next;
  // Callback
  /// Token parsed callback
  LANG_CALLBACK  Callback;
  // Options
  /// The options of this language state rule
  UINTN          Options;
  // NextState
  /// The identifier of the language state to which to change when this rule is satisfied
  UINTN          NextState;
  // PushState
  /// If Options has LANG_RULE_PUSH, the identifier of the language state to push or LANG_STATE_CURRENT to push the current language state
  UINTN          PushState;
  // TokenCount
  /// The count of characters in the rule token string
  UINTN          TokenCount;
  // Token
  /// The token string
  CHAR16        *Token;

};
// LANG_STATE
/// Language state
struct LANG_STATE {

  // Next
  /// The next language state
  LANG_STATE    *Next;
  // Callback
  /// Token parsed callback
  LANG_CALLBACK  Callback;
  // Id
  /// The language state identifier
  UINTN          Id;
  // Rules
  /// The language state rules list
  LANG_RULE     *Rules;

};
// LANG_STATE_STACK
/// Language state stack
typedef struct LANG_STATE_STACK LANG_STATE_STACK;
struct LANG_STATE_STACK {

  // Previous
  /// The previous state on the stack or NULL if this is the only state on the stack
  LANG_STATE_STACK *Previous;
  // State
  /// The language state
  LANG_STATE       *State;
  // SaveState
  /// The language state when the state was pushed
  LANG_STATE       *SaveState;

};
// LANG_PARSER
/// Language parser
struct LANG_PARSER {

  // DecodeCount
  /// Decoded character expected remaining character count
  UINT32                  DecodeCount;
  // DecodedCharacter
  /// Decoded character
  UINT32                  DecodedCharacter;
  // LineNumber
  /// The current line number of parsed characters
  UINTN                   LineNumber;
  // LineOffset
  /// The current offset of the current line of parsed characters
  UINTN                   LineOffset;
  // TokenCount
  /// The current parsed token count of characters
  UINTN                   TokenCount;
  // TokenSize
  /// The current parsed token maximum count of characters, including null-terminator
  UINTN                   TokenSize;
  // Token
  /// The current parsed token
  CHAR16                 *Token;
  // Source
  /// The source being parsed
  CHAR16                 *Source;
  // Callback
  /// Token parsed callback
  LANG_CALLBACK           Callback;
  // MessagesCallback
  /// The parser messages callback
  LANG_MESSAGE_CALLBACK   MessageCallback;
  // Messages
  /// The parser messages list
  LANG_MESSAGE           *Messages;
  // States
  /// The parser states list
  LANG_STATE             *States;
  // State
  /// The current parser state
  LANG_STATE             *State;
  // PreviousStates
  /// The previous parser states
  LANG_STATE_STACK       *PreviousStates;

};

// LogParseMessages
/// Log the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @return Whether the parser messafes were logged or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EFI_STATUS
EFIAPI
LogParseMessages (
  IN LANG_PARSER   *Parser,
  IN CHAR16        *Format OPTIONAL,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Args;
  VA_START(Args, Format);
  Status = LogParseVMessages(Parser, Format, Args);
  VA_END(Args);
  return Status;
}
// LogParseVMessages
/// Log the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @param Args     The arguments
/// @return Whether the parser messafes were logged or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EFI_STATUS
EFIAPI
LogParseVMessages (
  IN LANG_PARSER   *Parser,
  IN CHAR16        *Format OPTIONAL,
  IN VA_LIST        Args
) {
  EFI_STATUS   Status;
  UINTN        Count;
  UINTN        Index;
  CHAR16     **Messages;
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the parser messages
  Count = 0;
  Messages = NULL;
  Status = ParseVMessages(Parser, &Count, &Messages, Format, Args);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Check there are messages
  if ((Count != 0) && (Messages == NULL)) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Log the messages
  for (Index = 0; Index < Count; ++Index) {
    LOG(L"%s\n", Messages[Index]);
  }
  // Free the messages list
  return StrListFree(Messages, Count);
}

// ParseMessages
/// Get the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @param Count    On output, the count of parser messages
/// @param Messages On output, a null-terminated array of parser message strings, which must be freed with StrListFree
/// @return Whether the parser messafes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser, Count, or Messages is NULL or *Message is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EFI_STATUS
EFIAPI
ParseMessages (
  IN  LANG_PARSER   *Parser,
  OUT UINTN         *Count,
  OUT CHAR16      ***Messages,
  IN  CHAR16        *Format OPTIONAL,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Args;
  VA_START(Args, Format);
  Status = ParseVMessages(Parser, Count, Messages, Format, Args);
  VA_END(Args);
  return Status;
}
// ParseVMessages
/// Get the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @param Count    On output, the count of parser messages
/// @param Messages On output, a null-terminated array of parser message strings, which must be freed with StrListFree
/// @param Args     The arguments
/// @return Whether the parser messafes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser, Count, or Messages is NULL or *Message is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EFI_STATUS
EFIAPI
ParseVMessages (
  IN  LANG_PARSER   *Parser,
  OUT UINTN         *Count,
  OUT CHAR16      ***Messages,
  IN  CHAR16        *Format OPTIONAL,
  IN  VA_LIST        Args
) {
  EFI_STATUS     Status;
  LANG_MESSAGE  *Message;
  CHAR16       **MsgList = NULL;
  UINTN          MsgCount = 0;
  // Check parameters
  if ((Parser == NULL) || (Count == NULL) || (Messages == NULL) || (*Messages != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Traverse the messages
  for (Message = Parser->Messages; Message != NULL; Message = Message->Next) {
    CHAR16 *Text;
    CHAR16 *Type = L"";
    // Get the type of message
    if (EFI_BITS_ANY_SET(Message->Flags, LANG_MESSAGE_ERROR)) {
      Type = L"Error: ";
    } else if (EFI_BITS_ANY_SET(Message->Flags, LANG_MESSAGE_WARNING)) {
      Type = L"Warning: ";
    }
    // Format each parser message
    if (Format == NULL) {
      Text = EfiPoolPrint(LANG_MESSAGE_FORMAT, Message->Source, Message->LineNumber, Message->LineOffset, Type, Message->Message);
    } else {
      Text = EfiPoolVPrint(Format, Args);
    }
    if (Text != NULL) {
      // Add message to messages list
      Status = StrList(&MsgList, &MsgCount, Text, 0, 0);
      EfiFreePool(Text);
    } else {
      // Allocation error
      Status = EFI_OUT_OF_RESOURCES;
    }
    // Check for success or free message list and return status
    if (EFI_ERROR(Status)) {
      StrListFree(MsgList, MsgCount);
      return Status;
    }
  }
  // Return the messages and count
  *Messages = MsgList;
  *Count = MsgCount;
  return EFI_SUCCESS;
}

// FreeParseMessage
/// Free a parser message
/// @param Message The parser message to free
/// @retval EFI_INVALID_PARAMETER If Message is NULL
/// @retval EFI_SUCCESS           If the parser message was freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseMessage (
  IN LANG_MESSAGE *Message
) {
  // Check parameters
  if (Message == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free the message source
  if (Message->Source != NULL) {
    EfiFreePool(Message->Source);
    Message->Source = NULL;
  }
  // Free the message text
  if (Message->Message != NULL) {
    EfiFreePool(Message->Message);
    Message->Message = NULL;
  }
  // Free the message
  EfiFreePool(Message);
  return EFI_SUCCESS;
}
// FreeParseMessages
/// Free a parser messages
/// @param Messages The parser messages to free
/// @retval EFI_INVALID_PARAMETER If Messages is NULL
/// @retval EFI_SUCCESS           If the parser messages were freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseMessages (
  IN LANG_MESSAGE *Messages
) {
  // Check parameters
  if (Messages == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  while (Messages != NULL) {
    LANG_MESSAGE *Next = Messages->Next;
    FreeParseMessage(Messages);
    Messages = Next;
  }
  return EFI_SUCCESS;
}

// ParseMessage
/// Add a parser message
/// @param Parser      The language parser to which to add a message
/// @param Flags       The message flags
/// @param MessageDesc The description of the message
/// @return Whether the message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or MessageDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the message was added successfully
EFI_STATUS
EFIAPI
ParseMessage (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Flags,
  IN     CHAR16      *MessageDesc,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Args;
  VA_START(Args, MessageDesc);
  Status = ParseVMessage(Parser, Flags, MessageDesc, Args);
  VA_END(Args);
  return Status;
}
// ParseVMessage
/// Add a parser message
/// @param Parser      The language parser to which to add a message
/// @param Flags       The message flags
/// @param MessageDesc The description of the message
/// @param Args        The arguments
/// @return Whether the message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or MessageDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the message was added successfully
EFI_STATUS
EFIAPI
ParseVMessage (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Flags,
  IN     CHAR16      *MessageDesc,
  IN     VA_LIST      Args
) {
  LANG_MESSAGE *Message;
  // Check parameters
  if ((Parser == NULL) || (MessageDesc == NULL) || (*MessageDesc == L'\0')) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate a new message
  Message = EfiAllocateByType(LANG_MESSAGE);
  if (Message == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set message
  Message->Next = NULL;
  Message->Source = StrDup(Parser->Source);
  Message->Flags = Flags;
  Message->LineNumber = Parser->LineNumber;
  Message->LineOffset = Parser->LineOffset;
  Message->Message = EfiPoolVPrint(MessageDesc, Args);
  if (Message->Message == NULL) {
    EfiFreePool(Message);
    return EFI_OUT_OF_RESOURCES;
  }
  // Add the message
  if (Parser->Messages == NULL) {
    Parser->Messages = Message;
  } else {
    LANG_MESSAGE *Ptr = Parser->Messages;
    while (Ptr->Next != NULL) {
      Ptr = Ptr->Next;
    }
    Ptr->Next = Message;
  }
  // Log the message if neccessary
  if (Parser->MessageCallback != NULL) {
    Parser->MessageCallback(Parser, Message->Message);
  }
  return EFI_SUCCESS;
}

// ParseError
/// Add a parser error
/// @param Parser    The language parser to which to add an error
/// @param ErrorDesc The description of the error
/// @return Whether the error message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the error message was added successfully
EFI_STATUS
EFIAPI
ParseError (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *ErrorDesc,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Args;
  VA_START(Args, ErrorDesc);
  Status = ParseVError(Parser, ErrorDesc, Args);
  VA_END(Args);
  return Status;
}
// ParseVError
/// Add a parser error
/// @param Parser    The language parser to which to add an error
/// @param ErrorDesc The description of the error
/// @param Args      The arguments
/// @return Whether the error message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the error message was added successfully
EFI_STATUS
EFIAPI
ParseVError (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *ErrorDesc,
  IN     VA_LIST      Args
) {
  return ParseVMessage(Parser, LANG_MESSAGE_ERROR, ErrorDesc, Args);
}

// ParseWarn
/// Add a parser warning
/// @param Parser   The language parser to which to add a warning
/// @param WarnDesc The description of the warning
/// @return Whether the warning message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the warning message was added successfully
EFI_STATUS
EFIAPI
ParseWarn (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *WarnDesc,
  ...
) {
  EFI_STATUS Status;
  VA_LIST    Args;
  VA_START(Args, WarnDesc);
  Status = ParseVWarn(Parser, WarnDesc, Args);
  VA_END(Args);
  return Status;
}
// ParseVWarn
/// Add a parser warning
/// @param Parser   The language parser to which to add a warning
/// @param WarnDesc The description of the warning
/// @param Args     The arguments
/// @return Whether the warning message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the warning message was added successfully
EFI_STATUS
EFIAPI
ParseVWarn (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *WarnDesc,
  IN     VA_LIST      Args
) {
  return ParseVMessage(Parser, LANG_MESSAGE_WARNING, WarnDesc, Args);
}

// FindParseState
/// Find parser state by identifier
/// @param Parser  The language parser
/// @param StateId The identifier of the parser state to find
/// @return The parser state or NULL if there was an error or the parser state was not found
STATIC
LANG_STATE *
EFIAPI
FindParseState (
  IN LANG_PARSER *Parser,
  IN UINTN        StateId
) {
  LANG_STATE *State;
  // Check parameters
  if ((Parser == NULL) || (Parser->States == NULL)) {
    return NULL;
  }
  // Check if looking for current state
  if (StateId == LANG_STATE_CURRENT) {
    // Get current state
    State = Parser->State;
  } else {
    // Find state by identiifer
    for (State = Parser->States; State != NULL; State = State->Next) {
      // Check if identifier matches
      if (State->Id == StateId) {
        // Found state by identifier
        break;
      }
    }
  }
  // Return the state or NULL if not found
  return State;
}
// SetNextParseState
/// Set the next parse state
/// @param Parser  The language parser
/// @param StateId The identifier of the parser state to set
/// @param PushId  The identifier of the parser state to push or LANG_STATE_CURRENT to only set the state without pushing
/// @param Options The options for setting the next parse state
/// @retval EFI_OUT_OF_RESOURCES There was not enough memory to push the state
/// @retval EFI_NOT_FOUND        The parser state with the specified identifier was not found
/// @retval EFI_WARN_STALE_DATA  The parser state was popped but expects to be popped again, SetNextParseState should be called again after parse callbacks
/// @retval EFI_SUCCESS          The parser state was set successfully
STATIC
EFI_STATUS
EFIAPI
SetNextParseState (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        StateId,
  IN     UINTN        PushId,
  IN     UINTN        Options
) {
  EFI_STATUS        Status = EFI_SUCCESS;
  LANG_STATE_STACK *Stack;
  LANG_STATE       *State;
  LANG_STATE       *Previous;
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Get the current state to push later if needed
  State = Previous = Parser->State;
  // Check if popping previous state
  if (EFI_BITS_ANY_SET(Options, LANG_RULE_POP | LANG_RULE_POP_BEFORE | LANG_RULE_POP_AFTER)) {
    if (Parser->PreviousStates == NULL) {
      // Stack underflow
      ParseError(Parser, L"Unexpected parser state stack underflow");
      return EFI_NOT_READY;
    }
    // Pop the previous state
    Stack = Parser->PreviousStates;
    Parser->PreviousStates = Stack->Previous;
    // When safe popping, only set the popped state if the
    // previous set state id is the same as the current state id
    if (EFI_BITS_ARE_UNSET(Options, LANG_RULE_POP_SAFE) ||
        (Stack->SaveState == NULL) ||
        (Stack->SaveState->Id == State->Id)) {
      Parser->State = Stack->State;
    }
    EfiFreePool(Stack);
    // Check parser state is valid
    if (Parser->State == NULL) {
      // Stack underflow
      ParseError(Parser, L"Unexpected empty state on stack");
      Status = EFI_NOT_READY;
    }
  } else if ((StateId != LANG_STATE_CURRENT) && (StateId != Parser->State->Id)) {
    // Change parser state only if not already the current state
    Parser->State = FindParseState(Parser, StateId);
    // Check parser state is valid
    if (Parser->State == NULL) {
      // State not found
      Status = EFI_NOT_FOUND;
    }
  }
  // Check if pushing the current or next state
  if (!EFI_ERROR(Status) && EFI_BITS_ANY_SET(Options, LANG_RULE_PUSH)) {
    // Check if the push state is not the current state
    if ((PushId != LANG_STATE_CURRENT) && ((State == NULL) || (PushId != State->Id))) {
      // Find and push the state
      State = FindParseState(Parser, PushId);
    }
    // Check parser state is valid
    if (State != NULL) {
      // Allocate the stack item
      Stack = EfiAllocateByType(LANG_STATE_STACK);
      if (Stack == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      // Push the state
      Stack->State = State;
      Stack->SaveState = Previous;
      Stack->Previous = Parser->PreviousStates;
      Parser->PreviousStates = Stack;
    } else {
      // State not found
      Status = EFI_NOT_FOUND;
    }
  }
  return Status;
}
// SEARCH_FUNCTION
/// Returns the first occurrence of a case-insensitive sub-string in a string
/// @param String       A pointer to a string
/// @param SearchString A pointer to a string for which to search
/// @return A pointer to the matched sub-string in String
/// @retval NULL If SearchString was not found in String
typedef CHAR16 *
(EFIAPI
*SEARCH_FUNCTION) (
  IN CHAR16 *String,
  IN CHAR16 *SearchString
);
// COMPARE_FUNCTION
/// Performs a comparison of two strings
/// @param Str1  The first string to comapre
/// @param Str2  The second string to compare
/// @param Count The maximum count of characters to compare
/// @retval 0  Str1 is equivalent to Str2
/// @retval >0 Str1 is lexically greater than Str2
/// @retval <0 Str1 is lexically less than Str2
typedef INTN
(EFIAPI
*COMPARE_FUNCTION) (
  IN CONST CHAR16 *Str1,
  IN CONST CHAR16 *Str2,
  IN UINTN         Count
);
// ParseCheckRules
/// Check whether a rule matching is satisfied
/// @param Parser  The language parser used for parsing
/// @param Rules   The language rules to check if satisfied
/// @param Options The parse options
/// @param Context The parse context
/// @return Whether the rule matching was satisfied or not
/// @retval EFI_INVALID_PARAMETER If Parser or Rule is NULL or internally invalid
/// @retval EFI_NOT_FOUND         If the rule matching was not satisfied
/// @retval EFI_SUCCESS           If the rule matching was satisfied
STATIC
EFI_STATUS
EFIAPI
ParseCheckRules (
  IN OUT LANG_PARSER *Parser,
  IN     LANG_RULE   *Rules,
  IN     UINTN        Options,
  IN     VOID        *Context OPTIONAL
) {
  EFI_STATUS        Status;
  CHAR16           *Token;
  CHAR16           *Match;
  LANG_RULE        *PartialRule;
  LANG_RULE        *MatchRule;
  UINTN             PartialOffset;
  UINTN             PartialLength;
  UINTN             MatchOffset;
  UINTN             MatchLength;
  UINTN             Offset;
  UINTN             Length;
  UINTN             RuleOptions;
  LANG_CALLBACK     Callback;
  SEARCH_FUNCTION   Search;
  COMPARE_FUNCTION  Compare;
  // Check parameters
  if ((Parser == NULL) || (Rules == NULL) ||
      (Parser->State == NULL) ||
      (Parser->Token == NULL) ||
      (Parser->TokenCount == 0)) {
    return EFI_BITS_ANY_SET(Options, LANG_PARSE_FINISH) ? EFI_SUCCESS : EFI_INVALID_PARAMETER;
  }
  // Iterate through all the rules
  PartialRule = NULL;
  PartialOffset = 0;
  PartialLength = 0;
  MatchRule = NULL;
  MatchOffset = 0;
  MatchLength = 0;
  LANG_LOG(L"State: 0x%x [%s] 0x%x\n", Parser->State->Id, Parser->Token, Parser->TokenCount);
  if (Parser->PreviousStates == NULL) {
    LANG_VERBOSE(L"No previous states\n");
  } else {
    LANG_STATE_STACK *Stack = Parser->PreviousStates;
    for (Length = 0; Stack != NULL; Stack = Stack->Previous) {
      ++Length;
    }
    LANG_VERBOSE(L"%u previous states {", Length);
    Stack = Parser->PreviousStates;
    while (Stack != NULL) {
      LANG_VERBOSE(L" 0x%p", Stack->State);
      if (Stack->State != NULL) {
        LANG_VERBOSE(L" (0x%x)", Stack->State->Id);
      }
      if (Stack->SaveState != NULL) {
        LANG_VERBOSE(L" [0x%p (0x%x)]", Stack->SaveState, Stack->SaveState->Id);
      }
      Stack = Stack->Previous;
    }
    LANG_VERBOSE(L" }\n");
  }
  while (Rules != NULL) {
    LANG_WHISPER(L"Rule: 0x%p, 0x%x, 0x%x, 0x%x, [%s] 0x%x\n", Rules->Callback, Rules->Options, Rules->NextState, Rules->PushState, Rules->Token, Rules->TokenCount);
    // Check if the rule allows or skips tokens before match
    if (EFI_BITS_ANY_SET(Rules->Options, LANG_RULE_TOKEN | LANG_RULE_SKIP_TOKEN | LANG_RULE_SKIP_EMPTY_TOKEN)) {
      // Determine if the rule token is inside the parser token
      if (EFI_BITS_ANY_SET(Rules->Options, LANG_RULE_INSENSITIVE)) {
        Search = (SEARCH_FUNCTION)StriStr;
        Compare = (COMPARE_FUNCTION)StrniCmp;
      } else {
        Search = (SEARCH_FUNCTION)StrStr;
        Compare = (COMPARE_FUNCTION)StrnCmp;
      }
      Token = Search(Parser->Token, Rules->Token);
      if (Token != NULL) {
        // The rule token was found inside the parser token so check if match when rule token is not the end
        // of parser token and not the beginning of the parser token unless skipping tokens before match
        Offset = (UINTN)(Token - Parser->Token);
        if (EFI_BITS_ANY_SET(Options, LANG_PARSE_FINISH)) {
          Length = Offset + Rules->TokenCount - 1;
        } else {
          Length = Offset + Rules->TokenCount;
        }
        if ((Length < Parser->TokenCount) &&
            ((Offset != 0) || EFI_BITS_ANY_SET(Rules->Options, LANG_RULE_SKIP_EMPTY_TOKEN))) {
          // This is an exact token match but may not be the best match so check
          if ((MatchLength < Rules->TokenCount) || (Offset < MatchOffset)) {
            // This is the best match
            LANG_WHISPER(L"Match\n");
            MatchRule = Rules;
            MatchOffset = Offset;
            MatchLength = Rules->TokenCount;
          }
        }
        Length = Parser->TokenCount;
      } else if ((Parser->TokenCount <= Rules->TokenCount) && EFI_BITS_ANY_SET(Rules->Options, LANG_RULE_SKIP_EMPTY_TOKEN) &&
                 (Compare(Parser->Token, Rules->Token, Parser->TokenCount) == 0)) {
        // This is a partial match to a rule that allows an empty token
        Offset = 0;
        Length = Parser->TokenCount;
        // If finishing check if this is a match
        if (EFI_BITS_ANY_SET(Options, LANG_PARSE_FINISH) && (Parser->TokenCount == Rules->TokenCount)) {
          // This is an exact token match but may not be the best match so check
          if ((MatchLength < Rules->TokenCount) || (Offset < MatchOffset)) {
            // This is the best match
            LANG_WHISPER(L"Match\n");
            MatchRule = Rules;
            MatchOffset = Offset;
            MatchLength = Rules->TokenCount;
          }
        }
      } else {
        // All these rules allow tokens before so always a partial match
        Offset = Parser->TokenCount;
        Length = 0;
      }
      // Check for partial match
      if (MatchRule != Rules) {
        BOOLEAN Matched = FALSE;
        if (MatchRule != NULL) {
          // Check if this partial match may be better than the match eventually
          if ((Offset < MatchOffset) || ((Offset == MatchOffset) && (MatchLength <= Length))) {
            Matched = TRUE;
          }
        } else if (PartialRule == NULL) {
          // Even if no match is found all of these rules allow tokens before match so all partially match
          Matched = TRUE;
        } else if ((Offset < PartialOffset) || ((PartialOffset == Offset) && (PartialLength <= Length))) {
          Matched = TRUE;
        }
        if (Matched) {
          // This may be a partial token match but it may be better than the match if there are more characters appended to the parser token
          LANG_WHISPER(L"Partial match\n");
          PartialRule = Rules;
          PartialOffset = Offset;
          PartialLength = Length;
        }
      }
    } else {
      // Check if this is not an exact rule match because we can not know if truly a rule
      //  match until only the beginning of the parser token matches the rule token
      Compare = (COMPARE_FUNCTION)(EFI_BITS_ANY_SET(Rules->Options, LANG_RULE_INSENSITIVE) ? StrniCmp : StrnCmp);
      if (Rules->TokenCount <= Parser->TokenCount) {
        if (Compare(Parser->Token, Rules->Token, Rules->TokenCount) == 0) {
          // This is an exact token match but may not be the best match so check
          if ((MatchRule == NULL) || (MatchOffset != 0) || (MatchLength < Rules->TokenCount)) {
            // This is the best match
            LANG_WHISPER(L"Match\n");
            MatchRule = Rules;
            MatchOffset = 0;
            MatchLength = Rules->TokenCount;
          }
        }
      }
      // Check for a partial match
      if ((MatchRule != Rules) &&
          ((PartialRule == NULL) || (PartialOffset != 0) || (PartialLength <= Parser->TokenCount)) &&
          (Compare(Parser->Token, Rules->Token, Parser->TokenCount) == 0)) {
        // This may be a partial token match but it may be better than the match if there are more characters appended to the parser token
        LANG_WHISPER(L"Partial match\n");
        PartialRule = Rules;
        PartialOffset = 0;
        PartialLength = Parser->TokenCount;
      }
    }
    // Get next rule
    Rules = Rules->Next;
  }
  if (PartialRule != NULL) {
    LANG_LOG(L"Partial match: [%s] [%s] 0x%x 0x%x\n", Parser->Token, PartialRule->Token, PartialOffset, PartialLength);
  }
  // Check if a rule was matched
  if (MatchRule == NULL) {
    if ((PartialRule != NULL) && ((PartialOffset != 0) || (PartialLength <= PartialRule->TokenCount))) {
      if (EFI_BITS_ARE_UNSET(Options, LANG_PARSE_FINISH)) {
        // Partially matching rule that does not warrant action so return success to continue parsing
        return EFI_SUCCESS;
      }
    }
    // No matching rule
    if (PartialRule != NULL) {
      ParseError(Parser, L"Unexpected token `%.*s`", PartialLength, PartialRule->Token);
    } else {
      ParseError(Parser, L"Unexpected token `%s`", Parser->Token);
    }
    return EFI_NOT_FOUND;
  }
  // Check the partial rule is still valid
  if (EFI_BITS_ARE_UNSET(Options, LANG_PARSE_FINISH) && (PartialRule != NULL) &&
      ((PartialOffset < MatchOffset) ||
       ((PartialOffset == MatchOffset) && (PartialLength >= MatchLength)))) {
    // There is a partially matching rule that is a better match and does not warrant action so return success to continue parsing
    return EFI_SUCCESS;
  }
  // Set callback
  if (MatchRule->Callback != NULL) {
    Callback = MatchRule->Callback;
  } else if (Parser->State->Callback != NULL) {
    Callback = Parser->State->Callback;
  } else {
    Callback = Parser->Callback;
  }
  if (Callback == NULL) {
    ParseError(Parser, L"No inherited callback for matched parser rule");
    return EFI_NOT_READY;
  }
  // Get the current token from the parser token
  Token = NULL;
  RuleOptions = MatchRule->Options;
  if (EFI_BITS_ANY_SET(RuleOptions, LANG_RULE_TOKEN)) {
    if (MatchOffset != 0) {
      if (EFI_BITS_ARE_UNSET(RuleOptions, LANG_RULE_SKIP_TOKEN)) {
        // Duplicate the token before match
        Token = StrnDup(Parser->Token, MatchOffset);
        if (Token == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
      }
    } else if (EFI_BITS_ARE_UNSET(RuleOptions, LANG_RULE_SKIP_EMPTY_TOKEN)) {
      // Unexepected token
      ParseError(Parser, L"Unexpected empty token");
      return EFI_NOT_FOUND;
    }
  }
  // Get the match token from the parser token
  Match = StrnDup(Parser->Token + MatchOffset, MatchLength);
  if (Match == NULL) {
    if (Token != NULL) {
      EfiFreePool(Token);
    }
    return EFI_OUT_OF_RESOURCES;
  }
  // Change parser token to the part that belongs to the next token
  Length = MatchOffset + MatchLength;
  Parser->TokenCount -= Length;
  EfiCopyArray(CHAR16, Parser->Token, Parser->Token + Length, Parser->TokenCount);
  EfiZeroArray(CHAR16, Parser->Token + Parser->TokenCount, Length);
  LANG_LOG(L"Match: 0x%x [%s] [%s]\n", Parser->State->Id, Token, Match);
  // Check if this is a previous state pop
  if (EFI_BITS_ANY_SET(RuleOptions, LANG_RULE_POP_BEFORE)) {
    // Pop the previous state
    Status = SetNextParseState(Parser, MatchRule->NextState, MatchRule->PushState, EFI_BITS_UNSET(RuleOptions, LANG_RULE_PUSH | LANG_RULE_POP | LANG_RULE_POP_AFTER));
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  // Callback for token
  if (Token != NULL) {
    Status = Callback(Parser, Parser->State->Id, Token, Context);
    // Check if there was an error parsing this token
    if (EFI_ERROR(Status)) {
      ParseError(Parser, L"Unexpected token `%s`", Token);
      EfiFreePool(Token);
      EfiFreePool(Match);
      return Status;
    }
    EfiFreePool(Token);
  }
  // Check if this is a previous state pop
  if (EFI_BITS_ANY_SET(RuleOptions, LANG_RULE_POP_AFTER)) {
    // Pop the previous state
    Status = SetNextParseState(Parser, MatchRule->NextState, MatchRule->PushState, EFI_BITS_UNSET(RuleOptions, LANG_RULE_PUSH | LANG_RULE_POP | LANG_RULE_POP_BEFORE));
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  // Callback for match
  if (EFI_BITS_ARE_UNSET(RuleOptions, LANG_RULE_SKIP) &&
      (EFI_BITS_ARE_UNSET(RuleOptions, LANG_RULE_SKIP_EMPTY) || (Token != NULL))) {
    Status = Callback(Parser, Parser->State->Id, Match, Context);
    // Check if there was an error parsing this token
    if (EFI_ERROR(Status)) {
      ParseError(Parser, L"Unexpected token `%s`", Match);
      EfiFreePool(Match);
      return Status;
    }
  }
  EfiFreePool(Match);
  // Pop the parser state, or set or push the next parser state
  return SetNextParseState(Parser, MatchRule->NextState, MatchRule->PushState, EFI_BITS_UNSET(RuleOptions, LANG_RULE_POP_BEFORE | LANG_RULE_POP_AFTER));
}

// ParseCharacter
/// Parse a character
/// @param Parser    The language parser to use in parsing
/// @param Character The next character to parse
/// @param Options   The parser options
/// @param Context   The parse context
/// @return Whether the character was parsed or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL or Character is invalid
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the character was parsed successfully
EFI_STATUS
EFIAPI
ParseCharacter (
  IN OUT LANG_PARSER *Parser,
  IN     UINT32       Character,
  IN     UINTN        Options,
  IN     VOID        *Context OPTIONAL
) {
  EFI_STATUS Status;
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Check langage parser is valid
  if (Parser->State == NULL) {
    ParseError(Parser, L"Invalid parser state");
    return EFI_NOT_FOUND;
  }
  if (Parser->State->Rules == NULL) {
    ParseError(Parser, L"Invalid parser state");
    return EFI_NOT_FOUND;
  }
  // Append the character to the token
  Status = StrAppend(&(Parser->Token), &(Parser->TokenCount), &(Parser->TokenSize), Character);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if ((Parser->Token == NULL) || (Parser->TokenCount == 0)) {
    if (EFI_BITS_ANY_SET(Options, LANG_PARSE_FINISH)) {
      return EFI_SUCCESS;
    }
    ParseError(Parser, L"Invalid token");
    return EFI_INVALID_PARAMETER;
  }
  // Check each rule
  LANG_VERBOSE(L"ParseCharacter(0x%x, 0x%x, 0x%p [%s], 0x%p [0x%x], 0x%p [0x%x])\n", Character, Parser->State->Id, &(Parser->Token), Parser->Token, &(Parser->TokenCount), Parser->TokenCount, &(Parser->TokenSize), Parser->TokenSize);
  Status = ParseCheckRules(Parser, Parser->State->Rules, Options, Context);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  // Advance the line offset
  if (Character == L'\n') {
    ++(Parser->LineNumber);
    Parser->LineOffset = 1;
  } else if (Character != L'\r') {
    ++(Parser->LineOffset);
  }
  return Status;
}
// ParseMacro
/// Parse a buffer for tokens as a macro
/// @param Parser   The language parser to use in parsing
/// @param Encoding The encoding of the buffer or NULL to try to detect the encoding
/// @param Buffer   The buffer to parse
/// @param Size     The size in bytes of the buffer
/// @param Macro    The name of the macro being parsed
/// @param Options  The parser options
/// @param Context  The context for the parse callback
/// @retval EFI_INVALID_PARAMETER If Parser or Buffer is NULL or Size is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_NOT_FOUND         If Encoding is NULL and the encoding could not be detected
/// @retval EFI_SUCCESS           If the string was parsed successfully
EFI_STATUS
EFIAPI
ParseMacro (
  IN OUT LANG_PARSER           *Parser,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     VOID                  *Buffer,
  IN     UINTN                  Size,
  IN     UINTN                  Options,
  IN     CHAR16                *Macro OPTIONAL,
  IN     VOID                  *Context OPTIONAL
) {
  EFI_STATUS  Status;
  CHAR16     *Token;
  CHAR16     *Source;
  UINTN       LineNumber;
  UINTN       LineOffset;
  UINTN       Length;
  UINTN       TokenSize;
  // Check parameters
  if ((Parser == NULL) || (Buffer == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Macro == NULL) {
    Macro = L"";
  }
  LANG_VERBOSE(L"Before macro: %s [%s] 0x%x 0x%x\n", Macro, Parser->Token, Parser->TokenCount, Parser->TokenSize);
  // Store token for after macro parsing
  Token = Parser->Token;
  Length = Parser->TokenCount;
  TokenSize = Parser->TokenSize;
  Parser->Token = NULL;
  Parser->TokenCount = 0;
  Parser->TokenSize = 0;
  // Store source and replace with macro relative
  Source = Parser->Source;
  if (Source == NULL) {
    Source = L"";
  }
  LineNumber = Parser->LineNumber;
  LineOffset = (Length < Parser->LineOffset) ? (Parser->LineOffset - Length) : 1;
  Parser->Source = EfiPoolPrint(L"%s(%d,%d):%s", Source, LineNumber, LineOffset, Macro);
  if (Parser->Source == NULL) {
    Parser->Source = Source;
    return EFI_OUT_OF_RESOURCES;
  }
  Parser->LineNumber = 1;
  Parser->LineOffset = 1;
  // Parse macro
  LANG_VERBOSE(L"Macro: %s [%s] 0x%x 0x%x\n", Macro, Token, Length, TokenSize);
  Status = Parse(Parser, Encoding, Buffer, Size, Options, Context);
  // Restore source
  EfiFreePool(Parser->Source);
  Parser->Source = Source;
  Parser->LineNumber = LineNumber;
  Parser->LineOffset = LineOffset;
  // Restore previous token to end of current token
  if ((Token != NULL) && (Length != 0)) {
    if (!EFI_ERROR(Status)) {
      if ((Parser->Token == NULL) || (Parser->TokenCount == 0)) {
        // Set the previous token back
        if (Parser->Token != NULL) {
          EfiFreePool(Parser->Token);
        }
        Parser->Token = Token;
        Parser->TokenCount = Length;
        Parser->TokenSize = TokenSize;
      } else {
        // Merge the current and previous token
        UINTN Count = Parser->TokenCount + Length;
        Count *= (Encoding->MaxUnits * Encoding->UnitSize);
        if (Parser->TokenSize > Count) {
          // Append the previous token in the current token
          EfiCopyArray(CHAR16, Parser->Token + Parser->TokenCount, Token, Length);
          Count /= (Encoding->MaxUnits * Encoding->UnitSize);
          Parser->Token[Count] = L'\0';
          Parser->TokenCount = Count;
          // Free the previous token
          EfiFreePool(Token);
        } else if (TokenSize > Count) {
          // Prepend the current token in the previous token
          EfiCopyArray(CHAR16, Token + Length, Token, Length);
          EfiCopyArray(CHAR16, Token, Parser->Token, Parser->TokenCount);
          Count /= (Encoding->MaxUnits * Encoding->UnitSize);
          Parser->Token[Count] = L'\0';
          Parser->TokenCount = Count;
          Parser->TokenSize = TokenSize;
          // Free the current token
          EfiFreePool(Parser->Token);
          Parser->Token = Token;
        } else {
          // There's not enough room in either token string for both tokens so create a new one
          CHAR16 *MergedToken = EfiPoolPrint(L"%*s%*s", Parser->TokenCount, Parser->Token, Length, Token);
          if (MergedToken == NULL) {
            // Out of memory?
            Status = EFI_OUT_OF_RESOURCES;
          } else {
            // Free the current and previous tokens and replace it with the merged token
            EfiFreePool(Parser->Token);
            EfiFreePool(Token);
            Parser->TokenSize = Count;
            Count /= (Encoding->MaxUnits * Encoding->UnitSize);
            Parser->Token = MergedToken;
            Parser->TokenCount = Count;
          }
        }
        LANG_VERBOSE(L"After macro: [%s] 0x%x 0x%x\n", Parser->Token, Parser->TokenCount, Parser->TokenSize);
      }
    }
  }
  return Status;
}
// Parse
/// Parse a buffer for tokens
/// @param Parser   The language parser to use in parsing
/// @param Encoding The encoding of the buffer or NULL to try to detect the encoding
/// @param Buffer   The buffer to parse
/// @param Options  The parser options
/// @param Size     The size in bytes of the buffer
/// @param Context  The context for the parse callback
/// @retval EFI_INVALID_PARAMETER If Parser or Buffer is NULL or Size is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_NOT_FOUND         If Encoding is NULL and the encoding could not be detected
/// @retval EFI_SUCCESS           If the string was parsed successfully
EFI_STATUS
EFIAPI
Parse (
  IN OUT LANG_PARSER           *Parser,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     VOID                  *Buffer,
  IN     UINTN                  Size,
  IN     UINTN                  Options,
  IN     VOID                  *Context OPTIONAL
) {
  EFI_STATUS Status;
  UINT32     Character;
  UINTN      OriginalOptions = Options;
  // Check parameters
  if ((Parser == NULL) || (Buffer == NULL) || (Size == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Try to detect encoding
  if (Encoding == NULL) {
    // Detect UTF-16 byte order mark
    if (Size >= (2 * sizeof(CHAR16))) {
      CHAR16 *Detect = (CHAR16 *)Buffer;
      if (*Detect == 0xFEFF) {
        // UTF-16 byte order mark
        Encoding = EfiUtf16Encoding();
      } else if (*Detect == 0xFFFE) {
        // UTF-16 byte swapped byte order mark
        Encoding = EfiUtf16SwappedEncoding();
      }
    }
    // Detect UTF-8 byte order mark
    if (Encoding == NULL) {
      if (Size >= (3 * sizeof(CHAR8))) {
        CHAR8 *Detect = (CHAR8 *)Buffer;
        if (*Detect++ == 0xEF) {
          if (*Detect++ == 0xBB) {
            if (*Detect++ == 0xBF) {
              // UTF-8 byte order mark
              Encoding = EfiUtf8Encoding();
            }
          }
        }
      }
    }
    // TODO: Attempt to detect encoding through pattern recognition

    // Fallback to default as last resort
    if (Encoding == NULL) {
      Encoding = EfiDefaultEncoding();
      if (Encoding == NULL) {
        return EFI_NOT_FOUND;
      }
    }
  }
  // Make sure options are suitable for parsing the entire character buffer
  Options &= ~LANG_PARSE_FINISH;
  // Parse the buffer
  do {
    UINTN ThisSize = Size;
    // Decode each character from the buffer
    Character = 0;
    Status = EfiDecodeCharacter(Encoding, Buffer, &ThisSize, &Character);
    if (EFI_ERROR(Status) || (Character == 0)) {
      break;
    }
    // Advance the buffer
    Buffer = ADDRESS_OFFSET(VOID, Buffer, ThisSize);
    // Decrease the remaining size
    Size -= ThisSize;
    // Parse the character
    Status = ParseCharacter(Parser, Character, Options, Context);
  // Check no error and there are remaining encoding units
  } while (!EFI_ERROR(Status) && (Size >= Encoding->UnitSize));
  // If the finish option is set then make sure to finish 
  if (!EFI_ERROR(Status) && EFI_BITS_ANY_SET(OriginalOptions, LANG_PARSE_FINISH)) {
    // Finish parsing
    Status = ParseFinish(Parser, Context);
  }
  // Return status
  return Status;
}
// ParseFinish
/// Ensure all parsing is completed
/// @param Parser   The language parser to use in parsing
/// @param Context  The context for the parse callback
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_NOT_FOUND         If Encoding is NULL and the encoding could not be detected
/// @retval EFI_NOT_FOUND         If parsing failed because an unexpected token was reached when no rule was matched
/// @retval EFI_NOT_READY         If parsing failed because a rule was matched but the callback determined the parser state was invalid or incomplete
/// @retval EFI_SUCCESS           If the string was parsed successfully
EFI_STATUS
EFIAPI
ParseFinish (
  IN OUT LANG_PARSER *Parser,
  IN     VOID        *Context OPTIONAL
) {
  // Finish parsing
  return ParseCharacter(Parser, L'\0', LANG_PARSE_FINISH, Context);
}

// FreeParseRule
/// Free parser state rule
/// @param Rule The parser state rule to free
/// @return Whether the parser state rule was freed or not
/// @retval EFI_INVALID_PARAMETER If Rule is NULL
/// @retval EFI_SUCCESS           If the parser state rule was freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseRule (
  IN LANG_RULE *Rule
) {
  // Check parameters
  if (Rule == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free the rule token
  if (Rule->Token != NULL) {
    EfiFreePool(Rule->Token);
    Rule->Token = NULL;
  }
  // Free the rule
  EfiFreePool(Rule);
  return EFI_SUCCESS;
}
// FreeParseRules
/// Free parser state rules
/// @param Rules The parser state rules to free
/// @return Whether the parser state rules were freed or not
/// @retval EFI_INVALID_PARAMETER If Rules is NULL or Count is zero
/// @retval EFI_SUCCESS           If the parser state rules were freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseRules (
  IN LANG_RULE *Rules
) {
  // Check parameters
  if (Rules == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Iterate through the rules
  while (Rules != NULL) {
    LANG_RULE *Next = Rules->Next;
    // Free each rule
    FreeParseRule(Rules);
    // Get the next rule to free
    Rules = Next;
  }
  return EFI_SUCCESS;
}

// CreateParseRule
/// Create parser state rule
/// @param Parser   The language parser for which to create the rule
/// @param StateId   The identifier of parser state for which to create the rule
/// @param Callback  The token parsed callback
/// @param Options   The parser state rule matching options
/// @param NextState The state to which to change if this rule is satisfied
/// @param PushState If Options has LANG_RULE_PUSH, the identifier of the language state to push or NULL to push the current language state
/// @param Token     The token used for matching
/// @return Whether the parser state rule was created or not
/// @retval EFI_INVALID_PARAMETER If Parser or Token is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not allocated for the parser state rule
/// @retval EFI_NOT_FOUND         If a parser state with the specified identifier was not found
/// @retval EFI_SUCCESS           If the parser state rule was created successfully
EFI_STATUS
EFIAPI
CreateParseRule (
  IN LANG_PARSER   *Parser,
  IN UINTN          StateId,
  IN LANG_CALLBACK  Callback OPTIONAL,
  IN UINTN          Options,
  IN UINTN          NextState,
  IN UINTN          PushState OPTIONAL,
  IN CHAR16        *Token
) {
  LANG_STATE *State;
  LANG_RULE  *Rule;
  // Check parameters
  if ((Parser == NULL) || (Token == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Find the parser state by identifier
  State = FindParseState(Parser, StateId);
  if (State == NULL) {
    return EFI_NOT_FOUND;
  }
  // Allocate state rule
  Rule = EfiAllocateByType(LANG_RULE);
  if (Rule == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Rule->Next = NULL;
  Rule->Callback = Callback;
  Rule->Options = Options;
  Rule->NextState = NextState;
  Rule->PushState = PushState;
  // Duplicate tokens
  Rule->Token = StrDup(Token);
  if (Rule->Token == NULL) {
    FreeParseRule(Rule);
    return EFI_OUT_OF_RESOURCES;
  }
  Rule->TokenCount = StrLen(Rule->Token);
  if (Rule->TokenCount == 0) {
    FreeParseRule(Rule);
    return EFI_INVALID_PARAMETER;
  }
  // Append the rule to the tail of the state's rules list
  if (State->Rules == NULL) {
    State->Rules = Rule;
  } else {
    LANG_RULE *Ptr = State->Rules;
    while (Ptr->Next != NULL) {
      Ptr = Ptr->Next;
    }
    Ptr->Next = Rule;
  }
  return EFI_SUCCESS;
}
// CreateParseRules
/// Create parser states from static states
/// @param Parser  The language parser for which to create this state
/// @param StateId The identifier of parser state for which to create the rules
/// @param Count   The count of parser state rules
/// @param Rules   The static parser state rules
/// @return Whether the parser states were created or not
/// @retval EFI_INVALID_PARAMETER If Parser or States is NULL or Count is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the states
/// @retval EFI_SUCCESS           If the parser states were created successfully
EFI_STATUS
EFIAPI
CreateParseRules (
  IN LANG_PARSER      *Parser,
  IN UINTN             StateId,
  IN UINTN             Count,
  IN LANG_STATIC_RULE *Rules
) {
  UINTN      Index;
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  // Check parameters
  if ((Parser != NULL) && (Rules != NULL) && (Count != 0) && (StateId != LANG_STATE_CURRENT)) {
    // Iterate through the static parser state rules
    for (Index = 0; Index < Count; ++Index) {
      LANG_STATIC_RULE *Rule = Rules + Index;
      // Create each parser state rule
      Status = CreateParseRule(Parser, StateId, Rule->Callback, Rule->Options, Rule->NextState, Rule->PushState, Rule->Token);
      if (EFI_ERROR(Status)) {
        break;
      }
    }
  }
  return Status;
}

// FreeParseState
/// Free parser state
/// @param State The parser state to free
/// @return Whether the parser state was freed or not
/// @retval EFI_INVALID_PARAMETER If States is NULL
/// @retval EFI_SUCCESS           If the parser state was freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseState (
  IN LANG_STATE *State
) {
  // Check parameters
  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free parser state rules list
  if (State->Rules != NULL) {
    FreeParseRules(State->Rules);
    State->Rules = NULL;
  }
  // Free parser state
  EfiFreePool(State);
  return EFI_SUCCESS;
}
// FreeParseStates
/// Free parser states
/// @param States The parser states to free
/// @return Whether the parser states were freed or not
/// @retval EFI_INVALID_PARAMETER If States is NULL or Count is zero
/// @retval EFI_SUCCESS           If the parser states were freed successfully
STATIC
EFI_STATUS
EFIAPI
FreeParseStates (
  IN LANG_STATE *States
) {
  // Check parameters
  if (States == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Iterate through parser states
  while (States != NULL) {
    LANG_STATE *Next = States->Next;
    // Free each parser state
    FreeParseState(States);
    // Get the next parser state
    States = Next;
  }
  return EFI_SUCCESS;
}

// CreateParseState
/// Create parser state
/// @param Parser   The language parser for which to create the state
/// @param Callback The token parsed callback
/// @param StateId  The identifier of this parser state
/// @return Whether the parser state was created or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_INVALID_PARAMETER If StateId is already associated with another state
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the state
/// @retval EFI_SUCCESS           If the parser state was created successfully
EFI_STATUS
EFIAPI
CreateParseState (
  IN  LANG_PARSER   *Parser,
  IN  LANG_CALLBACK  Callback OPTIONAL,
  IN  UINTN          StateId
) {
  LANG_STATE *State;
  // Check parameters
  if ((Parser == NULL) || (StateId == LANG_STATE_CURRENT)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check to make sure the parser state identifier does not exist already
  if (FindParseState(Parser, StateId) != NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate parser state
  State = EfiAllocateByType(LANG_STATE);
  if (State == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set parser state identifier
  State->Next = NULL;
  State->Id = StateId;
  State->Callback = Callback;
  State->Rules = NULL;
  // Append the state to the tail of the parser's states list
  if (Parser->States == NULL) {
    Parser->States = State;
  } else {
    LANG_STATE *Ptr = Parser->States;
    while (Ptr->Next != NULL) {
      Ptr = Ptr->Next;
    }
    Ptr->Next = State;
  }
  return EFI_SUCCESS;
}
// CreateParseStates
/// Create parser states from static states
/// @param Parser The language parser for which to create this state
/// @param Count  The count of parser states
/// @param States The static parser states
/// @return Whether the parser states were created or not
/// @retval EFI_INVALID_PARAMETER If Parser or States is NULL or Count is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the states
/// @retval EFI_SUCCESS           If the parser states were created successfully
EFI_STATUS
EFIAPI
CreateParseStates (
  IN LANG_PARSER       *Parser,
  IN UINTN              Count,
  IN LANG_STATIC_STATE *States
) {
  UINTN      Index;
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  // Check parameters
  if ((Parser != NULL) && (States != NULL) && (Count != 0)) {
    // Iterate through the parser states
    for (Index = 0; Index < Count; ++Index) {
      LANG_STATIC_STATE *State = States + Index;
      // Create each parser state
      Status = CreateParseState(Parser, State->Callback, State->Id);
      if (EFI_ERROR(Status)) {
        break;
      }
      // Create parser state rules
      Status = CreateParseRules(Parser, State->Id, State->Count, State->Rules);
      if (EFI_ERROR(Status)) {
        break;
      }
    }
  }
  return Status;
}

// SetParseSource
/// Set the source being parsed
/// @param Parser The parser used for parsing
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the source could not be changed
/// @retval EFI_SUCCESS           The source was changed successfully
EFI_STATUS
EFIAPI
SetParseSource (
  IN LANG_PARSER  *Parser,
  IN CONST CHAR16 *Source OPTIONAL
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free old source
  if (Parser->Source != NULL) {
    EfiFreePool(Parser->Source);
  }
  // Set new source
  if (Source == NULL) {
    Parser->Source = NULL;
  } else {
    Parser->Source = StrDup(Source);
    if (Parser->Source == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }
  return EFI_SUCCESS;
}

// SetParseCallback
/// Set the parser token parsed callback
/// @param Parser   The language parser
/// @param Callback The token parsed callback
/// @return Whether the callback was set or not
/// @retval EFI_SUCCESS If the callback was set successfully
EFI_STATUS
EFIAPI
SetParseCallback (
  IN OUT LANG_PARSER   *Parser,
  IN     LANG_CALLBACK  Callback OPTIONAL
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Parser->Callback = Callback;
  return EFI_SUCCESS;
}
// SetParseState
/// Set the language parser state
/// @param Parser The language parser
/// @param Id     The language parser state identifier
/// @return Whether the parser state was set or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_NOT_FOUND         If a parser state with the given identifier was not found
/// @retval EFI_SUCCESS           If the language parser state was set successfully
EFI_STATUS
EFIAPI
SetParseState (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Id
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Set the state
  return SetNextParseState(Parser, Id, LANG_STATE_CURRENT, 0);
}
// GetParseState
/// Get the language parser state
/// @param Parser The language parser
/// @param Id     On output, the language parser state identifier
/// @return Whether the parser state was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Id is NULL
/// @retval EFI_NOT_FOUND         If there is no current parser state
/// @retval EFI_SUCCESS           If the language parser state was retrieved successfully
EFI_STATUS
EFIAPI
GetParseState (
  IN OUT LANG_PARSER *Parser,
  OUT    UINTN       *Id
) {
  // Check parameters
  if ((Parser == NULL) || (Id == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there is a current state
  if ((Parser->State == NULL) ||
      (Parser->State->Id == LANG_STATE_CURRENT)) {
    return EFI_NOT_FOUND;
  }
  // Return current state
  *Id = Parser->State->Id;
  return EFI_SUCCESS;
}
// GetPreviousParseState
/// Get the previous language parser state
/// @param Parser     The language parser
/// @param PreviousId The language parser state identifier
/// @return Whether the previous parser state was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or PreviousId is NULL
/// @retval EFI_NOT_FOUND         If there is no previous parser state
/// @retval EFI_SUCCESS           If the previous language parser state was retrieved successfully
EFI_STATUS
EFIAPI
GetPreviousParseState (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN       *PreviousId
) {
  LANG_STATE *State;
  // Check parameters
  if ((Parser == NULL) || (PreviousId == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Check if there is a current state
  if (Parser->PreviousStates == NULL) {
    return EFI_NOT_FOUND;
  }
  // Get the previous state
  State = Parser->PreviousStates->State;
  if ((State == NULL) || (State->Id == LANG_STATE_CURRENT)) {
    return EFI_NOT_FOUND;
  }
  // Return current state
  *PreviousId = State->Id;
  return EFI_SUCCESS;
}

// LANG_MESSAGE_CALLBACK
/// Message callback
/// @param Parser  The language parser
/// @param Message The message created during parsing
/// @return Whether the token was valid or not
STATIC
VOID
EFIAPI
DebugMessageCallback (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *Message
) {
  UNUSED_PARAMETER(Parser);
  UNUSED_PARAMETER(Message);
  if (Message != NULL) {
    LANG_VERBOSE(L"%s\n", Message);
  }
}

// CreateParser
/// Create a language parser
/// @param Parser   On output, the created language parser, which must be freed with FreeParser
/// @param Source   The name of the source being parsed
/// @param Callback The token parsed callback
/// @return Whether the language parser was created or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the parser
/// @retval EFI_SUCCESS           If the language parser was created successfully
EFI_STATUS
EFIAPI
CreateParser (
  OUT LANG_PARSER   **Parser,
  IN  CONST CHAR16   *Source OPTIONAL,
  IN  LANG_CALLBACK   Callback OPTIONAL
) {
  LANG_PARSER *Ptr;
  // Check parameters
  if ((Parser == NULL) || (*Parser != NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  // Allocate the language parser
  Ptr = EfiAllocateByType(LANG_PARSER);
  if (Ptr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Set parsed token callback
  Ptr->Source = StrDup(Source);
  Ptr->Callback = Callback;
  Ptr->LineNumber = 1;
  Ptr->LineOffset = 1;
  Ptr->MessageCallback = DebugMessageCallback;
  Ptr->PreviousStates = NULL;
  // Return language parser
  *Parser = Ptr;
  return EFI_SUCCESS;
}
// CreateParserFromStates
/// Create a language parser with static parser states
/// @param Parser   On output, the created language parser, which must be freed with FreeParser
/// @param Source   The name of the source being parsed
/// @param Callback The token parsed callback
/// @param StateId  The identifier of the default state to set for the parser
/// @param Count    The count of static parser states
/// @param States   The static parser states to set for the language parser
/// @return Whether the language parser was created or not
/// @retval EFI_INVALID_PARAMETER If Parser or States is NULL or Count is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the parser
/// @retval EFI_NOT_FOUND         If the state with the specified identifier could not be found
/// @retval EFI_SUCCESS           If the language parser was created successfully
EFI_STATUS
EFIAPI
CreateParserFromStates (
  OUT LANG_PARSER       **Parser,
  IN  CONST CHAR16       *Source OPTIONAL,
  IN  LANG_CALLBACK       Callback OPTIONAL,
  IN  UINTN               StateId,
  IN  UINTN               Count,
  IN  LANG_STATIC_STATE  *States
) {
  EFI_STATUS   Status;
  LANG_PARSER *Ptr;
  // Check parameters
  if ((Parser == NULL) || (States == NULL) || (Count == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  // Create a parser
  Ptr = NULL;
  Status = CreateParser(&Ptr, Source, Callback);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  if (Ptr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Create the parser states
  Status = CreateParseStates(Ptr, Count, States);
  if (EFI_ERROR(Status)) {
    FreeParser(Ptr);
    return Status;
  }
  // Find initial state by identifier
  Ptr->State = FindParseState(Ptr, StateId);
  // Check if identifier found for initial state
  if (Ptr->State == NULL) {
    FreeParser(Ptr);
    return EFI_NOT_FOUND;
  }
  // Return parser
  *Parser = Ptr;
  return EFI_SUCCESS;
}
// ResetParser
/// Reset a language parser for reuse
/// @param Parser  The language parser
/// @param StateId The identifier of the default state to set for the parser
/// @param Source  The source being parsed
/// @return Whether the language parser was reset or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the language parser was reset successfully
EFI_STATUS
EFIAPI
ResetParser (
  IN OUT LANG_PARSER  *Parser,
  IN     UINTN         StateId,
  IN     CONST CHAR16 *Source OPTIONAL
) {
  EFI_STATUS Status;
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Reset the parser
  Parser->DecodeCount = 0;
  Parser->DecodedCharacter = 0;
  Parser->LineNumber = 1;
  Parser->LineOffset = 1;
  // Free the previous states
  while (Parser->PreviousStates != NULL) {
    LANG_STATE_STACK *State = Parser->PreviousStates;
    Parser->PreviousStates = State->Previous;
    EfiFreePool(State);
  }
  // Free the parser messages
  if (Parser->Messages != NULL) {
    FreeParseMessages(Parser->Messages);
    Parser->Messages = NULL;
  }
  // Free the token if present
  if (Parser->Token != NULL) {
    EfiFreePool(Parser->Token);
    Parser->Token = NULL;
  }
  Parser->TokenCount = 0;
  Parser->TokenSize = 0;
  // Set the source
  Status = SetParseSource(Parser, Source);
  if (EFI_ERROR(Status) || (StateId == LANG_STATE_CURRENT)) {
    return Status;
  }
  // Set the state by identifier
  return SetParseState(Parser, StateId);
}
// FreeParser
/// Free a language parser
/// @param Parser The language parser to free
/// @return Whether the language parser was freed or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the language parser was freed successfully
EFI_STATUS
EFIAPI
FreeParser (
  IN LANG_PARSER *Parser
) {
  // Check parameters
  if (Parser == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // Free the source
  if (Parser->Source != NULL) {
    EfiFreePool(Parser->Source);
    Parser->Source = NULL;
  }
  // Free the previous states
  while (Parser->PreviousStates != NULL) {
    LANG_STATE_STACK *State = Parser->PreviousStates;
    Parser->PreviousStates = State->Previous;
    EfiFreePool(State);
  }
  // Free the parser token
  if (Parser->Token != NULL) {
    EfiFreePool(Parser->Token);
    Parser->Token = NULL;
  }
  // Free the parser states
  if (Parser->States != NULL) {
    FreeParseStates(Parser->States);
    Parser->States = NULL;
  }
  // Free the parser messages
  if (Parser->Messages != NULL) {
    FreeParseMessages(Parser->Messages);
    Parser->Messages = NULL;
  }
  // Free the parser
  EfiFreePool(Parser);
  return EFI_SUCCESS;
}

// ParseLibInitialize
/// Parse library initialize use
/// @return Whether the parse library initialized successfully or not
/// @retval EFI_SUCCESS The parse library successfully initialized
EFI_STATUS
EFIAPI
ParseLibInitialize (
  VOID
) {
  return EFI_SUCCESS;
}

// ParseLibFinish
/// Parse library finish use
/// @return Whether the parse library finished successfully or not
/// @retval EFI_SUCCESS The parse library successfully finished
EFI_STATUS
EFIAPI
ParseLibFinish (
  VOID
) {
  return EFI_SUCCESS;
}
