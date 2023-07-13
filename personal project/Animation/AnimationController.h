#pragma once
//애니메이션 컨트롤러마다 재생가능한 애니메이션을 등록해놓고 애니메이션마다 바꿔가면서 플레이하게
class AnimationController
{
protected:
	//정렬할 일이 없어서 unordered_map
	std::unordered_map<std::string, AnimationClip> clips;
	std::queue<std::string> queue;

	float speed = 1.f;
	bool isPlaying = false;

	AnimationClip* currentClip = nullptr;
	int currentFrame = -1;
	int totalFrame = 0;
	float clipDuration = 0.f;

	float accumTime = 0.f;

	//애니메이션이 적용될 스프라이트
	sf::Sprite* target = nullptr;

public:

	void AddClip(const AnimationClip& newClip);
	void SetTarget(sf::Sprite* sprite) { target = sprite; }
	sf::Sprite* GetTarget() const { return target; }
	bool IsPlaying() { return isPlaying; }
	void SetSpeed(float s) { speed = s; }
	float Speed() { return speed; }

	void Update(float dt);
	
	void Play(const std::string& clipid, bool clearQueue = true);
	void PlayQueue(const std::string& clipid);
	void Stop();
	void SetFrame(const AnimationFrame& frame);
	std::string GetCurrentClipId();
};