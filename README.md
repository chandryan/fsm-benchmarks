# fsm-benchmarks

This repo contains benchmarks for C++ state machine libraries.

It has been originally created to test various optimizations of the [Boost MSM](https://github.com/boostorg/msm) library.


## Benchmark scenarios

The benchmarks are inspired by the [fsm-benchmark](https://github.com/fgoujeon/fsm-benchmark) repository.


**Large state machine:**

Same benchmark as tested in [fsm-benchmark](https://github.com/fgoujeon/fsm-benchmark).


**Large hierarchical state machine:**

This benchmarks is an extension of the large state machine and consists of a hierarchical structure of 3 state machines:

- fsm0 is the root state machine
- fsm1 is contained in fsm0
- fsm2 is contained in fsm1

The events handled by each state machine overlap with each other to simulate transition conflicts.


## Tested libraries

The following libraries are benchmarked:

**MSM:**

MSM is used from a [fork](https://github.com/chandryan/msm-ng) with a C++11-optimized backend called `backmp11`).
For more details about the applied optimizations take a look at the [backend's README](https://github.com/chandryan/msm-ng/tree/develop/include/boost/msm/backmp11#readme).

Only the functor frontend is used in the benchmarks. The following backend configurations are tested:
  - `back` backend with `favor_runtime_speed` and `favor_compile_time` policy
  - `back11` backend
  - `backmp11` backend with `favor_runtime_speed` and `favor_compile_time` policy

**SML:**

[SML](https://github.com/boost-ext/sml) is tested with a default configuration.


## Benchmark results

The benchmark results will be updated every once in a while in case one of the libraries has performance-relevant updates for the benchmarks.

Following results are from a benchmark run from *2025-04-15* with the following HW & SW:

- Intel i7-6700K @4GHZ, 32 GB RAM
- Clang 19.1.4, RelWithDebInfo, C++20
- CUs are compiled one-by-one except the multi_cu benchmark for MSM with `backmp11` & `favor_compile_time`


### Large state machine

|                             | Compile / sec | RAM / MB | Runtime / sec  |
|-----------------------------|---------------|----------|----------------|
| back                        | 17            | 954      | 7              |
| back_favor_compile_time     | 21            | 1000     | 7              |
| back11                      | 39            | 2794     | 7              |
| backmp11                    | 5             | 332      | 3              |
| backmp11_favor_compile_time | 4             | 282      | 13             |
| sml                         | 11            | 362      | 3              |

### Large hierarchical state machine

|                                      | Compile / sec | RAM / MB | Runtime / sec  |
|--------------------------------------|---------------|----------|----------------|
| back                                 | 62            | 2849     | 22             |
| back_favor_compile_time              | 74            | 2552     | 267            |
| backmp11                             | 15            | 627      | 10             |
| backmp11_favor_compile_time          | 8             | 405      | 42             |
| backmp11_favor_compile_time_multi_cu | 6             | ~1237    | 42             |
| sml                                  | 43            | 1121     | 11             |