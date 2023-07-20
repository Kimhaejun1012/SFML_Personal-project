#include "stdafx.h"
#include "GameObject.h"
#include "Map.h"
#include "TileMap.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "DataTable.h"
#include "DataTableMgr.h"
#include "VertexArrayGo.h"
#include "Scene.h"
#include "SpriteGo.h"

Map::Map()
{
	//resourceListPath = "scripts/DefaultResourceList.csv";
}

Map::~Map()
{

}

void Map::Init()
{

}

void Map::Release()
{
}

void Map::Enter()
{
	tileMap = (TileMap*)AddGo(new TileMap("mapsprite/tile.png", "Tile Map"));
	mapmap = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));

	tileMap->Load("map/map1.csv");
	tileMap->SetOrigin(Origins::MC);
	tileSize = tileMap->vertexArray.getBounds();
	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);

	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);
}

void Map::Exit()
{
}

void Map::Update(float dt)
{
}

void Map::Draw(sf::RenderWindow& window)
{
}

