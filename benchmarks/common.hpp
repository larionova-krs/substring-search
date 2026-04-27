#pragma once
#include <string>
#include <vector>
#include <random>
#include <cstdint>
#include <cassert>
#include <algorithm>

namespace bm_common {

inline constexpr uint64_t defaultSeed = 42;

std::string random_string(size_t length, uint64_t seed = defaultSeed);

std::string random_dna(size_t length, uint64_t seed = defaultSeed);

} // namespace bm_common