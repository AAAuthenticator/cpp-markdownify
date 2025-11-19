#include "markdownify/converter.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace markdownify {

static std::string preprocess_html(const std::string& html) {
    static const std::regex self_closing_pattern(
        R"(<(area|base|br|col|embed|hr|img|input|link|meta|param|source|track|wbr)([^>]*)>)",
        std::regex_constants::icase
    );

    std::string result = std::regex_replace(html, self_closing_pattern, "<$1$2/>");
    return result;
}

MarkdownConverter::MarkdownConverter(const Options& options)
    : options_(options) {}

std::string MarkdownConverter::convert(const std::string& html) {
    tinyxml2::XMLDocument doc;

    std::string preprocessed = preprocess_html(html);
    std::string wrapped_html = "<html>" + preprocessed + "</html>";
    doc.Parse(wrapped_html.c_str());

    if (doc.Error()) {
        return "";
    }

    return convert_soup(doc);
}

std::string MarkdownConverter::convert_soup(tinyxml2::XMLDocument& doc) {
    auto root = doc.RootElement();
    if (!root) {
        return "";
    }

    std::string result = process_element(root, std::unordered_set<std::string>());
    return strip_document(result);
}

std::string MarkdownConverter::get_tag_name(tinyxml2::XMLElement* element) {
    const char* name = element->Name();
    return name ? std::string(name) : "";
}

bool MarkdownConverter::should_remove_whitespace_inside(const std::string& tag_name) {
    if (tag_name.empty()) return false;

    if (tag_name[0] == 'h' && tag_name.length() == 2 &&
        tag_name[1] >= '1' && tag_name[1] <= '6') {
        return true;
    }

    static const std::unordered_set<std::string> block_tags = {
        "p", "blockquote", "article", "div", "section",
        "ol", "ul", "li", "dl", "dt", "dd",
        "table", "thead", "tbody", "tfoot", "tr", "td", "th"
    };

    return block_tags.find(tag_name) != block_tags.end();
}

bool MarkdownConverter::should_remove_whitespace_outside(const std::string& tag_name) {
    return should_remove_whitespace_inside(tag_name) || tag_name == "pre";
}

std::string MarkdownConverter::process_element(tinyxml2::XMLNode* node,
                                              const std::unordered_set<std::string>& parent_tags) {
    if (!node) return "";

    if (auto element = node->ToElement()) {
        return process_tag(element, parent_tags);
    } else if (auto text = node->ToText()) {
        return process_text(text, parent_tags);
    }

    return "";
}

std::string MarkdownConverter::process_text(tinyxml2::XMLText* text,
                                           const std::unordered_set<std::string>& parent_tags) {
    if (!text) return "";

    const char* value = text->Value();
    if (!value) return "";

    std::string text_content(value);

    if (parent_tags.find("_noformat") != parent_tags.end()) {
        return text_content;
    }

    text_content = collapse_whitespace(text_content);
    return escape(text_content, parent_tags);
}

