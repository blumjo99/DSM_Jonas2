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
#define YYNOCODE 68
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
#define YYNSTATE             92
#define YYNRULE              54
#define YY_MAX_SHIFT         91
#define YY_MIN_SHIFTREDUCE   124
#define YY_MAX_SHIFTREDUCE   177
#define YY_MIN_REDUCE        178
#define YY_MAX_REDUCE        231
#define YY_ERROR_ACTION      232
#define YY_ACCEPT_ACTION     233
#define YY_NO_ACTION         234
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
#define YY_ACTTAB_COUNT (467)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    37,   56,  126,  154,   75,  167,    1,   67,   25,  154,
 /*    10 */   137,  155,  233,   36,   30,  177,  134,  135,  136,  138,
 /*    20 */   139,  140,  141,  142,   28,   69,   37,  154,   24,   19,
 /*    30 */    18,  167,    1,   41,  156,  154,  137,  175,   34,   89,
 /*    40 */    30,   88,  134,  135,  136,  138,  139,  140,  141,  142,
 /*    50 */    41,   20,  154,   37,  171,   19,   18,   85,  167,    1,
 /*    60 */    42,   83,  154,  137,   82,   33,   43,   30,  154,  134,
 /*    70 */   135,  136,  138,  139,  140,  141,  142,  174,   35,  173,
 /*    80 */    37,  170,   19,   18,   87,  167,    1,   44,    7,  154,
 /*    90 */   137,   45,   79,  154,   30,    8,  134,  135,  136,  138,
 /*   100 */   139,  140,  141,  142,   23,   81,   31,   29,    9,   19,
 /*   110 */    18,  126,   63,   27,   26,   25,   23,  172,   31,   29,
 /*   120 */    23,   11,   31,   29,   63,   27,   26,   25,   63,   27,
 /*   130 */    26,   25,   46,   16,  154,   24,   17,   12,   63,   27,
 /*   140 */    26,   25,   47,   10,  154,    4,   89,   24,   88,   22,
 /*   150 */    21,   24,   48,   49,  154,  154,   39,    5,   89,  153,
 /*   160 */    88,   24,   89,  180,   88,   40,  180,   23,    6,   31,
 /*   170 */    29,   65,   89,  154,   88,   63,   27,   26,   25,  180,
 /*   180 */   180,   50,  132,  154,   23,  180,   31,   29,   27,   26,
 /*   190 */    25,  180,   63,   27,   26,   25,  180,  180,   24,  131,
 /*   200 */    66,   68,  154,  154,   53,  180,  154,   73,  180,   89,
 /*   210 */    24,   88,  180,  180,   32,   24,   13,   74,   54,  180,
 /*   220 */   154,   89,   55,   88,  154,  180,   89,  180,   88,   57,
 /*   230 */    23,  154,   31,   29,   58,   59,  154,  154,   63,   27,
 /*   240 */    26,   25,  180,  180,  180,  130,   80,   23,  180,   31,
 /*   250 */    29,    3,   60,   76,  154,   63,   27,   26,   25,  180,
 /*   260 */   180,   24,  129,   70,   71,  154,  154,  180,  180,   72,
 /*   270 */    73,  154,   89,   64,   88,  154,  180,   32,   24,   13,
 /*   280 */    74,  180,   62,   51,  154,  154,   61,  180,  154,   89,
 /*   290 */   180,   88,   52,   23,  154,   31,   29,   23,  180,   31,
 /*   300 */    29,   63,   27,   26,   25,   63,   27,   26,   25,   80,
 /*   310 */   180,   84,  128,  180,    3,  180,   77,   38,    2,  180,
 /*   320 */    73,  180,  180,  180,   24,  180,  180,   32,   24,   13,
 /*   330 */    74,  180,  180,  180,  180,   89,  180,   88,  180,   89,
 /*   340 */   180,   88,  180,   23,  180,   31,   29,   23,  180,   31,
 /*   350 */    29,   63,   27,   26,   25,   63,   27,   26,   25,   80,
 /*   360 */   180,  180,  180,   78,    3,  180,  180,   91,  180,  133,
 /*   370 */   180,   90,  180,  180,   24,  180,  180,  180,   24,  180,
 /*   380 */   180,  180,  180,  180,   14,   89,  180,   88,  180,   89,
 /*   390 */   180,   88,  180,   23,  180,   31,   29,  180,  150,  151,
 /*   400 */   152,   63,   27,   26,   25,   86,   15,  180,  127,  180,
 /*   410 */    23,  180,   31,   29,  178,  180,  180,  180,   63,   27,
 /*   420 */    26,   25,   73,  180,   24,  180,  180,  180,  180,   32,
 /*   430 */   180,   13,   74,  180,  180,   89,  180,   88,  180,  180,
 /*   440 */   180,   24,  180,  180,  180,  180,  180,  180,  180,  180,
 /*   450 */   180,  180,   89,  180,   88,  180,  180,  180,  180,  180,
 /*   460 */   180,   80,  180,  180,  180,  180,    3,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,   61,   60,   63,   64,   13,   14,   61,   12,   63,
 /*    10 */    18,   19,   58,   59,   22,   21,   24,   25,   26,   27,
 /*    20 */    28,   29,   30,   31,   62,   61,    8,   63,   32,   37,
 /*    30 */    38,   13,   14,   61,   19,   63,   18,   65,   59,   43,
 /*    40 */    22,   45,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    50 */    61,   36,   63,    8,   65,   37,   38,   39,   13,   14,
 /*    60 */    61,   18,   63,   18,   21,   59,   61,   22,   63,   24,
 /*    70 */    25,   26,   27,   28,   29,   30,   31,   16,   59,   16,
 /*    80 */     8,   16,   37,   38,   39,   13,   14,   61,   48,   63,
 /*    90 */    18,   61,   18,   63,   22,   20,   24,   25,   26,   27,
 /*   100 */    28,   29,   30,   31,    1,   18,    3,    4,   20,   37,
 /*   110 */    38,   60,    9,   10,   11,   12,    1,   66,    3,    4,
 /*   120 */     1,   20,    3,    4,    9,   10,   11,   12,    9,   10,
 /*   130 */    11,   12,   61,    5,   63,   32,    5,   14,    9,   10,
 /*   140 */    11,   12,   61,   20,   63,    7,   43,   32,   45,   44,
 /*   150 */    46,   32,   61,   61,   63,   63,   53,    7,   43,   35,
 /*   160 */    45,   32,   43,   67,   45,   50,   67,    1,   49,    3,
 /*   170 */     4,   61,   43,   63,   45,    9,   10,   11,   12,   67,
 /*   180 */    67,   61,   16,   63,    1,   67,    3,    4,   10,   11,
 /*   190 */    12,   67,    9,   10,   11,   12,   67,   67,   32,   16,
 /*   200 */    61,   61,   63,   63,   61,   67,   63,    8,   67,   43,
 /*   210 */    32,   45,   67,   67,   15,   32,   17,   18,   61,   67,
 /*   220 */    63,   43,   61,   45,   63,   67,   43,   67,   45,   61,
 /*   230 */     1,   63,    3,    4,   61,   61,   63,   63,    9,   10,
 /*   240 */    11,   12,   67,   67,   67,   16,   47,    1,   67,    3,
 /*   250 */     4,   52,   61,   54,   63,    9,   10,   11,   12,   67,
 /*   260 */    67,   32,   16,   61,   61,   63,   63,   67,   67,   61,
 /*   270 */     8,   63,   43,   61,   45,   63,   67,   15,   32,   17,
 /*   280 */    18,   67,   61,   61,   63,   63,   61,   67,   63,   43,
 /*   290 */    67,   45,   61,    1,   63,    3,    4,    1,   67,    3,
 /*   300 */     4,    9,   10,   11,   12,    9,   10,   11,   12,   47,
 /*   310 */    67,   19,   16,   67,   52,   67,   54,   55,   56,   67,
 /*   320 */     8,   67,   67,   67,   32,   67,   67,   15,   32,   17,
 /*   330 */    18,   67,   67,   67,   67,   43,   67,   45,   67,   43,
 /*   340 */    67,   45,   67,    1,   67,    3,    4,    1,   67,    3,
 /*   350 */     4,    9,   10,   11,   12,    9,   10,   11,   12,   47,
 /*   360 */    67,   67,   67,   51,   52,   67,   67,    2,   67,   23,
 /*   370 */    67,    6,   67,   67,   32,   67,   67,   67,   32,   67,
 /*   380 */    67,   67,   67,   67,   42,   43,   67,   45,   67,   43,
 /*   390 */    67,   45,   67,    1,   67,    3,    4,   67,   33,   34,
 /*   400 */    35,    9,   10,   11,   12,   40,   41,   67,   16,   67,
 /*   410 */     1,   67,    3,    4,    0,   67,   67,   67,    9,   10,
 /*   420 */    11,   12,    8,   67,   32,   67,   67,   67,   67,   15,
 /*   430 */    67,   17,   18,   67,   67,   43,   67,   45,   67,   67,
 /*   440 */    67,   32,   67,   67,   67,   67,   67,   67,   67,   67,
 /*   450 */    67,   67,   43,   67,   45,   67,   67,   67,   67,   67,
 /*   460 */    67,   47,   67,   67,   67,   67,   52,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_COUNT (91)
