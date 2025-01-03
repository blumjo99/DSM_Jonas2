/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 1 "grammar.y"
 

#include "grammar.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <iostream>
#include "cJSON.h"


using namespace std;
int get_token_id (char*);
const char *getValue (cJSON *token);
const char *getLine (cJSON *token);
cJSON *unary (char *fname, cJSON *a); 
cJSON *binary (char *fname, cJSON *a, cJSON *b); 
cJSON *ternary (char *fname, cJSON *a, cJSON *b, cJSON *c); 
char *linenumber;
char *curtoken;
char *curtype;
#line 50 "grammar.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 60
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE cJSON *
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE             77
#define YYNRULE              47
#define YY_MAX_SHIFT         76
#define YY_MIN_SHIFTREDUCE   104
#define YY_MAX_SHIFTREDUCE   150
#define YY_MIN_REDUCE        151
#define YY_MAX_REDUCE        197
#define YY_ERROR_ACTION      198
#define YY_ACCEPT_ACTION     199
#define YY_NO_ACTION         200
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (405)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    31,   47,  106,  134,   64,  145,    1,   21,  199,   30,
 /*    10 */   117,  135,  150,   57,   26,  134,  114,  115,  116,  118,
 /*    20 */   119,  120,  121,  122,   24,   70,   31,   20,   69,   15,
 /*    30 */    14,  145,    1,   33,  136,  134,  117,   74,   29,   73,
 /*    40 */    26,  148,  114,  115,  116,  118,  119,  120,  121,  122,
 /*    50 */     4,   16,   34,   31,  134,   15,   14,   72,  145,    1,
 /*    60 */    35,   66,  134,  117,   36,   13,  134,   26,    5,  114,
 /*    70 */   115,  116,  118,  119,  120,  121,  122,   19,    9,   27,
 /*    80 */    25,   68,   15,   14,    7,   58,   23,   22,   21,   37,
 /*    90 */    19,  134,   27,   25,    6,   38,   17,  134,   58,   23,
 /*   100 */    22,   21,   39,   19,  134,   27,   25,    8,   20,  151,
 /*   110 */    18,   58,   23,   22,   21,    2,  153,   62,   74,  133,
 /*   120 */    73,   20,  153,  153,   28,   32,   10,   63,   40,  153,
 /*   130 */   134,   74,  153,   73,   20,  153,   55,    3,  134,   19,
 /*   140 */   153,   27,   25,   11,   74,  153,   73,   58,   23,   22,
 /*   150 */    21,   41,  153,  134,  112,   67,   19,  153,   27,   25,
 /*   160 */    58,   23,   22,   21,   58,   23,   22,   21,  153,  153,
 /*   170 */    20,  111,  153,  153,   23,   22,   21,   56,  153,  134,
 /*   180 */    74,  153,   73,   20,   44,  153,  134,   20,  153,   45,
 /*   190 */    46,  134,  134,   74,  153,   73,   20,   74,   48,   73,
 /*   200 */   134,   19,  153,   27,   25,   49,   74,  134,   73,   58,
 /*   210 */    23,   22,   21,  153,  153,   50,  110,  134,   19,  153,
 /*   220 */    27,   25,   51,   59,  134,  134,   58,   23,   22,   21,
 /*   230 */   153,  153,   20,  109,  153,   60,   61,  134,  134,  153,
 /*   240 */   153,   62,   74,   54,   73,  134,  153,  153,   28,   20,
 /*   250 */    10,   63,   53,   76,  134,  153,   42,   75,  134,   74,
 /*   260 */    52,   73,  134,   19,  153,   27,   25,   19,  153,   27,
 /*   270 */    25,   58,   23,   22,   21,   58,   23,   22,   21,   67,
 /*   280 */   153,   71,  108,   65,  130,  131,  132,  153,  153,  153,
 /*   290 */   153,   12,  153,   43,   20,  134,  153,  153,   20,  153,
 /*   300 */   153,  153,  153,  153,   74,  153,   73,  153,   74,  153,
 /*   310 */    73,  153,   19,  153,   27,   25,   19,  153,   27,   25,
 /*   320 */    58,   23,   22,   21,   58,   23,   22,   21,  153,  153,
 /*   330 */   153,  107,  153,  153,  113,  153,  153,  153,  153,  153,
 /*   340 */   153,  153,  153,   20,  153,  153,  153,   20,  153,  153,
 /*   350 */   153,  153,  153,   74,  153,   73,  153,   74,  153,   73,
 /*   360 */   153,   19,  153,   27,   25,  153,  153,  153,  153,   58,
 /*   370 */    23,   22,   21,  153,  153,  153,  153,  153,  153,  153,
 /*   380 */   153,  153,  153,  153,  153,  153,  153,  153,  153,  153,
 /*   390 */   153,  153,   20,  153,  153,  153,  153,  153,  153,  153,
 /*   400 */   153,  153,   74,  153,   73,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,   55,   54,   57,   58,   13,   14,   12,   52,   53,
 /*    10 */    18,   19,   21,   55,   22,   57,   24,   25,   26,   27,
 /*    20 */    28,   29,   30,   31,   56,   18,    8,   32,   21,   37,
 /*    30 */    38,   13,   14,   55,   19,   57,   18,   42,   53,   44,
 /*    40 */    22,   16,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    50 */    47,   36,   55,    8,   57,   37,   38,   39,   13,   14,
 /*    60 */    55,   18,   57,   18,   55,    5,   57,   22,   20,   24,
 /*    70 */    25,   26,   27,   28,   29,   30,   31,    1,   14,    3,
 /*    80 */     4,   18,   37,   38,   20,    9,   10,   11,   12,   55,
 /*    90 */     1,   57,    3,    4,   20,   55,   45,   57,    9,   10,
 /*   100 */    11,   12,   55,    1,   57,    3,    4,   20,   32,    0,
 /*   110 */    43,    9,   10,   11,   12,    7,   59,    8,   42,   35,
 /*   120 */    44,   32,   59,   59,   15,   49,   17,   18,   55,   59,
 /*   130 */    57,   42,   59,   44,   32,   59,   55,   48,   57,    1,
 /*   140 */    59,    3,    4,   41,   42,   59,   44,    9,   10,   11,
 /*   150 */    12,   55,   59,   57,   16,   46,    1,   59,    3,    4,
 /*   160 */     9,   10,   11,   12,    9,   10,   11,   12,   59,   59,
 /*   170 */    32,   16,   59,   59,   10,   11,   12,   55,   59,   57,
 /*   180 */    42,   59,   44,   32,   55,   59,   57,   32,   59,   55,
 /*   190 */    55,   57,   57,   42,   59,   44,   32,   42,   55,   44,
 /*   200 */    57,    1,   59,    3,    4,   55,   42,   57,   44,    9,
 /*   210 */    10,   11,   12,   59,   59,   55,   16,   57,    1,   59,
 /*   220 */     3,    4,   55,   55,   57,   57,    9,   10,   11,   12,
 /*   230 */    59,   59,   32,   16,   59,   55,   55,   57,   57,   59,
 /*   240 */    59,    8,   42,   55,   44,   57,   59,   59,   15,   32,
 /*   250 */    17,   18,   55,    2,   57,   59,   55,    6,   57,   42,
 /*   260 */    55,   44,   57,    1,   59,    3,    4,    1,   59,    3,
 /*   270 */     4,    9,   10,   11,   12,    9,   10,   11,   12,   46,
 /*   280 */    59,   19,   16,   50,   33,   34,   35,   59,   59,   59,
 /*   290 */    59,   40,   59,   55,   32,   57,   59,   59,   32,   59,
 /*   300 */    59,   59,   59,   59,   42,   59,   44,   59,   42,   59,
 /*   310 */    44,   59,    1,   59,    3,    4,    1,   59,    3,    4,
 /*   320 */     9,   10,   11,   12,    9,   10,   11,   12,   59,   59,
 /*   330 */    59,   16,   59,   59,   23,   59,   59,   59,   59,   59,
 /*   340 */    59,   59,   59,   32,   59,   59,   59,   32,   59,   59,
 /*   350 */    59,   59,   59,   42,   59,   44,   59,   42,   59,   44,
 /*   360 */    59,    1,   59,    3,    4,   59,   59,   59,   59,    9,
 /*   370 */    10,   11,   12,   59,   59,   59,   59,   59,   59,   59,
 /*   380 */    59,   59,   59,   59,   59,   59,   59,   59,   59,   59,
 /*   390 */    59,   59,   32,   59,   59,   59,   59,   59,   59,   59,
 /*   400 */    59,   59,   42,   59,   44,
};
#define YY_SHIFT_USE_DFLT (-10)
#define YY_SHIFT_COUNT (76)
#define YY_SHIFT_MIN   (-9)
#define YY_SHIFT_MAX   (360)
static const short yy_shift_ofst[] = {
 /*     0 */   -10,   -8,   18,   45,   45,   45,   45,   45,   45,   45,
 /*    10 */    45,   45,   45,   45,   45,   45,   45,   45,   45,   45,
 /*    20 */    45,   45,   45,   45,   45,   45,   45,   45,   45,  233,
 /*    30 */   109,   -9,  -10,   76,   89,  138,  155,  200,  217,  262,
 /*    40 */   266,  102,  311,  315,  360,  360,  360,  360,  360,  360,
 /*    50 */   360,  360,  151,  151,  151,  164,  164,  164,  251,   -5,
 /*    60 */    -5,   -5,    7,   64,   15,   25,    3,   43,   48,   63,
 /*    70 */    74,   87,   60,   51,   67,  108,   84,
};
#define YY_REDUCE_USE_DFLT (-55)
#define YY_REDUCE_COUNT (32)
#define YY_REDUCE_MIN   (-54)
#define YY_REDUCE_MAX   (238)
static const short yy_reduce_ofst[] = {
 /*     0 */   -44,  -54,  -42,  -22,   -3,    5,    9,   34,   40,   47,
 /*    10 */    73,   81,   96,  122,  129,  134,  135,  143,  150,  160,
 /*    20 */   167,  168,  180,  181,  188,  197,  201,  205,  238,  -52,
 /*    30 */   -52,  -32,  -15,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   152,  198,  198,  198,  198,  198,  198,  198,  198,  198,
 /*    10 */   198,  198,  198,  198,  198,  198,  198,  198,  198,  198,
 /*    20 */   198,  198,  198,  198,  198,  198,  198,  198,  198,  198,
 /*    30 */   198,  196,  152,  198,  198,  198,  198,  198,  198,  198,
 /*    40 */   198,  198,  198,  198,  187,  186,  185,  184,  194,  193,
 /*    50 */   188,  176,  171,  172,  170,  191,  190,  189,  198,  175,
 /*    60 */   174,  173,  198,  198,  198,  198,  198,  198,  198,  198,
 /*    70 */   198,  198,  198,  198,  198,  198,  198,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "WHERE",         "NOT",           "PLUS",        
  "MINUS",         "EQUAL",         "LESS",          "THAN",        
  "TIME",          "IS",            "TIMES",         "DIVIDE",      
  "POWER",         "IT",            "LSPAR",         "WRITE",       
  "SEMICOLON",     "TRACE",         "IDENTIFIER",    "RSPAR",       
  "ASSIGN",        "OF",            "LPAR",          "RPAR",        
  "NUMTOKEN",      "TIMETOKEN",     "STRTOKEN",      "TRUE",        
  "FALSE",         "CURRENTTIME",   "NOW",           "NULL1",       
  "AMPERSAND",     "NUMBER",        "STRING",        "LIST",        
  "COMMA",         "COUNT",         "FIRST",         "OR",          
  "WITHIN",        "TO",            "OCCURS",        "BEFORE",      
  "OCCURRED",      "AFTER",         "FOR",           "IN",          
  "FORRANGE",      "DO",            "ENDDO",         "error",       
  "code",          "statementblock",  "statement",     "ex",          
  "of",            "jsonarray",     "exlist",      
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "code ::= statementblock",
 /*   1 */ "statementblock ::=",
 /*   2 */ "statementblock ::= statementblock statement",
 /*   3 */ "statement ::= WRITE ex SEMICOLON",
 /*   4 */ "statement ::= TRACE ex SEMICOLON",
 /*   5 */ "statement ::= IDENTIFIER LSPAR ex RSPAR ASSIGN ex SEMICOLON",
 /*   6 */ "statement ::= IDENTIFIER ASSIGN ex SEMICOLON",
 /*   7 */ "statement ::= TIME IDENTIFIER ASSIGN ex SEMICOLON",
 /*   8 */ "statement ::= TIME OF IDENTIFIER ASSIGN ex SEMICOLON",
 /*   9 */ "ex ::= LPAR ex RPAR",
 /*  10 */ "ex ::= NUMTOKEN",
 /*  11 */ "ex ::= TIMETOKEN",
 /*  12 */ "ex ::= STRTOKEN",
 /*  13 */ "ex ::= IDENTIFIER",
 /*  14 */ "ex ::= TRUE",
 /*  15 */ "ex ::= FALSE",
 /*  16 */ "ex ::= CURRENTTIME",
 /*  17 */ "ex ::= NOW",
 /*  18 */ "ex ::= NULL1",
 /*  19 */ "ex ::= TIME of ex",
 /*  20 */ "ex ::= ex PLUS ex",
 /*  21 */ "ex ::= ex MINUS ex",
 /*  22 */ "ex ::= ex TIMES ex",
 /*  23 */ "ex ::= ex DIVIDE ex",
 /*  24 */ "ex ::= ex POWER ex",
 /*  25 */ "ex ::= ex AMPERSAND ex",
 /*  26 */ "ex ::= ex IS NUMBER",
 /*  27 */ "ex ::= ex IS STRING",
 /*  28 */ "ex ::= ex IS LIST",
 /*  29 */ "ex ::= ex IS NOT LIST",
 /*  30 */ "ex ::= jsonarray",
 /*  31 */ "jsonarray ::= LSPAR RSPAR",
 /*  32 */ "jsonarray ::= LSPAR exlist RSPAR",
 /*  33 */ "exlist ::= ex",
 /*  34 */ "exlist ::= exlist COMMA ex",
 /*  35 */ "ex ::= COUNT ex",
 /*  36 */ "ex ::= FIRST ex",
 /*  37 */ "ex ::= ex WHERE ex",
 /*  38 */ "ex ::= ex IS LESS THAN ex",
 /*  39 */ "ex ::= ex IS LESS THAN OR EQUAL ex",
 /*  40 */ "ex ::= ex IS WITHIN ex TO ex",
 /*  41 */ "ex ::= IT",
 /*  42 */ "ex ::= ex OCCURS BEFORE ex",
 /*  43 */ "ex ::= ex OCCURRED AFTER ex",
 /*  44 */ "statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON",
 /*  45 */ "of ::=",
 /*  46 */ "of ::= OF",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
   yypParser->yytos--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 52, 1 },
  { 53, 0 },
  { 53, 2 },
  { 54, 3 },
  { 54, 3 },
  { 54, 7 },
  { 54, 4 },
  { 54, 5 },
  { 54, 6 },
  { 55, 3 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 4 },
  { 55, 1 },
  { 57, 2 },
  { 57, 3 },
  { 58, 1 },
  { 58, 3 },
  { 55, 2 },
  { 55, 2 },
  { 55, 3 },
  { 55, 5 },
  { 55, 7 },
  { 55, 6 },
  { 55, 1 },
  { 55, 4 },
  { 55, 4 },
  { 54, 10 },
  { 56, 0 },
  { 56, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* code ::= statementblock */
#line 290 "grammar.y"
{
	printf (cJSON_Print(yymsp[0].minor.yy0)); 
}
#line 960 "grammar.c"
        break;
      case 1: /* statementblock ::= */
#line 302 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	yymsp[1].minor.yy0 = res;
}
#line 971 "grammar.c"
        break;
      case 2: /* statementblock ::= statementblock statement */
