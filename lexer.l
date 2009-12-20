%{
	#include "tokens.h"
%}

%option yylineno 

%%

[:alnum:]+	{ return SYMBOL;	}
[:digit:]+	{ return NUMBER; 	}
\"[:alnum:]+\"  { return STRING;	}

<<EOF>>		{ return EOF;		}
