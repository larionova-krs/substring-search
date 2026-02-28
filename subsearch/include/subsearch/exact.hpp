#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Match.hpp"

namespace subsearch::exact {

std::vector<Match> rabinKarp(const std::string& s, const std::string& pattern);
std::vector<Match> rabinKarpNoChecking(const std::string& s, const std::string& pattern);

class BoyerMoore {
    friend class BoyerMooreTestApi;

private:
    static void buildBadCharTable(const std::string& pattern, std::unordered_map<char, int>& badChar);
    static void computeSuffixes(const std::string& pat, std::vector<int>& suff);
    static void buildGoodSuffTable(const std::string& pattern, std::vector<int>& goodSuff);

public:
    static std::vector<Match> searchBoyerMooreHorspool(const std::string& s, const std::string& pattern);
    static std::vector<Match> searchBoyerMoore(const std::string& s, const std::string& pattern);
};

}  // namespace subsearch::exact