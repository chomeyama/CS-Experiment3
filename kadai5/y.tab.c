/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SBEGIN = 258,
     DO = 259,
     ELSE = 260,
     SEND = 261,
     FOR = 262,
     FORWARD = 263,
     FUNCTION = 264,
     IF = 265,
     PROCEDURE = 266,
     PROGRAM = 267,
     READ = 268,
     THEN = 269,
     TO = 270,
     VAR = 271,
     WHILE = 272,
     WRITE = 273,
     MINUS = 274,
     PLUS = 275,
     DIV = 276,
     MULT = 277,
     EQ = 278,
     NEQ = 279,
     LE = 280,
     LT = 281,
     GE = 282,
     GT = 283,
     LPAREN = 284,
     RPAREN = 285,
     LBRACE = 286,
     RBRACE = 287,
     COMMA = 288,
     SEMICOLON = 289,
     COLON = 290,
     INTERVAL = 291,
     PERIOD = 292,
     ASSIGN = 293,
     NUMBER = 294,
     IDENT = 295
   };
#endif
/* Tokens.  */
#define SBEGIN 258
#define DO 259
#define ELSE 260
#define SEND 261
#define FOR 262
#define FORWARD 263
#define FUNCTION 264
#define IF 265
#define PROCEDURE 266
#define PROGRAM 267
#define READ 268
#define THEN 269
#define TO 270
#define VAR 271
#define WHILE 272
#define WRITE 273
#define MINUS 274
#define PLUS 275
#define DIV 276
#define MULT 277
#define EQ 278
#define NEQ 279
#define LE 280
#define LT 281
#define GE 282
#define GT 283
#define LPAREN 284
#define RPAREN 285
#define LBRACE 286
#define RBRACE 287
#define COMMA 288
#define SEMICOLON 289
#define COLON 290
#define INTERVAL 291
#define PERIOD 292
#define ASSIGN 293
#define NUMBER 294
#define IDENT 295




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

/*
 * parser; Parser for PL-*
 */

#define MAXLENGTH 16

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylineno;
extern char *yytext;

typedef enum {
    GLOBAL_VAR,
    LOCAL_VAR,
    PROC_NAME,
    CONSTANT
} Scope;

typedef struct {
    Scope type;
    char vname[16];
    int val;
} Factor;

typedef enum {
    INT,
    VOID
} Rettype;

/*Factorスタック*/
typedef struct _FactorStack {
    Factor data;
    struct _FactorStack *prev;
} FactorStack;

/*Tableスタック*/
typedef struct _TableStack{
    Factor data; //記号表のレコード
    struct _TableStack *prev; //前のノードへのポインタ
} TableStack;

/* LLVM命令名の定義 */
typedef enum {
  Alloca,   /* alloca */
  Global,   /* global */
  Store,    /* store  */
  Load,     /* load   */
  BrUncond, /* br     */
  BrCond,   /* brc    */
  Label,    /* label  */
  Add,      /* add    */
  Sub,      /* sub    */
  Mult,     /* mult   */
  Div,      /* div    */
  Icmp,     /* icmp   */
  Read,     /* read   */
  Write,    /* write  */
  Call,     /* call   */
  Retint,   /* ret    */
  Retvoid   /* retvoid*/
} LLVMcommand;

/* 比較演算子の種類 */
typedef enum {
  EQUAL, /* eq （==）*/
  NE,    /* ne （!=）*/
  SGT,   /* sgt （>，符号付き） */
  SGE,   /* sge （>=，符号付き）*/
  SLT,   /* slt （<，符号付き） */
  SLE    /* sle （<=，符号付き）*/
} Cmptype;

typedef struct llvmcode {
  LLVMcommand command; /* 命令名 */
  union { /* 命令の引数 */
    struct { /* alloca */
      Factor retval;
    } alloca;
    struct { /* global */
      Factor retval;
    } global;
    struct { /* store  */
      Factor arg1;  Factor arg2;
    } store;
    struct { /* load   */
      Factor arg1;  Factor retval;
    } load;
    struct { /* br     */
      int arg1;
    } bruncond;
    struct { /* brc    */
      Factor arg1;  int arg2;  int arg3;
    } brcond;
    struct { /* label  */
      int l;
    } label;
    struct { /* add    */
      Factor arg1;  Factor arg2;  Factor retval;
    } add;
    struct { /* sub    */
      Factor arg1;  Factor arg2;  Factor retval;
    } sub;
    struct { /* mult   */
      Factor arg1;  Factor arg2;  Factor retval;
    } mult;
    struct { /* div    */
      Factor arg1;  Factor arg2;  Factor retval;
    } div;
    struct { /* icmp   */
      Cmptype type; Factor arg1;  Factor arg2;  Factor retval;
    } icmp;
    struct { /* read   */
      Factor arg1; Factor retval;
    } read;
    struct { /* call   */
      char fname[20];
    } call;
    struct { /* write   */
      Factor arg1; Factor retval;
    } write;
    struct { /* retint */
      Factor arg1; Factor retval;
    } retint;
    struct { /* retvoid */
    } retvoid;
  } args;
  /* 次の命令へのポインタ */
  struct llvmcode *next;
} LLVMcode;

