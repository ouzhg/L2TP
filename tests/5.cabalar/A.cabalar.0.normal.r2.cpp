#include <stdio.h>
#include <assert.h>

#include "Cabalar.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/A.cabalar/0.normal.r2.in", "r" );
	if(!yyin)
    {
        printf("Parser Error: cannot open the input file.\n");
		assert(false);
    }
	yyparse();
	
	//symbols
	int symbol_TRUE = PRED_TRUE;
	
	int symbol_f = query_symbol("f",PREDICATE);
	assert(symbol_f >= 0);
	
	int symbol_g = query_symbol("g",PREDICATE);
	assert(symbol_g >= 0);
	
	//convert
	_formulas *fmls = Cabalar_Trans(gformula);
	assert(fmls);
	assert(fmls->curr_formula);
	assert(fmls->remained_formulas == NULL);
	
	//validation
	_formula *fml = fmls->curr_formula;
	assert(fml->formula_type == ATOM);
	assert(fml->predicate_id == symbol_TRUE);
	
	destroy_vocabulary();
	return 0;
}

