#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"

class SceneIngame : public Scene {
private:
	GameState state;
	// blockData�� 0���ϰ�� ����ִ� ����, 0�� �ƴ� ������� ��� ����
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// blockSprite�� nullptr�� ��� �������
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	// �Ʒ��������� ã�� �ö󰡸鼭 ����ִ� ������ ã��
	// -1 �� ������ �Ǹ� ����ִ� ������ ���ٴ� ���̴�.
	int findEmptyBlockYIndex(int x, int  y);

	// y ��ġ���� ã�� �ö󰡸鼭 ������� ���� ������ ã��
	// -1�� ������ �Ǹ� ������� ���� ������ ���ٴ� ���̴�
	int findFilledBlockYIndex(int x, int y);

	// ������ ����߸��� �Լ�
	void dropBlocks(int x);
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