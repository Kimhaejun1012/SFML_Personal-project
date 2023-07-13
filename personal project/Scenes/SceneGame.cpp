#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Framework.h"
#include "Player2.h"
#include "UIButton.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/RubySheet.png"));
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/sprite_sheet.png"));
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/button.png"));
	//resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/button2.png"));
}

void SceneGame::Init()
{
	Release();




	auto size = FRAMEWORK.GetWindowSize();


	Player* player = (Player*)AddGo(new Player());
	//Player2* player2 = (Player2*)AddGo(new Player2());
	UIButton* testbutton = (UIButton*)AddGo(new UIButton("graphics/button.png"));
	testbutton->SetOrigin(Origins::TR);
	testbutton->sortLayer = 100;
	testbutton->SetPosition(size.x, 0.f);
	testbutton->OnEnter = [testbutton]() {
		std::cout << "들어옴" << std::endl;
		
	};
	testbutton->OnExit = [testbutton]() {
		sf::Texture* tex = RESOURCE_MGR.GetTexture(testbutton->textureId);
		testbutton->sprite.setTexture(*tex);
		std::cout << "나감" << std::endl;

	};
	testbutton->OnClick = [testbutton]() {
		
		SCENE_MGR.ChangeScene(SceneId::Game);
		std::cout << "클릭" << std::endl;
		
	};
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{

	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter({size * 0.5f});

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
