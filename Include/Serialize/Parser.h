//
/// @file Include/Serialize/Parser.h
///
/// Language parser implementation
///

#pragma once
#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <Uefi.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// LANG_PARSE_FINISH
/// Finish parsing any rules, otherwise there may be unused tokens remaining
#define LANG_PARSE_FINISH EFI_BIT(0)

// LANG_STATE_PREVIOUS
/// Previous language state
#define LANG_STATE_PREVIOUS (~(UINTN)0)
// LANG_STATE_CURRENT
/// Current language state
#define LANG_STATE_CURRENT (~(UINTN)0)

// LANG_RULE_INSENSITIVE
/// Language state rule that uses case-insensitive matching
#define LANG_RULE_INSENSITIVE EFI_BIT(0)
// LANG_RULE_TOKEN
/// Language state rule that parses token before match, if present
#define LANG_RULE_TOKEN EFI_BIT(1)
// LANG_RULE_SKIP
/// Language state rule that skips match
#define LANG_RULE_SKIP EFI_BIT(2)
// LANG_RULE_SKIP_TOKEN
/// Language state rule that skips token before match
#define LANG_RULE_SKIP_TOKEN EFI_BIT(3)
// LANG_RULE_SKIP_EMPTY
/// Language state rule that skips match if token is empty
#define LANG_RULE_SKIP_EMPTY EFI_BIT(4)
// LANG_RULE_SKIP_EMPTY_TOKEN
/// Language state rule that skips empty token before match
#define LANG_RULE_SKIP_EMPTY_TOKEN EFI_BIT(5)
// LANG_RULE_PUSH
/// Language state rule that pushes the push state or current state on to the previous states stack
#define LANG_RULE_PUSH EFI_BIT(6)
// LANG_RULE_POP
/// Language state rule that pops the previous state before all callbacks
#define LANG_RULE_POP EFI_BIT(7)
// LANG_RULE_POP_BEFORE
/// Language state rule that pops the previous state before the match callback
#define LANG_RULE_POP_BEFORE EFI_BIT(8)
// LANG_RULE_POP_AFTER
/// Language state rule that pops the previous state after the match callback
#define LANG_RULE_POP_AFTER EFI_BIT(9)
// LANG_RULE_POP_SAFE
/// Language state rule that supplements popping the state but ignore the popped state if the state has changed since pushing
#define LANG_RULE_POP_SAFE EFI_BIT(10)

// DECL_LANG_RULES
/// Begin static state rules
/// @param Rules The declaration name of the static state rules
#define DECL_LANG_RULES(Rules) STATIC LANG_STATIC_RULE Rules[] = {
// END_LANG_RULES
/// End static state rules
#define END_LANG_RULES() }
// DECL_LANG_RULE_FULL
/// Declare a static state rule
/// @param Callback  The token parsed callback
/// @param Options   The options of parser state rule matching
/// @param NextState The state to which to change after this rule is satisfied
/// @param PushState If Options has LANG_RULE_PUSH, the identifier of the language state to push, or LANG_STATE_CURRENT to push the current language state
/// @param Token     The token to use for matching
#define DECL_LANG_RULE_FULL(Callback, Options, NextState, PushState, Token) { Callback, Options, NextState, PushState, Token }
// DECL_LANG_RULE_CALLBACK
/// Declare a static state rule
/// @param Callback  The token parsed callback
/// @param Options   The options of parser state rule matching
/// @param NextState The state to which to change after this rule is satisfied
/// @param Token     The token to use for matching
#define DECL_LANG_RULE_CALLBACK(Callback, Options, NextState, Token) DECL_LANG_RULE_FULL(Callback, Options, NextState, LANG_STATE_CURRENT, Token)
// DECL_LANG_RULE_PUSH
/// Declare a static state rule
/// @param Options   The options of parser state rule matching
/// @param NextState The state to which to change after this rule is satisfied
/// @param PushState If Options has LANG_RULE_PUSH, the identifier of the language state to push, or LANG_STATE_CURRENT to push the current language state
/// @param Token     The token to use for matching
#define DECL_LANG_RULE_PUSH(Options, NextState, PushState, Token) DECL_LANG_RULE_FULL(NULL, Options, NextState, PushState, Token)
// DECL_LANG_RULE
/// Declare a static state rule
/// @param Options   The options of parser state rule matching
/// @param NextState The state to which to change after this rule is satisfied
/// @param Token     The token to use for matching
#define DECL_LANG_RULE(Options, NextState, Token) DECL_LANG_RULE_FULL(NULL, Options, NextState, LANG_STATE_CURRENT, Token)

