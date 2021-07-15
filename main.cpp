#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Entry.hpp"
#include "Graph.hpp"

using namespace std;

// Declare any methods in the main file here, before implementing them below
void loadData(string filePath, vector<Entry*>& entries);
void graphData(vector<vector<Entry*>>& entries);


// Declare any global variables here
Graph* graph = nullptr;


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
        } else if (it->country == "Argentina") {
            v2.push_back(it);
        } else if (it->country == "Norway") {
            v3.push_back(it);
        } else if (it->country == "Bangladesh") {
            v4.push_back(it);
        } else if (it->country == "Estonia") {
            v5.push_back(it);
        }
    }
    vector<vector<Entry*>> data = {v1, v2, v3, v4, v5};
    graphData(data);
    
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
