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
	if (!Scene::init()) return false;

	Sprite* bird = Sprite::create("res/frame-1.png");
	bird->setScale(0.2);
	bird->setPosition(Vec2(0 / 2, 720 / 2));
	addChild(bird);

	bird->runAction(
		RepeatForever::create(
			Sequence::create(
				CallFunc::create([=]() {bird->setFlippedX(false); }),
				EaseExponentialInOut::create(Spawn::create(
					MoveTo::create(1.0f, Vec2(1280, 720 / 2)),
					ScaleTo::create(1.0f, 0.1f),
					nullptr
				)),
				CallFunc::create([=]() {bird->setFlippedX(true); }),
				EaseExponentialInOut::create(Spawn::create(
					MoveTo::create(1.0f, Vec2(0, 720 / 2)),
					ScaleTo::create(1.0f, 0.2f),
					nullptr
				)),
				nullptr
			))
	);


	Animation* animation = Animation::create();
	animation->addSpriteFrameWithFile("res/frame-1.png");
	animation->addSpriteFrameWithFile("res/frame-2.png");
	animation->addSpriteFrameWithFile("res/frame-3.png");
	animation->addSpriteFrameWithFile("res/frame-4.png");
	animation->setDelayPerUnit(0.1);
	Animate* animate = Animate::create(animation);

	bird->runAction(RepeatForever::create(animate));

	bird->runAction(Sequence::create(DelayTime::create(5.0f), RemoveSelf::create(), nullptr));
	return true;
}
