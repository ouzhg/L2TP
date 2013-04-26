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

_formula* 
convert_prenex(_formula* fml)
{
	_formula *fml_curr, *fml_prev;
	_formula *fml_l, *fml_r;
	
	int priority_l, priority_r;
	FORMULA_TYPE type_curr, type_temp;
	
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
			fml_curr = fml->subformula_l;
			while(fml_curr->formula_type == UNIV || fml_curr->formula_type == EXIS)
			{
				fml_curr->formula_type == (fml_curr->formula_type == UNIV)?EXIS:UNIV;
				
				fml_curr = fml_curr->subformula_l;
			}
		}
		
        //extract quantifiers
        fml_curr = fml;
        type_curr = UNIV;
		while(true)
		{
			fml_l = fml_curr->subformula_l;
			fml_r = fml_curr->subformula_r;
            
            //extract done
			if(fml_l->formula_type != UNIV && fml_l->formula_type != EXIS && 
               fml_r->formula_type != UNIV && fml_r->formula_type != EXIS)
            {
                break;
			}
            
            //change quantifiers
            if(fml_l->formula_type != type_curr && fml_r->formula_type != type_curr)
            {
                type_curr = (type_curr==UNIV)?EXIS:UNIV;
            }
            
            //special rules
            if((fml_l->variable_id == fml_r->variable_id) &&
               (fml->formula_type == CONJ && fml_l->formula_type == fml_r->formula_type == UNIV) ||
               (fml->formula_type == DISJ && fml_l->formula_type == fml_r->formula_type == EXIS))
            {
				fml_r = fml_r->subformula_l;
				free(fml_curr->subformula_r);
                
                //swap
                type_temp = fml_curr->formula_type;

                fml_curr->formula_type = fml_l->formula_type;//pop quantifier
                fml_curr->variable_id  = fml_l->variable_id;
                fml_curr->subformula_l = fml_l;

                fml_l->formula_type = type_temp;//sink CONJ/DISJ
                fml_l->subformula_l = fml_l->subformula_l;
                fml_l->subformula_r = fml_r;
                
                fml_curr = fml_l;
                continue;
            }
			
			//priority
			priority_l = 0;
			priority_r = 0;
			
			if(fml_l->formula_type == type_curr) priority_l+=2;
			if(fml_r->formula_type == type_curr) priority_r+=2;
			if(fml_l->formula_type == fml_r->formula_type == type_curr)
			{
                if(fml_l->variable_id  < fml_r->variable_id)
                    priority_l+=1;
                if(fml_l->variable_id  > fml_r->variable_id)
                    priority_r+=1;
			}
			
            //extract quantifier(swap value)
			if(priority_l >= priority_r)
			{
				if( find_var_formula(fml_r, fml_l->variable_id) == true)
				{
					rename_var_formula(fml_r, fml_l->variable_id, prenex_newName());
				}
                
                //swap
                type_temp = fml_curr->formula_type;

                fml_curr->formula_type = fml_l->formula_type;//pop quantifier
                fml_curr->variable_id  = fml_l->variable_id;
                fml_curr->subformula_l = fml_l;

                fml_l->formula_type = type_temp;//sink CONJ/DISJ
                fml_l->subformula_l = fml_l->subformula_l;
                fml_l->subformula_r = fml_r;
                
                fml_curr = fml_l;
                continue;
			}
			else
			{
				if( find_var_formula(fml_l, fml_r->variable_id) == true)
				{
					rename_var_formula(fml_l, fml_r->variable_id, prenex_newName());
				}
                
                //swap
                type_temp = fml_curr->formula_type;

                fml_curr->formula_type = fml_r->formula_type;//pop quantifier
                fml_curr->variable_id  = fml_r->variable_id;
                fml_curr->subformula_l = fml_r;

                fml_r->formula_type = type_temp;//sink CONJ/DISJ
                fml_r->subformula_r = fml_r->subformula_l;
                fml_r->subformula_l = fml_l;
                
                fml_curr = fml_r;
                continue;
			}
		}
        return fml;
	///////////////////////////////////////////////////////////////////////////
    case UNIV:
    case EXIS:
        assert(fml->subformula_l);
        fml->subformula_l = convert_prenex(fml->subformula_l);
        return fml;
	///////////////////////////////////////////////////////////////////////////
	default:
		assert(false);//out of valid formula type
    }

    return NULL;
}
