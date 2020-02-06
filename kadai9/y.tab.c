#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20130304

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
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
    FUNC_NAME,
    ARGUMENT,
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
    Factor data; /*記号表のレコード*/
    int lnum;
    int rnum;
    int num;
    struct _TableStack *prev; /*前のノードへのポインタ*/
} TableStack;

/* 引数を管理する線形リスト */
typedef struct _Argument {
    Factor data; /*引数*/
    struct _Argument *next; /*次のノードへのポインタ*/
} Argument;

/* LLVM命令名の定義 */
typedef enum {
  Alloca,   /* alloca */
  Global,   /* global */
  ArrayGlobal, /* arrayglobal */
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
  Retvoid,  /* retvoid*/
  Sext,     /* sext   */
  Getelementptr, /* getelementptr */
  Shl,      /* shl    */
  Ashr      /* ashr   */
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
    struct { /* arrayglobal */
      char vname[256]; int num;
    } arrayglobal;
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
      char fname[256];
      Rettype type;
      Factor ret;
      Argument *arghd; /*先頭アドレス*/
      Argument *argtl; /*末尾　アドレス*/
    } call;
    struct { /* write   */
      Factor arg1; Factor retval;
    } write;
    struct { /* retint */
      Factor arg1; Factor retval;
    } retint;
    struct { /* retvoid */
    } retvoid;
    struct { /* sext */
      Factor arg1; Factor retval;
    } sext;
    struct { /* getelementptr */
      char vname[256]; Factor arg1; Factor retval;
    } getelementptr;
    struct { /* shl */
      Factor arg1; int arg2; Factor retval;
    } shl;
    struct { /* ashr */
      Factor arg1; int arg2; Factor retval;
    } ashr;
  } args;
  /* 次の命令へのポインタ */
  struct llvmcode *next;
} LLVMcode;

/* LLVMの関数定義*/
typedef struct fundecl {
  char fname[256]; /*関数名*/
  unsigned arity; /*引数個数*/
  Argument *arghd;
  Argument *argtl;
  Rettype rettype;
  LLVMcode *codes; /*命令列の線形リストへのポインタ*/
  struct fundecl *next; /*次の関数定義へのポインタ*/
} Fundecl;

/* call命令へのポインタを記憶するためのスタック */
typedef struct callcommand {
    LLVMcode *ptr;
    struct callcommand *prev;
} CallCommand;

/* br命令へのポインタを記憶するためのスタック */
typedef struct brcommand {
    LLVMcode *ptr;
    struct brcommand *prev;
} BrCommand;

FILE *fp; /*result.llのファイルポインタ*/
Scope flag = GLOBAL_VAR; /*変数のスコープはグローバルで初期化*/
unsigned int read_arg = 0;
unsigned int read_decl_flag = 0;
unsigned int write_decl_flag = 0;
unsigned int reg_cntr = 0; /*レジスタ番号管理のためのカウンタ*/

TableStack *TableStackPtr = NULL; /*Tableスタックのスタックポインタ*/
void TablePush(Factor*); /*Tableスタックのpush関数*/
void TablePop(); /*Tableスタックのpop関数*/
void TableShow(); /*Tableスタックの全データを出力するための関数*/

void insert(char*, Scope); /*記号表への追加を行う関数*/
 void insertArray(char*, int, int, Scope);
Factor* lookup(char*); /*記号表から検索を行う関数*/
TableStack* lookupArray(char*);
void delete(); /*記号表から要素を削除する関数*/
void show();

FactorStack *FactorStackPtr = NULL; /*Factorスタックのスタックポインタ*/
void FactorPush(Factor); /*Factorスタックのpush関数*/
Factor FactorPop(); /*Factorスタックのpop関数*/
void Factordelete(); /*Factorスタックのdelete関数*/
Factor makeFactor(Scope, char*, int); /*Factorを作成して返す関数*/

void CallCommandGenPush(char*, Rettype); /*CallCommandのpush関数*/
CallCommand *CallCommandStackPtr = NULL; /*CallCommandのスタックポインタ*/

void BrCommandPush(LLVMcode*); /*BrCommandのpush関数*/
void BrCommandPop(); /*BrCommandのpop関数*/
void backpatch(int);
BrCommand *BrCommandStackPtr = NULL; /*BrCommandのスタックポインタ*/

void AllocaPush(Factor); /*AllocaのLLVMcodeを作成して線形リストに追加する関数*/
void GlobalPush(Factor);
void ArrayGlobalPush(char*, int);
void StorePush(Factor, Factor);
void ArrayStorePush(char*, Factor);
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
void CallPush();
void RetPush(Rettype);
void SextPush(Factor, Factor);
void GetelementptrPush(char*, Factor, Factor);
void ShlPush(Factor, int, Factor);
void AshrPush(Factor, int, Factor);

void cmpcalc(Cmptype); /*比較演算*/
void calc(LLVMcommand); /*四則演算*/
void ArgPush(); /*仮引数設定*/

void LLVMPush(LLVMcode*); /*LLVMcodeのpush関数*/
LLVMcode *codehd = NULL; /* 命令列の先頭のアドレスを保持するポインタ */
LLVMcode *codetl = NULL; /* 命令列の末尾のアドレスを保持するポインタ */

void FunPush(Fundecl*); /*Fundeclのpush関数*/
Fundecl *declhd = NULL; /*関数定義の線形リストの先頭の要素のアドレスを保持するポインタ*/
Fundecl *decltl = NULL; /*関数定義の線形リストの末尾の要素のアドレスを保持するポインタ*/

void displayFactor(Factor);
void displayLlvmcodes(LLVMcode*);
void displayLlvmfundecl(Fundecl*);

