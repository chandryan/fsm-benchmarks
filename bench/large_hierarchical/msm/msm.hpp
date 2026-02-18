//Copyright Florian Goujeon 2021.
//Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
//https://www.boost.org/LICENSE_1_0.txt)
//Official repository: https://github.com/fgoujeon/fsm-benchmark

#include "msm_common.hpp"
#include "large_hierarchical_common.hpp"

#define TRANSITION_ROW_WITH_OFFSET(N) Row<state_tpl<N, Offset>, state_transition_event<N + Offset>, state_tpl<(N + 1) % 25, Offset>, state_transition_action<N + Offset>, guard<N + Offset>>
#define INTERNAL_TRANSITION_ROW_WITH_OFFSET(N) Row<state_tpl<N, Offset>, internal_transition_event, none, internal_transition_action<N + Offset>>

template<size_t Offset = 0, typename SubFsm = void>
struct fsm_: public msm::front::state_machine_def<fsm_<Offset, SubFsm>>
{
    using initial_state = state_tpl<0, Offset>;
    using sub_fsm = SubFsm;

    using transition_table = TRANSITION_TABLE_TYPE
    <
#define X(N) \
        COMMA_IF_NOT_0(N) \
        TRANSITION_ROW_WITH_OFFSET(N) \
        , \
        INTERNAL_TRANSITION_ROW_WITH_OFFSET(N)
        FOR_RANGE_25
#undef X
    , Row<state_tpl<0, Offset>, enter_sub_fsm_event, sub_fsm, none, none>
    , Row<sub_fsm, exit_sub_fsm_event, state_tpl<0, Offset>, none, none>
    >;

    int transition_counter = 0;
    int internal_transition_counter = 0;
    int enqueued_internal_transition_counter = 0;
    int entry_action_counter = 0;
    static constexpr size_t offset = Offset;
};

template<size_t Offset>
struct fsm_<Offset, void>: public msm::front::state_machine_def<fsm_<Offset, void>>
{
    using initial_state = state_tpl<0, Offset>;

    using transition_table = TRANSITION_TABLE_TYPE
    <
#define X(N) \
        COMMA_IF_NOT_0(N) \
        TRANSITION_ROW_WITH_OFFSET(N) \
        , \
        INTERNAL_TRANSITION_ROW_WITH_OFFSET(N)
        FOR_RANGE_25
#undef X
    >;

    int transition_counter = 0;
    int internal_transition_counter = 0;
    int enqueued_internal_transition_counter = 0;
    int entry_action_counter = 0;
    static constexpr size_t offset = Offset;
};

template<typename Fsm>
int run_fsm()
{
    auto first_sm = Fsm{};
    auto& second_sm = first_sm.template get_state<typename Fsm::sub_fsm&>();
    auto& third_sm = second_sm.template get_state<typename Fsm::sub_fsm::sub_fsm&>();

    first_sm.start(starting{});

    // First FSM
    for(auto i = 0; i < test_loop_size; ++i)
    {
#define X(N) \
    first_sm.process_event(state_transition_event<N>{}); \
    first_sm.process_event(internal_transition_event{});
        FOR_RANGE_25
#undef X
    }

    // Second FSM
    first_sm.process_event(enter_sub_fsm_event{});
    for(auto i = 0; i < test_loop_size; ++i)
    {
#define X(N) \
    first_sm.process_event(state_transition_event<N+Fsm::sub_fsm::offset>{}); \
    first_sm.process_event(internal_transition_event{});
        FOR_RANGE_25
#undef X
    }

    // Third FSM
    first_sm.process_event(enter_sub_fsm_event{});
    for(auto i = 0; i < test_loop_size; ++i)
    {
#define X(N) \
    first_sm.process_event(state_transition_event<N+Fsm::sub_fsm::sub_fsm::offset>{}); \
    first_sm.process_event(internal_transition_event{});
        FOR_RANGE_25
#undef X
    }

    first_sm.process_event(exit_sub_fsm_event{});
    first_sm.process_event(exit_sub_fsm_event{});

    auto sum = first_sm.transition_counter +
               first_sm.internal_transition_counter +
               first_sm.enqueued_internal_transition_counter +
               first_sm.entry_action_counter +
               second_sm.transition_counter +
               second_sm.internal_transition_counter +
               second_sm.enqueued_internal_transition_counter +
               second_sm.entry_action_counter +
               third_sm.transition_counter +
               third_sm.internal_transition_counter +
               third_sm.enqueued_internal_transition_counter +
               third_sm.entry_action_counter;

    return sum;
}

template<typename Fsm>
int test_fsm()
{
    constexpr auto main_loop_size = 1000;

    auto counter = 0;

    for(auto i = 0; i < main_loop_size; ++i)
    {
        counter += run_fsm<Fsm>();
    }

    volatile bool success = (counter == 195004000);
    return (success ? 0 : 1);
}
