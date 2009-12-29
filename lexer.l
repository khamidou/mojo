%{
	#include "tokens.h"
%}

%option yylineno 

%%


[0-9]+		{ return NUMBER; 	}
";"		{ return COMMA;		}
\"[:alnum:]+\"  { return STRING;	}
[:alnum:]+	{ return SYMBOL;	}
<<EOF>>		{ return T_EOF;		}
