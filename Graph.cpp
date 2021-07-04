#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include "Graph.hpp"
#include "Entry.hpp"

using namespace std;

Graph::Graph(vector<vector<Entry*>>& data, sf::RenderWindow& window) {
    
    // Set the position and size of the graph.  Yes they're hard coded (sad face)
    x = 150;
    y = 50;
    width = window.getSize().x - x - 200;
    height = window.getSize().y - 2*y - 30;
    
    // Define colors for each country (this is the only limiting factor in how many countries there can be at once)
    colors[0] = sf::Color::Red;
    colors[1] = sf::Color::Green;
    colors[2] = sf::Color::Blue;
    colors[3] = sf::Color::Magenta;
    colors[4] = sf::Color::Cyan;
    
    // Find the first entry, last entry, and the entry with the largest value
    // Simultaneously, add points to the lines vector
    startEntry = data.front().front();
    endEntry = data.front().back();
    maxEntry = data.front().front();
    
    int startDate = startEntry->dateInt;
    int endDate = endEntry->dateInt;
    int maxVal = startEntry->total;
    
    int count = 0;
    for (auto& country : data) {
        lines.push_back(vector<Point*>());
        
        if (country.front()->dateInt < startDate) {
            startEntry = country.front();
            startDate = startEntry->dateInt;
        }
        
        
        if (country.back()->dateInt > endDate) {
            endEntry = country.back();
            endDate = endEntry->dateInt;
        }
        
        for (auto& entry : country) {
            lines.back().push_back(new Point(entry, colors[count]));
            
            if (entry->total > maxVal) {
                maxEntry = entry;
                maxVal = maxEntry->total;
            }
        }
        count += 1;
    }
    // Now the data is organized correctly.  Just need to scale the point positions based on the values
    for (auto& line : lines) {
        for (auto& point : line) {
            point->x = mapVal(point->entry->dateInt, startDate, endDate, x, x + width);
            point->y = mapVal(point->entry->total, 0, maxVal, y + height, y);
        }
    }
    
    if (!font.loadFromFile("sansation.ttf")) {
        std::cout << "Error loading font" << endl;
    }
    
    setTextLabels();
    setLegend(window);
    

}


void Graph::show(sf::RenderWindow& window) {
    // Horizontal and Vertical axis lines
    sf::Vertex yBar[] = {
        sf::Vertex(sf::Vector2f(x, y)),
        sf::Vertex(sf::Vector2f(x,  y + height)),
    };
    sf::Vertex xBar[] = {
        sf::Vertex(sf::Vector2f(x, y + height)),
        sf::Vertex(sf::Vector2f(x + width, y + height))
    };
    window.draw(yBar, 2, sf::Lines);
    window.draw(xBar, 2, sf::Lines);
    // Draw each point.  Also, check if the mouse is hovering over that point
    // If it is, draw the info box for that point
    // infoShown makes sure only 1 info box can be drawn at a time
    bool infoShown = false;
    for (auto& line : lines) {
        for (auto& point : line) {
            point->show(window);
            if (!infoShown) {
                int mouseX = sf::Mouse::getPosition(window).x;
                int mouseY = sf::Mouse::getPosition(window).y;
                if (dist(point->x, point->y, mouseX, mouseY) < 4) {
                    sf::RectangleShape infoBox(sf::Vector2f(240.0f, 110.0f));
                    infoBox.setFillColor(sf::Color::Transparent);
                    infoBox.setOutlineColor(sf::Color::Red);
                    infoBox.setOutlineThickness(2);
                    infoBox.setPosition(x + 20, y);
                    window.draw(infoBox);
                    
                    sf::Text infoText;
                    infoText.setFont(font);
                    infoText.setString("Country: " + point->entry->country + "\n\nDate: " + point->entry->dateStr + "\n\nCumulative Tests: " + formatWithCommas(point->entry->total));
                    infoText.setCharacterSize(15);
                    infoText.setFillColor(sf::Color::White);
                    infoText.setPosition(infoBox.getPosition().x+10, infoBox.getPosition().y+10);
                    window.draw(infoText);
                    
                    infoShown = true;
                }
            }
        }
    }
    
    for (int i = 0; i < sizeof(axesTexts)/sizeof(axesTexts[0]); i += 1) {
        window.draw(axesTexts[i]);
    }
    
    for (auto& box : legendBoxes) {
        window.draw(box);
    }
    
    for (auto& text : legendTexts) {
        window.draw(text);
    }
}

