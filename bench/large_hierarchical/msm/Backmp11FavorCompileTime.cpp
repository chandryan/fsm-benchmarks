#define TRANSITION_TABLE_TYPE boost::mp11::mp_list
#include <boost/mp11.hpp>
#include "nested_fsm.hpp"
//
#include <boost/msm/backmp11/favor_compile_time.hpp>
#include <boost/msm/backmp11/state_machine.hpp>

struct config;

using fsm2 = msm::backmp11::state_machine<fsm_<PROBLEM_SIZE / 2>,
                                    config>;
using fsm1 = msm::backmp11::state_machine<fsm_<PROBLEM_SIZE / 3, fsm2>,
                                    config>;
using fsm0 =
    msm::backmp11::state_machine<fsm_<0, fsm1>, config>;

struct config : msm::backmp11::state_machine_config
{
    using compile_policy = msm::backmp11::favor_compile_time;
    // Using root_sm is functionally not required for the test,
    // but it reduces compilation by enabling backmp11
    // to filter out for which SM it requires to instantiate
    // construction-related methods.
    using root_sm = fsm0;
};


int main() {
  test_fsm<fsm0>();

  return 0;
}
