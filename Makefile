.DEFAULT_GOAL := cplusp

cplusp: lex.yy.o cplusp.tab.o cplusp.o cplusp.h
	g++ lex.yy.o cplusp.tab.o cplusp.o -ll -o cplusp `llvm-config --cxxflags --ldflags --system-libs --libs core`

cplusp.o: cplusp.cpp cplusp.h
	g++ -c cplusp.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core`

cplusp.tab.o: cplusp.y cplusp.h
	bison -dvg cplusp.y
	g++ -c cplusp.tab.c `llvm-config --cxxflags --ldflags --system-libs --libs core`

lex.yy.o: cplusp.l cplusp.tab.o
	flex cplusp.l
	g++ -c lex.yy.c `llvm-config --cxxflags --ldflags --system-libs --libs core`

clean:
	rm -f lex.yy.* cplusp.tab.* *.o cplusp
