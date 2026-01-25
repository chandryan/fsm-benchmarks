add_benchmark_executable(large_msm_back Back.cpp)
add_test(large_msm_back large_msm_back)

add_benchmark_executable(large_msm_back_favor_compile_time BackFavorCompileTime.cpp)
add_test(large_msm_back_favor_compile_time large_msm_back_favor_compile_time)

add_benchmark_executable(large_msm_back11 Back11.cpp)
add_test(large_msm_back11 large_msm_back11)

add_benchmark_executable(large_msm_backmp11 Backmp11.cpp)
add_test(large_msm_backmp11 large_msm_backmp11)

add_benchmark_executable(large_msm_backmp11_favor_compile_time Backmp11FavorCompileTime.cpp)
add_test(large_msm_backmp11_favor_compile_time large_msm_backmp11_favor_compile_time)

add_custom_target(benchmarks_large_backmp11)
add_dependencies(benchmarks_large_backmp11
    large_msm_backmp11
    large_msm_backmp11_favor_compile_time
)

# add_custom_target(benchmarks_large_back)
# add_dependencies(benchmarks_large_back
#     bench_back
#     bench_back_favor_compile_time
# )
