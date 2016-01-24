#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"
#include "data-mov/pop.h"
#include "data-mov/movzx.h"
#include "data-mov/movsx.h"

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
#include "arith/adc.h"
#include "arith/sbb.h"

#include "bit/cld.h"

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
#include "control-transfer/seta.h"
#include "control-transfer/setae.h"
#include "control-transfer/setb.h"
#include "control-transfer/setbe.h"
#include "control-transfer/setc.h"
#include "control-transfer/sete.h"
#include "control-transfer/setg.h"
#include "control-transfer/setge.h"
#include "control-transfer/setl.h"
#include "control-transfer/setle.h"
#include "control-transfer/setna.h"
#include "control-transfer/setnae.h"
#include "control-transfer/setnb.h"
#include "control-transfer/setnbe.h"
#include "control-transfer/setnc.h"
#include "control-transfer/setne.h"
#include "control-transfer/setng.h"
#include "control-transfer/setnge.h"
#include "control-transfer/setnl.h"
#include "control-transfer/setnle.h"
#include "control-transfer/setno.h"
#include "control-transfer/setnp.h"
#include "control-transfer/setns.h"
#include "control-transfer/setnz.h"
#include "control-transfer/seto.h"
#include "control-transfer/setp.h"
#include "control-transfer/setpe.h"
#include "control-transfer/setpo.h"
#include "control-transfer/sets.h"
#include "control-transfer/setz.h"


#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
