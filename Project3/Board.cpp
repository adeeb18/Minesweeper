#include "Board.h" 

Board::Board(int cols, int rows, int mines, int height, int width) {
	this->cols = (cols);
	this->rows = (rows);
	this->numMines = (mines);
	this->width = width;
	this->height = height;
	this->totalTiles = (cols) * (rows);
	this->gameOver = false;
	this->debugMode = false;
	this->tilesRevealed = 0;
	this->gameOver = false;
	this->gameWon = false;
	this->configReadMines = mines;
	this->numFlags = 0;

	CreateTileArray();

	float iconx = width / 2;
	float icony = rows * 32;
	smiley.setTexture(TextureManager::GetTexture("face_happy"));
	smiley.setOrigin(sf::Vector2f(32.0f, 0.f));
	smiley.setPosition(sf::Vector2f(iconx, icony));
	iconx += 96;
	debugIcon.setTexture(TextureManager::GetTexture("debug"));
	debugIcon.setPosition(sf::Vector2f(iconx, icony));
	iconx += 64;
	test1.setTexture(TextureManager::GetTexture("test_1"));
	test1.setPosition(sf::Vector2f(iconx, icony));
	iconx += 64;
	test2.setTexture(TextureManager::GetTexture("test_2"));
	test2.setPosition(sf::Vector2f(iconx, icony));
	iconx += 64;
	test3.setTexture(TextureManager::GetTexture("test_3"));
	test3.setPosition(sf::Vector2f(iconx, icony));

	iconx = 0;
	icony = 32 * rows;
	negative.setTexture(TextureManager::GetTexture("digits"));
	negative.setPosition(sf::Vector2f(iconx, icony));
	negative.setTextureRect(sf::IntRect(21*10, 0, 21, 32));
	iconx += 21;
	onesPlace.setTexture(TextureManager::GetTexture("digits"));
	onesPlace.setPosition(sf::Vector2f(iconx, icony));
	iconx += 21;
	tensPlace.setTexture(TextureManager::GetTexture("digits"));
	tensPlace.setPosition(sf::Vector2f(iconx, icony));
	iconx += 21;
	hundredsPlace.setTexture(TextureManager::GetTexture("digits"));
	hundredsPlace.setPosition(sf::Vector2f(iconx, icony));

	SetValue(SetRandomValues());
	ReadTestCases();
}

void Board::CreateTileArray() {
	float tilepositionx = 0;
	float tilepositiony = 0;
	for (int i = 0; i < cols; i++) {
		vector<Tile> newvec;
		Tiles.push_back(newvec);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Tile tile(tilepositionx, tilepositiony);
			Tiles[j].push_back(tile);
			tilepositionx += 32;
			if (tilepositionx == width) {
				tilepositionx = 0;
				tilepositiony += 32;
			}
		}
	}
	for (int i = 0; i < Tiles.size(); i++) {
		for (int j = 0; j < Tiles[i].size(); j++) {
			if (i - 1 >= 0) {
				Tile* current = &Tiles[i-1][j];
				Tiles[i][j].neighbors.push_back(current);
				if (j - 1 >= 0) {
					Tile* current = &Tiles[i - 1][j - 1];
					Tiles[i][j].neighbors.push_back(current);
				}
				if (j + 1 < Tiles[i].size()) {
					Tile* current = &Tiles[i - 1][j + 1];
					Tiles[i][j].neighbors.push_back(current);
				}
			}
			if (i + 1 < Tiles.size()) {
				Tile* current = &Tiles[i + 1][j];
				Tiles[i][j].neighbors.push_back(current);
				if (j - 1 >= 0) {
					Tile* current = &Tiles[i + 1][j - 1];
					Tiles[i][j].neighbors.push_back(current);
				}
				if (j + 1 < Tiles[i].size()) {
					Tile* current = &Tiles[i + 1][j + 1];
					Tiles[i][j].neighbors.push_back(current);
				}
			}
			if (j - 1 >= 0) {
				Tile* current = &Tiles[i][j-1];
				Tiles[i][j].neighbors.push_back(current);
			}
			if (j + 1 < Tiles[i].size()) {
				Tile* current = &Tiles[i][j+1];
				Tiles[i][j].neighbors.push_back(current);
			}
		}
	}
}

