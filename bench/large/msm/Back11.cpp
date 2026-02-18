#define TRANSITION_TABLE_TYPE boost::fusion::vector

#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm/back11/state_machine.hpp>

using fsm = msm::back11::state_machine<fsm_>;

int test()
{
    return run_fsm<fsm>();
}
