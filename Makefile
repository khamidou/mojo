CFLAGS=-c -g
OBJS=interp.o lex.yy.o main.o object.o list.o

mojo: $(OBJS)
	cc -o mojo $(OBJS) -lfl 

lex.yy.o: lexer.l
	lex lexer.l
	cc $(CFLAGS) lex.yy.c

object.o: object.h object.c
interp.o: interp.c interp.h
list.o: list.c
main.o: main.c

clean:
	rm *.o
	rm *~
