#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char *wexpr;       //store the expression
	uint32_t oldvalue,newvalue; 

	/* TODO: Add more members if necessary */


} WP;

WP * new_wp();
void free_wp(WP *wp);
WP *heado();


#endif
