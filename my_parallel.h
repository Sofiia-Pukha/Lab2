#pragma once 

#include <vector>
#include <functional>

void my_parallel_transform(
    const std::vector<double>& input,
    std::vector<double>& output,
    const std::function<double(double)>& op,
    unsigned int num_threads
);
