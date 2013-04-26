#include "mstrings.h"

struct mojo_string* dup_string(char *s)
{

	struct mojo_string *str = zalloc(sizeof(struct mojo_list));
    int len = strlen(s);
    str->contents = strdup(s);
    str->length = len;

    return str;
}

void free_string(struct mojo_string *s)
{
    if (s == NULL)
        return;

    free(s->contents);
    free(s);
}

void create_string_object(void)
{
	/* init number object */
	number_object = clone_object(base_object);
	number_object->type = T_STRING;
	number_object->name = "String";
	number_object->value.s_value = dup_string("");
	
	struct Object *plus_met = clone_object(builtin_object);
	plus_met->name = "+";
	plus_met->value.c_method = (struct Object * (*)(struct Object *parent, struct Object *arg1, 
						   struct Object *arg2))num_plus;

	list_append(number_object->methods, plus_met);
}
