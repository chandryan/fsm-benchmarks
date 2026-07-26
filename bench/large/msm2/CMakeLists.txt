add_benchmark_executable(large_msm2 Normal.cpp)

add_benchmark_executable(large_msm2_favor_compile_time FavorCompileTime.cpp)

add_custom_target(benchmarks_large_msm2)
add_dependencies(benchmarks_large_msm2
    large_msm2
    large_msm2_favor_compile_time
)
