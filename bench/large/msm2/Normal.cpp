#define TRANSITION_TABLE_TYPE boost::mp11::mp_list

#include <boost/mp11.hpp>
#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm2/back/state_machine.hpp>

using fsm = back::state_machine<fsm_>;

int test()
{
    return run_fsm<fsm>();
}
