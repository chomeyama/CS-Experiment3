y.tab.c : parser.y
	yacc -d parser.y

lex.yy.c : scanner.l y.tab.h
	lex scanner.l

parser : lex.yy.c y.tab.c
	cc y.tab.c lex.yy.c -ll -o parser
