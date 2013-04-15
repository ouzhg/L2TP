#include "utility.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//dictionary
///////////////////////////////////////////////////////////////////////////////
void init_vocabulary ( void )
{
    memset ( &vocabulary, 0, sizeof ( vocabulary ) );
}

void destroy_vocabulary ( void )
{
    int i;

    for ( i=0; i<vocabulary.num_variable; i++ )
    {
        free ( vocabulary.names_variable[i] );
    }
    for ( i=0; i<vocabulary.num_function; i++ )
    {
        free ( vocabulary.names_function[i] );
    }
    for ( i=0; i<vocabulary.num_predicate; i++ )
    {
        free ( vocabulary.names_predicate[i] );
    }
}

bool in_list ( const int* list, int len, int obj )
{
    for ( len--; len>=0; len-- )
        if ( list[len]==obj ) return true;
    return false;
}

bool is_negative ( const _formula* phi, 
					 const int* sm_preds, int num_sp, bool negative )
{
    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        if ( negative || !in_list ( sm_preds, num_sp, phi->predicate_id ) )
            return true;
        break;
    case NEGA:
        assert ( phi->subformula_l );
        return is_negative ( phi->subformula_l, sm_preds, num_sp, !negative );
    case CONJ:
    case DISJ:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (is_negative ( phi->subformula_l, sm_preds, num_sp, negative ) &&
               is_negative ( phi->subformula_r, sm_preds, num_sp, negative ));
    case IMPL:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (is_negative( phi->subformula_l, sm_preds, num_sp, !negative ) &&
               is_negative ( phi->subformula_r, sm_preds, num_sp, negative ));
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        return is_negative ( phi->subformula_l, sm_preds, num_sp, negative );
    default:
        assert ( 0 );
    }

    return false;
}

bool is_universal(_formula* fml)
{
	assert(fml);
	
	switch(fml->formula_type)
	{
		case ATOM:
			return true;
		case CONJ:
		case DISJ:
		case IMPL:
			return is_universal(fml->subformula_l) && is_universal(fml->subformula_r);
		case NEGA:
		case UNIV:
			return is_universal(fml->subformula_l);
		case EXIS:
			return false;
		default:
			assert(0);
	}
	
	return false;
}

// symbol operations
////////////////////////////////////////////////////////////////////////////////
int add_symbol ( const char* name, SYMBOL_TYPE type, int arity )
{
	// return value: 1. >=0 - id; 2. -1 - too many symbols; 3. -2 - type error
    char* s;

    assert ( name );

    s = (char*)malloc( (strlen(name) + 1) *sizeof(char) );
    strcpy (s, name);

    switch (type)
    {
    case VARIABLE:
        if ( vocabulary.num_variable>=MAX_NUM_VARIABLE ) break;
        vocabulary.names_variable[vocabulary.num_variable] = s;
        return ( vocabulary.num_variable ) ++;
    case FUNCTION:
        if ( vocabulary.num_variable>=MAX_NUM_FUNCTION ) break;
        vocabulary.names_function[vocabulary.num_function] = s;
        vocabulary.arities_function[vocabulary.num_function] = arity;
        return ( vocabulary.num_function ) ++;
    case PREDICATE:
        if ( vocabulary.num_variable>=MAX_NUM_PREDICATE ) break;
        vocabulary.names_predicate[vocabulary.num_predicate] = s;
        vocabulary.arities_predicate[vocabulary.num_predicate] = arity;
        return ( vocabulary.num_predicate ) ++;
    default:
        assert ( 0 );
        return -2;
    }

    return -1;
}

int query_symbol ( const char* name, SYMBOL_TYPE type )
{
	//return value: 1. >=0 id; 2. -1 no such symbol; 3. -2 type error
    int i, n;
    char **ps;

    assert(name);

    switch(type)
    {
    case VARIABLE:
        n = vocabulary.num_variable;
        ps = vocabulary.names_variable;
        break;
    case FUNCTION:
        n = vocabulary.num_function;
        ps = vocabulary.names_function;
        break;
    case PREDICATE:
        n = vocabulary.num_predicate;
        ps = vocabulary.names_predicate;
        break;
    default:
        assert ( 0 );
        return -2;
    }

    for(i=0; i<n; i++)
    {
        if ( 0 == strcmp(name, ps[i]) )
		{
            return i;
		}
    }

    return -1;
}

int predicate_arity(int id)
{
    if (id < 0)
    {
        return spec_pred_arities[-id-1];
    }
    else
    {
        assert ( vocabulary.arities_predicate );
        assert ( id<vocabulary.num_predicate );
        return vocabulary.arities_predicate[id];
    }
}

