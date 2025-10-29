#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    std::string html = "<hr>";
    std::string result = markdownify::markdownify(html);
    std::cout << "Result: [" << result << "]" << std::endl;
    std::cout << "Length: " << result.length() << std::endl;
    
    for (size_t i = 0; i < result.length() && i < 50; i++) {
        std::cout << "char[" << i << "] = '" << result[i] << "' (ASCII " << (int)result[i] << ")" << std::endl;
    }
    
    return 0;
}
