#include "stdafx.h"
#include "SceneTitle.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "StringTable.h"
#include "Scene.h"

SceneTitle::SceneTitle()
	: Scene(SceneId::Title)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();
	title = ((SpriteGo*)AddGo(new SpriteGo("graphics/Title.png", "Title")));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	title->sprite.setScale(1, 1);
	title->SetOrigin(Origins::MC);
	title->SetPosition(centerPos);
	title->sortLayer = 100;
	getColor = title->sprite.getColor();
	getColor.a = 0;
	speed = 1;

}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
	
	fadeTimer += dt;
	if(getColor.a < 255)
	if (fadeTimer > fadeDuration)
	{
		fadeTimer = 0.f;
		getColor.a = std::max((float)(getColor.a + speed), 0.f);
	}

	title->sprite.setColor(getColor);

	if (getColor.a >= 250 || INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}