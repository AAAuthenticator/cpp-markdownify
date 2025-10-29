#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    markdownify::Options options;
    options.convert_tags = {"b"};
    std::string html = "<b>Bold</b> <em>Italic</em>";
    std::string result = markdownify::markdownify(html, options);
    
    std::cout << "HTML: " << html << std::endl;
    std::cout << "Result: [" << result << "]" << std::endl;
    std::cout << "Contains **Bold**: " << (result.find("**Bold**") != std::string::npos ? "YES" : "NO") << std::endl;
    std::cout << "Contains *Italic*: " << (result.find("*Italic*") != std::string::npos ? "YES" : "NO") << std::endl;
    
    return 0;
}
