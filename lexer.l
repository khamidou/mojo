%{
	#include "tokens.h"
%}

%option yylineno 

%%


[0-9]+	{ return NUMBER; 	}
\"[:alnum:]+\"  { return STRING;	}
[:alnum:]+	{ return SYMBOL;	}
<<EOF>>		{ return T_EOF;		}
