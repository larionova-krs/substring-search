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

int wagnerFischerLevenshteinDist(const std::string& s1, const std::string& s2, int insertCost, int deleteCost, int replaceCost) {
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

}  // namespace subsearch::fuzzy