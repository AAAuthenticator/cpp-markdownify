#include <markdownify/markdownify.hpp>
#include <gtest/gtest.h>



TEST(ConversionsTest, StripTags) {
    markdownify::Options options;
    options.strip_tags = {"a"};
    std::string html = "<b>Bold</b> <a href=\"http://example.com\">Link</a>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_EQ(result, "**Bold**");
}

TEST(ConversionsTest, ConvertTags) {
    markdownify::Options options;
    options.convert_tags = {"b"};
    std::string html = "<b>Bold</b> <em>Italic</em>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("**Bold**") != std::string::npos);
    EXPECT_TRUE(result.find("*Italic*") == std::string::npos);
}

TEST(ConversionsTest, UnderscoreEmphasis) {
    markdownify::Options options;
    options.strong_em_symbol = markdownify::StrongEmSymbol::UNDERSCORE;
    std::string html = "<em>Italic</em> <strong>Bold</strong>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("_Italic_") != std::string::npos);
    EXPECT_TRUE(result.find("__Bold__") != std::string::npos);
}

TEST(ConversionsTest, ATXClosedHeading) {
    markdownify::Options options;
    options.heading_style = markdownify::HeadingStyle::ATX_CLOSED;
    std::string html = "<h1>Title</h1>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("# Title #") != std::string::npos);
}

TEST(ConversionsTest, BackslashNewline) {
    markdownify::Options options;
    options.newline_style = markdownify::NewlineStyle::BACKSLASH;
    std::string html = "Line 1<br>Line 2";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("\\\n") != std::string::npos);
}

TEST(ConversionsTest, EscapeAsterisks) {
    markdownify::Options options;
    options.escape_asterisks = true;
    std::string html = "Text with * asterisk";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("\\*") != std::string::npos);
}

TEST(ConversionsTest, EscapeUnderscores) {
    markdownify::Options options;
    options.escape_underscores = true;
    std::string html = "Text with _ underscore";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("\\_") != std::string::npos);
}

TEST(ConversionsTest, CodeLanguage) {
    markdownify::Options options;
    options.code_language = "python";
    std::string html = "<pre><code>print('hello')</code></pre>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_TRUE(result.find("```python") != std::string::npos);
}

TEST(ConversionsTest, LinkWithTitle) {
    std::string html = R"(<a href="http://example.com" title="Example Site">Link</a>)";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("\"Example Site\"") != std::string::npos);
}

TEST(ConversionsTest, ImageWithTitle) {
    std::string html = R"(<img src="image.png" alt="Image" title="Image Title">)";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("\"Image Title\"") != std::string::npos);
}

TEST(ConversionsTest, NestedLists) {
    std::string html = R"(<ul>
<li>Item 1
<ul>
<li>Nested 1</li>
<li>Nested 2</li>
</ul>
</li>
<li>Item 2</li>
</ul>)";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("* Item 1") != std::string::npos);
    EXPECT_TRUE(result.find("+ Nested 1") != std::string::npos ||
                result.find("- Nested 1") != std::string::npos);
}

TEST(ConversionsTest, Strikethrough) {
    std::string html = "<del>Deleted text</del>";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("~~Deleted text~~") != std::string::npos);
}

TEST(ConversionsTest, ComplexDocument) {
    std::string html = R"(<h1>Title</h1>
<p>This is a <strong>complex</strong> document with <em>various</em> elements.</p>
<ul>
<li>List item 1</li>
<li>List item 2</li>
</ul>
<p>A <a href="https://example.com">link</a> and an image:</p>
<img src="photo.jpg" alt="Photo">)";

    markdownify::Options options;
    options.heading_style = markdownify::HeadingStyle::ATX;
    std::string result = markdownify::markdownify(html, options);

    EXPECT_TRUE(result.find("# Title") != std::string::npos);
    EXPECT_TRUE(result.find("**complex**") != std::string::npos);
    EXPECT_TRUE(result.find("*various*") != std::string::npos);
    EXPECT_TRUE(result.find("* List item 1") != std::string::npos);
    EXPECT_TRUE(result.find("[link](https://example.com)") != std::string::npos);
    EXPECT_TRUE(result.find("![Photo](photo.jpg)") != std::string::npos);
}