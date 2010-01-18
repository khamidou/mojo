%{
	#include "tokens.h"
%}

%option yylineno 

%%


[0-9]+		{ return NUMBER; 	}
;		{ return COMMA;		}
:=		{ return AFFECTATION;   }
\"[:alnum:]+\"  { return STRING;	}
[a-zA-Z][a-zA-Z0-9]*	{ return SYMBOL;	}
<<EOF>>		{ exit(0);}