#line 266 "parser.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    int num;
    char ident[MAXLENGTH+1];
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 294 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define SBEGIN 257
#define DO 258
#define ELSE 259
#define SEND 260
#define FOR 261
#define FORWARD 262
#define FUNCTION 263
#define IF 264
#define PROCEDURE 265
#define PROGRAM 266
#define READ 267
#define THEN 268
#define TO 269
#define VAR 270
#define WHILE 271
#define WRITE 272
#define PLUS 273
#define MINUS 274
#define MULT 275
#define DIV 276
#define EQ 277
#define NEQ 278
#define LE 279
#define LT 280
#define GE 281
#define GT 282
#define LPAREN 283
#define RPAREN 284
#define LBRACE 285
#define RBRACE 286
#define COMMA 287
#define SEMICOLON 288
#define COLON 289
#define INTERVAL 290
#define PERIOD 291
#define ASSIGN 292
#define NUMBER 293
#define IDENT 294
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    1,    0,    5,    7,    2,    3,    3,    8,    8,    9,
    4,    4,   11,   11,   12,   12,   17,   13,   18,   19,
   13,   15,   21,   14,   22,   23,   14,   20,   16,   24,
   24,    6,    6,    6,    6,    6,    6,    6,    6,    6,
   25,   35,   25,   37,   38,   40,   26,   39,   41,   39,
   42,   43,   44,   27,   45,   46,   47,   48,   28,   29,
   29,   49,   31,   32,   32,   33,   30,   36,   36,   36,
   36,   36,   36,   34,   34,   34,   34,   34,   51,   51,
   51,   52,   52,   52,   52,   55,   54,   53,   53,   50,
   50,   50,   10,   10,   10,   10,
};
static const short yylen[] = {                            2,
    0,    6,    0,    0,    5,    0,    2,    3,    1,    2,
    0,    2,    3,    1,    1,    1,    0,    5,    0,    0,
    9,    1,    0,    5,    0,    0,    9,    1,    2,    3,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    0,    7,    0,    0,    0,    8,    0,    0,    3,
    0,    0,    0,    7,    0,    0,    0,    0,   12,    1,
    4,    1,    3,    4,    7,    4,    0,    3,    3,    3,
    3,    3,    3,    1,    2,    2,    3,    3,    1,    3,
    3,    1,    1,    3,    1,    0,    5,    1,    4,    0,
    1,    3,    1,    6,    3,    8,
};
static const short yydefred[] = {                         0,
    1,    0,    0,    0,    0,    0,    0,    3,    0,    9,
    0,    0,    2,    0,    0,    0,    0,    0,    0,    4,
    0,   14,   15,   16,    8,    0,    0,   28,    0,   22,
    0,    0,    0,    0,    0,   23,    0,   17,    0,    0,
    0,    0,    0,   51,    0,    0,    5,   32,   33,   34,
   35,   36,   37,   38,   39,   40,    0,   13,    0,    0,
    0,    0,    0,    0,   31,    0,    0,    0,    0,    0,
   83,    0,    0,   44,    0,   79,   82,   85,    0,    0,
    0,    0,    0,    0,   94,    0,    0,   24,    0,   18,
    0,   63,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   52,    0,    0,    0,    0,    0,    0,   29,    0,
    0,   30,    0,   84,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   45,   80,   81,   64,    0,    0,
   66,    0,   61,    0,   96,   26,   20,    0,   89,    0,
    0,    0,   53,    0,    0,    0,    0,   56,   87,   46,
    0,    0,    0,   27,   21,    0,    0,   65,   54,    0,
    0,   49,   47,    0,    0,   58,   50,    0,   59,
};
static const short yydgoto[] = {                          2,
    3,    7,   87,   20,   14,   47,   32,    9,   10,   12,
   21,   22,   23,   24,   31,   88,   63,   39,  157,   29,
   61,   37,  156,   66,   48,   49,   50,   51,   52,   53,
   54,   55,   56,   73,  142,   74,  108,  151,  173,  167,
  175,   80,  140,  162,  148,  166,  174,  178,   57,  117,
   75,   76,   77,   78,   99,
};
static const short yysindex[] = {                      -261,
    0,    0, -255, -237, -211, -229, -207,    0, -197,    0,
 -137,  -83,    0, -241, -211, -107,  -86,  -85,  -80,    0,
  -73,    0,    0,    0,    0,  -74,  -68,    0,  -70,    0,
  -65, -103, -241,  -64,  -57,    0,  -63,    0,  -59, -103,
  -75, -267,  -58,    0,  -55, -264,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -45,    0,  -44,  -42,
 -211, -229, -211, -229,    0, -257,  -49, -138, -138, -267,
    0,  -36,  105,    0, -231,    0,    0,    0,  -43, -267,
 -267, -267, -267, -267,    0,  -41, -103,    0, -215,    0,
 -194,    0, -103, -267, -231, -231, -254, -267,  -33, -138,
 -138, -267, -267, -267, -267, -267, -267,  -15, -138, -138,
 -100,    0, -114,  -92,  -92,  -92, -122,  -32,    0,  -31,
  -30,    0,  -92,    0, -263, -267, -231, -231,  -92,  -92,
  -92,  -92,  -92,  -92,    0,    0,    0,    0, -267,    1,
    0,  -24,    0, -267,    0,    0,    0,   -6,    0, -121,
 -103, -146,    0,  -22,  -92, -211, -211,    0,    0,    0,
  -12, -103, -267,    0,    0, -267,   17,    0,    0,  -92,
  -92,    0,    0,   19, -103,    0,    0, -103,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0, -158,    0,    0,    0,    0,    0,
 -220,   -5,    0, -141, -209,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -145,    0,   -1,    0,
    2,   -7, -120,    0,    0,    0,    0,    0,    0, -256,
    0,    0,    0,    0,    0, -217,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -242,    0,    0,    0,
 -184,    0, -184,    0,    0,    0,    0,    0,    0,    0,
    0,  -81,    0,    0,  -47,    0,    0,    0,    0,    0,
    0,    0,    0, -112,    0,    0,   -2,    0,    0,    0,
    0,    0, -256,    0,  -13,   21,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    5, -199, -111,    0,    0,    0,    0,
    0,    0,   18,    0,    0, -112,   55,   89, -182, -160,
 -156, -143, -134, -101,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -159,    0,    0,    0, -104, -184, -184,    0,    0,    0,
    0, -159,    0,    0,    0,    0, -210,    0,    0, -142,
   30,    0,    0,    0, -159,    0,    0, -159,    0,
};
static const short yygindex[] = {                         0,
    0,    0,  287,    0,    0,  -40,    0,    0,  278,   57,
    0,  263,    0,    0,    0,  -61,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -69,    0,  217,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  178,
  -60,   80,    0,    0,    0,
};
#define YYTABLESIZE 387
static const short yytable[] = {                         65,
   97,   90,   92,   67,    1,   68,   69,   95,   96,  100,
  101,  113,  114,  115,  116,   70,   60,   60,  100,  101,
   82,   18,  149,   19,  123,   71,   72,   83,  125,  124,
   93,   67,  129,  130,  131,  132,  133,  134,    4,  127,
  128,   62,   62,  109,  110,   60,  119,    7,   60,   48,
    5,    7,  122,    7,    7,    7,  116,    7,    6,   41,
   41,    7,    7,   93,   11,   62,   93,   93,  120,  152,
   62,   17,    6,   62,  155,   68,    6,   48,    7,    6,
   48,    7,    6,   13,    7,   68,    6,    6,   41,  121,
   15,   41,   17,  170,  164,  165,  171,   69,    6,   67,
   67,   71,    6,    6,    6,    6,    6,   69,    6,    6,
  160,   71,    6,    6,   70,   11,   43,   43,   89,   11,
   91,  169,   11,   73,   70,   11,  100,  101,   67,   11,
   11,   67,    6,   73,  177,    6,   12,  179,   95,  161,
   12,   95,   95,   12,   70,   43,   12,   16,   43,   11,
   12,   12,   11,   40,   71,   72,   72,   41,  100,  101,
   42,  143,  159,   43,  144,  144,   72,   44,   45,  141,
   12,   90,   91,   12,   90,   91,   88,   88,   88,   92,
  100,  101,   92,  138,  139,   26,   88,   88,  136,  137,
   46,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   86,   88,   17,   88,   88,   88,   27,   28,   88,
   74,   74,   74,   30,   33,   34,   35,   36,   67,   62,
   74,   74,   38,   64,   79,   74,   74,   81,   59,   74,
   74,   74,   74,   74,   74,   60,   74,   84,   74,   74,
   74,   85,   94,   74,   75,   75,   75,   86,   98,  126,
  111,  118,  135,  145,   75,   75,  146,  147,  153,   75,
   75,  154,  158,   75,   75,   75,   75,   75,   75,  163,
   75,  168,   75,   75,   75,  172,  176,   75,   76,   76,
   76,   25,   10,   67,   19,   67,   55,   57,   76,   76,
   42,    8,   25,   76,   76,   58,  112,   76,   76,   76,
   76,   76,   76,  150,   76,    0,   76,   76,   76,    0,
    0,   76,   77,   77,   77,    0,    0,    0,    0,    0,
    0,    0,   77,   77,    0,    0,    0,   77,   77,    0,
    0,   77,   77,   77,   77,   77,   77,    0,   77,    0,
   77,   77,   77,    0,    0,   77,   78,   78,   78,    0,
    0,    0,    0,    0,    0,    0,   78,   78,    0,    0,
    0,   78,   78,    0,    0,   78,   78,   78,   78,   78,
   78,    0,   78,    0,   78,   78,   78,  100,  101,   78,
    0,  102,  103,  104,  105,  106,  107,
};
static const short yycheck[] = {                         40,
   70,   63,  260,  260,  266,  273,  274,   68,   69,  273,
  274,   81,   82,   83,   84,  283,  259,  260,  273,  274,
  285,  263,  286,  265,   94,  293,  294,  292,   98,  284,
  288,  288,  102,  103,  104,  105,  106,  107,  294,  100,
  101,  259,  260,  275,  276,  288,   87,  257,  291,  260,
  288,  261,   93,  263,  264,  265,  126,  267,  270,  259,
  260,  271,  272,  284,  294,  283,  287,  288,  284,  139,
  288,  287,  257,  291,  144,  258,  261,  288,  288,  264,
  291,  291,  267,  291,  294,  268,  271,  272,  288,  284,
  288,  291,  287,  163,  156,  157,  166,  258,  257,  259,
  260,  258,  261,  288,  263,  264,  265,  268,  267,  294,
  151,  268,  271,  272,  258,  257,  259,  260,   62,  261,
   64,  162,  264,  258,  268,  267,  273,  274,  288,  271,
  272,  291,  291,  268,  175,  294,  257,  178,  284,  286,
  261,  287,  288,  264,  283,  288,  267,  285,  291,  291,
  271,  272,  294,  257,  293,  294,  258,  261,  273,  274,
  264,  284,  284,  267,  287,  287,  268,  271,  272,  284,
  291,  284,  284,  294,  287,  287,  258,  259,  260,  284,
  273,  274,  287,  284,  285,  293,  268,  269,  109,  110,
  294,  273,  274,  275,  276,  277,  278,  279,  280,  281,
  282,  283,  284,  287,  286,  287,  288,  294,  294,  291,
  258,  259,  260,  294,  288,  290,  285,  288,  294,  283,
  268,  269,  288,  283,  283,  273,  274,  283,  293,  277,
  278,  279,  280,  281,  282,  293,  284,  283,  286,  287,
  288,  286,  292,  291,  258,  259,  260,  290,  285,  283,
  294,  293,  268,  286,  268,  269,  288,  288,  258,  273,
  274,  286,  269,  277,  278,  279,  280,  281,  282,  292,
  284,  284,  286,  287,  288,  259,  258,  291,  258,  259,
  260,  283,  288,  291,  283,  288,  269,  258,  268,  269,
  286,    5,   15,  273,  274,   33,   80,  277,  278,  279,
  280,  281,  282,  126,  284,   -1,  286,  287,  288,   -1,
   -1,  291,  258,  259,  260,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  268,  269,   -1,   -1,   -1,  273,  274,   -1,
   -1,  277,  278,  279,  280,  281,  282,   -1,  284,   -1,
  286,  287,  288,   -1,   -1,  291,  258,  259,  260,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  268,  269,   -1,   -1,
   -1,  273,  274,   -1,   -1,  277,  278,  279,  280,  281,
  282,   -1,  284,   -1,  286,  287,  288,  273,  274,  291,
   -1,  277,  278,  279,  280,  281,  282,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 294
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"SBEGIN","DO","ELSE","SEND","FOR",
"FORWARD","FUNCTION","IF","PROCEDURE","PROGRAM","READ","THEN","TO","VAR",
"WHILE","WRITE","PLUS","MINUS","MULT","DIV","EQ","NEQ","LE","LT","GE","GT",
"LPAREN","RPAREN","LBRACE","RBRACE","COMMA","SEMICOLON","COLON","INTERVAL",
"PERIOD","ASSIGN","NUMBER","IDENT",
};
static const char *yyrule[] = {
"$accept : program",
"$$1 :",
"program : PROGRAM $$1 IDENT SEMICOLON outblock PERIOD",
"$$2 :",
"$$3 :",
"outblock : var_decl_part $$2 subprog_decl_part $$3 statement",
"var_decl_part :",
"var_decl_part : var_decl_list SEMICOLON",
"var_decl_list : var_decl_list SEMICOLON var_decl",
"var_decl_list : var_decl",
"var_decl : VAR id_list",
"subprog_decl_part :",
"subprog_decl_part : subprog_decl_list SEMICOLON",
"subprog_decl_list : subprog_decl_list SEMICOLON subprog_decl",
"subprog_decl_list : subprog_decl",
"subprog_decl : proc_decl",
"subprog_decl : func_decl",
"$$4 :",
"proc_decl : PROCEDURE proc_name SEMICOLON $$4 inblock",
"$$5 :",
"$$6 :",
"proc_decl : PROCEDURE proc_name $$5 LPAREN id_list RPAREN SEMICOLON $$6 inblock",
"proc_name : IDENT",
"$$7 :",
"func_decl : FUNCTION func_name SEMICOLON $$7 inblock",
"$$8 :",
"$$9 :",
"func_decl : FUNCTION func_name $$8 LPAREN id_list RPAREN SEMICOLON $$9 inblock",
"func_name : IDENT",
"inblock : var_decl_part statement",
"statement_list : statement_list SEMICOLON statement",
"statement_list : statement",
"statement : assignment_statement",
"statement : if_statement",
"statement : while_statement",
"statement : for_statement",
"statement : proc_call_statement",
"statement : null_statement",
"statement : block_statement",
"statement : read_statement",
"statement : write_statement",
"assignment_statement : IDENT ASSIGN expression",
"$$10 :",
"assignment_statement : IDENT LBRACE expression $$10 RBRACE ASSIGN expression",
"$$11 :",
"$$12 :",
"$$13 :",
"if_statement : IF condition $$11 THEN $$12 statement $$13 else_statement",
"else_statement :",
"$$14 :",
"else_statement : ELSE $$14 statement",
"$$15 :",
"$$16 :",
"$$17 :",
"while_statement : WHILE $$15 condition $$16 DO $$17 statement",
"$$18 :",
"$$19 :",
"$$20 :",
"$$21 :",
"for_statement : FOR IDENT ASSIGN expression $$18 TO $$19 expression $$20 DO $$21 statement",
"proc_call_statement : proc_call_name",
"proc_call_statement : proc_call_name LPAREN arg_list RPAREN",
"proc_call_name : IDENT",
"block_statement : SBEGIN statement_list SEND",
"read_statement : READ LPAREN IDENT RPAREN",
"read_statement : READ LPAREN IDENT LBRACE expression RBRACE RPAREN",
"write_statement : WRITE LPAREN expression RPAREN",
"null_statement :",
"condition : expression EQ expression",
"condition : expression NEQ expression",
"condition : expression LT expression",
"condition : expression LE expression",
"condition : expression GT expression",
"condition : expression GE expression",
"expression : term",
"expression : PLUS term",
"expression : MINUS term",
"expression : expression PLUS term",
"expression : expression MINUS term",
"term : factor",
"term : term MULT factor",
"term : term DIV factor",
"factor : var_name",
"factor : NUMBER",
"factor : LPAREN expression RPAREN",
"factor : func_call",
"$$22 :",
"func_call : IDENT $$22 LPAREN arg_list RPAREN",
"var_name : IDENT",
"var_name : IDENT LBRACE expression RBRACE",
"arg_list :",
"arg_list : expression",
"arg_list : arg_list COMMA expression",
"id_list : IDENT",
"id_list : IDENT LBRACE NUMBER INTERVAL NUMBER RBRACE",
"id_list : id_list COMMA IDENT",
"id_list : id_list COMMA IDENT LBRACE NUMBER INTERVAL NUMBER RBRACE",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 577 "parser.y"

yyerror(char *s)
{
  extern int yylineno;
  extern char *yytext;
  fprintf(stderr, "%s %s in line %d\n", s, yytext, yylineno);
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
    } else if (flag == ARGUMENT) {
        f->type = LOCAL_VAR;
        f->val = reg_cntr++;
        Argument *a = (Argument *)malloc(sizeof(Argument));
        a->data = *f;
        a->next = NULL;
        if (decltl->arghd == NULL) {
            decltl->arghd = a;
            decltl->argtl = a;
        } else {
            decltl->argtl->next = a;
            decltl->argtl = a;
        }
        decltl->arity++;
    } else if (flag == PROC_NAME) {
        Fundecl *tmp = (Fundecl *)malloc(sizeof(Fundecl));
        strcpy(tmp->fname, s);
        tmp->arity = 0;
        tmp->rettype = VOID;
        tmp->codes = NULL;
        FunPush(tmp);
    } else {
        Fundecl *tmp = (Fundecl *)malloc(sizeof(Fundecl));
        strcpy(tmp->fname, s);
        tmp->arity = 0;
        tmp->rettype = INT;
        tmp->codes = NULL;
        FunPush(tmp);
    }
    TablePush(f); //記号表への追加
    show(); //記号表のデータを出力
}

