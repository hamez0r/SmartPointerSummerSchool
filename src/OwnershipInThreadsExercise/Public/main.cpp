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

// The program creates an array of 1000 integers, then passses it
// to two threads. Each thread will find the minimum value in a given range.
// We're using a smart pointer to be smart about our resources.

std::mutex mutex; // It's related to threads, don't worry about this

class ReferenceCounter {
public:
	ReferenceCounter() { }

	// No copy constructor
	ReferenceCounter(const ReferenceCounter& other) { }

	~ReferenceCounter() { }	

	// No asignment operator
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

class VectorUser {
public:
	VectorUser(std::vector<int>* resource)  { }

	VectorUser(const VectorUser& other) {

	}

	~VectorUser() {

	}

	auto operator=(const VectorUser& other) -> VectorUser& {

	}

	auto operator*() -> std::vector<int>& {
		return std::vector<int>(); // remove this line when you start coding
	}

	auto operator->() -> std::vector<int>* {
		return nullptr;
	}

private:
};

auto PrintMinimum(const size_t startRange, const size_t endRange, const int min) -> void {
	// If we remove the lock guard, the output might get scrambled. Go ahead, try it!
	std::lock_guard<std::mutex> lock(mutex);
	std::cout << "Minimum found in range " << startRange << " - " << endRange << ": " << min << std::endl;	
}

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

// Makes the thread sleep for some random amount of time
auto GetSomeRest() -> void {
	const auto sleepTime = 0 + (std::rand() % (100));
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

auto ShowMinimumInRange(VectorUser numbers, const size_t start, const size_t end) -> void {
	// This call has no real value, it's only to show that threads 
	// can finish their job in an arbitrary order
	GetSomeRest();

	// These are iterators that point to the positions we want
	// STL containers usually work with iterators
	auto startPosition = numbers->begin() + start;
	auto endPosition = numbers->begin() + end;
	
	// min_element is fonund in <algorithm>
	// Make sure to check out this header, you'll find a lot of cool functionality
	auto min = std::min_element(startPosition, endPosition);

	PrintMinimum(start, end, *min);
}

auto GenerateNumbers(const size_t size) -> VectorUser {
	auto numbers = VectorUser(new std::vector<int>(size));

	FillVector(*numbers, 0, size);
	return numbers;
}

auto main() -> int {
	// Don't worry about this, it's just something to make rand() work
	std::srand(0);

	const size_t size = 10;
	auto numbers = GenerateNumbers(size);
	PrintVector(*numbers);

	std::thread firstHalf(ShowMinimumInRange, numbers, 0, size / 2);
	std::thread secondHalf(ShowMinimumInRange, numbers, size / 2, size);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	firstHalf.join();
	secondHalf.join();

	return 0;
}