// DECL_LANG_STATES
/// Begin static states
/// @param States The declaration name of the static states
#define DECL_LANG_STATES(States) STATIC LANG_STATIC_STATE States[] = {
// END_LANG_STATES
/// End static states
#define END_LANG_STATES() }
// DECL_LANG_STATE
/// Begin a static state
/// @param State   The identifier of the state
/// @param Rules   The rules for this state
#define DECL_LANG_STATE(State, Rules) { NULL, State, ARRAY_COUNT(Rules), Rules }
// DECL_LANG_STATE_CALLBACK
/// Begin a static state
/// @param Callback The token parsed callback
/// @param State    The identifier of the state
/// @param Rules    The rules for this state
#define DECL_LANG_STATE_CALLBACK(Callback, State, Rules) { Callback, State, ARRAY_COUNT(Rules), Rules }

// LANG_RULE
/// Language state rule
typedef struct LANG_RULE LANG_RULE;
// LANG_STATE
/// Language state
typedef struct LANG_STATE LANG_STATE;
// LANG_PARSER
/// Language parser
typedef struct LANG_PARSER LANG_PARSER;

// LANG_CALLBACK
/// Token parsed callback
/// @param Parser  The language parser
/// @param StateId The current language parser state identifier
/// @param Token   The parsed token
/// @param Context The parse context
/// @return Whether the token was valid or not
typedef EFI_STATUS
(EFIAPI
*LANG_CALLBACK) (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        StateId,
  IN     CHAR16      *Token,
  IN     VOID        *Context OPTIONAL
);
// LANG_MESSAGE_CALLBACK
/// Message callback
/// @param Parser  The language parser
/// @param Message The message created during parsing
/// @return Whether the token was valid or not
typedef VOID
(EFIAPI
*LANG_MESSAGE_CALLBACK) (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *Message
);

// LANG_STATIC_RULE
/// Static information for state rule
typedef struct LANG_STATIC_RULE LANG_STATIC_RULE;
struct LANG_STATIC_RULE {

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
  /// The identifier of the language state to push if Options has LANG_RULE_PUSH or NULL for the current language state
  UINTN          PushState;
  // Token
  /// The token string
  CHAR16        *Token;

};
// LANG_STATIC_STATE
/// Static information for state
typedef struct LANG_STATIC_STATE LANG_STATIC_STATE;
struct LANG_STATIC_STATE {

  // Callback
  /// Token parsed callback
  LANG_CALLBACK     Callback;
  // Id
  /// The language state identifier
  UINTN             Id;
  // Count
  /// The count of language state rules
  UINTN             Count;
  // States
  /// The language state rules
  LANG_STATIC_RULE *Rules;

};

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
EXTERN
EFI_STATUS
EFIAPI
ParseCharacter (
  IN OUT LANG_PARSER *Parser,
  IN     UINT32       Character,
  IN     UINTN        Options,
  IN     VOID        *Context OPTIONAL
);
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
EXTERN
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
);
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
/// @retval EFI_NOT_FOUND         If parsing failed because an unexpected token was reached when no rule was matched
/// @retval EFI_NOT_READY         If parsing failed because a rule was matched but the callback determined the parser state was invalid or incomplete
/// @retval EFI_SUCCESS           If the string was parsed successfully
EXTERN
EFI_STATUS
EFIAPI
Parse (
  IN OUT LANG_PARSER           *Parser,
  IN     EFI_ENCODING_PROTOCOL *Encoding OPTIONAL,
  IN     VOID                  *Buffer,
  IN     UINTN                  Size,
  IN     UINTN                  Options,
  IN     VOID                  *Context OPTIONAL
);
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
EXTERN
EFI_STATUS
EFIAPI
ParseFinish (
  IN OUT LANG_PARSER *Parser,
  IN     VOID        *Context OPTIONAL
);

// LogParseMessages
/// Log the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @return Whether the parser messafes were logged or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
LogParseMessages (
  IN LANG_PARSER   *Parser,
  IN CHAR16        *Format OPTIONAL,
  ...
);
// LogParseVMessages
/// Log the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @param Args     The arguments
/// @return Whether the parser messafes were logged or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
LogParseVMessages (
  IN LANG_PARSER   *Parser,
  IN CHAR16        *Format OPTIONAL,
  IN VA_LIST        Args
);

