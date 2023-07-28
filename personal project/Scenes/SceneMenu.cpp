#include "stdafx.h"
#include "SceneMenu.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "StringTable.h"
#include "Scene.h"
#include "UIButton.h"

SceneMenu::SceneMenu()
	: Scene(SceneId::Menu)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{
	Release();
	for (auto go : gameObjects)
	{
		go->Init();
	}

	main = ((SpriteGo*)AddGo(new SpriteGo("graphics/main.png", "main")));
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	startbutton = ((UIButton*)AddGo(new UIButton("graphics/startbutton.png", "main")));

	main->SetPosition(0,0);
	main->SetOrigin(Origins::TL);
	main->sprite.setScale(0.85,0.45);

	startbutton->SetPosition(490,870);
	startbutton->SetOrigin(Origins::MC);
	startbutton->sprite.setScale(0.4, 0.4);


	startbutton->OnClick = []() {


		SCENE_MGR.ChangeScene(SceneId::Dev1);
	};

}

void SceneMenu::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneMenu::Enter()
{

	Scene::Enter();
}

void SceneMenu::Exit()
{
	Scene::Exit();
}

void SceneMenu::Update(float dt)
{
	Scene::Update(dt);

		startbutton->OnMouse = [this, dt]()
		{
			sf::Vector2f scale = { 0.4f,0.4f };
			sf::Vector2f targetScale = { 15.f, 15.f };
			if (scale.x < targetScale.x)
				scale += sf::Vector2f(10.f * dt, 10.f * dt);
			else
			scale -= sf::Vector2f(10.f * dt, 10.f * dt);
			startbutton->sprite.setScale(scale);
		};

}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