int function_arity ( int id )
{
    assert ( vocabulary.arities_function );
    assert ( id>=0&&id<vocabulary.num_function );
    return vocabulary.arities_function[id];
}

char* query_name ( int id, SYMBOL_TYPE type )
{
    switch ( type )
    {
    case VARIABLE:
        assert ( 0<=id&&id<vocabulary.num_variable );
        assert ( vocabulary.names_variable[id] );
        return vocabulary.names_variable[id];

    case FUNCTION:
        assert ( 0<=id&&id<vocabulary.num_function );
        assert ( vocabulary.names_function[id] );
        return vocabulary.names_function[id];

    case PREDICATE:
        assert ( 0<=id&&id<vocabulary.num_predicate );
        assert ( vocabulary.names_predicate[id] );
        return vocabulary.names_predicate[id];

    default:
        assert ( 0 );
    }

    return NULL;
}

//terms operations
///////////////////////////////////////////////////////////////////////////////
_term* combine_terms(const _term* head, int head_size, 
					   const _term* tail, int tail_size)
{
	_term* terms = (_term*)malloc( sizeof(_term) * (head_size+tail_size) );
	
	memcpy(terms, head, sizeof(_term)*head_size);
	memcpy(terms+sizeof(_term)*head_size, tail, sizeof(_term)*tail_size);
	
	return terms;
}

_term* combine_terms(const std::vector<int>& head,
					   const std::vector<int>& tail)
{
    _term* terms = (_term*)malloc(sizeof(_term) * (head.size()+tail.size()));

    int 
	index = 0;
    for(std::vector<int>::const_iterator it = head.begin();
		it != head.end(); it++, index++)
    {
        terms[index].variable_id = head[index];
        terms[index].term_type = VARI;
    }
	index = 0;
    for(std::vector<int>::const_iterator it = tail.begin(); 
		it != tail.end(); it++, index++)
    {
        terms[index+head.size()].variable_id = tail[index];
        terms[index+head.size()].term_type = VARI;
    }
	
	return terms;
}

_term* copy_terms(const _term* terms, int size)//deep copy
{
    _term* newTerms = (_term*)malloc(sizeof(_term)*size);
    assert(newTerms);

    memcpy(newTerms, terms, sizeof(_term)*size);

    for(int i = 0; i < size; i++)
    {
        if(newTerms[i].term_type == FUNC)
        {
            newTerms[i].parameters = copy_terms(newTerms[i].parameters,
									   function_arity(newTerms[i].function_id));
        }
    }

    return newTerms;
}

void delete_terms(_term* terms, int size)//deep delete
{
    for(int i = 0; i < size; i++)
    {
        if(terms[i].term_type == FUNC)
        {
            delete_terms(terms[i].parameters,
										function_arity(terms[i].function_id));
        }
    }
    free(terms);
}

bool compare_term(const _term* t, const _term* s)
{
    int k;

    assert(t);
    assert(s);

    if (t->term_type!=s->term_type) return false;

    switch (t->term_type)
    {
    case VARI:
        return (t->variable_id==s->variable_id);
    case FUNC:
        if (t->function_id==s->function_id)
        {
            k = function_arity(t->function_id) - 1;
            assert(k<0||t->parameters);
            assert(k<0||s->parameters);
            for ( ; k>=0; k--)
            {
                if (!compare_term(t->parameters+k,s->parameters+k))
                    return false;
            }
            return true;
        }
    default:
        assert(0);
    }
    return false;
}

bool find_var_term(const _term* t, int var_id)
{
    int i;

    assert(t);

    switch(t->term_type)
    {
    case VARI:
        return (t->variable_id==var_id);
    case FUNC:
        assert(t->parameters);
        for(i = 0; i < function_arity(t->function_id); i++)
        {
            if ( find_var_term(t->parameters+i, var_id) )
                return true;
        }
        break;
    default:
        assert(0);
    }
    return false;
}

void rename_var_term(_term* t, int oldv, int newv)
{
    int i;
    assert(t);

    switch(t->term_type)
    {
    case VARI:
        if (t->variable_id == oldv)
		{
			t->variable_id = newv;
		}
        break;
    case FUNC:
        assert (t->parameters);
        for(i = 0; i < function_arity(t->function_id); i++)
        {
            rename_var_term(t->parameters+i, oldv, newv);
        }
        break;
    default:
        assert(0);
    }
}

