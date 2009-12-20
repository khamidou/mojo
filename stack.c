#include "stack.h"
#include "zalloc.h"

void stack_push(struct Object *o)
{
	if (o == NULL)
		return;

	struct stack_elem *e = zalloc(sizeof(struct stack_elem));
	e->obj = o;

	TAILQ_INSERT_TAIL(&stack_head, e, stacks);
}

struct Object *stack_pop(void)
{
	struct stack_elem *e = TAILQ_LAST(&stack_head, stacks);
	TAILQ_REMOVE(&stack_head, e, stacks);
	
	/* Also free the associated object : */
	free(e->obj);
	free(e);
}
struct Object* stack_top(void)
{
	struct stack_elem *e = TAILQ_LAST(&stack_head, stacks);
	return e->obj;
}
