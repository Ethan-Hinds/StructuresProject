#include "Bar.hpp"
#include "Entry.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

Bar::Bar(Entry* entry, sf::Color color, int inputChoice, int graphBottom) {
    this->entry = entry;
    this->color = color;
    this->inputChoice = inputChoice;
    
    // Used for the animation
    this->graphBottom = graphBottom;
    this->currentY = graphBottom;
    this->currentHeight = 0;
    
    if (!font.loadFromFile("sansation.ttf")) {
        std::cout << "Error loading font" << endl;
    }
    valueText.setFont(font);
    valueText.setString(formatWithCommas(Selection(entry, inputChoice)));
    valueText.setCharacterSize(14);
    valueText.setFillColor(sf::Color::Black);

    
}

void Bar::show(sf::RenderWindow& window) {
    
    // Show a rectangle based on the current position and size
    // Animation works by initially setting the height to 0 and growing toward the targget size
    
    sf::RectangleShape rectangle(sf::Vector2f(width, currentHeight));
    rectangle.setPosition(x, currentY);
    rectangle.setFillColor(color);
    window.draw(rectangle);
    
    // If the animation is done, snap to the true value
    if (abs(currentY - y) < 1) {
        currentY = y;
        currentHeight = height;
        valueText.setString(formatWithCommas(Selection(entry, inputChoice)));
    } else {
        // This is to get the correct number of digits for the animation
        // The number of digits during animation should be the same number of digits as the true number
        string targetString = formatWithCommas((Selection(entry, inputChoice)));
        int decimals = 0;
        for (int i = targetString.length()-1; i >= 0; i -= 1) {
            if (targetString[i] != '.') {
                decimals += 1;
            } else {
                break;
            }
        }
        if (decimals == targetString.length()) {
            decimals = 0;
        }
        
        string currentString = formatWithCommas(mapVal(currentY, graphBottom, y, 0, Selection(entry, inputChoice)));
        
        int periodIndex = -1;
        for (int i = 0; i < currentString.length(); i += 1) {
            if (currentString[i] == '.') {
                periodIndex = i;
                break;
            }
        }
        if (periodIndex != -1) {
            try {
                currentString = currentString.substr(0, periodIndex + decimals + 1);
            } catch (const exception& e) {
                
            }
        }
        valueText.setString(currentString);
    }
    
    // Draw the rectangle
    sf::FloatRect bounds = valueText.getLocalBounds();
    valueText.setPosition(x + width/2 - bounds.width/2, currentY - 30);
    window.draw(valueText);
    
    // Increment the currentY and currentHeight for animation
    currentY -= (currentY - y) * 0.005;
    currentHeight = graphBottom - currentY;
}

// Get the correct value based on what data are being displayed
float Bar::Selection(Entry* entry, int& inputChoice) {
    if (inputChoice == 0) { return (float)entry->dateInt; }
    else if (inputChoice == 1) { return (float)entry->total; }
    else if (inputChoice == 2) { return (float)entry->changeInTotal; }
    else if (inputChoice == 3) { return entry->totalPerThou; }
    else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

// Formats a float with commas
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
    if (result[0] == ',') {
        result.erase(result.begin());
    }
    if (result[result.length()-1] == '.') {
        result.erase(result.end()-1);
    }
    return result;
}

// Maps a value linearly from between min1 and max1 to min2 and max2
float Bar::mapVal(float x, float min1, float max1, float min2, float max2) {
    return min2 + (((float) max2 - min2) / (max1 - min1)) * (x - min1);
}
