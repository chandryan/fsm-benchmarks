#define TRANSITION_TABLE_TYPE boost::mp11::mp_list

#include <boost/mp11.hpp>
#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm/backmp11/favor_compile_time.hpp>
#include <boost/msm/backmp11/state_machine.hpp>

struct config : msm::backmp11::state_machine_config
{
    using compile_policy = msm::backmp11::favor_compile_time;
};

using fsm = msm::backmp11::state_machine<fsm_, config>;

int test()
{
    return run_fsm<fsm>();
}
