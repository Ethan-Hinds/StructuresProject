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
    Bar(Entry* entry, sf::Color color, int inputChoice);
    void show(sf::RenderWindow& window);
    float Selection(Entry* entry, int& inputChoice);
    
private:
    sf::Color color;
    sf::Text valueText;
    sf::Font font;
    int inputChoice;
    string formatWithCommas (float n);
};

