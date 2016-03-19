#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}
void refreshtlb();

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
	print_asm("mov %%cr0 %%%s",REG_NAME(op_src->reg));
	return len+1;	
}
#endif

#if DATA_BYTE==4
make_helper(concat(mov_r2c_,SUFFIX))
{
	int len=decode_rm_l(eip+1);
	uint32_t opcode=instr_fetch(eip+1,1);
	if (opcode==0xc0)
	{
		cpu.cr0.val=op_src->val;
		print_asm("mov  %%%s %%cr0",REG_NAME(op_src->reg));
	}
	else 
 	{
		cpu.cr3.val=op_src->val;
		refreshtlb();
		print_asm("mov  %%%s %%cr3",REG_NAME(op_src->reg));
	}
	return len+1;
}
#endif

make_helper(concat(mov_r2sr_,SUFFIX))
{
	int len=decode_rm_l(eip+1);
	int test=instr_fetch(eip+1,1);
	int sreg=0,index=0;
	if (test==0xd8) 
	{
		sreg=2;
		cpu.DS.val=op_src->val&0x0000ffff;
		index=cpu.DS.index;
	}
	else if(test==0xc0) 
	{
		sreg=0;
		cpu.ES.val=op_src->val&0x0000ffff;
		index=cpu.ES.index;
	}
	else if (test==0xd0) 
	{	
		sreg=3;
		cpu.SS.val=op_src->val&0x0000ffff;
		index=cpu.SS.index;
	}
	
	lnaddr_t descaddr=cpu.GDTR.base+8*index;
	cpu.DESC[sreg].limit_15_0=lnaddr_read(descaddr,2)&0xffff;
	cpu.DESC[sreg].base_15_0=lnaddr_read(descaddr+2,2)&0xffff;
	cpu.DESC[sreg].base_23_16=lnaddr_read(descaddr+4,1)&0xff;
	cpu.DESC[sreg].limit_19_16=lnaddr_read(descaddr+6,1)&0xf;
	cpu.DESC[sreg].base_31_24=lnaddr_read(descaddr+7,1)&0xff;
	if (sreg==0)
		print_asm("mov %%%s %%es",REG_NAME(op_src->reg));
	else if (sreg==2)
		print_asm("mov %%%s %%ds",REG_NAME(op_src->reg));
	else 
		print_asm("mov %%%s %%ss",REG_NAME(op_src->reg));
	return len+1;
}

#include "cpu/exec/template-end.h"
