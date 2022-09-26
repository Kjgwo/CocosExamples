#include "stdafx.h"
#include "SceneIngame.h"
#include "Bullet.h"
#include "Unit.h"
#include "EnemyComponent.h"

SceneIngame * SceneIngame::create()
{
	auto ret = new SceneIngame();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init()
{
	if (!Scene::initWithPhysics()) return false; // Scene이 물리공간을 가지게 됨

	auto keybd = EventListenerKeyboard::create();
	keybd->onKeyPressed = std::bind(&SceneIngame::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keybd->onKeyReleased = std::bind(&SceneIngame::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybd, this);

	auto contact = EventListenerPhysicsContact::create();
	contact->onContactBegin = std::bind(&SceneIngame::onContactBegin, this, std::placeholders::_1); // 두 개의 물리적인 무언가가 접촉 시 한번 실행
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact, this);
	
	
	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic));
	
	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();

	auto world = getPhysicsWorld();
	if(DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);  // PhysicsWorld에서 어떤식으로 객체를 보게 될 것인지 Debug할 때만 쓰임 
	world->setGravity(Vec2::ZERO);

	player = Unit::create(Size(75,75), PLAYER_MASK , TAG_PLAYER);
	player->setPosition(Vec2(1280 / 2, 720 / 2 - 200));
	addChild(player);


	auto enemy = Unit::create(Size(75, 75), ENEMY_MASK, TAG_ENEMY);
	enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::NORMAL));
	enemy->setPosition(Vec2(1280 / 2, 720 / 2 + 200));
	addChild(enemy);

}

Unit * SceneIngame::getPlayer()
{
	return player;
}

void SceneIngame::onKeyPressed(EventKeyboard::KeyCode c, Event * e)
{
	const bool value = true;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up = value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

void SceneIngame::onKeyReleased(EventKeyboard::KeyCode c, Event * e)
{
	const bool value = false;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up = value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

bool SceneIngame::onContactBegin(PhysicsContact & contact)
{
	int tagA = contact.getShapeA()->getBody()->getTag();
	int tagB = contact.getShapeB()->getBody()->getTag();

	Node* a = contact.getShapeA()->getBody()->getNode();
	Node* b = contact.getShapeB()->getBody()->getNode();

	if (tagA == TAG_PLAYER_BULLET && tagB == TAG_ENEMY) {
		b->removeFromParent();
	}
	if (tagB == TAG_ENEMY_BULLET && tagA == TAG_ENEMY) {
		a->removeFromParent();
	}

	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY) {
		a->removeFromParent();
		player = nullptr; 
	}

	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY) {
		b->removeFromParent();
		player = nullptr;
	}

	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY_BULLET) {
		a->removeFromParent();
		player = nullptr;
	}
	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY_BULLET) {
		b->removeFromParent();
		player = nullptr;
	}


	return true;
}

void SceneIngame::logic(float dt)
{
	if (!player) return;

	Vec2 pos = player->getPosition();


	if (up) pos += Vec2(0, dt * PLAYER_MOVEMENT_SPEDD);
	if (down) pos -= Vec2(0, dt * PLAYER_MOVEMENT_SPEDD);

	if (left)pos -= Vec2(dt * PLAYER_MOVEMENT_SPEDD, 0);
	if (right)pos += Vec2(dt * PLAYER_MOVEMENT_SPEDD, 0);

	player->setPosition(pos);


	if (fire) {
		auto bullet = Bullet::create(PLAYER_BULLET_MASK, TAG_PLAYER_BULLET);
		bullet->setPosition(player->getPosition());
		bullet->getBody()->setVelocity(Vec2(0, 1000)); // setVeloctiy(): 속도 계산
		bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));
		addChild(bullet);
	}

}