vector<vector<int>> Board::SetRandomValues() {
	vector<vector<int>> truthValues;
	srand(time(NULL));
	for (int i = 0; i < cols; i++) {
		vector<int> newvec;
		truthValues.push_back(newvec);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			truthValues[j].push_back(0);
		}
	}
	for (int i = 0; i < numMines; i++) {
		int x = rand() % cols;
		int y = rand() % rows;
		if (truthValues[x][y] == 0) {
			truthValues[x][y] = 1;
		}
		else {
			i--;
		}
	}
	return truthValues;
}

void Board::Draw(sf::RenderWindow& window) {
	Adjust(window);
	DrawTiles(window);
	DrawMenu(window);
}

void Board::DrawTiles(sf::RenderWindow &window) {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			window.draw(Tiles[i][j].back);
			if (Tiles[i][j].showFront == true) {
				window.draw(Tiles[i][j].front);
			}
			if (debugMode) {
				if (Tiles[i][j].isbomb) {
					if (gameOver) {
						Tiles[i][j].back.setTexture(TextureManager::GetTexture("tile_revealed"));
						window.draw(Tiles[i][j].back);
						if (Tiles[i][j].flag) {
							window.draw(Tiles[i][j].front);
						}
					}
					window.draw(Tiles[i][j].debugsprite);
				}
			}
		}
	}
}

void Board::DrawMenu(sf::RenderWindow& window) {
	//Counter
	int minesleft = numMines - numFlags;
	bool isneg = false;;
	if (minesleft < 0) {
		isneg = true;
		minesleft = minesleft * -1;
	}
	int hundreds = minesleft / 100;
	minesleft = minesleft % 100;
	int tens = minesleft / 10;
	minesleft = minesleft % 10;
	int ones = minesleft;

	if (isneg) {
		window.draw(negative);
	}

	hundredsPlace.setTextureRect(sf::IntRect(21 * ones, 0, 21, 32));
	tensPlace.setTextureRect(sf::IntRect(21 * tens, 0, 21, 32));
	onesPlace.setTextureRect(sf::IntRect(21 * hundreds, 0, 21, 32));

	window.draw(onesPlace); 
	window.draw(tensPlace);
	window.draw(hundredsPlace);
	//Smiley
	if (gameOver) {
		smiley.setTexture(TextureManager::GetTexture("face_lose"));
		window.draw(smiley);
	}
	else {
		window.draw(smiley);
	}
	//Rest of Menu
	window.draw(debugIcon);
	window.draw(test1);
	window.draw(test2);
	window.draw(test3);
}

void Board::SetValue(vector<vector<int>> truths) {
	
	for (int i = 0; i < Tiles.size(); i++) {
		for (int j = 0; j < Tiles[i].size(); j++) {
			if (truths[i][j] == 1) {
				Tiles[i][j].SetBomb();
			}
		} 
	}
	for (int i = 0; i < Tiles.size(); i++) {
		for (int j = 0; j < Tiles[i].size(); j++) {
			Tiles[i][j].FindBombNeighbors();
		}
	}
}

