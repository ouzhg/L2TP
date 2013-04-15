#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "struct.h"
#include <vector>

extern _vocabulary vocabulary;
extern _formula* gformula;
extern int context_flag;

//utility
///////////////////////////////////////////////////////////////////////////////
void init_vocabulary(void);
void destroy_vocabulary(void);
bool in_list(const int* list, int len, int obj);
bool is_negative(const _formula* phi, 
				  const int* sm_preds, int num_sp, bool negative);
bool is_universal(_formula* fml);

//symbol operations
///////////////////////////////////////////////////////////////////////////////
int add_symbol(const char* name, SYMBOL_TYPE type, int arity);
int query_symbol(const char* name, SYMBOL_TYPE type);
int predicate_arity(int id);
int function_arity(int id);
char* query_name(int id, SYMBOL_TYPE type);

//terms operations
///////////////////////////////////////////////////////////////////////////////
_term* combine_terms(const _term* head, int head_size,
					   const _term* tail, int tail_size);//VARI terms ONLY
_term* combine_terms(const std::vector<int>& head, 
					   const std::vector<int>& tail);//VARI terms ONLY
_term*   copy_terms (const _term* terms, int size);
void   delete_terms (_term* terms, int size);
bool   compare_term (const _term* t, const _term* s);

bool find_var_term(const _term* t, int var_id);
void rename_var_term(_term* t, int oldv, int newv);

_term* replace_term (_term* terms, int arity, 
			const std::vector<int>& exis, const std::vector<int>& replacements);

void output_term(FILE* out, const _term* t);

//formula operations
///////////////////////////////////////////////////////////////////////////////
_formula* composite_atom(FORMULA_TYPE formula_type, 
							int predicate_id, _term* parameters);	 //ATOM
_formula* composite_qntf(FORMULA_TYPE formula_type, 
							_formula* subformula_l, int variable_id);//EXIS,UNIV
_formula* composite_bool(FORMULA_TYPE formula_type,
							_formula* subformula_l, _formula* subformula_r);

_formula* copy_formula(const _formula *fml);
bool  compare_formula(const _formula* phi, const _formula* psi);
void   delete_formula(_formula* fml);

bool   find_var_formula(const _formula* phi, int var_id);
void rename_var_formula(_formula* phi, int oldv, int newv);
_formula* replace_terms(_formula* fml, 
			const std::vector<int>& exis, const std::vector<int>& replacements);

void output_formula(FILE* out, const _formula* phi);

//formulas operations
///////////////////////////////////////////////////////////////////////////////
_formulas* join_formulas(_formulas* head, _formulas* tail);
_formulas* push_formulas(_formula* fml, _formulas* tail);
_formulas* pop_formulas (_formulas* fmls);//DO NOT DELETE formula
_formula*  top_formulas (_formulas* fmls);
_formulas* copy_formulas(const _formulas* sigma);
void	   delete_formulas(_formulas* sigma);
int		   size_formulas(_formulas* fmls);

void output_formulas(FILE* out, const _formulas* fmls);

//formula transforms
///////////////////////////////////////////////////////////////////////////////
void output_extraDefinition(FILE* out);

_formula* double_negation(_formula* phi, const int* int_preds, int num_ip);
_formula* minimal_simu(_formula* phi, const int* int_preds, int num_ip,
						 const _formula* reff);

#endif