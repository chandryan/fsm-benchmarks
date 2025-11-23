add_library(header_inclusion_msm_back Back.cpp)

add_library(header_inclusion_msm_back_favor_compile_time BackFavorCompileTime.cpp)

add_library(header_inclusion_msm_back11 Back11.cpp)

add_library(header_inclusion_msm_backmp11 Backmp11.cpp)

add_library(header_inclusion_msm_backmp11_favor_compile_time Backmp11FavorCompileTime.cpp)

add_custom_target(benchmarks_header_inclusion_backmp11)
add_dependencies(benchmarks_header_inclusion_backmp11
    header_inclusion_msm_backmp11
    header_inclusion_msm_backmp11_favor_compile_time
)

# add_custom_target(benchmarks_header_inclusion_back)
# add_dependencies(benchmarks_header_inclusion_back
#     bench_back
#     bench_back_favor_compile_time
# )
