#pragma once

#include "common.hpp"

struct enter_sub_fsm_event {
  int two = 2;
};

struct exit_sub_fsm_event {
  int two = 2;
};

int test();


#ifndef EXCLUDE_MAIN
int main() // NOLINT
{
    auto counter = 0;

    for (auto i = 0; i < test_loop_size; ++i)
    {
        counter += test();
    }

    volatile bool success = (counter == 195004000);
    return (success ? 0 : 1);
}
#endif
