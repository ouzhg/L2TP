#include "Cabalar.h"

#include <stdlib.h>
#include <assert.h>

//#define DEBUG

#ifdef DEBUG
#define DBPRINTF(...) printf(__VA_ARGS__)
#else
#define DBPRINTF(...) 
#endif

_formula* Cabalar_DLT(_formula* originFml)//Disjunction, Left PRED_TRUE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == DISJ);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_TRUE);
	assert(originFml->subformula_r);
	
	_formula* f = originFml->subformula_l;
	
	//cleanup
	delete_formula(originFml->subformula_r);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_DLF(_formula* originFml)//Disjunction, Left PRED_FALSE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == DISJ);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_FALSE);
	assert(originFml->subformula_r);
	
	_formula* f = originFml->subformula_r;
	
	//cleanup
	free(originFml->subformula_l);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_CLT(_formula* originFml)//Conjunction, Left PRED_TRUE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == CONJ);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_TRUE);
	assert(originFml->subformula_r);
	
	_formula* f = originFml->subformula_r;
	
	//cleanup
	free(originFml->subformula_l);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_CLF(_formula* originFml)//Conjunction, Left PRED_FALSE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == CONJ);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_FALSE);
	assert(originFml->subformula_r);
	
	_formula* f = originFml->subformula_l;
	
	//cleanup
	delete_formula(originFml->subformula_r);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_ILT(_formula* originFml)//Implementation, Left PRED_TRUE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == IMPL);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_TRUE);
	assert(originFml->subformula_r);
	
	_formula* f = originFml->subformula_r;
	
	//cleanup
	free(originFml->subformula_l);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_ILF(_formula* originFml)//Implementation, Left PRED_FALSE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == IMPL);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->formula_type == ATOM);
	assert(originFml->subformula_l->predicate_id == PRED_FALSE);
	assert(originFml->subformula_r);
	
	_formula* f = composite_atom(ATOM,PRED_TRUE,NULL);
	
	//cleanup
	delete_formula(originFml);
	
	//output
	return f;
}

_formula* Cabalar_IRF(_formula* originFml)//Implementation, Right PRED_FALSE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == IMPL);
	assert(originFml->subformula_l);
	assert(originFml->subformula_r);
	assert(originFml->subformula_r->formula_type == ATOM);
	assert(originFml->subformula_r->predicate_id == PRED_FALSE);
	
	_formula* L = originFml->subformula_l;
	_formula* f = convert_negative_normal_form(composite_bool(NEGA,L,NULL));
	
	//cleanup
	free(originFml->subformula_r);
	free(originFml);
	
	//output
	return f;
}

_formula* Cabalar_IRT(_formula* originFml)//Implementation, Right PRED_TRUE
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->formula_type == IMPL);
	assert(originFml->subformula_l);
	assert(originFml->subformula_r);
	assert(originFml->subformula_r->formula_type == ATOM);
	assert(originFml->subformula_r->predicate_id == PRED_TRUE);

	_formula* f = composite_atom(ATOM,PRED_TRUE,NULL);
	
	//cleanup
	delete_formula(originFml);
	
	//output
	return f;
}

// Cabalar. (2005)
// Negation normal form N1
// -TRUE ----> FALSE
_formula* Cabalar_N1(_formula* originFml)
{
	assert(originFml);
	assert(originFml->subformula_l);
	
	_formula* f = composite_atom(ATOM, PRED_FALSE, NULL);
	
	//cleanup
	free(originFml->subformula_l);
	free(originFml);
	
	//output
    return f;
}

// Cabalar. (2005)
// Negation normal form N2
// -FALSE ----> TRUE
_formula* Cabalar_N2(_formula* originFml)
{
	assert(originFml);
	assert(originFml->subformula_l);
	
	_formula* f  = composite_atom(ATOM, PRED_TRUE, NULL);
	
	//cleanup
	free(originFml->subformula_l);
	free(originFml);
	
	//output
    return f;
}

// Cabalar. (2005)
// Negation normal form N3
// ---F ----> -F
_formula* Cabalar_N3(_formula* originFml)
{
    assert(originFml);//---F
    assert(originFml->subformula_l);//--F
    assert(originFml->subformula_l->subformula_l);//-F
    assert(originFml->subformula_l->subformula_l->subformula_l);//F

    _formula* _F = convert_negative_normal_form( originFml->subformula_l->subformula_l );

    //cleanup
    free(originFml->subformula_l);//--F
    free(originFml);//---F

    //output
    return _F;
}

