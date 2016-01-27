#include "cpu/exec/template-start.h"

#define instr cwd

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(cwd_a2e_, SUFFIX)) {
		if(DATA_BYTE==2){
			if(((cpu.gpr[0]._16>>15)&1)==1)
				cpu.gpr[2]._16=0xffff;
			else
		        cpu.gpr[2]._16=0;
			print_asm("cwd");
		}
		else if(DATA_BYTE==4){
			if(((cpu.eax>>31)&1)==1)
				cpu.edx=0xffffffff;
			else
				cpu.edx=0;
			print_asm("cdq");
		}
		//print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
		return DATA_BYTE+1;
}
#endif

#include "cpu/exec/template-end.h"
