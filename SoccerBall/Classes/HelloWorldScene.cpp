/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#define FORCE 200
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }


	//ball[0] = Sprite::create("res/SoccerBall.png");

	//ball[0]->setPosition(120, 120);
	//addChild(ball[0]);

	//ball[1] = Sprite::create("res/SoccerBall.png");

	//ball[1]->setPosition(320, 120);
	//addChild(ball[1]);

	//schedule(std::bind(&HelloWorld::logic, this, std::placeholders::_1), "logic");

	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [=](Touch* t, Event* e) ->bool {
		ball[count] = Sprite::create("res/SoccerBall.png");
		ball[count]->setPosition(t->getLocation().x, t->getLocation().y);
		addChild(ball[count]);
		schedule(std::bind(&HelloWorld::logic, this, std::placeholders::_1), "logic");
		count++;
		return true;
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

		

	

    return true;
}

void HelloWorld::logic(float dt)
{
	
	for (int i = 0; i < count; i++) {
		Vec2 position = ball[i]->getPosition();

		if (position.y < 0) {
			direction_y[i] = 2.0;
		}
		if (position.x < 0) {
			direction_x[i] = 2.0;
		}
		else if (position.x > 1280) {
			direction_x[i] = -2.0;
		}
		direction_y[i] = direction_y[i] - dt;

		position.y = position.y + (FORCE * dt) * direction_y[i];
		position.x = position.x + (FORCE * dt) * direction_x[i];

		ball[i]->setPosition(position);
	}






		/*Vec2 position = ball[0]->getPosition();

		if (position.y < 0) {
			direction = 2.0;
		}
		direction = direction - dt;

		position.y = position.y + (FORCE * dt) * direction;

		ball[0]->setPosition(position);

		Vec2 position1 = ball[1]->getPosition();

		if (position1.y < 0) {
			direction = 2.0;
		}
		direction = direction - dt;

		position1.y = position1.y + (FORCE * dt) * direction;

		ball[1]->setPosition(position1);*/


}


