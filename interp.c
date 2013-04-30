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
#include "list.h"
#include "error.h"

extern int yylex();
extern int yyleng;
extern FILE *yyin;
extern char *yytext;

int compile_image(FILE *fp)
{
   
    while(!feof(fp)) {
        struct Object* ast = clone_object(list_object);
        struct Object* scope = clone_object(list_object);
     
        expression(ast);
        display_ast(ast, 0);
        execute_branch(ast, scope);
        free_object(ast);
    }

    return 0;
}

/*
void unput_token(void) {
   int i;
   for (i = yyleng - 1; i >= 0; --i )
           unput(yytext[i]); 
}
*/

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

        case SYMBOL:
            obj = clone_object(base_object);
            obj->type = T_SYMBOL;
            obj->name = strdup(yytext);
            params = clone_object(list_object);
            list_append(params, obj);
            list_append(ast, params);
            return message(params);

            break; 
        default:
            fail("Unexpected : %s", yytext);
            return -1;
            break;
    }
}

int message(struct Object *ast) {
    int r = yylex();
    struct Object *obj;

    if (r == SYMBOL) {
            obj = clone_object(base_object);
            obj->type = T_MESSAGE;
            obj->name = strdup(yytext);
            list_append(ast, obj);
            expression(ast);
    } else if (r == SEMICOLON) {
            return 0;
    } else {
            fail("Expected symbol, got : %s", yytext);
            return -1;
    }

    return -1;
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

const char *type_to_string(enum o_types e)
{
    switch(e) {
        case T_NIL:
            return "Nil";
            break;

        case T_BOOL:
            return "Boolean";
            break;

        case T_NUMBER:
            return "Number";
            break;

        case T_SYMBOL:
            return "Symbol";
            break;

        case T_LIST:
            return "List";
            break;

        case T_STRING:
            return "String";
            break;

        case T_OBJECT:
            return "Object";
            break;

        case T_BUILTIN:
            return "Builtin function";
            break;

        case T_BLOCK:
            return "Block";
            break;

        case T_MESSAGE:
            return "Message";
            break;

        case T_SEPARATOR:
            return "Separator";
            break;

        default:
            return "Unknkown object";
            break;
    }
}

void display_object(struct Object *obj, int nindents) {

        int i = 0;
        for (i = 0; i < nindents; i++)
            putc(' ', stdout);

        printf("class: %s, type: %s\n", obj->name, type_to_string(obj->type));

        if(obj->type == T_LIST) {
            display_ast(obj, nindents + 4);
        }
}

struct Object* execute_branch(struct Object *ast, struct Object *scope) {
    if (ast == NULL || ast->type != T_LIST) 
        return NULL;


	struct Object* stack = clone_object(list_object);

    int length = mojo_list_length(ast->value.l_value);
    if (length == 0)
        return NULL;


    // first, execute all the sub-expressions
    int i = 0;
    for (i = 0; i < length; i++) {
            struct Object *obj = (struct Object*) list_nth(ast, i);
            if(obj == NULL)
                continue;

            if(obj->type == T_LIST) {
                list_append(stack, execute_branch(obj, scope));
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
    
        
        if(destObject->type == T_SYMBOL && strcmp(methodName->name, ":=") == 0) {
            // this is an assignment
            puts("assignment");
            struct Object *value = list_nth(stack, 2);
            struct Object* var = clone_object(base_object);
            var->type = T_SYMBOL;
            var->name = strdup(destObject->name);
            var->value.v_value = clone_object(value);
            list_append(scope->symtab, var); 

        } else {
            // this is a regular method call
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
            if (param1->type == T_SYMBOL) {
                struct Object *o = lookup_variable(scope, param1->name);
                if (o != nil_object)
                    param1 = o;
            }
                

            destObject = method->value.c_method(destObject, param1, param2, param3, param4);
        }

        puts("freeing stuff");
        free_object(stack);

        return destObject;
    }

    return nil_object;
}
