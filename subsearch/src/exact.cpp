#include <cctype>
#include <subsearch/exact.hpp>
#include <unordered_map>

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

void BoyerMoore::buildBadCharTable(const std::string& pattern, int m,
                                   std::unordered_map<char, int>& badChar) {
    for (int i = 0; i < m - 1; ++i) {
        char c = tolower(static_cast<unsigned char>(pattern[i]));
        badChar[c] = m - 1 - i;
    }
}

void BoyerMoore::computeSuffixes(const std::string& pat, int m, std::vector<int>& suff) {
    // Note: processed from right to left
    int checkedBound = m - 1;
    int segmentStart = m - 1;

    suff.resize(m);
    suff[m - 1] = m;

    for (int i = m - 2; i >= 0; --i) {
        int twinPos = m - 1 - (segmentStart - i);
        int boundOutrun = i - checkedBound;

        if (boundOutrun > 0 && suff[twinPos] < boundOutrun) {
            suff[i] = suff[twinPos];
        } else {
            if (i < checkedBound)
                checkedBound = i;
            segmentStart = i;
            while (checkedBound >= 0 &&
                   std::tolower(static_cast<unsigned char>(pat[checkedBound])) ==
                       std::tolower(static_cast<unsigned char>(pat[m - 1 - (segmentStart - checkedBound)]))) {
                --checkedBound;
            }
            suff[i] = segmentStart - checkedBound;
        }
    }
}

void BoyerMoore::buildGoodSuffTable(const std::string& pat, int m, std::vector<int>& goodSuff) {
    std::vector<int> suff;
    computeSuffixes(pat, m, suff);

    goodSuff.assign(m, m);
    for (int i = m - 1, j = 0; i >= 0; --i) {
        bool suffixExtendsToStart = (suff[i] == i + 1);
        if (suffixExtendsToStart) {
            for (; j < m - 1 - i; ++j) {
                if (goodSuff[j] == m)
                    goodSuff[j] = m - 1 - i;
            }
        }
    }
    for (int i = 0; i < m - 1; ++i) {
        int mismatchPos = m - 1 - suff[i];
        goodSuff[mismatchPos] = m - 1 - i;
    }
}

std::vector<Match> BoyerMoore::searchBoyerMooreHorspool(
    const std::string& text, const std::string& pattern) {
    std::vector<Match> matches;
    int n = text.length();
    int m = pattern.length();

    if (m > n || m == 0)
        return matches;

    std::unordered_map<char, int> badChar;
    buildBadCharTable(pattern, m, badChar);

    for (int i = m - 1; i < n;) {
        int iT = i;
        int iP = m - 1;

        while (iP >= 0 &&
               std::tolower(static_cast<unsigned char>(text[iT])) ==
                   std::tolower(static_cast<unsigned char>(pattern[iP]))) {
            --iT, --iP;
        }

        if (iP < 0) {
            matches.push_back({i - m + 1, m, &text});
        }

        char ch = std::tolower(static_cast<unsigned char>(text[i]));
        auto bcIt = badChar.find(ch);
        i += (bcIt != badChar.end()) ? bcIt->second : m;
    }
    return matches;
}

std::vector<Match> BoyerMoore::searchBoyerMoore(const std::string& text,
                                                const std::string& pattern) {
    std::vector<Match> matches;
    int n = text.length();
    int m = pattern.length();

    if (m > n || m == 0)
        return matches;

    std::unordered_map<char, int> badChar;
    std::vector<int> goodSuff;
    buildBadCharTable(pattern, m, badChar);
    buildGoodSuffTable(pattern, m, goodSuff);

    for (int j = 0; j <= n - m;) {
        int i = m - 1;

        while (i >= 0 &&
               std::tolower(static_cast<unsigned char>(text[j + i])) ==
                   std::tolower(static_cast<unsigned char>(pattern[i]))) {
            --i;
        }

        if (i < 0) {
            matches.push_back({j, m, &text});
            j += goodSuff[0];
        } else {
            char ch = std::tolower(static_cast<unsigned char>(text[j + i]));
            auto bcIt = badChar.find(ch);
            int bcShift = i - ((bcIt != badChar.end()) ? bcIt->second : -1);
            j += std::max(bcShift, goodSuff[i]);
        }
    }
    return matches;
}

}  // namespace subsearch::exact