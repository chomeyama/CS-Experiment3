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

int global_num = 0;
int local_num = 0;
 
typedef enum {
    GLOBAL_VAR,
    LOCAL_VAR,
    PROC_NAME,
    CONSTANT
} Scope;

Scope flag = GLOBAL_VAR;

struct TableData {
    char name[256];
    int num;
    int kind;
};

struct STACK {
    struct STACK_DATA *top;
};

struct STACK_DATA {
    struct STACK_DATA *prev;
    struct TableData data;
};

void push(struct STACK *stk, struct TableData *td) {
    struct STACK_DATA *tmp = (struct STACK_DATA *)malloc(sizeof(struct STACK_DATA));
    tmp->prev = stk->top;
    strcpy(tmp->data.name ,td->name);
    tmp->data.num = td->num;
    tmp->data.kind = td->kind;
    stk->top = tmp;
}

void pop(struct STACK *stk) {
    (*stk).top = (*stk).top->prev;
}

void show(struct STACK *stk) {
    struct STACK_DATA *p = stk->top;
    while (1) {
        if (p->prev == NULL) { 
            printf("%s %d %d  forward\n", p->data.name, p->data.num, p->data.kind);
            break;
        } else {
            printf("%s %d %d  not forward\n", p->data.name, p->data.num, p->data.kind);
            p = p->prev;
        }
    }
    printf("------------------------\n");
}

struct STACK stk;

void insert(char *s, Scope flag) {
    struct TableData *td = (struct TableData *)malloc(sizeof(struct TableData));
    strcpy(td->name,s);
    if (flag == GLOBAL_VAR) {
        td->num = global_num;
        global_num += 1;
    } else if (flag == LOCAL_VAR) {
        td->num = local_num;
	local_num += 1;
    } else {
	td->num = -1;
    }
    td->kind = flag;
    push(&stk, td);
    show(&stk);
}


void lookup(char *s) {
    struct STACK_DATA *p = stk.top;
    while (strcmp(p->data.name, s)) {
        if (p->prev == NULL) {
            return;
        }
	p = p->prev;
    }
    /*return p->data;*/
}


void delete() {
    while (stk.top->data.kind == 1) {
        pop(&stk);
    } 
    show(&stk);
    local_num = 0;
}

