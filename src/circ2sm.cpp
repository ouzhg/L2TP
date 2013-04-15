#include "circ2sm.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Cabalar.h"

_formula* convert_circ2sm(_formula* phi, const int* int_preds, int num_ip)
{
    static int new_vars[MAX_LEN_PARAMETER];
    _formula *psi, *rho, *eta;
    int i, j, curr_arity, max_arity;
    char name[128];

    //convert negative normal form
    assert(phi);
    phi = convert_negative_normal_form(phi);
    assert (phi);

    //find max_arity
    max_arity = 0;
    for(i=0; i<num_ip; i++)
    {
        curr_arity = predicate_arity(int_preds[i]);
        if (curr_arity > max_arity)
        {
            max_arity = curr_arity;
        }
    }

    //generate new variables
    for(i=0; i<max_arity; i++)
    {
        sprintf(name, "_V%d", vocabulary.num_new_variable);
        new_vars[i] = add_symbol(name, VARIABLE, 0);
        vocabulary.num_new_variable ++;
    }

    //generate rho
    rho=NULL;
    for(i=0; i<num_ip; i++)
    {
        curr_arity = predicate_arity(int_preds[i]);
        
        psi = composite_atom(ATOM, int_preds[i], NULL);
        psi->parameters = (_term*)malloc(curr_arity * sizeof(_term));
        for ( j=0; j<curr_arity; j++ )
        {
            psi->parameters[j].term_type = VARI;
            psi->parameters[j].variable_id = new_vars[j];
        }
        eta = composite_bool ( NEGA, copy_formula(psi), NULL);
        psi = composite_bool ( DISJ, psi, eta );
        
        if(NULL==rho) {
            rho = psi;
        }
        else {
            rho = composite_bool(CONJ, psi, rho);
        }
    }

    //universal closure
    for(i=max_arity-1; i>=0; i--)
    {
        rho = composite_qntf(UNIV, rho, new_vars[i]);
    }

    vocabulary.num_variable += max_arity;

    psi = composite_bool(DISJ, minimal_simu(copy_formula(phi), int_preds, num_ip, rho), rho);
    psi = composite_bool(CONJ, double_negation ( phi, int_preds, num_ip ), psi);

    return psi;
}
