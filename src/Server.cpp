#include <iostream>
#include <string>

#include "Matcher.hpp"


int main(int argc, char *argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    const std::string flag = argv[1];
    const std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    std::string input_line;
    std::getline(std::cin, input_line);

    const bool matches_pattern = Matcher::match_pattern(input_line, pattern);

    return (matches_pattern) ? 0 : 1;
}