#line 111 "parser.y"
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
#line 139 "y.tab.c"

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
    1,    0,    2,    3,    3,    6,    6,    7,    4,    4,
    9,    9,   10,   13,   11,   12,   14,   15,   15,    5,
    5,    5,    5,    5,    5,    5,    5,    5,   16,   17,
   27,   27,   18,   19,   20,   28,   22,   23,   24,   21,
   26,   26,   26,   26,   26,   26,   25,   25,   25,   25,
   25,   29,   29,   29,   30,   30,   30,   31,   32,   32,
    8,    8,
};
static const short yylen[] = {                            2,
    0,    6,    3,    0,    2,    3,    1,    2,    0,    2,
    3,    1,    1,    0,    5,    1,    2,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    3,    5,
    0,    2,    4,    8,    1,    1,    3,    4,    4,    0,
    3,    3,    3,    3,    3,    3,    1,    2,    2,    3,
    3,    1,    3,    3,    1,    1,    3,    1,    1,    3,
    1,    3,
};
static const short yydefred[] = {                         1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    7,
   61,    0,    2,   14,    0,    0,   12,   13,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    3,   20,
   21,   22,   23,   24,   25,   26,   27,   28,   35,    0,
    6,   62,   16,    0,   19,    0,    0,    0,    0,    0,
   56,   58,    0,    0,    0,   52,   55,    0,    0,    0,
    0,   11,    0,   37,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   15,   18,    0,   57,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   54,
   38,   33,   39,   17,    0,    0,   30,    0,   32,    0,
   34,
};
static const short yydgoto[] = {                          1,
    2,    7,    8,   15,   29,    9,   10,   12,   16,   17,
   18,   44,   21,   86,   46,   30,   31,   32,   33,   34,
   35,   36,   37,   38,   53,   54,  107,   39,   55,   56,
   57,    0,
};
static const short yysindex[] = {                         0,
    0, -205, -284, -215, -194, -209, -212, -178, -162,    0,
    0, -160,    0,    0, -158, -156,    0,    0, -194, -159,
 -151, -158, -145,  -40, -133,  -40, -126, -130,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -178,
    0,    0,    0, -125,    0, -246, -122, -186, -186,  -40,
    0,    0,  -16,  -92, -236,    0,    0, -117,  -79,  -40,
  -40,    0, -194,    0, -158,  -40, -236, -236, -267, -186,
 -186,  -40,  -40,  -40,  -40,  -40,  -40, -158, -186, -186,
 -104, -158, -163, -149, -158,    0,    0, -115,    0, -236,
 -236, -149, -149, -149, -149, -149, -149,  -78,    0,    0,
    0,    0,    0,    0,  -40, -158,    0, -254,    0, -158,
    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0, -220,    0,    0, -202,    0,    0,
    0, -103,    0,    0, -107,    0,    0,    0, -256,    0,
    0, -230,    0,    0,    0,    0,    0, -238,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -189,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -140,    0,    0,    0,    0,    0,
    0,    0, -171,    0, -230,    0, -113,  -86,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -211,    0,    0,
    0, -211,    0, -193, -156,    0,    0,    0,    0,  -59,
  -32, -235, -222, -204, -177, -164, -146, -257,    0,    0,
    0,    0,    0,    0,    0, -211,    0,    0,    0, -211,
    0,
};
static const short yygindex[] = {                         0,
    0,    0,  123,    0,  -22,    0,  170,    0,    0,  150,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -48,  171,    0,    0,   82,   36,
    0,    0,
};
#define YYTABLESIZE 266
static const short yytable[] = {                         45,
    5,   69,   31,  110,    5,   70,   71,    5,    5,    4,
    5,   83,   84,   64,    5,    5,   89,   88,   70,   71,
   36,   36,   41,   92,   93,   94,   95,   96,   97,   40,
   31,    5,   41,   31,    5,   42,    4,    5,   79,   80,
    4,   65,   87,    4,    4,   42,    4,   40,   40,   36,
    4,    4,   36,   44,    9,   98,  108,   40,    9,  102,
    3,    9,  104,   44,    9,   29,   29,   10,    9,    9,
    4,   10,    5,    4,   10,    6,   40,   10,   13,   40,
   43,   10,   10,  109,   11,    4,   14,  111,    9,    4,
   43,    9,    4,   46,   29,    4,   50,   29,   22,    4,
    4,   10,   23,   46,   10,   24,   51,   52,   25,   70,
   71,   45,   26,   27,   99,  100,    4,   47,   47,   47,
  103,   45,    4,   70,   71,   19,   20,   47,   47,   67,
   68,   40,   47,   47,   42,   28,   47,   47,   47,   47,
   47,   47,   43,   47,   48,   48,   48,   47,   47,   58,
   47,   90,   91,  105,   48,   48,   60,   70,   71,   48,
   48,   61,   63,   48,   48,   48,   48,   48,   48,   66,
   48,   49,   49,   49,   48,   78,   81,   48,   82,  101,
  106,   49,   49,   40,    8,   85,   49,   49,   41,   62,
   49,   49,   49,   49,   49,   49,   59,   49,   50,   50,
   50,   49,    0,    0,   49,    0,    0,    0,   50,   50,
    0,    0,    0,   50,   50,    0,    0,   50,   50,   50,
   50,   50,   50,    0,   50,   51,   51,   51,   50,    0,
    0,   50,   48,   49,    0,   51,   51,    0,    0,    0,
   51,   51,   50,    0,   51,   51,   51,   51,   51,   51,
    0,   51,   51,   52,    0,   51,   70,   71,   51,    0,
   72,   73,   74,   75,   76,   77,
};
static const short yycheck[] = {                         22,
  257,   50,  260,  258,  261,  273,  274,  264,  265,  294,
  267,   60,   61,  260,  271,  272,  284,   66,  273,  274,
  259,  260,  258,   72,   73,   74,   75,   76,   77,  260,
  288,  288,  268,  291,  291,  258,  257,  294,  275,  276,
  261,  288,   65,  264,  265,  268,  267,  259,  260,  288,
  271,  272,  291,  258,  257,   78,  105,  288,  261,   82,
  266,  264,   85,  268,  267,  259,  260,  257,  271,  272,
  291,  261,  288,  294,  264,  270,  288,  267,  291,  291,
  258,  271,  272,  106,  294,  257,  265,  110,  291,  261,
  268,  294,  264,  258,  288,  267,  283,  291,  257,  271,
  272,  291,  261,  268,  294,  264,  293,  294,  267,  273,
  274,  258,  271,  272,   79,   80,  288,  258,  259,  260,
  284,  268,  294,  273,  274,  288,  287,  268,  269,   48,
   49,  288,  273,  274,  294,  294,  277,  278,  279,  280,
  281,  282,  294,  284,  258,  259,  260,  288,  294,  283,
  291,   70,   71,  269,  268,  269,  283,  273,  274,  273,
  274,  292,  288,  277,  278,  279,  280,  281,  282,  292,
  284,  258,  259,  260,  288,  268,  294,  291,  258,  284,
  259,  268,  269,  291,  288,   63,  273,  274,   19,   40,
  277,  278,  279,  280,  281,  282,   26,  284,  258,  259,
  260,  288,   -1,   -1,  291,   -1,   -1,   -1,  268,  269,
   -1,   -1,   -1,  273,  274,   -1,   -1,  277,  278,  279,
  280,  281,  282,   -1,  284,  258,  259,  260,  288,   -1,
   -1,  291,  273,  274,   -1,  268,  269,   -1,   -1,   -1,
  273,  274,  283,   -1,  277,  278,  279,  280,  281,  282,
   -1,  284,  293,  294,   -1,  288,  273,  274,  291,   -1,
  277,  278,  279,  280,  281,  282,
};
#define YYFINAL 1
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
"program : $$1 PROGRAM IDENT SEMICOLON outblock PERIOD",
"outblock : var_decl_part subprog_decl_part statement",
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
"$$2 :",
"proc_decl : PROCEDURE $$2 proc_name SEMICOLON inblock",
"proc_name : IDENT",
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
"if_statement : IF condition THEN statement else_statement",
"else_statement :",
"else_statement : ELSE statement",
"while_statement : WHILE condition DO statement",
"for_statement : FOR IDENT ASSIGN expression TO expression DO statement",
"proc_call_statement : proc_call_name",
"proc_call_name : IDENT",
"block_statement : SBEGIN statement_list SEND",
"read_statement : READ LPAREN IDENT RPAREN",
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
"var_name : IDENT",
"arg_list : expression",
"arg_list : arg_list COMMA expression",
"id_list : IDENT",
"id_list : id_list COMMA IDENT",

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
#line 287 "parser.y"
 

