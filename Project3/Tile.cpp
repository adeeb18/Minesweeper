#include "Tile.h"
#include "TextureManager.h"

Tile::Tile(float x, float y) {

	this->back.setTexture(TextureManager::GetTexture("tile_hidden"));
	this->front.setPosition(sf::Vector2f(x, y));
	this->back.setPosition(sf::Vector2f(x, y));
	this->debugsprite.setPosition(sf::Vector2f(x, y));
	flag = false;
	isbomb = false;
	BombNeighbors = 0;
	revealed = false;
}

void Tile::SetBomb() {
	this->isbomb = true;
}

void Tile::FindBombNeighbors() {
	for (int i = 0; i < this->neighbors.size(); i++) {
		if (neighbors[i]->isbomb == true) {
			this->BombNeighbors += 1;
		}
	}
}