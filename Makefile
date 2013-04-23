CFLAGS=-c -g
OBJS=lex.yy.o object.o list.o error.o number.o main.o interp.o

mojo: $(OBJS)
	cc -o mojo $(OBJS) -lfl 

lex.yy.o: lexer.l
	lex lexer.l
	cc $(CFLAGS) lex.yy.c

object.o: object.h object.c
main.o: main.c
list.o: list.c
interp.o: interp.c
number.o: number.c number.h
error.o: error.h error.c

clean:
	rm *.o
	rm *~

count:
	wc -l `ls *.[chl] -1 | grep -v lex.yy.c | grep -v queue`
