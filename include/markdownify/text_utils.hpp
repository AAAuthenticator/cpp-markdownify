#pragma once

#include <string>
#include <tuple>
#include <regex>

namespace markdownify {

struct ChompResult {
    std::string prefix;
    std::string suffix;
    std::string text;
};

ChompResult chomp(const std::string& text);

std::string strip_pre(const std::string& text);

std::string strip1_pre(const std::string& text);

std::string escape_markdown(const std::string& text, bool escape_asterisks,
                           bool escape_underscores, bool escape_misc);

std::string underline(const std::string& text, char pad_char);

bool is_whitespace_only(const std::string& text);

std::string trim(const std::string& str);

std::string trim_left(const std::string& str);

std::string trim_right(const std::string& str);

std::string wrap_text(const std::string& text, int width);

std::string collapse_whitespace(const std::string& text);

std::string normalize_newlines(const std::string& text);

class RegexPatterns {
public:
    static const std::regex& whitespace();
    static const std::regex& all_whitespace();
    static const std::regex& newline_whitespace();
    static const std::regex& pre_lstrip1();
    static const std::regex& pre_rstrip1();
    static const std::regex& pre_lstrip();
    static const std::regex& pre_rstrip();
    static const std::regex& escape_misc_chars();
    static const std::regex& escape_misc_dash_sequences();
    static const std::regex& escape_misc_hashes();
    static const std::regex& escape_misc_list_items();
    static const std::regex& backtick_runs();
};

}