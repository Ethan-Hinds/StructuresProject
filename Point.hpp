#pragma once

#include "Entry.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

class Point {
public:
    float x;
    float y;
    Entry* entry;
    Point(Entry* entry, sf::Color color);
    void show(sf::RenderWindow& window);
private:
    sf::Color color;
};
