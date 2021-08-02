#include "Bar.hpp"
#include "Entry.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

Bar::Bar(Entry* entry, sf::Color color, int inputChoice) {
    this->entry = entry;
    this->color = color;
    this->inputChoice = inputChoice;
    
    if (!font.loadFromFile("sansation.ttf")) {
        std::cout << "Error loading font" << endl;
    }
    valueText.setFont(font);
    valueText.setString(formatWithCommas(Selection(entry, inputChoice)));
    valueText.setCharacterSize(14);
    valueText.setFillColor(sf::Color::Black);

    
}

void Bar::show(sf::RenderWindow& window) {
    
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setPosition(x, y);
    rectangle.setFillColor(color);
    window.draw(rectangle);
    
    sf::FloatRect bounds = valueText.getLocalBounds();
    valueText.setPosition(x + width/2 - bounds.width/2, y - 30);
    window.draw(valueText);
}

float Bar::Selection(Entry* entry, int& inputChoice) {
    if (inputChoice == 0) { return (float)entry->dateInt; }
    else if (inputChoice == 1) { return (float)entry->total; }
    else if (inputChoice == 2) { return (float)entry->changeInTotal; }
    else if (inputChoice == 3) { return entry->totalPerThou; }
    else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

string Bar::formatWithCommas (float n) {
    string result = "";
    string num = to_string(n);
    int counter = 0;
    int index = -1;
    for (int i = 0; i < num.size() - 1; i += 1) {
        if (num[i] == '.') {
            index = i;
        }
    }
    if (index == -1) {
        index = num.size() - 1;
    }

    bool isTrailing = true;
    for (int i = num.size() - 1; i >= 0; i-= 1) {
        if (num[i] != '0') {
            isTrailing = false;
        }
        if (isTrailing) {
            continue;
        }
        if (i >= index) {
            result.push_back(num[i]);
        } else {
            counter += 1;
            result.push_back(num[i]);
            if (counter == 3) {
                result.push_back(',');
                counter = 0;
            }
        }
    }
    reverse(result.begin(), result.end());
//    if (result.size() % 4 == 0) {
//        result.erase(result.begin());
//    }
    if (result[0] == ',') {
        result.erase(result.begin());
    }
    if (result[result.length()-1] == '.') {
        result.erase(result.end()-1);
    }
    return result;
}

