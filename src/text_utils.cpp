#include "markdownify/text_utils.hpp"
#include <algorithm>
#include <sstream>

namespace markdownify {

ChompResult chomp(const std::string& text) {
    ChompResult result;

    if (text.empty()) {
        return result;
    }

    result.prefix = (text[0] == ' ') ? " " : "";
    result.suffix = (text.back() == ' ') ? " " : "";

    result.text = text;
    result.text.erase(0, result.text.find_first_not_of(" "));
    result.text.erase(result.text.find_last_not_of(" ") + 1);

    return result;
}

std::string strip_pre(const std::string& text) {
    return std::regex_replace(
        std::regex_replace(text, RegexPatterns::pre_lstrip(), ""),
        RegexPatterns::pre_rstrip(), ""
    );
}

std::string strip1_pre(const std::string& text) {
    return std::regex_replace(
        std::regex_replace(text, RegexPatterns::pre_lstrip1(), ""),
        RegexPatterns::pre_rstrip1(), ""
    );
}

std::string escape_markdown(const std::string& text, bool escape_asterisks,
                           bool escape_underscores, bool escape_misc) {
    std::string result = text;

    if (escape_misc) {
        result = std::regex_replace(result, RegexPatterns::escape_misc_chars(), "\\$1");
        result = std::regex_replace(result, RegexPatterns::escape_misc_dash_sequences(), "$1\\$2");
        result = std::regex_replace(result, RegexPatterns::escape_misc_hashes(), "$1\\$2");
        result = std::regex_replace(result, RegexPatterns::escape_misc_list_items(), "$1\\$2");
    }

    if (escape_asterisks) {
        size_t pos = 0;
        while ((pos = result.find('*', pos)) != std::string::npos) {
            result.replace(pos, 1, "\\*");
            pos += 2;
        }
    }

    if (escape_underscores) {
        size_t pos = 0;
        while ((pos = result.find('_', pos)) != std::string::npos) {
            result.replace(pos, 1, "\\_");
            pos += 2;
        }
    }

    return result;
}

std::string underline(const std::string& text, char pad_char) {
    std::string trimmed = trim_right(text);
    if (trimmed.empty()) {
        return "";
    }

    std::string underline_str(trimmed.length(), pad_char);
    return "\n\n" + trimmed + "\n" + underline_str + "\n\n";
}

bool is_whitespace_only(const std::string& text) {
    return std::all_of(text.begin(), text.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}

std::string trim(const std::string& str) {
    return trim_left(trim_right(str));
}

std::string trim_left(const std::string& str) {
    auto it = std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });
    return std::string(it, str.end());
}

std::string trim_right(const std::string& str) {
    auto it = std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    });
    return std::string(str.begin(), it.base());
}

std::string wrap_text(const std::string& text, int width) {
    if (width <= 0) {
        return text;
    }

    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;

    int line_length = 0;
    while (words >> word) {
        if (line_length + word.length() + 1 > static_cast<size_t>(width)) {
            if (line_length > 0) {
                wrapped << "\n";
            }
            wrapped << word;
            line_length = word.length();
        } else {
            if (line_length > 0) {
                wrapped << " ";
                line_length++;
            }
            wrapped << word;
            line_length += word.length();
        }
    }

    return wrapped.str();
}

std::string collapse_whitespace(const std::string& text) {
    return std::regex_replace(text, RegexPatterns::whitespace(), " ");
}

std::string normalize_newlines(const std::string& text) {
    return std::regex_replace(text, RegexPatterns::newline_whitespace(), "\n");
}

const std::regex& RegexPatterns::whitespace() {
    static std::regex pattern(R"([\t ]+)");
    return pattern;
}

const std::regex& RegexPatterns::all_whitespace() {
    static std::regex pattern(R"([\t \r\n]+)");
    return pattern;
}

const std::regex& RegexPatterns::newline_whitespace() {
    static std::regex pattern(R"([\t \r\n]*[\r\n][\t \r\n]*)");
    return pattern;
}

const std::regex& RegexPatterns::pre_lstrip1() {
    static std::regex pattern(R"(^ *\n)");
    return pattern;
}

const std::regex& RegexPatterns::pre_rstrip1() {
    static std::regex pattern(R"(\n *$)");
    return pattern;
}

const std::regex& RegexPatterns::pre_lstrip() {
    static std::regex pattern(R"(^[ \n]*\n)");
    return pattern;
}

const std::regex& RegexPatterns::pre_rstrip() {
    static std::regex pattern(R"([ \n]*$)");
    return pattern;
}

const std::regex& RegexPatterns::escape_misc_chars() {
    static std::regex pattern(R"([]\&<`[>~=+|])");
    return pattern;
}

const std::regex& RegexPatterns::escape_misc_dash_sequences() {
    static std::regex pattern(R"((\s|^)(-+(?:\s|$)))");
    return pattern;
}

const std::regex& RegexPatterns::escape_misc_hashes() {
    static std::regex pattern(R"((\s|^)(#{1,6}(?:\s|$)))");
    return pattern;
}

const std::regex& RegexPatterns::escape_misc_list_items() {
    static std::regex pattern(R"(((?:\s|^)[0-9]{1,9})([.)](?:\s|$)))");
    return pattern;
}

const std::regex& RegexPatterns::backtick_runs() {
    static std::regex pattern(R"(`+)");
    return pattern;
}

}