std::string MarkdownConverter::process_tag(tinyxml2::XMLElement* element,
                                          const std::unordered_set<std::string>& parent_tags) {
    if (!element) return "";

    std::string tag_name = get_tag_name(element);
    if (tag_name.empty()) return "";

    std::transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);

    if (!options_.should_convert_tag(tag_name)) {
        return "";
    }

    auto new_parent_tags = parent_tags;
    new_parent_tags.insert(tag_name);

    if ((tag_name[0] == 'h' && tag_name.length() == 2) ||
        tag_name == "td" || tag_name == "th") {
        new_parent_tags.insert("_inline");
    }

    if (tag_name == "pre" || tag_name == "code" ||
        tag_name == "kbd" || tag_name == "samp") {
        new_parent_tags.insert("_noformat");
    }

    if (tag_name == "ol") {
        list_depth_++;
        list_counters_.push_back(1);
    } else if (tag_name == "ul") {
        list_depth_++;
    }

    std::ostringstream child_text;
    for (auto child = element->FirstChild(); child; child = child->NextSibling()) {
        child_text << process_element(child, new_parent_tags);
    }

    std::string text = child_text.str();

    if (tag_name == "ol") {
        list_counters_.pop_back();
        list_depth_--;
    } else if (tag_name == "ul") {
        list_depth_--;
    }

    if (tag_name == "a") {
        return convert_a(element, text, parent_tags);
    } else if (tag_name == "blockquote") {
        return convert_blockquote(element, text, parent_tags);
    } else if (tag_name == "br") {
        return convert_br(element, text, parent_tags);
    } else if (tag_name == "code") {
        return convert_code(element, text, parent_tags);
    } else if (tag_name == "em") {
        return convert_em(element, text, parent_tags);
    } else if (tag_name == "strong") {
        return convert_strong(element, text, parent_tags);
    } else if (tag_name == "b") {
        return convert_b(element, text, parent_tags);
    } else if (tag_name == "i") {
        return convert_i(element, text, parent_tags);
    } else if (tag_name == "del" || tag_name == "s" || tag_name == "strike") {
        return convert_del(element, text, parent_tags);
    } else if (tag_name == "hr") {
        return convert_hr(element, text, parent_tags);
    } else if (tag_name[0] == 'h' && tag_name.length() == 2 &&
               tag_name[1] >= '1' && tag_name[1] <= '6') {
        int level = tag_name[1] - '0';
        return convert_h(element, text, parent_tags, level);
    } else if (tag_name == "img") {
        return convert_img(element, text, parent_tags);
    } else if (tag_name == "li") {
        return convert_li(element, text, parent_tags);
    } else if (tag_name == "ol") {
        return convert_ol(element, text, parent_tags);
    } else if (tag_name == "ul") {
        return convert_ul(element, text, parent_tags);
    } else if (tag_name == "p") {
        return convert_p(element, text, parent_tags);
    } else if (tag_name == "pre") {
        return convert_pre(element, text, parent_tags);
    } else if (tag_name == "table") {
        return convert_table(element, text, parent_tags);
    } else if (tag_name == "tr") {
        return convert_tr(element, text, parent_tags);
    } else if (tag_name == "td") {
        return convert_td(element, text, parent_tags);
    } else if (tag_name == "th") {
        return convert_th(element, text, parent_tags);
    } else if (tag_name == "script" || tag_name == "style") {
        return "";
    }

    return text;
}

std::string MarkdownConverter::escape(const std::string& text,
                                     const std::unordered_set<std::string>& parent_tags) {
    if (text.empty()) return "";

    if (parent_tags.find("_noformat") != parent_tags.end()) {
        return text;
    }

    return escape_markdown(text, options_.escape_asterisks,
                          options_.escape_underscores, options_.escape_misc);
}

std::string MarkdownConverter::inline_markup(const std::string& markup_prefix,
                                            const std::string& text,
                                            const std::unordered_set<std::string>& parent_tags) {
    std::string markup_suffix = markup_prefix;

    if (!markup_prefix.empty() && markup_prefix[0] == '<' &&
        markup_prefix.back() == '>') {
        markup_suffix = "</" + markup_prefix.substr(1);
    }

    if (parent_tags.find("_noformat") != parent_tags.end()) {
        return text;
    }

    auto chomped = chomp(text);
    if (chomped.text.empty()) {
        return "";
    }

    return chomped.prefix + markup_prefix + chomped.text +
           markup_suffix + chomped.suffix;
}

std::string MarkdownConverter::convert_a(tinyxml2::XMLElement* el, const std::string& text,
                                        const std::unordered_set<std::string>& parent_tags) {
    if (parent_tags.find("_noformat") != parent_tags.end()) {
        return text;
    }

    auto chomped = chomp(text);
    if (chomped.text.empty()) {
        return "";
    }

    const char* href = el->Attribute("href");
    const char* title = el->Attribute("title");

    std::string href_str = href ? href : "";
    std::string title_str = title ? title : "";

    std::string unescaped_text = chomped.text;
    size_t pos = 0;
    while ((pos = unescaped_text.find("\\_", pos)) != std::string::npos) {
        unescaped_text.replace(pos, 2, "_");
        pos += 1;
    }

    if (options_.autolinks && unescaped_text == href_str && title_str.empty()) {
        return chomped.prefix + "<" + href_str + ">" + chomped.suffix;
    }

    if (!title_str.empty()) {
        return chomped.prefix + "[" + chomped.text + "](" +
               href_str + " \"" + title_str + "\")" + chomped.suffix;
    }

    return chomped.prefix + "[" + chomped.text + "](" +
           href_str + ")" + chomped.suffix;
}

