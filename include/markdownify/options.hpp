#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <functional>

namespace markdownify {

enum class HeadingStyle {
    ATX,
    ATX_CLOSED,
    UNDERLINED,
    SETEXT = UNDERLINED
};

enum class NewlineStyle {
    SPACES,
    BACKSLASH
};

enum class StrongEmSymbol {
    ASTERISK,
    UNDERSCORE
};

enum class StripMode {
    NONE,
    LSTRIP,
    RSTRIP,
    STRIP,
    STRIP_ONE
};

struct Options {
    bool autolinks = true;

    std::vector<std::string> bullets = {"*", "+", "-"};

    std::string code_language = "";

    std::function<std::string(const std::string&)> code_language_callback = nullptr;

    std::unordered_set<std::string> convert_tags;

    bool default_title = false;

    bool escape_asterisks = true;
    bool escape_underscores = true;
    bool escape_misc = false;

    HeadingStyle heading_style = HeadingStyle::ATX;

    std::vector<std::string> keep_inline_images_in;

    NewlineStyle newline_style = NewlineStyle::SPACES;

    std::unordered_set<std::string> strip_tags;

    StripMode strip_document = StripMode::STRIP;
    StripMode strip_pre = StripMode::STRIP;

    StrongEmSymbol strong_em_symbol = StrongEmSymbol::ASTERISK;

    std::string sub_symbol = "";
    std::string sup_symbol = "";

    bool table_infer_header = false;

    bool wrap = false;
    int wrap_width = 80;

    bool should_convert_tag(const std::string& tag) const {
        // Always allow wrapper/container tags
        if (tag == "html" || tag == "body" || tag == "div") {
            return true;
        }

        if (!strip_tags.empty()) {
            return strip_tags.find(tag) == strip_tags.end();
        } else if (!convert_tags.empty()) {
            return convert_tags.find(tag) != convert_tags.end();
        }
        return true;
    }
};

}
