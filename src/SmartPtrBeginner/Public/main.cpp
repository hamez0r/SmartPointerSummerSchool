#include <memory>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono>

// This program will let us see how smart pointers work

class ReferenceCounter {
public:
	ReferenceCounter() { }

	ReferenceCounter(const ReferenceCounter& other) { }

	~ReferenceCounter() { }	

	auto operator=(const ReferenceCounter& other) { }

	// Public API
	auto Incremet() -> void {
		
	}

	auto Decrement() -> void {
		
	}

	auto GetCount() const -> unsigned int {
		return 0;
	}

private:
	
};

class SmartPtr {
public:
	SmartPtr(std::vector<int>* resource)  { }

	SmartPtr(const SmartPtr& other) {

	}

	~SmartPtr() {

	}

	auto operator=(const SmartPtr& other) -> SmartPtr& {

	}

	auto operator*() -> std::vector<int>& {
		return std::vector<int>(); // remove this line when you start coding
	}

	auto operator->() -> std::vector<int>* {
		return nullptr;
	}

private:
};

auto PrintVector(const std::vector<int>& vec) -> void {
	for (const auto& entry : vec)
		std::cout << entry << " ";
	std::cout << std::endl;
}

auto FillVector(std::vector<int>& vec, const size_t minRange, const size_t maxRange) {
	for (auto& entry : vec) {
		int randomValue = minRange + (std::rand() % (maxRange - minRange + 1));
		entry = randomValue;
	}
}

auto GenerateNumbers(const size_t size) -> SmartPtr {
	auto numbers = SmartPtr(new std::vector<int>(size));

	FillVector(*numbers, 0, size);
	return numbers;
}

auto main() -> int {
	const size_t size = 10;
	auto numbers = GenerateNumbers(size);
	PrintVector(*numbers);

	{
		SmartPtr sameNumbers = numbers;
		SmartPtr alsoSameNumbers = numbers;
	}

	{
		SmartPtr sameNumbers = numbers;
	}

	return 0;
}