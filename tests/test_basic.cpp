#include <markdownify/markdownify.hpp>
#include <gtest/gtest.h>

TEST(BasicTest, SimpleText) {
    std::string html = "Hello World";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "Hello World");
}

TEST(BasicTest, BoldText) {
    std::string html = "<b>Bold text</b>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "**Bold text**");
}

TEST(BasicTest, ItalicText) {
    std::string html = "<em>Italic text</em>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "*Italic text*");
}

TEST(BasicTest, StrongText) {
    std::string html = "<strong>Strong text</strong>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "**Strong text**");
}

TEST(BasicTest, Link) {
    std::string html = "<a href=\"http://example.com\">Example</a>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "[Example](http://example.com)");
}

TEST(BasicTest, Autolink) {
    std::string html = "<a href=\"http://example.com\">http://example.com</a>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "<http://example.com>");
}

TEST(BasicTest, Image) {
    std::string html = "<img src=\"image.png\" alt=\"Image\">";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "![Image](image.png)");
}

TEST(BasicTest, Paragraph) {
    std::string html = "<p>This is a paragraph.</p>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "This is a paragraph.");
}

TEST(BasicTest, Heading1ATX) {
    markdownify::Options options;
    options.heading_style = markdownify::HeadingStyle::ATX;
    std::string html = "<h1>Heading 1</h1>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_EQ(result, "# Heading 1");
}

TEST(BasicTest, Heading2ATX) {
    markdownify::Options options;
    options.heading_style = markdownify::HeadingStyle::ATX;
    std::string html = "<h2>Heading 2</h2>";
    std::string result = markdownify::markdownify(html, options);
    EXPECT_EQ(result, "## Heading 2");
}

TEST(BasicTest, UnorderedList) {
    std::string html = R"(<ul>
<li>Item 1</li>
<li>Item 2</li>
<li>Item 3</li>
</ul>)";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("* Item 1") != std::string::npos);
    EXPECT_TRUE(result.find("* Item 2") != std::string::npos);
    EXPECT_TRUE(result.find("* Item 3") != std::string::npos);
}

TEST(BasicTest, OrderedList) {
    std::string html = R"(<ol>
<li>First</li>
<li>Second</li>
<li>Third</li>
</ol>)";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("1. First") != std::string::npos);
    EXPECT_TRUE(result.find("2. Second") != std::string::npos);
    EXPECT_TRUE(result.find("3. Third") != std::string::npos);
}

TEST(BasicTest, CodeInline) {
    std::string html = "<code>code</code>";
    std::string result = markdownify::markdownify(html);
    EXPECT_EQ(result, "`code`");
}

TEST(BasicTest, CodeBlock) {
    std::string html = "<pre><code>code block</code></pre>";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("```") != std::string::npos);
    EXPECT_TRUE(result.find("code block") != std::string::npos);
}

TEST(BasicTest, Blockquote) {
    std::string html = "<blockquote>Quote</blockquote>";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("> Quote") != std::string::npos);
}

TEST(BasicTest, HorizontalRule) {
    std::string html = "<hr>";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("---") != std::string::npos);
}

TEST(BasicTest, LineBreak) {
    std::string html = "Line 1<br>Line 2";
    std::string result = markdownify::markdownify(html);
    EXPECT_TRUE(result.find("Line 1  \nLine 2") != std::string::npos ||
                result.find("Line 1\\\nLine 2") != std::string::npos);
}