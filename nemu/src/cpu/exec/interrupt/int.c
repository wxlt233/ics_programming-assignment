#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

void raise_intr(uint8_t NO);

make_helper(int_i_b)
{
	uint8_t index=instr_fetch(cpu.eip+1,1);
	cpu.eip+=2;
	raise_intr(index);
	return 2;
}




