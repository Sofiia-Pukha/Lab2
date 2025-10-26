#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 
#include <chrono> 
#include <random>
#include <execution> 
#include <thread> 
#include <numeric> 
#include <cmath>
#include <iomanip>
#include <functional>
#include <limits>
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

template <typename Func>
double time_operation(Func f)
{
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

int main() {
    std::cout << "Laboratory Work: Research transform " << std::endl;
    std::cout << "------------------------------------" << std::endl;
 
    std::vector<size_t> data_sizes = {100000, 1000000, 5000000};
    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Number of available cores: " << num_cores << std::endl;

    for(size_t size : data_sizes)
    {
        std::cout << "================================" << std::endl;
        std::cout << "Data size: " << size << "elements" << std::endl;
        std::cout << "================================" << std::endl;

        auto data = generate_data(size);
    std::vector <double> result(size);

    std::cout << std::fixed << std::setprecision(4);

    double time_ms;

std::cout << "--- Fast Operation ---" << std::endl;
time_ms = time_operation([&]() {
    std::transform(data.begin(), data.end(), result.begin(), fast_op);
});
std::cout << "1) std::transform (no policy): " << std::setw(10) << time_ms << " ms" << std::endl;

time_ms = time_operation([&]() {
    std::transform(std::execution::seq, data.begin(), data.end(), result.begin(), fast_op);
});
std::cout << " 2a) std:: transform (std::execution::seq): " << std::setw(10) << time_ms << " ms" << std::endl;

    time_ms = time_operation([&]() {
            std::transform(std::execution::par, data.begin(), data.end(), result.begin(), fast_op);
        });
        std::cout << "2b) std::transform (std::execution::par):" << std::setw(10) << time_ms << " ms" << std::endl;

        time_ms = time_operation([&]() {
            std::transform(std::execution::par_unseq, data.begin(), data.end(), result.begin(), fast_op);
        });
        std::cout << "2c) std::transform (std::execution::par_unseq):" << std::setw(10) << time_ms << " ms" << std::endl;
    }
    return 0;
}
