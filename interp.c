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

int compile_image(FILE *fp)
{
	struct Object* ast = clone_object(list_object);
    expression(ast);
    display_ast(ast);
}

int expression(struct Object *ast) {

	int r = yylex();
    switch(r) {
        case NUMBER:
            puts("number here bro");
            struct Object* obj = clone_object(number_object);
            list_append(ast, obj);
            return message(ast);
            break;
        default:
            fail("Unexpected : %s", yytext);
            break;
    }
}

int message(struct Object *ast) {
    int r = yylex();
    switch(r) {
        case SYMBOL:
            printf("message : %s\n", yytext);
            struct Object* obj = clone_object(base_object);
            obj->type = T_MESSAGE;
            obj->name = strdup(yytext);
            list_append(ast, obj);
            expression(ast);
            break;

        case SEMICOLON:
            return 0;
            break;

        default:
            fail("Expected symbol, got : %s", yytext);
            break;
    }

}

void display_ast(struct Object *ast) {
    
    int length = mojo_list_length(ast->value.l_value);
    
    printf("ast->value.lv: %x, ast len: %d\n", ast->value.l_value, length);
}
