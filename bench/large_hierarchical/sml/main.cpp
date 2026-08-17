// Copyright Florian Goujeon 2021.
// Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
// https://www.boost.org/LICENSE_1_0.txt)
// Official repository: https://github.com/fgoujeon/fsm-benchmark

#include "sml_common.hpp"
#include "large_hierarchical_common.hpp"

#define TRANSITION_ROW_WITH_OFFSET(N) state<state_tpl<N + Offset>> + event<state_transition_event<N + Offset>>[guard<N + Offset>{}] / state_transition_action<N + Offset>{} = state<state_tpl<(N + 1) % 25 + Offset>>

template <size_t Offset = 0, typename SubFsm = void>
struct fsm;

template <size_t Offset, typename SubFsm>
struct fsm {
  using sub_fsm = SubFsm;

  auto operator()() const {
    using namespace boost::sml;

    return make_transition_table(
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      TRANSITION_ROW_WITH_OFFSET(N),           \
      INTERNAL_TRANSITION_ROW(N + Offset)
        *FOR_RANGE_25
#undef X
,
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      ENTRY_ACTION_ROW((N*2) + Offset)
        *FOR_RANGE_10
#undef X
        ,
        state<state_tpl<Offset>> + event<enter_sub_fsm_event> = state<SubFsm>,
        state<SubFsm> + event<exit_sub_fsm_event> = state<state_tpl<Offset>>
        );
  }

  static constexpr size_t offset = Offset;
};

template <size_t Offset>
struct fsm<Offset, void> {
  auto operator()() const {
    using namespace boost::sml;

    // clang-format off
    return make_transition_table(
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      TRANSITION_ROW_WITH_OFFSET(N),           \
      INTERNAL_TRANSITION_ROW(N + Offset)
        *FOR_RANGE_25
#undef X
,
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      ENTRY_ACTION_ROW((N*2) + Offset)
        *FOR_RANGE_10
#undef X
    );

    // clang-format on
  }

  static constexpr size_t offset = Offset;
};

template <typename Fsm>
int run_fsm() {
  using fsm = boost::sml::sm<Fsm, boost::sml::process_queue<std::queue>>;
  auto ctx = context{};
  auto first_sm = fsm{ctx};
  int counter = 0;

  // First FSM
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
  first_sm.process_event(state_transition_event<N>{}); \
  first_sm.process_event(internal_transition_event{});
    FOR_RANGE_25
#undef X
  }

  // Second FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
  first_sm.process_event(state_transition_event<N+Fsm::sub_fsm::offset>{}); \
  first_sm.process_event(internal_transition_event{});
    FOR_RANGE_25
#undef X
  }

  // Third FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
  first_sm.process_event(state_transition_event<N+Fsm::sub_fsm::sub_fsm::offset>{}); \
  first_sm.process_event(internal_transition_event{});
    FOR_RANGE_25
#undef X
  }

  first_sm.process_event(exit_sub_fsm_event{});
  first_sm.process_event(exit_sub_fsm_event{});

  auto sum = ctx.transition_counter +
             ctx.internal_transition_counter +
             ctx.enqueued_internal_transition_counter +
             ctx.entry_action_counter;
  
  ctx.transition_counter = 0;
  ctx.internal_transition_counter = 0;
  ctx.enqueued_internal_transition_counter = 0;
  ctx.entry_action_counter = 0;

  return sum;
}

using fsm2 = fsm<PROBLEM_SIZE / 2>;
using fsm1 = fsm<PROBLEM_SIZE / 3, fsm2>;
using StateMachine = fsm<0, fsm1>;

int test() {
  return run_fsm<StateMachine>();
}