// Cabalar. (2005)
// Negation normal form N4
// -(F ^ G) ----> -F v -G
_formula* Cabalar_N4(_formula* originFml)
{
    assert(originFml);//-(F ^ G)
    assert(originFml->subformula_l);//F ^ G
    assert(originFml->subformula_l->subformula_l);//F
    assert(originFml->subformula_l->subformula_r);//G

    _formula* F = originFml->subformula_l->subformula_l;
    _formula* G = originFml->subformula_l->subformula_r;

    //-F v -G
    _formula* _F = convert_negative_normal_form( composite_bool(NEGA, F, NULL) );
    _formula* _G = convert_negative_normal_form( composite_bool(NEGA, G, NULL) );
    _formula* f  = composite_bool(DISJ, _F, _G);

    //cleanup
    free(originFml->subformula_l);//F ^ G
    free(originFml);// -(F ^ G)

    //output
    return f;
}

// Cabalar. (2005)
// Negation normal form N5
// -(F v G) ----> -F ^ -G
_formula* Cabalar_N5(_formula* originFml)
{
    assert(originFml);//-(F v G)
    assert(originFml->subformula_l);//F v G
    assert(originFml->subformula_l->subformula_l);//F
    assert(originFml->subformula_l->subformula_r);//G

    _formula* F = originFml->subformula_l->subformula_l;
    _formula* G = originFml->subformula_l->subformula_r;

    //-F v -G
    _formula* _F = convert_negative_normal_form( composite_bool(NEGA, F, NULL) );
    _formula* _G = convert_negative_normal_form( composite_bool(NEGA, G, NULL) );
    _formula* f  = composite_bool(CONJ, _F, _G);

    //cleanup
    free(originFml->subformula_l);//F v G
    free(originFml);//-(F v G)

    //output
    return f;
}

// Cabalar. (2005)
// Negation normal form N6
// -(F -> G) ----> --F ^ -G
_formula* Cabalar_N6(_formula* originFml)
{
    assert(originFml);//-(F -> G)
    assert(originFml->subformula_l);//F -> G
    assert(originFml->subformula_l->subformula_l);//F
    assert(originFml->subformula_l->subformula_r);//G

    _formula* F = originFml->subformula_l->subformula_l;
    _formula* G = originFml->subformula_l->subformula_r;

    //-F v -G
    _formula* _F = convert_negative_normal_form( composite_bool(NEGA, F, NULL) );
    _formula* __F= convert_negative_normal_form( composite_bool(NEGA, _F, NULL) );
    _formula* _G = convert_negative_normal_form( composite_bool(NEGA, G, NULL) );
    _formula* f  = composite_bool(CONJ, __F, _G);

    //cleanup
    free(originFml->subformula_l);//F -> G
    free(originFml);//-(F -> G)

    //output
    return f;
}

// Cabalar. (2005)
// Left side rules L1
// TRUE ^ F -> G ----> {F->G}
_formulas* Cabalar_L1(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->subformula_l);//TRUE ^ F
	assert(originFml->subformula_l->subformula_l);//TRUE
	assert(originFml->subformula_l->subformula_r);//F
	
	_formula* F = originFml->subformula_l->subformula_r;
	_formula* G = originFml->subformula_r;
	
	//F->G
	_formula* f1 = composite_bool(IMPL, F, G);
	
	//cleanup
	free(originFml->subformula_l);//TRUE ^ F
	free(originFml);//TRUE ^ F -> G
	
	//output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    return fmls;
}

// Cabalar. (2005)
// Left side rules L2
// FALSE ^ F -> G ----> {NULL}
_formulas* Cabalar_L2(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->subformula_l);
	assert(originFml->subformula_l->subformula_l);
	assert(originFml->subformula_l->subformula_r);
    assert(originFml->subformula_r);
	
	_formula* f1 = composite_atom(ATOM,PRED_TRUE,0);
	
	//cleanup
	delete_formula(originFml);
	
	//output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    return fmls;
}

