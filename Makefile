.DEFAULT_GOAL := cplusp

cplusp: lex.yy.o cplusp.tab.o cplusp.o
	g++ lex.yy.o cplusp.tab.o cplusp.o -ll -o cplusp

cplusp.o: cplusp.cpp cplusp.h
	g++ -c cplusp.cpp

cplusp.tab.o: cplusp.y
	bison -dv cplusp.y
	g++ -c cplusp.tab.c

lex.yy.o: cplusp.l cplusp.tab.o
	flex cplusp.l
	g++ -c lex.yy.c

clean:
	rm -f lex.yy.* cplusp.tab.* *.o cplusp
