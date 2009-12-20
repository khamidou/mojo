#ifndef __STACK_H__
#define __STACK_H__

#include "object.h"
#include "queue.h"

struct stack_elem {
	Object *obj;
	TAILQ_ENTRY(stack_elem) stacks;
};

TAILQ_HEAD(Stacks_head, stack_elem) stack_head;

void stack_push(struct Object *o);
void stack_pop(struct Object *o);
struct Object* stack_top(void);

#endif
