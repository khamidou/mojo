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
        display_object(o);
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
        
        int statement_length = mojo_list_length(statement_list->value.l_value);
        int j = 0;
        
        // returnValue holds the result of the evaluation of an expression
        struct Object *returnValue = (struct Object*) list_nth(statement_list, statement_length - 1);
        struct Object *destObject, *method;
        for(j = statement_length - 2; j >= 0; j--) {
            struct Object *obj = (struct Object*) list_nth(statement_list, j);
            switch(obj->type) {
                case T_MESSAGE:
                    if (j - 1 > 0) { // guard
                        destObject = (struct Object*) list_nth(statement_list, j - 1);
                        method = (struct Object*) lookup_method(destObject, obj->name);
                        if(method == nil_object) {
                            fail("error : method %s not found in object %s", obj->name, destObject->name);
                        }

                        returnValue = method->value.c_method(destObject, returnValue, NULL, NULL, NULL);
                    }

                    break;
            }


        }
       printf("value %d", returnValue->value.i_value);
    }
}
