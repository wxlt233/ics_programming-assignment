#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"
#include "data-mov/lea.h"
#include "data-mov/pop.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/cmp.h"
#include "arith/sub.h"
#include "arith/add.h"


#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/test.h"
#include "logic/and.h"

#include "control-transfer/call.h"
#include "control-transfer/jmp.h"
#include "control-transfer/leave.h"
#include "control-transfer/ret.h"
#include "control-transfer/ja.h"
#include "control-transfer/jae.h"
#include "control-transfer/jb.h"
#include "control-transfer/jbe.h"
#include "control-transfer/jc.h"
#include "control-transfer/jcxz.h"
#include "control-transfer/jecxz.h"
#include "control-transfer/je.h"
#include "control-transfer/jz.h"
#include "control-transfer/jg.h"
#include "control-transfer/jge.h"
#include "control-transfer/jl.h"
#include "control-transfer/jle.h"
#include "control-transfer/jna.h"
#include "control-transfer/jnae.h"
#include "control-transfer/jnb.h"
#include "control-transfer/jnbe.h"
#include "control-transfer/jnc.h"
#include "control-transfer/jne.h"
#include "control-transfer/jnge.h"
#include "control-transfer/jnl.h"
#include "control-transfer/jnle.h"
#include "control-transfer/jno.h"
#include "control-transfer/jnp.h"
#include "control-transfer/jns.h"
#include "control-transfer/jnz.h"
#include "control-transfer/jo.h"
#include "control-transfer/jp.h"
#include "control-transfer/jpe.h"
#include "control-transfer/jpo.h"
#include "control-transfer/js.h"
#include "control-transfer/jz.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
