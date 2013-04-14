#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/2.nega/3.clause-double.in", "r" );
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
	assert(gformula->formula_type == NEGA);
	assert(gformula->subformula_l);
	assert(gformula->subformula_l->formula_type == NEGA);
	assert(gformula->subformula_l->subformula_l);
	assert(gformula->subformula_l->subformula_l->formula_type == ATOM);
	assert(gformula->subformula_l->subformula_l->predicate_id == symbol_p);
	
	destroy_vocabulary();
	return 0;
}

