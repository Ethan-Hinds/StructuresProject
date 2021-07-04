#include "Point.hpp"
#include "Entry.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

Point::Point(Entry* entry, sf::Color color) {
    this->entry = entry;
    this->color = color;
}

void Point::show(sf::RenderWindow& window) {
    sf::CircleShape circle(1);
    circle.setPosition(x, y);
    circle.setFillColor(color);
    window.draw(circle);
}