void insertArray(char* s, int lnum, int rnum, Scope flag) {
    TableStack *tmp = (TableStack *)malloc(sizeof(TableStack));
    tmp->prev = TableStackPtr; //新しいデータのprevは現先頭データを指す

    //データのレコード部分を作成
    strcpy(tmp->data.vname ,s);
    tmp->data.type = flag;
    tmp->lnum = lnum;
    tmp->rnum = rnum;
    tmp->num = rnum-lnum+1;

    if (flag == GLOBAL_VAR) {
        ArrayGlobalPush(s, rnum-lnum+1);
    }

    TableStackPtr = tmp; //スタックの先頭を新しいデータに変更
}

/* 記号表から検索を行う関数 */
Factor* lookup(char *s) {
    TableStack *p = TableStackPtr;
    while (strcmp(p->data.vname, s) != 0) { //引数と一致する記号を見つけるまで
        if (p->prev == NULL) { //先頭に達しても見つからない場合として終了
            printf("%s not found\n", s);
            return NULL;
        }
	      p = p->prev; //スタックの一つ前のデータへ辿る
    }
    return &(p->data);
}

/* 記号表からarray検索を行う関数 */
TableStack* lookupArray(char *s) {
    TableStack *p = TableStackPtr;
    while (strcmp(p->data.vname, s) != 0) { //引数と一致する記号を見つけるまで
        if (p->prev == NULL) { //先頭に達しても見つからない場合として終了
            printf("%s not found\n", s);
            return NULL;
        }
	      p = p->prev; //スタックの一つ前のデータへ辿る
    }
    return p;
}

