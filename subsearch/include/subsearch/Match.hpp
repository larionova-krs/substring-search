#pragma once
#include <iostream>
#include <string>

namespace subsearch {

class Match {
private:
    int pos;
    int len;
    const std::string* text_ptr;
    double percentage;

public:
    Match(int pos, int len, const std::string* ptr, double sim = 100.0);

    std::partial_ordering operator<=>(const Match& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Match& m);
};

}  // namespace subsearch