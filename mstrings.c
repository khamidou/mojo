#include "mstrings.h"

void create_string_object(void)
{
	/* init number object */
	number_object = clone_object(base_object);
	number_object->type = T_STRING;
	number_object->name = "String";
	number_object->value.i_value = 0;
	
	struct Object *plus_met = clone_object(builtin_object);
	plus_met->name = "+";
	plus_met->value.c_method = (struct Object * (*)(struct Object *parent, struct Object *arg1, 
						   struct Object *arg2))num_plus;

	list_append(number_object->methods, plus_met);
}
