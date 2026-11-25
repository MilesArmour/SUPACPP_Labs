// AnalyseData.cxx
// Main steering script: reads (x, y) data and lets the user pick an analysis.
// Author: Miles Armour

// Usage: ./AnalyseData [input_file] [error_file]
// (the defaults below are used if no arguments are given)

#include "AnalyseData.h"
#include "CustomFunctions.h"

#include <iostream>
#include <limits>
#include <string>


// ----------------------------------------------------------------------
// Menu helpers
// ----------------------------------------------------------------------
void printMenu() {
    std::cout << "\n===== Data analysis menu =====\n"
              << " 1. Print the first N lines of data\n"
              << " 2. Calculate the magnitude of every point\n"
              << " 3. Fit a straight line (least squares) with chi2/NDOF\n"
              << " 4. Calculate x^y for every point (y rounded)\n"
              << " 0. Exit\n";
}

bool getIntFromUser(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return true;
        }
        if (std::cin.eof()) {
            return false;
        }
        // Bad input (e.g. letters): clear the error and discard the line
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a whole number.\n";
    }
}

bool askToContinue() {
    char answer;
    while (true) {
        std::cout << "\nPerform another action? (y/n): ";
        if (!(std::cin >> answer)) {
            return false;
        }
        if (answer == 'y' || answer == 'Y') return true;
        if (answer == 'n' || answer == 'N') return false;
        std::cout << "Please answer y or n.\n";
    }
}


// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
int main(int argc, char* argv[]) {
    std::string inputFile = "/workspaces/SUPACPP_Labs/Exercises/Lab1and2/input2D_float.txt";
    std::string errorFile = "/workspaces/SUPACPP_Labs/Exercises/Lab1and2/error2D_float.txt";
    if (argc >= 2) inputFile = argv[1];
    if (argc >= 3) errorFile = argv[2];

    DataPoints data = readData(inputFile);
    if (data.empty()) {
        std::cerr << "No data was read from " << inputFile << ". Exiting." << std::endl;
        return 1;
    }
    std::cout << "Read " << data.size() << " data points from " << inputFile << std::endl;

    bool running = true;
    while (running) {
        printMenu();
        int choice;
        if (!getIntFromUser("Select an option: ", choice)) {
            break;  // input ended
        }

        switch (choice) {
            case 1: {
                int N;
                if (!getIntFromUser("How many lines would you like to print? ", N)) {
                    running = false;
                    break;
                }
                printData(data, N);
                break;
            }
            case 2: {
                std::vector<double> magnitudes = calculateMagnitudes(data);
                printData(magnitudes, "magnitude");
                saveResults("magnitudes.txt", magnitudes, "magnitude");
                break;
            }
            case 3: {
                DataPoints errors = readData(errorFile);
                std::string result = fitLine(data, errors);
                printData(result);
                saveResults("line_fit.txt", result);
                break;
            }
            case 4: {
                std::vector<double> powers = calculatePowers(data);
                printData(powers, "x^y");
                saveResults("x_to_the_y.txt", powers, "x^y with y rounded to nearest integer");
                break;
            }
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid option, please choose from the menu.\n";
                continue;  // re-show the menu without asking to continue
        }

        if (running) {
            running = askToContinue();
        }
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}