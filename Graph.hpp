#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Entry.hpp"
#include "Point.hpp"
#include <vector>

using namespace std;

class Graph {
public:
    Graph(vector<vector<Entry*>>& data, sf::RenderWindow& window);
    void show(sf::RenderWindow& window);
    
    ~Graph();
    
private:
    // Position and size of the graph wrt the window
    int x;
    int y;
    int width;
    int height;
    
    
    Entry* startEntry;  // First entry chronologically
    Entry* endEntry;    // Last entry chronologically
    Entry* maxEntry;    // Entry with the highest value
    
    vector<vector<Point*>> lines;   // A 2d vector containing point objects.  Each subvector is a country's line
    sf::Color colors[5];            // This is the only limiting factor in how many countries can be graphed at once
    sf::Font font;
    
    sf::Text axesTexts[7];  // Text for axis labels and amounts
    vector<sf::Text> legendTexts;   // Text for the legend
    vector<sf::RectangleShape> legendBoxes; // Colored boxes for the legend
    
    void setTextLabels();
    void setLegend(sf::RenderWindow& window);
    
    float mapVal(int x, int min1, int max1, int min2, int max2);
    string formatWithCommas(int n);
    float dist(float x1, float y1, float x2, float y2);
    
    
};


