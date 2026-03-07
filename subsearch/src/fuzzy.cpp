#include <algorithm>
#include <cctype>
#include <subsearch/fuzzy.hpp>

namespace subsearch::fuzzy {

std::vector<Match> naiveFuzzySearch(const std::string& text, const std::string& pattern, double threshold) {
    std::vector<Match> matches;
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; ++i) {
        int matchesCount = 0;
        for (int j = 0; j < m; ++j) {
            if (tolower(static_cast<unsigned char>(text[i + j])) ==
                tolower(static_cast<unsigned char>(pattern[j])))
                ++matchesCount;
        }
        double similarity = (matchesCount * 100.0) / m;
        if (similarity >= threshold) {
            matches.push_back(Match(i, m, &text, similarity));
        }
    }
    std::sort(matches.begin(), matches.end());
    return matches;
}

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

void fillMatches(std::vector<Match>& matches, const std::string& text, const std::string& pattern, const std::vector<std::vector<int>>& dp, int threshold, int insertCost, int deleteCost, int replaceCost) {
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

std::vector<Match> sellersSearch(const std::string& text, const std::string& pattern, int threshold, int insertCost, int deleteCost, int replaceCost) {
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

}  // namespace subsearch::fuzzy