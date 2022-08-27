#include "stdafx.h"
#include "SceneIngame.h"

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

	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic));
	
	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();

	auto world = getPhysicsWorld();
	if(DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);  // PhysicsWorld에서 어떤식으로 객체를 보게 될 것인지 Debug할 때만 쓰임 
	world->setGravity(Vec2::ZERO);

	player = Node::create();
	player->setPosition(Vec2(1280 / 2, 720 / 2));

	auto body = PhysicsBody::createBox(Size(75, 75));
	body->setRotationEnable(false); // player 객체 회전 불가
	body->setCollisionBitmask(0);
	player->addComponent(body);

	addChild(player);


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
		auto bullet = Node::create();
		auto body = PhysicsBody::createCircle(5);
		bullet->setPosition(player->getPosition());
		bullet->addComponent(body);
		body->setVelocity(Vec2(0, 1000)); // 속도
		body->setCollisionBitmask(0);
		bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));
		addChild(bullet);
	}

}
