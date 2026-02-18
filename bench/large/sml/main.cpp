// Copyright Florian Goujeon 2021.
// Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
// https://www.boost.org/LICENSE_1_0.txt)
// Official repository: https://github.com/fgoujeon/fsm-benchmark

#include "sml_common.hpp"
#include "large_common.hpp"

struct large {
  auto operator()() const {
    using namespace boost::sml;

    return make_transition_table(
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      TRANSITION_ROW(N),                    \
      INTERNAL_TRANSITION_ROW(N)
        *FOR_RANGE_25
#undef X
,
#define X(N)                                \
  COMMA_IF_NOT_0(N)                         \
      ENTRY_ACTION_ROW(N*2)
        *FOR_RANGE_10
#undef X
    );
  }
};

using fsm =
    boost::sml::sm<large,
                   boost::sml::process_queue<std::queue>  // Enable
                                                          // run-to-completion
                   >;

int test() {
  auto ctx = context{};
  auto sm = fsm{ctx};

  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
    sm.process_event(state_transition_event<N>{}); \
    sm.process_event(internal_transition_event{});
    FOR_RANGE_25
#undef X
  }

  return ctx.transition_counter +
         ctx.internal_transition_counter +
         ctx.enqueued_internal_transition_counter +
         ctx.entry_action_counter;
}
