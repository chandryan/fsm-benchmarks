#define TRANSITION_TABLE_TYPE boost::mp11::mp_list
#include <boost/mp11.hpp>
#include "msm.hpp"
//
#include <boost/msm2/back/favor_compile_time.hpp>
#include <boost/msm2/back/state_machine.hpp>

struct config : back::state_machine_config
{
    using compile_policy = back::favor_compile_time;
};

using Composite2 = Composite<PROBLEM_SIZE / 2>;
using Composite1 = Composite<PROBLEM_SIZE / 3, Composite2>;
using StateMachine = back::state_machine<Composite<0, Composite1>, config>;

int test()
{
    return run_fsm<StateMachine>();
}
