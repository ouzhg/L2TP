#include <stdio.h>
#include <assert.h>

#include "Cabalar.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/A.cabalar/0.normal.l3.in", "r" );
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
	
	int symbol_h = query_symbol("h",PREDICATE);
	assert(symbol_h >= 0);
	
	//convert
	_formulas *fmls = Cabalar_Trans(gformula);
	assert(fmls);
	assert(fmls->curr_formula);
	assert(fmls->remained_formulas == NULL);
	
	//validation
	_formula *fml = fmls->curr_formula;
	assert(fml->formula_type == IMPL);
	assert(fml->subformula_l);
	assert(fml->subformula_r);
	
	assert(fml->subformula_l->formula_type == ATOM);
	assert(fml->subformula_l->predicate_id == symbol_g);
	
	assert(fml->subformula_r->formula_type == DISJ);
	assert(fml->subformula_r->subformula_l);
	assert(fml->subformula_r->subformula_r);
	
	assert(fml->subformula_r->subformula_l->formula_type == NEGA);
	assert(fml->subformula_r->subformula_l->subformula_l);
	
	assert(fml->subformula_r->subformula_l->subformula_l->formula_type == ATOM);
	assert(fml->subformula_r->subformula_l->subformula_l->predicate_id == symbol_f);
	
	assert(fml->subformula_r->subformula_r->formula_type == ATOM);
	assert(fml->subformula_r->subformula_r->predicate_id == symbol_h);
	
	destroy_vocabulary();
	return 0;
}

