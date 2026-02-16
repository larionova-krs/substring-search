#include <algorithm>
#include <cctype>
#include <subsearch/Match.hpp>

namespace subsearch {

Match::Match(int pos, int len, const std::string* ptr, double sim)
    : pos(pos), len(len), text_ptr(ptr), percentage(sim) {}

std::partial_ordering Match::operator<=>(const Match& other) const {
    return percentage <=> other.percentage;
}

std::ostream& operator<<(std::ostream& out, const Match& m) {
    int context = 10;
    int start = std::max(0, m.pos - context);
    int end = std::min((int)m.text_ptr->length(), m.pos + m.len + context);
    out << "...";
    int i = start;
    bool isCtrl;
    for (; i < end && i != m.pos; ++i) {
        isCtrl = std::iscntrl(static_cast<unsigned char>((*m.text_ptr)[i]));
        out << ((isCtrl) ? ' ' : (*m.text_ptr)[i]);
    }
    out << "[";
    for (; i < end && i != m.pos + m.len; ++i) {
        isCtrl = std::iscntrl(static_cast<unsigned char>((*m.text_ptr)[i]));
        out << ((isCtrl) ? ' ' : (*m.text_ptr)[i]);
    }
    out << "]";
    for (; i < end; ++i) {
        isCtrl = std::iscntrl(static_cast<unsigned char>((*m.text_ptr)[i]));
        out << ((isCtrl) ? ' ' : (*m.text_ptr)[i]);
    }
    out << "... (Match: " << m.percentage << "%)";
    return out;
}

}  // namespace subsearch