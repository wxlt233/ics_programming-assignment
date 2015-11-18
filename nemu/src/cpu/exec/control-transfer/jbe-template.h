#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
	printf("%d",op_src->val);
	if (cpu.eflags.ZF==1||cpu.eflags.CF==1) cpu.eip+=(op_src->val);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


