add_executable(substring_benchmarks
    src/main.cpp
    src/Tester.cpp
)

target_include_directories(substring_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(substring_benchmarks PRIVATE
    subsearch
)