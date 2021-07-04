#pragma once

#include <string>

using namespace std;

class Entry {
public:
    string country;
    int dateInt; // Use this for sorting purposes.  This value is (roughly) the number of days since Jan. 1, 2000 (See comments in Entry.cpp)
    string dateStr; // This is just used temporarily for reading from the data file and maybe for displaying the date.
    int changeInTotal;
    int total;
    float totalPerThou;
    float changeInTotalPerThou;
    Entry(string country, string dateStr, int changeInTotal, int total, float totalPerThou, float changeInTotalperThou);
private:
    // Converts a date in m/d/yy into days (sort of) see comments in Entry.cpp
    int dateStrToInt(string& dateStr);
};