Graph::~Graph() {
    for (auto& line : lines) {
        for (auto& point : line) {
            delete point;
        }
    }
}


void Graph::setLegend(sf::RenderWindow& window) {
    int i = 0;
    for (auto& line : lines) {
        sf::Color color = colors[i];
        float boxX = x + width + 20;
        float boxY = y + 40*i;
        sf::RectangleShape box(sf::Vector2f(20, 20));
        box.setFillColor(color);
        box.setPosition(boxX, boxY);
        legendBoxes.push_back(box);
        
        sf::Text text;
        text.setFont(font);
        text.setString(line.front()->entry->country);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(boxX + 30, boxY);
        legendTexts.push_back(text);
        i += 1;
    }
}

void Graph::setTextLabels() {
    sf::Text maxValText;
    maxValText.setFont(font);
    maxValText.setString(formatWithCommas(maxEntry->total));
    maxValText.setCharacterSize(12);
    maxValText.setFillColor(sf::Color::White);
    sf::FloatRect bounds = maxValText.getLocalBounds();
    maxValText.setPosition(x - bounds.width - 10, y - bounds.height/2);
    axesTexts[0] = maxValText;
    
    sf::Text avgValText;
    avgValText.setFont(font);
    avgValText.setString(formatWithCommas(maxEntry->total/2));
    avgValText.setCharacterSize(12);
    avgValText.setFillColor(sf::Color::White);
    bounds = avgValText.getLocalBounds();
    avgValText.setPosition(x - bounds.width - 10, y + height/2 - bounds.height/2);
    axesTexts[1] = avgValText;
    
    sf::Text zeroText;
    zeroText.setFont(font);
    zeroText.setString("0");
    zeroText.setCharacterSize(12);
    zeroText.setFillColor(sf::Color::White);
    bounds = zeroText.getLocalBounds();
    zeroText.setPosition(x - bounds.width - 20, y + height - bounds.height);
    axesTexts[2] = zeroText;
    
    sf::Text firstDateText;
    firstDateText.setFont(font);
    firstDateText.setString(startEntry->dateStr);
    firstDateText.setCharacterSize(12);
    firstDateText.setFillColor(sf::Color::White);
    bounds = firstDateText.getLocalBounds();
    firstDateText.setPosition(x - bounds.width/2, y + height + bounds.height);
    axesTexts[3] = firstDateText;
    
    sf::Text lastDateText;
    lastDateText.setFont(font);
    lastDateText.setString(endEntry->dateStr);
    lastDateText.setCharacterSize(12);
    lastDateText.setFillColor(sf::Color::White);
    bounds = lastDateText.getLocalBounds();
    lastDateText.setPosition(x + width - bounds.width/2, y + height + bounds.height);
    axesTexts[4] = lastDateText;
    
    sf::Text vAxisLabel;
    vAxisLabel.setFont(font);
    vAxisLabel.setString("Cumulative Cases");
    vAxisLabel.setCharacterSize(15);
    vAxisLabel.setFillColor(sf::Color::White);
    bounds = vAxisLabel.getLocalBounds();
    vAxisLabel.rotate(-90.f);
    vAxisLabel.setPosition(x - bounds.width/2 - 60, y + height/2 + bounds.width/2);
    axesTexts[5] = vAxisLabel;
    
    sf::Text hAxisLabel;
    hAxisLabel.setFont(font);
    hAxisLabel.setString("Date");
    hAxisLabel.setCharacterSize(15);
    hAxisLabel.setFillColor(sf::Color::White);
    bounds = hAxisLabel.getLocalBounds();
    hAxisLabel.setPosition(x + width/2 - bounds.width/2, y + height + bounds.height + 20);
    axesTexts[6] = hAxisLabel;
    
}



float Graph::mapVal(int x, int min1, int max1, int min2, int max2) {
    return min2 + (((float) max2 - min2) / (max1 - min1)) * (x - min1);
}

string Graph::formatWithCommas (int n) {
    string result = "";
    string num = to_string(n);
    int counter = 0;
    for (int i = num.size() - 1; i >= 0; i-= 1) {
        counter += 1;
        result.push_back(num[i]);
        if (counter == 3) {
            result.push_back(',');
            counter = 0;
        }
    }
    reverse(result.begin(), result.end());
    if (result.size() % 4 == 0) {
        result.erase(result.begin());
    }
    return result;
}

float Graph::dist(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
