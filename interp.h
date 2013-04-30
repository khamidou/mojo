#ifndef __VM_H__
#define __VM_H__

#include <stdio.h>

int compile_image(FILE *fp);
void display_object(struct Object *obj, int nindents);
void execute(struct Object *ast);
struct Object* execute_branch(struct Object *ast, struct Object *scope);


int expression(struct Object *ast);
int message(struct Object *ast);

void display_ast(struct Object *ast, int nindents);
    

#endif
