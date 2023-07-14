#pragma once
#include "SpriteGo.h"

class Player;

class Element : public SpriteGo
{

public:
	enum class Types
	{
		Element1,
		Element2,
		Element3,
	};


protected:
	Types elementType;


public:
	Element(const std::string& textureId = "", const std::string& n = "");
	virtual ~Element() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

