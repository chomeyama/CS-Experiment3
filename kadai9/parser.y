%{
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
    Factor data; //記号表のレコード
    int lnum;
    int rnum;
    int num;
    struct _TableStack *prev; //前のノードへのポインタ
} TableStack;

/* 引数を管理する線形リスト */
typedef struct _Argument {
    Factor data; //引数
    struct _Argument *next; //次のノードへのポインタ
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
  Getelementptr /* getelementptr */
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

FILE *fp; //result.llのファイルポインタ
Scope flag = GLOBAL_VAR; //変数のスコープはグローバルで初期化
unsigned int read_arg = 0;
unsigned int read_decl_flag = 0;
unsigned int write_decl_flag = 0;
unsigned int reg_cntr = 0; //レジスタ番号管理のためのカウンタ

TableStack *TableStackPtr = NULL; //Tableスタックのスタックポインタ
void TablePush(Factor*); //Tableスタックのpush関数
void TablePop(); //Tableスタックのpop関数
void TableShow(); //Tableスタックの全データを出力するための関数

void insert(char*, Scope); //記号表への追加を行う関数
 void insertArray(char*, int, int, Scope);
Factor* lookup(char*); //記号表から検索を行う関数
TableStack* lookupArray(char*);
void delete(); //記号表から要素を削除する関数
void show();

FactorStack *FactorStackPtr = NULL; //Factorスタックのスタックポインタ
void FactorPush(Factor); //Factorスタックのpush関数
Factor FactorPop(); //Factorスタックのpop関数
void Factordelete(); //Factorスタックのdelete関数
Factor makeFactor(Scope, char*, int); //Factorを作成して返す関数

void CallCommandGenPush(char*, Rettype); //CallCommandのpush関数
CallCommand *CallCommandStackPtr = NULL; //CallCommandのスタックポインタ

void BrCommandPush(LLVMcode*); //BrCommandのpush関数
void BrCommandPop(); //BrCommandのpop関数
void backpatch(int);
BrCommand *BrCommandStackPtr = NULL; //BrCommandのスタックポインタ

void AllocaPush(Factor); //AllocaのLLVMcodeを作成して線形リストに追加する関数
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

void cmpcalc(Cmptype); //比較演算
void calc(LLVMcommand); //四則演算
void ArgPush(); //仮引数設定

void LLVMPush(LLVMcode*); //LLVMcodeのpush関数
LLVMcode *codehd = NULL; /* 命令列の先頭のアドレスを保持するポインタ */
LLVMcode *codetl = NULL; /* 命令列の末尾のアドレスを保持するポインタ */

void FunPush(Fundecl*); //Fundeclのpush関数
Fundecl *declhd = NULL; //関数定義の線形リストの先頭の要素のアドレスを保持するポインタ
Fundecl *decltl = NULL; //関数定義の線形リストの末尾の要素のアドレスを保持するポインタ

void displayFactor(Factor);
void displayLlvmcodes(LLVMcode*);
void displayLlvmfundecl(Fundecl*);

%}

%union {
    int num;
    char ident[MAXLENGTH+1];
}


%token SBEGIN DO ELSE SEND
%token FOR FORWARD FUNCTION IF PROCEDURE
%token PROGRAM READ THEN TO VAR
%token WHILE WRITE

%left PLUS MINUS
%left MULT DIV

%token EQ NEQ LE LT GE GT
%token LPAREN RPAREN LBRACE RBRACE
%token COMMA SEMICOLON COLON INTERVAL
%token PERIOD ASSIGN
%token <num> NUMBER
%token <ident> IDENT

%%

program
    : PROGRAM {fp = fopen("result.ll", "w");}
      IDENT SEMICOLON outblock PERIOD {displayLlvmfundecl(declhd); fclose(fp);}
    ;

outblock
    : var_decl_part {displayLlvmcodes(codehd);}
      subprog_decl_part {/* main 関数 */
                         reg_cntr = 1;
	                       insert("main", FUNC_NAME);
	                       flag = LOCAL_VAR;}
      statement {RetPush(INT);}
    ;

var_decl_part
    : /* empty */
    | var_decl_list SEMICOLON
    ;

