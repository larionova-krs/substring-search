#include <cctype>
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

}  // namespace subsearch::exact