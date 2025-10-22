#include <iostream>
#include <vector>
#include <algorithm> 
#include <chrono> 
#include <random>
#include <thread> 
#include <numeric> 
#include <cmath>
#include <iomanip>
#include "my_parallel.h"


std::vector<double> generate_data(size_t size)
{
    std::vector<double> data(size);
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 100.0);

    for (size_t i = 0; i < size; ++i)
    {
        data[i] = dist(gen);
    }
    return data;
}

auto fast_op = [] (double val) 
{
    return val * 1.5 - 10.0;
};

double slow_op(double val)
{
double result = val;

for (int i = 0; i < 50; ++i)
{
    result = std::sin(result) + std::cos(val);
}
return result;
}

int main() {
    std::cout << "Laboratory Work: Research transform " << std::endl;
    std::cout << "------------------------------------" << std::endl;
 
    auto data = generate_data(10);
    std::vector <double> result(data.size());

    my_parallel_transform(data, result, fast_op, 4);
    return 0;
}
