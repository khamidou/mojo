CFLAGS=-c -g
OBJS=lex.yy.o object.o list.o error.o number.o compiler.o

mojo: $(OBJS)
	cc -o mojo $(OBJS) -lfl 

lex.yy.o: lexer.l
	lex lexer.l
	cc $(CFLAGS) lex.yy.c

object.o: object.h object.c
compiler.o: compiler.c compiler.h
list.o: list.c
number.o: number.c number.h
error.o: error.h error.c

clean:
	rm *.o
	rm *~

count:
	wc -l `ls *.[chl] -1 | grep -v lex.yy.c | grep -v queue`