#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object

	//	create函数调用了init函数
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//	这是游戏界面的背景图
	auto bg_game = Sprite::create("level-background-0.jpg");
	bg_game->setPosition(Vec2(480, 320));
	this->addChild(bg_game, 0);

	//	这是石头的layer和其上的sprite石头
	auto stoneLayer = Layer::create();
	stoneLayer->setName("stoneLayer");
	auto stone = Sprite::create("stone.png");
	stone->setName("stone");
	stone->setPosition(560, 480);
	stoneLayer->ignoreAnchorPointForPosition(false);
	stoneLayer->setAnchorPoint(Vec2(0, 0));
	stoneLayer->setPosition(0, 0);
	//	sprite添加到layer上，layer添加到父layer上
	stoneLayer->addChild(stone);
	this->addChild(stoneLayer, 1);

	//	这是老鼠的layer和其上的sprite老鼠
	auto MouseLayer = Layer::create();
	MouseLayer->setName("mouseLayer");
	//	Sprite Sheet创建精灵
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("level-sheet.plist");
	auto mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	//	设置mouse的名字，用以在onTouchBegan中获取sprite mouse
	mouse->setName("mouse");
	//	按作业要求进行设置
	mouse->setPosition(visibleSize.width / 2, 0);
	MouseLayer->ignoreAnchorPointForPosition(false);
	MouseLayer->setAnchorPoint(Vec2(0, 0));
	//	sprite添加到layer上，layer添加到父layer上
	MouseLayer->addChild(mouse);
	MouseLayer->setPosition(0, visibleSize.height / 2);
	this->addChild(MouseLayer, 1);

	//	http://www.jellythink.com/archives/764
	//	根据以上链接内容添加标签菜单项
	auto labelshoot = Label::createWithTTF("Shoot", "fonts/MF-TongZhen-Noncommercial-Regular.ttf", 72);
	auto shoot = MenuItemLabel::create(labelshoot, CC_CALLBACK_1(GameSence::shootCallback, this));
	shoot->setPosition(Vec2(visibleSize.width - 150, visibleSize.height - 100));
	auto menu = Menu::create(shoot, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//	用过以下代码创建shoot.jpg
	//auto labelshoot = Label::createWithTTF("Shoot", "fonts/MF-TongZhen-Noncommercial-Regular.ttf", 72);
	//labelshoot->setPosition(visibleSize.width - 150, visibleSize.height - 100);
	//this->addChild(labelshoot, 1);

	return true;
}

void GameSence::shootCallback(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto stone = this->getChildByName("stoneLayer")->getChildByName("stone");
	auto mouse = this->getChildByName("mouseLayer")->getChildByName("mouse");

	//	先让其正在进行的action都停下来，否则坐标会出错
	mouse->stopAllActions();
	stone->stopAllActions();

	stone->runAction(MoveTo::create(1, Vec2(mouse->getPositionX(), mouse->getPositionY() + visibleSize.height / 2)));

	/*	无动画版钻石
	auto diamond = Sprite::create("diamond.png");
	diamond->setPosition(Vec2(mouse->getPositionX(), mouse->getPositionY() + visibleSize.height / 2));
	this->addChild(diamond, 1);*/
	
	//	粗制滥造版有动画钻石
	auto diamond = Sprite::createWithSpriteFrameName("diamond-0.png");
	Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
	diamond->runAction(RepeatForever::create(diamondAnimate));
	diamond->setPosition(mouse->getPositionX(), mouse->getPositionY() + visibleSize.height / 2);
	this->addChild(diamond, 1);

	mouse->runAction(MoveTo::create(1, Vec2(5 + random() % (int)(visibleSize.width - 10), random() % (int)visibleSize.height - visibleSize.height / 2)));
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location.x, location.y);
	//	老师给的pdf运行不了,自行尝试如下可以运行
	auto delay = DelayTime::create(1.0f);  //  delay设置成2是因为下面老鼠设置了到达奶酪要1秒
	auto fadeOut = FadeOut::create(2.0f);  //  奶酪2秒被“吃完”
	auto seq = Sequence::create(delay, fadeOut, nullptr);  //  nullptr绝对不能省
	cheese->runAction(seq);
	this->addChild(cheese);

	auto mouse = this->getChildByName("mouseLayer")->getChildByName("mouse");
	//	如果快速在屏幕上疯狂点击的话，mouse绝大情况下去不到最后一个奶酪的地方
	//	因为mouse一下多了很多action，并且会先执行完之前的action再执行新添加的action
	//	所以当疯狂点击的时候，只执行最后一个添加的action，确保mouse最终停在最后的一个奶酪上
	//	由于作业要求没有明确说，所以剩下多余的奶酪不设置新的行为，按正常时间自动消失
	mouse->stopAllActions();
	//	不能是mouse->runAction(MoveTo::create(2, Vec2(location.x, location.y)));
	//	因为mouse的location.y是本地坐标，而其父节点的坐标系为（0，visbleSize.height / 2）
	mouse->runAction(MoveTo::create(1, Vec2(location.x, location.y - visibleSize.height / 2)));
	auto eating = DelayTime::create(2.0f);
	mouse->runAction(eating);

	return true;
}
