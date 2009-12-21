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
			struct Object *o  = clone_object(number_object);
			o->value.i_value = atoi(yytext);
			struct Object *no;

			if (yylex() == NUMBER) {
				no  = clone_object(number_object);
				no->value.i_value = atoi(yytext);
			}
			
			struct Method *met = lookup_method(o, "+");
			struct Object *reto = met->c_method(o, no, NULL);

			printf("%d\n", reto->value.i_value); 
			break;
		}
		default:
			break;
		}
	}
}
