add_subdirectory(large)
add_subdirectory(large_hierarchical)

add_custom_target(benchmarks_backmp11)
add_dependencies(benchmarks_backmp11
    benchmarks_large_backmp11
    benchmarks_large_hierarchical_backmp11
)