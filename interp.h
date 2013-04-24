#ifndef __VM_H__
#define __VM_H__

#include <stdio.h>

int compile_image(FILE *fp);
void display_object(struct Object *obj);
void execute(struct Object *ast);

#endif