/* LLVMの関数定義*/
typedef struct fundecl {
  char fname[256];/*関数名*/
  unsigned arity;/*引数個数*/
  FactorStack args;/*引数*/
  Rettype rettype;
  LLVMcode *codes;/*命令列の線形リストへのポインタ*/
  struct fundecl *next; /*次の関数定義へのポインタ*/
} Fundecl;

/* br命令へのポインタを記憶するためのスタック */
typedef struct brcommand {
    LLVMcode *ptr;
    struct brcommand *prev;
} BrCommand;

Scope flag = GLOBAL_VAR; //変数のスコープはグローバルで初期化
FILE *fp; //result.llのファイルポインタ
unsigned int reg_cntr = 1; //レジスタ番号管理のためのカウンタ

TableStack *TableStackPtr = NULL; //Tableスタックのスタックポインタ
void TablePush(Factor*); //Tableスタックのpush関数
void TablePop(); //Tableスタックのpop関数
void TableShow(); //Tableスタックの全データを出力するための関数

void insert(char*, Scope); //記号表への追加を行う関数
Factor* lookup(char*); //記号表から検索を行う関数
void delete(); //記号表から要素を削除する関数

FactorStack *FactorStackPtr = NULL; //Factorスタックのスタックポインタ
void FactorPush(Factor*); //Factorスタックのpush関数
Factor FactorPop(); //Factorスタックのpop関数
Factor makeFactor(Scope, char*, int); //Factorを作成して返す関数

void AllocaPush(Factor); //AllocaのLLVMcodeを作成して線形リストに追加する関数
void GlobalPush(Factor);
void StorePush(Factor, Factor);
void LoadPush(Factor, Factor);
void BrUncondPush(int);
void BrCondPush(Factor, int, int);
void LabelPush(int);
void AddPush(Factor, Factor, Factor);
void SubPush(Factor, Factor, Factor);
void MultPush(Factor, Factor, Factor);
void DivPush(Factor, Factor, Factor);
void IcmpPush(Cmptype, Factor, Factor, Factor);
void ReadPush(Factor, Factor);
void WritePush(Factor, Factor);
void CallPush(char*);
void RetPush(Rettype);

void cmpcalc(Cmptype); //比較演算
void calc(LLVMcommand); //四則演算

void LLVMPush(LLVMcode*); //LLVMcodeのpush関数
LLVMcode *codehd = NULL; /* 命令列の先頭のアドレスを保持するポインタ */
LLVMcode *codetl = NULL; /* 命令列の末尾のアドレスを保持するポインタ */

void FunPush(Fundecl*); //Fundeclのpush関数
Fundecl *declhd = NULL; //関数定義の線形リストの先頭の要素のアドレスを保持するポインタ
Fundecl *decltl = NULL; //関数定義の線形リストの末尾の要素のアドレスを保持するポインタ

void BrCommandPush(LLVMcode*); //BrCommandのpush関数
void BrCommandPop(); //BrCommandのpop関数
void backpatch(int);
BrCommand *BrCommandStackPtr = NULL; //BrCommandのスタックポインタ

