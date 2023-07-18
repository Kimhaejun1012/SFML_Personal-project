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
#include "UIButton.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
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
	//coin = (SpriteGo*)AddGo(new SpriteGo("graphics/coin.png", "asdf"));
	//monster = (Monster*)AddGo(new Monster());
	//애초에 그림 어떻게 띄우더라 csv로
	//지형지물을 1,2,3,4,5 다 쓸건데
	//장애물 충돌이 되는 혹은 데미지를 입는



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
	//for (int i = 0; i < 5; ++i)
	//{
	//	auto coin = new SpriteGo("graphics/coin.png", "coin");
	//	coin->SetActive(false);
	//	coins.push_back(coin);
	//	AddGo(coin);
	//}
	//tempCoin = ((SpriteGo*)AddGo(new SpriteGo("graphics/coin.png", "coin")));
	
		testbutton1 = (UIButton*)AddGo(new UIButton("upgrade/doubleArrow.png", "mouse"));
		testbutton2 = (UIButton*)AddGo(new UIButton("upgrade/doubleAttack.png", ""));
		testbutton3 = (UIButton*)AddGo(new UIButton("upgrade/doubleSpeed.png", ""));
		testbutton1->SetOrigin(Origins::MC);
		testbutton1->SetPosition(size.x * 0.5, size.y * 0.5);
		testbutton1->sortLayer = 100;
		testbutton1->SetActive(false);
		testbutton2->SetOrigin(Origins::MC);
		testbutton2->SetPosition(testbutton1->GetPosition().x - 300, testbutton1->GetPosition().y);
		testbutton2->sortLayer = 100;
		testbutton2->SetActive(false);
		testbutton3->SetOrigin(Origins::MC);
		testbutton3->SetPosition(testbutton1->GetPosition().x + 300, testbutton1->GetPosition().y);
		testbutton3->sortLayer = 100;
		testbutton3->SetActive(false);
		testbutton1->OnClick = [this]() {
			//testbutton1->SetPlayer(this);
			//testbutton1->IncreaseBullet();
			std::cout << "1클릭" << std::endl;
			testbutton1->SetActive(false);
			testbutton2->SetActive(false);
			testbutton3->SetActive(false);
		};
		testbutton2->OnClick = [this]() {
			//testbutton2->SetPlayer(this);
			//testbutton2->IncreaseAttact();
			std::cout << "2클릭" << std::endl;
			testbutton1->SetActive(false);
			testbutton2->SetActive(false);
			testbutton3->SetActive(false);
		};

		testbutton3->OnClick = [this]() {
			//testbutton3->SetPlayer(this);
			//testbutton3->IncreaseSpeed();
			std::cout << "3클릭" << std::endl;
			testbutton1->SetActive(false);
			testbutton2->SetActive(false);
			testbutton3->SetActive(false);
		};

	tempCoin = ((SpriteGo*)AddGo(new SpriteGo("graphics/coin.png", "coin")));
	for (auto go : gameObjects)
	{
		go->Init();
	}
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
	SpawnMonsters(15, sf::Vector2f(0, 0));

	
	//poolMonsters.Clear();
	//uiView.setSize(size);
	//uiView.setCenter(player->GetPosition());
	//SpawnMonsters(5, player->GetPosition());

	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);	

	//coin->SetPosition(player->GetPosition());
	//coin->SetOrigin(Origins::MC);
}

void SceneDev1::Exit()
{
	ClearObjectPool(poolMonsters);
	player->Reset();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	//std::cout << "씬데브1"<<std::endl;
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition());
	NextScene();
	player->SetMonsterList(monsters);
	if (nextScene && player->sprite.getGlobalBounds().intersects(nextdoor->sprite.getGlobalBounds()))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev2);
	}

	if (player->ReturnExp() >= player->ReturnMaxExp())
	{
		std::cout << "나 혼자만 레벨업" << std::endl;
		testbutton1->SetActive(true);
		testbutton2->SetActive(true);
		testbutton3->SetActive(true);
		player->GetMaxExp(1.3);
	}

	for (int i = 0; i < coins.size(); ++i)
	{
		coinDir = Utils::Normalize(player->GetPosition() - coins[i]->sprite.getPosition());
		float distance = Utils::Distance(player->GetPosition(), coins[i]->sprite.getPosition());

		if (distance > 25.f) 
		{
			coinPos = coins[i]->sprite.getPosition() + coinDir * speed * dt;
			coins[i]->sprite.setPosition(coinPos);
		}
		if (coins[i]->GetActive()) {

			if (coins[i]->sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
			{
				//std::cout << "경험치 올랐어요" << std::endl;
				int a = 30;
				player->GetExp(a);
				coins[i]->SetActive(false);
			}
		}
	}
	//std::cout << player->ExpExp() << std::endl;
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

		tempCoin->SetActive(false);
		coins.push_back(tempCoin);


		//if (zombiePool.empty())
		//if (poolMonsters.GetPool().empty())
		//{
		//	CreateMonster(2);
		//}

		//Zombie* zombie = zombiePool.front();
		//zombiePool.pop_front();
		Monster* monster = poolMonsters.Get();
		monsters = poolMonsters.GetUseList();

		//zombie->SetActive(true);
		//이미지 2개를 두고 위에서 아래로 보낸다 일정 시간동안 보내다가 일정 시간이 지나면 사진 고정되게
		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(400);
		} while (Utils::Distance(center, pos) < 200.f && 3 > 200.f);

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
	monsters.remove(monster);
	
	for (int i = 0; i < coins.size(); ++i)
	{
		if (coins[i]->GetActive())
		{
			continue;
		}
		else
		{
			std::cout << "코인 테스트"  << std::endl;
			coins[i]->SetActive(true);
			coins[i]->SetPosition(monster->GetPosition());
			coins[i]->SetOrigin(Origins::MC);
			coins[i]->sortLayer = 99;
			//GetCoin(coin);
			break;
			//std::cout << "코인 생성" << coin->GetPosition().x << std::endl;// 적절한 위치 설정
			// 코인을 화면에 그리는 로직 추가 (예: RenderWindow에 그리기)
		}
	}
	//auto cointex = FindGo("coin");
	//cointex->SetPosition(player->GetPosition());
	//cointex->SetOrigin(Origins::MC);
	//cointex->sortLayer= 5;
	//std::cout << cointex->GetPosition().x << cointex->GetPosition().y << std::endl;
	//player->RemoveMonster(monster);
	monsterCount--;

	std::cout << "몬스터 죽음" << monsterCount << std::endl;
}

void SceneDev1::GetCoin(std::vector<SpriteGo*> coin)
{
	coin;
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

