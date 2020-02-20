#include <iostream>
#include <chrono>
#include <time.h>
#include <vector>
#include <memory>
#include <limits>
#include <cmath>

unsigned int MAX_POWER = 24;
unsigned int MIN_POWER = 8;
unsigned int NUM_ITER = 100000;
unsigned int NUM_READ = 100;

std::shared_ptr<std::vector<int32_t>> rand_list(size_t n)
{
	std::shared_ptr<std::vector<int32_t>> indices (new std::vector<int32_t>(n, 0));
	for (unsigned int i = 0; i < n; ++i)
	{
		indices->at(i) = static_cast<int32_t>(rand() % n);
	}
	return indices;
}

int main()
{
	srand(time(NULL)); //initialize seed for the random number generator
	std::cout << "# KB" << "\t" << "time" << "\n";

	for (unsigned int i = MIN_POWER; i <= MAX_POWER; ++i)
	{
		size_t curr_size = std::pow(2,i);
		std::cout << curr_size * 4 / 1024 << "\t";
		auto buffer = rand_list(curr_size);
		
		double min_time = std::numeric_limits<double>::infinity();
		for (unsigned int j = 0; j < NUM_ITER; ++j)
		{
			unsigned int index = rand() % curr_size;
			auto start = std::chrono::high_resolution_clock::now();
			for (unsigned int z = 0; z < NUM_READ; ++z)
			{
				index = buffer->at(index);
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto curr_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (curr_time < min_time)
			{
				min_time = static_cast<double>(curr_time);
			}
		}
		double avg_time = min_time; // / static_cast<double>(NUM_READ);
		std::cout << avg_time << "\n";
	}
}
