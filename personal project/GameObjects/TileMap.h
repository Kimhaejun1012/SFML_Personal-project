#pragma once
#include "VertexArrayGo.h"

class SpriteGo;
class Player;

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex = 0;
};

class TileMap : public VertexArrayGo
{
protected:


	Tile tile;
	Player* player;
	SpriteGo* mapmap;
	sf::FloatRect wallBounds;
	sf::FloatRect tileSize;

public:
	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;

	bool Load(const std::string& filePath);

	std::vector<Tile> tiles;
};

