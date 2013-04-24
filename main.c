#include <stdio.h>

#include "object.h"

main() {

	init_object_system();
    puts("this is mojo.");
	compile_image(stdin);

}
