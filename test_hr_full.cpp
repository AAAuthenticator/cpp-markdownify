#include <markdownify/converter.hpp>
#include <iostream>

int main() {
    markdownify::Options opts;
    markdownify::MarkdownConverter conv(opts);
    
    std::string html = "<hr>";
    std::string result = conv.convert(html);
    
    std::cout << "Input:  [" << html << "]" << std::endl;
    std::cout << "Result: [" << result << "]" << std::endl;
    std::cout << "Length: " << result.length() << std::endl;
    
    // Check if it contains ---
    if (result.find("---") != std::string::npos) {
        std::cout << "Contains ---: YES" << std::endl;
    } else {
        std::cout << "Contains ---: NO" << std::endl;
    }
    
    return 0;
}