var_decl_list
	  : var_decl_list SEMICOLON var_decl
	  | var_decl
	  ;

var_decl
	  : VAR id_list
	  ;

subprog_decl_part
	  : /* empty */
	  | subprog_decl_list SEMICOLON
    ;

subprog_decl_list
	  : subprog_decl_list SEMICOLON subprog_decl
  	| subprog_decl
	  ;

subprog_decl
  	: proc_decl {RetPush(decltl->rettype); delete(); flag = GLOBAL_VAR;}
    | func_decl {RetPush(decltl->rettype); delete(); flag = GLOBAL_VAR;}
	  ;

proc_decl
  	: PROCEDURE proc_name SEMICOLON {flag = LOCAL_VAR; reg_cntr = 1;} inblock
    | PROCEDURE proc_name {flag = ARGUMENT; reg_cntr = 0;}
      LPAREN id_list RPAREN SEMICOLON {flag = LOCAL_VAR;
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
      inblock
	  ;

proc_name
    : IDENT {insert($1, PROC_NAME);
             if (decltl->rettype == INT)
                 AllocaPush(makeFactor(LOCAL_VAR, $1, reg_cntr++));
            }
	  ;

func_decl
  	: FUNCTION func_name SEMICOLON {flag = LOCAL_VAR; reg_cntr = 1;} inblock
    | FUNCTION func_name {flag = ARGUMENT; reg_cntr = 0;}
      LPAREN id_list RPAREN SEMICOLON {flag = LOCAL_VAR;
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
      inblock
	  ;

func_name
    : IDENT {insert($1, FUNC_NAME);}
	  ;

inblock
    : var_decl_part statement
	  ;

statement_list
	  :statement_list SEMICOLON statement
   	|statement
	  ;

statement
  	: assignment_statement
  	| if_statement
  	| while_statement
  	| for_statement
  	| proc_call_statement
  	| null_statement
  	| block_statement
  	| read_statement
  	| write_statement
  	;

    assignment_statement
    : IDENT ASSIGN expression {StorePush(FactorStackPtr->data, *lookup($1));}
    | IDENT LBRACE expression {FactorPush(makeFactor(CONSTANT, "", lookupArray($1)->lnum));
                               calc(Sub);
                               Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                               SextPush(FactorPop(), arg1);
                               Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                               GetelementptrPush($1, arg1, retval);
                               FactorPush(retval);
                               }
      RBRACE ASSIGN expression {Factor arg1 = FactorPop();
                                Factor retval = FactorPop();
                                StorePush(arg1, retval);}
    ;

    if_statement
    : IF condition {BrCondPush(FactorStackPtr->data, reg_cntr, -1);}
      THEN {LabelPush(reg_cntr++);}
      statement {backpatch(reg_cntr);}
      else_statement {BrCommandPop(); LabelPush(reg_cntr++);}
    ;

    else_statement
    : /* empty */ {BrUncondPush(reg_cntr);}
    | ELSE {BrUncondPush(-1); LabelPush(reg_cntr++);}
      statement {backpatch(reg_cntr);
                 BrUncondPush(reg_cntr);}
    ;

    while_statement
    : WHILE {BrUncondPush(reg_cntr); LabelPush(reg_cntr++);}
      condition {BrCondPush(FactorStackPtr->data, reg_cntr, -1);}
      DO {LabelPush(reg_cntr++);}
      statement {backpatch(reg_cntr); BrUncondPush(BrCommandStackPtr->ptr->args.bruncond.arg1); BrCommandPop(); BrCommandPop(); LabelPush(reg_cntr++);}
    ;

    for_statement
    : FOR IDENT ASSIGN expression {StorePush(FactorStackPtr->data, *lookup($2));
                                   BrUncondPush(reg_cntr);}
      TO {LabelPush(reg_cntr++);}
      expression {Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                  LoadPush(*lookup($2), arg1);
                  Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                  IcmpPush(SLE, arg1, FactorStackPtr->data, retval); BrCondPush(retval, reg_cntr, -1);}
      DO {LabelPush(reg_cntr++);}
      statement {Factor incrementee = *lookup($2);
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
    ;

    proc_call_statement
    : proc_call_name {CallPush();}
    | proc_call_name LPAREN arg_list RPAREN {CallPush();}
    ;

    proc_call_name
    : IDENT {CallCommandGenPush($1, VOID);}
    ;

    block_statement
    : SBEGIN statement_list SEND
    ;

    read_statement
    : READ LPAREN IDENT RPAREN {if (read_decl_flag == 0) {
                                    fprintf(fp, "@.str = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
                                    fprintf(fp, "declare i32 @scanf(i8*, ...) #1\n");
                                    read_decl_flag = 1;
                                    read_arg = 0;
                                }
                                ReadPush(*lookup($3), makeFactor(LOCAL_VAR, "", reg_cntr++));}
    | READ LPAREN IDENT LBRACE expression RBRACE RPAREN {if (read_decl_flag == 0) {
                                                             fprintf(fp, "@.str = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
                                                             fprintf(fp, "declare i32 @scanf(i8*, ...) #1\n");
                                                             read_decl_flag = 1;
                                                             read_arg = 0;
                                                         }
                                                         FactorPush(makeFactor(CONSTANT, "", lookupArray($3)->lnum));
                                                         calc(Sub);
                                                         Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                                         SextPush(FactorPop(), arg1);
                                                         Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                                         GetelementptrPush($3, arg1, retval);
                                                         ReadPush(retval, makeFactor(LOCAL_VAR, "", reg_cntr++));} 
    ;

    write_statement
    : WRITE LPAREN expression RPAREN {WritePush(FactorStackPtr->data, makeFactor(LOCAL_VAR, "", reg_cntr++));
                                      if (write_decl_flag == 0) {
                                      fprintf(fp, "@.str.1 = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n");
                                      fprintf(fp, "declare i32 @printf(i8*, ...) #1\n");
                                      write_decl_flag = 1;
                                      }}
    ;

    null_statement
    : /* empty */
    ;

    condition
    : expression EQ expression {cmpcalc(EQUAL);}
    | expression NEQ expression {cmpcalc(NE);}
    | expression LT expression {cmpcalc(SLT);}
    | expression LE expression {cmpcalc(SLE);}
    | expression GT expression {cmpcalc(SGT);}
    | expression GE expression {cmpcalc(SGE);}
    ;

    expression
    : term
    | PLUS term {FactorPush(makeFactor(CONSTANT, "", 1)); calc(Mult);}
    | MINUS term {FactorPush(makeFactor(CONSTANT, "", -1)); calc(Mult);}
    | expression PLUS term {calc(Add);}
    | expression MINUS term {calc(Sub);}
    ;

    term
    : factor
    | term MULT factor {calc(Mult);}
    | term DIV factor {calc(Div);}
    ;

    factor
    : var_name
    | NUMBER {FactorPush(makeFactor(CONSTANT, "", $1));}
    | LPAREN expression RPAREN
    | func_call
    ;

    func_call
    : IDENT {CallCommandGenPush($1, INT);} LPAREN arg_list RPAREN {CallPush();}
    ;

    var_name
    : IDENT {FactorPush(*lookup($1));}
    | IDENT LBRACE expression RBRACE {FactorPush(makeFactor(CONSTANT, "", lookupArray($1)->lnum));
                                      calc(Sub);
                                      Factor arg1 = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      SextPush(FactorPop(), arg1);
                                      Factor retval = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      GetelementptrPush($1, arg1, retval);
                                      Factor tmp = makeFactor(LOCAL_VAR, "", reg_cntr++);
                                      LoadPush(retval, tmp);
                                      FactorPush(tmp);
                                      }
    ;

    arg_list
    : /* empty */
    | expression {ArgPush();}
    | arg_list COMMA expression {ArgPush();}
    ;

    id_list
    : IDENT {insert($1, flag);}
    | IDENT LBRACE NUMBER INTERVAL NUMBER RBRACE {insertArray($1, $3, $5, flag);}
    | id_list COMMA IDENT {insert($3, flag);}
    | id_list COMMA IDENT LBRACE NUMBER INTERVAL NUMBER RBRACE {insertArray($3, $5, $7, flag);}
    ;


%%

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
            MultPush(arg1, arg2, retval);
            break;
        case Div:
            DivPush(arg1, arg2, retval);
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
