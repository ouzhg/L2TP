#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "prenex.h"
#include "HengZhang.h"
#include "Cabalar.h"
#include "rule.h"

extern FILE* yyin;
	   FILE* fout;
extern int yyparse();

_formulas* elim_quantifier(_formulas* fmls)
{	
    assert(fmls);

    _formulas* transFmls = NULL;
    _formulas* finalFmls = NULL;
    _formula* fml = NULL;

    while(fmls != NULL)
    {
        fml = top_formulas(fmls);
        fmls = pop_formulas(fmls);//pop out the head of the formulas

        if(is_universal(fml))// no translation
        {
			finalFmls = push_formulas(fml,finalFmls);
		}
        else
		{
			fml = convert_prenex(fml);
			transFmls = convert_Cabalar(HengZhang_create(fml));
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
    _formulas *fmls;
    _rules *rules;
    
    //input
	if(argc < 3)
	{
		io("res/C.sample/sample.in","output/C.sample/sample.out");
	}
	else{
		io(argv[1], argv[2]);
	}
    
    //parse
    init_vocabulary();
    yyparse();
	
	//convert
	fmls = push_formulas(gformula,NULL);
	fmls = elim_quantifier(fmls);
	rules= convert_rules(fmls);
	
	
	//output
	fprintf(fout,"succ(X,Y) :- X==Y-1, i(X), i(Y).\n\n");
	output_rules(fout,rules);
	fprintf(fout,"\n");
	
    //clean
	destroy_vocabulary();
	return 0;
}
