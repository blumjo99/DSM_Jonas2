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
#define YY_ACTTAB_COUNT (510)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    37,  233,   36,   56,  126,  173,    1,  160,   75,  126,
 /*    10 */   142,  161,  135,   26,   41,  138,   41,  134,  160,   67,
 /*    20 */   160,  177,   34,  160,  139,  140,  141,  143,  144,  145,
 /*    30 */   146,  147,   83,   30,   69,   82,   21,   37,  160,   19,
 /*    40 */    18,   25,  173,    1,   42,   43,   33,  142,  160,  160,
 /*    50 */    62,   28,   27,   26,   89,   44,   88,   35,  137,  160,
 /*    60 */   136,  139,  140,  141,  143,  144,  145,  146,  147,   12,
 /*    70 */   133,    7,    8,   21,   37,   10,   19,   18,   85,  173,
 /*    80 */     1,   25,   45,   91,  142,   46,  160,   90,   47,  160,
 /*    90 */    79,   48,  160,   49,   89,  160,   88,  160,  139,  140,
 /*   100 */   141,  143,  144,  145,  146,  147,  162,   81,    9,   16,
 /*   110 */    21,   37,   11,   19,   18,   87,  173,    1,    4,   17,
 /*   120 */    22,  142,  155,  156,  157,   23,   65,   28,   27,   26,
 /*   130 */   160,   86,   15,   20,  158,  139,  140,  141,  143,  144,
 /*   140 */   145,  146,  147,    5,   50,   66,  180,   21,  160,  160,
 /*   150 */    19,   18,   24,  180,   31,   29,   24,   25,   31,   29,
 /*   160 */    62,   28,   27,   26,   62,   28,   27,   26,  180,  180,
 /*   170 */    89,  180,   88,   68,  180,   73,  180,  160,  180,   39,
 /*   180 */    40,  180,   32,   53,   13,   74,   54,  160,   55,   80,
 /*   190 */   160,   25,  160,  180,    3,   25,   77,   38,    2,   51,
 /*   200 */   180,   57,  180,  160,   89,  160,   88,   58,   89,   59,
 /*   210 */    88,  160,   24,  160,   31,   29,   24,  180,   31,   29,
 /*   220 */    62,   28,   27,   26,   62,   28,   27,   26,  180,   60,
 /*   230 */   180,  132,   70,  160,   71,    6,  160,   24,  160,   31,
 /*   240 */    29,  180,  180,  180,  180,   62,   28,   27,   26,   72,
 /*   250 */    63,   25,  131,  160,  160,   25,   64,   61,   52,  180,
 /*   260 */   160,  160,  160,  180,   89,  180,   88,  180,   89,  180,
 /*   270 */    88,  180,   24,  180,   31,   29,   25,  180,  180,  180,
 /*   280 */    62,   28,   27,   26,  180,  180,  180,  130,  180,   89,
 /*   290 */   180,   88,  180,   24,  180,   31,   29,  180,  180,  180,
 /*   300 */   180,   62,   28,   27,   26,  180,  180,  180,  129,  180,
 /*   310 */    24,   25,   31,   29,  180,  180,  180,  180,   62,   28,
 /*   320 */    27,   26,  180,  180,   89,  180,   88,   73,   84,  180,
 /*   330 */   180,  180,   25,  180,   32,  180,   13,   74,  180,  180,
 /*   340 */   180,   80,  180,  180,  180,   89,    3,   88,   76,   25,
 /*   350 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*   360 */   180,  180,   89,  180,   88,  180,   24,  180,   31,   29,
 /*   370 */   180,  180,  180,  180,   62,   28,   27,   26,  180,  180,
 /*   380 */   180,  128,  180,   24,  180,   31,   29,   24,  180,   31,
 /*   390 */    29,   62,   28,   27,   26,   62,   28,   27,   26,   24,
 /*   400 */   180,   31,   29,  180,  180,   25,  180,   62,   28,   27,
 /*   410 */    26,  180,  180,  180,  127,  180,  180,  180,   89,  180,
 /*   420 */    88,  180,   25,  180,  180,  180,   25,  180,  180,  180,
 /*   430 */   180,  159,  178,  180,   14,   89,  180,   88,   25,   89,
 /*   440 */    73,   88,  180,  180,  180,  180,  180,   32,  180,   13,
 /*   450 */    74,   89,  180,   88,   80,   24,  180,   31,   29,    3,
 /*   460 */    73,  180,  180,   62,   28,   27,   26,   32,  180,   13,
 /*   470 */    74,  180,  180,  180,   80,  180,  180,  180,   78,    3,
 /*   480 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*   490 */   180,  180,  180,  180,   25,  180,  180,  180,  180,  180,
 /*   500 */   180,  180,  180,  180,  180,  180,  180,   89,  180,   88,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,   58,   59,   61,   60,   13,   14,   65,   66,   60,
 /*    10 */    18,   19,   63,   12,   61,   62,   61,   62,   65,   61,
 /*    20 */    65,   21,   59,   65,   32,   33,   34,   35,   36,   37,
 /*    30 */    38,   39,   18,   64,   61,   21,   44,    8,   65,   47,
 /*    40 */    48,   40,   13,   14,   61,   61,   59,   18,   65,   65,
 /*    50 */     9,   10,   11,   12,   53,   61,   55,   59,   16,   65,
 /*    60 */    16,   32,   33,   34,   35,   36,   37,   38,   39,   14,
 /*    70 */    16,   23,   20,   44,    8,   20,   47,   48,   49,   13,
 /*    80 */    14,   40,   61,    2,   18,   61,   65,    6,   61,   65,
 /*    90 */    18,   61,   65,   61,   53,   65,   55,   65,   32,   33,
 /*   100 */    34,   35,   36,   37,   38,   39,   19,   18,   20,    5,
 /*   110 */    44,    8,   20,   47,   48,   49,   13,   14,    7,    5,
 /*   120 */    56,   18,   41,   42,   43,   54,   61,   10,   11,   12,
 /*   130 */    65,   50,   51,   46,   43,   32,   33,   34,   35,   36,
 /*   140 */    37,   38,   39,    7,   61,   61,   67,   44,   65,   65,
 /*   150 */    47,   48,    1,   67,    3,    4,    1,   40,    3,    4,
 /*   160 */     9,   10,   11,   12,    9,   10,   11,   12,   67,   67,
 /*   170 */    53,   67,   55,   61,   67,    8,   67,   65,   67,   28,
 /*   180 */    25,   67,   15,   61,   17,   18,   61,   65,   61,   22,
 /*   190 */    65,   40,   65,   67,   27,   40,   29,   30,   31,   61,
 /*   200 */    67,   61,   67,   65,   53,   65,   55,   61,   53,   61,
 /*   210 */    55,   65,    1,   65,    3,    4,    1,   67,    3,    4,
 /*   220 */     9,   10,   11,   12,    9,   10,   11,   12,   67,   61,
 /*   230 */    67,   16,   61,   65,   61,   24,   65,    1,   65,    3,
 /*   240 */     4,   67,   67,   67,   67,    9,   10,   11,   12,   61,
 /*   250 */    61,   40,   16,   65,   65,   40,   61,   61,   61,   67,
 /*   260 */    65,   65,   65,   67,   53,   67,   55,   67,   53,   67,
 /*   270 */    55,   67,    1,   67,    3,    4,   40,   67,   67,   67,
 /*   280 */     9,   10,   11,   12,   67,   67,   67,   16,   67,   53,
 /*   290 */    67,   55,   67,    1,   67,    3,    4,   67,   67,   67,
 /*   300 */    67,    9,   10,   11,   12,   67,   67,   67,   16,   67,
 /*   310 */     1,   40,    3,    4,   67,   67,   67,   67,    9,   10,
 /*   320 */    11,   12,   67,   67,   53,   67,   55,    8,   19,   67,
 /*   330 */    67,   67,   40,   67,   15,   67,   17,   18,   67,   67,
 /*   340 */    67,   22,   67,   67,   67,   53,   27,   55,   29,   40,
 /*   350 */    67,   67,   67,   67,   67,   67,   67,   67,   67,   67,
 /*   360 */    67,   67,   53,   67,   55,   67,    1,   67,    3,    4,
 /*   370 */    67,   67,   67,   67,    9,   10,   11,   12,   67,   67,
 /*   380 */    67,   16,   67,    1,   67,    3,    4,    1,   67,    3,
 /*   390 */     4,    9,   10,   11,   12,    9,   10,   11,   12,    1,
 /*   400 */    67,    3,    4,   67,   67,   40,   67,    9,   10,   11,
 /*   410 */    12,   67,   67,   67,   16,   67,   67,   67,   53,   67,
 /*   420 */    55,   67,   40,   67,   67,   67,   40,   67,   67,   67,
 /*   430 */    67,   45,    0,   67,   52,   53,   67,   55,   40,   53,
 /*   440 */     8,   55,   67,   67,   67,   67,   67,   15,   67,   17,
 /*   450 */    18,   53,   67,   55,   22,    1,   67,    3,    4,   27,
 /*   460 */     8,   67,   67,    9,   10,   11,   12,   15,   67,   17,
 /*   470 */    18,   67,   67,   67,   22,   67,   67,   67,   26,   27,
 /*   480 */    67,   67,   67,   67,   67,   67,   67,   67,   67,   67,
 /*   490 */    67,   67,   67,   67,   40,   67,   67,   67,   67,   67,
 /*   500 */    67,   67,   67,   67,   67,   67,   67,   53,   67,   55,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_COUNT (91)
