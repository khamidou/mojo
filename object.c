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