// Cabalar. (2005)
// Left side rules L3
// --F ^ G -> H ----> {G -> -F v H}
_formulas* Cabalar_L3(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);//--F ^ G
    assert(originFml->subformula_l->subformula_l);//--F
    assert(originFml->subformula_l->subformula_l->subformula_l);//-F
    assert(originFml->subformula_l->subformula_l->subformula_l->subformula_l);//F
    assert(originFml->subformula_l->subformula_r);//G
    assert(originFml->subformula_r);//H

    _formula* H = originFml->subformula_r;
    _formula* G = originFml->subformula_l->subformula_r;
    _formula* _F = originFml->subformula_l->subformula_l->subformula_l;

    //G -> -F v H
    _formula* f1_R = composite_bool(DISJ, _F, H);
    _formula* f1   = composite_bool(IMPL, G, f1_R);
	
    //cleanup
    free(originFml->subformula_l->subformula_l);//--F
    free(originFml->subformula_l);//--F ^ G
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    return fmls;
}

// Cabalar. (2005)
// Left side rules L4, with H has real value
// (F v G) ^ H -> K ----> { F ^ H -> K
//                          G ^ H -> K}
_formulas* Cabalar_L4(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_l->subformula_l);
    assert(originFml->subformula_l->subformula_l->subformula_l);
    assert(originFml->subformula_l->subformula_l->subformula_r);
    assert(originFml->subformula_l->subformula_r);
    assert(originFml->subformula_r);

    _formula* F = originFml->subformula_l->subformula_l->subformula_l;
    _formula* G = originFml->subformula_l->subformula_l->subformula_r;
    _formula* H = originFml->subformula_l->subformula_r;
    _formula* K = originFml->subformula_r;

    //F ^ H -> K
    _formula* f1_L = composite_bool(CONJ, F, H);
    _formula* f1   = composite_bool(IMPL, f1_L, K);

    //G ^ H -> K
    _formula* f2_L = composite_bool(CONJ, G, copy_formula(H));
    _formula* f2   = composite_bool(IMPL, f2_L, copy_formula(K));

    //cleanup
    free(originFml->subformula_l->subformula_l);
    free(originFml->subformula_l);
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    fmls = push_formulas(f2, fmls);
    return fmls;
}

// Cabalar. (2005)
// Left side rules L5, with H has real value.
// (F -> G) ^ H -> K  ----> { -F ^ H -> K
//                             G ^ H -> K
//                             H -> F v -G v K}
_formulas* Cabalar_L5(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);

    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_l->subformula_l);
    assert(originFml->subformula_l->subformula_l->subformula_l);
    assert(originFml->subformula_l->subformula_l->subformula_r);
    assert(originFml->subformula_l->subformula_r);
    assert(originFml->subformula_r);

    _formula* F = originFml->subformula_l->subformula_l->subformula_l;
    _formula* G = originFml->subformula_l->subformula_l->subformula_r;
    _formula* H = originFml->subformula_l->subformula_r;
    _formula* K = originFml->subformula_r;

    //-F ^ H -> K
    _formula* _F = convert_negative_normal_form(
						composite_bool(NEGA, copy_formula(F), NULL) 
				   );
    _formula* f1_L = composite_bool(CONJ, _F, copy_formula(H));
    _formula* f1   = composite_bool(IMPL, f1_L, copy_formula(K));

    //G ^ H -> K
    _formula* f2_L = composite_bool(CONJ, copy_formula(G), copy_formula(H));
    _formula* f2 = composite_bool(IMPL, f2_L, copy_formula(K));

    //H -> F v -G v K
    _formula* _G  = convert_negative_normal_form( composite_bool(NEGA, G, NULL) );
    _formula* f3_R_L = composite_bool(DISJ, F, _G);
    _formula* f3_R = composite_bool(DISJ, f3_R_L, K);
    _formula* f3   = composite_bool(IMPL, H, f3_R);

    //cleanup
    free(originFml->subformula_l->subformula_l);
    free(originFml->subformula_l);
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    fmls = push_formulas(f2, fmls);
    fmls = push_formulas(f3, fmls);
    return fmls;
}

_formulas* Cabalar_R1(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
	assert(originFml);
	assert(originFml->subformula_l);
	assert(originFml->subformula_r);
	assert(originFml->subformula_r->subformula_l);
	assert(originFml->subformula_r->subformula_r);
	
	_formula* F = originFml->subformula_l;
	_formula* G = originFml->subformula_r->subformula_r;
	
	_formula* f1 = composite_bool(IMPL,F,G);
	
	//cleanup
	free(originFml->subformula_r);
	free(originFml);
	
	_formulas* fmls = NULL;
	fmls = push_formulas(f1,fmls);
	return fmls;
}