#line 312 "grammar.y"
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( yymsp[-1].minor.yy0, "statements"), yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 979 "grammar.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* statement ::= WRITE ex SEMICOLON */
#line 325 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yymsp[-2].minor.yy0 = res; 
}
#line 990 "grammar.c"
        break;
      case 4: /* statement ::= TRACE ex SEMICOLON */
#line 334 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRACE");
	cJSON_AddStringToObject(res, "line", getLine(yymsp[-2].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yylhsminor.yy0 = res;
}
#line 1001 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* statement ::= IDENTIFIER LSPAR ex RSPAR ASSIGN ex SEMICOLON */
#line 344 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "LIST_ASSIGN");
    cJSON_AddStringToObject(res, "varname", getValue(yymsp[-6].minor.yy0)); 
    cJSON_AddItemToObject(res, "list_index", yymsp[-4].minor.yy0);     
    cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
    yylhsminor.yy0 = res;
}
#line 1014 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 6: /* statement ::= IDENTIFIER ASSIGN ex SEMICOLON */
#line 354 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "VARIABLE_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yylhsminor.yy0 = res;
}
#line 1026 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* statement ::= TIME IDENTIFIER ASSIGN ex SEMICOLON */
#line 368 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-4].minor.yy0 = res;
}
#line 1038 "grammar.c"
        break;
      case 8: /* statement ::= TIME OF IDENTIFIER ASSIGN ex SEMICOLON */
