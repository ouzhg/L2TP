#ifndef PRENEX_H
#define	PRENEX_H

#include "utility.h"

//utility
int prenex_newName(void);

//transformations
_formula* convert_prenex(_formula* fml);

#endif	/* PRENEX_H */