#define YY_SHIFT_MIN   (-8)
#define YY_SHIFT_MAX   (414)
static const short yy_shift_ofst[] = {
 /*     0 */    -9,   -8,   72,   72,   18,   45,   72,   72,   72,   72,
 /*    10 */    72,   72,   72,   72,   72,   72,   72,   72,   72,   72,
 /*    20 */    72,   72,   72,   72,   72,   72,   72,   72,   72,   72,
 /*    30 */    72,   72,   72,  262,  199,  312,  414,   -6,   -9,   -9,
 /*    40 */    -9,  103,  115,  119,  166,  183,  229,  246,  292,  296,
 /*    50 */   342,  346,  392,  409,  409,  409,  409,  409,  409,  409,
 /*    60 */   409,  129,  129,  365,  129,  178,  178,  178,  178,  178,
 /*    70 */    -4,   -4,   -4,   43,  123,   15,   61,   63,   65,   40,
 /*    80 */    74,   75,   87,   88,  101,  128,  138,  131,  104,  105,
 /*    90 */   150,  124,
};
#define YY_REDUCE_USE_DFLT (-61)
#define YY_REDUCE_COUNT (40)
#define YY_REDUCE_MIN   (-60)
#define YY_REDUCE_MAX   (231)
static const short yy_reduce_ofst[] = {
 /*     0 */   -46,  -60,  -28,  -11,  -54,  -36,   -1,    5,   26,   30,
 /*    10 */    71,   81,   91,   92,  110,  120,  139,  140,  143,  157,
 /*    20 */   161,  168,  173,  174,  191,  202,  203,  208,  212,  221,
 /*    30 */   222,  225,  231,   51,  -58,  -58,  -58,  -38,  -21,    6,
 /*    40 */    19,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   179,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    10 */   232,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    20 */   232,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    30 */   232,  232,  232,  232,  232,  232,  232,  230,  179,  179,
 /*    40 */   179,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    50 */   232,  232,  232,  214,  213,  212,  211,  223,  222,  215,
 /*    60 */   203,  198,  199,  232,  197,  220,  219,  218,  217,  216,
 /*    70 */   202,  201,  200,  232,  232,  232,  232,  232,  232,  232,
 /*    80 */   232,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    90 */   232,  232,
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
  "GREATER",       "WITHIN",        "TO",            "OCCURS",      
  "BEFORE",        "OCCURRED",      "AFTER",         "FOR",         
  "IN",            "FORRANGE",      "DO",            "ENDDO",       
  "IF",            "THEN",          "ENDIF",         "ELSE",        
  "ELSEIF",        "error",         "code",          "statementblock",
  "statement",     "ex",            "of",            "jsonarray",   
  "exlist",        "if_then_else",  "elseif",      
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
 /*  40 */ "ex ::= ex IS GREATER THAN ex",
 /*  41 */ "ex ::= ex IS GREATER THAN OR EQUAL ex",
 /*  42 */ "ex ::= ex IS WITHIN ex TO ex",
 /*  43 */ "ex ::= IT",
 /*  44 */ "ex ::= ex OCCURS BEFORE ex",
 /*  45 */ "ex ::= ex OCCURRED AFTER ex",
 /*  46 */ "statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON",
 /*  47 */ "statement ::= IF if_then_else",
 /*  48 */ "if_then_else ::= ex THEN statementblock elseif",
 /*  49 */ "elseif ::= ENDIF SEMICOLON",
 /*  50 */ "elseif ::= ELSE statementblock ENDIF SEMICOLON",
 /*  51 */ "elseif ::= ELSEIF if_then_else",
 /*  52 */ "of ::=",
 /*  53 */ "of ::= OF",
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
  { 58, 1 },
  { 59, 0 },
  { 59, 2 },
  { 60, 3 },
  { 60, 3 },
  { 60, 7 },
  { 60, 4 },
  { 60, 5 },
  { 60, 6 },
  { 61, 3 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 4 },
  { 61, 1 },
  { 63, 2 },
  { 63, 3 },
  { 64, 1 },
  { 64, 3 },
  { 61, 2 },
  { 61, 2 },
  { 61, 3 },
  { 61, 5 },
  { 61, 7 },
  { 61, 5 },
  { 61, 7 },
  { 61, 6 },
  { 61, 1 },
  { 61, 4 },
  { 61, 4 },
  { 60, 10 },
  { 60, 2 },
  { 65, 4 },
  { 66, 2 },
  { 66, 4 },
  { 66, 2 },
  { 62, 0 },
  { 62, 1 },
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
#line 300 "grammar.y"
{
	printf (cJSON_Print(yymsp[0].minor.yy0)); 
}
#line 993 "grammar.c"
        break;
      case 1: /* statementblock ::= */
#line 312 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	yymsp[1].minor.yy0 = res;
}
#line 1004 "grammar.c"
        break;
      case 2: /* statementblock ::= statementblock statement */
