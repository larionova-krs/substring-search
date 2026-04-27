#include <benchmark/benchmark.h>

#include <subsearch/fuzzy.hpp>
#include <vector>

#include "common.hpp"

namespace fuzzy = subsearch::fuzzy;

struct FuzzyFixture {
    std::string text;
    std::string pattern;
    int k;
    FuzzyFixture(std::string t, std::string p, int k_) : text(std::move(t)), pattern(std::move(p)), k(k_) {}
};

static void FuzzyCustomArgs_K(benchmark::internal::Benchmark* b) {
    const std::vector<int64_t> text_lens = {1 << 10, 1 << 14, 1 << 18};
    const std::vector<int64_t> pat_lens = {10, 30, 100};
    const std::vector<int64_t> ks = {0, 1, 2, 3, 5};
    for (auto tl : text_lens)
        for (auto pl : pat_lens)
            for (auto k : ks)
                b->Args({tl, pl, k});
}

#define REGISTER_FUZZY(Algorithm)                                                                   \
    // BENCHMARK_TEMPLATE(BM_Fuzzy_K, Algorithm)->Apply(FuzzyCustomArgs_K)->Complexity();              
    
REGISTER_FUZZY(fuzzy::Sellers::searchSellers);
REGISTER_FUZZY(fuzzy::WuManber::searchWuManber);

#undef REGISTER_FUZZY
