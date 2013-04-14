#include <stdio.h>
#include <assert.h>

#include "Cabalar.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/A.cabalar/0.normal.l4.in", "r" );
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
	
	int symbol_k = query_symbol("k",PREDICATE);
	assert(symbol_k >= 0);
	
	//convert
	_formulas *fmls = Cabalar_Trans(gformula);
	assert(fmls);
	assert(fmls->curr_formula);
	assert(fmls->remained_formulas);
	assert(fmls->remained_formulas->curr_formula);
	
	//validation
	_formula *fml1 = fmls->curr_formula;
	_formula *fml2 = fmls->remained_formulas->curr_formula;
	
	assert(fml1->formula_type == IMPL);
	assert(fml1->subformula_l != NULL);
	assert(fml1->subformula_r != NULL);
	
	assert(fml1->subformula_l->formula_type == CONJ);
	assert(fml1->subformula_l->subformula_l != NULL);
	assert(fml1->subformula_l->subformula_r != NULL);
	
	assert(fml1->subformula_l->subformula_l->formula_type == ATOM);
	assert(fml1->subformula_l->subformula_l->predicate_id == symbol_g);

	assert(fml1->subformula_l->subformula_r->formula_type == ATOM);
	assert(fml1->subformula_l->subformula_r->predicate_id == symbol_h);

	assert(fml1->subformula_r->formula_type == ATOM);
	assert(fml1->subformula_r->predicate_id == symbol_k);
	
	//fml2
	assert(fml2->formula_type == IMPL);
	assert(fml2->subformula_l != NULL);
	assert(fml2->subformula_r != NULL);
	
	assert(fml2->subformula_l->formula_type == CONJ);
	assert(fml2->subformula_l->subformula_l != NULL);
	assert(fml2->subformula_l->subformula_r != NULL);
	
	assert(fml2->subformula_l->subformula_l->formula_type == ATOM);
	assert(fml2->subformula_l->subformula_l->predicate_id == symbol_f);

	assert(fml2->subformula_l->subformula_r->formula_type == ATOM);
	assert(fml2->subformula_l->subformula_r->predicate_id == symbol_h);

	assert(fml2->subformula_r->formula_type == ATOM);
	assert(fml2->subformula_r->predicate_id == symbol_k);
	
	destroy_vocabulary();
	return 0;
}

