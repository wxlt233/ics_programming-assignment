#include "cpu/exec/template-start.h"

#define instr cmov

#if DATA_BYTE==2||DATA_BYTE==4
static void do_execute()
{
	DATA_TYPE opcode=instr_fetch(cpu.eip+1,1);
	if (opcode==0x40&&cpu.eflags.OF==1)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x41&&cpu.eflags.OF==0)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x42&&cpu.eflags.CF==1)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x43&&cpu.eflags.CF==0)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x44&&cpu.eflags.ZF==1)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x45&&cpu.eflags.ZF==0)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x46&&(cpu.eflags.CF==1||cpu.eflags.ZF==1))
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x47&&(cpu.eflags.CF==0&&cpu.eflags.ZF==0))
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x48&&cpu.eflags.SF==1)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x49&&cpu.eflags.SF==0)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4A&&cpu.eflags.PF==1)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4B&&cpu.eflags.PF==0)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4C&&cpu.eflags.SF!=cpu.eflags.OF)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4D&&cpu.eflags.SF==cpu.eflags.OF)
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4E&&(cpu.eflags.ZF==1||cpu.eflags.SF!=cpu.eflags.OF))
	{
		OPERAND_W(op_dest,op_src->val);
	}
	else if (opcode==0x4F&&(cpu.eflags.ZF==0&&cpu.eflags.SF==cpu.eflags.OF))
	{
		OPERAND_W(op_dest,op_src->val);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)

#endif
#include "cpu/exec/template-end.h"
