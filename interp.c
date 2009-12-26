#include "interp.h"
#include "tokens.h"
#include "object.h"

extern int yylex();
extern char *yytext;

int eval_image(FILE *fp)
{
	struct Object *o1 = clone_object(number_object);
	struct Object *o2 = clone_object(number_object);

	if (o1 == NULL || o2 == NULL)
		fail("Unable to clone both objects");

	o1->value.i_value = 2;
	o2->value.i_value = 2;

	struct Object *met = lookup_method(o1, "+");

	if (met == NULL)
		fail("Unable to lookup method");

	struct Object *ret = met->value.c_method(o1, o2, NULL);

	fail("valeur : %d", ret->value.i_value);

	int r;
	while((r = yylex()) != T_EOF) {
		switch(r) {
		case NUMBER:
		{
			puts("ho");
			struct Object *o1 = clone_object(number_object);
			struct Object *o2 = clone_object(number_object);

			if (o1 == NULL || o2 == NULL)
				fail("Unable to clone both objects");

			printf("%s\n", yytext);
			o1->value.i_value = atoi(yytext);
			o2->value.i_value = 3;

			struct Object *met = lookup_method(o1, "+");
			printf("%s", met->name);
			if (met == NULL)
				fail("Unable to lookup method");

			struct Object *ret = met->value.c_method(o1, o2, NULL);

			printf("valeur : %d\n", ret->value.i_value);
		}
		default:
			break;
		}
	}
}
