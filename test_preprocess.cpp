#include <regex>
#include <iostream>

std::string preprocess_html(const std::string& html) {
    static const std::regex self_closing_pattern(
        R"(<(area|base|br|col|embed|hr|img|input|link|meta|param|source|track|wbr)([^>]*)>)",
        std::regex_constants::icase
    );

    std::string result = std::regex_replace(html, self_closing_pattern, "<$1$2/>");
    return result;
}

int main() {
    std::string html1 = "<hr>";
    std::string html2 = "<img src='test.png' alt='Test'>";
    
    std::cout << "Input:  [" << html1 << "]" << std::endl;
    std::cout << "Output: [" << preprocess_html(html1) << "]" << std::endl;
    std::cout << std::endl;
    std::cout << "Input:  [" << html2 << "]" << std::endl;
    std::cout << "Output: [" << preprocess_html(html2) << "]" << std::endl;
    
    return 0;
}
