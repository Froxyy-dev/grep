#include "Matcher.hpp"


bool Matcher::match_pattern(const std::string &input, const std::string &pattern) {
    auto root = tokenize(pattern);
    std::cout << root->to_string(0) << '\n';

    auto positions = std::views::iota(0, (int)input.size());

    return std::ranges::any_of(positions, [&](size_t position) {
        Backreference backreference;
        return root->get_matches(MatchContext(input, position, backreference)).has_matched();
    });
}
