/**
 * a short explanation of the abstract syntax tree used mojo :
 * in short, the code is parsed and is transformed in a bunch of
 * s-expressions.
 *
 * For instance, the following code :
 * MyClass :setValue o :setPosx 3 becomes :
 * (MyClass
 *          (:setValue o
 *              (: setPosx 3)))
 */

#include "object.h"
#include "interp.h"
#include "tokens.h"

extern int yylex();
extern char *yytext;

int compile_image(FILE *fp)
{
	struct Object* ast = clone_object(list_object);
    
    expression(ast);
    display_ast(ast, 0);
    puts(">>> AST <<<");

    struct Object *returnValue = execute_branch(ast);
    printf("Value : %d", returnValue->value.i_value);
}

int expression(struct Object *ast) {

	int r = yylex();
    struct Object *obj;
	struct Object *params;
    switch(r) {
        case NUMBER:
            obj = clone_object(number_object);
            obj->value.i_value = atoi(yytext);
            params = clone_object(list_object);
            list_append(params, obj);
            list_append(ast, params);

            return message(params);
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
            return 0;
            break;

        default:
            fail("Expected symbol, got : %s", yytext);
            break;
    }

}

void display_ast(struct Object *ast, int nindents) {
    if (ast == NULL)
        return;

    if (ast->type != T_LIST) {
        display_object(ast, nindents);
        return;
    }

    int length = mojo_list_length(ast->value.l_value);
    
    int i = 0;
    for(i = 0; i < length; i++) {
        struct Object *o = list_nth(ast, i);
        display_object(o, nindents);
    }
}

void display_object(struct Object *obj, int nindents) {

        int i = 0;
        for (i = 0; i < nindents; i++)
            putc(' ', stdout);

        printf("class: %s\n", obj->name);

        if(obj->type == T_LIST) {
            display_ast(obj, nindents + 4);
        }
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

struct Object* execute_branch(struct Object *ast) {
    if (ast == NULL || ast->type != T_LIST) 
        return;


	struct Object* stack = clone_object(list_object);

    int length = mojo_list_length(ast->value.l_value);
    if (length == 0)
        return;


    // first, execute all the sub-expressions
    int i = 0;
    for (i = 0; i < length; i++) {
            struct Object *obj = (struct Object*) list_nth(ast, i);
            if(obj == NULL)
                continue;

            if(obj->type == T_LIST) {
                list_append(stack, execute_branch(obj));
            } else {
                list_append(stack, obj);
            }
    }

    // now execute the expression itself
    
    length = mojo_list_length(ast->value.l_value);
    if (length == 0) {
        return nil_object;
    } else if (length == 1) {
        return list_nth(stack, 1);
    } else if (length > 1) {
        struct Object *destObject = list_nth(stack, 0);
        struct Object *methodName = list_nth(stack, 1);

        struct Object *method = (struct Object*) lookup_method(destObject, methodName->name);
        if(method == nil_object) {
                        fail("error : method %s not found in object %s", methodName->name, destObject->name);
        }


        /* we use the fact that if list_nth doesn't find an
         * object because it doesn't exists at the index, it returns nil
         */ 
        struct Object *param1 = list_nth(stack, 2), 
                      *param2 = list_nth(stack, 3),
                      *param3 = list_nth(stack, 4), 
                      *param4 = list_nth(stack, 5);

        destObject = method->value.c_method(destObject, param1, param2, param3, param4);
        return destObject;
    }

    return nil_object;
}
