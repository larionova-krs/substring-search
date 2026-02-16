template <typename Func>
void Tester::testAlgorithm(const std::string& algorithmName,
                           Func searchFunction,
                           const std::vector<std::string>& testPatterns) {
    std::ostream& out = *outputStream;

    out << "======================= ";
    out << "Algorithm: " << algorithmName;
    out << " ======================= " << std::endl;

    for (const auto& pattern : testPatterns) {
        out << "Substring: \"" << pattern << "\"" << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<subsearch::Match> positions = searchFunction(text, pattern);

        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            endTime - startTime);

        out << "\tMatches found: " << positions.size() << std::endl;
        out << "\tExecution time: " << std::fixed << std::setprecision(2)
            << duration.count() / 1000.0 << " ms" << std::endl;

        if (!positions.empty()) {
            out << "\tFirst 5 matches:" << std::endl;
            int limit = std::min(5, static_cast<int>(positions.size()));
            for (int i = 0; i < limit; ++i) {
                out << positions[i] << std::endl;
            }
            out << std::endl;
        }
    }
    out << "==============================================================="
        << std::endl
        << std::endl;
}