#include "MenuSence.h"
#include "GameSence.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//	主界面石头
	auto bg_start = Sprite::create("menu-start-gold.png");
	bg_start->setPosition(Vec2(visibleSize.width - 200, 150));
	//	主界面START
	auto closeItem = MenuItemImage::create(
		"start-0.png",
		"start-1.png",
		CC_CALLBACK_1(MenuSence::startMenuCallback, this));
	closeItem->setAnchorPoint(Vec2(0, 0));	//  can not omit
	closeItem->setPosition(Vec2(60, 140));	//	take a long time to set
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2(0, 0));	//	can not omit
	bg_start->addChild(menu);
	this->addChild(bg_start, 1);
	//	主界面GOLD MINER文字
	auto text = Sprite::create("gold-miner-text.png");
	text->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200));
	this->addChild(text, 1);

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	//	添加抖腿动画
	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

    return true;
}

void MenuSence::startMenuCallback(cocos2d::Ref* pSender)
{
	auto scene = GameSence::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
