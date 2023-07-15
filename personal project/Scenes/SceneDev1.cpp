#include "stdafx.h"
#include "SceneDev1.h"
#include "TextGo.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
#include "MapStructure.h"
#include "Monster.h"
#include "DataTable.h"
#include "DataTableMgr.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
}

void SceneDev1::Init()
{
	Release();


	sf::Vector2f size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	TextGo* sceneName = (TextGo*)AddGo(new TextGo("", "Scene Name"));
	sceneName->sortLayer = 100;
	sceneName->text.setCharacterSize(25);
	sceneName->text.setFillColor(sf::Color::White);
	sceneName->text.setString(L"데브 1");

	tileMap = (TileMap*)AddGo(new TileMap("mapsprite/tile.png", "Tile Map"));
	AddStructure();
	element = (Element*)AddGo(new Element("mapsprite/element1.png", "element1"));
	player = (Player*)AddGo(new Player());

	//애초에 그림 어떻게 띄우더라 csv로
	//지형지물을 1,2,3,4,5 다 쓸건데
	//장애물 충돌이 되는 혹은 데미지를 입는

	for (auto go : gameObjects)
	{
		go->Init();
	}
	tileMap->Load("map/map1.csv");
	tileMap->SetOrigin(Origins::MC);
	//->SetPosition(player->GetPosition());
	element->SetOrigin(Origins::MC);
	element->SetPosition(sf::Vector2f{ tileMap->GetPosition()});
	//structure->SetPosition(sf::Vector2f{ 400,400 });
	tileSize = tileMap->vertexArray.getBounds();
	//element->sortLayer = 101;


}

void SceneDev1::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	auto size = FRAMEWORK.GetWindowSize();
	//worldView.setCenter(tileMap->GetPosition());

	//uiView.setSize(size);
	//uiView.setCenter(player->GetPosition());

	IsStructure();
	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);
}

void SceneDev1::Exit()
{
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition());
	
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev2);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneDev1::AddStructure()
{
	
	mapmap = (MapStructure*)AddGo(new MapStructure("mapstructure/mapmap.png", "mapmap"));
}

void SceneDev1::IsStructure()
{
	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);

}

void SceneDev1::Scene1MonsterSetting(int count)
{
	for (int i = 0; i < count; i++)
	{
		Monster* monster = new Monster();
		Monster::Types monsterType = (Monster::Types)0;
		monster->SetType(monsterType);
		monster->SetPlayer(player);
		monster->Init();
		//Zombie* zombie = poolZombies.Get();
		//zombie->SetActive(false);
	}
}
