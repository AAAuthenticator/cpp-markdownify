#pragma once

#include "markdownify/converter.hpp"
#include "markdownify/options.hpp"
#include <string>

namespace markdownify {

inline std::string markdownify(const std::string& html, const Options& options = Options()) {
    MarkdownConverter converter(options);
    return converter.convert(html);
}

}