#ifndef _HENGZHANG_H_
#define _HENGZHANG_H_

#include "utility.h"

//utilities
int HengZhang_addSymbol(const char* name, SYMBOL_TYPE type, int arity);
_formula* HengZhang_recordQuantifier(_formula* originalFml);

//creators
_formulas* HengZhang_create(_formula* originalFml);
_formula* HengZhang_createFormula_1(_formula* originalFml);
_formula* HengZhang_createFormula_2(_formula* originalFml);
_formula* HengZhang_createFormula_3(_formula* originalFml);
_formula* HengZhang_createFormula_4(_formula* originalFml);
_formula* HengZhang_createFormula_5(_formula* originalFml);

#endif
