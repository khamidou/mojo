#ifndef __LIST_H__
#define __LIST_H__

#include "object.h"
#include "queue.h"

/* see http://blog.jasonish.org/2006/08/tailq-example.html for help on bsd tail queues */
struct mojo_list_elem {
	struct Object *obj;
	TAILQ_ENTRY(mojo_list_elem) mojo_lists;
};

struct mojo_list {
	TAILQ_HEAD(Lists_head, mojo_list_elem) mojo_list_head;
};

struct mojo_list* mojo_list_init(void);
int mojo_list_length(struct mojo_list *list);
void mojo_list_append(struct mojo_list *list, struct Object *o);
void mojo_list_remove(struct mojo_list* list);
struct Object* mojo_list_nth(struct mojo_list *list, int n);
struct Object* mojo_list_last(struct mojo_list *list);
void mojo_list_free(struct mojo_list *list);

struct Object* list_append(struct Object *list, struct Object *o);
struct Object* list_remove(struct Object *list);
struct Object* list_nth(struct Object *list, int n);
struct Object* list_last(struct Object *list);
struct Object* create_list_object(void);

#endif
