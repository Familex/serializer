#pragma once

#include <unordered_map>

#include <expected>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace dynser::config
{

struct FileName
{
    std::string config_file_name;
};

struct RawContents
{
    std::string config;
};

namespace details::yaml
{
struct Regex
{
    std::string value;
};

struct DynRegex
{
    std::string value;
};

using GroupValues = std::unordered_map<std::size_t, std::string>;

using DynGroupValues = std::unordered_map<std::size_t, std::string>;
}    // namespace details::yaml

namespace details
{
// regex parsing
namespace regex
{

// inclusive both ends
struct Quantifier
{
    std::size_t from;
    std::optional<std::size_t> to;
    bool is_lazy;

    constexpr inline auto operator<=>(const Quantifier&) const noexcept = default;
} inline const without_quantifier{ 1, 1, false };

using Token = std::variant<
    struct Empty,
    struct WildCard,
    struct Group,
    struct Backreference,
    struct Lookup,
    struct CharacterClass,
    struct Disjunction>;

struct Empty
{ };

// fullstop symbol
struct WildCard
{
    Quantifier quantifier;
};

struct Group
{
    std::unique_ptr<struct Regex> value;
    bool is_capturing;
    Quantifier quantifier;

    explicit Group(std::unique_ptr<struct Regex>&& value, bool is_capturing, Quantifier&& quantifier) noexcept;
    explicit Group(Group&& other) noexcept = default;
    Group(const Group& other) noexcept;
};

struct Backreference
{
    std::size_t group_number;
    Quantifier quantifier;
};

struct Lookup
{
    std::unique_ptr<struct Regex> value;
    bool is_negative;
    bool is_forward;    // true if forward lookup, false if backward

    explicit Lookup(std::unique_ptr<struct Regex>&& value, bool is_negative, bool is_forward) noexcept;
    explicit Lookup(Lookup&& other) noexcept = default;
    Lookup(const Lookup& other) noexcept;
};

struct CharacterClass
{
    std::string characters;    // raw (without negation '^' symbol)
    bool is_negative;
    Quantifier quantifier;
};

struct Disjunction
{
    std::unique_ptr<Token> left;
    std::unique_ptr<Token> right;

    explicit Disjunction(std::unique_ptr<Token>&& left, std::unique_ptr<Token>&& right) noexcept;
    explicit Disjunction(Disjunction&& other) noexcept = default;
    Disjunction(const Disjunction& other) noexcept;
};

struct Regex
{
    std::vector<Token> value;
};

// error position if error
std::expected<Regex, std::size_t> from_string(const std::string_view sv) noexcept;

enum class ToStringErrorType {
    RegexSyntaxError,    /// group regex syntax error
    MissingValue,        /// value with current group num not found
    InvalidValue,        /// group regex missmatched
};

struct ToStringError
{
    ToStringErrorType type;
    std::size_t group_num;    // wrong field arrangement
};

using ToStringResult = std::expected<std::string, ToStringError>;

ToStringResult to_string(const Regex& reg, const yaml::GroupValues& vals) noexcept;

}    // namespace regex

yaml::Regex resolve_dyn_regex(yaml::DynRegex&& dyn_reg, yaml::DynGroupValues&& dyn_gr_vals) noexcept;

regex::ToStringResult resolve_regex(yaml::Regex&& reg, yaml::GroupValues&& vals) noexcept;

}    // namespace details

struct Config
{
    explicit Config() noexcept { }

    Config(std::string_view config) noexcept
    {
        // TODO some config parse etc.
    }
};

}    // namespace dynser::config