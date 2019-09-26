.DEFAULT_GOAL := cplusp

cplusp.tab.c cplusp.tab.h: cplusp.y
	bison -d cplusp.y

lex.yy.c: cplusp.l cplusp.tab.h
	flex cplusp.l

cplusp: lex.yy.c cplusp.tab.c cplusp.tab.h
	g++ cplusp.tab.c lex.yy.c -ll -o cplusp

clean:
	rm cplusp.tab.c lex.yy.c
