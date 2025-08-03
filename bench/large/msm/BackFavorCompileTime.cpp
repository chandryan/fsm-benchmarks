#define TRANSITION_TABLE_TYPE mpl::vector
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE PROBLEM_SIZE_X_2
#define BOOST_MPL_LIMIT_MAP_SIZE PROBLEM_SIZE_X_2
#define BOOST_MPL_LIMIT_SET_SIZE PROBLEM_SIZE_X_2
#include "common.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/back/favor_compile_time.hpp>

using fsm0 = msm::back::state_machine<fsm_, msm::back::favor_compile_time>;

int main()
{
    test_fsm<fsm0>();

    return 0;
}