#include "tokenizer.hpp"


int get_matched(TokenizerStack &tokenizer_stack, const std::string& text, const int id) {
    int offset = 1;
    std::smatch match;

    // Helper function for matching a pattern and performing an action
    auto match_and_act = [&](const std::regex& regex_pattern, auto action) {
        if (std::regex_search(text, match, regex_pattern)) {
            action();
            return true;
        }
        return false;
    };

    // Helper to add tokens and push to the stack if needed
    auto add_token = [&](auto token_ptr, bool push_to_stack = false, bool is_alternative = false) {
        tokenizer_stack.add_token(token_ptr);
        if (push_to_stack) {
            tokenizer_stack.stack_push(is_alternative);
        }
    };

    // Check and perform actions based on matched patterns
    if (match_and_act(patterns::WILDCARD, [&]() {
        add_token(std::make_shared<Wildcard>(id));
        })) return offset;

    if (match_and_act(patterns::BEGIN, [&]() {
        add_token(std::make_shared<BeginAnchor>(id));
        })) return offset;

    if (match_and_act(patterns::END, [&]() {
        add_token(std::make_shared<EndAnchor>(id));
        })) return offset;

    if (match_and_act(patterns::ONE_OR_MORE, [&]() {
        tokenizer_stack.assign_previous(std::make_shared<OneOrMore>(id));
        })) return offset;

    if (match_and_act(patterns::ZERO_OR_ONE, [&]() {
        tokenizer_stack.assign_previous(std::make_shared<ZeroOrOne>(id));
        })) return offset;

    if (match_and_act(patterns::DIGIT, [&]() {
        add_token(std::make_shared<Digit>(id));
        offset = 2;
        })) return offset;

    if (match_and_act(patterns::ALNUM, [&]() {
        add_token(std::make_shared<Alnum>(id));
        offset = 2;
        })) return offset;

    if (match_and_act(patterns::BACKREFERENCE, [&]() {
        auto backref_index_str = match[0].str().substr(1);
        int backref_index = std::stoi(backref_index_str) + 1;
        add_token(std::make_shared<Backref>(id, backref_index));
        offset = static_cast<int>(backref_index_str.size()) + 1;
        })) return offset;

    if (match_and_act(patterns::NEGATIVE_GROUP, [&]() {
        add_token(std::make_shared<NegativeCharacterGroup>(id), true);
        offset = 2;
        })) return offset;

    if (match_and_act(patterns::POSITIVE_GROUP, [&]() {
        add_token(std::make_shared<PositiveCharacterGroup>(id), true);
        })) return offset;

    if (match_and_act(patterns::END_GROUP, [&]() {
        tokenizer_stack.assign_group();
        })) return offset;

    if (match_and_act(patterns::ALTERNATIVE, [&]() {
        add_token(std::make_shared<Alternation>(id), true, true);
        add_token(std::make_shared<Level>(id), true);
        })) return offset;

    if (match_and_act(patterns::ALTERNATION, [&]() {
        tokenizer_stack.assign_group();
        tokenizer_stack.assign_level();
        add_token(std::make_shared<Level>(id), true);
        })) return offset;

    if (match_and_act(patterns::BEGIN_LEVEL, [&]() {
        add_token(std::make_shared<Level>(id), true);
        })) return offset;

    if (match_and_act(patterns::END_LEVEL, [&]() {
        tokenizer_stack.assign_group();
        tokenizer_stack.handle_alternation_group();
        })) return offset;

    // Default case: Add a literal token
    tokenizer_stack.add_token(std::make_shared<Literal>(id, text[0]));
    return offset;
}

std::shared_ptr<Token> tokenize(const std::string& input) {
    auto tokenizer_stack = TokenizerStack();
    int id = 0;

    for (size_t index = 0; index < input.size(); id++) {
        index += get_matched(tokenizer_stack, input.substr(index), id);
    }

    auto root = std::make_shared<Level>(id);
    root->children = tokenizer_stack.tokens;
    return root;
}
