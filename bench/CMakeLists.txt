include_directories(include)

add_subdirectory(header_inclusion)
add_subdirectory(large)
add_subdirectory(large_hierarchical)

add_custom_target(benchmarks_backmp11)
add_dependencies(benchmarks_backmp11
    benchmarks_header_inclusion_backmp11
    benchmarks_large_backmp11
    benchmarks_large_hierarchical_backmp11
)

add_custom_target(benchmarks_msm2)
add_dependencies(benchmarks_msm2
    benchmarks_header_inclusion_msm2
    benchmarks_large_msm2
    benchmarks_large_hierarchical_msm2
)

add_custom_target(benchmarks_sml)
add_dependencies(benchmarks_sml
    header_inclusion_sml
    large_sml
    large_hierarchical_sml
)
