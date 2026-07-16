cmake_minimum_required(VERSION 3.16)
project(fsm_benchmarks)
set(CMAKE_CXX_STANDARD 20)

# add_compile_options("-ftime-trace")
# add_compile_options("-save-temps")

# Force DWARF-4 for perf compatibility if needed
# add_compile_options(-gdwarf-4 -fno-omit-frame-pointer)

file(WRITE "${CMAKE_BINARY_DIR}/print_size.sh"
"#!/bin/sh\nsize \"$1\" | awk -v name=\"$2\" 'NR==2{print name \": \" int(($1 + $2) / 1000 + 0.5) \" kB\"}'\n")
function(add_benchmark_executable name)
    add_executable(${name}_bin ${ARGN})
    add_custom_target(${name} ALL
        COMMAND sh "${CMAKE_BINARY_DIR}/print_size.sh" "$<TARGET_FILE:${name}_bin>" "${name}"
        DEPENDS ${name}_bin
        COMMENT "Printing size of ${name}"
    )
    add_test(${name} ${name}_bin)
endfunction()

file(WRITE "${CMAKE_BINARY_DIR}/timed_compile.sh"
"#!/bin/sh\ntmpfile=$(mktemp)\n/usr/bin/time -f \"%e seconds, %M kB RAM\" -o \"$tmpfile\" \"$@\"\nresult=$?\nawk '{printf \"%s seconds, %d MB RAM\\n\", $1, int($3/1000+0.5)}' \"$tmpfile\"\nrm \"$tmpfile\"\nexit $result\n")


enable_testing()
set(CMAKE_CXX_COMPILER_LAUNCHER sh "${CMAKE_BINARY_DIR}/timed_compile.sh")

include_directories(msm/include)
# include_directories(../msm/include)
include_directories(sml/include)

add_subdirectory(bench)

add_custom_target(benchmarks_main)
add_dependencies(benchmarks_main
    # Large SM
    large_msm_back
    large_msm_back_favor_compile_time
    large_msm_back11
    large_msm_backmp11
    large_msm_backmp11_favor_compile_time
    large_sml
    # Large hierarchical SM
    large_hierarchical_msm_back
    large_hierarchical_msm_back_favor_compile_time
    large_hierarchical_msm_backmp11
    large_hierarchical_msm_backmp11_favor_compile_time
    large_hierarchical_sml
)
