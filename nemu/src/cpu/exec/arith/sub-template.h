#include "cpu/exec/template-start.h"
#define instr sub

static void do_execute(){
	uint64_t src=op_src->val;
	uint64_t result=op_dest->val-src;
	OPERAND(op_dest,result);


//	print_asm_template1();
}
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

