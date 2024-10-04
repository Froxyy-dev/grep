#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace constants {
    const std::string DIGITS = "0123456789";
}

struct Backreference {
    int size;
    std::vector<int> stack;
    std::map<int, std::string> index_to_matched;

    Backreference() : size(0) {}

    void init_index(){
        stack.push_back(++size);
    }

    void add_match(const std::string &matched) {
        index_to_matched[stack.back()] = matched;
        stack.pop_back();
    }

    std::string get_matched_at(const int index) {
        if (not index_to_matched.contains(index)) {
            return "";
        }

        return index_to_matched[index];
    }

};

#endif //UTILS_HPP
