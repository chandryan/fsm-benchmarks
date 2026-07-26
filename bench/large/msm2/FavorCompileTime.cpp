#define TRANSITION_TABLE_TYPE boost::mp11::mp_list

#include <boost/mp11.hpp>
#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm2/back/favor_compile_time.hpp>
#include <boost/msm2/back/state_machine.hpp>

struct config : back::state_machine_config
{
    using compile_policy = back::favor_compile_time;
};

using fsm = back::state_machine<fsm_, config>;

int test()
{
    return run_fsm<fsm>();
}
