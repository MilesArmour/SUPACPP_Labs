// CustomFunctions.h
// Declarations for the data-handling functions used by AnalyseData.cxx
// Author: Miles Armour


#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

#include <string>
#include <utility>
#include <vector>

// One (x, y) pair per entry. A vector is used because the number of
// data points is not known in advance.
using DataPoints = std::vector<std::pair<float, float>>;

// ---- Reading ---------------------------------------------------------
// Reads "x,y" (or "x y") lines from a file.
DataPoints readData(const std::string& filename);


// ---- Printing (overloaded so every result uses the same function) ----
// Prints the first N points. If N > data.size() a warning is shown and
// only the first 5 points are printed.
void printData(const DataPoints& data, int N);
// Prints a list of computed values, one per line, with a label.
void printData(const std::vector<double>& values, const std::string& label);
// Prints a block of text 
void printData(const std::string& text);


// ---- Analysis --------------------------------------------------------
// Magntiude sqrt(x^2 + y^2) of every point (vector from the origin).
std::vector<double> calculateMagnitudes(const DataPoints& data);

// Least-squares straight-line fit y = px + q
std::string fitLine(const DataPoints& data, const DataPoints& errors);

// chi2 / NDOF for the line y = px + q (NDOF = N - 2)
double chiSquaredPerNDOF(const DataPoints& data, const DataPoints& errors,
                         double p, double q);

// base^exponent for an integer exponent
double intPower(double base, long exponent);

// x^y for every point, where y rounded to the nearest whole number.
std::vector<double> calculatePowers(const DataPoints& data);

// ---- Saving (overloaded) ---------------------------------------------
void saveResults(const std::string& filename, const std::vector<double>& values,
                 const std::string& label);
void saveResults(const std::string& filename, const std::string& text);

#endif // CUSTOMFUNCTIONS_H