_formulas* Cabalar_R2(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_r);
    assert(originFml->subformula_r->subformula_l);
    assert(originFml->subformula_r->subformula_r);

	_formula* f1 = composite_atom(ATOM,PRED_TRUE,0);
    //cleanup
    delete_formula(originFml);

	//output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    return fmls;
}

// Cabalar. (2005)
// Right side rules R3
// F -> --G v H ----> {-G ^ F -> H}
_formulas* Cabalar_R3(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_r);
    assert(originFml->subformula_r->subformula_r);
    assert(originFml->subformula_r->subformula_r->subformula_l);

    _formula* F = originFml->subformula_l;
    _formula* H = originFml->subformula_r->subformula_r;
    _formula* _G = originFml->subformula_r->subformula_l->subformula_l;

    //-G ^ F -> H
    _formula* f1_L = composite_bool(CONJ, _G, F);
    _formula* f1   = composite_bool(IMPL, f1_L, H);

    //cleanup
    free(originFml->subformula_r->subformula_l);
    free(originFml->subformula_r);
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    return fmls;
}

// Cabalar. (2005)
// Right side rules R4, with K has real value
// F -> (G ^ H) v K ----> { F -> G v K
//                          F -> H v K}
_formulas* Cabalar_R4(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_r);
    assert(originFml->subformula_r->subformula_l);
    assert(originFml->subformula_r->subformula_l->subformula_l);
    assert(originFml->subformula_r->subformula_l->subformula_r);
    assert(originFml->subformula_r->subformula_r);

    _formula* F = originFml->subformula_l;
    _formula* G = originFml->subformula_r->subformula_l->subformula_l;
    _formula* H = originFml->subformula_r->subformula_l->subformula_r;
    _formula* K = originFml->subformula_r->subformula_r;

    //F -> G v K
    _formula* f1_R = composite_bool(DISJ, G, K);
    _formula* f1   = composite_bool(IMPL, F, f1_R);

    //F -> H v K
    _formula* f2_R = composite_bool(DISJ, H, copy_formula(K));
    _formula* f2   = composite_bool(IMPL, copy_formula(F), f2_R);

    //cleanup
    free(originFml->subformula_r->subformula_l);
    free(originFml->subformula_r);
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    fmls = push_formulas(f2, fmls);
    return fmls;
}

// Cabalar. (2005)
// Right side rules R5, with K has real formula.
// F -> (G -> H) v K  ----> { G ^ F ->  H v K
//                           -H ^ F -> -G v K}
_formulas* Cabalar_R5(_formula* originFml)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(originFml);
    assert(originFml->subformula_l);
    assert(originFml->subformula_r);
    assert(originFml->subformula_r->subformula_l);
    assert(originFml->subformula_r->subformula_l->subformula_l);
    assert(originFml->subformula_r->subformula_l->subformula_r);
    assert(originFml->subformula_r->subformula_r);

    _formula* F = originFml->subformula_l;
    _formula* G = originFml->subformula_r->subformula_l->subformula_l;
    _formula* H = originFml->subformula_r->subformula_l->subformula_r;
    _formula* K = originFml->subformula_r->subformula_r;

    //G ^ F ->  H v K
    _formula* f1_L = composite_bool(CONJ, G, F);
    _formula* f1_R = composite_bool(DISJ, H, K);
    _formula* f1   = composite_bool(IMPL, f1_L, f1_R);

    //-H ^ F -> -G v K
    _formula* _H  = convert_negative_normal_form( 
						composite_bool(NEGA, copy_formula(H), NULL) 
					);
    _formula* _G  = convert_negative_normal_form(
						composite_bool(NEGA, copy_formula(G), NULL)
					);
    _formula* f2_L = composite_bool(CONJ, _H, copy_formula(F));
    _formula* f2_R = composite_bool(DISJ, _G, copy_formula(K));
    _formula* f2   = composite_bool(IMPL, f2_L, f2_R);

    //cleanup
    free(originFml->subformula_r->subformula_l);
    free(originFml->subformula_r);
    free(originFml);

    //output
    _formulas* fmls = NULL;
    fmls = push_formulas(f1, fmls);
    fmls = push_formulas(f2, fmls);
    return fmls;
}

