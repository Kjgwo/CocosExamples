#include "stdafx.h"
#include "SceneHome.h"
#include "Environment.h"
#include "SceneIngame.h"

SceneHome * SceneHome::create()
{
	auto ret = new SceneHome();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneHome::init()
{
	if (!Scene::init()) return false;

	Sprite* img = Sprite::create("res/title_girl.png");
	addChild(img);
	img->setOpacity(0);
	img->runAction(FadeIn::create(0.25));
	img->setPosition(Vec2(720 / 2, 800));

	Sprite* title = Sprite::create("res/title.png");
	addChild(title);
	title->setPosition(Vec2(720 / 2, -100));
	title->runAction(EaseSineInOut::create(MoveTo::create(0.65f, Vec2(720 / 2, 600))));

	addChild(btnStart = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));
	addChild(btnQuit = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));
	
	btnStart->setPosition(Vec2(720 / 2, 400));
	btnQuit->setPosition(Vec2(720 / 2, 250));
	
	btnStart->setTitleFontName(FONT_NAME);
	btnQuit->setTitleFontName(FONT_NAME);

	btnStart->setTitleFontSize(40.0f);
	btnQuit->setTitleFontSize(40.0f);

	btnStart->setTitleColor(Color3B(0, 50, 50));
	btnQuit->setTitleColor(Color3B(0, 50, 50));

	btnStart->setTitleText("Start Game");
	btnQuit->setTitleText("Quit Game");

	Global::getInstance()->stopBackgroundMusic();

	btnStart->addClickEventListener([=](Ref* r) {
		auto scene = SceneIngame::create();
		auto transit = TransitionSlideInR::create(0.125f, scene);
		Director::getInstance()->replaceScene(transit);
		Global::getInstance()->playPop();
	});

	btnQuit->addClickEventListener([=](Ref* r) {
		Global::getInstance()->playPop();
		Director::getInstance()->end();
	});

	return true;

}
