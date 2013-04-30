#include <stdio.h>

#include "object.h"
#include "interp.h"

int main() {

	init_object_system();
    puts("this is mojo.");
	compile_image(stdin);
    return 0;
}
