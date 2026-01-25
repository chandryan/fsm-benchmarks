cmake_minimum_required(VERSION 3.16)
project(fsm_benchmarks)
set(CMAKE_CXX_STANDARD 20)

# add_compile_options("-ftime-trace")
# add_compile_options("-save-temps")

function(add_benchmark_executable name)
    add_executable(${name} ${ARGN})
    add_custom_command(TARGET ${name} POST_BUILD
        COMMAND size $<TARGET_FILE:${name}>
    )
endfunction()

enable_testing()
set(CMAKE_CXX_COMPILER_LAUNCHER time -f "%e seconds, %M kB RAM")

include_directories(msm-ng/include)
# include_directories(../boost/libs/msm/include)
include_directories(sml/include)

add_subdirectory(bench)
