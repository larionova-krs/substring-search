#include <cctype>
#include <unordered_map>
#include <subsearch/exact.hpp>

namespace subsearch::exact {

std::vector<Match> rabinKarp(const std::string& s, const std::string& pattern) {
    std::vector<Match> matches;
    int n = s.length();
    int m = pattern.length();
    if (m > n)
        return matches;

    const int x = 257;
    unsigned long long patternHash = 0, sHash = 0;
    unsigned long long h = 1;
    for (int i = 0; i < m - 1; ++i) {
        h = h * x;
    }

    for (int i = 0; i < m; ++i) {
        patternHash =
            x * patternHash + tolower(static_cast<unsigned char>(pattern[i]));
        sHash = x * sHash + tolower(static_cast<unsigned char>(s[i]));
    }

    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == sHash) {
            bool match = true;
            for (int j = 0; j < m && match; ++j) {
                match = (tolower(static_cast<unsigned char>(s[i + j])) ==
                         tolower(static_cast<unsigned char>(pattern[j])));
            }
            if (match) {
                matches.push_back(Match(i, m, &s));
            }
        }
        sHash = x * (sHash - tolower(static_cast<unsigned char>(s[i])) * h) +
                tolower(static_cast<unsigned char>(s[i + m]));
    }
    return matches;
}

std::vector<Match> rabinKarpNoChecking(const std::string& s,
                                       const std::string& pattern) {
    std::vector<Match> matches;
    int n = s.length();
    int m = pattern.length();
    if (m > n)
        return matches;

    const int x = 256;
    unsigned long long patternHash = 0, sHash = 0;
    unsigned long long h = 1;
    for (int i = 0; i < m - 1; ++i) {
        h = h * x;
    }

    for (int i = 0; i < m; ++i) {
        patternHash =
            x * patternHash + tolower(static_cast<unsigned char>(pattern[i]));
        sHash = x * sHash + tolower(static_cast<unsigned char>(s[i]));
    }

    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == sHash) {
            matches.push_back(Match(i, m, &s));
        }
        sHash = x * (sHash - tolower(static_cast<unsigned char>(s[i])) * h) +
                tolower(static_cast<unsigned char>(s[i + m]));
    }
    return matches;
}

std::vector<Match> boyerMooreHorspool(const std::string& s,
                                      const std::string& pattern) {
    std::vector<Match> matches;
    int n = s.length();
    int m = pattern.length();
    if (m > n || pattern == "")
        return matches;

    std::unordered_map<char, int> charsLastOccuranceInPattern;
    for (int i = 0; i < m; ++i) {
        charsLastOccuranceInPattern[tolower(static_cast<unsigned char>(pattern[i]))] = i;
    }
    for (int i = m - 1; i < n; ++i) {
        bool isOk = true;
        for (int j = m - 1; j >= 0; --j) {
            if (tolower(static_cast<unsigned char>(pattern[j])) !=
                tolower(static_cast<unsigned char>(s[i - (m - 1 - j)]))) {
                isOk = false;
                if (!charsLastOccuranceInPattern.contains(tolower(
                        static_cast<unsigned char>(s[i - (m - 1 - j)])))) {
                    i += j;
                } else {
                    i += m -
                         charsLastOccuranceInPattern.at(tolower(
                             static_cast<unsigned char>(s[i - (m - 1 - j)]))) - 2;
                }
                break;
            }
        }
        if (isOk) {
            matches.push_back(Match(i - m + 1, m, &s));
        }
    }
    return matches;
}

// std::vector<Match> boyerMoore(const std::string& s,
//                                       const std::string& pattern) {
//     std::vector<Match> matches;
//     int n = s.length();
//     int m = pattern.length();
//     if (m > n)
//         return matches;

//     return matches;
// }

}  // namespace subsearch::exact