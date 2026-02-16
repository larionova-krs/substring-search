#include <fstream>
#include <string>
#include <vector>

#include "Tester.hpp"

int main() {
    std::ofstream outfile("benchmark/data/output/testput.txt");
    if (!outfile) {
        std::cerr << "Cannot open file testput.txt" << std::endl;
        return 1;
    }
    Tester tester(outfile);

    std::string infile = "benchmark/data/input//war-and-peace.txt";
    try {
        tester.loadTextFromFile(infile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<std::string> testPatterns = {"war", "peace", "and", "pancake",
                                             "hahahahhaa"};

    tester.testAlgorithm("Rabin-Karp", subsearch::exact::rabinKarp,
                         testPatterns);

    tester.testAlgorithm("Rabin-Karp (no checking)",
                         subsearch::exact::rabinKarpNoChecking, testPatterns);

    auto fuzzySearchWithThreshold = [](const std::string& text,
                                       const std::string& pattern) {
        return subsearch::fuzzy::naiveFuzzySearch(text, pattern);
    };

    tester.testAlgorithm("Fuzzy search", fuzzySearchWithThreshold,
                         testPatterns);

    return 0;
}