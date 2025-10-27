//g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0

#include <iostream>
#include <execution>
#include <vector>
#include <string>
#include <algorithm> 
#include <chrono>    
#include <random> 
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
    std::chrono::duration<double, std::milli> duration_ms = end - start;
    return duration_ms.count();
}

template <typename Op>
void run_all_tests_for_op(
    const std::string& op_name,
    Op op,
    const std::vector<double>& data,
    std::vector<double>& result, 
    unsigned int num_cores
)
{
    std::cout << "--- Testing: " << op_name << " ---" << std::endl;
    double time_ms;

    time_ms = time_operation([&]() {
        std::transform(data.begin(), data.end(), result.begin(), op);
    });
    std::cout << "1) std::transform (no policy):       " << std::setw(10) << time_ms << " ms" << std::endl;


    time_ms = time_operation([&]() {
        std::transform(std::execution::seq, data.begin(), data.end(), result.begin(), op);
    });
    std::cout << "2a) std::transform (std::execution::seq): " << std::setw(10) << time_ms << " ms" << std::endl;

    time_ms = time_operation([&]() {
        std::transform(std::execution::par, data.begin(), data.end(), result.begin(), op);
    });
    std::cout << "2b) std::transform (std::execution::par): " << std::setw(10) << time_ms << " ms" << std::endl;

    time_ms = time_operation([&]() {
        std::transform(std::execution::par_unseq, data.begin(), data.end(), result.begin(), op);
    });
    std::cout << "2c) std::transform (std::execution::par_unseq):" << std::setw(10) << time_ms << " ms" << std::endl;
    

    std::cout << "\n3) Proprietary parallel algorithm (my_parallel_transform):" << std::endl;

    unsigned int max_k = std::max(16u, num_cores * 2);
    max_k = std::min(max_k, 64u); 

    std::cout << "   ---------------------" << std::endl;
    std::cout << "   | K   | Time (ms)  |" << std::endl;
    std::cout << "   ---------------------" << std::endl;

    double min_time = std::numeric_limits<double>::max();
    unsigned int best_k = 0;

    std::function<double(double)> op_func = op;

    for (unsigned int k = 1; k <= max_k; ++k)
    {
        time_ms = time_operation([&]() {
            my_parallel_transform(data, result, op_func, k);
        });

        std::cout << "   | " << std::setw(3) << k << " | "
                  << std::setw(9) << time_ms << " |" << std::endl;

        if (time_ms < min_time)
        {
            min_time = time_ms;
            best_k = k;
        }
    }
    std::cout << "   ---------------------" << std::endl;

    std::cout << "   The best result for your own algorithm:" << std::endl;
    std::cout << "   - The best K: " << best_k << std::endl;
    std::cout << "   - Min time : " << min_time << " ms" << std::endl;
    std::cout << "   - Ratio (Best K / Hardware Threads): "
              << static_cast<double>(best_k) / num_cores << std::endl;
}

int main() {
    std::cout << "Laboratory Work: Research transform " << std::endl;
    std::cout << "------------------------------------" << std::endl;
 
    std::vector<size_t> data_sizes = {100000, 1000000, 5000000};

    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Number of hardware threads (cores): " << num_cores << std::endl;

    std::cout << std::fixed << std::setprecision(4);

    for(size_t size : data_sizes)
    {
        std::cout << "\n================================================" << std::endl;
        std::cout << "   DATA SIZE: " << size << " елементів" << std::endl;
        std::cout << "================================================" << std::endl;

        auto data = generate_data(size);
        std::vector<double> result(size); 

        run_all_tests_for_op(
            "fast_op (val * 1.5 - 10.0)", 
            fast_op, 
            data, 
            result, 
            num_cores
        );

        run_all_tests_for_op(
            "slow_op (sin/cos loop)", 
            slow_op, 
            data, 
            result, 
            num_cores
        );
    }
    return 0;
}
