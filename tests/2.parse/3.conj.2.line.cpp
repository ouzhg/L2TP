#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/3.conj/2.line.in", "r" );
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
	assert(gformula->formula_type == CONJ);
	assert(gformula->subformula_l);
	assert(gformula->subformula_r);
	
	assert(gformula->subformula_l->formula_type == CONJ);
	assert(gformula->subformula_l->subformula_l);
	assert(gformula->subformula_l->subformula_r);
	
	assert(gformula->subformula_r->formula_type == ATOM);
	
	destroy_vocabulary();
	return 0;
}

