#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Entry.hpp"
#include "Bar.hpp"
#include <vector>

using namespace std;

class BarGraph {
public:
    BarGraph(vector<Entry*>& data, int inputChoice, sf::RenderWindow& window);
    void show(sf::RenderWindow& window);
    
    ~BarGraph();
    
private:
    // Position and size of the graph wrt the window
    int x;
    int y;
    int width;
    int height;
    
    vector<Bar*> bars;
    vector<Entry*> data;
    int inputChoice;
    
    sf::Color colors[5];
    sf::Font font;
    sf::Text axesTexts[3];  // Text for axis labels and amounts
    
    vector<sf::Text> legendTexts;   // Text for the legend
    vector<sf::RectangleShape> legendBoxes; // Colored boxes for the legend
    
    float mapVal(float x, float min1, float max1, float min2, float max2);
    string formatWithCommas(float n);
    float Selection(Entry* entry, int& inputChoice);
    void setLegend(sf::RenderWindow& window);
    void setTextLabels();
    
};



