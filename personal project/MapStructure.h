#pragma once
#include "SpriteGo.h"


class MapStructure : public SpriteGo
{

public:

	static const int TotalTypes = 9;


protected:



public:
	MapStructure(const std::string& textureId = "", const std::string& n = "");
	virtual ~MapStructure() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

