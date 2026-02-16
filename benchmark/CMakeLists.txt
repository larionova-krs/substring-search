add_executable(substring_benchmark
    src/main.cpp
    src/Tester.cpp
)

target_include_directories(substring_benchmark PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(substring_benchmark PRIVATE
    subsearch
)