_term* replace_term(_term* terms, int arity, 
			const std::vector<int>& exis, const std::vector<int>& replacements)
{
    for(int i = 0; i < arity; i++)
    {
        //replacement
        if(terms[i].term_type == VARI)
        {
            for(int j = 0; j < exis.size(); j++)
            {
                if(terms[i].variable_id == exis[j])
                {
                    terms[i].variable_id = replacements[j];
                }
            }
        }
        //traversal
        else if(terms[i].term_type == FUNC)
        {
            replace_term(terms[i].parameters, 
					function_arity(terms[i].function_id), exis, replacements);
        }
    }
    return terms;
}

void output_term ( FILE* out, const _term* t )
{
    int i, k;

    assert (t);

    if ( VARI==t->term_type )
    {
        fprintf ( out, "%s", query_name ( t->variable_id, VARIABLE ) );
    }
    else
    {
        fprintf ( out, "%s", query_name ( t->function_id, FUNCTION ) );
        k = function_arity ( t->function_id );
        if ( k > 0 )
        {
            fprintf ( out, "(" );
            for ( i=0; i<k; i++ )
            {
                if ( 0 < i ) printf ( ", " );
                output_term ( out, t->parameters+i );
            }
            fprintf ( out, ")" );
        }
    }
}
//formula operations
///////////////////////////////////////////////////////////////////////////////
_formula* composite_atom(FORMULA_TYPE formula_type,
									const int predicate_id, _term* parameters)
{
    assert(formula_type == ATOM);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->predicate_id = predicate_id;
    fml->parameters   = parameters;
    return fml;
}

_formula* composite_bool(FORMULA_TYPE formula_type, 
								_formula* subformula_l, _formula* subformula_r)
{
    assert(formula_type == NEGA || formula_type == DISJ ||
		   formula_type == CONJ || formula_type == IMPL);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->subformula_l = subformula_l;
    fml->subformula_r = subformula_r;
    return fml;
}

