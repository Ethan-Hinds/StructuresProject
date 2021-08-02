//#include <SFML/Graphics.hpp>
//#include "ResourcePath.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Entry.h"
#include "BarGraph.hpp"


using namespace std;

// Declare any methods in the main file here, before implementing them below
void loadData(string filePath, vector<Entry*>& entries);
//void graphData(vector<vector<Entry*>>& entries);
float Selection(Entry* entry, int& inputChoice);
void merge(vector<Entry*>& arr, int left, int mid, int right, int choice);
void mergeSort(vector<Entry*>& arr, int left, int right, int choice);
void quickSort(vector<Entry*>& arr, int low, int high, int choice);
int split(vector<Entry*>& arr, int low, int high, int choice);
void generateMap(map<float, vector<Entry*>>& map, vector<Entry*>& arr, int choice);
void barGraphData(vector<Entry*>& data, int inputChoice);


// Declare any global variables here
Graph* graph = nullptr;
BarGraph* barGraph = nullptr;


int main() {
    // Vector of all of the entries
    vector<Entry*> entries;
    loadData("covid-testing-all-observations.csv", entries);
    if (entries.size() == 0) {
        cout << "Unable to load data!" << endl;
        return 0;
    }
    cout << "done" << endl;
    // The entries are loaded into the entries vector, and can now be accessed.
    //cout << entries.at(0)->country << endl;
    //cout << entries.at(0)->dateStr << endl;
    //cout << entries.at(1)->total << endl;



    // The code below is just some sample code I left in place to demonstrate the graphing feature
    // Feel free to delete it. I have it saved
    // Currently graphs these random countries I chose
    // Can graph up to 5 countries at once

    vector<Entry*> v1;
    vector<Entry*> v2;
    vector<Entry*> v3;
    vector<Entry*> v4;
    vector<Entry*> v5;

    for (auto& it : entries) {
        if (it->country == "Albania") {
            v1.push_back(it);
        }
        else if (it->country == "Argentina") {
            v2.push_back(it);
        }
        else if (it->country == "Norway") {
            v3.push_back(it);
        }
        else if (it->country == "Bangladesh") {
            v4.push_back(it);
        }
        else if (it->country == "Estonia") {
            v5.push_back(it);
        }
    }
    vector<vector<Entry*>> data = { v1, v2, v3, v4, v5 };
    //graphData(data);
    

    vector<Entry*> vTest;

    /*for (int i = 0; i < 5; i++) {
        v1[i]->PrintEntry();
    }

    for (int i = 0; i < 5; i++) {
        vTest.push_back(v1[i]);
    }*/

    //quickSort(v1, 0, v1.size() - 1, 4);
    
    //cout << endl << endl << endl;

    /*for (int i = 0; i < 100; i++) {
        v1[i]->PrintEntry();
    }*/

    map<float, vector<Entry*>> testMap;

    generateMap(testMap, v1, 4);

    int t = 0;
    for (auto iter = testMap.begin(); iter != testMap.end(); ++iter) {
        t++;
        if (t == 5) { break; }
        for (int i = 0; i < iter->second.size(); i++) {
            iter->second[i]->PrintEntry();
        }
    }


    bool isRun = true;
    bool correctInput = true;

    string userChoice;

    while (isRun) {
        if (correctInput) {
            cout << "Type the following integer to select menu option: " << endl;
            cout << "1. Print a Data Entry Based on Country and Date" << endl;
            cout << "2. Print a Ranking of Data Entries for a Specific Date based on User Selected Criteria" << endl;
            cout << "3. Compare Timelines of up to 5 Countries based on User Selected Criteria" << endl;
            cout << "4. Print a Ranking of which Countries were Quickest to Respond Based on First Reported Testing Date" << endl;
            cout << "5. End Program" << endl;
        }

        cin >> userChoice;

        correctInput = true;

        if (userChoice == "1") {
            bool run1 = true;
            while (run1) {
                cout << "Please Type a Country: ";
                cin >> userChoice;
                string selCountry = userChoice;
                cout << endl << "Please Input a Date in the form MM/DD/YY (Please note don't include any leading zeros): ";
                cin >> userChoice;
                string selDate = userChoice;

                int i = 0;
                while (i < entries.size()) {
                    if (entries[i]->country == selCountry) {
                        if (entries[i]->dateStr == selDate) {
                            cout << endl;
                            entries[i]->PrintEntry();
                            cout << endl;
                            run1 = false;
                            break;
                        }
                    }
                    i++;
                }
                if (i == entries.size()) {
                    cout << "Data not found Please Try Again" << endl;
                }
            }
        }
        else if (userChoice == "2") {
            bool run2 = true;
            while (run2) {
                cout << endl << "Please Input a Date in the form MM/DD/YY (Please note don't include any leading zeros): ";
                cin >> userChoice;
                string selDate = userChoice;
                vector<Entry*> dateList;
                int i = 0;
                while (i < entries.size()) {
                    if (entries[i]->dateStr == selDate) {
                        dateList.push_back(entries[i]);
                    }
                    i++;
                }
                run2 = false;
                if (dateList.empty()) {
                    cout << "Data not found Please Try Again" << endl;
                    run2 = true;
                }
                else {
                    cout << "Please Select a Value to Sort By:" << endl;
                    cout << "1. Tests Performed on Date in Question" << endl;
                    cout << "2. Total Cumulative Tests Performed to Date" << endl;
                    cout << "3. Tests Performed on Date in Question per Thousand People of the Country's Population" << endl;
                    cout << "4. Total Cumulative Tests Performed to Date per Thousand People of the Country's Population" << endl;

                    int choice;

                    bool run22 = true;

                    while (run22) {
                        run22 = false;
                        cin >> userChoice;
                        if (userChoice == "1") {
                            choice = 2;
                        }
                        else if (userChoice == "2") {
                            choice = 1;
                        }
                        else if (userChoice == "3") {
                            choice = 4;
                        }
                        else if (userChoice == "4") {
                            choice = 3;
                        }
                        else {
                            run22 = true;
                            cout << "Input not recognized please try again" << endl;
                        }
                    }

                    vector<Entry*> dateListMerge = dateList;

                    mergeSort(dateListMerge, 0, dateListMerge.size() - 1, choice);

                    map<float, vector<Entry*>> dateMap;

                    generateMap(dateMap, dateList, choice);

                    quickSort(dateList, 0, dateList.size() - 1, choice);

                    for (int i = 0; i < dateList.size(); i++) {
                        cout << dateList[i]->country << ": " << Selection(dateList[i], choice) << endl;
                    }



                }
            }
        }
        else if (userChoice == "3") {

        }
        else if (userChoice == "4") {

        }
        else if (userChoice == "5") {
            isRun = false;
        }
        else {
            cout << "Input not recognized please try again" << endl;
            correctInput = false;
        }


    }

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

void barGraphData(vector<Entry*>& data, int inputChoice) {
    // Initialize SFML window size and window
    int windowWidth = 1200;
    int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Data Visualization", sf::Style::Titlebar | sf::Style::Close);
    // Initialize graph object
    barGraph = new BarGraph(data, inputChoice, window);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(255, 255, 255));
        barGraph->show(window);
        window.display();
    }
    delete barGraph;
}



