#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "BarGraph.hpp"
#include "Entry.hpp"

using namespace std;

// Text for labeling the graph beneath the x axis
string selectionText(int inputChoice);

BarGraph::BarGraph(vector<Entry*>& data, int lowerLimit, int inputChoice, sf::RenderWindow& window) {
    
    this->inputChoice = inputChoice;
    this->data = data;
    this->lowerLimit = lowerLimit;
    
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
    
    // Since the entries come in sorted, the one with the maximum value is always first
    float maxVal = Selection(data.front(), inputChoice);
    int numCountries = data.size();
    
    float barWidth = width/numCountries;
    
    // Initialize the bars based on their value and position
    int count = 0;
    for (auto& entry : data) {
        Bar* bar = new Bar(entry, colors[count], inputChoice, y + height);
        bars.push_back(bar);
        bar->x = x + barWidth * count + 10;
        bar->y = mapVal(Selection(entry, inputChoice), 0, maxVal, y + height, y);
        bar->width = barWidth - 5;
        bar->height = y + height - bar->y;
        count += 1;
    }
    
    if (!font.loadFromFile("sansation.ttf")) {
        std::cout << "Error loading font" << endl;
    }
    
    setTextLabels();
    setLegend(window);
}

// Show the graph, bars, legend, and axis labels
void BarGraph::show(sf::RenderWindow& window) {

    sf::RectangleShape xAxis(sf::Vector2f(width + 5, 5));
    xAxis.setPosition(x, y + height);
    xAxis.setFillColor(sf::Color(0, 0, 0));
    
    sf::RectangleShape yAxis(sf::Vector2f(5, height));
    yAxis.setPosition(x, y);
    yAxis.setFillColor(sf::Color(0, 0, 0));
    
    window.draw(xAxis);
    window.draw(yAxis);

    bool infoShown = false;
    for (auto& bar : bars) {
        bar->show(window);
    }
    
    for (auto& box : legendBoxes) {
        window.draw(box);
    }
    
    for (auto& text : legendTexts) {
        window.draw(text);
    }
    
    for (int i = 0; i < sizeof(axesTexts)/sizeof(axesTexts[0]); i += 1) {
        window.draw(axesTexts[i]);
    }
}

// Destructor
BarGraph::~BarGraph() {
    for (auto& bar : bars) {
        delete bar;
    }
}


// Sets the legend boxes and the legend text
void BarGraph::setLegend(sf::RenderWindow& window) {
    
    for (int i = 0; i < bars.size(); i += 1) {
        Bar* bar = bars[i];
        sf::Color color = colors[i];
        float boxX = x + width + 20;
        float boxY = y + 40*i;
        sf::RectangleShape box(sf::Vector2f(20, 20));
        box.setFillColor(color);
        box.setPosition(boxX, boxY);
        legendBoxes.push_back(box);

        sf::Text text;
        text.setFont(font);
        text.setString(to_string(i+1+lowerLimit) + ".  " + bar->entry->country);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setPosition(boxX + 30, boxY);
        legendTexts.push_back(text);
    }
}


// Set the axis labels
void BarGraph::setTextLabels() {
    float maxVal = Selection(data.front(), inputChoice);
    sf::Text maxValText;
    maxValText.setFont(font);
    maxValText.setString(formatWithCommas(maxVal));
    maxValText.setCharacterSize(14);
    maxValText.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = maxValText.getLocalBounds();
    maxValText.setPosition(x - bounds.width - 10, y - bounds.height/2);
    axesTexts[0] = maxValText;
    
    sf::Text zeroText;
    zeroText.setFont(font);
    zeroText.setString("0");
    zeroText.setCharacterSize(14);
    zeroText.setFillColor(sf::Color::Black);
    bounds = zeroText.getLocalBounds();
    zeroText.setPosition(x - bounds.width - 10, y + height - bounds.height/2);
    axesTexts[1] = zeroText;
    
    sf::Text midText;
    midText.setFont(font);
    midText.setString(formatWithCommas(maxVal/2));
    midText.setCharacterSize(14);
    midText.setFillColor(sf::Color::Black);
    bounds = midText.getLocalBounds();
    midText.setPosition(x - bounds.width - 10, y + height/2 - bounds.height/2);
    axesTexts[2] = midText;
    
    sf::Text dateText;
    dateText.setFont(font);
    dateText.setString(data.at(0)->dateStr + ": " + selectionText(inputChoice));
    dateText.setCharacterSize(20);
    dateText.setFillColor(sf::Color::Black);
    bounds = dateText.getLocalBounds();
    dateText.setPosition(x + width/2 - bounds.width/2, y + height + bounds.height/2 + 10);
    axesTexts[3] = dateText;
    
}


// Maps a value linearly from between min1 and max1 to min2 and max2.  Used for scaling the y axis
float BarGraph::mapVal(float x, float min1, float max1, float min2, float max2) {
    return min2 + (((float) max2 - min2) / (max1 - min1)) * (x - min1);
}

// Formats a float with commas every 3, except if it's beyond the decimal point
string BarGraph::formatWithCommas (float n) {
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

// Get the correct value depending on what data are being displayed in the graph
float BarGraph::Selection(Entry* entry, int& inputChoice) {
    if (inputChoice == 0) { return (float)entry->dateInt; }
    else if (inputChoice == 1) { return (float)entry->total; }
    else if (inputChoice == 2) { return (float)entry->changeInTotal; }
    else if (inputChoice == 3) { return entry->totalPerThou; }
    else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

// Get the correct text for below the x axis depending on what data are being displayed in the graph
string BarGraph::selectionText(int inputChoice) {
    if (inputChoice == 0) {
        return "Date Int";
    } else if (inputChoice == 1) {
        return "Total Cumulative Tests Performed to Date";
    } else if (inputChoice == 2) {
        return "Tests Performed";
    } else if (inputChoice == 3) {
        return "Total Cumulative Tests Performed to Date per Thousand People of the Country's Population";
    } else if (inputChoice == 4) {
        return "Tests Performed per Thousand People of the Country's Population";
    }
}
