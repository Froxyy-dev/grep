#include "tokens.hpp"

#include <iostream>


matched_results Level::match_here(const MatchContext& context, const size_t tokens_pos) const {
    matched_results matches;

    if (tokens_pos == children.size()) {
        matches.emplace_back(context.backreference, context.position);
        return matches;
    }

    auto [add_backreference, matched_results] = children[tokens_pos]->get_matches(context);

    if (matched_results.empty()) {
        return matches;
    }

    for (const auto& matched_result: matched_results) {
        Backreference new_backreference = matched_result.first;
        MatchContext new_context = MatchContext(context.input, matched_result.second, new_backreference);

        if (add_backreference) {
            const size_t matched_len = matched_result.second - context.position;
            new_backreference.add_match(context.input.substr(context.position, matched_len));
        }

        std::ranges::for_each(match_here(new_context, tokens_pos + 1), [&](auto match) {
            matches.push_back(match);
        });
    }

    return matches;
}

MatchResult Level::get_matches(const MatchContext &context) const {
    context.backreference.init_index();
    auto result = MatchResult(true, match_here(context, 0));

    return result;
}

std::string Level::to_string(int depth) const {
    std::string str = std::string(depth, '\t') + "Level [" + std::to_string(index) + "]:\n";

    for (const auto& token: children) {
        str += token->to_string(depth + 1);
    }

    return str;
}


MatchResult Backref::get_matches(const MatchContext &context) const {
    auto result = MatchResult();

    std::string to_match = context.backreference.get_matched_at(backref_index);
    if (context.input.substr(context.position, to_match.size()) == to_match) {
        result.add_matched_result({context.backreference, context.position + to_match.size()});
    }

    return result;
}

std::string Backref::to_string(int depth) const {
    return std::string(depth, '\t') + "Backref: " + std::to_string(backref_index - 1) + "\n";
}


MatchResult Literal::get_matches(const MatchContext &context) const {
    auto result = MatchResult();

    if (context.position >= context.input.size() or context.input.at(context.position) != literal) {
        return result;
    }

    result.add_matched_result({context.backreference, context.position + 1});
    return result;
}

std::string Literal::to_string(int depth) const {
    return std::string(depth, '\t') + "Literal: '" + std::string(1, literal) + "'\n";
}


MatchResult Digit::get_matches(const MatchContext &context) const {
    auto result = MatchResult();

    auto is_digit = [&](const char c) {
        return std::ranges::any_of(constants::DIGITS, [&](const char digit) -> bool {
            return c == digit;
        });
    };

    if (context.position >= context.input.size() or not is_digit(context.input.at(context.position))) {
        return result;
    }

    result.add_matched_result({context.backreference, context.position + 1});
    return result;
}

std::string Digit::to_string(int depth) const {
    return std::string(depth, '\t') + "Digit\n";
}


MatchResult Alnum::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    if (context.position >= context.input.size() or not isalnum(context.input.at(context.position)
    )) {
        return result;
    }

    result.add_matched_result({context.backreference, context.position + 1});
    return result;
}

std::string Alnum::to_string(int depth) const {
    return std::string(depth, '\t') + "Alnum\n";
}


MatchResult PositiveCharacterGroup::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    auto matches_position = [&](const std::shared_ptr<Token> &token) {
        return token->get_matches(context).has_matched();
    };

    if (std::ranges::any_of(children, matches_position)) {
        result.add_matched_result({context.backreference, context.position + 1});
    }

    return result;
}

std::string PositiveCharacterGroup::to_string(int depth) const {
    std::string str = std::string(depth, '\t') + "PositiveCharacterGroup:\n";
    for (const auto& token : children) {
        str += token->to_string(depth + 1);
    }
    return str;
}


MatchResult NegativeCharacterGroup::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    auto matches_position = [&](const std::shared_ptr<Token> &token) {
        return token->get_matches(context).has_matched();
    };

    if (std::ranges::none_of(children, matches_position)) {
        result.add_matched_result({context.backreference, context.position + 1});
    }

    return result;
}


std::string NegativeCharacterGroup::to_string(int depth) const {
    std::string str = std::string(depth, '\t') + "NegativeCharacterGroup:\n";
    for (const auto& token : children) {
        str += token->to_string(depth + 1);
    }
    return str;
}


MatchResult BeginAnchor::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    if (context.position == 0) {
        result.add_matched_result({context.backreference, context.position});
    }

    return result;
}

std::string BeginAnchor::to_string(int depth) const {
    return std::string(depth, '\t') + "BeginAnchor\n";
}


MatchResult EndAnchor::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    if (context.position == context.input.size()) {
        result.add_matched_result({context.backreference, context.position});
    }

    return result;
}

std::string EndAnchor::to_string(int depth) const {
    return std::string(depth, '\t') + "EndAnchor\n";
}


MatchResult OneOrMore::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    auto matches_position = [&](const std::shared_ptr<Token> &token) -> bool {
        return token->get_matches(context).has_matched();
    };

    size_t position = context.position;
    while (position < context.input.size() and matches_position(children.back())) {
        result.add_matched_result({context.backreference, ++position});
    }

    return result;
}

std::string OneOrMore::to_string(int depth) const {
    return std::string(depth, '\t') + "OneOrMore:\n" + children.back()->to_string(depth + 1);
}


MatchResult ZeroOrOne::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    result.add_matched_result({context.backreference, context.position});

    if (children.back()->get_matches(context).has_matched()) {
        result.add_matched_result({context.backreference, context.position + 1});
    }

    return result;
}

std::string ZeroOrOne::to_string(int depth) const {
    return std::string(depth, '\t') + "ZeroOrOne:\n" + children.back()->to_string(depth + 1);
}


MatchResult Wildcard::get_matches(const MatchContext& context) const {
    auto result = MatchResult();

    if (context.position < context.input.size()) {
        result.add_matched_result({context.backreference, context.position + 1});
    }

    return result;
}

std::string Wildcard::to_string(int depth) const {
    return std::string(depth, '\t') + "Wildcard\n";
}


MatchResult Alternation::get_matches(const MatchContext& context) const {
    auto result = MatchResult(true);

    for (const auto& token: children) {
        Backreference new_backreference = context.backreference;
        MatchContext new_context = MatchContext(context.input, context.position, new_backreference);

        for (const auto& match: token->get_matches(new_context).results) {
            result.add_matched_result(match);
        }
    }

    return result;
}

std::string Alternation::to_string(int depth) const {
    std::string str = std::string(depth, '\t') + "Alternation [" + std::to_string(index) + "]:\n";
    for (const auto& token: children) {
        str += token->to_string(depth + 1);
    }
    return str;
}
