#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Match.hpp"

namespace subsearch::exact {

std::vector<Match> rabinKarp(const std::string& s, const std::string& pattern);
std::vector<Match> rabinKarpNoChecking(const std::string& s,
                                       const std::string& pattern);

class BoyerMoore {
private:
    void buildBadCharTable(const std::string& pattern, int m,
                           std::unordered_map<char, int>& badChar);
    void buildGoodSuffTable(const std::string& pattern, int m,
                              std::vector<int>& goodSuff);

public:
    std::vector<Match> searchBoyerMooreHorspool(const std::string& s,
                                                const std::string& pattern);
};

}  // namespace subsearch::exact