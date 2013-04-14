#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/1.atom/2.vari.2.in", "r" );
	if(!yyin)
    {
        printf("Parser Error: cannot open the input file.\n");
		assert(false);
    }
	
	yyparse();
	
	int symbol_p = query_symbol("p",PREDICATE);
	assert(symbol_p >= 0);
	
	int symbol_X = query_symbol("X",VARIABLE);
	assert(symbol_X >= 0);
	
	int symbol_Y = query_symbol("Y",VARIABLE);
	assert(symbol_Y >= 0);
	
	//validation
	assert(gformula);
	assert(gformula->formula_type == ATOM);
	assert(gformula->predicate_id == symbol_p);
	
	assert(gformula->parameters);
	assert(predicate_arity(gformula->predicate_id) == 2);
	
	assert(gformula->parameters->term_type == VARI);
	assert(gformula->parameters->variable_id == symbol_X);	
	
	assert(gformula->parameters[1].term_type == VARI);
	assert(gformula->parameters[1].variable_id == symbol_Y);
	
	destroy_vocabulary();
	return 0;
}

