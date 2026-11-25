#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm> 

std::vector<std::pair<float, float>> readData(const std::string& filename){
    std::vector<std::pair<float, float>> data;
    std::ifstream data_file(filename);

    if (!data_file.is_open()){
        std::cerr << "Error: could not open " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(data_file, line)){
        std::stringstream ss(line);
        std::string xStr, yStr;
        if (std::getline(ss, xStr, ',') && std::getline(ss, yStr)){
            try {
                data.push_back({std::stof(xStr), std::stof(yStr)});
            } catch (const std::exception&) {
                continue;
            }
        }
    }
    return data;
}

void printData(const std::vector<std::pair<float, float>>& data, int N){
    if (data.empty()){
        std::cout << "No data to print." << std::endl;
        return;
    }
    if (N <= 0){
        std::cout << "N must be a positive number." << std::endl;
        return;
    }

    int count = N;
    if (static_cast<std::size_t>(N) > data.size()){
        std::cout << "Warning: requested " << N << " lines but only "
                  << data.size() << " data points exist. Printing the first 5 instead."
                  << std::endl;
        count = static_cast<int>(std::min<std::size_t>(5, data.size()));
    }

    for (int i = 0; i < count; ++i){
        std::cout << "x = " << data[i].first << ", y = " << data[i].second << std::endl;
    }
}

int main(){
    std::string inputfile = "/workspaces/SUPACPP_Labs/Exercises/Lab1and2/input2D_float.txt";
    std::vector<std::pair<float, float>> data = readData(inputfile);

    int N;
    std::cout << "How many lines would you like to print? ";
    if (!(std::cin >> N)){
        std::cerr << "Error: please enter a whole number." << std::endl;
        return 1;
    }

    printData(data, N);
    return 0;
}