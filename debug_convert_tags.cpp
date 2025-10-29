#include <markdownify/options.hpp>
#include <iostream>

int main() {
    markdownify::Options options;
    options.convert_tags = {"b"};
    
    std::cout << "should_convert_tag('b'): " << options.should_convert_tag("b") << std::endl;
    std::cout << "should_convert_tag('em'): " << options.should_convert_tag("em") << std::endl;
    std::cout << "should_convert_tag('html'): " << options.should_convert_tag("html") << std::endl;
    
    return 0;
}
