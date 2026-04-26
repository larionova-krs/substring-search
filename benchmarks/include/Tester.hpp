#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <subsearch/subsearch.hpp>
#include <vector>

class Tester {
private:
    std::string text;
    std::ostream* outputStream;

public:
    Tester(std::ostream& output = std::cout);

    void loadTextFromFile(const std::string& filename);

    template <typename Func>
    void testAlgorithm(const std::string& algorithmName, Func searchFunction,
                       const std::vector<std::string>& testPatterns);
};

#include "Tester.tpp"