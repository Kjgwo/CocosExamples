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

	// 유니크 스택을 위한 자료구조
	Vec2 judgeStack[128];
	// 스택에 있는 자료의 수
	int judgeStackCount = 0;
	// 0이라면 스택에 자료가 없음, 그게 아니라면 자료가 없음
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	// ==

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	// 아래에서부터 찾아 올라가면서 비어있는 블록을 찾고
	// -1 이 리턴이 되면 비어있는 블록이 없다는 뜻이다.
	int findEmptyBlockYIndex(int x, int  y);

	// y 위치부터 찾아 올라가면서 비어있지 않은 블록을 찾고
	// -1이 리턴이 되면 비어있지 않은 블록이 없다는 뜻이다
	int findFilledBlockYIndex(int x, int y);

	// 블록을 떨어뜨리는 함수
	void dropBlocks(int x);

	void stackPush(Vec2 value);
	Vec2 stackPop();
	void stackEmpty();
	bool stackFind(Vec2 value);
	void judgeMatch(int x, int y);

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