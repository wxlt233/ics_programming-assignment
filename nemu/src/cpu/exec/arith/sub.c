#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub-template.h"
#undef DATA_BYTE


make_helper_v(sub_i2a)
make_helper_v(sub_i2rm)
int sub_si2rm_v(swaddr_t eip) { printf("doing si2rm  ");return (ops_decoded.is_data_size_16 ? sub_si2rm_w : sub_si2rm_l) (eip); }
//make_helper_v(sub_si2rm)
make_helper_v(sub_r2rm)
make_helper_v(sub_rm2r)