//return value: 0 - no need to swap, 1- need swap, 2- swap finish
int 
Commutative_Trans(_formula* root, _formula* curr,
					FORMULA_TYPE commutativeSymbol)
{	
// find sub-formula different from (input)>commutativeSymbol,
// and swap it to the left side of (input)>root.
// example:
//              CONJ                               CONJ
//       ------------------                   ----------------
//       |                |                   |              |
//    ----------         CONJ2     ====>     DISJ       -----------
//  DISJ      CONJ1                                   CONJ2      CONJ1

	assert(root);
	assert(curr);
	assert(commutativeSymbol == DISJ || commutativeSymbol == CONJ);
	
	static bool isLeftSide;
	int value_l;
	int value_r;
	_formula* temp;
	
	// terminals
	if(curr->formula_type != commutativeSymbol)
	{
		if(curr->formula_type == ATOM &&
			curr->predicate_id != PRED_TRUE && curr->predicate_id != PRED_FALSE)
		{
			return 0;//normal predicates
		}
		if(curr->formula_type == NEGA &&
				curr->subformula_l->formula_type != NEGA)
		{
			return 0;//normal predicates
		}
		if(curr->formula_type == UNIV || curr->formula_type == EXIS)
		{
			return 0;//normal predicates
		}
		return 1;// predicates need to swap
	}
	
	// left sub-formula
	if(curr == root) { isLeftSide = true; }
	value_l = Commutative_Trans(root,curr->subformula_l,commutativeSymbol);
	
	if(value_l == 2) { return 2; }
	if(value_l == 1)
	{
		if(isLeftSide)
		{
			temp = curr->subformula_l;
			curr->subformula_l = root->subformula_r;

			root->subformula_r = root->subformula_l;
			root->subformula_l = temp;
		}
		else
		{
			temp = curr->subformula_l;
			curr->subformula_l = root->subformula_l;

			root->subformula_l = temp;
		}
		return 2;
	}
	
	// right sub-formula
	if(curr == root) { isLeftSide = false; }
	value_r = Commutative_Trans(root,curr->subformula_r,commutativeSymbol);
	
	if(value_r == 2) { return 2; }
	if(value_r == 1)
	{
		if(isLeftSide)
		{
			temp = curr->subformula_r;
			curr->subformula_r = root->subformula_r;

			root->subformula_r = root->subformula_l;
			root->subformula_l = temp;
		}
		else
		{
			temp = curr->subformula_r;
			curr->subformula_r = root->subformula_l;

			root->subformula_l = temp;
		}
		return 2;
	}
	
	return 0;
}

