#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <chrono>
#include "Entry.hpp"
#include "BarGraph.hpp"
#include "Graph.hpp"


using namespace std;
using namespace std::chrono;

// Declare any methods in the main file here, before implementing them below
void loadData(string filePath, vector<Entry*>& entries, set<string>& uniqueCountries);
void graphData(vector<vector<Entry*>>& entries);
float Selection(Entry* entry, int& inputChoice);
void merge(vector<Entry*>& arr, int left, int mid, int right, int choice);
void mergeSort(vector<Entry*>& arr, int left, int right, int choice);
void quickSort(vector<Entry*>& arr, int low, int high, int choice);
int split(vector<Entry*>& arr, int low, int high, int choice);
void generateMap(map<float, vector<Entry*>>& map, vector<Entry*>& arr, int choice);
void barGraphData(vector<Entry*>& data, int lowerLimit, int inputChoice);


// Declare any global variables here
Graph* graph = nullptr;
BarGraph* barGraph = nullptr;


int main() {
    // Vector of all of the entries
    vector<Entry*> entries;
    set<string> uniqueCountries; //Set of all country names loaded
    loadData("covid-testing-all-observations.csv", entries, uniqueCountries);
    if (entries.size() == 0) {
        cout << "Unable to load data!" << endl;
        return 0;
    }
    cout << "done" << endl;
    
    bool isRun = true;
    bool correctInput = true;
    
    string userChoice;
    
    while (isRun) {
        if (correctInput) { //main user menu
            cout << endl << "Type the following integer to select menu option: " << endl;
            cout << "1. Print a Data Entry Based on Country and Date" << endl;
            cout << "2. Print a Ranking of Data Entries for a Specific Date based on User Selected Criteria" << endl;
            cout << "3. Compare Timelines of up to 5 Countries based on User Selected Criteria" << endl;
            cout << "4. Print a Ranking of which Countries were Quickest to Respond Based on First Reported Testing Date" << endl;
            cout << "5. End Program" << endl;
        }
        
        cin >> userChoice;
        
        correctInput = true;
        
        if (userChoice == "1") {//menu selection
            bool run1 = true;
            while (run1) {
                cout << "Please Type a Country: ";
                cin >> userChoice;
                string selCountry = userChoice;
                cout << endl << "Please Input a Date in the form MM/DD/YY (Please note don't include any leading zeros): ";
                cin >> userChoice;
                string selDate = userChoice;
                
                int i = 0;
                while (i < entries.size()) { //finds relevent entry
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
                cout << endl << "Please Input a Date in the form MM/DD/YY (Please note don't include any leading zeros): " << endl;
                cin >> userChoice;
                string selDate = userChoice;
                vector<Entry*> dateList; //list for all contries with an entry on that date
                int i = 0;
                while (i < entries.size()) {
                    if (entries[i]->dateStr == selDate) {
                        dateList.push_back(entries[i]);
                    }
                    i++;
                }
                run2 = false;
                if (dateList.empty()) {
                    cout << endl << "Data not found Please Try Again" << endl;
                    run2 = true;
                }
                else {
                    cout << "Please Select a Value to Sort By:" << endl; // sort data based on user selection
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
                    
                    //perfoms the diffrent sorting algorithms and outputs runtime
                    
                    vector<Entry*> dateListMerge = dateList;
                    
                    auto mergeStart = high_resolution_clock::now();
                    mergeSort(dateListMerge, 0, dateListMerge.size() - 1, choice);
                    auto mergeStop = high_resolution_clock::now();
                    
                    auto mapStart = high_resolution_clock::now();
                    map<float, vector<Entry*>> dateMap;
                    generateMap(dateMap, dateList, choice);
                    auto mapStop = high_resolution_clock::now();
                    
                    
                    auto quickSortStart = high_resolution_clock::now();
                    quickSort(dateList, 0, dateList.size() - 1, choice);
                    auto quickSortStop = high_resolution_clock::now();
                    
                    
                    auto mergeDuration = duration_cast<microseconds>(mergeStop - mergeStart);
                    auto mapDuration = duration_cast<microseconds>(mapStop - mapStart);
                    auto quickSortDuration = duration_cast<microseconds>(quickSortStop - quickSortStart);
                    
                    cout << endl;
                    cout << "Merge Sort runtime: " << mergeDuration.count() << " microseconds" << endl;
                    cout << "Quick Sort runtime: " << quickSortDuration.count() << " microseconds" << endl;
                    cout << "Map generation runtime: " << mapDuration.count() << " microseconds" << endl;
                    cout << endl;
                    
                    
                    int lowerLimit;
                    int upperLimit;
                    
                    while (true) { //print up to 5 countries in the ranking
                        
                        cout << "Please select a range of ranks to evaluate: " << endl;
                        cout << "Lower limit: ";
                        string lowerLimitStr;
                        cin >> lowerLimitStr;
                        cout << endl << "Upper limit: ";
                        string upperLimitStr;
                        cin >> upperLimitStr;
                        cout << endl;
                        
                        lowerLimit = stoi(lowerLimitStr) - 1;
                        upperLimit = stoi(upperLimitStr);
                        
                        if (lowerLimit < 0 || upperLimit < 0) {
                            cout << "Please enter a valid range!" << endl << endl;
                            continue;
                        }
                        if (upperLimit >= dateListMerge.size()) {
                            cout << "Please enter a valid range!" << endl << endl;
                            continue;
                        }
                        if (upperLimit - lowerLimit > 5) {
                            cout << "Please enter a valid range!" << endl << endl;
                            continue;
                        }
                        break;
                        
                    }
                    
                    
                    reverse(dateListMerge.begin(), dateListMerge.end());
                    
                    vector<Entry*> data;
                    
                    for (int i = lowerLimit; i < upperLimit; i += 1) {
                        data.push_back(dateListMerge.at(i));
                    }
                    
                    //                    for (int i = 0; i < dateListMerge.size(); i++) {
                    //                        cout << (i+1) << ": " << dateListMerge[i]->country << ": " << Selection(dateListMerge[i], choice) << endl;
                    //                    }
                    
                    barGraphData(data, lowerLimit, choice);
                    
                    
                    
                    
                    
                }
            }
        }
        else if (userChoice == "3") {
            int n;
            while (true) { //compare up to 5 countries
                cout << "How many countries would you like to graph? (1-5) ";
                string nStr;
                cin >> nStr;
                cout << endl;
                n = stoi(nStr);
                if (n > 5 || n < 1) {
                    cout << "Please enter a valid number between 1 and 5" << endl << endl;
                }
                else {
                    break;
                }
            }
            
            vector<string> countries;
            for (int i = 0; i < n; i += 1) {
                while (true) {
                    cout << "Enter a country to graph: ";
                    string country;
                    cin >> country;
                    cout << endl;
                    
                    if (uniqueCountries.count(country) == 0) { //checks if the user entry is valid
                        cout << "This country is not in the database!" << endl << endl;
                    }
                    else {
                        countries.push_back(country);
                        break;
                    }
                }
            }
            
            vector<Entry*> v1;
            vector<Entry*> v2;
            vector<Entry*> v3;
            vector<Entry*> v4;
            vector<Entry*> v5;
            
            for (auto& it : entries) { //creates the graph data for up to five countries
                if (it->country == countries[0]) {
                    v1.push_back(it);
                }
                else if (countries.size() > 1 && it->country == countries[1]) {
                    v2.push_back(it);
                }
                else if (countries.size() > 2 && it->country == countries[2]) {
                    v3.push_back(it);
                }
                else if (countries.size() > 3 && it->country == countries[3]) {
                    v4.push_back(it);
                }
                else if (countries.size() > 4 && it->country == countries[4]) {
                    v5.push_back(it);
                }
            }
            vector<vector<Entry*>> data;
            data.push_back(v1);
            if (n > 1) {
                data.push_back(v2);
            }
            if (n > 2) {
                data.push_back(v3);
            }
            if (n > 3) {
                data.push_back(v4);
            }
            if (n > 4) {
                data.push_back(v5);
            }
            
            graphData(data);
            
        }
        else if (userChoice == "4") {
            
            vector<Entry*> data; //finds the first date entry for each country
            set<string> countriesUsed;
            for (auto& entry : entries) {
                if (countriesUsed.count(entry->country) == 0) {
                    countriesUsed.insert(entry->country);
                    data.push_back(entry);
                }
            }
            
            //Sorts data based on Date and prints runtime
            
            vector<Entry*> dateListMerge = data;
            
            auto mergeStart = high_resolution_clock::now();
            mergeSort(dateListMerge, 0, dateListMerge.size() - 1, 0);
            auto mergeStop = high_resolution_clock::now();
            
            auto mapStart = high_resolution_clock::now();
            map<float, vector<Entry*>> dateMap;
            generateMap(dateMap, data, 0);
            auto mapStop = high_resolution_clock::now();
            
            
            auto quickSortStart = high_resolution_clock::now();
            quickSort(data, 0, data.size() - 1, 0);
            auto quickSortStop = high_resolution_clock::now();
            
            
            auto mergeDuration = duration_cast<microseconds>(mergeStop - mergeStart);
            auto mapDuration = duration_cast<microseconds>(mapStop - mapStart);
            auto quickSortDuration = duration_cast<microseconds>(quickSortStop - quickSortStart);
            
            cout << endl;
            cout << "Merge Sort runtime: " << mergeDuration.count() << " microseconds" << endl;
            cout << "Quick Sort runtime: " << quickSortDuration.count() << " microseconds" << endl;
            cout << "Map generation runtime: " << mapDuration.count() << " microseconds" << endl;
            cout << endl;
            
            
            int lowerLimit;
            int upperLimit;
            
            while (true) { //user selects what ranks to view
                
                cout << "Please select a range of ranks to evaluate: (1 - " << (uniqueCountries.size() - 1) << ") " << endl;
                cout << "Lower limit: ";
                string lowerLimitStr;
                cin >> lowerLimitStr;
                cout << endl << "Upper limit: ";
                string upperLimitStr;
                cin >> upperLimitStr;
                cout << endl;
                
                lowerLimit = stoi(lowerLimitStr) - 1;
                upperLimit = stoi(upperLimitStr);
                
                if (lowerLimit < 0 || upperLimit < 0) {
                    cout << "Please enter a valid range!" << endl << endl;
                    continue;
                }
                if (upperLimit >= dateListMerge.size()) {
                    cout << "Please enter a valid range!" << endl << endl;
                    continue;
                }
                
                if (upperLimit < lowerLimit + 1) {
                    cout << "Please enter a valid range!" << endl << endl;
                    continue;
                }
                
                break;
                
            }
            
            for (int i = lowerLimit; i < upperLimit; i += 1) {
                cout << (i + 1) << ".  " << dateListMerge[i]->country << ": " << dateListMerge[i]->dateStr << endl;
            }
            
        }
        else if (userChoice == "5") {
            cout << "Closing program" << endl;
            isRun = false;
        }
        else {
            cout << "Input not recognized please try again" << endl;
            correctInput = false;
        }
        
        
    }
    
}


// Loads the data from the csv file and populates the entries vector with Entry objects
void loadData(string filePath, vector<Entry*>& entries, set<string>& uniqueCountries) {
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
            
            uniqueCountries.insert(entry->country);
            
        }
    }
}