/* 記号表の削除を行う関数 */
void delete() {
    while (TableStackPtr->data.type == LOCAL_VAR) { //局所変数である間
        TablePop(); //記号表から1つ削除
    }
    show(); //記号表の出力
    Factordelete();
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
              kind = "FUNC_NAME";
              break;
          default:
              break;
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

/* Factorの作成を行う関数 */
Factor makeFactor(Scope type, char* vname, int val) {
    Factor f;
    f.type = type;
    strcpy(f.vname, vname);
    f.val = val;
    return f;
}

//Factorスタックのpush関数
void FactorPush(Factor f) {
    FactorStack *tmp = (FactorStack *)malloc(sizeof(FactorStack));
    if (f.type == GLOBAL_VAR || (f.type == LOCAL_VAR && strcmp(f.vname, "") != 0)) {
        Factor _f = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(f, _f);
        tmp->data = _f;
    } else {
        tmp->data = f;
    }
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

void Factordelete() {
    while (FactorStackPtr != NULL && FactorStackPtr->data.type == LOCAL_VAR) {
        FactorPop();
    }
}

/* 比較演算 */
void cmpcalc(Cmptype type) {
    Factor arg1, arg2, retval;
    arg2 = FactorPop();
    arg1 = FactorPop();

    retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
    FactorPush(retval); //結果を突っ込む
    IcmpPush(type, arg1, arg2, retval);
}

//四則演算
void calc(LLVMcommand ope) {
    Factor arg1, arg2, retval;
    arg2 = FactorPop();
    arg1 = FactorPop();
    int n = 0, flag = 1;

    retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
    FactorPush(retval); //結果を突っ込む

    switch (ope) { //オペレータによって場合分け
        case Add:
            AddPush(arg1, arg2, retval);
            break;
        case Sub:
            SubPush(arg1, arg2, retval);
            break;
        case Mult:
            if (arg2.type == CONSTANT) {
                int x = arg2.val;
                if (x < 0) {
                    x = -x;
                }
                while (x > 1) {
                    if (x % 2 == 1) {
                        flag = 0;
                        break;
                    }
                    x >>= 1;
                    n++;
                }
            } else {
                flag = 0;
            }
            if (flag) {
                ShlPush(arg1, n, retval);
            } else {
                MultPush(arg1, arg2, retval);
            }
            break;
        case Div:
            if (arg2.type == CONSTANT) {
                int x = arg2.val;
                if (x < 0) {
                    x = -x;
                }
                while (x > 1) {
                    if (x % 2 == 1) {
                        flag = 0;
                        break;
                    }
                    x >>= 1;
                    n++;
                }
            } else {
                flag = 0;
            }
            if (flag) {
                AshrPush(arg1, n, retval);
            } else {
                DivPush(arg1, arg2, retval);
            }
            break;
        default:
            printf("Unexpected token error\n");
            break;
    }
}

/* 仮引数設定 */
void ArgPush() {
    Argument *a = (Argument *)malloc(sizeof(Argument));
    a->data = FactorPop();
    a->next = NULL;
    if (CallCommandStackPtr->ptr->args.call.arghd == NULL) {
        CallCommandStackPtr->ptr->args.call.arghd = a;
        CallCommandStackPtr->ptr->args.call.argtl = a;
    } else {
        CallCommandStackPtr->ptr->args.call.argtl->next = a;
        CallCommandStackPtr->ptr->args.call.argtl = a;
    }
}

/* CallCommandを生成してスタックに詰む関数 */
void CallCommandGenPush(char* fname, Rettype type) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Call;
    strcpy(tmp->args.call.fname, fname);
    tmp->args.call.type = type;
    tmp->args.call.arghd = NULL;
    tmp->args.call.argtl = NULL;
    tmp->next = NULL;
    CallCommand *tmp2 = (CallCommand *)malloc(sizeof(CallCommand));
    tmp2->ptr = tmp;
    tmp2->prev = CallCommandStackPtr;
    CallCommandStackPtr = tmp2;
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

void ArrayGlobalPush(char* vname, int num) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = ArrayGlobal;
    strcpy(tmp->args.arrayglobal.vname, vname);
    tmp->args.arrayglobal.num = num;
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

void ArrayStorePush(char* s, Factor f) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));

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
    tmp->args.read.arg1 = arg1;
    tmp->args.read.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

