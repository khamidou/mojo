#include "list.h"
#include "zalloc.h"

struct mojo_list* list_init(void)
{
	struct mojo_list *l = zalloc(sizeof(struct mojo_list));
	TAILQ_INIT(&l->mojo_list_head);

	return l;
}

void list_append(struct Object *list, struct Object *o)
{
	if (list == NULL || o == NULL || list->type != T_LIST)
		return;

	struct list_elem *e = zalloc(sizeof(struct list_elem));
	e->obj = o;

	TAILQ_INSERT_TAIL(&list->mojo_list_head, e, mojo_lists);
}

void list_remove(struct Object * list, struct Object *o)
{
	struct list_elem *e = TAILQ_LAST(&list->mojo_list_head, mojo_lists);
	TAILQ_REMOVE(&list->mojo_list_head, e, mojo_lists);
	
	/* Also free the associated object : */
	free(e->obj);
	free(e);
}

struct Object* list_nth(struct Object *list, int n)
{
	if (list == NULL)
		return NULL;

	struct Object *o = TAILQ_FIRST(&list->mojo_list_head);
	int i = 0;
	
	for (i = 0; i < n && o != NULL; i++) {
		o = TAILQ_NEXT(o);
	}

	return o;
}

struct Object* list_last(struct Object *list)
{
	struct list_elem *e = TAILQ_LAST(&mojo_list_head, lists);
	return e->obj;
}

/*
  Here begins the public facing list api.

 */

struct Object* create_list(void)
{
	/* init base object */
	struct Object *list_object
	base_object = new_object();
	base_object->super = nil_object;
	base_object->type = T_OBJECT;
	base_object->name = "Object";

}
