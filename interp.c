#include "object.h"
#include "interp.h"
#include "tokens.h"

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
	struct Object* first_leaf = clone_object(list_object);
    list_append(ast, first_leaf);
    
    expression(first_leaf);
    execute(ast);
}

int expression(struct Object *ast) {

	int r = yylex();
    struct Object *obj;
    switch(r) {
        case NUMBER:
            obj = clone_object(number_object);
            obj->value.i_value = atoi(yytext);
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
    struct Object *obj;
    switch(r) {
        case SYMBOL:
            obj = clone_object(base_object);
            obj->type = T_MESSAGE;
            obj->name = strdup(yytext);
            list_append(ast, obj);
            expression(ast);
            break;

        case SEMICOLON:
            obj = clone_object(base_object);
            obj->type = T_SEPARATOR;
            obj->name = "Separator";
            list_append(ast, obj);
            return 0;
            break;

        default:
            fail("Expected symbol, got : %s", yytext);
            break;
    }

}

void display_ast(struct Object *ast) {
    
    int length = mojo_list_length(ast->value.l_value);
    
    printf("ast len: %d\n", length);
    int i = 0;
    for(i = 0; i < length; i++) {
        struct Object *o = list_nth(ast, i);
        printf("type: %d name: %s\n", o->type, o->name);
    }
}

void display_object(struct Object *obj) {
        printf("type: %d name: %s\n", obj->type, obj->name);
}

void execute(struct Object *ast) {
    int length = mojo_list_length(ast->value.l_value);
    
    int i = 0;
    for(i = 0; i < length; i++) {
        struct Object *statement_list = (struct Object*) list_nth(ast, i);
        
        int length = mojo_list_length(statement_list->value.l_value);
        struct Object *destObject = (struct Object*) list_nth(statement_list, 0);
        struct Object *message = (struct Object*) list_nth(statement_list, 1);
        struct Object *arg1 = (struct Object*) list_nth(statement_list, 2);
        
        struct Object *method = (struct Object*) lookup_method(destObject, message->name);
        if(method == nil_object) {
            fail("error : method %s not found in object %s", message->name, destObject->name);
        }

        struct Object *ret = method->value.c_method(destObject, arg1, NULL, NULL, NULL);
        printf("value %d", ret->value.i_value);
    }
}
