#include <stdio.h>
#include <assert.h>

#include "utility.h"

extern FILE* yyin;
extern int yyparse();

int main(int argc, char** argv) {
	
	init_vocabulary();
	yyin = fopen ( "res/1.atom/3.func.4.double.in", "r" );
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
	
	//validation
	assert(gformula);
	assert(gformula->formula_type == ATOM);
	assert(gformula->predicate_id == symbol_p);
	
	assert(gformula->parameters);
	assert(predicate_arity(gformula->predicate_id) == 2);
	
	assert(gformula->parameters[0].term_type == FUNC);
	assert(gformula->parameters[0].function_id == symbol_a);
	
	assert(gformula->parameters[1].term_type == FUNC);
	assert(gformula->parameters[1].function_id == symbol_b);
	
	assert(gformula->parameters[0].parameters);
	assert(function_arity(symbol_a) == 2);
	
	assert(gformula->parameters[1].parameters);
	assert(function_arity(symbol_b) == 2);
	
	assert(gformula->parameters[0].parameters[0].term_type == VARI);
	assert(gformula->parameters[0].parameters[0].variable_id == symbol_X);
	
	assert(gformula->parameters[0].parameters[1].term_type == VARI);
	assert(gformula->parameters[0].parameters[1].variable_id == symbol_Y);
	
	assert(gformula->parameters[1].parameters[0].term_type == VARI);
	assert(gformula->parameters[1].parameters[0].variable_id == symbol_Y);
	
	assert(gformula->parameters[1].parameters[1].term_type == VARI);
	assert(gformula->parameters[1].parameters[1].variable_id == symbol_X);
	
	destroy_vocabulary();
	return 0;
}

