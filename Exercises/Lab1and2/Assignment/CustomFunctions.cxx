// CustomFunctions.cxx
// Implementations of the data-handling functions used by AnalyseData.cxx
// Author: Miles Armour

#include "CustomFunctions.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

// ======================================================================
// Reading
// ======================================================================
DataPoints readData(const std::string& filename) {
    DataPoints data;
    std::ifstream dataFile(filename);

    if (!dataFile.is_open()) {
        std::cerr << "Error: could not open " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(dataFile, line)) {
        // Treat commas as spaces so "1.0,2.0" and "1.0 2.0" both work
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        float x, y;
        if (ss >> x >> y) {
            data.push_back({x, y});
        }
        // otherwise (header, blank line, junk) the line is skipped
    }
    return data;
}

// ======================================================================
// Printing
// ======================================================================
void printData(const DataPoints& data, int N) {
    if (data.empty()) {
        std::cout << "No data to print." << std::endl;
        return;
    }
    if (N <= 0) {
        std::cout << "N must be a positive number." << std::endl;
        return;
    }

    std::size_t count = static_cast<std::size_t>(N);
    if (count > data.size()) {
        std::cout << "Warning: requested " << N << " lines but only "
                  << data.size() << " data points exist. "
                  << "Printing the first 5 instead." << std::endl;
        count = std::min<std::size_t>(5, data.size());
    }

    for (std::size_t i = 0; i < count; ++i) {
        std::cout << "x = " << data[i].first << ", y = " << data[i].second << std::endl;
    }
}

void printData(const std::vector<double>& values, const std::string& label) {
    if (values.empty()) {
        std::cout << "No " << label << " values to print." << std::endl;
        return;
    }
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << label << "[" << i << "] = " << values[i] << std::endl;
    }
}

void printData(const std::string& text) {
    std::cout << text << std::endl;
}

// ======================================================================
// Magnitudes
// ======================================================================
std::vector<double> calculateMagnitudes(const DataPoints& data) {
    std::vector<double> magnitudes;
    magnitudes.reserve(data.size());
    for (const auto& point : data) {
        const double x = point.first;
        const double y = point.second;
        magnitudes.push_back(std::sqrt(x * x + y * y));
    }
    return magnitudes;
}


// ======================================================================
// Least squares fit + chi-squared
// ======================================================================
double chiSquaredPerNDOF(const DataPoints& data, const DataPoints& errors,
                         double p, double q) {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    if (data.size() != errors.size()) {
        std::cerr << "Error: " << data.size() << " data points but "
                  << errors.size() << " errors." << std::endl;
        return nan;
    }
    // Two fitted parameters (p and q) => NDOF = N - 2
    if (data.size() <= 2) {
        std::cerr << "Error: need more than 2 points for a chi2/NDOF." << std::endl;
        return nan;
    }

    double chi2 = 0.0;
    for (std::size_t i = 0; i < data.size(); ++i) {
        const double sigma = errors[i].second;  // error on y
        if (sigma == 0.0) {
            std::cerr << "Error: zero error on point " << i << "." << std::endl;
            return nan;
        }
        const double expected = p * data[i].first + q;
        const double diff = data[i].second - expected;
        chi2 += (diff * diff) / (sigma * sigma);
    }
    const double ndof = static_cast<double>(data.size()) - 2.0;
    return chi2 / ndof;
}


std::string fitLine(const DataPoints& data, const DataPoints& errors) {
    const std::size_t n = data.size();
    if (n < 2) {
        return "Error: need at least 2 data points to fit a line.";
    }

    // Running sums needed by the least-squares formulae
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumXX = 0.0;
    for (const auto& point : data) {
        sumX += point.first;
        sumY += point.second;
        sumXY += static_cast<double>(point.first) * point.second;
        sumXX += static_cast<double>(point.first) * point.first;
    }

    const double N = static_cast<double>(n);
    const double denominator = N * sumXX - sumX * sumX;
    if (std::fabs(denominator) < 1e-12) {
        return "Error: all x values are identical, cannot fit y = px + q.";
    }

    const double p = (N * sumXY - sumX * sumY) / denominator;
    const double q = (sumXX * sumY - sumXY * sumX) / denominator;

    std::ostringstream out;
    out << "y = " << p << "x " << (q < 0 ? "- " : "+ ") << std::fabs(q) << "\n";

    const double chi2ndof = chiSquaredPerNDOF(data, errors, p, q);
    if (std::isnan(chi2ndof)) {
        out << "chi2/NDOF = unavailable";
    } else {
        out << "chi2/NDOF = " << chi2ndof;
    }

    return out.str();
}

// ======================================================================
// x^y by recursion
// ======================================================================
double intPower(double base, long exponent) {
    if (exponent < 0) {
        return 1.0 / intPower(base, -exponent);  // x^-n = 1 / x^n
    }
    if (exponent == 0) {
        return 1.0;  // base case
    }
    // Exponentiation by squaring: depth is ~log2(exponent), not exponent
    const double half = intPower(base, exponent / 2);
    return (exponent % 2 == 0) ? half * half : half * half * base;
}

std::vector<double> calculatePowers(const DataPoints& data) {
    std::vector<double> results(data.size());
    // std::transform applies the function to every point without an
    // explicit for/while loop in our code
    std::transform(data.begin(), data.end(), results.begin(),
                   [](const std::pair<float, float>& point) {
                       return intPower(point.first, std::lround(point.second));
                   });
    return results;
}

// ======================================================================
// Saving
// ======================================================================
void saveResults(const std::string& filename, const std::vector<double>& values,
                 const std::string& label) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not write to " << filename << std::endl;
        return;
    }
    file << "# " << label << " (one value per data point)\n";
    for (double v : values) {
        file << v << "\n";
    }
    std::cout << "Saved results to " << filename << std::endl;
}

void saveResults(const std::string& filename, const std::string& text) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not write to " << filename << std::endl;
        return;
    }
    file << text << "\n";
    std::cout << "Saved results to " << filename << std::endl;
}