#line 322 "grammar.y"
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( yymsp[-1].minor.yy0, "statements"), yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 1012 "grammar.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* statement ::= WRITE ex SEMICOLON */
#line 335 "grammar.y"
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
	yymsp[-2].minor.yy0 = res; 
}
#line 1023 "grammar.c"
        break;
      case 4: /* statement ::= TRACE ex SEMICOLON */
#line 344 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRACE");
	cJSON_AddStringToObject(res, "line", getLine(yymsp[-2].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yylhsminor.yy0 = res;
}
#line 1034 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* statement ::= IDENTIFIER LSPAR ex RSPAR ASSIGN ex SEMICOLON */
#line 354 "grammar.y"
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "LIST_ASSIGN");
    cJSON_AddStringToObject(res, "varname", getValue(yymsp[-6].minor.yy0)); 
    cJSON_AddItemToObject(res, "list_index", yymsp[-4].minor.yy0);     
    cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0); 
    yylhsminor.yy0 = res;
}
#line 1047 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 6: /* statement ::= IDENTIFIER ASSIGN ex SEMICOLON */
#line 364 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "VARIABLE_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yylhsminor.yy0 = res;
}
#line 1059 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* statement ::= TIME IDENTIFIER ASSIGN ex SEMICOLON */
#line 378 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-4].minor.yy0 = res;
}
#line 1071 "grammar.c"
        break;
      case 8: /* statement ::= TIME OF IDENTIFIER ASSIGN ex SEMICOLON */
