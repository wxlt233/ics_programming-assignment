#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX),3);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr,3);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE==4
make_helper(concat(mov_c2r_,SUFFIX))
{
	int len=decode_rm_l(eip+1);
	OPERAND_W(op_dest,cpu.cr0.val);
	return len+1;	
}
#endif

#if DATA_BYTE==4
make_helper(concat(mov_r2c_,SUFFIX))
{
	int len=decode_rm_l(eip+1);
	cpu.cr0.val=op_src->val;
	return len+1;
}
#endif

#if DATA_BYTE==2
make_helper(concat(mov_r2sr_,SUFFIX))
{
	int len=decode_rm_l(eip+1);
	int sreg=0,index=0;
	lnaddr_t descaddr=cpu.GDTR.base+8*index;
	cpu.DESC[sreg].limit_15_0=lnaddr_read(descaddr,2)&0xffff;
	cpu.DESC[sreg].base_15_0=lnaddr_read(descaddr+2,2)&0xffff;
	cpu.DESC[sreg].base_23_16=lnaddr_read(descaddr+4,1)&0xff;
	cpu.DESC[sreg].limit_19_16=lnaddr_read(descaddr+6,1)&0xf;
	cpu.DESC[sreg].base_31_24=lnaddr_read(descaddr+7,1)&0xff;
	

	return len+1;
}
#endif

#include "cpu/exec/template-end.h"
