#ifndef __VM_H__
#define __VM_H__

#include <stdio.h>

int compile_image(FILE *fp);
void display_object(struct Object *obj, int nindents);
void execute(struct Object *ast);
struct Object* execute_branch(struct Object *ast);

#endif
