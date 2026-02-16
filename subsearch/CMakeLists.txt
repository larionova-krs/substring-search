add_library(subsearch STATIC
    src/exact.cpp
    src/fuzzy.cpp
    src/Match.cpp
)

target_include_directories(subsearch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)