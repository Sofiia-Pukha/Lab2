#include "my_parallel.h"
#include <vector>
#include <functional>
#include <thread>
#include <iostream>
#include <algorithm>

void my_parallel_transform(
    const std::vector<double>& input,
    std::vector<double>& output,
    const std::function<double(double)>& op,
    unsigned int num_threads
)
{
if (num_threads == 0)
{
    return;
}

size_t total_size = input.size();
size_t chunk_size = (total_size + num_threads - 1) / num_threads;

std::vector<std::thread> threads;
 
for(unsigned int i = 0; i < num_threads; ++i)
{
    size_t start = i * chunk_size;
    size_t end = std::min(start + chunk_size, total_size);

    if (start >=  end)
    {
        continue; 
    }

    threads.emplace_back([&, start, end]() {
        for (size_t j = start; j < end; ++j)
        {
            output[j] = op(input[j]);
        }
    });
}
for (auto& t : threads)
{
    t.join(); 
}
}
