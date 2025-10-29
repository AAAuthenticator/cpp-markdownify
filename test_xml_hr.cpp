#include <tinyxml2.h>
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;
    std::string html = "<html><hr/></html>";
    
    auto err = doc.Parse(html.c_str());
    std::cout << "Parse error: " << err << std::endl;
    
    auto root = doc.RootElement();
    if (root) {
        std::cout << "Root: " << root->Name() << std::endl;
        
        for (auto child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
            std::cout << "Child: " << child->Name() << std::endl;
            const char* text = child->GetText();
            std::cout << "Text: " << (text ? text : "(null)") << std::endl;
        }
    }
    
    return 0;
}
