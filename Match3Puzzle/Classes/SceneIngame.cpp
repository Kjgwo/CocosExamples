#include "stdafx.h"
#include "SceneIngame.h"

void SceneIngame::createBlock(int x, int y, int type)
{
	auto cache = Director::getInstance()->getTextureCache();
	auto spr = Sprite::createWithTexture(
		cache->getTextureForKey("res/match3_tiles_px.png"), 
		Rect(0 + (type*40), 0, 40, 40)
	);
	spr->setScale(2);
	this->addChild(spr);
	setBlockData(x, y, type);
	setBlockSprite(x, y, spr);
}

int SceneIngame::getBlockData(int x, int y)
{
	return blockData[y][x];
}

void SceneIngame::setBlockData(int x, int y, int type)
{
	blockData[y][x] = type;
}

Sprite * SceneIngame::getBlockSprite(int x, int y)
{
	return blockSprite[y][x];
}

void SceneIngame::setBlockSprite(int x, int y, Sprite * s)
{
	blockSprite[y][x] = s;
}

void SceneIngame::destroyBlock(int x, int y)
{
	if (blockData[y][x] != 0) {
		blockSprite[y][x]->removeFromParent();
		blockSprite[y][x] = nullptr;
		blockData[y][x] = 0;
	}
}

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET
		- Vec2(BLOCK_HORIZONTAL*BLOCK_WIDTH / 2, (BLOCK_VERTICAL*BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;

	Vec2 delta = gameCoord - blockOrigin;
	Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5));

	return pos;
}


Vec2 SceneIngame::convertBlockCoordToGameCoord(Vec2 blockCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET 
		- Vec2((BLOCK_HORIZONTAL*BLOCK_WIDTH) / 2, (BLOCK_VERTICAL*BLOCK_HEIGHT) / 2) 
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) /2;
	return blockOrigin
		+ Vec2(BLOCK_WIDTH * blockCoord.x, BLOCK_HEIGHT * blockCoord.y);

}

int SceneIngame::findEmptyBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) == 0) return i;
	}
	return -1;
}

int SceneIngame::findFilledBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) != 0) return i;
	}
	return -1;
}

void SceneIngame::dropBlocks(int x)
{
	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockYIndex(x, i);

		int filled_y = findFilledBlockYIndex(x, empty_y);

		{
			int a = getBlockData(x, empty_y);
			int b = getBlockData(x, filled_y);
			SWAP(int, a, b);
			setBlockData(x, empty_y, a);
			setBlockData(x, filled_y, b);
		} {
			Sprite* a = getBlockSprite(x, empty_y);
			Sprite* b = getBlockSprite(x, filled_y);
			SWAP(Sprite*, a, b);
			setBlockSprite(x, empty_y, a);
			setBlockSprite(x, filled_y, b);
		}
		alignBlockSprite();
	}

}

SceneIngame * SceneIngame::create()
{
	auto ret = new SceneIngame();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init()
{
	if (!Scene::init()) return false;

	srand(time(0));

	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = std::bind(&SceneIngame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);


	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();

	this->initUI();
	this->initGame();
	this->startGame();
}

void SceneIngame::initUI()
{
}

void SceneIngame::initGame()
{
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			createBlock(i, k, rand()%4 + 1);
		}
	}

	this->alignBlockSprite();
}

void SceneIngame::destroyUI()
{
}

void SceneIngame::destroyGame()
{
}

void SceneIngame::alignBlockSprite()
{
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			auto s = getBlockSprite(i, k);
			if (s != nullptr) s->setPosition(convertBlockCoordToGameCoord(Vec2(i,k)));
		}
	}
}

bool SceneIngame::onTouchBegan(Touch * t, Event * e)
{
	Vec2 p = convertGameCoordToBlockCoord(t->getLocation());
	CCLOG("%f, %f", p.x, p.y);
	destroyBlock(p.x, p.y);
	dropBlocks(p.x);

	return true;
}

void SceneIngame::onTouchMoved(Touch * t, Event * e)
{
}

void SceneIngame::onTouchEnded(Touch * t, Event * e)
{
}

void SceneIngame::startGame()
{
}

void SceneIngame::pauseGame()
{
}

void SceneIngame::winGame()
{
}

void SceneIngame::loseGame()
{
}