#line 388 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_OF");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-5].minor.yy0 = res;
}
#line 1082 "grammar.c"
        break;
      case 9: /* ex ::= LPAR ex RPAR */
#line 403 "grammar.y"
{ 
	yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; 
}
#line 1089 "grammar.c"
        break;
      case 10: /* ex ::= NUMTOKEN */
#line 409 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1099 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* ex ::= TIMETOKEN */
#line 417 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMETOKEN");
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1110 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 12: /* ex ::= STRTOKEN */
#line 425 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1121 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* ex ::= IDENTIFIER */
#line 438 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(yymsp[0].minor.yy0)); 
	cJSON_AddStringToObject(res, "line", getLine(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1133 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* ex ::= TRUE */
#line 447 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	yymsp[0].minor.yy0 = res;
}
#line 1143 "grammar.c"
        break;
      case 15: /* ex ::= FALSE */
#line 454 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	yymsp[0].minor.yy0 = res;
}
#line 1152 "grammar.c"
        break;
      case 16: /* ex ::= CURRENTTIME */
#line 461 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "CURRENT_TIME");
	yymsp[0].minor.yy0 = res;
}
#line 1161 "grammar.c"
        break;
      case 17: /* ex ::= NOW */
#line 468 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	yymsp[0].minor.yy0 = res;
}
#line 1170 "grammar.c"
        break;
      case 18: /* ex ::= NULL1 */
