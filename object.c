#include "object.h"

struct Method *lookup_method(struct Object *o, char *name)
{
	if (name == NULL)
		return NULL;

	struct Method *m;

	TAILQ_FOREACH(m, &methods_head, methods) {
		if (strcmp(name, m->name) == 0)
			return m;
	}

	return NULL;
}

struct Object *clone_object(struct Object *o)
{
	if (o == NULL)
		return NULL;

	struct Object *new = zalloc(sizeof(struct Object *));
	new->super = o->super;
	new->type = o->type;
	new->name = strndup(o->name, 255); /* FIXME : test the value of strdup */

	struct Method *m;
	struct Method *new_method;
	TAILQ_INIT(new->methods_head);
	TAILQ_FOREACH(m, &o->methods_head, methods) {
		new_method = clone_method(m);
		TAILQ_INSERT_TAIL(&new->methods_head, new_method, methods);
	}
	
}

void free_object(struct Object *o)
{
	if (o == NULL)
		return;

	free(o->name);

	struct Method *m;
	TAILQ_FOREACH(m, &o->methods_head, methods) {
		free_method(m);
	}
}

struct Method *clone_method(struct Method *m)
{
	if (m == NULL)
		return NULL;

	struct Method *new = zalloc(sizeof(struct Method));
	
	new->name = strndup(m->name, 255);
	new->c_method = m->c_method;
	new->parent = m->parent;

	return new;
}

void free_method(struct Method *m)
{
	if (m == NULL)
		return;
	TAILQ_REMOVE(m->parent->methods_head, m, methods); 
	free(m->name);
	free(m);
}
