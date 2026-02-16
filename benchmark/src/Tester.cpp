#include "Tester.hpp"

#include <stdexcept>

Tester::Tester(std::ostream& output) : outputStream(&output) {}

void Tester::loadTextFromFile(const std::string& filename) {
    std::ostream& out = *outputStream;

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Exception: Can not open file " + filename);
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    text.resize(fileSize);
    file.read(&text[0], fileSize);
    file.close();

    out << "File: " << filename << std::endl;
    out << "Size: " << std::fixed << std::setprecision(2)
        << (double)fileSize / 1024 / 1024 << " MiB" << std::endl;
    out << "First 100 characters: " << std::endl
        << "..." << text.substr(0, 100) << "..." << std::endl
        << std::endl;
}