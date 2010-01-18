#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "tokens.h"
#include "object.h"

#define source_prologue "#include <stdio.h>\n\
#include <stdlib.h>\n\
#include \"object.h\"\n\
\n\
int main(int argc, char **argv) {\n\
	struct Object *Lexer;\n"

#define source_epilogue "}"

#endif
