#define TRANSITION_TABLE_TYPE mpl::vector
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE PROBLEM_SIZE_X_2
#define BOOST_MPL_LIMIT_MAP_SIZE PROBLEM_SIZE_X_2
#define BOOST_MPL_LIMIT_SET_SIZE PROBLEM_SIZE_X_2

#include "msm.hpp"
#include "large_common.hpp"
#include <boost/msm/back/state_machine.hpp>

using fsm = msm::back::state_machine<fsm_>;

int test()
{
    return run_fsm<fsm>();
}