#line 378 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_OF");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-5].minor.yy0 = res;
}
#line 1049 "grammar.c"
        break;
      case 9: /* ex ::= LPAR ex RPAR */
#line 393 "grammar.y"
{ 
	yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; 
}
#line 1056 "grammar.c"
        break;
      case 10: /* ex ::= NUMTOKEN */
#line 399 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1066 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* ex ::= TIMETOKEN */
#line 407 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMETOKEN");
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1077 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 12: /* ex ::= STRTOKEN */
#line 415 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1088 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* ex ::= IDENTIFIER */
#line 428 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(yymsp[0].minor.yy0)); 
	cJSON_AddStringToObject(res, "line", getLine(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1100 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* ex ::= TRUE */
#line 437 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	yymsp[0].minor.yy0 = res;
}
#line 1110 "grammar.c"
        break;
      case 15: /* ex ::= FALSE */
#line 444 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	yymsp[0].minor.yy0 = res;
}
#line 1119 "grammar.c"
        break;
      case 16: /* ex ::= CURRENTTIME */
#line 451 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "CURRENT_TIME");
	yymsp[0].minor.yy0 = res;
}
#line 1128 "grammar.c"
        break;
      case 17: /* ex ::= NOW */
#line 458 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	yymsp[0].minor.yy0 = res;
}
#line 1137 "grammar.c"
        break;
      case 18: /* ex ::= NULL1 */
