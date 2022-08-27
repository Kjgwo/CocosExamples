#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__


#include "stdafx.h"

class SceneIngame : public Scene {
private:
	Node* player;
	bool up = false, down = false, left = false, right = false;
	bool fire = false;

public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override;

	void onKeyPressed(EventKeyboard::KeyCode c, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode c, Event* e);


	void logic(float dt);

};
#endif