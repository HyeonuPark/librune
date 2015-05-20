#include <stdlib.h>
#include "librune.h"

const rn_bool rn_true = 1;
const rn_bool rn_false = 0;
const rn_mem rn_null = 0;

const rn_schar rn_SCHAR_MIN = ((rn_schar) 1) << ((sizeof(rn_schar)*8) - 1);
const rn_sshort rn_SSHORT_MIN = ((rn_sshort) 1) << ((sizeof(rn_sshort)*8) - 1);
const rn_sint rn_SINT_MIN = ((rn_sint) 1) << ((sizeof(rn_sint)*8) - 1);
const rn_slong rn_SLONG_MIN = ((rn_slong) 1) << ((sizeof(rn_slong)*8) - 1);
const rn_schar rn_SCHAR_MAX = ~(((rn_schar) 1) << ((sizeof(rn_schar)*8) - 1));
const rn_sshort rn_SSHORT_MAX = ~(((rn_sshort) 1) << ((sizeof(rn_sshort)*8) - 1));
const rn_sint rn_SINT_MAX = ~(((rn_sint) 1) << ((sizeof(rn_sint)*8) - 1));
const rn_slong rn_SLONG_MAX = ~(((rn_slong) 1) << ((sizeof(rn_slong)*8) - 1));
const rn_uchar rn_UCHAR_MAX = ~((rn_uchar)0);
const rn_ushort rn_USHORT_MAX = ~((rn_sshort)0);
const rn_uint rn_UINT_MAX = ~((rn_sint)0);
const rn_ulong rn_ULONG_MAX = ~((rn_slong)0);
