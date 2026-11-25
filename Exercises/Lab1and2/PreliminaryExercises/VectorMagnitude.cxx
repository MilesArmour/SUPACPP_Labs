#include <iostream>
#include <cmath>

//Calculates and prints the magnitude of a 2D vector
int main(){

    double x = 7.5;
    double y = 3.4;

    double magnitude = std::sqrt((x * x) + (y * y));
    std::cout << magnitude << std::endl;
}