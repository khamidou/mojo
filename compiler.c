/*
  The mojo boostrap compiler.
  copyright (c) 2009 karim hamidou.

  This is a rough translator for a subset of mojo to C.
  It does no verifications whatsoever, everything is kept at the sole discretion
  of the programmer.

*/

#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define SYMBOL_LENGTHS 255

extern char *yytext;
extern FILE *yyin;

void symbol();

int main(int argc, char **argv)
{
	if (argc != 2) {
		fail("%s file", argv[0]);
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		fail("Unable to open %s", argv[1]);

	printf("%s", source_prologue);

	int r;
	while((r = yylex()) != T_EOF) {
		switch(r) {
		case EOF:
			break;

		case SYMBOL:
			symbol();
			break;

		default:
			break;
		}
	}
	printf("%s", source_epilogue);
}


void symbol()
{
	char symname[SYMBOL_LENGTHS];
	strncpy(symname, yytext, SYMBOL_LENGTHS);

	int r = yylex();
	switch(r) {
	case EOF:
		break;

	case AFFECTATION:
		yylex();
		printf("struct Object *%s = %s;", symname, yytext);
		break;

	default:
		printf ("------ %s -----", yytext);
		/* method call, like Lexer yylex; */
		puts("{"); /* put it in a block, to not disrupt other declarations */

		printf("struct Object *met = lookup_method(%s, %s);\n", symname, yytext);
		printf("if (met != nil_object && met->type == T_BUILTIN)\n");
		printf("	met->c_method(%s,", symname);

		/* this is the most complex part of the algorithm :
		   A call to c_method takes four arguments - as we don't know how much arguments we're gonna get,
		   we add them on the fly until we get a comma, at which point we fill the remainder arguments
		   with NULLs.
		*/

		int i = 0;

		while ((r = yylex()) && i < 4) {
			switch(r) {
			case EOF:
				goto loop_exit;
				break;

			case COMMA:
				break;

			default:
				printf("%s,", yytext);
				break;
			}
			i++;
		} 

	loop_exit:
		if (i < 4) {
			  while(i < 3) { 
				  printf("NULL,");
				  i++;
			  }
			  printf("NULL");
		}
		printf(");\n");
		puts("}");
	}
}
