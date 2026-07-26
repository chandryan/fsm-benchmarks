add_subdirectory(msm)
add_subdirectory(msm2)
add_subdirectory(sml)

add_custom_target(benchmarks_header_inclusion)
add_dependencies(benchmarks_header_inclusion
    header_inclusion_msm_back
    header_inclusion_msm_back_favor_compile_time
    header_inclusion_msm_back11
    header_inclusion_msm_backmp11
    header_inclusion_msm_backmp11_favor_compile_time
    header_inclusion_msm2
    header_inclusion_msm2_favor_compile_time
    header_inclusion_sml
)
