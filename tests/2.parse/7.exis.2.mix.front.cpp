#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/7.exis/2.mix.front.in", "r" );
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
	assert(gformula->formula_type == UNIV);
	assert(gformula->variable_id == symbol_X);
	assert(gformula->subformula_l);
	assert(gformula->subformula_l->formula_type == EXIS);
	assert(gformula->subformula_l->variable_id == symbol_Y);
	assert(gformula->subformula_l->subformula_l);
	assert(gformula->subformula_l->subformula_l->formula_type == ATOM);
	assert(gformula->subformula_l->subformula_l->predicate_id == symbol_p);
	
	destroy_vocabulary();
	return 0;
}

