#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "TextureManager.h"

using namespace std;

class Tile {
	
	

public: 
	Tile(float x, float y);

	sf::Sprite front;
	sf::Sprite back;
	sf::Sprite debugsprite;
	vector<Tile*> neighbors;

	void SetBomb(); 
	void FindBombNeighbors();

	bool showFront;
	bool flag;
	bool isbomb;
	int BombNeighbors;
	bool revealed;

};