#define YY_SHIFT_MIN   (-8)
#define YY_SHIFT_MAX   (454)
static const short yy_shift_ofst[] = {
 /*     0 */    -9,   -8,  103,  103,   29,   66,  103,  103,  103,  103,
 /*    10 */   103,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*    20 */   103,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*    30 */   103,  103,  103,  167,  319,  452,  432,    0,   -9,   -9,
 /*    40 */    -9,  151,  155,  211,  215,  236,  271,  292,  309,  365,
 /*    50 */   382,  386,  398,  454,  454,  454,  454,  454,  454,  454,
 /*    60 */   454,   41,   81,   41,   41,  117,  117,  117,  117,  117,
 /*    70 */     1,    1,    1,   14,   55,   87,   42,   44,   54,   48,
 /*    80 */    72,   52,   89,   88,   92,  104,  111,  114,   64,   71,
 /*    90 */   136,   91,
};
#define YY_REDUCE_USE_DFLT (-59)
#define YY_REDUCE_COUNT (40)
#define YY_REDUCE_MIN   (-58)
#define YY_REDUCE_MAX   (197)
static const short yy_reduce_ofst[] = {
 /*     0 */   -57,  -58,  -47,  -45,  -42,  -27,  -17,  -16,   -6,   21,
 /*    10 */    24,   27,   30,   32,   65,   83,   84,  112,  122,  125,
 /*    20 */   127,  138,  140,  146,  148,  168,  171,  173,  188,  189,
 /*    30 */   195,  196,  197,  -51,  -56,  -56,  -56,  -31,  -37,  -13,
 /*    40 */    -2,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   179,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    10 */   232,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    20 */   232,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    30 */   232,  232,  232,  232,  232,  232,  232,  230,  179,  179,
 /*    40 */   179,  232,  232,  232,  232,  232,  232,  232,  232,  232,
 /*    50 */   232,  232,  232,  220,  219,  218,  217,  229,  228,  221,
 /*    60 */   208,  203,  232,  204,  202,  226,  225,  224,  223,  222,
 /*    70 */   207,  206,  205,  232,  232,  232,  232,  232,  232,  232,
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
  "ASSIGN",        "OF",            "FOR",           "IN",          
  "FORRANGE",      "DO",            "ENDDO",         "IF",          
  "THEN",          "ENDIF",         "ELSE",          "ELSEIF",      
  "NUMTOKEN",      "TIMETOKEN",     "STRTOKEN",      "TRUE",        
  "FALSE",         "CURRENTTIME",   "NOW",           "NULL1",       
  "AMPERSAND",     "NUMBER",        "STRING",        "LIST",        
  "LPAR",          "RPAR",          "COMMA",         "COUNT",       
  "FIRST",         "OR",            "GREATER",       "WITHIN",      
  "TO",            "OCCURS",        "BEFORE",        "OCCURRED",    
  "AFTER",         "error",         "code",          "statementblock",
  "statement",     "ex",            "if_then_else",  "elseif",      
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
 /*   9 */ "statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON",
 /*  10 */ "statement ::= IF if_then_else",
 /*  11 */ "if_then_else ::= ex THEN statementblock elseif",
 /*  12 */ "elseif ::= ENDIF SEMICOLON",
 /*  13 */ "elseif ::= ELSE statementblock ENDIF SEMICOLON",
 /*  14 */ "elseif ::= ELSEIF if_then_else",
 /*  15 */ "ex ::= NUMTOKEN",
 /*  16 */ "ex ::= TIMETOKEN",
 /*  17 */ "ex ::= STRTOKEN",
 /*  18 */ "ex ::= IDENTIFIER",
 /*  19 */ "ex ::= TRUE",
 /*  20 */ "ex ::= FALSE",
 /*  21 */ "ex ::= CURRENTTIME",
 /*  22 */ "ex ::= NOW",
 /*  23 */ "ex ::= NULL1",
 /*  24 */ "ex ::= TIME of ex",
 /*  25 */ "ex ::= ex PLUS ex",
 /*  26 */ "ex ::= ex MINUS ex",
 /*  27 */ "ex ::= ex TIMES ex",
 /*  28 */ "ex ::= ex DIVIDE ex",
 /*  29 */ "ex ::= ex POWER ex",
 /*  30 */ "ex ::= ex AMPERSAND ex",
 /*  31 */ "ex ::= ex IS NUMBER",
 /*  32 */ "ex ::= ex IS STRING",
 /*  33 */ "ex ::= ex IS LIST",
 /*  34 */ "ex ::= ex IS NOT LIST",
 /*  35 */ "ex ::= LPAR ex RPAR",
 /*  36 */ "ex ::= jsonarray",
 /*  37 */ "jsonarray ::= LSPAR RSPAR",
 /*  38 */ "jsonarray ::= LSPAR exlist RSPAR",
 /*  39 */ "exlist ::= ex",
 /*  40 */ "exlist ::= exlist COMMA ex",
 /*  41 */ "ex ::= COUNT ex",
 /*  42 */ "ex ::= FIRST ex",
 /*  43 */ "ex ::= ex WHERE ex",
 /*  44 */ "ex ::= ex IS LESS THAN ex",
 /*  45 */ "ex ::= ex IS LESS THAN OR EQUAL ex",
 /*  46 */ "ex ::= ex IS GREATER THAN ex",
 /*  47 */ "ex ::= ex IS GREATER THAN OR EQUAL ex",
 /*  48 */ "ex ::= ex IS WITHIN ex TO ex",
 /*  49 */ "ex ::= IT",
 /*  50 */ "ex ::= ex OCCURS BEFORE ex",
 /*  51 */ "ex ::= ex OCCURRED AFTER ex",
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
  { 60, 10 },
  { 60, 2 },
  { 62, 4 },
  { 63, 2 },
  { 63, 4 },
  { 63, 2 },
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
  { 61, 3 },
  { 61, 1 },
  { 65, 2 },
  { 65, 3 },
  { 66, 1 },
  { 66, 3 },
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
  { 64, 0 },
  { 64, 1 },
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
#line 1001 "grammar.c"
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
#line 1012 "grammar.c"
        break;
      case 2: /* statementblock ::= statementblock statement */
#line 322 "grammar.y"
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( yymsp[-1].minor.yy0, "statements"), yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-1].minor.yy0;
}
#line 1020 "grammar.c"
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
#line 1031 "grammar.c"
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
#line 1042 "grammar.c"
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
#line 1055 "grammar.c"
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
#line 1067 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* statement ::= TIME IDENTIFIER ASSIGN ex SEMICOLON */
#line 373 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-4].minor.yy0 = res;
}
#line 1079 "grammar.c"
        break;
      case 8: /* statement ::= TIME OF IDENTIFIER ASSIGN ex SEMICOLON */
