#include "prenex.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int newNexName = 0;

int prenex_newName(void)
{
	char name_buf[512];

	sprintf(name_buf, "PN_%i", newNexName++);
	while( query_symbol(name_buf,VARIABLE) >= 0) // symbol exist
	{
		newNexName++;
	}
	return add_symbol(name_buf,VARIABLE,0);
}

_formula* convert_prenex(_formula* fml)
{
	_formula *fml_curr, *fml_prev;
	_formula *fml_l, *fml_r;
	
	int priority_l, priority_r;
	FORMULA_TYPE type_temp;
	
    assert(fml);
    switch (fml->formula_type)
    {
    case ATOM:
        return fml;
	///////////////////////////////////////////////////////////////////////////
	//Move all negations inward such that, in the end, negations only appear as
	//part of literals.
    case NEGA:
        assert (fml->subformula_l);
        fml->subformula_l = convert_prenex(fml->subformula_l);
		
        fml_prev = fml;
		fml_curr = fml->subformula_l;
        while(true)
        {
            assert(fml_curr);

            if(fml_curr->formula_type == UNIV)
            {
				/*from:   ...->|NEGA|->|UNIV|->...
				 *to:     ...->|EXIS|->|NEGA|->...
				 */   
				fml_curr->formula_type = NEGA;
                fml_prev->formula_type = EXIS;
				fml_prev->variable_id  = fml_curr->variable_id;
            }
            else 
			if(fml_curr->formula_type == EXIS)
            {
				/*from:   ...->|NEGA|->|EXIS|->...
				 *to:     ...->|UNIV|->|NEGA|->...
				 */   
				fml_curr->formula_type = NEGA;
                fml_prev->formula_type = UNIV;
				fml_prev->variable_id  = fml_curr->variable_id;
            }
            else
            {
                break;
            }

            fml_prev = fml_curr;
            fml_curr = fml_curr->subformula_l;
        }
        return fml;
	///////////////////////////////////////////////////////////////////////////
    case IMPL:
    case CONJ:
    case DISJ:
        assert(fml->subformula_l);
        assert(fml->subformula_r);

        fml->subformula_l = convert_prenex(fml->subformula_l);
        fml->subformula_r = convert_prenex(fml->subformula_r);
		
		//handle IMPL specials
		if(fml->formula_type == IMPL)
		{	
			fml_prev = fml;
			fml_curr = fml->subformula_l;
			while(fml_curr->formula_type == UNIV || fml_curr->formula_type == EXIS)
			{
				if(fml_prev->formula_type == EXIS && fml_curr->formula_type == EXIS)
				{
					break;
				}
				fml_curr->formula_type == (fml_curr->formula_type == UNIV)?EXIS:UNIV;
				if(fml_prev->formula_type == fml_curr->formula_type && 
				   fml_prev->variable_id  >  fml_curr->variable_id)
				{
					fml_prev->variable_id ^= fml_curr->variable_id;//swap
					fml_curr->variable_id ^= fml_prev->variable_id;
					fml_prev->variable_id ^= fml_curr->variable_id;
				}
				
				fml_prev = fml_curr;
				fml_curr = fml_curr->subformula_l;
			}
		}
		
		while(true)
		{
			fml_l = fml_curr->subformula_l;
			fml_r = fml_curr->subformula_r;
			
			//priority
			priority_l = 0;
			priority_r = 0;
			
			if(fml_l->formula_type == UNIV) priority_l+=4;
			if(fml_l->formula_type == EXIS) priority_l+=2;
			if(fml_r->formula_type == UNIV) priority_r+=4;
			if(fml_r->formula_type == EXIS) priority_r+=2;
			
			if(fml_l->formula_type == UNIV || fml_l->formula_type == EXIS)
			{
				if(fml_r->formula_type == UNIV || fml_r->formula_type == EXIS)
				{
					if(fml_l->variable_id  < fml_r->variable_id)
						priority_l+=1;
					if(fml_l->variable_id  > fml_r->variable_id)
						priority_r+=1;
				}
			}
			
			//done, break
			if(fml_curr->formula_type == EXIS)
			{
				if(fml_l->formula_type == UNIV && fml_r->formula_type == UNIV)
				{
					break;
				}
			}
			
			//done, break
			if(priority_l == 0 && priority_r == 0)
			{
				break;
			}
			
			if(priority_l == priority_r)
			{
				fml_r = fml_r->subformula_l;
				free(fml_curr->subformula_r);
			}
			if(priority_l > priority_r)
			{
				if( find_var_formula(fml_r, fml_l->variable_id) == true)
				{
					rename_var_formula(fml_r, fml_l->variable_id, prenex_newName());
				}
			}
			if(priority_l < priority_r)
			{
				if( find_var_formula(fml_l, fml_r->variable_id) == true)
				{
					rename_var_formula(fml_l, fml_r->variable_id, prenex_newName());
				}
				fml_l = fml_curr->subformula_r;
				fml_r = fml_curr->subformula_l;
				
			}
			
			//swap
			type_temp = fml_curr->formula_type;
			
			fml_curr->formula_type = fml_l->formula_type;//pop quantifier
			fml_curr->variable_id  = fml_l->variable_id;
			fml_curr->subformula_l = fml_l;
			
			fml_l->formula_type = type_temp;//sink CONJ/DISJ
			fml_l->subformula_r = fml_r;

			//move forward
			fml_curr = fml_l;
		}
        return fml;
	///////////////////////////////////////////////////////////////////////////
    case UNIV:
    case EXIS:
        assert(fml->subformula_l);
        fml->subformula_l = convert_prenex(fml->subformula_l);
		
		fml_prev = fml;
		fml_curr = fml->subformula_l;
		
		while(fml_prev->formula_type == fml_curr->formula_type && 
			  fml_prev->variable_id  >  fml_curr->variable_id)
		{
			fml_prev->variable_id ^= fml_curr->variable_id;//swap
			fml_curr->variable_id ^= fml_prev->variable_id;
			fml_prev->variable_id ^= fml_curr->variable_id;
			
			fml_prev = fml_curr;
			fml_curr = fml_curr->subformula_l;
		}
        return fml;
	///////////////////////////////////////////////////////////////////////////
	default:
		assert(false);//out of valid formula type
    }

    return NULL;
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