_formulas* Cabalar_Trans(_formula* fml)
{
    assert(fml);

    // Cabalar. (2005) Left/Right side rules
    if(fml->formula_type == IMPL)
    {
        _formula* subformula_l = fml->subformula_l;
        _formula* subformula_r = fml->subformula_r;

		Commutative_Trans(subformula_l,subformula_l,CONJ);
		Commutative_Trans(subformula_r,subformula_r,DISJ);
		
		_formula* CD_l = subformula_l->subformula_l;
		_formula* CD_r = subformula_r->subformula_l;
        
        if(subformula_l->formula_type == CONJ)
        {
            if(CD_l->formula_type == ATOM)
            {
                //L1
                if(CD_l->predicate_id == PRED_TRUE)
                {
                    return Cabalar_L1(fml);
                }
                //L2
                if(CD_l->predicate_id == PRED_FALSE)
                {
                    return Cabalar_L2(fml);
                }
            }
			//L3
			if(CD_l->formula_type == NEGA && 
					CD_l->subformula_l->formula_type == NEGA)
			{
				return Cabalar_L3(fml);
			}
            //L4
            if(CD_l->formula_type == DISJ)
            {
                return Cabalar_L4(fml);
            }
            //L5
            if(CD_l->formula_type == IMPL)
            {
                return Cabalar_L5(fml);
            }
        }//end of Cabalar Left Translation (full pattern)
		
        // Cabalar. (2005) Right side rules
        if(subformula_r->formula_type == DISJ)
        {
            if(CD_r->formula_type == ATOM)
            {
                //R1
                if(CD_r->predicate_id == PRED_FALSE)
                {
                    return Cabalar_R1(fml);
                }
                //R2
                if(CD_r->predicate_id == PRED_TRUE)
                {
                    return Cabalar_R2(fml);
                }
            }
			//R3
			if(CD_r->formula_type == NEGA &&
					CD_r->subformula_l->formula_type == NEGA)
			{
				return Cabalar_R3(fml);
			}
            //R4
            if(CD_r->formula_type == CONJ)
            {
                return Cabalar_R4(fml);
            }
            //R5
            if(CD_r->formula_type == IMPL)
            {
                return Cabalar_R5(fml);
            }
        }//end of Cabalar Right Translation(full pattern)

		
		//start of Commutative missing Translation
		if(subformula_l->formula_type == ATOM)
		{
		//L1 TRUE->G ------> G
			if(subformula_l->predicate_id == PRED_TRUE)
			{
				//return push_formulas(Cabalar_ILT(fml),NULL); 
				//// convert_rules() does this.
				//// may also product problem like Cabalar_IRF()
			}
		//L2 FALSE->G ------> ALWAYS BE TRUE
			if(subformula_l->predicate_id == PRED_FALSE)
			{
				return push_formulas(Cabalar_ILF(fml),NULL);
			}
		}
		//L3
		if(subformula_l->formula_type == NEGA &&
				subformula_l->subformula_l->formula_type == NEGA)
		{
			fml->subformula_l = composite_bool(CONJ,subformula_l,NULL);
			fml->subformula_l
					->subformula_r = composite_atom(ATOM,PRED_TRUE,NULL);
			return Cabalar_L3(fml);
		}
        //L4
        if(subformula_l->formula_type == DISJ)
        {
			fml->subformula_l = composite_bool(CONJ,subformula_l,NULL);
			fml->subformula_l
					->subformula_r = composite_atom(ATOM,PRED_TRUE,NULL);
            return Cabalar_L4(fml);
        }
        //L5
        if(subformula_l->formula_type == IMPL)
        {
			fml->subformula_l = composite_bool(CONJ,subformula_l,NULL);
			fml->subformula_l
					->subformula_r = composite_atom(ATOM,PRED_TRUE,NULL);
            return Cabalar_L5(fml);
        }
		
		//R1
		if(subformula_r->formula_type == ATOM)
		{
		//R1 F->FALSE -------->~F
			if(subformula_r->predicate_id == PRED_FALSE)
			{
				//return push_formulas(Cabalar_IRF(fml),NULL); 
				//// convert_rules() does this. 
				//// also leads s to ~~s.
			}
		//R2 F->TRUE --------->ALWAYS BE TRUE
			if(subformula_r->predicate_id == PRED_TRUE)
			{
				return push_formulas(Cabalar_IRT(fml),NULL);
			}
		}
		//R3
		if(subformula_r->formula_type == NEGA &&
				subformula_r->subformula_l->formula_type == NEGA)
		{
			fml->subformula_r = composite_bool(DISJ,subformula_r,NULL);
			fml->subformula_r
					->subformula_r = composite_atom(ATOM,PRED_FALSE,NULL);
			return Cabalar_R3(fml);
		}
        //R4
        if(subformula_r->formula_type == CONJ)
        {
			fml->subformula_r = composite_bool(DISJ,subformula_r,NULL);
			fml->subformula_r
					->subformula_r = composite_atom(ATOM,PRED_FALSE,NULL);
            return Cabalar_R4(fml);
        }
        //R5
        if(subformula_r->formula_type == IMPL)
        {
			fml->subformula_r = composite_bool(DISJ,subformula_r,NULL);
			fml->subformula_r
					->subformula_r = composite_atom(ATOM,PRED_FALSE,NULL);
            return Cabalar_R5(fml);
        }
	}
	
	//start of imply missing
	if(fml->formula_type == CONJ)
	{
		Commutative_Trans(fml,fml, CONJ);
		_formula* CD_o = fml->subformula_l;
		
		//L1
		if(CD_o->formula_type == ATOM && 
				CD_o->predicate_id == PRED_TRUE)
		{
			return push_formulas(Cabalar_CLT(fml),NULL);
		}
		//L2
		if(CD_o->formula_type == ATOM && 
				CD_o->predicate_id == PRED_FALSE)
		{
			return push_formulas(Cabalar_CLF(fml),NULL);
		}
		//Note: L3,L4,L5 lead to recursive translation
	}
	
	if(fml->formula_type == DISJ)
	{
        Commutative_Trans(fml,fml, DISJ);
		_formula* CD_o = fml->subformula_l;
		
		//R1
		if(CD_o->formula_type == ATOM &&
				CD_o->predicate_id == PRED_FALSE)
		{
			return push_formulas(Cabalar_DLF(fml),NULL);
		}
		//R2
		if(CD_o->formula_type == ATOM &&
				CD_o->predicate_id == PRED_TRUE)
		{
			return push_formulas(Cabalar_DLT(fml),NULL);
		}
		//R3
		if(CD_o->formula_type == NEGA && 
				CD_o->subformula_l->formula_type == NEGA)
		{
			fml = composite_bool(IMPL,NULL,fml);
			fml->subformula_l = composite_atom(ATOM,PRED_TRUE,NULL);
			return Cabalar_R3(fml);
		}
		//R4
		if(CD_o->formula_type == CONJ)
		{
			fml = composite_bool(IMPL,NULL,fml);
			fml->subformula_l = composite_atom(ATOM,PRED_TRUE,NULL);
			return Cabalar_R4(fml);
		}
		//R5
		if(CD_o->formula_type == IMPL)
		{
			fml = composite_bool(IMPL,NULL,fml);
			fml->subformula_l = composite_atom(ATOM,PRED_TRUE,NULL);
			return Cabalar_R5(fml);
		}
	}//end of fml->formula_type == DISJ
	
	DBPRINTF("FINAL\n");

    return NULL;//No rules in Cabalar. (2005) fix this formula, end.
}

