#include "HengZhang.h"

#include <stdlib.h>
#include <string.h>

static std::vector<int> terms_X;
static std::vector<int> terms_Y;
static std::vector<int> terms_Z;
static std::vector<int> terms_MIN;//waste space
static std::vector<int> terms_MAX;//waste space

static int symbol_s;
static int symbol_t;
static int symbol_MAX;
static int symbol_MIN;
static int symbol_succ;

int HengZhang_addSymbol(const char* name, SYMBOL_TYPE type, int arity)
{
	char name_buf[512];
	int i = 0;
	
	sprintf(name_buf,"%s",name);
	
	while( query_symbol(name_buf,type) >= 0) // symbol exist
	{
		sprintf(name_buf,"%s_%d",name,i);
	}
	
	return add_symbol(name_buf,type,arity);
}

_formula* HengZhang_recordQuantifier(_formula* fml)
{
	char str_buf[512];
	_formula* fml_temp;

    terms_X.clear();
    terms_Y.clear();
    terms_Z.clear();
    terms_MIN.clear();
    terms_MAX.clear();
    
	int i = 0;
    while(true)
    {
        if(fml->formula_type == UNIV)
        {
            terms_X.push_back(fml->variable_id);
        }
        else if(fml->formula_type == EXIS)
        {
			terms_Y  .push_back(fml->variable_id);
            terms_MIN.push_back(symbol_MIN);
            terms_MAX.push_back(symbol_MAX);
			
			sprintf(str_buf,"NV_%d",i++);
            terms_Z  .push_back(HengZhang_addSymbol(str_buf, VARIABLE, 0));
        }
        else
        {
            break;
        }
		
		fml_temp = fml;
		fml = fml->subformula_l;
		free(fml_temp);
    }
    return fml;
}

_formulas* HengZhang_create(_formula* fml)
{
	symbol_MAX = HengZhang_addSymbol("MAX", VARIABLE, 0);
	symbol_MIN = HengZhang_addSymbol("MIN", VARIABLE, 0);
	
	_formula* originalFml = HengZhang_recordQuantifier(fml);
	
	if(terms_Y.size() == 0)
	{
		_formulas* fmls = NULL;
		fmls = push_formulas(fml,fmls);
		return fmls;
	}

	
	symbol_s = HengZhang_addSymbol("s", PREDICATE, 
												terms_X.size()+terms_Y.size());
    symbol_t = HengZhang_addSymbol("t", PREDICATE, 
												terms_X.size()+terms_Y.size());
	symbol_succ = HengZhang_addSymbol("succ", PREDICATE, 
												terms_Y.size()+terms_Z.size());
	
    _formulas* fmls = NULL;
    fmls = push_formulas(HengZhang_createFormula_1(originalFml), fmls);
    fmls = push_formulas(HengZhang_createFormula_2(originalFml), fmls);
    fmls = push_formulas(HengZhang_createFormula_3(originalFml), fmls);
    fmls = push_formulas(HengZhang_createFormula_4(originalFml), fmls);
    fmls = push_formulas(HengZhang_createFormula_5(originalFml), fmls);

    return fmls;
}

// HengZhang formula_1
// --s(_X,_MIN)
///////////////////////////////////////////////////////////////////////////////
//                    NEGA
//                     |
//              ---------------
//              |              |
//            NEGA            NULL
//       --------------
//       |             |
//   s(_X,_MIN)      NULL
///////////////////////////////////////////////////////////////////////////////
_formula* HengZhang_createFormula_1(_formula* originalFml)
{
    // Add S(_X, _MIN)
    _term* LLT   = combine_terms(terms_X, terms_MIN);
    _formula* LL = composite_atom(ATOM, symbol_s, LLT);

    // create structure
    _formula* L  = composite_bool(NEGA, LL, NULL);
    _formula* F  = composite_bool(NEGA, L,  NULL);
	
    return F;
}

// HengZhang formula_2
// (_succ(_Y,_Z) ^ s(_X,_Z)) v theta__(_X,_Y) -> s(_X,_Y)
///////////////////////////////////////////////////////////////////////////////
//                                          IMPL
//                                           |
//                          --------------------------------
//                          |                               |
//                         DISJ                         s(_X,_Y)
//                          |
//               ------------------------
//               |                      |
//             CONJ              theta__(_X,_Y)
//               |
//   ------------------------
//   |                      |
//_succ(_Y,_Z)         s(_X,_Z)
///////////////////////////////////////////////////////////////////////////////
_formula* HengZhang_createFormula_2(_formula* originalFml)
{
    //create atom formulas
    _term* LLLT = combine_terms(terms_Y, terms_Z);
    _formula* LLL = composite_atom(ATOM, symbol_succ, LLLT);

    //2
    _term* LLRT = combine_terms(terms_X, terms_Z);
    _formula* LLR = composite_atom(ATOM, symbol_s, LLRT);

    //3
    _formula* LR = double_negation(copy_formula(originalFml),
			vocabulary.index_stable_predicate,
			vocabulary.num_stable_predicate);

    //4
    _term* RT = combine_terms(terms_X, terms_Y);
    _formula* R   = composite_atom(ATOM, symbol_s, RT);

    //create structure
    _formula* LL  = composite_bool(CONJ,LLL,LLR);
    _formula* L   = composite_bool(DISJ,LL,LR);
    _formula* F   = composite_bool(IMPL,L,R);
    return F;
}