#line 466 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	yymsp[0].minor.yy0 = res;
}
#line 1146 "grammar.c"
        break;
      case 19: /* ex ::= TIME of ex */
#line 477 "grammar.y"
{ yymsp[-2].minor.yy0 = unary ("TIME_READ", yymsp[0].minor.yy0); }
#line 1151 "grammar.c"
        break;
      case 20: /* ex ::= ex PLUS ex */
#line 480 "grammar.y"
{yylhsminor.yy0 = binary ("PLUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1156 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* ex ::= ex MINUS ex */
#line 483 "grammar.y"
{yylhsminor.yy0 = binary ("MINUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1162 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* ex ::= ex TIMES ex */
#line 486 "grammar.y"
{yylhsminor.yy0 = binary ("TIMES", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1168 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* ex ::= ex DIVIDE ex */
#line 489 "grammar.y"
{yylhsminor.yy0 = binary ("DIVIDE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1174 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 24: /* ex ::= ex POWER ex */
#line 492 "grammar.y"
{yylhsminor.yy0 = binary ("POWER", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1180 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* ex ::= ex AMPERSAND ex */
#line 496 "grammar.y"
{ yylhsminor.yy0 = binary ("STR_CONCAT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1186 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* ex ::= ex IS NUMBER */
#line 500 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NUMBER", yymsp[-2].minor.yy0); }
#line 1192 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* ex ::= ex IS STRING */
#line 503 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_STRING", yymsp[-2].minor.yy0); }
#line 1198 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* ex ::= ex IS LIST */
#line 506 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_LIST", yymsp[-2].minor.yy0); }
#line 1204 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* ex ::= ex IS NOT LIST */
#line 509 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NOT_LIST", yymsp[-3].minor.yy0); }
#line 1210 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* ex ::= jsonarray */
#line 519 "grammar.y"
{ yylhsminor.yy0 = yymsp[0].minor.yy0; }
#line 1216 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* jsonarray ::= LSPAR RSPAR */
#line 524 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "EMPTY_LIST");
	yymsp[-1].minor.yy0 = res;
}
#line 1226 "grammar.c"
        break;
      case 32: /* jsonarray ::= LSPAR exlist RSPAR */
#line 531 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "LIST");
	cJSON_AddItemToObject(res, "items", yymsp[-1].minor.yy0);
	cJSON_AddNumberToObject(res,"code", 1016);
	yymsp[-2].minor.yy0 = res;
}
#line 1237 "grammar.c"
        break;
      case 33: /* exlist ::= ex */
#line 540 "grammar.y"
{
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, yymsp[0].minor.yy0);
	yylhsminor.yy0 = arg;
}
#line 1246 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* exlist ::= exlist COMMA ex */
#line 547 "grammar.y"
{
	cJSON_AddItemToArray(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-2].minor.yy0;
}
#line 1255 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* ex ::= COUNT ex */
#line 553 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("COUNT", yymsp[0].minor.yy0); }
#line 1261 "grammar.c"
        break;
      case 36: /* ex ::= FIRST ex */
#line 556 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("FIRST", yymsp[0].minor.yy0); }
#line 1266 "grammar.c"
        break;
      case 37: /* ex ::= ex WHERE ex */
#line 562 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON *arg = cJSON_CreateArray();
	cJSON *opt = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, yymsp[-2].minor.yy0);
	cJSON_AddStringToObject(res, "type", "WHERE");
	cJSON_AddItemToObject(res, "arg", arg);
	cJSON_AddItemToArray(opt, yymsp[0].minor.yy0);
	cJSON_AddItemToObject(res, "opt", opt);
	yylhsminor.yy0 = res;
}
#line 1281 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* ex ::= ex IS LESS THAN ex */
#line 578 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_THAN", yymsp[-4].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1289 "grammar.c"
  yymsp[-4].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* ex ::= ex IS LESS THAN OR EQUAL ex */
#line 583 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_OR_EQUAL", yymsp[-6].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1297 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* ex ::= ex IS WITHIN ex TO ex */
#line 588 "grammar.y"
{ 
	yylhsminor.yy0 = ternary ("IS_WITHIN", yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1305 "grammar.c"
  yymsp[-5].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* ex ::= IT */
#line 597 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IT");
	yymsp[0].minor.yy0 = res;
}
#line 1315 "grammar.c"
        break;
      case 42: /* ex ::= ex OCCURS BEFORE ex */
#line 607 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURS_BEFORE", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1320 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* ex ::= ex OCCURRED AFTER ex */
#line 612 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURRED_AFTER", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1326 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 44: /* statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON */
#line 616 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FOR_LOOP");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-8].minor.yy0));
	cJSON_AddItemToObject(res, "expression", yymsp[-6].minor.yy0);
	cJSON_AddItemToObject(res, "expression2", yymsp[-4].minor.yy0);

	cJSON_AddItemToObject(res, "statements", yymsp[-2].minor.yy0);
	yymsp[-9].minor.yy0 = res;
}
#line 1341 "grammar.c"
        break;
      default:
      /* (45) of ::= */ yytestcase(yyruleno==45);
      /* (46) of ::= OF */ yytestcase(yyruleno==46);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 255 "grammar.y"

  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
#line 1406 "grammar.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= &yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
#line 25 "grammar.y"


using namespace std;
typedef struct {char *value; int line;} token;

token* create_token (char *value, int line) {
	token *t = (token*) malloc (sizeof (token));
	t->value = strdup (value);
	t->line = line;
	return t;
}

const char * getValue (cJSON* token) {
	return cJSON_GetObjectItem (token, "value")->valuestring;
}


const char * getLine (cJSON* token) {
	return cJSON_GetObjectItem (token, "line")->valuestring;
}


int main(int argc, char* argv[]) {
	char *result;
	std::string line;
	std::string input = "";
	while (std::getline(std::cin, line)) {
      input += line + "\n";
    }
	if (input == "") {
		cout << "Empty input";
		exit(0);
	}
	
	cJSON *root = cJSON_Parse(input.c_str());
	
	if (!root) {
		cout << "JSON invalid\n";
		exit(0);
	}
	
	void* pParser = ParseAlloc (malloc);
	int num = cJSON_GetArraySize (root);
	
	for (int i = 0; i < num; i++ ) {
	
		// Knoten im Token-Stream auslesen
		cJSON *node = cJSON_GetArrayItem(root,i);
		
		char *line = cJSON_GetArrayItem(node,0)->valuestring;
		char *type = cJSON_GetArrayItem(node,1)->valuestring;
		char *value = cJSON_GetArrayItem(node,2)->valuestring;
		
		cJSON *tok = cJSON_CreateObject();
		cJSON_AddStringToObject(tok, "value", value);
		cJSON_AddStringToObject(tok, "line", line);

		linenumber = line;
		curtoken = value;
		curtype = type;
		// THE und Kommentare werden ueberlesen
		if (strcmp(type, "THE") == 0) continue;
		if (strcmp(type, "COMMENT") == 0) continue;
		if (strcmp(type, "MCOMMENT") == 0) continue;
		
		int tokenid = get_token_id (type);
		Parse (pParser, tokenid, tok);
		
	}
	Parse (pParser, 0, 0);
    ParseFree(pParser, free );
}




/////////////////////// 
/////////////////////// 
// TOKENS
///////////////////////
/////////////////////// 

int get_token_id (char *token) {
	if (strcmp(token, "DIVIDE") == 0) return DIVIDE;
	if (strcmp(token, "IDENTIFIER") == 0) return IDENTIFIER;
	if (strcmp(token, "LPAR") == 0) return LPAR; 
	if (strcmp(token, "RPAR") == 0) return RPAR;
	if (strcmp(token, "MINUS") == 0) return MINUS;
	if (strcmp(token, "NUMTOKEN") == 0) return NUMTOKEN;
	if (strcmp(token, "PLUS") == 0) return PLUS;
	if (strcmp(token, "POWER") == 0) return POWER;
	if (strcmp(token, "SEMICOLON") == 0) return SEMICOLON;
	if (strcmp(token, "STRTOKEN") == 0) return STRTOKEN;
	if (strcmp(token, "WRITE") == 0) return WRITE;

	if (strcmp(token, "TRACE") == 0) return TRACE; 
	if (strcmp(token, "AMPERSAND") == 0) return AMPERSAND;
	if (strcmp(token, "ASSIGN") == 0) return ASSIGN;
	if (strcmp(token, "TRUE") == 0) return TRUE;
	if (strcmp(token, "FALSE") == 0) return FALSE;
	if (strcmp(token, "NULL") == 0) return NULL1;

	if (strcmp(token, "RSPAR") == 0) return RSPAR;
	if (strcmp(token, "LSPAR") == 0) return LSPAR;
	if (strcmp(token, "COMMA") == 0) return COMMA;

	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "NUMBER") == 0) return NUMBER;
	if (strcmp(token, "STRING") == 0) return STRING;

	if (strcmp(token, "LIST") == 0) return LIST;
	if (strcmp(token, "COUNT") == 0) return COUNT;
	if (strcmp(token, "FIRST") == 0) return FIRST;

	if (strcmp(token, "NOT") == 0) return NOT;

	if (strcmp(token, "TIMETOKEN") == 0) return TIMETOKEN;
	if (strcmp(token, "TIME") == 0) return TIME;
	if (strcmp(token, "TIMES") == 0) return TIMES;
	if (strcmp(token, "CURRENTTIME") == 0) return CURRENTTIME;

	if (strcmp(token, "NOW") == 0) return NOW;

	if (strcmp(token, "WHERE") == 0) return WHERE;
	if (strcmp(token, "LESS") == 0) return LESS;

	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "IT") == 0) return IT;

	if (strcmp(token, "THAN") == 0) return THAN;

	if (strcmp(token, "EQUAL") == 0) return EQUAL;
	if (strcmp(token, "OR") == 0) return OR;

	if (strcmp(token, "TO") == 0) return TO;
	if (strcmp(token, "WITHIN") == 0) return WITHIN;

	if (strcmp(token, "OF") == 0) return OF;

	if (strcmp(token, "OCCURS") == 0) return OCCURS;
	if (strcmp(token, "OCCURRED") == 0) return OCCURRED;
	if (strcmp(token, "AFTER") == 0) return AFTER;


	if (strcmp(token, "BEFORE") == 0) return BEFORE;

	if (strcmp(token, "THEY") == 0) return IT;


	if (strcmp(token, "FOR") == 0) return FOR;
	if (strcmp(token, "IN") == 0) return IN;
	if (strcmp(token, "DO") == 0) return DO;
	if (strcmp(token, "ENDDO") == 0) return ENDDO;

	if (strcmp(token, "FORRANGE") == 0) return FORRANGE;








	


















	
	printf ("{\"error\" : true, \"message\": \"UNKNOWN TOKEN TYPE %s\"}\n", token);
	exit(0);
} 
  


cJSON* unary (char* fname, cJSON* a) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a);
	cJSON_AddStringToObject(res, "type", fname);
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
} 



cJSON* binary (char *fname, cJSON *a, cJSON *b) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



cJSON* ternary (char *fname, cJSON *a, cJSON *b, cJSON *c) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b); 
	cJSON_AddItemToArray(arg, c);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



#line 1838 "grammar.c"
