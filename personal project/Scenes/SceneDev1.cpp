#include "stdafx.h"
#include "SceneDev1.h"
#include "TextGo.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
#include "DataTable.h"
#include "DataTableMgr.h"
#include "Monster.h"
#include "Bullet.h"
#include "UIButton.h"
#include "MonsterBullet.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
}

SceneDev1::~SceneDev1()
{
	//Release();
}

void SceneDev1::Init()
{
	Release();
	size = FRAMEWORK.GetWindowSize();
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
	tileMap2 = (TileMap*)AddGo(new TileMap("mapsprite/tile.png", "Tile Map"));
	mapmap = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));
	mapmap2 = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));
	element = (Element*)AddGo(new Element("mapsprite/element1.png", "element1"));
	nextdoor = (SpriteGo*)AddGo(new SpriteGo("mapstructure/LayerDoor_1.png", "door"));
	player = (Player*)AddGo(new Player());
	clear = (SpriteGo*)AddGo(new SpriteGo("graphics/win.png", "win"));
	clearbutton = (UIButton*)AddGo(new UIButton("graphics/home.png", "home"));

	clear->SetOrigin(Origins::MC);
	clear->SetPosition(size.x * 0.5, size.y * 0.5);
	clear->sortLayer = 102;
	clear->SetActive(false);

	clearbutton->SetOrigin(Origins::MC);
	clearbutton->SetPosition(size.x * 0.5, size.y * 0.8);
	clearbutton->sortLayer = 103;
	clearbutton->SetActive(false);

	tileMap->Load("map/map1.csv");
	tileMap->SetOrigin(Origins::MC);
	tileMap2->Load("map/map1.csv");
	tileMap2->SetOrigin(Origins::MC);
	tileMap2->SetPosition(tileMap->GetPosition().x, tileMap->GetPosition().y - 3000);
	mapmap2->SetPosition(tileMap2->GetPosition());
	mapmap2->SetOrigin(Origins::MC);


	element->SetOrigin(Origins::MC);
	element->SetPosition(sf::Vector2f{ tileMap->GetPosition() });

	tileSize = tileMap->vertexArray.getBounds();
	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);
	nextdoor->SetPosition(-19.f, tileSize.top - 37);
	nextdoor->SetOrigin(Origins::MC);
	nextdoor->SetActive(false);

	poolMonsters.OnCreate = [this](Monster* monster) {

		monster->SetPlayer(player);
	};
	poolMonsters.Init();


	tempCoin = ((SpriteGo*)AddGo(new SpriteGo("graphics/coin.png", "coin")));

	clearbutton->OnClick = [this]() {

		std::cout << "클리어 버튼 클릭" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Title);
		clear->SetActive(false);
		clearbutton->SetActive(false);
		//Release();
	};
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev1::Release()
{
	//poolMonsters.Release();
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go; // <- 요기
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();
	//SpawnMonsters(2, tileMap2->GetPosition());

	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);

	SpawnMonsters(2, sf::Vector2f(0, 0), monsterType0);
	//coin->SetPosition(player->GetPosition());
	//coin->SetOrigin(Origins::MC);
}

void SceneDev1::Exit()
{
	//player->Release();
	ClearObjectPool(poolMonsters);
	player->Reset();
	monster->Reset();
	//poolMonsters.Clear();
	//poolMonsters.Release();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	//std::cout << "씬데브1"<<std::endl;
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition().x, player->GetPosition().y);
	NextScene();
	player->SetMonsterList(monsters);
	

	if (nextScene && player->sprite.getGlobalBounds().intersects(nextdoor->sprite.getGlobalBounds()))
	{
		wallBounds = mapmap2->sprite.getGlobalBounds();
		//monsterbullet->SetMapSize(mapmap2->sprite.getGlobalBounds());
		monster->GetMap2(wallBounds);
		SpawnMonsters(1, tileMap2->GetPosition(), (Monster::Types)2);
		player->SetWallBounds(mapmap2->sprite.getGlobalBounds());
		player->SetPosition(tileMap2->GetPosition().x, tileMap2->GetPosition().y + 700);
		nextdoor->SetPosition(-19.f, tileMap2->vertexArray.getBounds().top - 37);
		nextdoor->SetActive(false);

	}
	if(INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	SCENE_MGR.ChangeScene(SceneId::Title);
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
	if (bossdie)
	{
		clear->SetActive(true);
		clearbutton->SetActive(true);
		std::cout << "보스 죽음 클리어 메뉴 띄워라" << std::endl;
		bossdie = false;
	}

	//if (exp >= maxexp)
	//{
	//	std::cout << "나 혼자만 레벨업" << std::endl;
	//	testbutton1->SetActive(true);
	//	testbutton2->SetActive(true);
	//	testbutton3->SetActive(true);
	//	exp -= maxexp;
	//	maxexp *= 1.3;
	//}
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

void SceneDev1::SpawnMonsters(int count, sf::Vector2f center, Monster::Types a)
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
		monster = poolMonsters.Get();
		monsters = poolMonsters.GetUseList();
		monster->SetType(a);
		monster->GetMap2(wallBounds);
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

void SceneDev1::SpawnMonsters2(int count, sf::Vector2f center)
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
		monster = poolMonsters.Get();
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
			//std::cout << "코인 테스트" << std::endl;
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

}

