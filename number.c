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
