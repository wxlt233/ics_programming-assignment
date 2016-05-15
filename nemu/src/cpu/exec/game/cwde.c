#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cwde)
{
	int result=(int16_t)reg_w(R_AX);
	reg_l(R_EAX)=result;
	print_asm("cwde");
	return 1;
}
