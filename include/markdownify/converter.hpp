#pragma once

#include "markdownify/options.hpp"
#include "markdownify/text_utils.hpp"
#include <tinyxml2.h>
#include <string>
#include <unordered_set>
#include <memory>

namespace markdownify {

class MarkdownConverter {
public:
    explicit MarkdownConverter(const Options& options = Options());

    std::string convert(const std::string& html);

    std::string convert_soup(tinyxml2::XMLDocument& doc);

private:
    Options options_;
    int list_depth_ = 0;
    std::vector<int> list_counters_;

    std::string process_element(tinyxml2::XMLNode* node,
                                const std::unordered_set<std::string>& parent_tags);

    std::string process_tag(tinyxml2::XMLElement* element,
                           const std::unordered_set<std::string>& parent_tags);

    std::string process_text(tinyxml2::XMLText* text,
                            const std::unordered_set<std::string>& parent_tags);

    std::string get_tag_name(tinyxml2::XMLElement* element);

    bool should_remove_whitespace_inside(const std::string& tag_name);
    bool should_remove_whitespace_outside(const std::string& tag_name);

    std::string convert_a(tinyxml2::XMLElement* el, const std::string& text,
                         const std::unordered_set<std::string>& parent_tags);
    std::string convert_blockquote(tinyxml2::XMLElement* el, const std::string& text,
                                   const std::unordered_set<std::string>& parent_tags);
    std::string convert_br(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_code(tinyxml2::XMLElement* el, const std::string& text,
                            const std::unordered_set<std::string>& parent_tags);
    std::string convert_em(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_strong(tinyxml2::XMLElement* el, const std::string& text,
                              const std::unordered_set<std::string>& parent_tags);
    std::string convert_b(tinyxml2::XMLElement* el, const std::string& text,
                         const std::unordered_set<std::string>& parent_tags);
    std::string convert_i(tinyxml2::XMLElement* el, const std::string& text,
                         const std::unordered_set<std::string>& parent_tags);
    std::string convert_del(tinyxml2::XMLElement* el, const std::string& text,
                           const std::unordered_set<std::string>& parent_tags);
    std::string convert_h(tinyxml2::XMLElement* el, const std::string& text,
                         const std::unordered_set<std::string>& parent_tags, int level);
    std::string convert_hr(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_img(tinyxml2::XMLElement* el, const std::string& text,
                           const std::unordered_set<std::string>& parent_tags);
    std::string convert_li(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_ol(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_ul(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_p(tinyxml2::XMLElement* el, const std::string& text,
                         const std::unordered_set<std::string>& parent_tags);
    std::string convert_pre(tinyxml2::XMLElement* el, const std::string& text,
                           const std::unordered_set<std::string>& parent_tags);
    std::string convert_table(tinyxml2::XMLElement* el, const std::string& text,
                             const std::unordered_set<std::string>& parent_tags);
    std::string convert_tr(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_td(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);
    std::string convert_th(tinyxml2::XMLElement* el, const std::string& text,
                          const std::unordered_set<std::string>& parent_tags);

    std::string escape(const std::string& text,
                      const std::unordered_set<std::string>& parent_tags);

    std::string inline_markup(const std::string& markup_prefix, const std::string& text,
                             const std::unordered_set<std::string>& parent_tags);

    std::string strip_document(const std::string& text);
};

}