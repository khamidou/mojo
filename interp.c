#include "interp.h"
#include "tokens.h"
#include "object.h"

extern int yylex();
extern char *yytext;

struct Object *apply_block(struct Object *o)
{
	if (o == NULL || o->type != T_BLOCK)
		return NULL;

	int i = 0;
	for (i = 0; i < list_length(o); i++) {
		
		
	}
}

int eval_image(FILE *fp)
{
	struct Object* stack = clone_object(list_object);
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
