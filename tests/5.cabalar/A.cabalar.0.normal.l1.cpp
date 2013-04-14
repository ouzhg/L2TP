#include <stdio.h>
#include <assert.h>

#include "Cabalar.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/A.cabalar/0.normal.l1.in", "r" );
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
	assert(fml->formula_type == IMPL);
	assert(fml->subformula_l != NULL);
	assert(fml->subformula_r != NULL);
	
	assert(fml->subformula_l->formula_type == ATOM);
	assert(fml->subformula_l->predicate_id == symbol_f);
	
	assert(fml->subformula_r->formula_type == ATOM);
	assert(fml->subformula_r->predicate_id == symbol_g);
	
	destroy_vocabulary();
	return 0;
}

