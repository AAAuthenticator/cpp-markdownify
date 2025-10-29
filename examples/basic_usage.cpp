#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    std::cout << "=== Basic HTML to Markdown Conversion ===" << std::endl << std::endl;

    std::string html1 = "<b>Yay</b> <a href=\"http://github.com\">GitHub</a>";
    std::cout << "HTML: " << html1 << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(html1) << std::endl << std::endl;

    std::string html2 = R"(<h1>Heading 1</h1>
<p>This is a <strong>paragraph</strong> with <em>emphasis</em>.</p>
<ul>
<li>Item 1</li>
<li>Item 2</li>
<li>Item 3</li>
</ul>)";

    std::cout << "HTML: " << html2 << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(html2) << std::endl << std::endl;

    markdownify::Options options;
    options.strip_tags = {"a"};
    std::cout << "=== With strip tags ===" << std::endl;
    std::cout << "HTML: " << html1 << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(html1, options) << std::endl << std::endl;

    markdownify::Options atx_options;
    atx_options.heading_style = markdownify::HeadingStyle::ATX;
    std::cout << "=== ATX Heading Style ===" << std::endl;
    std::cout << "HTML: <h2>Heading 2</h2>" << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify("<h2>Heading 2</h2>", atx_options) << std::endl << std::endl;

    std::string code_html = R"(<pre><code>def hello():
    print("Hello, World!")
</code></pre>)";

    markdownify::Options code_options;
    code_options.code_language = "python";
    std::cout << "=== Code Block with Language ===" << std::endl;
    std::cout << "HTML: " << code_html << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(code_html, code_options) << std::endl;

    std::string link_html = R"(<a href="https://example.com">https://example.com</a>)";
    std::cout << "=== Autolink ===" << std::endl;
    std::cout << "HTML: " << link_html << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(link_html) << std::endl << std::endl;

    std::string img_html = R"(<img src="logo.png" alt="Logo">)";
    std::cout << "=== Image ===" << std::endl;
    std::cout << "HTML: " << img_html << std::endl;
    std::cout << "Markdown: " << markdownify::markdownify(img_html) << std::endl;

    return 0;
}