#pragma once

#include "Entry.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

class Bar {
public:
    // Position and size properties
    float x;
    float y;
    float width;
    float height;
    Entry* entry; // The Entry associated with this bar
    Bar(Entry* entry, sf::Color color, int inputChoice, int graphBottom);
    void show(sf::RenderWindow& window);
    float Selection(Entry* entry, int& inputChoice);
    
private:
    
    float currentY; // Used for the animation
    float currentHeight; // Used for the animation
    int graphBottom; // Used for the animation
    
    sf::Color color;
    sf::Text valueText;
    sf::Font font;
    int inputChoice;
    string formatWithCommas (float n);
    float mapVal(float x, float min1, float max1, float min2, float max2);
};

