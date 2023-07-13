#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect texCoord;
	//매개변수도 보이드 반환형도 보이드인 함수 연결 가능
	std::function<void()> action;
};

struct AnimationClip
{
	std::string id;
	std::vector<AnimationFrame> frames;
	AnimationLoopTypes loopType;
	int fps;

	bool LoadFromFile(const std::string path);
};