// ParseMessages
/// Get the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Count    On output, the count of parser messages
/// @param Messages On output, a null-terminated array of parser message strings, which must be freed with StrListFree
/// @param Format   The style format specifier for the parser messages or NULL for the default style
/// @return Whether the parser messafes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser, Count, or Messages is NULL or *Messages is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
ParseMessages (
  IN  LANG_PARSER   *Parser,
  OUT UINTN         *Count,
  OUT CHAR16      ***Messages,
  IN  CHAR16        *Format OPTIONAL,
  ...
);
// ParseVMessages
/// Get the parser messages
/// @param Parser   The language parser of which to get the messages
/// @param Count    On output, the count of parser messages
/// @param Messages On output, a null-terminated array of parser message strings, which must be freed with StrListFree
/// @param Format   The style format specifier for the parser messages or NULL for the default styles
/// @param Args     The arguments
/// @return Whether the parser messafes were retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser, Count, or Messages is NULL or *Messages is not NULL
/// @retval EFI_OUT_OF_RESOURCES  If the messages could not be allocated
/// @retval EFI_SUCCESS           If the parser messages were retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
ParseVMessages (
  IN  LANG_PARSER   *Parser,
  OUT UINTN         *Count,
  OUT CHAR16      ***Messages,
  IN  CHAR16        *Format OPTIONAL,
  IN  VA_LIST        Args
);

// ParseMessage
/// Add a parser message
/// @param Parser      The language parser to which to add a message
/// @param Flags       The message flags
/// @param MessageDesc The description of the message
/// @return Whether the message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or MessageDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the message was added successfully
EXTERN
EFI_STATUS
EFIAPI
ParseMessage (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Flags,
  IN     CHAR16      *MessageDesc,
  ...
);
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
EXTERN
EFI_STATUS
EFIAPI
ParseVMessage (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Flags,
  IN     CHAR16      *MessageDesc,
  IN     VA_LIST      Args
);

// ParseError
/// Add a parser error
/// @param Parser    The language parser to which to add an error
/// @param ErrorDesc The description of the error
/// @return Whether the error message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the error message was added successfully
EXTERN
EFI_STATUS
EFIAPI
ParseError (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *ErrorDesc,
  ...
);
// ParseVError
/// Add a parser error
/// @param Parser    The language parser to which to add an error
/// @param ErrorDesc The description of the error
/// @param Args      The arguments
/// @return Whether the error message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the error message was added successfully
EXTERN
EFI_STATUS
EFIAPI
ParseVError (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *ErrorDesc,
  IN     VA_LIST      Args
);

// ParseWarn
/// Add a parser warning
/// @param Parser   The language parser to which to add a warning
/// @param WarnDesc The description of the warning
/// @return Whether the warning message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the warning message was added successfully
EXTERN
EFI_STATUS
EFIAPI
ParseWarn (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *WarnDesc,
  ...
);
// ParseVWarn
/// Add a parser warning
/// @param Parser   The language parser to which to add a warning
/// @param WarnDesc The description of the warning
/// @param Args     The arguments
/// @return Whether the warning message was added or not
/// @retval EFI_INVALID_PARAMETER If Parser or ErrorDesc is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated
/// @retval EFI_SUCCESS           If the warning message was added successfully
EXTERN
EFI_STATUS
EFIAPI
ParseVWarn (
  IN OUT LANG_PARSER *Parser,
  IN     CHAR16      *WarnDesc,
  IN     VA_LIST      Args
);

// SetParseSource
/// Set the source being parsed
/// @param Parser The parser used for parsing
/// @param Source The source being parsed
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If the source could not be changed
/// @retval EFI_SUCCESS           The source was changed successfully
EXTERN
EFI_STATUS
EFIAPI
SetParseSource (
  IN LANG_PARSER  *Parser,
  IN CONST CHAR16 *Source OPTIONAL
);