yyerror(char *s)
{
  extern int yylineno;
  extern char *yytext;
  fprintf(stderr, "%s %s in line %d\n", s, yytext, yylineno);
}






#line 478 "y.tab.c"

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
#line 135 "parser.y"
	{stk.top = NULL; }
break;
case 14:
#line 171 "parser.y"
	{flag = LOCAL_VAR;}
break;
case 15:
#line 171 "parser.y"
	{delete(); flag = GLOBAL_VAR;}
break;
case 16:
#line 175 "parser.y"
	{insert(yystack.l_mark[0].ident, PROC_NAME);}
break;
case 29:
#line 200 "parser.y"
	{lookup(yystack.l_mark[-2].ident);}
break;
case 34:
#line 217 "parser.y"
	{lookup(yystack.l_mark[-6].ident);}
break;
case 36:
#line 225 "parser.y"
	{lookup(yystack.l_mark[0].ident);}
break;
case 38:
#line 233 "parser.y"
	{lookup(yystack.l_mark[-1].ident);}
break;
case 58:
#line 274 "parser.y"
	{lookup(yystack.l_mark[0].ident);}
break;
case 61:
#line 283 "parser.y"
	{insert(yystack.l_mark[0].ident, flag);}
break;
case 62:
#line 284 "parser.y"
	{insert(yystack.l_mark[0].ident, flag);}
break;
#line 728 "y.tab.c"
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
