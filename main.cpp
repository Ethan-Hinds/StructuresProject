//#include <SFML/Graphics.hpp>
//#include "ResourcePath.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Entry.h"
#include "BTreeNode.h"


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


// Declare any global variables here
//Graph* graph = nullptr;


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
    /*BTreeNode* head = nullptr;
    int choice = 1;
    head = head->InsertEntry(v1[0], head, choice);
    head = head->InsertEntry(v1[1], head, choice);
    head = head->InsertEntry(v1[2], head, choice);


    head->keyList[0]->PrintEntry();

    //head->keyList[0]->PrintEntry();
    //head->keyList[1]->PrintEntry();
    //head->keyList[2]->PrintEntry();*/

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
