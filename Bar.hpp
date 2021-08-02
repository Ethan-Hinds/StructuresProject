#pragma once

#include "Entry.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

class Bar {
public:
    float x;
    float y;
    float width;
    float height;
    Entry* entry;
    Bar(Entry* entry, sf::Color color, int inputChoice, int graphBottom);
    void show(sf::RenderWindow& window);
    float Selection(Entry* entry, int& inputChoice);
    
private:
    
    float currentY;
    float currentHeight;
    int graphBottom;
    
    sf::Color color;
    sf::Text valueText;
    sf::Font font;
    int inputChoice;
    string formatWithCommas (float n);
    float mapVal(float x, float min1, float max1, float min2, float max2);
};

