#include "stdafx.h"
#include "MapStructure.h"
#include "DataTableMgr.h"
#include "DataTable.h"
#include "SceneDev1.h"
#include "Scene.h"
#include "MapStructureTable.h"


MapStructure::MapStructure(const std::string& textureId, const std::string& n) : SpriteGo(textureId, n)
{
}

MapStructure::~MapStructure()
{
}

void MapStructure::Init()
{
	SpriteGo::Init();
}

void MapStructure::Release()
{
	SpriteGo::Release();
}

void MapStructure::Reset()
{
	SpriteGo::Reset();
}

void MapStructure::Update(float dt)
{
	SpriteGo::Update(dt);
}

void MapStructure::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}