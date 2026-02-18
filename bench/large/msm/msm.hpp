//Copyright Florian Goujeon 2021.
//Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
//https://www.boost.org/LICENSE_1_0.txt)
//Official repository: https://github.com/fgoujeon/fsm-benchmark

#include "msm_common.hpp"

struct fsm_: public msm::front::state_machine_def<fsm_>
{
    using initial_state = state_tpl<0>;

    using transition_table = TRANSITION_TABLE_TYPE
    <
#define X(N) \
        COMMA_IF_NOT_0(N) \
        TRANSITION_ROW(N) \
        , \
        INTERNAL_TRANSITION_ROW(N)
        FOR_RANGE_25
#undef X
    >;

    int transition_counter = 0;
    int internal_transition_counter = 0;
    int enqueued_internal_transition_counter = 0;
    int entry_action_counter = 0;
};


template<typename Fsm>
int run_fsm()
{
    auto sm = Fsm{};

    sm.start(starting{});

    for(auto i = 0; i < test_loop_size; ++i)
    {
#define X(N) \
    sm.process_event(state_transition_event<N>{}); \
    sm.process_event(internal_transition_event{});
        FOR_RANGE_25
#undef X
    }

    return sm.transition_counter + 
           sm.internal_transition_counter +
           sm.enqueued_internal_transition_counter +
           sm.entry_action_counter;
}