#line 382 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_OF");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-3].minor.yy0));
	cJSON_AddItemToObject(res, "arg", yymsp[-1].minor.yy0);
	yymsp[-5].minor.yy0 = res;
}
#line 1090 "grammar.c"
        break;
      case 9: /* statement ::= FOR IDENTIFIER IN ex FORRANGE ex DO statementblock ENDDO SEMICOLON */
#line 391 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FOR_LOOP");
	cJSON_AddStringToObject(res, "varname", getValue(yymsp[-8].minor.yy0));
	cJSON_AddItemToObject(res, "expression", yymsp[-6].minor.yy0);
	cJSON_AddItemToObject(res, "expression2", yymsp[-4].minor.yy0);

	cJSON_AddItemToObject(res, "statements", yymsp[-2].minor.yy0);
	yymsp[-9].minor.yy0 = res;
}
#line 1104 "grammar.c"
        break;
      case 10: /* statement ::= IF if_then_else */
      case 14: /* elseif ::= ELSEIF if_then_else */ yytestcase(yyruleno==14);
#line 404 "grammar.y"
{ yymsp[-1].minor.yy0 = yymsp[0].minor.yy0; }
#line 1110 "grammar.c"
        break;
      case 11: /* if_then_else ::= ex THEN statementblock elseif */