void barGraphData(vector<Entry*>& data, int lowerLimit, int inputChoice) {
    // Initialize SFML window size and window
    int windowWidth = 1200;
    int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Data Visualization", sf::Style::Titlebar | sf::Style::Close);
    // Initialize graph object
    barGraph = new BarGraph(data, lowerLimit, inputChoice, window);
    
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



void graphData(vector<vector<Entry*>>& data) {
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
}

//Takes in user selection and converts to relevent float value

float Selection(Entry* entry, int& inputChoice) {
    if (inputChoice == 0) { return (float)entry->dateInt; }
    else if (inputChoice == 1) { return (float)entry->total; }
    else if (inputChoice == 2) { return (float)entry->changeInTotal; }
    else if (inputChoice == 3) { return entry->totalPerThou; }
    else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

void merge(vector<Entry*>& arr, int left, int mid, int right, int choice) { //merges split arrays back together
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

void mergeSort(vector<Entry*>& arr, int left, int right, int choice) { //recrusively splits and merges items in vector
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, choice);
        mergeSort(arr, mid + 1, right, choice);
        merge(arr, left, mid, right, choice);
    }
}

void quickSort(vector<Entry*>& arr, int low, int high, int choice) { //resurvivelly performs quick sort
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

int split(vector<Entry*>& arr, int low, int high, int choice) { //splits the quicksort and assigns piviot point
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

void generateMap(map<float, vector<Entry*>>& map, vector<Entry*>& arr, int choice) { //generates a map based on entered data storing a vector of entries that pertain to sorted value
    for (int i = 0; i < arr.size(); i++) {
        map[Selection(arr[i], choice)].push_back(arr[i]);
    }
}
