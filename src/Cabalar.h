/* Convert to logic
 * will destroy exist data structure
 */
#ifndef _CABALAR_H_
#define _CABALAR_H_

#include "utility.h"

// rules
_formula* Cabalar_DLT(_formula* originFml);
_formula* Cabalar_DLF(_formula* originFml);
_formula* Cabalar_CLT(_formula* originFml);
_formula* Cabalar_CLF(_formula* originFml);

_formula* Cabalar_ILT(_formula* originFml);
_formula* Cabalar_ILF(_formula* originFml);
_formula* Cabalar_IRT(_formula* originFml);
_formula* Cabalar_IRF(_formula* originFml);

_formula* Cabalar_N1(_formula* originFml);
_formula* Cabalar_N2(_formula* originFml);
_formula* Cabalar_N3(_formula* originFml);
_formula* Cabalar_N4(_formula* originFml);
_formula* Cabalar_N5(_formula* originFml);
_formula* Cabalar_N6(_formula* originFml);

_formulas* Cabalar_L1(_formula* originFml);
_formulas* Cabalar_L2(_formula* originFml);
_formulas* Cabalar_L3(_formula* originFml);
_formulas* Cabalar_L4(_formula* originFml);
_formulas* Cabalar_L5(_formula* originFml);

_formulas* Cabalar_R1(_formula* originFml);
_formulas* Cabalar_R2(_formula* originFml);
_formulas* Cabalar_R3(_formula* originFml);
_formulas* Cabalar_R4(_formula* originFml);
_formulas* Cabalar_R5(_formula* originFml);


 //tools
int Commutative_Trans(_formula* root, _formula* curr,
						FORMULA_TYPE commutativeSymbol);

_formula * Cabalar_Negative(_formula* fml);
_formulas* convert_Cabalar_Negative(_formulas* fmls);

_formulas* Cabalar_Trans(_formula* fml);
 
_formulas* convert_Cabalar(_formulas* fmls);

#endif