std::string MarkdownConverter::convert_blockquote(tinyxml2::XMLElement* el,
                                                  const std::string& text,
                                                  const std::unordered_set<std::string>& parent_tags) {
    std::istringstream stream(text);
    std::ostringstream result;
    std::string line;

    while (std::getline(stream, line)) {
        result << "> " << line << "\n";
    }

    return "\n" + result.str() + "\n";
}

std::string MarkdownConverter::convert_br(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    if (options_.newline_style == NewlineStyle::SPACES) {
        return "  \n";
    } else {
        return "\\\n";
    }
}

std::string MarkdownConverter::convert_code(tinyxml2::XMLElement* el, const std::string& text,
                                           const std::unordered_set<std::string>& parent_tags) {
    if (parent_tags.find("pre") != parent_tags.end()) {
        return text;
    }

    if (text.empty()) {
        return "";
    }

    int backtick_count = 1;
    std::smatch match;
    std::string temp = text;
    while (std::regex_search(temp, match, RegexPatterns::backtick_runs())) {
        backtick_count = std::max(backtick_count, static_cast<int>(match.str().length()) + 1);
        temp = match.suffix();
    }

    std::string backticks(backtick_count, '`');
    std::string space = (text[0] == '`' || text.back() == '`') ? " " : "";

    return backticks + space + text + space + backticks;
}

std::string MarkdownConverter::convert_em(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    char symbol = (options_.strong_em_symbol == StrongEmSymbol::ASTERISK) ? '*' : '_';
    return inline_markup(std::string(1, symbol), text, parent_tags);
}

std::string MarkdownConverter::convert_strong(tinyxml2::XMLElement* el, const std::string& text,
                                             const std::unordered_set<std::string>& parent_tags) {
    char symbol = (options_.strong_em_symbol == StrongEmSymbol::ASTERISK) ? '*' : '_';
    return inline_markup(std::string(2, symbol), text, parent_tags);
}

std::string MarkdownConverter::convert_b(tinyxml2::XMLElement* el, const std::string& text,
                                        const std::unordered_set<std::string>& parent_tags) {
    return convert_strong(el, text, parent_tags);
}

std::string MarkdownConverter::convert_i(tinyxml2::XMLElement* el, const std::string& text,
                                        const std::unordered_set<std::string>& parent_tags) {
    return convert_em(el, text, parent_tags);
}

std::string MarkdownConverter::convert_del(tinyxml2::XMLElement* el, const std::string& text,
                                          const std::unordered_set<std::string>& parent_tags) {
    return inline_markup("~~", text, parent_tags);
}

std::string MarkdownConverter::convert_h(tinyxml2::XMLElement* el, const std::string& text,
                                        const std::unordered_set<std::string>& parent_tags,
                                        int level) {
    if (options_.heading_style == HeadingStyle::ATX) {
        return "\n\n" + std::string(level, '#') + " " + text + "\n\n";
    } else if (options_.heading_style == HeadingStyle::ATX_CLOSED) {
        return "\n\n" + std::string(level, '#') + " " + text + " " +
               std::string(level, '#') + "\n\n";
    } else {
        if (level == 1) {
            return underline(text, '=');
        } else if (level == 2) {
            return underline(text, '-');
        } else {
            return "\n\n" + std::string(level, '#') + " " + text + "\n\n";
        }
    }
}

std::string MarkdownConverter::convert_hr(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return "\n\n---\n\n";
}

std::string MarkdownConverter::convert_img(tinyxml2::XMLElement* el, const std::string& text,
                                          const std::unordered_set<std::string>& parent_tags) {
    const char* alt = el->Attribute("alt");
    const char* src = el->Attribute("src");
    const char* title = el->Attribute("title");

    std::string alt_str = alt ? alt : "";
    std::string src_str = src ? src : "";
    std::string title_str = title ? title : "";

    if (parent_tags.find("_inline") != parent_tags.end()) {
        return alt_str;
    }

    if (!title_str.empty()) {
        return "![" + alt_str + "](" + src_str + " \"" + title_str + "\")";
    }

    return "![" + alt_str + "](" + src_str + ")";
}

