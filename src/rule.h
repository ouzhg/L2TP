#ifndef RULE_H
#define	RULE_H

#include "utility.h"

void convert_rule_CONJ(_rule* rule, _formula* fml);
void convert_rule_DISJ(_rule* rule, _formula* fml);
_rule*  convert_rule(_formula* fml);
_rules* convert_rules(_formulas* fmls);

int check_rule(const _rule* rule);

void output_rule (FILE* out, const _rule* rule);
void output_rules(FILE* out, const _rules* rules);

#endif	/* RULE_H */

