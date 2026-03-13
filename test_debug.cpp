#include "src/map.hpp"
#include <iostream>
#include <cassert>
#include <string>

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

void tester(void) {
	//	test: constructor
	sjtu::map<Integer, std::string, Compare> map;
	//	test: empty(), size()
	assert(map.empty() && map.size() == 0);

	std::cout << "Inserting elements..." << std::endl;
	//	test: operator[], insert()
	for (int i = 0; i < 100; ++i) {
		std::string string = "";
		for (int number = i; number; number /= 10) {
			char digit = '0' + number % 10;
			string = digit + string;
		}
		if (i & 1) {
			map[Integer(i)] = string;
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(!result.second);
		} else {
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(result.second);
		}
	}

	std::cout << "Size after insertion: " << map.size() << std::endl;
	std::cout << "Deleting elements..." << std::endl;

	//	test: count(), find(), erase()
	for (int i = 0; i < 100; ++i) {
		if (i > 19 && i <= 39) {
			continue;
		}
		std::cout << "Erasing " << i << "..." << std::endl;
		assert(map.count(Integer(i)) == 1);
		assert(map.find(Integer(i)) != map.end());
		map.erase(map.find(Integer(i)));
	}

	std::cout << "Size after deletion: " << map.size() << std::endl;

	//	test: constructor, operator=, clear();
	for (int i = 0; i < (int)map.size(); ++i) {
		std::cout << "Copy iteration " << i << std::endl;
		sjtu::map<Integer, std::string, Compare> copy(map);
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		copy = map;
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
	}
	std::cout << std::endl;

	//	test: const_iterator, cbegin(), cend(), operator++, at()
	sjtu::map<Integer, std::string, Compare>::const_iterator const_iterator;
	const_iterator = map.cbegin();
	std::cout << "Forward iteration:" << std::endl;
	while (const_iterator != map.cend()) {
		const Integer integer(const_iterator->first);
		const_iterator++;
		std::cout << map.at(integer) << " ";
	}
	std::cout << std::endl;

	//	test: iterator, operator--, operator->
	sjtu::map<Integer, std::string, Compare>::iterator iterator;
	iterator = map.end();
	std::cout << "Backward iteration:" << std::endl;
	while (true) {
		sjtu::map<Integer, std::string, Compare>::iterator peek = iterator;
		if (peek == map.begin()) {
			std::cout << std::endl;
			break;
		}
		--iterator;
		std::cout << iterator->second << " ";
	}
}

int main(void) {
	tester();
	std::cout << "All tests passed!" << std::endl;
	return 0;
}