std::string MarkdownConverter::convert_li(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    std::string prefix;

    if (parent_tags.find("ol") != parent_tags.end()) {
        int counter = list_counters_.empty() ? 1 : list_counters_.back();
        prefix = std::to_string(counter) + ". ";
        if (!list_counters_.empty()) {
            list_counters_.back()++;
        }
    } else {
        size_t bullet_index = (list_depth_ > 0 ? list_depth_ - 1 : 0) % options_.bullets.size();
        prefix = options_.bullets[bullet_index] + " ";
    }

    std::istringstream stream(text);
    std::ostringstream result;
    std::string line;
    bool first = true;

    while (std::getline(stream, line)) {
        if (first) {
            result << prefix << line << "\n";
            first = false;
        } else {
            result << "  " << line << "\n";
        }
    }

    return result.str();
}

std::string MarkdownConverter::convert_ol(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return "\n" + text + "\n";
}

std::string MarkdownConverter::convert_ul(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return "\n" + text + "\n";
}

std::string MarkdownConverter::convert_p(tinyxml2::XMLElement* el, const std::string& text,
                                        const std::unordered_set<std::string>& parent_tags) {
    if (text.empty()) {
        return "";
    }

    if (options_.wrap) {
        return "\n\n" + wrap_text(text, options_.wrap_width) + "\n\n";
    }

    return "\n\n" + text + "\n\n";
}

std::string MarkdownConverter::convert_pre(tinyxml2::XMLElement* el, const std::string& text,
                                          const std::unordered_set<std::string>& parent_tags) {
    std::string processed_text = text;

    if (options_.strip_pre == StripMode::STRIP) {
        processed_text = strip_pre(processed_text);
    } else if (options_.strip_pre == StripMode::STRIP_ONE) {
        processed_text = strip1_pre(processed_text);
    }

    std::string lang = options_.code_language;
    if (options_.code_language_callback) {
        const char* class_attr = el->Attribute("class");
        if (class_attr) {
            lang = options_.code_language_callback(class_attr);
        }
    }

    return "\n\n```" + lang + "\n" + processed_text + "\n```\n\n";
}

std::string MarkdownConverter::element_to_html(tinyxml2::XMLElement* element) {
    if (!element) return "";

    tinyxml2::XMLPrinter printer;
    element->Accept(&printer);
    return std::string(printer.CStr());
}

std::string MarkdownConverter::convert_table(tinyxml2::XMLElement* el, const std::string& text,
                                            const std::unordered_set<std::string>& parent_tags) {
    if (options_.preserve_html_tables) {
        return "\n\n" + element_to_html(el) + "\n\n";
    }

    // Count columns in the first row for GFM table separator
    int column_count = 0;
    tinyxml2::XMLElement* first_row = nullptr;

    // Try to find first row in thead, then tbody, then directly in table
    auto thead = el->FirstChildElement("thead");
    if (thead) {
        first_row = thead->FirstChildElement("tr");
    }
    if (!first_row) {
        auto tbody = el->FirstChildElement("tbody");
        if (tbody) {
            first_row = tbody->FirstChildElement("tr");
        }
    }
    if (!first_row) {
        first_row = el->FirstChildElement("tr");
    }

    // Count columns (th or td elements in first row)
    if (first_row) {
        for (auto cell = first_row->FirstChildElement(); cell; cell = cell->NextSiblingElement()) {
            std::string tag_name = get_tag_name(cell);
            std::transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);
            if (tag_name == "th" || tag_name == "td") {
                column_count++;
            }
        }
    }

    // Insert separator row after first line for GFM compatibility
    std::string result = text;
    if (column_count > 0) {
        size_t first_newline = result.find('\n');
        if (first_newline != std::string::npos) {
            std::string separator = "|";
            for (int i = 0; i < column_count; i++) {
                separator += " --- |";
            }
            separator += "\n";
            result.insert(first_newline + 1, separator);
        }
    }

    return "\n\n" + result + "\n";
}

std::string MarkdownConverter::convert_tr(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return "| " + text + "\n";
}

std::string MarkdownConverter::convert_td(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return text + " | ";
}

std::string MarkdownConverter::convert_th(tinyxml2::XMLElement* el, const std::string& text,
                                         const std::unordered_set<std::string>& parent_tags) {
    return text + " | ";
}

std::string MarkdownConverter::strip_document(const std::string& text) {
    std::string result = text;

    if (options_.strip_document == StripMode::LSTRIP ||
        options_.strip_document == StripMode::STRIP) {
        result = trim_left(result);
    }

    if (options_.strip_document == StripMode::RSTRIP ||
        options_.strip_document == StripMode::STRIP) {
        result = trim_right(result);
    }

    return result;
}

}
