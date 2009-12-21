#include <stdio.h>

#include "object.h"

main() {
	init_object_system();
	eval_image(stdin);
}
