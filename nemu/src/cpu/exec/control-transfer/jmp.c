#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(jmp_i)
make_helper_v(jmp_rm)
make_helper_v(jmp_ptr)
//make_helper_v(jmp_rel)
