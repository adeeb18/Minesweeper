

#include <iostream>
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"

using namespace std;

int main()
{
    //Readin config file

    ifstream inFile("boards/config.cfg");
    string cols;
    string rows;
    string mines;
    getline(inFile, cols);
    getline(inFile, rows);
    getline(inFile, mines);
    int width = stoi(cols) * 32;
    int height = stoi(rows) * 32 + 88;


    //Create window object
    sf::RenderWindow window(sf::VideoMode(width,height), "Game Window");


    //Create Board object
    Board newBoard(stoi(cols), stoi(rows), stoi(mines), height, width);
    newBoard.SetRandomValues();

    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Checking if Window Closes
            if (event.type == sf::Event::Closed){
                window.close();
            }
            //Clear window to white
            window.clear(sf::Color(255, 255, 255));
            newBoard.Draw(window);
            window.display();
                           
        }

    }

    TextureManager::Clear();
    return 0;
}