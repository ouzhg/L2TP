#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/1.atom/0.single.in", "r" );
	if(!yyin)
    {
        printf("Parser Error: cannot open the input file.\n");
		assert(false);
    }
	
	yyparse();
	
	int symbol_p = query_symbol("p",PREDICATE);
	assert(symbol_p >= 0);
	
	//validation
	assert(gformula);
	assert(gformula->formula_type == ATOM);
	assert(gformula->predicate_id == symbol_p);
	assert(gformula->parameters == NULL);
	
	destroy_vocabulary();
	return 0;
}

