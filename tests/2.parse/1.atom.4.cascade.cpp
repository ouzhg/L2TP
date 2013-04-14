#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/1.atom/4.cascade.in", "r" );
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
	
	int symbol_b = query_symbol("b",FUNCTION);
	assert(symbol_b >= 0);
	
	int symbol_X = query_symbol("X",VARIABLE);
	assert(symbol_X >= 0);
	
	int symbol_Y = query_symbol("Y",VARIABLE);
	assert(symbol_Y >= 0);

	int symbol_Z = query_symbol("Z",VARIABLE);
	assert(symbol_Z >= 0);	

	//validation
	assert(gformula);
	assert(gformula->formula_type == ATOM);
	assert(gformula->predicate_id == symbol_p);
	assert(gformula->parameters);
	assert(predicate_arity(symbol_p) == 1);
	
	assert(gformula->parameters->term_type == FUNC);
	assert(gformula->parameters->function_id == symbol_a);
	assert(gformula->parameters->parameters);
	assert(function_arity(symbol_a) == 1);
	
	assert(gformula->parameters->parameters->term_type == FUNC);
	assert(gformula->parameters->parameters->function_id == symbol_b);
	assert(gformula->parameters->parameters->parameters);
	assert(function_arity(symbol_b) == 3);
	
	_term *b_parameters = gformula->parameters->parameters->parameters;
	
	assert(b_parameters[0].term_type == VARI);
	assert(b_parameters[0].variable_id == symbol_X);
	
	assert(b_parameters[1].term_type == VARI);
	assert(b_parameters[1].variable_id == symbol_Y);
	
	assert(b_parameters[2].term_type == VARI);
	assert(b_parameters[2].variable_id == symbol_Z);
	
	destroy_vocabulary();
	return 0;
}

