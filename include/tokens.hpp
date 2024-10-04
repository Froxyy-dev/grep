#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "utils.hpp"


using matched_result = std::pair<Backreference, size_t>;
using matched_results = std::vector<matched_result>;


struct MatchResult {
    bool add_backreference;
    matched_results results;

    explicit MatchResult(bool _add_backreference = false, matched_results _results = {}) :
            add_backreference(_add_backreference), results(std::move(_results)) {}

    void add_matched_result(const matched_result &matched) {
        results.emplace_back(matched);
    }

    [[nodiscard]] bool has_matched() const {
        return not results.empty();
    }
};

struct MatchContext {
    std::string input;
    size_t position;
    Backreference &backreference;

    MatchContext(std::string input, size_t position, Backreference &backreference)
            : input(std::move(input)), position(position), backreference(backreference) {}
};


class Token {
public:
    int index;
    std::vector<std::shared_ptr<Token>> children{};

    explicit Token(const int index) : index(index) {}

    [[nodiscard]] virtual MatchResult get_matches(const MatchContext &context) const = 0;

    [[nodiscard]] virtual std::string to_string(int depth) const = 0;

    virtual ~Token() = default;
};

// Token for matching a character or group of tokens
class Level : public Token {
private:
    [[nodiscard]] matched_results match_here(const MatchContext &context, size_t tokens_pos) const;

public:
    explicit Level(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for backreference
class Backref : public Token {
private:
    int backref_index;
public:
    explicit Backref(const int index, const int backref_index) : Token(index), backref_index
            (backref_index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching literals
class Literal : public Token {
private:
    char literal;

public:
    explicit Literal(const int index, const char _literal) : Token(index), literal(_literal) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching digits
class Digit : public Token {
public:
    explicit Digit(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching alphanumeric characters
class Alnum : public Token {
public:
    explicit Alnum(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching positive character groups
class PositiveCharacterGroup : public Token {
public:
    explicit PositiveCharacterGroup(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching negative character groups
class NegativeCharacterGroup : public Token {
public:
    explicit NegativeCharacterGroup(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching the beginning of input
class BeginAnchor : public Token {
public:
    explicit BeginAnchor(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching the end of input
class EndAnchor : public Token {
public:
    explicit EndAnchor(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching one or more repetitions
class OneOrMore : public Token {
public:
    explicit OneOrMore(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching zero or one repetitions
class ZeroOrOne : public Token {
public:
    explicit ZeroOrOne(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for matching a wildcard
class Wildcard : public Token {
public:
    explicit Wildcard(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

// Token for handling alternations (e.g. | in regex)
class Alternation : public Token {
public:
    explicit Alternation(const int index) : Token(index) {}

    [[nodiscard]] MatchResult get_matches(const MatchContext &context) const override;

    [[nodiscard]] std::string to_string(int depth) const override;
};

#endif //TOKENS_HPP
