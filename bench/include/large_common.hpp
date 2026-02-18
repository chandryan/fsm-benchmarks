#pragma once

#include "common.hpp"

int test();

int main() // NOLINT
{
    auto counter = 0;

    for (auto i = 0; i < test_loop_size; ++i)
    {
        counter += test();
    }

    volatile bool success = (counter == 65000000);
    return (success ? 0 : 1);
}
