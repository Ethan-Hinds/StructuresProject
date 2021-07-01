#include "Entry.hpp"
#include <iostream>
#include <string>
#include <regex>

using namespace std;

//Constructor
Entry::Entry(string country, string dateStr, int changeInTotal, int total, float totalPerThou, float changeInTotalperThou) {
    this->dateStr = dateStr;
    this->changeInTotal = changeInTotal;
    this->total = total;
    this->totalPerThou = totalPerThou;
    this->changeInTotalPerThou = changeInTotalperThou;
    
    this->country = country.substr(0, country.find(" - "));
    this->dateInt =dateStrToInt(dateStr);
}

// Converts a date in m/d/yy format into days...sort of
// Converts it into the number of days since January 1, 2000, disregarding leap years
// It's primitive, but it works for our purposes.  Entry objects with a smaller dateInt is earlier in time

int Entry::dateStrToInt(string& dateStr) {
    // Regex :) Admire its beauty
    regex dateExp("(\\d+)\\/(\\d+)\\/(\\d+)");
    smatch m;
    regex_search(dateStr, m, dateExp);
    int month = stoi(m[1]);
    int day = stoi(m[2]);
    int year = stoi(m[3]);
    
    int monthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = year*365 + day;
    for (int i = 0; i < month - 1; i += 1) {
        days += monthLengths[i];
    }
    return days;
}

