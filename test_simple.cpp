#include "src/map.hpp"
#include <iostream>
#include <string>

int main() {
    sjtu::map<int, std::string> m;

    std::cout << "Empty: " << m.empty() << std::endl;
    std::cout << "Size: " << m.size() << std::endl;

    // Test insert
    auto r1 = m.insert(sjtu::pair<const int, std::string>(1, "one"));
    std::cout << "Inserted: " << r1.second << std::endl;

    auto r2 = m.insert(sjtu::pair<const int, std::string>(1, "ONE"));
    std::cout << "Re-Inserted: " << r2.second << std::endl;

    std::cout << "Size: " << m.size() << std::endl;

    // Test find
    auto it = m.find(1);
    if (it != m.end()) {
        std::cout << "Found: " << it->second << std::endl;
    }

    // Test operator[]
    m[2] = "two";
    std::cout << "m[2] = " << m[2] << std::endl;
    std::cout << "Size: " << m.size() << std::endl;

    // Test iteration
    std::cout << "All elements:" << std::endl;
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << "  " << it->first << " => " << it->second << std::endl;
    }

    // Test erase
    std::cout << "Erasing key 1..." << std::endl;
    m.erase(m.find(1));
    std::cout << "Size: " << m.size() << std::endl;

    std::cout << "Test passed!" << std::endl;
    return 0;
}