void Board::Adjust(sf::RenderWindow& window) {
	//Right Click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		int x = localPosition.x;
		int y = localPosition.y;
		x = x / 32;
		y = y / 32;
		if (gameOver == false && gameWon == false) {
			if (x >= 0 && x < cols && y >= 0 && y < rows) {
				if (Tiles[x][y].flag == false && Tiles[x][y].revealed == false) {
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("flag"));
					Tiles[x][y].showFront = true;
					Tiles[x][y].flag = true;
					numFlags += 1;
				}
				else if (Tiles[x][y].flag == true) {
					Tiles[x][y].showFront = false;
					Tiles[x][y].flag = false;
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("tile_revealed"));
					numFlags -= 1;

				}
			}
		}
	
		
	}
	//Left Click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		int x = localPosition.x;
		int y = localPosition.y;

		//Tile Click
		if ((x/32) >= 0 && (x/32) < cols && (y/32) >= 0 && (y/32) < rows) {
			x = x / 32;
			y = y / 32;
			if ((Tiles[x][y].flag == false) && (Tiles[x][y].revealed == false) && gameOver == false) {

				if (Tiles[x][y].isbomb == true) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("mine"));
					gameOver = true;
					debugMode = true;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
					for (int i = 0; i < cols; i++) {
						for (int j = 0; j < rows; j++) {
							if (Tiles[i][j].isbomb) {
								Tiles[i][j].debugsprite.setTexture(TextureManager::GetTexture("mine"));
							}
						}
					}
				}
				else if (Tiles[x][y].BombNeighbors == 1) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_1"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 2) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_2"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 3) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_3"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 4) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_4"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 5) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_5"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 6) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_6"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 7) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_7"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else if (Tiles[x][y].BombNeighbors == 8) {
					Tiles[x][y].back.setTexture(TextureManager::GetTexture("tile_revealed"));
					Tiles[x][y].front.setTexture(TextureManager::GetTexture("number_8"));
					tilesRevealed += 1;
					Tiles[x][y].showFront = true;
					Tiles[x][y].revealed = true;
				}
				else {
					Tile* tilepointer = &Tiles[x][y];
					RecursiveCheck(tilepointer);
				}
				

			}
		}
		
		//Smiley
		else if ((x > ((width / 2) - 32)) && (x  < ((width / 2) + 32)) && (y > rows * 32) && (y < ((rows * 32) + 64))) {
			for (int i = 0; i < cols; i++) {
				for (int j = 0; j < rows; j++) {
					Tiles[i][j].BombNeighbors = 0;
					Tiles[i][j].isbomb = false;
					Tiles[i][j].flag = false;
					Tiles[i][j].revealed = false;
					Tiles[i][j].showFront = false;
					Tiles[i][j].back.setTexture(TextureManager::GetTexture("tile_hidden"));
					tilesRevealed = 0;
					gameOver = false;
					gameWon = false;
					debugMode = false;
					numMines = configReadMines;
					numFlags = 0;
					smiley.setTexture(TextureManager::GetTexture("face_happy"));

				}
			}
			SetValue(SetRandomValues());
		}
		//Debug
		else if ((x  > ((width / 2) + 96)) && (x  < ((width / 2) + 160)) && (y  > rows * 32) && (y < ((rows * 32) + 64))) {
			if (gameOver == false && gameWon == false) {
				if (debugMode == false) {
					for (int i = 0; i < cols; i++) {
						for (int j = 0; j < rows; j++) {
							if (Tiles[i][j].isbomb) {
								Tiles[i][j].debugsprite.setTexture(TextureManager::GetTexture("mine"));
							}
						}
					}
					debugMode = true;
				}
				else {
					debugMode = false;
				}
			}
		}
		//Test1
		else if ((x > ((width / 2) + 160)) && (x < ((width / 2) + 224)) && (y  > rows * 32) && (y < ((rows * 32) + 64))) {
			for (int i = 0; i < cols; i++) {
				for (int j = 0; j < rows; j++) {
					Tiles[i][j].BombNeighbors = 0;
					Tiles[i][j].isbomb = false;
					Tiles[i][j].revealed = false;
					Tiles[i][j].showFront = false;
					Tiles[i][j].flag = false;
					Tiles[i][j].back.setTexture(TextureManager::GetTexture("tile_hidden"));
					tilesRevealed = 0;
					gameOver = false;
					gameWon = false;
					debugMode = false;
					numFlags = 0;
					smiley.setTexture(TextureManager::GetTexture("face_happy"));

			}
		}
		    SetValue(Test1);
			numMines = Test1NumMines;
		}
		//Test2
		else if ((x > ((width / 2) + 224)) && (x < ((width / 2) + 288)) && (y > rows * 32) && (y < ((rows * 32) + 64))) {
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				Tiles[i][j].BombNeighbors = 0;
				Tiles[i][j].isbomb = false;
				Tiles[i][j].revealed = false;
				Tiles[i][j].flag = false;
				Tiles[i][j].showFront = false;
				Tiles[i][j].back.setTexture(TextureManager::GetTexture("tile_hidden"));
				tilesRevealed = 0;
				gameOver = false;
				gameWon = false;
				debugMode = false;
				numFlags = 0;
				smiley.setTexture(TextureManager::GetTexture("face_happy"));

			}
		}
		SetValue(Test2);
		numMines = Test2NumMines;
		}
		//Test3
		else if ((x > ((width / 2) + 288)) && (x < ((width / 2) + 352)) && (y > rows * 32) && (y < ((rows * 32) + 64))) {
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				Tiles[i][j].BombNeighbors = 0;
				Tiles[i][j].isbomb = false;
				Tiles[i][j].revealed = false;
				Tiles[i][j].flag = false;
				Tiles[i][j].showFront = false;
				Tiles[i][j].back.setTexture(TextureManager::GetTexture("tile_hidden"));
				tilesRevealed = 0;
				gameOver = false;
				gameWon = false;
				debugMode = false;
				numFlags = 0;
				smiley.setTexture(TextureManager::GetTexture("face_happy"));

			}
		}
		SetValue(Test3);
		numMines = Test3NumMines;
		}
	}
	//Win Condition
	if ((tilesRevealed == totalTiles - numMines) && gameWon == false) {
		gameWon = true;
		smiley.setTexture(TextureManager::GetTexture("face_win"));
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				if (Tiles[i][j].revealed == false) {
					Tiles[i][j].front.setTexture(TextureManager::GetTexture("flag"));
					Tiles[i][j].showFront = true;
					Tiles[i][j].flag = true;
					numFlags += 1;
					debugMode = false;
				}
			}
		}
		
	}
}

