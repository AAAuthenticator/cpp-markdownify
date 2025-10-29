#include <tinyxml2.h>
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;

    std::string html1 = "<html><img src=\"image.png\" alt=\"Image\"></html>";
    auto err = doc.Parse(html1.c_str());
    
    std::cout << "Parse result: " << err << std::endl;
    if (err != tinyxml2::XML_SUCCESS) {
        std::cout << "Error: " << doc.ErrorStr() << std::endl;
    }

    auto root = doc.RootElement();
    std::cout << "Root element: " << (root ? root->Name() : "null") << std::endl;
    
    if (root) {
        std::cout << "Children:" << std::endl;
        for (auto child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
            std::cout << "  - " << child->Name() << std::endl;
        }
    }

    return 0;
}
