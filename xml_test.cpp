#include <tinyxml2.h>
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;

    std::cout << "=== Test 1: IMG tag ===" << std::endl;
    std::string html1 = "<html><img src=\"image.png\" alt=\"Image\"></html>";
    doc.Parse(html1.c_str());

    auto root = doc.RootElement();
    if (root) {
        std::cout << "Root: " << root->Name() << std::endl;
        auto img = root->FirstChildElement("img");
        if (img) {
            std::cout << "Found img" << std::endl;
            std::cout << "src: " << (img->Attribute("src") ? img->Attribute("src") : "null") << std::endl;
            std::cout << "alt: " << (img->Attribute("alt") ? img->Attribute("alt") : "null") << std::endl;
        } else {
            std::cout << "No img found!" << std::endl;
        }
    }

    std::cout << "\n=== Test 2: HR tag ===" << std::endl;
    doc.Clear();
    std::string html2 = "<html><hr></html>";
    doc.Parse(html2.c_str());

    root = doc.RootElement();
    if (root) {
        auto hr = root->FirstChildElement("hr");
        if (hr) {
            std::cout << "Found hr" << std::endl;
        } else {
            std::cout << "No hr found!" << std::endl;
        }
    }

    std::cout << "\n=== Test 3: HR self-closing ===" << std::endl;
    doc.Clear();
    std::string html3 = "<html><hr/></html>";
    doc.Parse(html3.c_str());

    root = doc.RootElement();
    if (root) {
        auto hr = root->FirstChildElement("hr");
        if (hr) {
            std::cout << "Found hr (self-closing)" << std::endl;
        } else {
            std::cout << "No hr found (self-closing)!" << std::endl;
        }
    }

    return 0;
}