#line 407 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IF");
	cJSON_AddItemToObject(res, "condition", yymsp[-3].minor.yy0);
	cJSON_AddItemToObject(res, "thenbranch", (yymsp[-1].minor.yy0));
	cJSON_AddItemToObject(res, "elsebranch", (yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1122 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 12: /* elseif ::= ENDIF SEMICOLON */
#line 417 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENT_BLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg);
	yymsp[-1].minor.yy0 = res;
}
#line 1134 "grammar.c"
        break;
      case 13: /* elseif ::= ELSE statementblock ENDIF SEMICOLON */
#line 426 "grammar.y"
{ yymsp[-3].minor.yy0 = yymsp[-2].minor.yy0; }
#line 1139 "grammar.c"
        break;
      case 15: /* ex ::= NUMTOKEN */
#line 442 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1149 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 16: /* ex ::= TIMETOKEN */
#line 450 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMETOKEN");
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0));
	yylhsminor.yy0 = res;
}
#line 1160 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* ex ::= STRTOKEN */
#line 458 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1171 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 18: /* ex ::= IDENTIFIER */
#line 466 "grammar.y"
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(yymsp[0].minor.yy0)); 
	cJSON_AddStringToObject(res, "line", getLine(yymsp[0].minor.yy0)); 
	yylhsminor.yy0 = res; 
}
#line 1183 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* ex ::= TRUE */
#line 475 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	yymsp[0].minor.yy0 = res;
}
#line 1193 "grammar.c"
        break;
      case 20: /* ex ::= FALSE */
