// AnalyseData.h
// Declarations of the menu/input helpers used by AnalyseData.cxx
// Author: Miles Armour

#ifndef ANALYSEDATA_H
#define ANALYSEDATA_H

#include <string>

// Prints the list of available actions.
void printMenu();

// Prompts until the user enters a whole number. Returns false if input
bool getIntFromUser(const std::string& prompt, int& value);

// Asks "perform another action?" until the user answers y or n.
bool askToContinue();

#endif // ANALYSEDATA_H