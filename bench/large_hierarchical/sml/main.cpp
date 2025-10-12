// Copyright Florian Goujeon 2021.
// Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
// https://www.boost.org/LICENSE_1_0.txt)
// Official repository: https://github.com/fgoujeon/fsm-benchmark

#include <boost/sml.hpp>
#include <cassert>
// #include <iostream>
#include <queue>
#include "common.hpp"

struct context {
  int counter = 0;
};

template <int Index>
struct state_tpl {};

template <int Index>
struct state_transition_event {
  int two = 2;
};

struct internal_transition_event {
  int two = 2;
};

struct enter_sub_fsm_event {
  int two = 2;
};

struct exit_sub_fsm_event {
  int two = 2;
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct state_transition_action {
  void operator()(const state_transition_event<Index>& evt, context& ctx) {
    ctx.counter = (ctx.counter + 1) * evt.two;
  }
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct internal_transition_action {
  void operator()(const internal_transition_event& evt, context& ctx) {
    ctx.counter /= evt.two;
  }
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct exit_action {
  void operator()(
      boost::sml::back::process<internal_transition_event> process) {
    process(internal_transition_event{});
  }
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct guard {
  bool operator()(const state_transition_event<Index>& evt) {
    return evt.two >= 0;
  }
};

template <size_t Offset = 0, typename SubFsm = void>
struct fsm_;

template <size_t Offset, typename SubFsm>
struct fsm_ {
  using sub_fsm = SubFsm;

  auto operator()() const {
    using namespace boost::sml;

    return make_transition_table(
#define X(N)                                                                 \
  COMMA_IF_NOT_0(N)                                                          \
  state<state_tpl<N + Offset>> +                                             \
      event<state_transition_event<N + Offset>>[guard<N + Offset>{}] /       \
          state_transition_action<N + Offset>{} =                            \
      state<state_tpl<(N + 1) % PROBLEM_SIZE + Offset>>,                     \
                                      state<state_tpl<N + Offset>> +         \
                                          event<internal_transition_event> / \
                                              internal_transition_action<    \
                                                  N + Offset>{}
        *COUNTER
#undef X

#define X(N)                       \
  , state<state_tpl<N + Offset>> + \
        boost::sml::on_exit<_> / exit_action<N + Offset> {}
            COUNTER
#undef X
        ,
        state<state_tpl<Offset>> + event<enter_sub_fsm_event> = state<SubFsm>,
        state<SubFsm> + event<exit_sub_fsm_event> = state<state_tpl<Offset>>);
  }

  static constexpr size_t offset = Offset;
};

template <size_t Offset>
struct fsm_<Offset, void> {
  auto operator()() const {
    using namespace boost::sml;

    // clang-format off
    return make_transition_table(
#define X(N)                                                                 \
  COMMA_IF_NOT_0(N)                                                          \
  state<state_tpl<N + Offset>> +                                             \
    event<state_transition_event<N + Offset>>[guard<N + Offset>{}] /         \
    state_transition_action<N + Offset>{} =                                  \
    state<state_tpl<(N + 1) % PROBLEM_SIZE + Offset>>,                       \
  state<state_tpl<N + Offset>> +                                             \
    event<internal_transition_event> /                                       \
    internal_transition_action<N + Offset>{}
  *COUNTER
#undef X


#define X(N)                       \
  , state<state_tpl<N + Offset>> + \
        boost::sml::on_exit<_> / exit_action<N + Offset> {}
    COUNTER
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
#define X(N) first_sm.process_event(state_transition_event<N>{});
    COUNTER
#undef X
  }

  counter += ctx.counter;
  ctx.counter = 0;

  // Second FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
  first_sm.process_event(state_transition_event<N + Fsm::sub_fsm::offset>{});
    COUNTER
#undef X
  }

  counter += ctx.counter;
  ctx.counter = 0;

  // Third FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N)              \
  first_sm.process_event( \
      state_transition_event<N + Fsm::sub_fsm::sub_fsm::offset>{});
    COUNTER
#undef X
  }

  counter += ctx.counter;
  ctx.counter = 0;

  first_sm.process_event(exit_sub_fsm_event{});
  first_sm.process_event(exit_sub_fsm_event{});

  return counter;
}

template <typename Fsm>
void test_fsm() {
  constexpr auto main_loop_size = 1000;

  auto counter = 0;

  for (auto i = 0; i < main_loop_size; ++i) {
    counter += run_fsm<Fsm>();
  }

  constexpr auto expected_counter =
      test_loop_size * main_loop_size * PROBLEM_SIZE * 3;

//   std::cout << counter << std::endl;
//   std::cout << expected_counter << std::endl;

  assert(counter == expected_counter);
}

using fsm2 = fsm_<PROBLEM_SIZE / 2>;
using fsm1 = fsm_<PROBLEM_SIZE / 3, fsm2>;
using fsm0 = fsm_<0, fsm1>;

int main() {
  test_fsm<fsm0>();

  return 0;
}