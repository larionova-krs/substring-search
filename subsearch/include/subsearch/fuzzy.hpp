#pragma once
#include <string>
#include <vector>

#include "Match.hpp"

namespace subsearch::fuzzy {

std::vector<Match> naiveFuzzySearch(const std::string& text, const std::string& pattern, double threshold = 70.0);

int wagnerFischerLevenshteinDist(const std::string& s1, const std::string& s2, int insertCost = 1, int deleteCost = 1, int replaceCost = 1);

}  // namespace subsearch::fuzzy