_formula* convert_negative_normal_form(_formula* fml)
{
    assert(fml);
	assert(fml->formula_type == ATOM || fml->formula_type == NEGA ||
		   fml->formula_type == DISJ || fml->formula_type == CONJ ||
		   fml->formula_type == IMPL);

    //Cabalar. (2005)
	if(fml->formula_type == ATOM)
	{
		//do nothing
	}
	else if(fml->formula_type == NEGA)
    {
        _formula* subformula_l = fml->subformula_l;

        if(subformula_l->formula_type == ATOM)
        {
            //N1
            if(subformula_l->predicate_id == PRED_TRUE)
            {
                return Cabalar_N1(fml);
            }
            //N2
            if(subformula_l->predicate_id == PRED_FALSE)
            {
                return Cabalar_N2(fml);
            }
        }
        if(subformula_l->formula_type == NEGA)
        {
            //N3
            if(subformula_l->subformula_l->formula_type == NEGA)
            {
                return Cabalar_N3(fml);
            }
        }
        //N4
        if(subformula_l->formula_type == CONJ)
        {
            return Cabalar_N4(fml);
        }
        //N5
        if(subformula_l->formula_type == DISJ)
        {
            return Cabalar_N5(fml);
        }
        //N6
        if(subformula_l->formula_type == IMPL)
        {
            return Cabalar_N6(fml);
        }
    }
	else//if(fml->formula_type == CONJ || 
		//   fml->formula_type == DISJ ||
		//   fml->formula_type == IMPL)
	{
		fml->subformula_l = convert_negative_normal_form(fml->subformula_l);
		fml->subformula_r = convert_negative_normal_form(fml->subformula_r);
	}
    
    return fml;
}

_formulas* convert_negative_normal_forms(_formulas* fmls)
{
    _formulas* currFml = fmls;
    while(currFml != NULL)
    {
        currFml->curr_formula = convert_negative_normal_form(currFml->curr_formula);
        currFml = currFml->remained_formulas;
    }
    return fmls;
}

_formulas* convert_Cabalar(_formulas* fmls)
{
	DBPRINTF("%s\n",__FUNCTION__);
	
    assert(fmls);

    _formulas* transFmls = NULL;
    _formulas* finalFmls = NULL;
    _formula* fml = NULL;

    fmls = convert_negative_normal_forms(fmls);
	
    while(fmls != NULL)
    {
        fml = top_formulas(fmls);
        fmls = pop_formulas(fmls);//pop out the head of the formulas
		
#ifdef DEBUG
		output_formula(stdout,fml);
		printf("\n");
#endif

        transFmls = Cabalar_Trans(fml);
        if(transFmls == NULL)// no translation
        {
			finalFmls = push_formulas(fml,finalFmls);
		}
        else
		{
			fmls = join_formulas(transFmls, fmls);
        }
    }

    return finalFmls;
}