// HengZhang formula_3
// t(_X, _MIN) v theta(_X,_MIN)
///////////////////////////////////////////////////////////////////////////////
//                         DISJ
//                          |
//         ------------------------------------
//         |                                  |
//    t(_X,_MIN)                        theta(_X,_MIN)
///////////////////////////////////////////////////////////////////////////////
_formula* HengZhang_createFormula_3(_formula* originalFml)
{
    //create left sub-formula t(_X,_MIN)
    _term* LT = combine_terms(terms_X, terms_MIN);
    _formula* L   = composite_atom(ATOM, symbol_t, LT );

    //create right sub-formula theta(_X,_MIN)
    _formula* R = replace_terms(copy_formula(originalFml), terms_Y, terms_MIN);

    //create structure
    _formula* F   = composite_bool(DISJ,L,R);
    return F;
}

// HengZhang formula_4
// -(_succ(_Y,_Z)) v -(S(_X,_Z)) -> ( t(_T,_MAX)->theta(_X,_Y)
//                    ^ s(_X,_Y) ->   ^ (theta(_X,_Y)->t(_T,_MAX) )
///////////////////////////////////////////////////////////////////////////////
//                                   IMPL
//                                    |
//                    -----------------------------------
//                    |                                 |
//                   CONJ                              CONJ
//                    |                                 |
//             ----------------             ------------------------
//             |              |             |                      |
//            DISJ         s(_X,_Y)        IMPL                   IMPL
//             |                            |                      |
//        ------------------          ---------------       ---------------
//        |                |          |             |       |              |
//       NEGA             NEGA    t(_X,_MAX)  theta(_X,_Y) theta(_X,_Y) t(_X,_MAX)
//        |                |
//    -------------       ----------
//    |           |       |        |
// _succ(_Y,_Z)  NULL   s(_X,_Z)  NULL
///////////////////////////////////////////////////////////////////////////////
_formula* HengZhang_createFormula_4(_formula* originalFml)
{
    //create atom formulas
    _term* LLLLT = combine_terms(terms_Y, terms_Z);
    _formula* LLLL  = composite_atom(ATOM, symbol_succ, LLLLT);

    //2
    _term* LLRLT = combine_terms(terms_X,terms_Z);
    _formula* LLRL  = composite_atom(ATOM, symbol_s, LLRLT);

    //3
    _term* LRT = combine_terms(terms_X, terms_Y);
    _formula* LR  = composite_atom(ATOM, symbol_s, LRT);

    //4
    _term* RLLT = combine_terms(terms_X, terms_MAX);
    _formula* RLL  = composite_atom(ATOM, symbol_t, RLLT);

    //5
    _formula* RLR = copy_formula(originalFml);

    //6
    _formula* RRL = copy_formula(originalFml);

    //7
    _term* RRRT = combine_terms(terms_X, terms_MAX);
    _formula* RRR  = composite_atom(ATOM, symbol_t, RRRT);

    //create structure
    _formula* LLL = composite_bool(NEGA,LLLL,NULL);
    _formula* LLR = composite_bool(NEGA,LLRL,NULL);
    _formula* LL  = composite_bool(DISJ,LLL,LLR);
    _formula* L   = composite_bool(CONJ,LL,LR);
    _formula* RL  = composite_bool(IMPL,RLL,RLR);
    _formula* RR  = composite_bool(IMPL,RRL,RRR);
    _formula* R   = composite_bool(CONJ,RL,RR);
    _formula* F   = composite_bool(IMPL,L,R);
    return F;
}
// HengZhang formula_5
// _succ(_Y,_Z)-> ( T(_X,_Y)->theta(_X,_Z) ) 
//                  ^ (theta(_X,_Z)->t(_X,_Y)
//				  ) 
//                v t(_X,_Z)
///////////////////////////////////////////////////////////////////////////////
//                      IMPL
//                       |
//     ---------------------------------
//     |                               |
//   _succ(_Y,_Z)                     DISJ
//                                     |
//                          -----------------------
//                          |                     |
//                         CONJ                t(_X,_Z)
//                          |
//            ----------------------------
//            |                          |
//           IMPL                       IMPL
//            |                          |
//      -------------             ----------------
//      |           |             |              |
//  t(_X,_Y)   theta(_X,_Z)   theta(_X,_Z)    t(_X,_Y)
///////////////////////////////////////////////////////////////////////////////
_formula* HengZhang_createFormula_5(_formula* originalFml)
{
    //create atom formulas
    _term* LT = combine_terms(terms_Y, terms_Z);
    _formula* L  = composite_atom(ATOM, symbol_succ, LT);

    //2
    _term* RLLLT = combine_terms(terms_X, terms_Y);
    _formula* RLLL  = composite_atom(ATOM, symbol_t, RLLLT);

    //3
    _formula* RLLR = replace_terms(copy_formula(originalFml), terms_Y, terms_Z);

    //4
    _formula* RLRL = replace_terms(copy_formula(originalFml), terms_Y, terms_Z);

    //5
    _term* RLRRT = combine_terms(terms_X, terms_Y);
    _formula* RLRR  = composite_atom(ATOM, symbol_t, RLRRT);

    //6
    _term* RRT = combine_terms(terms_X,terms_Z);
    _formula* RR  = composite_atom(ATOM, symbol_t, RRT);

    //create structure
    _formula* RLL = composite_bool(IMPL,RLLL,RLLR);
    _formula* RLR = composite_bool(IMPL,RLRL,RLRR);
    _formula* RL  = composite_bool(CONJ,RLL,RLR);
    _formula* R   = composite_bool(DISJ,RL,RR);
    _formula* F   = composite_bool(IMPL,L,R);
    return F;
}
