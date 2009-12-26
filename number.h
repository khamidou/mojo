#ifndef __NUMBER_H__
#define __NUMBER_H__

#include "object.h"

struct Object *num_plus(struct Object *parent, struct Object *n);
struct Object *num_minus(struct Object *parent, struct Object *n);
void create_number_object(void);

#endif