/*void graphData(vector<vector<Entry*>>& data) {
    // Initialize SFML window size and window
    int windowWidth = 1200;
    int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Data Visualization", sf::Style::Titlebar | sf::Style::Close);
    // Initialize graph object
    graph = new Graph(data, window);
    // Show the graph each frame
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        graph->show(window);
        window.display();
    }
    delete graph;
}*/

float Selection(Entry* entry, int& inputChoice) {
    if (inputChoice == 0) { return (float)entry->dateInt; }
    else if (inputChoice == 1) { return (float)entry->total; }
    else if (inputChoice == 2) { return (float)entry->changeInTotal; }
    else if (inputChoice == 3) { return entry->totalPerThou; }
    else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

void merge(vector<Entry*>& arr, int left, int mid, int right, int choice) {
    int xCount = mid - left + 1;
    int yCount = right - mid;

    vector<Entry*> X, Y;
    for (int i = 0; i < xCount; i++) {
        X.push_back(arr[left + i]);
    }
    for (int i = 0; i < yCount; i++) {
        Y.push_back(arr[mid + 1 + i]);
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < xCount && j < yCount) {
        if (Selection(X[i], choice) <= Selection(Y[j], choice)) {
            arr[k] = X[i];
            i++;
        }
        else {
            arr[k] = Y[j];
            j++;
        }
        k++;
    }

    while (i < xCount) {
        arr[k] = X[i];
        i++;
        k++;
    }

    while (j < yCount) {
        arr[k] = Y[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Entry*>& arr, int left, int right, int choice) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, choice);
        mergeSort(arr, mid + 1, right, choice);
        merge(arr, left, mid, right, choice);
    }
}

void quickSort(vector<Entry*>& arr, int low, int high, int choice) {
    if (low < high) {
        int piv = split(arr, low, high, choice);

        /*for (int i = 0; i < 5; i++) {
            cout << arr[i]->changeInTotalPerThou << endl;;
        }
        cout << endl;*/

        quickSort(arr, low, piv - 1, choice);
        quickSort(arr, piv + 1, high, choice);
    }
}

int split(vector<Entry*>& arr, int low, int high, int choice) {
    int up = low;
    int down = high;
    
    Entry* tempE;

    while (up < down) {
        for (int i = up; i < high; i++) {
            if (Selection(arr[up], choice) > Selection(arr[low], choice)) {
                break;
            }
            up++;
        }
        for (int i = high; i > low; i--) {
            if (Selection(arr[down], choice) < Selection(arr[low], choice)) {
                break;
            }
            down--;
        }
        if (up < down) {
            tempE = arr[up];
            arr[up] = arr[down];
            arr[down] = tempE;
        }
    }
    tempE = arr[low];
    arr[low] = arr[down];
    arr[down] = tempE;
    return down;

}

void generateMap(map<float, vector<Entry*>>& map, vector<Entry*>& arr, int choice) {
    for (int i = 0; i < arr.size(); i++) {
        map[Selection(arr[i], choice)].push_back(arr[i]);
    }
}
