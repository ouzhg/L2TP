#include "rule.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//rules
///////////////////////////////////////////////////////////////////////////////
_rules* convert_rules(_formulas* fmls)
{
	_rules* head = NULL;
	_rules* tail = NULL;
	_formulas* curr;
	
	assert(fmls);
	
	head = tail = (_rules*)malloc(sizeof(_rules));
	memset(head, 0, sizeof(_rules));
	
	while(fmls)
	{
		tail->remained_rules = (_rules*)malloc(sizeof(_rules));
		tail = tail->remained_rules;
        tail->remained_rules = NULL;
        
        tail->curr_rule = convert_rule(fmls->curr_formula);
		
		curr = fmls;
		fmls = curr->remained_formulas;
		free(curr);
	}
	
	tail = head;
	head = head->remained_rules;
	free(tail);//borrow `tail` to store pre-patch pointer temporary 
	
	return head;
}

_rule* convert_rule(_formula* fml)
{
    _rule* rule;
    
	rule = (_rule*)malloc(sizeof(_rule));
	memset(rule, 0, sizeof(_rule));
    
	assert(fml);
    while(UNIV==fml->formula_type)//remove universal quantifier
    {
        assert(fml->subformula_l);
        fml = fml->subformula_l;
    }
	
	switch(fml->formula_type)
	{
	case IMPL://most frequent
		convert_rule_CONJ(rule,fml->subformula_l);
		convert_rule_DISJ(rule,fml->subformula_r);
		free(fml);
		break;
		
	case ATOM:
		fml = composite_bool(NEGA,fml,NULL);
		fml = composite_bool(NEGA,fml,NULL);
	case NEGA:
	case DISJ:
		convert_rule_DISJ(rule,fml);//  ~A == :- A
		break;
	
	default:
		assert(0);
	}
	
	return rule;
}

void convert_rule_CONJ(_rule* rule, _formula* fml)
{
	assert(rule);
	assert(fml);
	
	switch(fml->formula_type)
	{
	case ATOM:
        if(fml->predicate_id != PRED_TRUE)// A ^ TRUE == A
            rule->body = push_formulas(fml,rule->body);
		break;

	case NEGA:
        rule->body = push_formulas(fml,rule->body);
		break;

	case CONJ:
        convert_rule_CONJ(rule, fml->subformula_l), 
        convert_rule_CONJ(rule, fml->subformula_r);
        free(fml);
		break;

	case DISJ:
		assert(0);
		break;

	default:
		assert(0);
	}
}

void convert_rule_DISJ(_rule* rule, _formula* fml)
{
	assert(rule);
	assert(fml);
	
	switch(fml->formula_type)
	{
	case ATOM:
        if(fml->predicate_id != PRED_FALSE)// A v FALSE == A
            rule->head = push_formulas(fml,rule->head);
		break;

	case NEGA:
        if(fml->subformula_l->formula_type == ATOM)//reversing Cabalar L3 
        {
            rule->body = push_formulas(composite_bool(NEGA,fml,NULL),
                                        rule->body);
        }
        else
        {
            rule->body = push_formulas(fml->subformula_l,rule->body);
            free(fml);
        }
		break;

	case CONJ:
		assert(0);
		break;

	case DISJ:
        convert_rule_DISJ(rule, fml->subformula_l), 
        convert_rule_DISJ(rule, fml->subformula_r);
        free(fml);
		break;

	default:
		assert(0);
	}
}

int check_rule(_rule *rule)
{
	//output: 0 - OK, -1 - need to delete
    const _formulas* head;
    const _formulas* body;
	const _formulas *p, *q;
	_formula phi;
    
    head = rule->head;
    body = rule->body;

	phi.formula_type = NEGA;

	for (p=body; p; p=p->remained_formulas)
	{
		assert(p->curr_formula);
		for (q=head; q; q=q->remained_formulas)
		{
			assert(q->curr_formula);
			if (compare_formula(p->curr_formula,q->curr_formula))
				return -1;
		}

		switch (p->curr_formula->formula_type)
		{
		case ATOM:
			phi.subformula_l = p->curr_formula;
			for (q=p->remained_formulas; q; q=q->remained_formulas)
			{
				assert(q->curr_formula);
				if (compare_formula(&phi,q->curr_formula))
					return -1;
			}
			break;
		case NEGA:
			for (q=p->remained_formulas; q; q=q->remained_formulas)
			{
				assert(q->curr_formula);
				if (compare_formula(p->curr_formula->subformula_l,q->curr_formula))
					return -1;
			}
			break;
		default:
			assert(0);
		}
	}
	return 0;
}

void output_rule(FILE* out, const _rule* rule)
{
	_formulas* lit;
    
    for(lit=rule->head; lit; lit=lit->remained_formulas)
    {
        if(lit!=rule->head) {
            fprintf( out, " | " );
        }
        output_formula(out, lit->curr_formula);
    }

    if(NULL!=rule->body) fprintf(out, " :- ");

    for(lit=rule->body; lit; lit=lit->remained_formulas)
    {
        if(lit!=rule->body) {
            fprintf(out, ", ");
        }

        if(lit->curr_formula->formula_type == NEGA)
        {
            if(lit->curr_formula->subformula_l->formula_type == NEGA)
            {
                fprintf(out, "not _");
                output_formula(out, 
                        lit->curr_formula->subformula_l->subformula_l);
            }
            else
            {
                fprintf(out, "_");
                output_formula( out, lit->curr_formula->subformula_l );
            }
        }
        else
        {
            output_formula( out, lit->curr_formula );
        }
    }
    fprintf( out, ".\n" );
}

void output_rules(FILE* out, const _rules* rules)
{
    while(rules)
    {
        assert(rules->curr_rule);
        output_rule(out,rules->curr_rule);
        
        rules = rules->remained_rules;
    }
}
