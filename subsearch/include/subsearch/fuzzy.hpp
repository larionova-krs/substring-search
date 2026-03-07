#pragma once
#include <string>
#include <vector>

#include "Match.hpp"

namespace subsearch::fuzzy {

std::vector<Match> naiveFuzzySearch(const std::string& text, const std::string& pattern, double threshold = 70.0);

int wagnerFischerComputeDist(const std::string& s1, const std::string& s2, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);

class Sellers {
private:
    static void fillMatches(std::vector<Match>& matches, const std::string& text, const std::string& pattern, const std::vector<std::vector<int>>& dp, int threshold = 0, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);

public:
    static std::vector<Match> searchSellers(const std::string& text, const std::string& pattern, int threshold = 0, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);
};

}  // namespace subsearch::fuzzy