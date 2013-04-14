#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/1.atom/3.func.3.vari.in", "r" );
	if(!yyin)
    {
        printf("Parser Error: cannot open the input file.\n");
		assert(false);
    }
	
	yyparse();
	
	int symbol_p = query_symbol("p",PREDICATE);
	assert(symbol_p >= 0);
	
	int symbol_a = query_symbol("a",FUNCTION);
	assert(symbol_a >= 0);
	
	int symbol_A = query_symbol("A",VARIABLE);
	assert(symbol_A >= 0);
	
	//validation
	assert(gformula);
	assert(gformula->formula_type == ATOM);
	assert(gformula->predicate_id == symbol_p);
	
	assert(gformula->parameters);
	assert(predicate_arity(gformula->predicate_id) == 1);
	
	assert(gformula->parameters->term_type == FUNC);
	assert(gformula->parameters->function_id == symbol_a);	
	
	assert(gformula->parameters->parameters);
	assert(function_arity(symbol_a) == 1);
	
	assert(gformula->parameters->parameters->term_type == VARI);
	assert(gformula->parameters->parameters->variable_id == symbol_A);
	
	destroy_vocabulary();
	return 0;
}