void displayFactor(Factor);
void displayLlvmcodes(LLVMcode*);
void displayLlvmfundecl(Fundecl*);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 207 "parser.y"
{
    int num;
    char ident[MAXLENGTH+1];
}
/* Line 193 of yacc.c.  */
#line 387 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 400 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNRULES -- Number of states.  */
#define YYNSTATES  133

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,    11,    12,    13,    19,    20,    23,
      27,    29,    32,    33,    36,    40,    42,    44,    45,    51,
      53,    56,    60,    62,    64,    66,    68,    70,    72,    74,
      76,    78,    80,    84,    85,    86,    87,    96,    97,    98,
     102,   103,   104,   105,   113,   114,   115,   116,   117,   118,
     132,   134,   139,   141,   145,   150,   155,   156,   160,   164,
     168,   172,   176,   180,   182,   185,   188,   192,   196,   198,
     202,   206,   208,   210,   214,   216,   218,   222,   224
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      42,     0,    -1,    -1,    12,    43,    40,    34,    44,    37,
      -1,    -1,    -1,    47,    45,    50,    46,    58,    -1,    -1,
      48,    34,    -1,    48,    34,    49,    -1,    49,    -1,    16,
      88,    -1,    -1,    51,    34,    -1,    51,    34,    52,    -1,
      52,    -1,    53,    -1,    -1,    11,    54,    55,    34,    56,
      -1,    40,    -1,    47,    58,    -1,    57,    34,    58,    -1,
      58,    -1,    59,    -1,    60,    -1,    66,    -1,    70,    -1,
      76,    -1,    81,    -1,    78,    -1,    79,    -1,    80,    -1,
      40,    38,    83,    -1,    -1,    -1,    -1,    10,    82,    61,
      14,    62,    58,    63,    64,    -1,    -1,    -1,     5,    65,
      58,    -1,    -1,    -1,    -1,    17,    67,    82,    68,     4,
      69,    58,    -1,    -1,    -1,    -1,    -1,    -1,     7,    71,
      40,    38,    83,    72,    15,    73,    83,    74,     4,    75,
      58,    -1,    77,    -1,    77,    29,    87,    30,    -1,    40,
      -1,     3,    57,     6,    -1,    13,    29,    40,    30,    -1,
      18,    29,    83,    30,    -1,    -1,    83,    23,    83,    -1,
      83,    24,    83,    -1,    83,    26,    83,    -1,    83,    25,
      83,    -1,    83,    28,    83,    -1,    83,    27,    83,    -1,
      84,    -1,    20,    84,    -1,    19,    84,    -1,    83,    20,
      84,    -1,    83,    19,    84,    -1,    85,    -1,    84,    22,
      85,    -1,    84,    21,    85,    -1,    86,    -1,    39,    -1,
      29,    83,    30,    -1,    40,    -1,    83,    -1,    87,    33,
      83,    -1,    40,    -1,    88,    33,    40,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   231,   231,   231,   240,   241,   240,   250,   252,   256,
     257,   261,   264,   266,   270,   271,   275,   279,   279,   283,
     292,   296,   297,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   313,   317,   317,   317,   317,   320,   322,   322,
     326,   327,   328,   326,   333,   334,   335,   336,   337,   333,
     342,   343,   347,   351,   355,   359,   362,   367,   368,   369,
     370,   371,   372,   376,   377,   378,   379,   380,   384,   385,
     386,   390,   391,   392,   396,   400,   401,   405,   406
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SBEGIN", "DO", "ELSE", "SEND", "FOR",
  "FORWARD", "FUNCTION", "IF", "PROCEDURE", "PROGRAM", "READ", "THEN",
  "TO", "VAR", "WHILE", "WRITE", "MINUS", "PLUS", "DIV", "MULT", "EQ",
  "NEQ", "LE", "LT", "GE", "GT", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
  "COMMA", "SEMICOLON", "COLON", "INTERVAL", "PERIOD", "ASSIGN", "NUMBER",
  "IDENT", "$accept", "program", "@1", "outblock", "@2", "@3",
  "var_decl_part", "var_decl_list", "var_decl", "subprog_decl_part",
  "subprog_decl_list", "subprog_decl", "proc_decl", "@4", "proc_name",
  "inblock", "statement_list", "statement", "assignment_statement",
  "if_statement", "@5", "@6", "@7", "else_statement", "@8",
  "while_statement", "@9", "@10", "@11", "for_statement", "@12", "@13",
  "@14", "@15", "@16", "proc_call_statement", "proc_call_name",
  "block_statement", "read_statement", "write_statement", "null_statement",
  "condition", "expression", "term", "factor", "var_name", "arg_list",
  "id_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    43,    42,    45,    46,    44,    47,    47,    48,
      48,    49,    50,    50,    51,    51,    52,    54,    53,    55,
      56,    57,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    61,    62,    63,    60,    64,    65,    64,
      67,    68,    69,    66,    71,    72,    73,    74,    75,    70,
      76,    76,    77,    78,    79,    80,    81,    82,    82,    82,
      82,    82,    82,    83,    83,    83,    83,    83,    84,    84,
      84,    85,    85,    85,    86,    87,    87,    88,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     0,     0,     5,     0,     2,     3,
       1,     2,     0,     2,     3,     1,     1,     0,     5,     1,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     0,     0,     8,     0,     0,     3,
       0,     0,     0,     7,     0,     0,     0,     0,     0,    13,
       1,     4,     1,     3,     4,     4,     0,     3,     3,     3,
       3,     3,     3,     1,     2,     2,     3,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     2,     0,     0,     1,     0,     7,     0,     0,     4,
       0,    10,    77,    11,     3,    12,     8,     0,    17,     5,
       0,    15,    16,     9,    78,     0,    56,    13,    19,     0,
      56,    44,     0,     0,    40,     0,    52,     6,    23,    24,
      25,    26,    27,    50,    29,    30,    31,    28,    14,     7,
       0,    22,     0,     0,     0,     0,    72,    74,    33,     0,
      63,    68,    71,     0,     0,     0,     0,     0,    56,    18,
      53,    56,     0,    65,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,     0,
      32,    75,     0,    20,    21,     0,    73,    34,    67,    66,
      57,    58,    60,    59,    62,    61,    70,    69,    54,     0,
      55,    51,     0,    45,    56,    42,    76,     0,    35,    56,
      46,    37,    43,     0,    38,    36,    47,    56,     0,    39,
      48,    56,    49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     8,    15,    26,     9,    10,    11,    19,
      20,    21,    22,    25,    29,    69,    50,    37,    38,    39,
      76,   114,   121,   125,   127,    40,    64,   109,   119,    41,
      52,   117,   123,   128,   131,    42,    43,    44,    45,    46,
      47,    58,    59,    60,    61,    62,    92,    13
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int8 yypact[] =
{
     -11,   -64,     3,    -1,   -64,    13,    21,     8,    12,   -64,
      18,   -64,   -64,    20,   -64,    44,    21,    16,   -64,   -64,
      23,   -64,   -64,   -64,   -64,    22,    33,    44,   -64,    24,
      33,   -64,    25,    31,   -64,    32,    28,   -64,   -64,   -64,
     -64,   -64,   -64,    34,   -64,   -64,   -64,   -64,   -64,    21,
      -2,   -64,    27,   -24,   -24,    25,   -64,   -64,   -64,    52,
      -3,   -64,   -64,    29,    25,    25,    25,    25,    33,   -64,
     -64,    33,    30,    -3,    -3,   -13,    60,   -24,   -24,    25,
      25,    25,    25,    25,    25,   -24,   -24,    51,   -64,     5,
       1,     1,   -22,   -64,   -64,    25,   -64,   -64,    -3,    -3,
       1,     1,     1,     1,     1,     1,   -64,   -64,   -64,    78,
     -64,   -64,    25,     1,    33,   -64,     1,    68,   -64,    33,
     -64,    80,   -64,    25,   -64,   -64,     1,    33,    82,   -64,
     -64,    33,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -64,   -64,   -64,   -64,   -64,   -64,    38,   -64,    72,   -64,
     -64,    63,   -64,   -64,   -64,   -64,   -64,   -30,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,    35,   -53,   -44,   -63,   -64,   -64,   -64
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      51,     1,    75,     4,    70,    55,    77,    78,   111,    73,
      74,   112,    89,    90,    91,    56,    57,    96,    85,    86,
      77,    78,   106,   107,    77,    78,   100,   101,   102,   103,
     104,   105,    71,    98,    99,   110,    30,     7,    93,     5,
      31,    94,   113,    32,    53,    54,    33,     6,    12,    14,
      34,    35,    16,    17,    55,    18,    24,    27,    49,   116,
      63,    65,    28,    67,    56,    57,    66,    72,    95,    87,
     126,    77,    78,    36,    97,    79,    80,    81,    82,    83,
      84,   108,   115,   120,   118,   124,   130,    68,    23,   122,
      48,     0,     0,     0,     0,     0,     0,   129,     0,    88,
       0,   132
};