#line 482 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	yymsp[0].minor.yy0 = res;
}
#line 1202 "grammar.c"
        break;
      case 21: /* ex ::= CURRENTTIME */
#line 489 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "CURRENT_TIME");
	yymsp[0].minor.yy0 = res;
}
#line 1211 "grammar.c"
        break;
      case 22: /* ex ::= NOW */
#line 496 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	yymsp[0].minor.yy0 = res;
}
#line 1220 "grammar.c"
        break;
      case 23: /* ex ::= NULL1 */
#line 503 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	yymsp[0].minor.yy0 = res;
}
#line 1229 "grammar.c"
        break;
      case 24: /* ex ::= TIME of ex */
#line 513 "grammar.y"
{ yymsp[-2].minor.yy0 = unary ("TIME_READ", yymsp[0].minor.yy0); }
#line 1234 "grammar.c"
        break;
      case 25: /* ex ::= ex PLUS ex */
#line 516 "grammar.y"
{yylhsminor.yy0 = binary ("PLUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1239 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* ex ::= ex MINUS ex */
#line 519 "grammar.y"
{yylhsminor.yy0 = binary ("MINUS", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1245 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* ex ::= ex TIMES ex */
#line 522 "grammar.y"
{yylhsminor.yy0 = binary ("TIMES", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1251 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* ex ::= ex DIVIDE ex */
#line 525 "grammar.y"
{yylhsminor.yy0 = binary ("DIVIDE", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1257 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* ex ::= ex POWER ex */
#line 528 "grammar.y"
{yylhsminor.yy0 = binary ("POWER", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1263 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* ex ::= ex AMPERSAND ex */
#line 531 "grammar.y"
{ yylhsminor.yy0 = binary ("STR_CONCAT", yymsp[-2].minor.yy0, yymsp[0].minor.yy0); }
#line 1269 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* ex ::= ex IS NUMBER */
#line 534 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NUMBER", yymsp[-2].minor.yy0); }
#line 1275 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* ex ::= ex IS STRING */
#line 537 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_STRING", yymsp[-2].minor.yy0); }
#line 1281 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* ex ::= ex IS LIST */
#line 540 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_LIST", yymsp[-2].minor.yy0); }
#line 1287 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* ex ::= ex IS NOT LIST */
#line 543 "grammar.y"
{ yylhsminor.yy0 = unary ("IS_NOT_LIST", yymsp[-3].minor.yy0); }
#line 1293 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* ex ::= LPAR ex RPAR */
#line 546 "grammar.y"
{ 
	yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; 
}
#line 1301 "grammar.c"
        break;
      case 36: /* ex ::= jsonarray */
#line 555 "grammar.y"
{ yylhsminor.yy0 = yymsp[0].minor.yy0; }
#line 1306 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* jsonarray ::= LSPAR RSPAR */
#line 558 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "EMPTY_LIST");
	yymsp[-1].minor.yy0 = res;
}
#line 1316 "grammar.c"
        break;
      case 38: /* jsonarray ::= LSPAR exlist RSPAR */
#line 565 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "LIST");
	cJSON_AddItemToObject(res, "items", yymsp[-1].minor.yy0);
	cJSON_AddNumberToObject(res,"code", 1016);
	yymsp[-2].minor.yy0 = res;
}
#line 1327 "grammar.c"
        break;
      case 39: /* exlist ::= ex */
#line 574 "grammar.y"
{
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, yymsp[0].minor.yy0);
	yylhsminor.yy0 = arg;
}
#line 1336 "grammar.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* exlist ::= exlist COMMA ex */
#line 581 "grammar.y"
{
	cJSON_AddItemToArray(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	yylhsminor.yy0 = yymsp[-2].minor.yy0;
}
#line 1345 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* ex ::= COUNT ex */
#line 587 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("COUNT", yymsp[0].minor.yy0); }
#line 1351 "grammar.c"
        break;
      case 42: /* ex ::= FIRST ex */
#line 590 "grammar.y"
{ yymsp[-1].minor.yy0 = unary ("FIRST", yymsp[0].minor.yy0); }
#line 1356 "grammar.c"
        break;
      case 43: /* ex ::= ex WHERE ex */
#line 593 "grammar.y"
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
#line 1371 "grammar.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 44: /* ex ::= ex IS LESS THAN ex */
#line 606 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_THAN", yymsp[-4].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1379 "grammar.c"
  yymsp[-4].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* ex ::= ex IS LESS THAN OR EQUAL ex */
#line 611 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("LESS_OR_EQUAL", yymsp[-6].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1387 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* ex ::= ex IS GREATER THAN ex */
#line 616 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("GREATER_THAN", yymsp[-4].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1395 "grammar.c"
  yymsp[-4].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* ex ::= ex IS GREATER THAN OR EQUAL ex */
#line 621 "grammar.y"
{ 
	yylhsminor.yy0 = binary ("GREATER_OR_EQUAL", yymsp[-6].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1403 "grammar.c"
  yymsp[-6].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* ex ::= ex IS WITHIN ex TO ex */
#line 626 "grammar.y"
{ 
	yylhsminor.yy0 = ternary ("IS_WITHIN", yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0); 
}
#line 1411 "grammar.c"
  yymsp[-5].minor.yy0 = yylhsminor.yy0;
        break;
      case 49: /* ex ::= IT */
#line 631 "grammar.y"
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IT");
	yymsp[0].minor.yy0 = res;
}
#line 1421 "grammar.c"
        break;
      case 50: /* ex ::= ex OCCURS BEFORE ex */
#line 638 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURS_BEFORE", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1426 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* ex ::= ex OCCURRED AFTER ex */
#line 641 "grammar.y"
{ yylhsminor.yy0 = binary ("OCCURRED_AFTER", yymsp[-3].minor.yy0, yymsp[0].minor.yy0); }
#line 1432 "grammar.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
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
#line 1498 "grammar.c"
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



#line 1940 "grammar.c"