// SetParseCallback
/// Set the parser token parsed callback
/// @param Parser   The language parser
/// @param Callback The token parsed callback
/// @return Whether the callback was set or not
/// @retval EFI_SUCCESS If the callback was set successfully
EXTERN
EFI_STATUS
EFIAPI
SetParseCallback (
  IN OUT LANG_PARSER   *Parser,
  IN     LANG_CALLBACK  Callback OPTIONAL
);
// SetParseState
/// Set the language parser state
/// @param Parser The language parser
/// @param Id     The language parser state identifier
/// @return Whether the parser state was set or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_NOT_FOUND         If a parser state with the given identifier was not found
/// @retval EFI_SUCCESS           If the language parser state was set successfully
EXTERN
EFI_STATUS
EFIAPI
SetParseState (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN        Id
);
// GetParseState
/// Get the language parser state
/// @param Parser The language parser
/// @param Id     On output, the language parser state identifier
/// @return Whether the parser state was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or Id is NULL
/// @retval EFI_NOT_FOUND         If there is no current parser state
/// @retval EFI_SUCCESS           If the language parser state was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
GetParseState (
  IN OUT LANG_PARSER *Parser,
  OUT    UINTN       *Id
);
// GetPreviousParseState
/// Get the previous language parser state
/// @param Parser     The language parser
/// @param PreviousId The language parser state identifier
/// @return Whether the previous parser state was retrieved or not
/// @retval EFI_INVALID_PARAMETER If Parser or PreviousId is NULL
/// @retval EFI_NOT_FOUND         If there is no previous parser state
/// @retval EFI_SUCCESS           If the previous language parser state was retrieved successfully
EXTERN
EFI_STATUS
EFIAPI
GetPreviousParseState (
  IN OUT LANG_PARSER *Parser,
  IN     UINTN       *PreviousId
);

// CreateParseRule
/// Create parser state rule
/// @param Parser   The language parser for which to create the rule
/// @param StateId   The identifier of parser state for which to create the rule
/// @param Callback  The token parsed callback
/// @param Options   The parser state rule matching options
/// @param NextState The state to which to change if this rule is satisfied
/// @param PushState If Options has LANG_RULE_PUSH, the identifier of the language state to push or LANG_STATE_CURRENT to push the current language state
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
  IN UINTN          PushState,
  IN CHAR16        *Token
);
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
EXTERN
EFI_STATUS
EFIAPI
CreateParseRules (
  IN LANG_PARSER      *Parser,
  IN UINTN             StateId,
  IN UINTN             Count,
  IN LANG_STATIC_RULE *Rules
);

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
EXTERN
EFI_STATUS
EFIAPI
CreateParseState (
  IN  LANG_PARSER   *Parser,
  IN  LANG_CALLBACK  Callback OPTIONAL,
  IN  UINTN          StateId
);
// CreateParseStates
/// Create parser states from static states
/// @param Parser The language parser for which to create this state
/// @param Count  The count of parser states
/// @param States The static parser states
/// @return Whether the parser states were created or not
/// @retval EFI_INVALID_PARAMETER If Parser or States is NULL or Count is zero
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the states
/// @retval EFI_SUCCESS           If the parser states were created successfully
EXTERN
EFI_STATUS
EFIAPI
CreateParseStates (
  IN LANG_PARSER       *Parser,
  IN UINTN              Count,
  IN LANG_STATIC_STATE *States
);

// CreateParser
/// Create a language parser
/// @param Parser   On output, the created language parser, which must be freed with FreeParser
/// @param Source   The name of the source being parsed
/// @param Callback The token parsed callback
/// @return Whether the language parser was created or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_OUT_OF_RESOURCES  If memory could not be allocated for the parser
/// @retval EFI_SUCCESS           If the language parser was created successfully
EXTERN
EFI_STATUS
EFIAPI
CreateParser (
  OUT LANG_PARSER   **Parser,
  IN  CONST CHAR16   *Source OPTIONAL,
  IN  LANG_CALLBACK   Callback OPTIONAL
);
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
EXTERN
EFI_STATUS
EFIAPI
CreateParserFromStates (
  OUT LANG_PARSER       **Parser,
  IN  CONST CHAR16       *Source OPTIONAL,
  IN  LANG_CALLBACK       Callback OPTIONAL,
  IN  UINTN               StateId,
  IN  UINTN               Count,
  IN  LANG_STATIC_STATE  *States
);
// ResetParser
/// Reset a language parser for reuse
/// @param Parser  The language parser
/// @param StateId The identifier of the default state to set for the parser
/// @param Source  The source being parsed
/// @return Whether the language parser was reset or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the language parser was reset successfully
EXTERN
EFI_STATUS
EFIAPI
ResetParser (
  IN OUT LANG_PARSER  *Parser,
  IN     UINTN         StateId,
  IN     CONST CHAR16 *Source OPTIONAL
);
// FreeParser
/// Free a language parser
/// @param Parser The language parser to free
/// @return Whether the language parser was freed or not
/// @retval EFI_INVALID_PARAMETER If Parser is NULL
/// @retval EFI_SUCCESS           If the language parser was freed successfully
EXTERN
EFI_STATUS
EFIAPI
FreeParser (
  IN LANG_PARSER *Parser
);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __PARSER_HEADER__
