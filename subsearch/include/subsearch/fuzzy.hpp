#pragma once
#include <string>
#include <vector>

#include "Match.hpp"

namespace subsearch::fuzzy {

std::vector<Match> naiveFuzzySearch(const std::string& s, const std::string& pattern, double threshold = 70.0);

}  // namespace subsearch::fuzzy