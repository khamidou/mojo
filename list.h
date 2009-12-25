#ifndef __LIST_H__
#define __LIST_H__

#include "object.h"
#include "queue.h"

struct mojo_list_elem {
	struct Object *obj;
	TAILQ_ENTRY(mojo_list_elem) mojo_lists;
};

struct mojo_list {
	TAILQ_HEAD(Lists_head, mojo_list_elem) mojo_list_head;
};

struct mojo_list* list_init(void);
struct Object* list_append(struct Object *list, struct Object *o);
struct Object* list_remove(struct Object *list);
struct Object* list_nth(struct Object *list, int n);
struct Object* list_last(struct Object *list);
struct Object* create_list_object(void);

#endif