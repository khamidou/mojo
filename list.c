#include "list.h"
#include "error.h"
#include "zalloc.h"

struct mojo_list* mojo_list_init(void)
{
	struct mojo_list *l = zalloc(sizeof(struct mojo_list));
	TAILQ_INIT(&l->mojo_list_head);

	return l;
}

void mojo_list_append(struct mojo_list *list, struct Object *o)
{
	if (list == NULL || o == NULL)
		return;

	struct mojo_list_elem *e = zalloc(sizeof(struct mojo_list_elem));
	e->obj = o;

	TAILQ_INSERT_TAIL(&list->mojo_list_head, e, mojo_lists);
}

void mojo_list_remove(struct mojo_list* list)
{
	struct mojo_list_elem *e = TAILQ_LAST(&list->mojo_list_head, Lists_head);
	TAILQ_REMOVE(&list->mojo_list_head, e, mojo_lists);
	
	/* Also free the associated object : */
	free(e->obj);
	free(e);
}

struct Object* mojo_list_nth(struct mojo_list *list, int n)
{
	if (list == NULL)
		return NULL;

	struct mojo_list_elem *e = (struct mojo_list_elem *) TAILQ_FIRST(&list->mojo_list_head);
	int i = 0;
	
	for (i = 0; i < n && TAILQ_NEXT(e, mojo_lists) != NULL; i++) {
		e = TAILQ_NEXT(e, mojo_lists);
	}

	return e->obj;
}

struct Object* mojo_list_last(struct mojo_list *list)
{
	if (list == NULL)
		return NULL;

	struct mojo_list_elem *e = TAILQ_LAST(&list->mojo_list_head, Lists_head);
	return e->obj;
}

int mojo_list_length(struct mojo_list *list)
{
	if (list == NULL)
		return -1;

	struct mojo_list_elem *e;
	int i = 0;
	TAILQ_FOREACH(e, &list->mojo_list_head, mojo_lists) {
		i++;
	}

	return i;
	
}
/*
  Here begins the public facing list api.

 */

struct Object* create_list_object(void)
{
	/* init base object - this needs to be called only once*/
	/* the rest of the time, this object is cloned */

	/* We can't use new_object because it calls create_list recursively */
	struct Object * list_object = (struct Object *) zalloc(sizeof(struct Object));

	if (list_object == NULL)
		error("Unable to create list object");

	list_object->super = nil_object;
	list_object->type = T_LIST;
	list_object->name = "List";
	list_object->value.l_value = mojo_list_init();

	/* Create a second list object that contains the methods of the list object */
	struct Object *l1 = (struct Object *) zalloc(sizeof(struct Object));
	struct Object *l2 = (struct Object *) zalloc(sizeof(struct Object));

	if (l1 == NULL || l2 == NULL)
		error("Unable to create list object at %s %d", __FILE__, __LINE__);

	l1->super = nil_object;
	l1->type = T_LIST;
	l1->name = "List";
	l1->value.l_value = mojo_list_init();
	l1->methods = nil_object; /* the methods of l1 are in l2, which is an empty list. This way, we don't fall into 
			     an infinite loop in clone_object()
			   */


	struct Object *met = zalloc(sizeof(struct Object));

	if (met == NULL)
		fail("Unable to create a method %d %d", __FILE__, __LINE__);

	met->type = T_BUILTIN;
	met->name = "+";
	met->value.c_method = BUILTIN_CAST list_append;
	list_append(l1, met);

	met = zalloc(sizeof(struct Object));

	if (met == NULL)
		fail("Unable to create a method %d %d", __FILE__, __LINE__);

	met->type = T_BUILTIN;
	met->name = "remove";
	met->value.c_method = BUILTIN_CAST list_remove;
	list_append(l1, met);
	
	met = zalloc(sizeof(struct Object));

	if (met == NULL)
		fail("Unable to create a method %d %d", __FILE__, __LINE__);

	met->type = T_BUILTIN;
	met->name = "nth:";
	met->value.c_method = BUILTIN_CAST list_nth;
	list_append(l1, met);

	met = zalloc(sizeof(struct Object));

	if (met == NULL)
		fail("Unable to create a method %d %d", __FILE__, __LINE__);

	met->type = T_BUILTIN;
	met->name = "last";
	met->value.c_method = BUILTIN_CAST list_last;
	list_append(l1, met);

	list_object->methods = l1;

	return list_object;
}

struct Object* list_append(struct Object *list, struct Object *o)
{
	if (list == NULL || list->type != T_LIST || list->type != T_BLOCK || list->value.l_value == NULL)
		return nil_object;

	if (o == NULL)
		return NULL;

	mojo_list_append(list->value.l_value, o);

	return o;
}

struct Object* list_remove(struct Object *list)
{
	if (list == NULL || list->type != T_LIST || list->type != T_BLOCK || list->value.l_value == NULL)
		return nil_object;

	mojo_list_remove(list->value.l_value);

	return nil_object;
}

struct Object* list_nth(struct Object *list, int n)
{
	if (list == NULL || list->type != T_LIST || list->type != T_BLOCK || list->value.l_value == NULL)
		return nil_object;

	struct Object *o = mojo_list_nth(list->value.l_value, n);

	if (o != NULL)
		return o;
	else
		return nil_object;

}


struct Object* list_last(struct Object *list)
{
	if (list == NULL || list->type != T_LIST || list->type != T_BLOCK || list->value.l_value == NULL)
		return nil_object;

	struct Object *o = mojo_list_last(list->value.l_value);

	if (o != NULL)
		return o;
	else
		return nil_object;
	
}

struct Object* list_length(struct Object *list)
{
	if (list == NULL || list->type != T_LIST || list->type != T_BLOCK || list->value.l_value == NULL)
		return nil_object;

	struct Object *o = clone_object(number_object);
	if (o == NULL)
		fail("Unable to clone number_object in %s at line %d", __FILE__, __LINE__);

	o->value.i_value =  mojo_list_length(list->value.l_value);

	return o;
}