_formula* composite_qntf(FORMULA_TYPE formula_type,
								_formula* subformula_l, const int variable_id)
{
	assert(formula_type == EXIS || formula_type == UNIV);

	_formula* fml = (_formula*)malloc(sizeof(_formula));
	assert(fml);

	fml->formula_type = formula_type;
	fml->subformula_l = subformula_l;
	fml->variable_id = variable_id;
	return fml;
}
//------------------------------------------------------------------------------
_formula* copy_formula (const _formula *fml)
{
    assert (fml);
	
    _formula* newFormula = (_formula*)malloc(sizeof(_formula));
    assert (newFormula);

    memcpy(newFormula, fml, sizeof (_formula));
    switch (fml->formula_type)
    {
    case ATOM:
        newFormula->parameters = copy_terms(fml->parameters, 
											predicate_arity(fml->predicate_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( fml->subformula_r );
        newFormula->subformula_r = copy_formula ( fml->subformula_r );
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( fml->subformula_l );
        newFormula->subformula_l = copy_formula ( fml->subformula_l );
        break;
    default:
        assert (0);
    }

    return newFormula;
}

bool compare_formula(const _formula* phi, const _formula* psi)
{
    int k;

    assert(phi);
    assert(psi);

    if (phi->formula_type!=psi->formula_type) return false;

    switch (phi->formula_type)
    {
    case ATOM:
        if (phi->predicate_id!=psi->predicate_id) return false;
        k = predicate_arity(phi->predicate_id) - 1;
        assert(k<0||phi->parameters);
        assert(k<0||psi->parameters);
        for ( ; k>=0; k--)
        {
            if (!compare_term(phi->parameters+k,psi->parameters+k))
                return FALSE;
        }
        return true;
    case UNIV:
    case EXIS:
        if (phi->variable_id!=psi->variable_id) return false;
    case NEGA:
        assert(phi->subformula_l);
        assert(psi->subformula_l);
        return compare_formula(phi->subformula_l,psi->subformula_l);
    case CONJ:
    case DISJ:
    case IMPL:
        assert(phi->subformula_l);
        assert(psi->subformula_l);
        assert(phi->subformula_r);
        assert(psi->subformula_r);
        return (compare_formula(phi->subformula_l,psi->subformula_l)
            && compare_formula(phi->subformula_r,psi->subformula_r));
    default:
        assert(0);
    }
    return false;
}

void delete_formula ( _formula* fml )
{
    assert ( fml );

    switch ( fml->formula_type )
    {
    case ATOM:
        if(fml->parameters)
			delete_terms(fml->parameters, predicate_arity(fml->variable_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( fml->subformula_r );
        delete_formula(fml->subformula_r);
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( fml->subformula_l );
        delete_formula(fml->subformula_l);
        break;
    default:
        assert ( 0 );
    }
    free(fml);
}

bool find_var_formula ( const _formula* phi, int var_id )
{
    int i;

    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        for ( i=0; i<predicate_arity ( phi->predicate_id ); i++ )
        {
            assert ( phi->parameters );
            if ( find_var_term ( phi->parameters+i, var_id ) )
                return true;
        }
        return false;
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        return find_var_formula ( phi->subformula_l, var_id );
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (find_var_formula ( phi->subformula_l, var_id )
               && find_var_formula ( phi->subformula_r, var_id ));
    default:
        assert ( 0 );
    }
    return false;
}



void rename_var_formula ( _formula* phi, int oldv, int newv )
{
    int i;
    assert ( phi );
	
	if(oldv == newv)
		return;

    switch ( phi->formula_type )
    {
    case ATOM:
        assert ( phi->parameters );
        for ( i=0; i<predicate_arity ( phi->predicate_id ); i++ )
        {
            rename_var_term ( phi->parameters+i, oldv, newv );
        }
        break;
    case UNIV:
    case EXIS:
        if ( phi->variable_id==oldv ) phi->variable_id = newv;
    case NEGA:
        assert ( phi->subformula_l );
        rename_var_formula ( phi->subformula_l, oldv, newv );
        break;
    default:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        rename_var_formula ( phi->subformula_l, oldv, newv );
        rename_var_formula ( phi->subformula_r, oldv, newv );
    }
}

_formula* replace_terms(_formula* fml, 
			const std::vector<int>& exis, const std::vector<int>& replacements)
{
    assert(fml);
    assert(exis.size() == replacements.size());

    switch(fml->formula_type)
    {
    case ATOM:
        replace_term(fml->parameters, predicate_arity(fml->predicate_id), 
					exis, replacements);
        break;
    case DISJ:
    case CONJ:
    case IMPL:
        replace_terms(fml->subformula_r, exis, replacements);
    case NEGA:
    case UNIV:
    case EXIS:
        replace_terms(fml->subformula_l, exis, replacements);
        break;
    default:
        assert(0);
    }
	
    return fml;//formula after replace_terms, formula do not change here.
}

void output_formula (FILE* out, const _formula* phi)
{
    char* s_conn = NULL;
    int i;

    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        if ( phi->predicate_id>=0 )
        {
            fprintf ( out, "%s(", query_name ( phi->predicate_id, PREDICATE ) );
            for ( i=0; i<predicate_arity ( phi->predicate_id ); i++ )
            {
                if ( i > 0 ) fprintf ( out, ", " );
                output_term ( out, phi->parameters+i );
            }
        }
        else
        {
            switch ( phi->predicate_id )
            {
			case PRED_TRUE:
				fprintf( out, "(TRUE");
				break;
			case PRED_FALSE:
				fprintf( out, "(FALSE");
				break;
            case PRED_EQUALITY:
                fprintf ( out, "(" );
                assert ( phi->parameters );
                output_term ( out, phi->parameters );
                fprintf ( out, " = " );
                output_term ( out, phi->parameters+1 );
                break;
			case PRED_MIN:
				fprintf( out, "MIN(");
				assert( phi->parameters );
				output_term( out, phi->parameters );
				break;
			case PRED_MAX:
				fprintf( out, "MAX(");
				assert( phi->parameters );
				output_term( out, phi->parameters );
				break;
			case PRED_SUCC:
                fprintf ( out, "SUCC(" );
                assert ( phi->parameters );
                output_term ( out, phi->parameters );
                fprintf ( out, "," );
                output_term ( out, phi->parameters+1 );
				break;
            default:
                assert ( 0 );
            }
        }
        fprintf ( out, ")" );
        break;
    case NEGA:
        fprintf ( out, "~ (" );
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, ")" );
        break;
    case CONJ:
        s_conn = (char*)" & ";
    case DISJ:
        if ( NULL==s_conn ) s_conn = (char*)" | ";
    case IMPL:
        if ( NULL==s_conn ) s_conn = (char*)" -> ";
        fprintf ( out, "(" );
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, "%s", s_conn );
        assert ( phi->subformula_r );
        output_formula ( out, phi->subformula_r );
        fprintf ( out, ")" );
        break;
    case UNIV:
        s_conn = (char*)"!";
    case EXIS:
        if ( NULL==s_conn ) s_conn = (char*)"?";
        fprintf(out, "[%s %s] (", s_conn, query_name(phi->variable_id, VARIABLE));
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, ")" );
        break;
    default:
        assert ( 0 );
    }
}

