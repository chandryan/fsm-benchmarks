#pragma once

#include "common.hpp"

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;

#define TRANSITION_ROW(N) Row<state_tpl<N>, state_transition_event<N>, state_tpl<(N + 1) % 25>, state_transition_action<N>, guard<N>>
#define INTERNAL_TRANSITION_ROW(N) Row<state_tpl<N>, internal_transition_event, none, internal_transition_action<N>>

struct starting {};

template<int Index, size_t Offset = 0>
struct state_tpl: msm::front::state<>
{
    template <typename Fsm>
    void on_entry(const starting&, Fsm&)
    {
    }

    template <typename Event, typename Fsm>
    void on_entry(const Event& event, Fsm& fsm)
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
            // fsm.enqueue_event(internal_transition_event{true});
            fsm.process_event(internal_transition_event{true});
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
