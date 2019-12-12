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
    //return p->data;
}


void delete() {
    while (stk.top->data.kind == 1) {
        pop(&stk);
    } 
    show(&stk);
    local_num = 0;
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