void Board::RecursiveCheck(Tile *tile) {
	if ((tile->revealed == false) && (tile->flag == false)) {
		if (tile->BombNeighbors == 0) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->showFront = false;
			tile->revealed = true;
			tilesRevealed += 1;
			for (int i = 0; i < tile->neighbors.size(); i++) {
				RecursiveCheck(tile->neighbors[i]);
			}
		}
		else if (tile->BombNeighbors == 1) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_1"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 2) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_2"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 3) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_3"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 4) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_4"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 5) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_5"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 6) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_6"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 7) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_7"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		else if (tile->BombNeighbors == 8) {
			tile->back.setTexture(TextureManager::GetTexture("tile_revealed"));
			tile->front.setTexture(TextureManager::GetTexture("number_8"));
			tilesRevealed += 1;
			tile->showFront = true;
			tile->revealed = true;
		}
		
	}
}

void Board::ReadTestCases() {
	//Test1
	for (int i = 0; i < cols; i++) {
		vector<int> newvec;
		Test1.push_back(newvec);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Test1[j].push_back(0);
		}
	}
	ifstream inFile1("boards/testboard1.brd");
	string line;
	Test1NumMines = 0;
	for (int i = 0; i < rows; i++) {
		getline(inFile1, line);
		for (int j = 0; j < cols; j++) {
			if (line.at(j) == '1') {
				Test1[j][i] = 1;
				Test1NumMines += 1;
			}
		}
	}

	//Test2
	for (int i = 0; i < cols; i++) {
		vector<int> newvec;
		Test2.push_back(newvec);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Test2[j].push_back(0);
		}
	}
	ifstream inFile2("boards/testboard2.brd");
	Test2NumMines = 0;
 
	for (int i = 0; i < rows; i++) {
		getline(inFile2, line);
		for (int j = 0; j < cols; j++) {
			if (line.at(j) == '1') {
				Test2[j][i] = 1;
				Test2NumMines += 1;
			}
		}
	}


	//Test3
	for (int i = 0; i < cols; i++) {
		vector<int> newvec;
		Test3.push_back(newvec);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Test3[j].push_back(0);
		}
	}
	ifstream inFile3("boards/testboard3.brd");
	Test3NumMines = 0;
	for (int i = 0; i < rows; i++) {
		getline(inFile3, line);
		for (int j = 0; j < cols; j++) {
			if (line.at(j) == '1') {
				Test3[j][i] = 1;
				Test3NumMines += 1;
			}
		}
	}

}