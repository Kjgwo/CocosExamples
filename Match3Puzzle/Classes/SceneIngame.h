#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"

class SceneIngame : public Scene {
private:
	GameState state;
	// blockData는 0값일경우 비어있는 블록, 0이 아닌 양수값일 경우 블록
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// blockSprite는 nullptr일 경우 비어있음
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);
public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override;

	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();

	void alignBlockSprite();

	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();
};

#endif