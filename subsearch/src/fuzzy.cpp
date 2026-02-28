#include <algorithm>
#include <cctype>
#include <subsearch/fuzzy.hpp>

namespace subsearch::fuzzy {

std::vector<Match> naiveFuzzySearch(const std::string& s, const std::string& pattern, double threshold) {
    std::vector<Match> matches;
    int n = s.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; ++i) {
        int matchesCount = 0;
        for (int j = 0; j < m; ++j) {
            if (tolower(static_cast<unsigned char>(s[i + j])) ==
                tolower(static_cast<unsigned char>(pattern[j])))
                ++matchesCount;
        }
        double similarity = (matchesCount * 100.0) / m;
        if (similarity >= threshold) {
            matches.push_back(Match(i, m, &s, similarity));
        }
    }
    std::sort(matches.begin(), matches.end());
    return matches;
}

}  // namespace subsearch::fuzzy