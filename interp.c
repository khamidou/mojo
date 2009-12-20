#include "interp.h"
#include "tokens.h"


int eval_image(FILE *fp)
{
	int r;
	while((r = yylex()) != EOF)
		switch(r) {
		case NUMBER:
			

		}
}
