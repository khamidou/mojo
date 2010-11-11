/* bytecode.c - bytecode interpretation 

   the structure of the bytecode is as follows:
   first, an opcode, followed by its parameters. The paramaters are separated
   by commas. A dot signals the end of sequence.

   The following opcodes are supported for the moment:
   PUSH val : push a value on top of the stack
   POP : remove the value on the top of the stack
   SEND_UNARY: send a message to an object with one parameter. The order of the parameters on the stack is :
   		object, message, param, from top to bottom
   SEND_BINARY: FIXME: yet to implement.
   ASSIGN val 
*/

enum {PUSH, POP, SEND_UNARY, SEND_BINARY, NEW, ASSIGN}

struct Object *stack;

init vm(void)
{
	stack = create_list_object();
}

/* get a copy of the next word contained in the bytecode */
char *get_next_bytecode_word(char **ptr)
{
	int len = 0;
	char *c = *ptr;
	while(*c != ',' && c!= '.' && c != 0) {
		len++;
		c++;
	}

	char retptr = calloc(len * sizeof(char), 1);

	if (!retptr)  
		fail("Unable to alloc in __FILE__:__LINE__");

	c = *ptr;
	while (*c != ',' && c!= '.' && c != 0) {
		*retptr++ = *c++;
	}

	/* don't forget to advance ptr by length + 1. */
	*ptr += len + 1;
	return retptr;

}		

int execute_bytecode(char *ptr)
{

	char c;

	while(*ptr) {
		switch(*ptr) {
			case PUSH:
				c = get_next_word(&ptr);
				//stack->append(
				break;

		}
	}
}
