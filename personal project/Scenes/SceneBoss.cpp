#include "stdafx.h"
#include "SceneBoss.h"
#include "TextGo.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
//#include "MapStructure.h"
//#include "Monster.h"
//#include "DataTable.h"
//#include "DataTableMgr.h"
#include "Monster.h"
#include "Bullet.h"

SceneBoss::SceneBoss() : Scene(SceneId::Dev2)
{
	//resourceListPath = "scripts/DefaultResourceList.csv";
}

void SceneBoss::Init()
{
	Release();


	for (auto go : gameObjects)
	{
		go->Init();
	}

}

void SceneBoss::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneBoss::Enter()
{
	Scene::Enter();
	sf::Vector2f size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
}

void SceneBoss::Exit()
{
	Scene::Exit();
}

void SceneBoss::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneBoss::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
