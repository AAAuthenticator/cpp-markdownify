#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    std::cout << "Test 1 - Image:" << std::endl;
    std::string html1 = "<img src=\"image.png\" alt=\"Image\">";
    std::string result1 = markdownify::markdownify(html1);
    std::cout << "HTML: " << html1 << std::endl;
    std::cout << "Result: [" << result1 << "]" << std::endl;
    std::cout << "Length: " << result1.length() << std::endl << std::endl;

    std::cout << "Test 2 - HR:" << std::endl;
    std::string html2 = "<hr>";
    std::string result2 = markdownify::markdownify(html2);
    std::cout << "HTML: " << html2 << std::endl;
    std::cout << "Result: [" << result2 << "]" << std::endl << std::endl;

    std::cout << "Test 3 - Ordered List:" << std::endl;
    std::string html3 = R"(<ol>
<li>First</li>
<li>Second</li>
<li>Third</li>
</ol>)";
    std::string result3 = markdownify::markdownify(html3);
    std::cout << "HTML: " << html3 << std::endl;
    std::cout << "Result: [" << result3 << "]" << std::endl;

    return 0;
}