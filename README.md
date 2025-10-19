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

MSM is used with 3 back-ends `back`, `back11` and `backmp11`.
The back-end `backmp11` is a recent addition. For more details about the applied optimizations and changes to previous back-ends take a look at the [backmp11's README](https://github.com/boostorg/msm/blob/develop/include/boost/msm/backmp11/README.md).

Only the functor frontend is used in the benchmarks. The following backend configurations are tested:
  - `back` backend with `favor_runtime_speed` and `favor_compile_time` compile policies
  - `back11` backend
  - `backmp11` backend with `favor_runtime_speed` and `favor_compile_time` compile policies

Last run was done on commit f5fc37e84d18dc4788bfefb345aec9b3eb40929f.


**SML:**

[SML](https://github.com/boost-ext/sml) is tested with a default configuration.

Last run was done on commit 0bdc4f593a3a1cc5bb45e0464c0923683d099d86.


## Benchmark results

The benchmark results will be updated every once in a while in case one of the libraries has performance-relevant updates for the benchmarks.

Following results are from a benchmark run from *2025-10-19* with the following HW & SW:

- Intel i7-6700K @4GHZ, 32 GB RAM
- Clang 19.1.4, RelWithDebInfo, C++20
- CUs are compiled one-by-one except the multi_cu benchmark for MSM with `backmp11` & `favor_compile_time`


### Large state machine

|                             | Compile / sec | RAM / MB | Runtime / sec  |
|-----------------------------|---------------|----------|----------------|
| back                        | 18            | 953      | 7              |
| back_favor_compile_time     | 21            | 1000     | 8              |
| back11                      | 43            | 2794     | 7              |
| backmp11                    | 4             | 261      | 3              |
| backmp11_favor_compile_time | 3             | 231      | 13             |
| sml                         | 12            | 363      | 3              |

### Large hierarchical state machine

|                                      | Compile / sec | RAM / MB | Runtime / sec  |
|--------------------------------------|---------------|----------|----------------|
| back                                 | 68            | 2849     | 23             |
| back_favor_compile_time              | 80            | 2551     | 261            |
| backmp11                             | 12            | 525      | 10             |
| backmp11_favor_compile_time          | 7             | 318      | 40             |
| backmp11_favor_compile_time_multi_cu | 5             | ~982     | 40             |
| sml                                  | 48            | 1128     | 11             |