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

auto GenerateNumbers(const size_t size) -> std::vector<int>* {
	auto numbers = new std::vector<int>(size);

	FillVector(*numbers, 0, size);
	return numbers;
}

class VectorUser {
public:
    VectorUser(std::vector<int>* vec) : m_vector(vec) { }
    ~VectorUser() { delete m_vector; }

    auto Vector() const -> const std::vector<int>& { return *m_vector; }

    private:
    std::vector<int>* m_vector;
};

auto main() -> int {
	const size_t size = 10;
	auto numbers = GenerateNumbers(size);
	PrintVector(*numbers);

	{
		auto sameNumbers = VectorUser(numbers);
		auto alsoSameNumbers = VectorUser(numbers);

        PrintVector(sameNumbers.Vector());
        PrintVector(alsoSameNumbers.Vector());
	}

	{
		auto sameNumbers = VectorUser(numbers);
        PrintVector(sameNumbers.Vector());
	}

	return 0;
}