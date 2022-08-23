#include "stdafx.h"
#include "SceneIngame.h"
#include "SceneHome.h"

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
		state = GameState::BLOCK_MOVING;
		blockSprite[y][x]->runAction(Sequence::create(
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			Spawn::create(ScaleTo::create(0.125f, 0.0), FadeOut::create(0.125f), nullptr),
			RemoveSelf::create(),
			nullptr));
		blockSprite[y][x] = nullptr;
		blockData[y][x] = 0;

		this->runAction(Sequence::create(
			DelayTime::create(0.625f),
			CallFunc::create([=]() {dropBlocks(x); }),
			nullptr
			));
	}
}

Vec2 SceneIngame::convertGameCoordToBlockCoord(const Vec2& gameCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET
		- Vec2(BLOCK_HORIZONTAL*BLOCK_WIDTH / 2, (BLOCK_VERTICAL*BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;

	Vec2 delta = gameCoord - blockOrigin;
	Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5));

	return pos;
}


Vec2 SceneIngame::convertBlockCoordToGameCoord(const Vec2& blockCoord)
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
	bool isDrop = false; // 쵯강단 블록이 추가되지 않을 경우에 유효함
	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockYIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockYIndex(x, empty_y);
		if (filled_y == -1) { 
			createBlock(x, empty_y, rand() % BLOCK_VAR + 1);
			blockSprite[empty_y][x]->setPosition(convertBlockCoordToGameCoord(Vec2(x, BLOCK_VERTICAL + 1)));
			blockSprite[empty_y][x]->runAction(MoveTo::create(0.125f, convertBlockCoordToGameCoord(Vec2(x, empty_y))));
			isDrop = true;
			continue; 
		}
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

			a->stopAllActions();
			a->runAction(MoveTo::create(0.125f, convertBlockCoordToGameCoord(Vec2(x, empty_y))));

		}
		isDrop = true;
	}
	if (isDrop) {
		for (int i = 0; i < BLOCK_VERTICAL; i++) {
			this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([=]() {judgeMatch(x, i); }), nullptr));
		}
	}
	else {
		state = GameState::PLAYING;
	}

}

void SceneIngame::stackPush(const Vec2& value)
{
	if (judgeData[(int)value.y][(int)value.x] != 0) return;
	judgeStack[judgeStackCount++] = value;
	judgeData[(int)value.y][(int)value.x] = 1;

}

const Vec2& SceneIngame::stackPop()
{
	auto ret = judgeStack[--judgeStackCount];
	judgeData[(int)ret.y][(int)ret.x] = 0;
	return ret;
}

void SceneIngame::stackEmpty()
{
	judgeStackCount = 0;
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			judgeData[k][i] = 0;
		}
	}
}

bool SceneIngame::stackFind(const Vec2& value)
{
	return judgeData[(int)value.y][(int)value.x] == 1;
}

void SceneIngame::judgeMatch(int x, int y)
{
	int blockData = getBlockData(x, y);
	if (blockData == 0)return;

	stackPush(Vec2(x, y));
	int push_cnt = 0;
	for (int i = 0; i < 4; i++) {
		int next_x = x;
		int next_y = y;
		int inc_x;
		int inc_y;

		switch (i) {
		case 0: inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1: inc_x = -1; inc_y = 0; break;
		case 2: inc_x = 0; inc_y = 1; push_cnt = 0; break;
		case 3: inc_x = 0; inc_y = -1; break;
		}

		while (true) {
			next_x += inc_x;
			next_y += inc_y;
			if (next_x < 0 || next_x >= BLOCK_HORIZONTAL) break;
			if (next_y < 0 || next_y >= BLOCK_VERTICAL) break;
			if (getBlockData(next_x, next_y) == blockData) {
				stackPush(Vec2(next_x, next_y));
				push_cnt++;
			}
			else break;
		}

		if (i % 2 == 0) continue;
		if (push_cnt < 2) {
			for (int k = 0; k < push_cnt; k++) {
				stackPop();
			}
		}
	}
	if (judgeStackCount > 1) {
		Global::getInstance()->addScore(judgeStackCount * 10);
		ui->setScore(Global::getInstance()->getScore());
		while (judgeStackCount > 0) {
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
		}
	}
	else {
		state = GameState::PLAYING;
	}
	stackEmpty();
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
	addChild(ui = LayerIngameUI::create());
	ui->setLocalZOrder(1);

	ui->btnPause->addClickEventListener([=](Ref* r) {
		if (state == GameState::PLAYING) {
			ui->showPausePanel();
			state = GameState::PAUSED;
			Global::getInstance()->playPop();
		}
		
	});

	ui->btnResume->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) {
			ui->hidePausePanel();
			state = GameState::PLAYING;
			Global::getInstance()->playPop();
		}
	});

	ui->btnRestart->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) {
			ui->hidePausePanel();
			ui->setScore(0);
			this->destroyGame();
			this->initGame();
			this->startGame();
			state = GameState::PLAYING;
			Global::getInstance()->playPop();
		}
	});

	ui->btnHome->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) {
			auto scene = SceneHome::create();
			auto transit = TransitionSlideInR::create(0.125f, scene);
			Director::getInstance()->replaceScene(transit);
			Global::getInstance()->playPop();
		}
	});
}

void SceneIngame::initGame()
{
	Global::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			createBlock(i, k, rand()%BLOCK_VAR + 1);
		}
	}

	this->alignBlockSprite();
}

void SceneIngame::destroyUI()
{
}

void SceneIngame::destroyGame()
{
	Global::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			setBlockData(i, k, 0);
			getBlockSprite(i, k)->removeFromParent();
			setBlockSprite(i, k, nullptr);
		}
	}
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

	if (state == GameState::PLAYING) {
		if (p.x >= BLOCK_HORIZONTAL || p.x < 0) return true;
		if (p.y >= BLOCK_VERTICAL || p.y < 0) return true;

		CCLOG("%f, %f", p.x, p.y);
		destroyBlock(p.x, p.y);

		for (int i = 0; i < BLOCK_VERTICAL; i++) {
			judgeMatch(p.x, i);
		}
	}


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
	Global::getInstance()->playBackgroundMusic();
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
