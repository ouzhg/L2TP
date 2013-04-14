#ifndef PRENEX_H
#define	PRENEX_H

#include "utility.h"

//utility
int prenex_newName(void);

//transformations
_formula* convert_prenex(_formula* fml);
_formula* minimal_simu(_formula* phi, const int* int_preds, int num_ip,
						 const _formula* reff);

#endif	/* PRENEX_H */