/* WriteのLLVMcodeを作成し線形リストに追加する関数 */
void WritePush(Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Write;
    if (arg1.type == GLOBAL_VAR) {
        Factor _arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
        LoadPush(arg1, _arg1);
        tmp->args.write.arg1 = _arg1;
    } else {
        tmp->args.write.arg1 = arg1;
    }
    tmp->args.write.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

void CallPush() {
    LLVMPush(CallCommandStackPtr->ptr);
    if (codetl->args.call.type == INT) {
        FactorPush(makeFactor(LOCAL_VAR, "", reg_cntr));
        codetl->args.call.ret = makeFactor(LOCAL_VAR, "", reg_cntr++);
    }
    CallCommandStackPtr = CallCommandStackPtr->prev;
}

void RetPush(Rettype rettype) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    if (rettype == INT) {
      tmp->command = Retint;
      if (strcmp(decltl->fname, "main") == 0) {
        tmp->args.retint.retval = makeFactor(CONSTANT, "", 0);
      } else {
        Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr);
        LoadPush(*lookup(decltl->fname), retval);
        tmp->args.retint.retval = retval;
      }
    } else if (rettype == VOID) {
      tmp->command = Retvoid;
    }
    tmp->next = NULL;
    LLVMPush(tmp);
}

void SextPush(Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Sext;
    tmp->args.sext.arg1 = arg1;
    tmp->args.sext.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

void GetelementptrPush(char* vname, Factor arg1, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Getelementptr;
    strcpy(tmp->args.getelementptr.vname, vname);
    tmp->args.getelementptr.arg1 = arg1;
    tmp->args.getelementptr.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

void ShlPush(Factor arg1, int arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Shl;
    tmp->args.shl.arg1 = arg1;
    tmp->args.shl.arg2 = arg2;
    tmp->args.shl.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
}

void AshrPush(Factor arg1, int arg2, Factor retval) {
    LLVMcode *tmp = (LLVMcode *)malloc(sizeof(LLVMcode));
    tmp->command = Ashr;
    tmp->args.ashr.arg1 = arg1;
    tmp->args.ashr.arg2 = arg2;
    tmp->args.ashr.retval = retval;
    tmp->next = NULL;
    LLVMPush(tmp);
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
  Argument *a;
  char *vname;
  int num;
  switch( code->command ) {
  case Alloca:
    fprintf(fp, "  ");
    displayFactor( (code->args).alloca.retval );
    fprintf(fp, " = alloca i32, align 4\n");
    break;
  case Global:
    displayFactor( (code->args).global.retval );
    fprintf(fp, " = common global i32 %d, align 4\n", (code->args).global.retval.val);
    break;
  case ArrayGlobal:
    fprintf(fp, "@%s = common global [%d x i32] zeroinitializer, align 16", (code->args).arrayglobal.vname, (code->args).arrayglobal.num);
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
    displayFactor((code->args).read.arg1);
    fprintf(fp, ")\n");
    break;
  case Write:
    fprintf(fp, "  ");
    displayFactor((code->args).write.retval);
    fprintf(fp, " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 ");
    displayFactor((code->args).write.arg1);
    fprintf(fp, ")");
    fprintf(fp, "\n");
    break;
  case Call:
    if ((code->args).call.type == VOID) {
      fprintf(fp, "  call void @");
      fprintf(fp, "%s(", (code->args).call.fname);
      a = (code->args).call.arghd;
      for (; a != (code->args).call.argtl; a = a->next) {
        fprintf(fp, "i32 ");
        displayFactor(a->data);
        fprintf(fp, ", ");
      }
      if (a != NULL) {
        fprintf(fp, "i32 ");
        displayFactor(a->data);
      }
      fprintf(fp, ")\n");
    } else {
      fprintf(fp, "  ");
      displayFactor((code->args).call.ret);
      fprintf(fp, " = call i32 @");
      fprintf(fp, "%s(", (code->args).call.fname);
      for (a = (code->args).call.arghd; a != (code->args).call.argtl; a = a->next) {
        fprintf(fp, "i32 ");
        displayFactor(a->data);
        fprintf(fp, ", ");
      }
      if (a != NULL) {
        fprintf(fp, "i32 ");
        displayFactor(a->data);
      }
      fprintf(fp, ")\n");
    }
    break;
  case Retint:
    fprintf(fp, "  ret i32 ");
    displayFactor((code->args).retint.retval);
    fprintf(fp, "\n");
    break;
  case Retvoid:
    fprintf(fp, "  ret void\n");
    break;
  case Sext:
    fprintf(fp, "  ");
    displayFactor((code->args).sext.retval);
    fprintf(fp, " = sext i32 ");
    displayFactor((code->args).sext.arg1);
    fprintf(fp, " to i64\n");
    break;
  case Getelementptr:
    fprintf(fp, "  ");
    displayFactor((code->args).getelementptr.retval);
    strcpy(vname, (code->args).getelementptr.vname);
    num = lookupArray(vname)->num;
    fprintf(fp, " = getelementptr inbounds [%d x i32], [%d x i32]* @%s, i64 0, i64 ", num, num, vname);
    displayFactor((code->args).getelementptr.arg1);
    fprintf(fp, "\n");
  case Shl:
    fprintf(fp, "  ");
    displayFactor((code->args).shl.retval);
    fprintf(fp, " = shl i32 ");
    displayFactor((code->args).shl.arg1);
    fprintf(fp, ", %d", (code->args).shl.arg2);
    fprintf(fp, "\n");
    break;
  case Ashr:
    fprintf(fp, "  ");
    displayFactor((code->args).shl.retval);
    fprintf(fp, " = ashr i32 ");
    displayFactor((code->args).shl.arg1);
    fprintf(fp, ", %d", (code->args).shl.arg2);
    fprintf(fp, "\n");
    break;
  default:
    break;
  }
  displayLlvmcodes( code->next );
}

/* 生成コードを印字する関数 */
void displayLlvmfundecl( Fundecl *decl ){
  int i;
  if( decl == NULL ) return;
  if (decl->rettype == INT) {
    fprintf(fp, "define i32 @%s(", decl->fname );
    for (i = 1; i < decl->arity; i++) {
      fprintf(fp, "i32, ");
    }
    if (decl->arity >= 1) {
      fprintf(fp, "i32");
    }
    fprintf(fp, ") #0 {\n");
  } else {
    fprintf(fp, "define void @%s(", decl->fname );
    for (i = 1; i < decl->arity; i++) {
      fprintf(fp, "i32, ");
    }
    if (decl->arity >= 1) {
      fprintf(fp, "i32");
    }
    fprintf(fp, ") #0 {\n");
  }
  displayLlvmcodes( decl->codes );
  fprintf(fp, "}\n");
  if( decl->next != NULL ) {
    fprintf(fp, "\n");
    displayLlvmfundecl( decl->next );
  }
  return;
}
#line 1552 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 290 "parser.y"
	{fp = fopen("result.ll", "w");}
break;
case 2:
#line 291 "parser.y"
	{displayLlvmfundecl(declhd); fclose(fp);}
break;
case 3:
#line 295 "parser.y"
	{displayLlvmcodes(codehd);}
break;
case 4:
#line 296 "parser.y"
	{/* main 関数 */
                         reg_cntr = 1;
	                       insert("main", FUNC_NAME);
	                       flag = LOCAL_VAR;}
break;
case 5:
#line 300 "parser.y"
	{RetPush(INT);}
break;
case 15:
#line 328 "parser.y"
	{RetPush(decltl->rettype); delete(); flag = GLOBAL_VAR;}
break;
case 16:
#line 329 "parser.y"
	{RetPush(decltl->rettype); delete(); flag = GLOBAL_VAR;}
break;
case 17:
#line 333 "parser.y"
	{flag = LOCAL_VAR; reg_cntr = 1;}
break;
case 19:
#line 334 "parser.y"
	{flag = ARGUMENT; reg_cntr = 0;}
break;
case 20:
#line 335 "parser.y"
	{flag = LOCAL_VAR;
                                       reg_cntr++;
                                       int i;
                                       for (i = 0; i < decltl->arity; i++) {
                                           AllocaPush(makeFactor(LOCAL_VAR, "", reg_cntr++));
                                       }
                                       Argument *a = decltl->arghd;
                                       i = 0;
                                       for (; a != decltl->argtl; a = a->next) {
                                           lookup(a->data.vname)->val = reg_cntr-decltl->arity;
                                           StorePush(a->data, makeFactor(LOCAL_VAR, "", reg_cntr-decltl->arity+i++));
                                       }
                                       lookup(a->data.vname)->val = reg_cntr-decltl->arity+i;
                                       StorePush(a->data, makeFactor(LOCAL_VAR, "", reg_cntr-decltl->arity+i));
                                       show();}
break;
case 22:
#line 354 "parser.y"
	{insert(yystack.l_mark[0].ident, PROC_NAME);
             if (decltl->rettype == INT)
                 AllocaPush(makeFactor(LOCAL_VAR, yystack.l_mark[0].ident, reg_cntr++));
            }
break;
case 23:
#line 361 "parser.y"
	{flag = LOCAL_VAR; reg_cntr = 1;}
break;
case 25:
#line 362 "parser.y"
	{flag = ARGUMENT; reg_cntr = 0;}
break;
case 26:
#line 363 "parser.y"
	{flag = LOCAL_VAR;
                                       reg_cntr++;
                                       insert(decltl->fname, LOCAL_VAR);
                                       int i;
                                       for (i = 0; i < decltl->arity; i++) {
                                           AllocaPush(makeFactor(LOCAL_VAR, "", reg_cntr++));
                                       }
                                       Argument *a = decltl->arghd;
                                       i = 0;
                                       for (; a != decltl->argtl; a = a->next) {
                                           lookup(a->data.vname)->val = reg_cntr-decltl->arity+i;
                                           StorePush(a->data, makeFactor(LOCAL_VAR, "", reg_cntr-decltl->arity+i++));
                                       }
                                       lookup(a->data.vname)->val = reg_cntr-decltl->arity+i;
                                       StorePush(a->data, makeFactor(LOCAL_VAR, "", reg_cntr-decltl->arity+i));
                                       show(); }
break;
case 28:
#line 383 "parser.y"
	{insert(yystack.l_mark[0].ident, FUNC_NAME);}
break;
case 41:
#line 408 "parser.y"
	{StorePush(FactorStackPtr->data, *lookup(yystack.l_mark[-2].ident));}
break;
case 42:
#line 409 "parser.y"
	{FactorPush(makeFactor(CONSTANT, "", lookupArray(yystack.l_mark[-2].ident)->lnum));
                               calc(Sub);
                               Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                               SextPush(FactorPop(), arg1);
                               Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                               GetelementptrPush(yystack.l_mark[-2].ident, arg1, retval);
                               FactorPush(retval);
                               }
break;
case 43:
#line 417 "parser.y"
	{Factor arg1 = FactorPop();
                                Factor retval = FactorPop();
                                StorePush(arg1, retval);}
break;
case 44:
#line 423 "parser.y"
	{BrCondPush(FactorStackPtr->data, reg_cntr, -1);}
break;
case 45:
#line 424 "parser.y"
	{LabelPush(reg_cntr++);}
break;
case 46:
#line 425 "parser.y"
	{backpatch(reg_cntr);}
break;
case 47:
#line 426 "parser.y"
	{BrCommandPop(); LabelPush(reg_cntr++);}
break;
case 48:
#line 430 "parser.y"
	{BrUncondPush(reg_cntr);}
break;
case 49:
#line 431 "parser.y"
	{BrUncondPush(-1); LabelPush(reg_cntr++);}
break;
case 50:
#line 432 "parser.y"
	{backpatch(reg_cntr);
                 BrUncondPush(reg_cntr);}
break;
case 51:
#line 437 "parser.y"
	{BrUncondPush(reg_cntr); LabelPush(reg_cntr++);}
break;
case 52:
#line 438 "parser.y"
	{BrCondPush(FactorStackPtr->data, reg_cntr, -1);}
break;
case 53:
#line 439 "parser.y"
	{LabelPush(reg_cntr++);}
break;
case 54:
#line 440 "parser.y"
	{backpatch(reg_cntr); BrUncondPush(BrCommandStackPtr->ptr->args.bruncond.arg1); BrCommandPop(); BrCommandPop(); LabelPush(reg_cntr++);}
break;
case 55:
#line 444 "parser.y"
	{StorePush(FactorStackPtr->data, *lookup(yystack.l_mark[-2].ident));
                                   BrUncondPush(reg_cntr);}
break;
case 56:
#line 446 "parser.y"
	{LabelPush(reg_cntr++);}
break;
case 57:
#line 447 "parser.y"
	{Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                  LoadPush(*lookup(yystack.l_mark[-6].ident), arg1);
                  Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                  IcmpPush(SLE, arg1, FactorStackPtr->data, retval); BrCondPush(retval, reg_cntr, -1);}
break;
case 58:
#line 451 "parser.y"
	{LabelPush(reg_cntr++);}
break;
case 59:
#line 452 "parser.y"
	{Factor incrementee = *lookup(yystack.l_mark[-10].ident);
                 Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr);
                 LoadPush(incrementee, makeFactor(LOCAL_VAR, "", reg_cntr++));
                 Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                 AddPush(arg1, makeFactor(CONSTANT, "", 1), retval);
                 StorePush(retval, incrementee);
                 backpatch(reg_cntr);
                 BrUncondPush(BrCommandStackPtr->ptr->args.bruncond.arg1);
                              BrCommandPop();
                              BrCommandPop();
                              LabelPush(reg_cntr++);}
break;
case 60:
#line 466 "parser.y"
	{CallPush();}
break;
case 61:
#line 467 "parser.y"
	{CallPush();}
break;
case 62:
#line 471 "parser.y"
	{CallCommandGenPush(yystack.l_mark[0].ident, VOID);}
break;
case 64:
#line 479 "parser.y"
	{if (read_decl_flag == 0) {
                                    fprintf(fp, "@.str = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
                                    fprintf(fp, "declare i32 @scanf(i8*, ...) #1\n");
                                    read_decl_flag = 1;
                                    read_arg = 0;
                                }
                                ReadPush(*lookup(yystack.l_mark[-1].ident), makeFactor(LOCAL_VAR, "", reg_cntr++));}
break;
case 65:
#line 486 "parser.y"
	{if (read_decl_flag == 0) {
                                                             fprintf(fp, "@.str = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
                                                             fprintf(fp, "declare i32 @scanf(i8*, ...) #1\n");
                                                             read_decl_flag = 1;
                                                             read_arg = 0;
                                                         }
                                                         FactorPush(makeFactor(CONSTANT, "", lookupArray(yystack.l_mark[-4].ident)->lnum));
                                                         calc(Sub);
                                                         Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                                         SextPush(FactorPop(), arg1);
                                                         Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                                         GetelementptrPush(yystack.l_mark[-4].ident, arg1, retval);
                                                         ReadPush(retval, makeFactor(LOCAL_VAR, "", reg_cntr++));}
break;
case 66:
#line 502 "parser.y"
	{WritePush(FactorStackPtr->data, makeFactor(LOCAL_VAR, "", reg_cntr++));
                                      if (write_decl_flag == 0) {
                                      fprintf(fp, "@.str.1 = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
                                      fprintf(fp, "declare i32 @printf(i8*, ...) #1\n");
                                      write_decl_flag = 1;
                                      }}
break;
case 68:
#line 515 "parser.y"
	{cmpcalc(EQUAL);}
break;
case 69:
#line 516 "parser.y"
	{cmpcalc(NE);}
break;
case 70:
#line 517 "parser.y"
	{cmpcalc(SLT);}
break;
case 71:
#line 518 "parser.y"
	{cmpcalc(SLE);}
break;
case 72:
#line 519 "parser.y"
	{cmpcalc(SGT);}
break;
case 73:
#line 520 "parser.y"
	{cmpcalc(SGE);}
break;
case 75:
#line 525 "parser.y"
	{FactorPush(makeFactor(CONSTANT, "", 1)); calc(Mult);}
break;
case 76:
#line 526 "parser.y"
	{FactorPush(makeFactor(CONSTANT, "", -1)); calc(Mult);}
break;
case 77:
#line 527 "parser.y"
	{calc(Add);}
break;
case 78:
#line 528 "parser.y"
	{calc(Sub);}
break;
case 80:
#line 533 "parser.y"
	{calc(Mult);}
break;
case 81:
#line 534 "parser.y"
	{calc(Div);}
break;
case 83:
#line 539 "parser.y"
	{FactorPush(makeFactor(CONSTANT, "", yystack.l_mark[0].num));}
break;
case 86:
#line 545 "parser.y"
	{CallCommandGenPush(yystack.l_mark[0].ident, INT);}
break;
case 87:
#line 545 "parser.y"
	{CallPush();}
break;
case 88:
#line 549 "parser.y"
	{FactorPush(*lookup(yystack.l_mark[0].ident));}
break;
case 89:
#line 550 "parser.y"
	{FactorPush(makeFactor(CONSTANT, "", lookupArray(yystack.l_mark[-3].ident)->lnum));
                                      calc(Sub);
                                      Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      SextPush(FactorPop(), arg1);
                                      Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      GetelementptrPush(yystack.l_mark[-3].ident, arg1, retval);
                                      Factor tmp = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      LoadPush(retval, tmp);
                                      FactorPush(tmp);
                                      }
break;
case 91:
#line 564 "parser.y"
	{ArgPush();}
break;
case 92:
#line 565 "parser.y"
	{ArgPush();}
break;
case 93:
#line 569 "parser.y"
	{insert(yystack.l_mark[0].ident, flag);}
break;
case 94:
#line 570 "parser.y"
	{insertArray(yystack.l_mark[-5].ident, yystack.l_mark[-3].num, yystack.l_mark[-1].num, flag);}
break;
case 95:
#line 571 "parser.y"
	{insert(yystack.l_mark[0].ident, flag);}
break;
case 96:
#line 572 "parser.y"
	{insertArray(yystack.l_mark[-5].ident, yystack.l_mark[-3].num, yystack.l_mark[-1].num, flag);}
break;
#line 2101 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