#line 476 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	yymsp[0].minor.yy0 = res;
}
#line 1179 "grammar.c"
        break;
      case 19: /* ex ::= TIME of ex */
#line 487 "grammar.y"
{ yymsp[-2].minor.yy0 = unary ("TIME_READ", yymsp[0].minor.yy0); }
#line 1184 "grammar.c"
        break;
      case 20: /* ex ::= ex PLUS ex */
#line 490 "grammar.y"
{yylhsminor.yy0 = binary ("PLUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1189 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* ex ::= ex MINUS ex */
#line 493 "grammar.y"
{yylhsminor.yy0 = binary ("MINUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1195 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* ex ::= ex TIMES ex */
#line 496 "grammar.y"
{yylhsminor.yy0 = binary ("TIMES", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1201 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* ex ::= ex DIVIDE ex */
#line 499 "grammar.y"
{yylhsminor.yy0 = binary ("DIVIDE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1207 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 24: /* ex ::= ex POWER ex */
#line 502 "grammar.y"
{yylhsminor.yy0 = binary ("POWER", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1213 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* ex ::= ex AMPERSAND ex */
#line 506 "grammar.y"
{ yylhsminor.yy0 = binary ("STR_CONCAT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1219 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* ex ::= ex IS NUMBER */
#line 510 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NUMBER", yymsp[-2].minor.yy0); }
#line 1225 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* ex ::= ex IS STRING */
#line 513 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_STRING", yymsp[-2].minor.yy0); }
#line 1231 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* ex ::= ex IS LIST */
#line 516 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_LIST", yymsp[-2].minor.yy0); }
#line 1237 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* ex ::= ex IS NOT LIST */
#line 519 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NOT_LIST", yymsp[-3].minor.yy0); }
#line 1243 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* ex ::= jsonarray */
#line 529 "grammar.y"
{ yylhsminor.yy0 = yymsp[0].minor.yy0; }
#line 1249 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* jsonarray ::= LSPAR RSPAR */
#line 534 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "EMPTY_LIST");
	yymsp[-1].minor.yy0 = res;
}
#line 1259 "grammar.c"
        break;
      case 32: /* jsonarray ::= LSPAR exlist RSPAR */
#line 541 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "LIST");
	cJSON_AddItemToObject(res, "items", yymsp[-1].minor.yy0);
	cJSON_AddNumberToObject(res,"code", 1016);
	yymsp[-2].minor.yy0 = res;
}
#line 1270 "grammar.c"
        break;
      case 33: /* exlist ::= ex */
#line 550 "grammar.y"
{
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, yymsp[0].minor.yy0);
	yylhsminor.yy0 = arg;
}
#line 1279 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* exlist ::= exlist COMMA ex */
#line 557 "grammar.y"
{
	cJSON_AddItemToArray(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-2].minor.yy0;
}
#line 1288 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* ex ::= COUNT ex */
#line 563 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("COUNT", yymsp[0].minor.yy0); }
#line 1294 "grammar.c"
        break;
      case 36: /* ex ::= FIRST ex */
#line 566 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("FIRST", yymsp[0].minor.yy0); }
#line 1299 "grammar.c"
        break;
      case 37: /* ex ::= ex WHERE ex */
#line 572 "grammar.y"
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
#line 1314 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* ex ::= ex IS LESS THAN ex */
#line 588 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_THAN", yymsp[-4].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1322 "grammar.c"
  yymsp[-4].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* ex ::= ex IS LESS THAN OR EQUAL ex */
#line 593 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_OR_EQUAL", yymsp[-6].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1330 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* ex ::= ex IS GREATER THAN ex */
#line 598 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("GREATER_THAN", yymsp[-4].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1338 "grammar.c"
  yymsp[-4].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* ex ::= ex IS GREATER THAN OR EQUAL ex */
#line 603 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("GREATER_OR_EQUAL", yymsp[-6].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1346 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* ex ::= ex IS WITHIN ex TO ex */
#line 608 "grammar.y"
{ 
	yylhsminor.yy0 = ternary ("IS_WITHIN", yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1354 "grammar.c"
  yymsp[-5].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* ex ::= IT */
#line 617 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IT");
	yymsp[0].minor.yy0 = res;
}
#line 1364 "grammar.c"
        break;
      case 44: /* ex ::= ex OCCURS BEFORE ex */
#line 627 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURS_BEFORE", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1369 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* ex ::= ex OCCURRED AFTER ex */
#line 632 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURRED_AFTER", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1375 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON */
#line 636 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FOR_LOOP");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-8].minor.yy0));
	cJSON_AddItemToObject(res, "expression", yymsp[-6].minor.yy0);
	cJSON_AddItemToObject(res, "expression2", yymsp[-4].minor.yy0);

	cJSON_AddItemToObject(res, "statements", yymsp[-2].minor.yy0);
	yymsp[-9].minor.yy0 = res;
}
#line 1390 "grammar.c"
        break;
      case 47: /* statement ::= IF if_then_else */
      case 51: /* elseif ::= ELSEIF if_then_else */ yytestcase(yyruleno==51);
#line 651 "grammar.y"
{ yymsp[-1].minor.yy0 = yymsp[0].minor.yy0; }
#line 1396 "grammar.c"
        break;
      case 48: /* if_then_else ::= ex THEN statementblock elseif */
#line 654 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IF");
	cJSON_AddItemToObject(res, "condition", yymsp[-3].minor.yy0);
	cJSON_AddItemToObject(res, "thenbranch", (yymsp[-1].minor.yy0));
	cJSON_AddItemToObject(res, "elsebranch", (yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1408 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 49: /* elseif ::= ENDIF SEMICOLON */
#line 664 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENT_BLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg);
	yymsp[-1].minor.yy0 = res;
}
#line 1420 "grammar.c"
        break;
      case 50: /* elseif ::= ELSE statementblock ENDIF SEMICOLON */
#line 673 "grammar.y"
{ yymsp[-3].minor.yy0 = yymsp[-2].minor.yy0; }
#line 1425 "grammar.c"
        break;
      default:
      /* (52) of ::= */ yytestcase(yyruleno==52);
      /* (53) of ::= OF */ yytestcase(yyruleno==53);
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
#line 265 "grammar.y"

  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
#line 1490 "grammar.c"
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
	if (strcmp(token, "GREATER") == 0) return GREATER;


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

	if (strcmp(token, "IF") == 0) return IF;
	if (strcmp(token, "THEN") == 0) return THEN;
	if (strcmp(token, "ELSEIF") == 0) return ELSEIF;
	if (strcmp(token, "ELSE") == 0) return ELSE;
	if (strcmp(token, "ENDIF") == 0) return ENDIF;











	


















	
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



#line 1932 "grammar.c"
