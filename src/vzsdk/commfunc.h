#ifndef SRC_VZSDK_COMMFUNC_H_
#define SRC_VZSDK_COMMFUNC_H_
#include "vzsdk/vzclientsdk_lpdefine.h"
#include "vzsdk/vzlprtcpsdkdefine.h"

namespace vzsdk{
inline    VZ_LPRC_RESULT_TYPE GetResultTypeFromTrigBits(unsigned uBitsTrigType) {
        if (uBitsTrigType == TRIGGER_TYPE_AUTO_BIT)
            return(VZ_LPRC_RESULT_STABLE);

        if (uBitsTrigType == TRIGGER_TYPE_EXTERNAL_BIT)
            return(VZ_LPRC_RESULT_IO_TRIGGER);

        if (uBitsTrigType == TRIGGER_TYPE_SOFTWARE_BIT)
            return(VZ_LPRC_RESULT_FORCE_TRIGGER);

        if (uBitsTrigType == TRIGGER_TYPE_VLOOP_BIT)
            return(VZ_LPRC_RESULT_VLOOP_TRIGGER);

        return(VZ_LPRC_RESULT_MULTI_TRIGGER);
    }
}

#endif