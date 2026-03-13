#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdint>
#include <subsearch/fuzzy.hpp>

namespace subsearch::fuzzy {

int wagnerFischerComputeDist(const std::string& s1, const std::string& s2, int insertCost, int deleteCost, int replaceCost) {
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    // dp[0][0] = 0;

    for (int j = 1; j <= n; ++j) {
        dp[0][j] = dp[0][j - 1] + insertCost;
    }
    for (int i = 1; i <= m; ++i) {
        dp[i][0] = dp[i - 1][0] + deleteCost;

        for (int j = 1; j <= n; ++j) {
            dp[i][j] = (s1[i - 1] == s2[j - 1]) ? dp[i - 1][j - 1] : std::min(dp[i - 1][j] + deleteCost, std::min(dp[i][j - 1] + insertCost, dp[i - 1][j - 1] + replaceCost));
        }
    }
    return dp[m][n];
}

void Sellers::fillMatches(std::vector<Match>& matches, const std::string& text, const std::string& pattern, const std::vector<std::vector<int>>& dp, int threshold, int insertCost, int deleteCost, int replaceCost) {
    int m = pattern.size();
    int n = text.size();

    for (int j_end = 1; j_end <= n; ++j_end) {
        if (dp[m][j_end] > threshold)
            continue;

        int i = m, j = j_end;
        while (i > 0) {
            if (j > 0) {
                if (dp[i][j] == dp[i - 1][j - 1] || dp[i][j] == dp[i - 1][j - 1] + replaceCost) {
                    --i, --j;
                } else if (dp[i][j] == dp[i - 1][j] + deleteCost) {
                    --i;
                } else {
                    --j;
                }
            } else {
                --i;
            }
        }

        int matchLen = j_end - j;
        int dist = dp[m][j_end];
        int maxDist = m * deleteCost + matchLen * insertCost;
        double similarity = (1.0 - static_cast<double>(dist) / maxDist) * 100.0;
        matches.emplace_back(j, matchLen, &text, similarity);
    }
}

std::vector<Match> Sellers::searchSellers(const std::string& text, const std::string& pattern, int threshold, int insertCost, int deleteCost, int replaceCost) {
    std::vector<Match> matches;
    int m = pattern.size();
    int n = text.size();

    if (m == 0)
        return matches;

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    for (int i = 1; i <= m; ++i) {
        dp[i][0] = dp[i - 1][0] + deleteCost;

        for (int j = 1; j <= n; ++j) {
            dp[i][j] = (pattern[i - 1] == text[j - 1]) ? dp[i - 1][j - 1] : std::min(dp[i - 1][j] + deleteCost, std::min(dp[i][j - 1] + insertCost, dp[i - 1][j - 1] + replaceCost));
        }
    }

    fillMatches(matches, text, pattern, dp, threshold, insertCost, deleteCost, replaceCost);

    return matches;
}

void WuManber::addMatch(int pos, const std::vector<uint64_t>& R, int m, const std::string& text, std::vector<Match>& matches) {
    uint64_t success_bit = (1ULL << (64 - m));
    int d = 0;
    while (!(R[d] & success_bit)) {
        ++d;
    }
    int start = std::max(pos - m + 1, 0);
    double similarity = (1.0 - static_cast<double>(d) / m) * 100.0;
    matches.emplace_back(start, m, &text, similarity);
}

std::vector<Match> WuManber::searchWuManber(const std::string& text, const std::string& pattern, int k) {
    std::vector<Match> matches;
    int m = static_cast<int>(pattern.size());

    if (m == 0) {
        return matches;
    }
    if (m > 64) {
        throw std::runtime_error("Pattern length exceeds 64, not supported");
    }

    uint64_t mask[UCHAR_MAX + 1] = {0};
    for (int i = 0; i < m; ++i) {
        mask[pattern[i]] |= (1ULL << (63 - i));
    }

    const uint64_t HIGH_BIT = (1ULL << 63);

    std::vector<uint64_t> R(k + 1, 0);

    for (size_t j = 0; j < text.size(); ++j) {
        unsigned char c = text[j];

        std::vector<uint64_t> R_old = R;

        R[0] = ((R_old[0] >> 1) | HIGH_BIT) & mask[c];

        for (int d = 1; d <= k; ++d) {
            // 1) Match – продолжаем существующие префиксы
            uint64_t match = ((R_old[d] >> 1) | HIGH_BIT) & mask[c];
            // 2) Substitution – состояние с d-1 ошибками до обработки
            uint64_t sub = ((R_old[d - 1] >> 1) | HIGH_BIT);
            // 3) Deletion – новое состояние с d-1 ошибками
            uint64_t del = ((R[d - 1] >> 1) | HIGH_BIT);
            // 4) Insertion – состояние с d-1 ошибками без сдвига
            uint64_t ins = R_old[d - 1];
            
            R[d] = match | sub | ins | del;
        }

        uint64_t success_bit = (1ULL << (64 - m));
        if ((R[k] & success_bit)) {
            addMatch(j, R, m, text, matches);
        }
    }

    return matches;
}

}  // namespace subsearch::fuzzy