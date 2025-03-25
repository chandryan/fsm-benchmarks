// Copyright Florian Goujeon 2021.
// Distributed under the Boost Software License, Version 1.0.
//(See accompanying file LICENSE or copy at
// https://www.boost.org/LICENSE_1_0.txt)
// Official repository: https://github.com/fgoujeon/fsm-benchmark

#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
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

// template <size_t Offset, typename SubFsm>
// struct fsm_ {
//   auto operator()() const {
//     using namespace boost::sml;

//     return make_transition_table(
// #define X(N)                                                                \
//   COMMA_IF_NOT_0(N)                                                         \
//   state<state_tpl<N + Offset>> +                                            \
//       event<state_transition_event<N + Offset>>[guard<N>{}] /               \
//           state_transition_action<N>{} =                                    \
//       state<state_tpl<(N + 1) % PROBLEM_SIZE + Offset>>,                    \
//           state<state_tpl<N + Offset>> + event<internal_transition_event> / \
//                                              internal_transition_action<N>{}
//         *COUNTER
// #undef X

// #define X(N)                       \
//   , state<state_tpl<N + Offset>> + \
//         boost::sml::on_exit<_> / exit_action<N + Offset> {}
//             COUNTER
// #undef X
//         ,
//         state<state_tpl<Offset>> + event<enter_sub_fsm_event> =
//         state<SubFsm>, state<SubFsm> + event<exit_sub_fsm_event> =
//         state<state_tpl<Offset>>);
//   }
// };

template <size_t Offset>
struct fsm_<Offset, void> {
  auto operator()() const {
    using namespace boost::sml;

    return make_transition_table(
#define X(N)                                                                \
  COMMA_IF_NOT_0(N)                                                         \
  state<state_tpl<N + Offset>> +                                            \
      event<state_transition_event<N + Offset>>[guard<N>{}] /               \
          state_transition_action<N>{} =                                    \
      state<state_tpl<(N + 1) % PROBLEM_SIZE + Offset>>,                    \
          state<state_tpl<N + Offset>> + event<internal_transition_event> / \
                                             internal_transition_action<N>{}
        *COUNTER
#undef X

#define X(N)                       \
  , state<state_tpl<N + Offset>> + \
        boost::sml::on_exit<_> / exit_action<N + Offset> {}
            COUNTER
#undef X
    );
  }
};

template <typename Fsm>
int run_fsm() {
  auto ctx = context{};
  auto first_sm = Fsm{ctx};
  auto& second_sm = first_sm.template get_state<typename Fsm::sub_fsm&>();
  auto& third_sm =
      second_sm.template get_state<typename Fsm::sub_fsm::sub_fsm&>();

  first_sm.start();

  // First FSM
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) first_sm.process_event(state_transition_event<N>{});
    COUNTER
#undef X
  }

  // Second FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N) \
  first_sm.process_event(state_transition_event<N + Fsm::sub_fsm::offset>{});
    COUNTER
#undef X
  }

  // Third FSM
  first_sm.process_event(enter_sub_fsm_event{});
  for (auto i = 0; i < test_loop_size; ++i) {
#define X(N)              \
  first_sm.process_event( \
      state_transition_event<N + Fsm::sub_fsm::sub_fsm::offset>{});
    COUNTER
#undef X
  }

  auto sum = first_sm.counter + second_sm.counter + third_sm.counter;

  first_sm.process_event(exit_sub_fsm_event{});
  first_sm.process_event(exit_sub_fsm_event{});

  return sum;
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

  std::cout << counter << std::endl;
  std::cout << expected_counter << std::endl;

  assert(counter == expected_counter);
}

using fsm2 = boost::sml::sm<fsm_<1>, boost::sml::process_queue<std::queue>>;

int main() {
  // test_fsm<fsm2>();

  auto ctx = context{};
  fsm2 machine{ctx};

  return 0;
}