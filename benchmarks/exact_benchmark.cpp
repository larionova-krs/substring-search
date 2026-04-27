#include <benchmark/benchmark.h>

#include <subsearch/exact.hpp>
#include <vector>

#include "common.hpp"

namespace exact = subsearch::exact;

struct SearchFixture {
    std::string text;
    std::string pattern;
    SearchFixture(std::string t, std::string p) : text(std::move(t)), pattern(std::move(p)) {}
};

template <auto Algorithm>
static void BM_RandomText(benchmark::State& state) {
    const size_t text_len = state.range(0);
    const size_t pat_len = state.range(1);
    std::string text = bm_common::random_string(text_len);
    std::string pattern = text.substr((text_len - pat_len) / 2, pat_len);
    SearchFixture fix(std::move(text), std::move(pattern));

    std::vector<subsearch::Match> matches;
    for (auto _ : state) {
        state.PauseTiming();
        matches.clear();
        matches.shrink_to_fit();
        state.ResumeTiming();

        matches = Algorithm(fix.text, fix.pattern);
        benchmark::DoNotOptimize(matches.data());
    }
    state.SetComplexityN(text_len);
    state.SetLabel(std::to_string(pat_len));
}

template <auto Algorithm>
static void BM_DNAText(benchmark::State& state) {
    const size_t text_len = state.range(0);
    const size_t pat_len = state.range(1);
    std::string text = bm_common::random_dna(text_len, 42);
    std::string pattern = text.substr((text_len - pat_len) / 2, pat_len);
    SearchFixture fix(std::move(text), std::move(pattern));

    std::vector<subsearch::Match> matches;
    for (auto _ : state) {
        state.PauseTiming();
        matches.clear();
        matches.shrink_to_fit();
        state.ResumeTiming();

        matches = Algorithm(fix.text, fix.pattern);
        benchmark::DoNotOptimize(matches.data());
    }
    state.SetComplexityN(text_len);
    state.SetLabel(std::to_string(pat_len));
}

static void CustomArguments(benchmark::internal::Benchmark* b) {
    const std::vector<int64_t> text_lengths = {1 << 10, 1 << 12, 1 << 14, 1 << 16, 1 << 18, 1 << 20};
    const std::vector<int64_t> pattern_lengths = {5, 20, 100, 1000};
    for (auto tl : text_lengths)
        for (auto pl : pattern_lengths)
            b->Args({tl, pl});
}

#define REGISTER_EXACT(Algorithm)                                                       \
    BENCHMARK_TEMPLATE(BM_RandomText, Algorithm)->Apply(CustomArguments)->Complexity(); \
    BENCHMARK_TEMPLATE(BM_DNAText, Algorithm)->Apply(CustomArguments)->Complexity();

REGISTER_EXACT(exact::rabinKarp);
REGISTER_EXACT(exact::rabinKarpNoChecking);
REGISTER_EXACT(exact::BoyerMoore::searchBoyerMooreHorspool);
REGISTER_EXACT(exact::BoyerMoore::searchBoyerMoore);
REGISTER_EXACT(exact::KMP::searchMP);
REGISTER_EXACT(exact::KMP::searchKMP);

#undef REGISTER_EXACT
