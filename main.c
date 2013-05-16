#include <stdio.h>

#include "object.h"
#include "interp.h"
#include "error.h"

extern FILE *yyin;

int main(int argc, char **argv) {

    yyin = stdin;
    if(argc == 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fail("unable to open file : %s", argv[1]);
        }
    }

	init_object_system();
    puts("this is mojo.");
	compile_image();
    return 0;
}
