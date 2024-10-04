#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <regex>
#include <utility>
#include <vector>

#include "tokens.hpp"
#include "utils.hpp"


namespace patterns {
    const std::regex ALNUM(R"(^\\w)");
    const std::regex DIGIT(R"(^\\d)");
    const std::regex WILDCARD(R"(^\.)");
    const std::regex BEGIN(R"(^\^)");
    const std::regex END(R"(^\$)");
    const std::regex ONE_OR_MORE(R"(^\+)");
    const std::regex ZERO_OR_ONE(R"(^\?)");
    const std::regex BACKREFERENCE(R"(^\\\d+)");
    const std::regex NEGATIVE_GROUP(R"(^\[\^)");
    const std::regex POSITIVE_GROUP(R"(^\[)");
    const std::regex END_GROUP(R"(^\])");
    const std::regex ALTERNATIVE(R"(^\([^\)]+\|)");
    const std::regex ALTERNATION(R"(^\|)");
    const std::regex BEGIN_LEVEL(R"(^\()");
    const std::regex END_LEVEL(R"(^\))");
}


struct TokenizerStack {
    std::vector<std::shared_ptr<Token>> tokens;
    std::vector<std::pair<std::shared_ptr<Token>, bool>> stack; // { pointer, is_alternative }

    // Add a token to the tokens vector
    void add_token(const std::shared_ptr<Token> &token) {
        tokens.push_back(token);
    };

    // Assign the previous token as a child to the given token
    void assign_previous(const std::shared_ptr<Token> &token) {
        token->children.push_back(tokens.back());

        tokens.pop_back();
        tokens.push_back(token);
    };

    // Assign a group of tokens from the stack to the top of the stack
    void assign_group() {
        auto top_ptr = stack.back();
        std::vector<std::shared_ptr<Token>> group;

        // 1. Pop from tokens until we found top of stack.
        while (tokens.back()->index != top_ptr.first->index) {
            group.push_back(tokens.back());
            tokens.pop_back();
        }

        // 2. Reverse the group.
        std::ranges::reverse(group);

        // 3. Assign children to top of stack.
        for (auto& token : group) {
            top_ptr.first->children.push_back(token);
        }

        // 4. Pop token from the stack.
        stack.pop_back();
    };

    // Push the top token to the stack, with an alternation flag
    void stack_push(bool is_alternative) {
        stack.emplace_back(tokens.back(), is_alternative);
    }

    // Assign the top token to its previous level
    void assign_level() {
        auto level_ptr = tokens.back();
        tokens.pop_back();
        tokens.back()->children.push_back(level_ptr);
    }

    // Handle an alternation group if it's the top of the stack
    void handle_alternation_group() {
        if (stack.empty() or not stack.back().second) {
            return;
        }

        stack.pop_back();
        assign_level();
    }
};


std::shared_ptr<Token> tokenize(const std::string& input);

#endif //TOKENIZER_HPP
