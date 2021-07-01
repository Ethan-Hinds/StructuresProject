#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Entry.hpp"

using namespace std;

// Declare any methods in the main file here, before implementing them below
void loadData(string filePath, vector<Entry*>& entries);



int main() {
    // Vector of all of the entries
    vector<Entry*> entries;
    loadData("covid-testing-all-observations.csv", entries);
    if (entries.size() == 0) {
        cout << "Unable to load data!" << endl;
        return 0;
    }
    cout << "done" << endl;
    // The entries are loaded into the entires vector, and can now be accessed.
    //cout << entries.at(0)->country << endl;
    //cout << entries.at(0)->dateStr << endl;
    //cout << entries.at(1)->total << endl;
}









// Loads the data from the csv file and populates the entries vector with Entry objects
void loadData(string filePath, vector<Entry*>& entries) {
    cout << "Loading data..." << endl;
    ifstream file;
    file.open(filePath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream stream(line);
            string country;
            string dateStr;
            string changeInTotal;
            string total;
            string totalPerThou;
            string changeInTotalPerThou;
            getline(stream, country, ',');
            getline(stream, dateStr, ',');
            getline(stream, changeInTotal, ',');
            getline(stream, total, ',');
            getline(stream, totalPerThou, ',');
            getline(stream, changeInTotalPerThou, ',');
            Entry* entry = new Entry(country, dateStr, stoi(changeInTotal), stoi(total), stof(totalPerThou), stof(changeInTotalPerThou));
            entries.push_back(entry);
        }
    }
}

