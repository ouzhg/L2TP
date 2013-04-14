#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "HengZhang.h"
#include "Cabalar.h"
#include "prenex.h"

extern FILE* yyin;
	   FILE* fout;
extern int yyparse();

bool isUniversal(_formula* fml)
{
	assert(fml);
	
	switch(fml->formula_type)
	{
		case ATOM:
			return true;
		case CONJ:
		case DISJ:
		case IMPL:
			return isUniversal(fml->subformula_l) && isUniversal(fml->subformula_r);
		case NEGA:
		case UNIV:
			return isUniversal(fml->subformula_l);
		case EXIS:
			return false;
		default:
			assert(0);
	}
	
	return false;
}

_formulas* elimQuantifier(_formulas* fmls)
{	
    assert(fmls);

    _formulas* transFmls = NULL;
    _formulas* finalFmls = NULL;
    _formula* fml = NULL;

    while(fmls != NULL)
    {
        fml = top_formulas(fmls);
        fmls = pop_formulas(fmls);//pop out the head of the formulas
		
		

        if(isUniversal(fml))// no translation
        {
			finalFmls = push_formulas(fml,finalFmls);
		}
        else
		{
			fml = convert_prenex(fml);
			transFmls = Cabalar_Trans(fml);
			fmls = join_formulas(transFmls, fmls);
        }
    }

    return finalFmls;
}

void io(const char* iPathName, const char* oPathName)
{
    yyin = fopen (iPathName, "r");
	fout = fopen (oPathName, "w");

    if (!yyin)
    {
        printf("IO Error: cannot open the input file.\n" );
		assert(0);
    }
	if(!fout)
	{
		printf("IO Error: cannot open the output file.\n");
		assert(0);
	}
}

int main ( int argc, char** argv )
{
	if(argc < 3)
	{
		io("res/2","res/output/2");
	}
	else{
		io(argv[1], argv[2]);
	}

    init_vocabulary();
	
    yyparse();
	
	
	_formulas* 
	fmls = push_formulas(gformula,NULL);
	fmls = elimQuantifier(fmls);
	_rules* rules = convert_rules(fmls);
	
	
	//output
	fprintf(fout,"succ(X,Y) :- X==Y-1, i(X), i(Y).\n\n");
	fprintf(fout,"%% Formula translate:\n");
	output_rules(fout,rules);
	fprintf(fout,"\n");
	output_extraDefinition(fout);
	

	
	
	destroy_vocabulary();
	
	return 0;
}
