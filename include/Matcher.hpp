#ifndef MATCHER_HPP
#define MATCHER_HPP

#include <ranges>
#include <string>

#include "tokenizer.hpp"
#include "utils.hpp"

class Matcher {
public:
    static bool match_pattern(const std::string &input, const std::string &pattern);
};


#endif //MATCHER_HPP
