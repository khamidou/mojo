#include "interp.h"
#include "tokens.h"
#include "object.h"

extern int yylex();
extern char *yytext;

int eval_image(FILE *fp)
{
	int r;
	while((r = yylex()) != T_EOF) {
		switch(r) {
		case NUMBER:
		{
		}
		default:
			break;
		}
	}
}
