#include "src/map.hpp"
#include <iostream>
#include <string>

int main() {
    sjtu::map<int, std::string> m;

    for (int i = 0; i < 10; ++i) {
        m.insert(sjtu::pair<const int, std::string>(i, std::to_string(i)));
    }

    std::cout << "Size: " << m.size() << std::endl;
    std::cout << "Begin: " << m.begin()->first << std::endl;
    std::cout << "Erasing all elements..." << std::endl;

    int count = 0;
    while (m.begin() != m.end()) {
        std::cout << "Iteration " << count++ << ", size=" << m.size() << ", erasing " << m.begin()->first << std::endl;
        m.erase(m.begin());
    }

    std::cout << "Done! Size: " << m.size() << std::endl;
    return 0;
}
