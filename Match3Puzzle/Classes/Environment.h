#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "stdafx.h"



enum class GameState {
	PLAYING, PAUSED, WIN, LOSE, BLOCK_MOVING
};

#define BLOCK_HORIZONTAL 7
#define BLOCK_VERTICAL 9
#define BLOCK_OFFSET Vec2(720/2, 1280/2)
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 80

#define BLOCK_VAR 4

#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

#define SWAP(TYPE, A, B){TYPE t = A; A = B; B = t; }

class Global {
private:
	Global(){}
	long long score = 0;
	int backgroundMusicId = -1;
public:
	static Global* getInstance();

	void setScore(long long value);
	long long getScore();
	void addScore(long long value);

	void playPop();
	void playBackgroundMusic();
	void stopBackgroundMusic();
};
#endif