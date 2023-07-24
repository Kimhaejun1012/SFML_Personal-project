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

	main->SetPosition(centerPos);
	main->SetOrigin(Origins::MC);
	main->sprite.setScale(0.4,0.4);
	main->sortLayer = 101;

	startbutton->SetPosition(centerPos.x, centerPos.y * 1.6);
	startbutton->SetOrigin(Origins::MC);
	startbutton->sprite.setScale(0.4, 0.4);
	startbutton->sortLayer = 101;





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
			sf::Vector2f scale = { 0.4f,0.4f };// = startbutton->sprite.getScale();
			sf::Vector2f targetScale = { 15.f, 15.f };
			if (scale.x < targetScale.x)
				scale += sf::Vector2f(1.f * dt, 1.f * dt);
			else
			scale -= sf::Vector2f(1.f * dt, 1.f * dt);

			std::cout<<scale.x;
			startbutton->sprite.setScale(scale);
			//startbutton->sprite.setScale({ std::min(scale.x, targetScale.x), std::min(scale.y, targetScale.y)});

		};

}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
