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
	ReferenceCounter() : m_counter(1) { }

	// No copy constructor
	ReferenceCounter(const ReferenceCounter& other) = delete;

	~ReferenceCounter() { }	

	// No asignment operator
	auto operator=(const ReferenceCounter& other) = delete;

	// Public API
	auto Incremet() -> void {
		m_counter++;
	}

	auto Decrement() -> void {
		m_counter--;
	}

	auto GetCount() const -> unsigned int {
		return m_counter;
	}

private:
	unsigned int m_counter;
};

class VectorUser {
public:
	VectorUser(std::vector<int>* resource) 
		: m_resource(resource), m_refCounts(new ReferenceCounter()) { }

	VectorUser(const VectorUser& other) {
		m_resource = other.m_resource;
		m_refCounts = other.m_refCounts;
		m_refCounts->Incremet();
	}

	~VectorUser() {
		m_refCounts->Decrement();

		auto refCount = m_refCounts->GetCount();
		if (refCount == 0) {
			delete m_refCounts;
			delete m_resource;
		}
	}

	auto operator=(const VectorUser& other) -> VectorUser& {
		if (this == &other) return *this;

		m_refCounts->Decrement();
		auto refCount = m_refCounts->GetCount();
		if (refCount == 0) {
			delete m_refCounts;
			delete m_resource;
		}

		m_resource = other.m_resource;
		m_refCounts = other.m_refCounts;
		m_refCounts->Incremet();
		
		return *this;
	}

	auto operator*() -> std::vector<int>& {
		return *m_resource;
	}

	auto operator->() -> std::vector<int>* {
		return m_resource;
	}

private:
	std::vector<int>* m_resource;
	ReferenceCounter* m_refCounts;
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

auto RunThreads() -> void {
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
}

auto ObserveScopes() -> void {
	const size_t size = 10;
	auto numbers = GenerateNumbers(size);
	PrintVector(*numbers);

	{
		auto sameNumbers = numbers;
		auto alsoSameNumbers = numbers;
	}

	{
		VectorUser sameNumbers = numbers;
	}
}

auto main() -> int {
	ObserveScopes();
	RunThreads();

	return 0;
}