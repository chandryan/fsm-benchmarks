#pragma once

#include "common.hpp"

#include <boost/msm2/front/composite_state.hpp>
#include <boost/msm2/front/rows.hpp>
#include <boost/msm2/front/states.hpp>

namespace front = boost::msm2::front;
// Declared here so the alias is valid; the back-end headers (included by the
// benchmark translation units) populate it.
namespace boost::msm2::back {}
namespace back = boost::msm2::back;

#define TRANSITION_ROW(N) front::row<state_tpl<N>, state_transition_event<N>, state_tpl<(N + 1) % 25>, state_transition_action<N>, guard<N>>
#define INTERNAL_TRANSITION_ROW(N) front::row<state_tpl<N>, internal_transition_event, front::none, internal_transition_action<N>>

struct starting {};

template<int Index, size_t Offset = 0>
struct state_tpl: front::state
{
    template <typename Fsm>
    void on_entry(Fsm&, const starting&)
    {
    }

    template <typename Fsm, typename Event>
    void on_entry(Fsm& fsm, const Event& event)
    {
        if constexpr ((Index < 20) && (Index % 2 == 0))
        {
            fsm.entry_action_counter += event.two / 2;
        }
    }
};

template<int Index>
struct state_transition_action
{
    template<class Event, class Fsm, class SourceState, class TargetState>
    void operator()(const Event& evt, Fsm& fsm, SourceState&, TargetState&)
    {
        fsm.transition_counter += evt.two / 2;
        if constexpr ((Index % 5) == 0)
        {
            fsm.enqueue_event(internal_transition_event{true});
        }
    }
};

template<int Index>
struct internal_transition_action
{
    template<class Event, class Fsm, class SourceState, class TargetState>
    void operator()(const Event& evt, Fsm& sm, SourceState&, TargetState&)
    {
        if (evt.enqueued)
        {
            sm.enqueued_internal_transition_counter += evt.two / 2;
        }
        else
        {
            sm.internal_transition_counter += evt.two / 2;
        }
    }
};

template<int Index>
struct guard
{
    template<class Event, class Fsm, class SourceState, class TargetState>
    bool operator()(const Event& evt, Fsm& sm, SourceState&, TargetState&)
    {
        return evt.two >= 0;
    }
};