//formulas operations
///////////////////////////////////////////////////////////////////////////////
_formulas* join_formulas(_formulas* head, _formulas* tail)
{
    _formulas* currNode = head;
    while(currNode->remained_formulas != NULL) 
    {
        currNode = currNode->remained_formulas;
    }
    currNode->remained_formulas = tail;
    return head;
}

_formulas* push_formulas(_formula* fml, _formulas* fmls)
{
    assert(fml);

    _formulas* newFmls = (_formulas*)malloc(sizeof(_formulas));
    assert(newFmls);

    newFmls->curr_formula = fml;
    newFmls->remained_formulas = fmls;
    return newFmls;
}

_formulas* pop_formulas(_formulas* fmls)//only remove the formulas node structure
{
    assert(fmls);

    _formulas* newFmls = fmls->remained_formulas;
    free(fmls);

    return newFmls;
}

_formula* top_formulas(_formulas* fmls)
{
    assert(fmls);

    return fmls->curr_formula;
}

_formulas* copy_formulas ( const _formulas* originFormulas )//deep copy
{
	_formulas *p = NULL;
	
	if(originFormulas != NULL)
	{
		p = (_formulas*)malloc(sizeof(_formulas));
		p->curr_formula = originFormulas->curr_formula;
		p->remained_formulas = copy_formulas(originFormulas->remained_formulas);//recurse
	}
	
    return p;
}

void delete_formulas ( _formulas* fmls )//deep delete
{
    _formulas *p;
    while ( fmls )
    {
        p = fmls->remained_formulas;
        delete_formula(fmls->curr_formula);
        free ( fmls );
        fmls = p;
    }
}

int size_formulas(_formulas* fmls)
{
    int size = 0;
    while(fmls != NULL)
    {
        size++;
        fmls = fmls->remained_formulas;
    }

    return size;
}

void output_formulas(FILE* out, const _formulas* fmls)
{
	while(fmls)
	{
		output_formula(out,fmls->curr_formula);
		fprintf(out,".\n");
		fmls = fmls->remained_formulas;
	}
}

//other
///////////////////////////////////////////////////////////////////////////////

void output_extraDefinition(FILE* out)//@TODO
{
	int i;
	for(i = 0; i < vocabulary.num_predicate; i++)
	{
		fprintf(out,"_%s :- not %s\n",vocabulary.names_predicate[i],
				vocabulary.names_predicate[i]);
	}
}

_formula* double_negation(_formula* phi, const int* int_preds, int num_ip)
{
    assert(phi);

    if (is_negative(phi, int_preds, num_ip, false))
    {
        return phi;
    }

    switch ( phi->formula_type )
    {
    case ATOM:
        if(in_list(int_preds, num_ip, phi->predicate_id))
        {
            phi = composite_bool ( NEGA, phi, NULL );
            phi = composite_bool ( NEGA, phi, NULL );
        }
        break;
    case CONJ:
    case DISJ:
    case IMPL:
		phi->subformula_r = double_negation(phi->subformula_r,int_preds,num_ip);
    case NEGA:
    case UNIV:
    case EXIS:
		phi->subformula_l = double_negation(phi->subformula_l,int_preds,num_ip);
        break;

    default:
        assert ( 0 );
    }

    return phi;
}

_formula* 
minimal_simu(_formula* phi, const int* int_preds, int num_ip,
		 const _formula* reff )
{
    assert(phi);

    if (num_ip <= 0)//No internal predicates, fast return.
    {
        return phi;
    }

    switch (phi->formula_type)
    {
	case ATOM:
		return phi;
	
    case NEGA:
        assert(phi->subformula_l);
        if ( ATOM == phi->subformula_l->formula_type &&
             in_list ( int_preds, num_ip, phi->subformula_l->predicate_id ) )
        {
			/* internal predicates 
			 * -F ----> (F->reff)
			 */
			phi->formula_type = IMPL;
			phi->subformula_r = copy_formula(reff);
        }
        else
        {
            phi->subformula_l = minimal_simu(phi->subformula_l, 
												int_preds, num_ip, reff );
        }
        return phi;
		
    case CONJ:
    case DISJ:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        phi->subformula_l = minimal_simu (phi->subformula_l, 
												int_preds, num_ip, reff );
        phi->subformula_r = minimal_simu (phi->subformula_r, 
												int_preds, num_ip, reff );
        return phi;
		
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        phi->subformula_l = minimal_simu (phi->subformula_l, 
												int_preds, num_ip, reff );
        return phi;
		
    default:
        assert(0);
    }
    return NULL;
}
