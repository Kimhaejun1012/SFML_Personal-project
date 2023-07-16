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
#include "Monster.h"
#include "Bullet.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1)
{
	//resourceListPath = "scripts/DefaultResourceList.csv";
}

SceneDev1::~SceneDev1()
{
	Release();
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
	mapmap = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));
	element = (Element*)AddGo(new Element("mapsprite/element1.png", "element1"));
	nextdoor = (SpriteGo*)AddGo(new SpriteGo("mapstructure/LayerDoor_1.png", "door"));
	player = (Player*)AddGo(new Player());
	//monster = (Monster*)AddGo(new Monster());
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
	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);
	nextdoor->SetPosition(-19.f, tileSize.top-37);
	nextdoor->SetOrigin(Origins::MC);
	nextdoor->SetActive(false);
	//CreateMonster(2);
	poolMonsters.OnCreate = [this](Monster* monster) {

		Monster::Types monsterType = (Monster::Types)0;
		monster->SetType(monsterType);
		monster->SetPlayer(player);
	};
	poolMonsters.Init();

}

void SceneDev1::Release()
{
	poolMonsters.Release();
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	
	//poolMonsters.Clear();
	//uiView.setSize(size);
	//uiView.setCenter(player->GetPosition());
	SpawnMonsters(2, player->GetPosition());

	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);
}

void SceneDev1::Exit()
{
	ClearObjectPool(poolMonsters);
	player->Reset();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition());
	NextScene();
	if (nextScene && player->sprite.getGlobalBounds().intersects(nextdoor->sprite.getGlobalBounds()))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev2);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

//void SceneDev1::Scene1MonsterSetting(int count)
//{
//	for (int i = 0; i < count; i++)
//	{
//		Monster::Types monsterType = (Monster::Types)0;
//		monster->SetType(monsterType);
//		monster->SetPlayer(player);
//		monster->Init();
//		//Zombie* zombie = poolZombies.Get();
//		//zombie->SetActive(false);
//	}
//}

void SceneDev1::SpawnMonsters(int count, sf::Vector2f center)
{
	for (int i = 0; i < count; i++)
	{
		//if (zombiePool.empty())
		//if (poolMonsters.GetPool().empty())
		//{
		//	CreateMonster(2);
		//}

		//Zombie* zombie = zombiePool.front();
		//zombiePool.pop_front();
		Monster* monster = poolMonsters.Get();
		player->SetMonster(monster);
		//zombie->SetActive(true);

		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(300.f);
		} while (Utils::Distance(center, pos) < 100.f && 3 > 100.f);

		monster->SetPosition(pos);

		//zombies.push_back(zombie);
		//zombie->Reset();
		AddGo(monster);
		monsterCount++;
		std::cout << "몬스터 생성" << std::endl;
	}
}

//void SceneDev1::CreateMonster(int count)
//{
//	for (auto zombie : poolMonsters.GetUseList())
//	{
//		//zombie->SetActive(false);
//		//RemoveGo(zombie);
//		//zombiePool.push_back(zombie);
//		//poolZombies.Return(zombie);
//		RemoveGo(zombie);
//	}
//	//zombies.clear();
//	poolMonsters.Clear();
//}

void SceneDev1::OnDieMonster(Monster* monster)
{
	RemoveGo(monster);
	poolMonsters.Return(monster);
	monsterCount--;
	std::cout << "몬스터 죽음" << monsterCount << std::endl;
}

const std::list<Monster*>* SceneDev1::GetMonsterList() const
{
	return &poolMonsters.GetUseList();
}

void SceneDev1::NextScene()
{
	if (monsterCount == 0)
	{
		nextScene = true;
		nextdoor->SetActive(true);
	}
}

