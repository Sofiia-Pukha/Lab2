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
    
}

int main() {
    std::cout << "Laboratory Work: Research transform " << std::endl;
    std::cout << "------------------------------------" << std::endl;
 
    auto data = generate_data(10);
    std::vector <double> result(data.size());

    my_parallel_transform(data, result, fast_op, 4);
    return 0;
}