void SceneDev1::GetCoin(std::vector<SpriteGo*> coin)
{
	coin;
}

const std::list<Monster*>* SceneDev1::GetMonsterList() const
{
	return &poolMonsters.GetUseList();
}

//void SceneDev1::PlayerUI()
//{
//	playerMaxHp = ((SpriteGo*)AddGo(new SpriteGo("", "")));
//	playerHp = ((SpriteGo*)AddGo(new SpriteGo("", "")));
//	maxexpbar = ((SpriteGo*)AddGo(new SpriteGo("graphics/ExpBarmax.png", "maxexpbar")));
//	expbar = ((SpriteGo*)AddGo(new SpriteGo("graphics/ExpBar.png", "expbar")));
//	playerHp->rect.setSize(sf::Vector2f(50.f, 7.f));
//	playerHp->rect.setFillColor(sf::Color::Green);
//
//
//
//
//	expbar->SetOrigin(Origins::BL);
//	expbar->sortLayer = 101;
//	//expbar->sprite.setScale(0, 0);
//	maxexpbar->sprite.setPosition(size.x * 0.27, 50);
//	expbar->sprite.setPosition(size.x * 0.27, 50);
//	maxexpbar->SetOrigin(Origins::BL);
//	maxexpbar->sortLayer = 101;
//	sf::Color expbar = maxexpbar->sprite.getColor();
//	maxexpbar->sprite.setColor({ 0,0,0,150 });
//	playerMaxHp->rect.setSize(sf::Vector2f(50.f, 7.f));
//	playerMaxHp->rect.setFillColor(sf::Color::Black);
//	sf::Color aaa = playerMaxHp->sprite.getColor();
//	aaa.a = 125;
//	playerMaxHp->rect.setFillColor(aaa);
//
//	testbutton1 = (UIButton*)AddGo(new UIButton("upgrade/doubleArrow.png", "mouse"));
//	testbutton2 = (UIButton*)AddGo(new UIButton("upgrade/doubleAttack.png", ""));
//	testbutton3 = (UIButton*)AddGo(new UIButton("upgrade/doubleSpeed.png", ""));
//	testbutton1->SetOrigin(Origins::MC);
//	testbutton1->SetPosition(size.x * 0.5, size.y * 0.5);
//	testbutton1->sortLayer = 100;
//	testbutton1->SetActive(false);
//	testbutton2->SetOrigin(Origins::MC);
//	testbutton2->SetPosition(testbutton1->GetPosition().x - 300, testbutton1->GetPosition().y);
//	testbutton2->sortLayer = 100;
//	testbutton2->SetActive(false);
//	testbutton3->SetOrigin(Origins::MC);
//	testbutton3->SetPosition(testbutton1->GetPosition().x + 300, testbutton1->GetPosition().y);
//	testbutton3->sortLayer = 100;
//	testbutton3->SetActive(false);
//	testbutton1->OnClick = [this]() {
//		//testbutton1->SetPlayer(this);
//		//testbutton1->IncreaseBullet();
//		std::cout << "1클릭" << std::endl;
//		testbutton1->SetActive(false);
//		testbutton2->SetActive(false);
//		testbutton3->SetActive(false);
//	};
//	testbutton2->OnClick = [this]() {
//		//testbutton2->SetPlayer(this);
//		//testbutton2->IncreaseAttact();
//		std::cout << "2클릭" << std::endl;
//		testbutton1->SetActive(false);
//		testbutton2->SetActive(false);
//		testbutton3->SetActive(false);
//	};
//
//	testbutton3->OnClick = [this]() {
//		//testbutton3->SetPlayer(this);
//		//testbutton3->IncreaseSpeed();
//		std::cout << "3클릭" << std::endl;
//		testbutton1->SetActive(false);
//		testbutton2->SetActive(false);
//		testbutton3->SetActive(false);
//	};
//}

void SceneDev1::NextScene()
{
	if (monsterCount == 0)
	{
		nextScene = true;
		nextdoor->SetActive(true);
	}
	else
		nextScene = false;
}

