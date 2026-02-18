#pragma once

#include "common.hpp"
#include <boost/sml.hpp>
#include <queue>

#define ENTRY_ACTION_ROW(N) state<state_tpl<N>> + boost::sml::on_entry<_> / entry_action<N> {}
#define TRANSITION_ROW(N) state<state_tpl<N>> + event<state_transition_event<N>>[guard<N>{}] / state_transition_action<N>{} = state<state_tpl<(N + 1) % 25>>
#define INTERNAL_TRANSITION_ROW(N) state<state_tpl<N>> + event<internal_transition_event> / internal_transition_action<N>{}

struct context {
  int transition_counter = 0;
  int internal_transition_counter = 0;
  int enqueued_internal_transition_counter = 0;
  int entry_action_counter = 0;
};

template <int Index>
struct state_tpl {};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct state_transition_action {
  void operator()(const state_transition_event<Index>& evt, 
                  boost::sml::back::process<internal_transition_event> process,
                  context& ctx) {
    ctx.transition_counter += evt.two / 2;
    if constexpr ((Index % 5) == 0)
    {
        process(internal_transition_event{true});
    }
  }
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct internal_transition_action {
  void operator()(const internal_transition_event& evt, context& ctx) {
    if (evt.enqueued)
    {
        ctx.enqueued_internal_transition_counter += evt.two / 2;
    }
    else
    {
        ctx.internal_transition_counter += evt.two / 2;
    }
  }
};

// Note: Using a constexpr lambda makes the build slightly slower (at least on
// GCC)
template <int Index>
struct entry_action {
  void operator()(const boost::sml::initial&, context&)
  {
  }

  void operator()(const auto& event, context& ctx) {
    // Limitation:
    // Cannot resolve the event type.
    const auto& real_event = reinterpret_cast<const state_transition_event<0>&>(event);
    ctx.entry_action_counter += real_event.two / 2;
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