static const yytype_int16 yycheck[] =
{
      30,    12,    55,     0,     6,    29,    19,    20,    30,    53,
      54,    33,    65,    66,    67,    39,    40,    30,    21,    22,
      19,    20,    85,    86,    19,    20,    79,    80,    81,    82,
      83,    84,    34,    77,    78,    30,     3,    16,    68,    40,
       7,    71,    95,    10,    19,    20,    13,    34,    40,    37,
      17,    18,    34,    33,    29,    11,    40,    34,    34,   112,
      29,    29,    40,    29,    39,    40,    38,    40,    38,    40,
     123,    19,    20,    40,    14,    23,    24,    25,    26,    27,
      28,    30,     4,    15,   114,     5,     4,    49,    16,   119,
      27,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    64,
      -1,   131
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    42,    43,     0,    40,    34,    16,    44,    47,
      48,    49,    40,    88,    37,    45,    34,    33,    11,    50,
      51,    52,    53,    49,    40,    54,    46,    34,    40,    55,
       3,     7,    10,    13,    17,    18,    40,    58,    59,    60,
      66,    70,    76,    77,    78,    79,    80,    81,    52,    34,
      57,    58,    71,    19,    20,    29,    39,    40,    82,    83,
      84,    85,    86,    29,    67,    29,    38,    29,    47,    56,
       6,    34,    40,    84,    84,    83,    61,    19,    20,    23,
      24,    25,    26,    27,    28,    21,    22,    40,    82,    83,
      83,    83,    87,    58,    58,    38,    30,    14,    84,    84,
      83,    83,    83,    83,    83,    83,    85,    85,    30,    68,
      30,    30,    33,    83,    62,     4,    83,    72,    58,    69,
      15,    63,    58,    73,     5,    64,    83,    65,    74,    58,
       4,    75,    58
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 231 "parser.y"
    {fp = fopen("result.ll", "w");
               fprintf(fp, "@.str = private unnamed_addr constant [3 x i8] c\"%%\\d\\00\", align 1\n");
               fprintf(fp, "@.str.1 = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
               fprintf(fp, "declare i32 @scanf(i8*, ...) #1\n");
               fprintf(fp, "declare i32 @printf(i8*, ...) #1\n");}
    break;

  case 3:
#line 236 "parser.y"
    {displayLlvmfundecl(declhd); fclose(fp);}
    break;

  case 4:
#line 240 "parser.y"
    {displayLlvmcodes(codehd);}
    break;

  case 5:
#line 241 "parser.y"
    {/* main 関数 */
                         printf("main\n");
	                       insert("main", PROC_NAME);
	                       flag = LOCAL_VAR;
	                       insert("main", flag);
                         StorePush(makeFactor(CONSTANT, "", 0), *lookup("main"));}
    break;

  case 6:
#line 247 "parser.y"
    {RetPush(decltl->rettype);}
    break;

  case 13:
#line 266 "parser.y"
    {printf("subprog_decl_part end.");}
    break;

  case 16:
#line 275 "parser.y"
    {printf("unko\n"); RetPush(decltl->rettype); delete(); flag = GLOBAL_VAR; reg_cntr = 1;}
    break;

  case 17:
#line 279 "parser.y"
    {flag = LOCAL_VAR;}
    break;

  case 19:
#line 283 "parser.y"
    {insert((yyvsp[(1) - (1)].ident), PROC_NAME);

             if (decltl->rettype == INT) {
                 AllocaPush(makeFactor(LOCAL_VAR, (yyvsp[(1) - (1)].ident), reg_cntr++));
             }
            }
    break;

  case 32:
#line 313 "parser.y"
    {StorePush(FactorStackPtr->data, *lookup((yyvsp[(1) - (3)].ident)));}
    break;

  case 33:
#line 317 "parser.y"
    {printf("if\n"); BrCondPush(FactorStackPtr->data, reg_cntr, NULL);}
    break;

  case 34:
#line 317 "parser.y"
    {LabelPush(reg_cntr++);}
    break;

  case 35:
#line 317 "parser.y"
    {backpatch(reg_cntr);}
    break;

  case 36:
#line 317 "parser.y"
    {LabelPush(reg_cntr++); printf("if end.\n");}
    break;

  case 38:
#line 322 "parser.y"
    {BrUncondPush(-1); LabelPush(reg_cntr);}
    break;

  case 39:
#line 322 "parser.y"
    {backpatch(reg_cntr);}
    break;

  case 40:
#line 326 "parser.y"
    {BrUncondPush(reg_cntr); LabelPush(reg_cntr++);}
    break;

  case 41:
#line 327 "parser.y"
    {BrCondPush(FactorStackPtr->data, reg_cntr, -1);}
    break;

  case 42:
#line 328 "parser.y"
    { LabelPush(reg_cntr++);}
    break;

  case 43:
#line 329 "parser.y"
    {backpatch(reg_cntr); BrUncondPush(BrCommandStackPtr->ptr->args.bruncond.arg1); BrCommandPop(); BrCommandPop(); LabelPush(reg_cntr++);}
    break;

  case 44:
#line 333 "parser.y"
    {AllocaPush(makeFactor(LOCAL_VAR, "FOR_COND_VAL", reg_cntr++));}
    break;

  case 45:
#line 334 "parser.y"
    {StorePush(FactorStackPtr->data, *lookup((yyvsp[(3) - (5)].ident)));}
    break;

  case 46:
#line 335 "parser.y"
    {LabelPush(reg_cntr++);}
    break;

  case 47:
#line 336 "parser.y"
    {IcmpPush(SLT, *lookup((yyvsp[(3) - (9)].ident)), FactorStackPtr->data, *lookup("FOR_COND_VAL")); BrCondPush(*lookup("FOR_COND_VAL"), reg_cntr, -1);}
    break;

  case 48:
#line 337 "parser.y"
    {LabelPush(reg_cntr++);}
    break;

  case 49:
#line 338 "parser.y"
    {BrUncondPush((BrCommandStackPtr->ptr->args.bruncond.arg1)-1); backpatch(reg_cntr); LabelPush(reg_cntr++);}
    break;

  case 52:
#line 347 "parser.y"
    {CallPush((yyvsp[(1) - (1)].ident));}
    break;

  case 53:
#line 351 "parser.y"
    {printf("block_statement end.");}
    break;

  case 54:
#line 355 "parser.y"
    {ReadPush(*lookup((yyvsp[(3) - (4)].ident)), makeFactor(LOCAL_VAR, "", reg_cntr++));}
    break;

  case 55:
#line 359 "parser.y"
    {WritePush(FactorStackPtr->data, makeFactor(LOCAL_VAR, "", reg_cntr++));}
    break;

  case 57:
#line 367 "parser.y"
    {cmpcalc(EQUAL);}
    break;

  case 58:
#line 368 "parser.y"
    {cmpcalc(NE);}
    break;

  case 59:
#line 369 "parser.y"
    {cmpcalc(SLT);}
    break;

  case 60:
#line 370 "parser.y"
    {cmpcalc(SLE);}
    break;

  case 61:
#line 371 "parser.y"
    {cmpcalc(SGT);}
    break;

  case 62:
#line 372 "parser.y"
    {cmpcalc(SGE);}
    break;

  case 66:
#line 379 "parser.y"
    {calc(Add);}
    break;

  case 67:
#line 380 "parser.y"
    {calc(Sub);}
    break;

  case 69:
#line 385 "parser.y"
    {calc(Mult);}
    break;

  case 70:
#line 386 "parser.y"
    {calc(Div);}
    break;

  case 72:
#line 391 "parser.y"
    {Factor f = makeFactor(CONSTANT, "", (yyvsp[(1) - (1)].num)); FactorPush(&f);}
    break;

  case 74:
#line 396 "parser.y"
    {FactorPush(lookup((yyvsp[(1) - (1)].ident)));}
    break;

  case 77:
#line 405 "parser.y"
    {insert((yyvsp[(1) - (1)].ident), flag);}
    break;

  case 78:
#line 406 "parser.y"
    {insert((yyvsp[(3) - (3)].ident), flag);}
    break;


/* Line 1267 of yacc.c.  */
#line 1951 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 410 "parser.y"


yyerror(char *s)
{
  extern int yylineno;
  extern char *yytext;
  fprintf(fp, stderr, "%s %s in line %d\n", s, yytext, yylineno);
}


//記号表のpush関数
void TablePush(Factor *f) {
    TableStack *tmp = (TableStack *)malloc(sizeof(TableStack));
    tmp->prev = TableStackPtr; //新しいデータのprevは現先頭データを指す

    //データのレコード部分を作成
    strcpy(tmp->data.vname ,f->vname);
    tmp->data.val = f->val;
    tmp->data.type = f->type;

    TableStackPtr = tmp; //スタックの先頭を新しいデータに変更
}

//記号表のpop関数
void TablePop() {
    TableStack *p = TableStackPtr; //メモリ解放するために先頭アドレスを一時記憶
    TableStackPtr = TableStackPtr->prev; //先頭アドレスを先頭から二番目に変更
    free(p); //先頭だったアドレスが示すメモリ領域を解放
}

//記号表の全データを出力するための関数
void show() {
    TableStack *p = TableStackPtr; //出力するデータを指すポインタ
    char *kind;
    printf(" _______________________________________\n");
    printf("|       kind             name   val/adr |\n");
    printf("|---------------------------------------|\n");
    while (1) {
        switch (p->data.type) {
          case 0:
              kind = "GLOBAL_VAR";
              break;
          case 1:
              kind = "LOCAL_VAR";
              break;
          case 2:
              kind = "PROC_NAME";
              break;
          case 3:
              kind = "CONSTANT";
        }
        if (p->prev == NULL) { //先頭データに達した場合
            printf("|%11s %16s %9d |\n", kind, p->data.vname, p->data.val);
            break;
        } else { //先頭データでない場合
            printf("|%11s %16s %9d |\n", kind, p->data.vname, p->data.val);
            p = p->prev; //次の出力対象を一つ前のデータに
        }
    }
    printf("|_______________________________________|\n\n");
}

/* 記号表への追加を行う関数 */
void insert(char *s, Scope flag) {
    Factor *f = (Factor *)malloc(sizeof(Factor));
    f->type = flag;
    strcpy(f->vname, s);
    if (flag == GLOBAL_VAR) {
        GlobalPush(*f);
    } else if (flag == LOCAL_VAR) {
        f->val = reg_cntr++;
	      AllocaPush(*f);
    } else {
        Fundecl *tmp = (Fundecl *)malloc(sizeof(Fundecl));
        strcpy(tmp->fname, s);
        tmp->arity = 0;
        if (strcmp(s, "main") == 0) {
            tmp->rettype = INT;
        } else {
            tmp->rettype = VOID;
        }
        tmp->codes = NULL;
        FunPush(tmp);
    }
    TablePush(f); //記号表への追加
    show(); //記号表のデータを出力
}

/* 記号表から検索を行う関数 */
Factor* lookup(char *s) {
    TableStack *p = TableStackPtr;
    while (strcmp(p->data.vname, s) != 0) { //引数と一致する記号を見つけるまで
        if (p->prev == NULL) { //先頭に達しても見つからない場合として終了
            printf("%s not found\n", s);
            return &(p->data);
        }
	      p = p->prev; //スタックの一つ前のデータへ辿る
    }
    return &(p->data);
}

/* 記号表の削除を行う関数 */
void delete() {
    while (TableStackPtr->data.type == LOCAL_VAR) { //局所変数である間
        TablePop(); //記号表から1つ削除
    }
    show(); //記号表の出力
}

/* Factorの作成を行う関数 */
Factor makeFactor(Scope type, char* vname, int val) {
    Factor f;
    f.type = type;
    strcpy(f.vname, vname);
    f.val = val;
    return f;
}

//Factorスタックのpush関数
void FactorPush(Factor *f) {
    FactorStack *tmp = (FactorStack *)malloc(sizeof(FactorStack));
    tmp->data = *f;
    tmp->prev = FactorStackPtr; //新しいデータのprevは現先頭データを指す
    FactorStackPtr = tmp; //スタックの先頭を新しいデータに変更
}

//Factorスタックのpop関数
Factor FactorPop() {
    Factor ret;
    FactorStack *p = FactorStackPtr; //メモリ解放するために先頭アドレスを一時記憶
    ret = p->data;
    FactorStackPtr = FactorStackPtr->prev; //先頭アドレスを先頭から二番目に変更
    free(p); //先頭だったアドレスが示すメモリ領域を解放
    return ret;
}

/* LLVMcodeを線形リストに追加する関数 */
void LLVMPush(LLVMcode *tmp) {
    if (codetl == NULL) {
        codehd = tmp;
        codetl = tmp;
    } else {
        codetl->next = tmp;
        codetl = tmp; //線形リストの末尾を新しいデータに変更
    }
    if (declhd != NULL) {
        if (decltl->codes == NULL) {
            decltl->codes = tmp;
        }
    }
}

/* AllocaのLLVMcodeを作成し線形リストに追加する関数 */
void AllocaPush(Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Alloca;
    tmp->args.alloca.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* GlobalのLLVMcodeを作成し線形リストに追加する関数 */
void GlobalPush(Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Global;
    tmp->args.global.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* StoreのLLVMcodeを作成し線形リストに追加する関数 */
void StorePush(Factor arg1, Factor arg2) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Store;
    tmp->args.store.arg1 = arg1;
    tmp->args.store.arg2 = arg2;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* LoadのLLVMcodeを作成し線形リストに追加する関数 */
void LoadPush(Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Load;
    tmp->args.load.arg1 = arg1;
    tmp->args.load.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* BrUncondのLLVMcodeを作成し線形リストに追加する関数 */
void BrUncondPush(int arg1) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = BrUncond;
    tmp->args.bruncond.arg1 = arg1;
    tmp->next = NULL;
    LLVMPush(tmp);
    BrCommandPush(tmp);
}

/* BrCondのLLVMcodeを作成し線形リストに追加する関数 */
void BrCondPush(Factor arg1, int arg2, int arg3) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = BrCond;
    tmp->args.brcond.arg1 = arg1;
    tmp->args.brcond.arg2 = arg2;
    tmp->args.brcond.arg3 = arg3;
    tmp->next = NULL;
    LLVMPush(tmp);
    BrCommandPush(tmp);
}

/* LabelのLLVMcodeを作成し線形リストに追加する関数 */
void LabelPush(int l) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Label;
    tmp->args.label.l = l;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* AddのLLVMcodeを作成し線形リストに追加する関数 */
void AddPush(Factor arg1, Factor arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Add;
    tmp->args.add.arg1 = arg1;
    tmp->args.add.arg2 = arg2;
    tmp->args.add.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* SubのLLVMcodeを作成し線形リストに追加する関数 */
void SubPush(Factor arg1, Factor arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Sub;
    tmp->args.sub.arg1 = arg1;
    tmp->args.sub.arg2 = arg2;
    tmp->args.sub.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* MultのLLVMcodeを作成し線形リストに追加する関数 */
void MultPush(Factor arg1, Factor arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Mult;
    tmp->args.mult.arg1 = arg1;
    tmp->args.mult.arg2 = arg2;
    tmp->args.mult.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* DivのLLVMcodeを作成し線形リストに追加する関数 */
void DivPush(Factor arg1, Factor arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Div;
    tmp->args.div.arg1 = arg1;
    tmp->args.div.arg2 = arg2;
    tmp->args.div.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* IcmpのLLVMcodeを作成し線形リストに追加する関数 */
void IcmpPush(Cmptype type, Factor arg1, Factor arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Icmp;
    tmp->args.icmp.type = type;
    tmp->args.icmp.arg1 = arg1;
    tmp->args.icmp.arg2 = arg2;
    tmp->args.icmp.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* ReadのLLVMcodeを作成し線形リストに追加する関数 */
void ReadPush(Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Read;
    if (arg1.type == GLOBAL_VAR) {
        Factor _arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(arg1, _arg1);
        tmp->args.read.arg1 = _arg1;
    } else {
        tmp->args.read.arg1 = arg1;
    }
    tmp->args.read.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* WriteのLLVMcodeを作成し線形リストに追加する関数 */
void WritePush(Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Write;
    tmp->args.write.arg1 = arg1;
    tmp->args.write.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

void CallPush(char *fname) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Call;
    strcpy(tmp->args.call.fname, fname);
    tmp->next = NULL;
    LLVMPush(tmp);
}

void RetPush(Rettype rettype) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    if (rettype == INT) {
      tmp->command = Retint;
      tmp->args.retint.retval = *lookup(decltl->fname);
    } else if (rettype == VOID) {
      tmp->command = Retvoid;
    }
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* 比較演算 */
void cmpcalc(Cmptype type) {
    Factor _arg1, _arg2, arg1, arg2, retval;
    _arg2 = FactorPop();
    _arg1 = FactorPop();

    if (_arg1.type == GLOBAL_VAR) {
        arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(_arg1, arg1);
    } else {
        arg1 = _arg1;
    }

    if (_arg2.type == GLOBAL_VAR) {
        arg2 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(_arg2, arg2);
    } else {
        arg2 = _arg2;
    }

    retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
    FactorPush(&retval); //結果を突っ込む
    IcmpPush(type, arg1, arg2, retval);
}

//四則演算
void calc(LLVMcommand ope) {
    Factor _arg1, _arg2, arg1, arg2, retval;
    _arg2 = FactorPop();
    _arg1 = FactorPop();

    if (_arg1.type == GLOBAL_VAR) {
        arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(_arg1, arg1);
    } else {
        arg1 = _arg1;
    }

    if (_arg2.type == GLOBAL_VAR) {
        arg2 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(_arg2, arg2);
    } else {
        arg2 = _arg2;
    }

    retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
    FactorPush(&retval); //結果を突っ込む

    switch (ope) { //オペレータによって場合分け
        case Add:
            AddPush(arg1, arg2, retval);
            break;
        case Sub:
            SubPush(arg1, arg2, retval);
            break;
        case Mult:
            MultPush(arg1, arg2, retval);
            break;
        case Div:
            DivPush(arg1, arg2, retval);
            break;
        default:
            fprintf(fp, "Unexpected token error\n");
            break;
    }
}

/* BrCommandのpush関数 */
void BrCommandPush(LLVMcode *ptr) {
    BrCommand *tmp = (BrCommand *)malloc(sizeof(BrCommand));
    tmp->ptr = ptr;
    tmp->prev = BrCommandStackPtr;
    BrCommandStackPtr = tmp;
}

/* BrCommandのpop関数 */
void BrCommandPop() {
    BrCommand *p = (BrCommand *)malloc(sizeof(BrCommand));
    BrCommandStackPtr = BrCommandStackPtr->prev;
    free(p);
}

/* バックパッチを行う関数 */
void backpatch(int l) {
    if (BrCommandStackPtr->ptr->command == BrCond) {
        BrCommandStackPtr->ptr->args.brcond.arg3 = l;
    } else {
        BrCommandStackPtr->ptr->args.bruncond.arg1 = l;
    }
    BrCommandPop();
}

void FunPush(Fundecl *tmp) {
    if (declhd == NULL) {
        declhd = tmp;
        decltl = tmp;
    } else {
        decltl->next = tmp;
        decltl = tmp; //線形リストの末尾を新しいデータに変更
    }
    codetl->next = NULL;
    codehd = NULL;
    codetl = NULL;
}

/* 生成コードを印字する関数 */
void displayFactor(Factor factor){
  switch( factor.type ){
  case GLOBAL_VAR:
    fprintf(fp, "@%s", factor.vname );
    break;
  case LOCAL_VAR:
    fprintf(fp, "%%%d", factor.val );
    break;
  case CONSTANT:
    fprintf(fp, "%d", factor.val );
    break;
  default:
    break;
  }
  return;
}

/* 生成コードを印字する関数 */
void displayLlvmcodes(LLVMcode *code){
  if( code == NULL ) return;
  switch( code->command ) {
  case Alloca:
    fprintf(fp, "  ");
    displayFactor( (code->args).alloca.retval );
    fprintf(fp, " = alloca i32, align 4\n");
    break;
  case Global:
    displayFactor( (code->args).global.retval );
    fprintf(fp, " = common global i32  %d, align 4\n", (code->args).global.retval.val);
    break;
  case Store:
    fprintf(fp, "  ");
    fprintf(fp, "store i32 ");
    displayFactor((code->args).store.arg1);
    fprintf(fp, ", i32* ");
    displayFactor((code->args).store.arg2);
    fprintf(fp, ", align 4\n");
    break;
  case Load:
    fprintf(fp, "  ");
    displayFactor((code->args).load.retval);
    fprintf(fp, " = load i32, i32* ");
    displayFactor((code->args).load.arg1);
    fprintf(fp, ", align 4\n");
    break;
  case BrUncond:
    fprintf(fp, "  ");
    fprintf(fp, "br label %%%d\n", code->args.bruncond.arg1);
    break;
  case BrCond:
    fprintf(fp, "  ");
    fprintf(fp, "br i1 ");
    displayFactor((code->args).brcond.arg1);
    fprintf(fp, ", label %%%d, label %%%d\n", code->args.brcond.arg2, code->args.brcond.arg3);
    break;
  case Label:
    fprintf(fp, "\n; <label>:%d:\n", code->args.label.l);
    break;
  case Add:
    fprintf(fp, "  ");
    displayFactor((code->args).add.retval);
    fprintf(fp, " = add nsw i32 ");
    displayFactor((code->args).add.arg1);
    fprintf(fp, ", ");
    displayFactor((code->args).add.arg2);
    fprintf(fp, "\n");
    break;
  case Sub:
    fprintf(fp, "  ");
    displayFactor((code->args).sub.retval);
    fprintf(fp, " = sub nsw i32 ");
    displayFactor((code->args).sub.arg1);
    fprintf(fp, ", ");
    displayFactor((code->args).sub.arg2);
    fprintf(fp, "\n");
    break;
   case Mult:
    fprintf(fp, "  ");
    displayFactor((code->args).mult.retval);
    fprintf(fp, " = mul nsw i32 ");
    displayFactor((code->args).mult.arg1);
    fprintf(fp, ", ");
    displayFactor((code->args).mult.arg2);
    fprintf(fp, "\n");
    break;
   case Div:
    fprintf(fp, "  ");
    displayFactor((code->args).div.retval);
    fprintf(fp, " = sdiv i32 ");
    displayFactor((code->args).div.arg1);
    fprintf(fp, ", ");
    displayFactor((code->args).div.arg2);
    fprintf(fp, "\n");
    break;
  case Icmp:
    fprintf(fp, "  ");
    displayFactor((code->args).icmp.retval);
    fprintf(fp, " = icmp ");
    switch ((code->args).icmp.type) {
      case EQUAL:
        fprintf(fp, "eq");
        break;
      case NE:
        fprintf(fp, "ne");
        break;
      case SGT:
        fprintf(fp, "sgt");
        break;
      case SGE:
        fprintf(fp, "sge");
        break;
      case SLT:
        fprintf(fp, "slt");
        break;
      case SLE:
        fprintf(fp, "sle");
        break;
    }
    fprintf(fp, " i32 ");
    displayFactor((code->args).icmp.arg1);
    fprintf(fp, ", ");
    displayFactor((code->args).icmp.arg2);
    fprintf(fp, "\n");
    break;
  case Read:
    fprintf(fp, "  ");
    displayFactor((code->args).read.retval);
    fprintf(fp, " = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* ");
    fprintf(fp, "%d", (code->args).read.arg1);
    fprintf(fp, "\n");
    break;
  case Write:
    fprintf(fp, "  ");
    displayFactor((code->args).write.retval);
    fprintf(fp, " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 ");
    fprintf(fp, "%d", (code->args).write.arg1);
    fprintf(fp, "\n");
    break;
  case Call:
    fprintf(fp, "  call void @");
    fprintf(fp, "%s()\n", (code->args).call.fname);
    break;
  case Retint:
    fprintf(fp, "  ret i32 ");
    displayFactor((code->args).retint.retval);
    fprintf(fp, "\n");
    break;
  case Retvoid:
    fprintf(fp, "  ret void\n");
    break;
  default:
    break;
  }
  displayLlvmcodes( code->next );
}

/* 生成コードを印字する関数 */
void displayLlvmfundecl( Fundecl *decl ){
  if( decl == NULL ) return;
  if (decl->rettype == INT) {
    fprintf(fp, "define i32 @%s() #0 {\n", decl->fname );
  } else if (decl->rettype == VOID) {
    fprintf(fp, "define void @%s() #0 {\n", decl->fname );
  }
  displayLlvmcodes( decl->codes );
  fprintf(fp, "}\n");
  if( decl->next != NULL ) {
    fprintf(fp, "\n");
    displayLlvmfundecl( decl->next );
  }
  return;
}

