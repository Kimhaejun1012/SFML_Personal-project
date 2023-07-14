#pragma once
#include "SpriteGo.h"


class MapStructure : public SpriteGo
{

public:

	enum class Types
	{
		Grass_down,
		Grass_up,
		Left,
		Left_down,
		Left_up,
		Maze_dege,
		Right,
		Right_down,
		right_up
	};
	static const int TotalTypes = 9;


protected:
	Types structureTyes;



public:
	MapStructure(const std::string& textureId = "", const std::string& n = "");
	virtual ~MapStructure() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

