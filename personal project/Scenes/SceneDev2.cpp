#include "stdafx.h"
#include "SceneDev2.h"
#include "TextGo.h"
#include "TileMap.h"
//#include "Player.h"
#include "Element.h"
//#include "MapStructure.h"
//#include "Monster.h"
//#include "DataTable.h"
//#include "DataTableMgr.h"
//#include "Monster.h"
//#include "Bullet.h"

SceneDev2::SceneDev2() : Scene(SceneId::Dev2)
{
	//resourceListPath = "scripts/DefaultResourceList.csv";
}

void SceneDev2::Init()
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
	sceneName->text.setString(L"µ¥ºê 2");

	mapmap = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev2::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev2::Enter()
{
	Scene::Enter();
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
