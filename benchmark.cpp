#include <iostream>
#include <chrono>
#include <time.h>
#include <vector>
#include <memory>
#include <limits>
#include <cmath>
#include <algorithm>
#include <random>


unsigned int MAX_POWER = 21;
unsigned int MIN_POWER = 8;
unsigned int NUM_ITER = 10000;
unsigned int NUM_READ = 1000;

/* initiate a vector with random numbers smaller than its size */
void init_rand_list(std::vector<int32_t> buf)
{
	for (auto it = buf.begin(); it < buf.end(); ++it)
	{
		*it = static_cast<int32_t>(rand() % buf.size());
	}
}

int main()
{
	/* set the time-seed for random engines */
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(time(NULL)); //initialize seed for the random number generator

	std::cout << "# log(KB)" << "\t" << "time" << "\n";

	/*iterate vector size from 2^8 to 2^21 elements */
	for (unsigned int i = MIN_POWER; i <= MAX_POWER; ++i)
	{
		size_t curr_size = std::pow(2,i);

		std::cout << i - 8<< "\t"; // calculate how many log_2 KB current vector occupies
		
		// set up the random vector with current size
		std::vector<int32_t> buffer(curr_size, 0);
		init_rand_list(buffer);

		// set up a vector that contains shuffled indices that we will read one by one	
		std::vector<int32_t> rand_indices(curr_size, 0);
		std::iota(rand_indices.begin(), rand_indices.end(), 0);
		shuffle(rand_indices.begin(), rand_indices.end(), std::default_random_engine(seed));

		double min_time = std::numeric_limits<double>::infinity();
		
		// repeat timing the NUM_ITER readings and choose the shortest run
		for (unsigned int j = 0; j < NUM_ITER; ++j)
		{
			auto start = std::chrono::high_resolution_clock::now();
			
			// go through the random index vector and read from the buffer using those indices
			for (auto& it : rand_indices)
			{
				buffer[it]++;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto curr_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (static_cast<double>(curr_time) < min_time)
			{
				min_time = static_cast<double>(curr_time);
			}
		}
		double avg_time = min_time / static_cast<double>(curr_size);
		std::cout << avg_time << "\n";
	}
}
