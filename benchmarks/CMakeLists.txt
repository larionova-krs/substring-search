find_package(benchmark REQUIRED)

add_executable(subsearch_benchmarks
    benchmark_main.cpp
    exact_benchmark.cpp
    fuzzy_benchmark.cpp
)

target_include_directories(subsearch_benchmarks PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(subsearch_benchmarks PRIVATE
    subsearch
    benchmark::benchmark
)

# enable_testing()
# add_test(NAME benchmarks
#     COMMAND subsearch_benchmarks
#         --benchmark_out=benchmarks/data/output/benchmark_results.json
#         --benchmark_out_format=json
#         --benchmark_repetitions=30
#         --benchmark_counters_tabular=true
# )
# set_tests_properties(benchmarks PROPERTIES
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     LABELS "benchmarks"
# )