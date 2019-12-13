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

int global_num = 0; //大域変数ナンバリングするための変数
int local_num = 0; //局所変数ナンバリングするための変数

typedef enum {
    GLOBAL_VAR,
    LOCAL_VAR,
    PROC_NAME,
    CONSTANT
} Scope;

Scope flag = GLOBAL_VAR; //変数のスコープはグローバルで初期化

struct TableData { //記号表のレコードを一般化した構造体
    char name[16]; //記号
    int num; //割り当てられた番号
    int kind; //Scopeで管理される種別番号
};

struct STACK { //スタックを一般化した構造体
    struct STACK_DATA *top; //スタックの先頭アドレス(最後に追加されたデータのアドレス)
};

struct STACK_DATA { //スタックに積まれるデータの構造体
    struct STACK_DATA *prev; //前のデータへのポインタ
    struct TableData data; //記号表のレコード
};

//スタックのプッシュを行う関数
void push(struct STACK *stk, struct TableData *td) {
    struct STACK_DATA *tmp = (struct STACK_DATA *)malloc(sizeof(struct STACK_DATA));
    tmp->prev = stk->top; //新しいデータのprevは現先頭データを指す

    //データのレコード部分を作成
    strcpy(tmp->data.name ,td->name);
    tmp->data.num = td->num;
    tmp->data.kind = td->kind;

    stk->top = tmp; //スタックの先頭を新しいデータに変更
}

//スタックのポップを行う関数
void pop(struct STACK *stk) {
    struct STACK_DATA *p = stk->top; //メモリ解放するために先頭アドレスを一時記憶
    stk->top = stk->top->prev; //先頭アドレスを先頭から二番目に変更
    free(p); //先頭だったアドレスが示すメモリ領域を解放
}

//記号表の全データを出力するための関数
//スタックに積まれたデータを先頭から順に出力する
void show(struct STACK *stk) {
    struct STACK_DATA *p = stk->top; //出力するデータを指すポインタ
    char *kind;
    printf(" _______________________________________\n");
    printf("|       kind             Name   val/adr |\n");
    printf("|---------------------------------------|\n");
    while (1) {
        switch (p->data.kind) {
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
            printf("|%11s %16s %9d |\n", kind, p->data.name, p->data.num);
            break;
        } else { //先頭データでない場合
            printf("|%11s %16s %9d |\n", kind, p->data.name, p->data.num);
            p = p->prev; //次の出力対象を一つ前のデータに
        }
    }
    printf("|_______________________________________|\n\n");
}

struct STACK stk; //記号表のためのスタックをグローバルで宣言

void insert(char *s, Scope flag) {
    struct TableData *td = (struct TableData *)malloc(sizeof(struct TableData));
    strcpy(td->name,s);
    if (flag == GLOBAL_VAR) {
        td->num = global_num; //局所変数に番号を割り当てる
        global_num++; //番号をインクリメント
    } else if (flag == LOCAL_VAR) {
        td->num = local_num; //大域変数に番号を割り当てる
	      local_num++; //番号をインクリメント
    } else {
	      td->num = -1; //変数でなければ無効な適当な値
    }
    td->kind = flag;
    push(&stk, td); //記号表への追加
    show(&stk); //記号表のデータを出力
}

void lookup(char *s) { //後の実験で拡張
    struct STACK_DATA *p = stk.top;
    while (strcmp(p->data.name, s)) { //引数と一致する記号を見つけるまで
        if (p->prev == NULL) { //先頭に達しても見つからない場合として終了
            printf("%s not found\n", s);
            return;
        }
	      p = p->prev; //スタックの一つ前のデータへ辿る
    }
}

void delete() {
    while (stk.top->data.kind == 1) { //局所変数である間
        pop(&stk); //記号表から削除
    }
    show(&stk); //記号表の出力
    local_num = 0; //局所変数のナンバリング用の変数を0に再初期化
}

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
        : {stk.top = NULL; } PROGRAM IDENT SEMICOLON outblock PERIOD
        ;

outblock
        : var_decl_part subprog_decl_part statement
        ;

var_decl_part
        : /* empty */
        | var_decl_list SEMICOLON
        ;

var_decl_list
	:var_decl_list SEMICOLON var_decl
	|var_decl
	;

var_decl
	:VAR id_list
	;

subprog_decl_part
	: /* empty */
	|subprog_decl_list SEMICOLON
        ;

subprog_decl_list
	:subprog_decl_list SEMICOLON subprog_decl
	|subprog_decl
	;

subprog_decl
	:proc_decl
	;

proc_decl
	:PROCEDURE {flag = LOCAL_VAR;} proc_name SEMICOLON inblock {delete(); flag = GLOBAL_VAR;}
	;

proc_name
	:IDENT {insert($1, PROC_NAME);}
	;

inblock
	:var_decl_part statement
	;

statement_list
	:statement_list SEMICOLON statement
	|statement
	;

statement
	:assignment_statement
	|if_statement
	|while_statement
	|for_statement
	|proc_call_statement
	|null_statement
	|block_statement
	|read_statement
	|write_statement
	;

assignment_statement
	:IDENT ASSIGN expression {lookup($1);}
	;

if_statement
	:IF condition THEN statement else_statement
	;

else_statement
	: /* empty */
	|ELSE statement
	;

while_statement
	:WHILE condition DO statement
	;

for_statement
	:FOR IDENT ASSIGN expression TO expression DO statement {lookup($2);}
	;

proc_call_statement
        : proc_call_name
	;

proc_call_name
        : IDENT {lookup($1);}
	;

block_statement
        : SBEGIN statement_list SEND
        ;

read_statement
        : READ LPAREN IDENT RPAREN {lookup($3);}
        ;

write_statement
        : WRITE LPAREN expression RPAREN
        ;

null_statement
        : /* empty */
        ;

condition
        : expression EQ expression
        | expression NEQ expression
        | expression LT expression
        | expression LE expression
        | expression GT expression
        | expression GE expression
        ;

expression
        : term
        | PLUS term
        | MINUS term
        | expression PLUS term
        | expression MINUS term
        ;

term
        : factor
        | term MULT factor
        | term DIV factor
        ;

factor
        : var_name
	| NUMBER
	| LPAREN expression RPAREN
        ;

var_name
        : IDENT {lookup($1);}
        ;

arg_list
        : expression
        | arg_list COMMA expression
        ;

id_list
        : IDENT {insert($1, flag);}
        | id_list COMMA IDENT {insert($3, flag);}
        ;

%%

yyerror(char *s)
{
  extern int yylineno;
  extern char *yytext;
  fprintf(stderr, "%s %s in line %d\n", s, yytext, yylineno);
}
