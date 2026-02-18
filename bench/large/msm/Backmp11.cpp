#define TRANSITION_TABLE_TYPE boost::mp11::mp_list

#include <boost/mp11.hpp>
#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm/backmp11/state_machine.hpp>

using fsm = msm::backmp11::state_machine<fsm_>;

int test()
{
    return run_fsm<fsm>();
}
