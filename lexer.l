%{
	#include "tokens.h"
%}

%option yylineno 
%s raw

%%


[0-9]+		{ return NUMBER; 	}
;		{ return SEMICOLON;		}
:=		{ return AFFECTATION;   }
\"[:alnum:]+\"  { return STRING;	}
[a-zA-Z][a-zA-Z0-9]*	{ return SYMBOL;	}
<<EOF>>		{ return; }
\%\{		{ BEGIN(raw); }
<raw>\%\}		{ BEGIN(INITIAL); }
<raw>.+       { printf("%s", yytext); }
