#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "Tile.h"
using namespace std;

class Board {
	int rows;
	int cols;
	int height;
	int width;
	int totalTiles;
	int numMines;
	int configReadMines;
	bool debugMode;
	bool gameOver;
	bool gameWon;
	int tilesRevealed;
	int numFlags;
	vector<int> tileTruthValues;
	vector<vector<Tile>> Tiles;
	vector<vector<int>> Test1;
	int Test1NumMines;
	vector<vector<int>> Test2;
	int Test2NumMines;
	vector<vector<int>> Test3;
	int Test3NumMines;
	sf::Sprite smiley;
	sf::Sprite debugIcon;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite test3;
	sf::Sprite onesPlace;
	sf::Sprite tensPlace;
	sf::Sprite hundredsPlace;
	sf::Sprite negative;

public:
	Board(int cols, int rows, int mines, int height, int width);
	vector<vector<int>> SetRandomValues();
	void CreateTileArray();
	void Draw(sf::RenderWindow& window);
	void DrawTiles(sf::RenderWindow &window);
	void DrawMenu(sf::RenderWindow& window);
	void Adjust(sf::RenderWindow& window);
	void SetValue(vector<vector<int>> truths);
	void RecursiveCheck(Tile *tile);
	void ReadTestCases();
};