#pragma once
#include <string>
#include <vector>

#include "Match.hpp"

namespace subsearch::fuzzy {

int wagnerFischerComputeDist(const std::string& s1, const std::string& s2, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);

class Sellers {
private:
    static void fillMatches(std::vector<Match>& matches, const std::string& text, const std::string& pattern, const std::vector<std::vector<int>>& dp, int threshold = 0, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);

public:
    static std::vector<Match> searchSellers(const std::string& text, const std::string& pattern, int threshold = 0, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);
};

class WuManber {
private:
    // Filled Match len is not quite correct, approximate value used
    static void addMatch(int pos, const std::vector<uint64_t>& R, int m, const std::string& text, std::vector<Match>& matches);

public:
    static std::vector<Match> searchWuManber(const std::string& text, const std::string& pattern, int k = 0);
};

}  // namespace subsearch::fuzzy