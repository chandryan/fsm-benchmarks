cmake_minimum_required(VERSION 3.16)
project(fsm_benchmarks)
set(CMAKE_CXX_STANDARD 20)
# Required due to bug in clang19
# add_compile_options("-Wno-missing-template-arg-list-after-template-kw")
# add_compile_options("-ftime-trace")
# add_compile_options("-save-temps")

enable_testing()
set(CMAKE_CXX_COMPILER_LAUNCHER time -f "%e seconds, %M kB RAM")
include_directories(msm-ng/include)
include_directories(sml/include)

add_subdirectory(bench)
