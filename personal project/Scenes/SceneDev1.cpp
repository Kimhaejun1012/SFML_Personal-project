#include "stdafx.h"
#include "SceneDev1.h"
#include "TextGo.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
#include "MapStructure.h"
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
	player = (Player*)AddGo(new Player());
	element = (Element*)AddGo(new Element("mapsprite/element1.png", "element1"));

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
	//sf::FloatRect left_downFR = left_down->sprite.getGlobalBounds();
	//sf::FloatRect left_upFR = left_up->sprite.getGlobalBounds();
	//sf::FloatRect rightFR = right->sprite.getGlobalBounds();
	//sf::FloatRect right_downFR = right_down->sprite.getGlobalBounds();
	//sf::FloatRect grass_downFR = grass_down->sprite.getGlobalBounds();
	//sf::FloatRect grass_upFR = grass_up->sprite.getGlobalBounds();
	//sf::FloatRect maze_edgeFR = maze_edge->sprite.getGlobalBounds();

	//grass_down = (MapStructure*)AddGo(new MapStructure("mapstructure/grass_down.png", "grass_down"));
	//grass_up = (MapStructure*)AddGo(new MapStructure("mapstructure/grass_up.png", "grass_up"));
	//grass_up2 = (MapStructure*)AddGo(new MapStructure("mapstructure/grass_up2.png", "grass_up2"));
	//left_down = (MapStructure*)AddGo(new MapStructure("mapstructure/left_down.png", "left_down"));
	//left_up = (MapStructure*)AddGo(new MapStructure("mapstructure/left_up.png", "left_up"));
	//left = (MapStructure*)AddGo(new MapStructure("mapstructure/left.png", "left"));
	//right_down = (MapStructure*)AddGo(new MapStructure("mapstructure/right_down.png", "right_down"));
	//right_up = (MapStructure*)AddGo(new MapStructure("mapstructure/right_up.png", "right_up"));
	//right = (MapStructure*)AddGo(new MapStructure("mapstructure/right.png", "right"));
	//maze_edge = (MapStructure*)AddGo(new MapStructure("mapstructure/maze_edge.png", "maze_edge"));
	//edge_close = (MapStructure*)AddGo(new MapStructure("mapstructure/edge_close.png", "edge_close"));
	mapmap = (MapStructure*)AddGo(new MapStructure("mapstructure/mapmap.png", "mapmap"));
}

void SceneDev1::IsStructure()
{
	//left->SetPosition(tileSize.left, (tileSize.top + tileSize.height / 2));
	//left->SetOrigin(Origins::ML);
	//left->sprite.setScale(1.f, 3.f);

	//sf::FloatRect leftFR = left->sprite.getGlobalBounds();

	//left_down->SetPosition((leftFR.left), (leftFR.top + leftFR.height));
	//left_down->SetOrigin(Origins::TL);
	//sf::FloatRect left_downFR = left_down->sprite.getGlobalBounds();

	//grass_down->SetPosition(left_downFR.left + left_downFR.width, left_downFR.top + left_downFR.height);
	//grass_down->SetOrigin(Origins::BL);
	//grass_down->sprite.setScale(2.f, 1.f);
	//sf::FloatRect grass_downFR = grass_down->sprite.getGlobalBounds();

	//right->SetPosition(tileSize.left + tileSize.width, (tileSize.top + tileSize.height / 2));
	//right->SetOrigin(Origins::MR);
	//right->sprite.setScale(1.f, 3.f);
	//sf::FloatRect rightFR = right->sprite.getGlobalBounds();

	//right_down->SetPosition((rightFR.left + rightFR.width), (rightFR.top + rightFR.height));
	//right_down->SetOrigin(Origins::TR);
	//right_down->sprite.setScale(1.4f, 1.05f);
	//sf::FloatRect right_downFR = right_down->sprite.getGlobalBounds();

	//left_up->SetPosition(leftFR.left + 5, leftFR.top);
	//left_up->SetOrigin(Origins::BL);
	//sf::FloatRect left_upFR = left_up->sprite.getGlobalBounds();

	//grass_up->SetPosition(left_upFR.left + left_upFR.width, left_upFR.top + 25);
	//grass_up->SetOrigin(Origins::TL);
	//sf::FloatRect grass_upFR = grass_up->sprite.getGlobalBounds();

	//right_up->SetPosition(rightFR.left + rightFR.width, rightFR.top);
	//right_up->SetOrigin(Origins::BR);
	//sf::FloatRect right_upFR = right_up->sprite.getGlobalBounds();


	//grass_up2->SetPosition(right_upFR.left, left_upFR.top + 25);
	//grass_up2->SetOrigin(Origins::TR);
	//sf::FloatRect grass_up2FR = grass_up2->sprite.getGlobalBounds();

	//maze_edge->SetPosition(0, tileSize.top);
	//maze_edge->SetOrigin(Origins::MC);
	////maze_edge->Set
	//sf::FloatRect maze_edgeFR = maze_edge->sprite.getGlobalBounds();

	//edge_close->SetPosition(0, (maze_edgeFR.top + maze_edgeFR.height - 30));
	//edge_close->SetOrigin(Origins::BC);

	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);

}

void SceneDev1::MapFR()
{
	mapmap;
}
