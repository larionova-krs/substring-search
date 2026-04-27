#include "common.hpp"

namespace bm_common {

std::string random_string(size_t length, uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist('a', 'z');
    std::string s(length, '\0');
    for (auto& c : s) c = static_cast<char>(dist(rng));
    return s;
}

std::string random_dna(size_t length, uint64_t seed) {
    static const char dna[] = "ACGT";
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, 3);
    std::string s(length, '\0');
    for (auto& c : s) c = dna[dist(rng)];
    return s;
}

}  // namespace bm_common