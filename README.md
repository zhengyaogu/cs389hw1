## Implementation of the Benchmark (Part 1)
The benchmark measures the per element reading time of vectors (served as our buffer) with 2^8 (1KB) to 2^21(8192KB) 32-bit integers.
To measure the desired metric, each vector of size `n` is read in a random order. The order of reading is implemented by a vector with non-repeating 0 - n-1 in it and is shuffled.
Then the benchmark iterates over the random index vector and increment the j-th element of the buffer if the current element in the random index vector is `j`.
The time it takes to iterate over the random index vector and read the according elements in the buffer is timed with c++'s `high_resolution_clock` and averaged over the totaled number of iterations before being printed in the console.

The random reading order is designed to disrupt the temporal and spatial locality of ordinary code, thus, preventing the computer from prefetching data in the buffer into lower cache levels they are originally located;
that is, the buffer would stay in the level of cache that is big enough to accomodate it. Although the random index buffer would take up space in cache, its behavior would be regular.
Since we are reading it by order, it would have a strong spatial locality, which means the computer is highly likely to allocate the indexes we need to read from in the fastest level of cache.
This means that the per element reading time of the random index vector would stay constant as its size grows. Thus, the change in time we measured would solely reflect the change in the per element time of the buffer.

The system clock does not have a fine enough granularity to time a single read from the buffer; we have to time the total time to read every single element of the buffer.
The problem is that the OS might interrupt our process and the time while the program is on halt would be part of the measured time. To resolve this issue, we time the reading of the same buffer a large number of times and choose the shortest measure as our reference.

## Analysis of Empirical Measure (Part 2 & 3)
The relationship between the per element time and the buffer size is shown in the plot below. For more numeric representation of the data, one can visit `example.dat`, where the time to the fourth decimal is recorded.
![Time per Element Read with Respect to Buffer Size](plot.png)

(Fig.1 Time per Element Read with Respect to Buffer Size from the Benchmark)

One sees a small drop from about 0.8 ns to 0.5 ns of per element read time from the buffer with size 1 KB to 32 KB. This is probably due to interruptions to the process rather than where the buffer is placed in caches. As the total reading time increases with size of the buffer, the proportion of the measured time that is due to interruption decreases.

The per element reading time jumps from 0.5ns to 1.2ns between 32 KB and 64KB, signifying that the L1 cache has a size between 32 and 64 KB. It turns out that the size of my L1 cache is indeed 32KB per core.

The per element reading time slowing climbs after 64 KB until 1024 KB. It is likely that the computer has L2 cache of size 256KB or 512 KB for a core. The reason that the plot does not step up on the plot might be that there are multiple threads for one core. Other threads might be occupying the cache space so a part of the buffer would be placed in L2 while the other part might be placed in a slower level. There is also always the chance that the computer distributes the buffer among different levels of caches at the time of initialization. Thus, even randomizing the access order would not stop the computer from loading the data into faster caches. As the the size of the buffer increases, more and more data has to be allocated in slower caches which leads to the slight climb in the graph. It turns out that the CPU has 256KB for each core.

The plot is inflicted at 2^11, which means L3 cache reach its limit when 2048KB of data is loaded. The size of the cache might be larger given that the cache might be inclusive, which means it has to contain contain copies of data in faster caches. I guess that the size of the cache is about 3MB. It turns out that the size of L3 cache is indeed 3M.