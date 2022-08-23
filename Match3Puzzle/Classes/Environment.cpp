#include "stdafx.h"
#include "Environment.h"

Global* instance = nullptr;

Global * Global::getInstance()
{
	if (instance == nullptr) instance = new Global();

	return instance;
}

void Global::setScore(long long value)
{
	this->score = value;
}

long long Global::getScore()
{
	return score;
}

void Global::addScore(long long value)
{
	this->score += value;
}

void Global::playPop()
{
	AudioEngine::play2d("res/pop.mp3", false, 0.2);
}

void Global::playBackgroundMusic()
{
	this->backgroundMusicId = AudioEngine::play2d("res/Attic Secrets.mp3", true, 0.2);
}

void Global::stopBackgroundMusic()
{
	if (backgroundMusicId == -1) return;
	AudioEngine::stop(this->backgroundMusicId);
}
