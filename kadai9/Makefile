parser : lex.yy.c y.tab.c parser.y
	cc y.tab.c lex.yy.c -ll -o parser

y.tab.c, y.tab.h : parser.y
	yacc -d parser.y

lex.yy.c : scanner.l y.tab.h
	lex scanner.l

