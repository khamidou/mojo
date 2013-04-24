#include "number.h"


struct Object *num_plus(struct Object *parent, struct Object *n)
{
	if (parent == NULL || n == NULL || 
	    parent->type != T_NUMBER || n->type != T_NUMBER)
		return NULL;

	struct Object* ret = clone_object(parent);
	if (ret == NULL)
		fail("Unable to clone object at %s, %d", __FILE__, __LINE__);

	ret->value.i_value += n->value.i_value;

	return ret;
}

struct Object *num_minus(struct Object *parent, struct Object *n)
{
	if (parent == NULL || n == NULL || 
	    parent->type != T_NUMBER || n->type != T_NUMBER)
		return NULL;

	struct Object* ret = clone_object(parent);
	if (ret == NULL)
		fail("Unable to clone object at %s, %d", __FILE__, __LINE__);

	ret->value.i_value -= n->value.i_value;
	
	return ret;
}

void create_number_object(void)
{
	/* init number object */
	number_object = clone_object(base_object);
	number_object->type = T_NUMBER;
	number_object->name = "Number";
	number_object->value.i_value = 0;
	
	struct Object *plus_met = clone_object(builtin_object);
	plus_met->name = "+";
	plus_met->value.c_method = (struct Object * (*)(struct Object *parent, struct Object *arg1, 
						   struct Object *arg2))num_plus;

	list_append(number_object->methods, plus_met);

	struct Object *minus_met = clone_object(builtin_object);
	minus_met->name = "-";
	minus_met->value.c_method = (struct Object * (*)(struct Object *parent, struct Object *arg1, 
						   struct Object *arg2))num_minus;


	list_append(number_object->methods, minus_met);
}
