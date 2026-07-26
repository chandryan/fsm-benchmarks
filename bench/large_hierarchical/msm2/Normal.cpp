#define TRANSITION_TABLE_TYPE boost::mp11::mp_list

#include <boost/mp11.hpp>
#include "msm.hpp"
#include <boost/msm2/back/state_machine.hpp>
#include "large_hierarchical_common.hpp"


struct config;

using fsm2 = back::state_machine<fsm_<PROBLEM_SIZE/2>, config>;
using fsm1 = back::state_machine<fsm_<PROBLEM_SIZE/3, fsm2>, config>;
using fsm0 = back::state_machine<fsm_<0, fsm1>, config>;

struct config : back::state_machine_config
{
    // Using root_sm is functionally not required for the test,
    // but it reduces compilation by enabling the back-end
    // to filter out for which SM it requires to instantiate
    // construction-related methods.
    using root_sm = fsm0;
};

int test()
{
    return run_fsm<fsm0>();
}
