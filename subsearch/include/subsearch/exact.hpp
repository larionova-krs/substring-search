#pragma once
#include <string>
#include <vector>

#include "Match.hpp"

namespace subsearch::exact {

std::vector<Match> rabinKarp(const std::string& s, const std::string& pattern);
std::vector<Match> rabinKarpNoChecking(const std::string& s,
                                       const std::string& pattern);

std::vector<Match> boyerMooreHorspool(const std::string& s,
                                      const std::string& pattern);

}  // namespace subsearch::exact