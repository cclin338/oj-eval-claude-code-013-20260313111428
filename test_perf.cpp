#include "src/map.hpp"
#include <iostream>
#include <string>
#include <cassert>

class Integer {
public:
    static int counter;
    int val;

    Integer(int val) : val(val) {
        counter++;
    }

    Integer(const Integer &rhs) {
        val = rhs.val;
        counter++;
    }

    Integer& operator = (const Integer &rhs) {
        assert(false);
        return *this;
    }

    ~Integer() {
        counter--;
    }
};

int Integer::counter = 0;

class Compare {
public:
    bool operator () (const Integer &lhs, const Integer &rhs) const {
        return lhs.val < rhs.val;
    }
};

int main() {
    sjtu::map<Integer, std::string, Compare> map;

    std::cout << "Inserting 1000 elements..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        map[Integer(i)] = std::to_string(i);
    }

    std::cout << "Size: " << map.size() << std::endl;

    std::cout << "Deleting 880 elements..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        if (i > 19 && i <= 139) {
            continue;
        }
        map.erase(map.find(Integer(i)));
    }

    std::cout << "Size after deletion: " << map.size() << std::endl;

    std::cout << "Testing copy operations..." << std::endl;
    for (int i = 0; i < (int)map.size() && i < 3; ++i) {
        sjtu::map<Integer, std::string, Compare> copy(map);
        std::cout << "Copy size: " << copy.size() << std::endl;
        map.clear();
        std::cout << "After clear: " << map.size() << std::endl;
        map = copy;
        std::cout << "After assign: " << map.size() << std::endl;
    }

    std::cout << "Test complete!" << std::endl;
    return 0;
}
