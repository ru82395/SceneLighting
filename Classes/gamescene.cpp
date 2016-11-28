#include "GameScene.h"
#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <unordered_map>

USING_NS_CC;

#define MOVESPEED 125

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;
using namespace std;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

GameScene::GameScene()
{


}

GameScene::~GameScene()
{
	if (_boyAction != nullptr)  _boyAction->release();
//	if (_superboyAction != nullptr)  _superboyAction->release();
	delete _returnBtn;
	delete _runningBtn;
	this->removeAllChildren();
	// 釋放 AnimationCache 取得的資源，否則 boyrunning.png 不會被釋放 
	AnimationCache::destroyInstance();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("gamescene.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("boyrunning.plist");
	// 釋放
#ifdef LIGHTING 
	Director::getInstance()->getTextureCache()->getTextureForKey("boyrunning_n.png")->release();
#endif
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gamescene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boyrunning.plist");

	auto rootNode = CSLoader::createNode("GameScene.csb");
	this->addChild(rootNode);

	// 讀取並建立 _fgnode1
	// 第一段前景
	_fgnode1 = CSLoader::createNode("fgnode.csb");
	_fgnode1->setPosition(640, 0);
	addChild(_fgnode1,1);
	for (int i = 0; i < 3; i++ )
	{
		_fg1Loc[i].x = -400 +(float)i*400;
		_fg1Loc[i].y = 0;
	}

#if CFGOBJECT_VERSION == 1
	_fg1obj[0] = CFGObject::create();
	_fg1obj[0]->init("treenode.csb", 3, _fg1Loc[0]);
	_fgnode1->addChild(_fg1obj[0], -1);	//加入_fgnode1 中
	_fg1obj[1] = CFGObject::create();
	_fg1obj[1]->init("stonenode.csb", 3, _fg1Loc[1]);
	_fgnode1->addChild(_fg1obj[1], -1);	//加入_fgnode1 中
	_fg1obj[2] = CFGObject::create();
	_fg1obj[2]->init("treenode.csb", 3, _fg1Loc[2]);
	_fgnode1->addChild(_fg1obj[2], -1);	//加入_fgnode1 中
#else
	_fg1obj[0].init("treenode.csb", 3, _fg1Loc[0], *_fgnode1);
	_fg1obj[1].init("stonenode.csb", 3, _fg1Loc[1], *_fgnode1);
	_fg1obj[2].init("treenode.csb", 3, _fg1Loc[2], *_fgnode1);
#endif

	// 第二段前景
	_fgnode2 = CSLoader::createNode("fgnode.csb");
	_fgnode2->setPosition(1280+640, 0);
	this->addChild(_fgnode2, 1);
	for (int i = 0; i < 3; i++)
	{
		_fg2Loc[i].x = -500 + (float)i * 420;
		_fg2Loc[i].y = 0;
	}

#if CFGOBJECT_VERSION == 1
	_fg2obj[0] = CFGObject::create();
	_fg2obj[0]->init("stonenode.csb", 3, _fg2Loc[0]);
	_fgnode2->addChild(_fg2obj[0], -1);	// 加入 _fgnode2 中
	_fg2obj[1] = CFGObject::create();
	_fg2obj[1]->init("treenode.csb", 3, _fg2Loc[1]);
	_fgnode2->addChild(_fg2obj[1], -1);	// 加入 _fgnode2 中
	_fg2obj[2] = CFGObject::create();
	_fg2obj[2]->init("treenode.csb", 3, _fg2Loc[2]);
	_fgnode2->addChild(_fg2obj[2], -1);	// 加入 _fgnode2 中
#else
	_fg2obj[0].init("stonenode.csb",3, _fg2Loc[0], *_fgnode2);
	_fg2obj[1].init("treenode.csb", 3, _fg2Loc[1], *_fgnode2);
	_fg2obj[2].init("treenode.csb", 3, _fg2Loc[2], *_fgnode2);
#endif


// 返回的按鈕
	_bToMainScene = false;
	_returnBtn = new CButton();
	_returnBtn->setButtonInfo("returnnormal.png", "returnon.png", *this, Point(124, 645));

	// 開始跑步按鈕
	_runningBtn = new CButton();
	_runningBtn->setButtonInfo("runbtnnormal.png", "runbtnon.png", *this, Point(110, 90));
	_iTouchID = -9999;	// 給予一個負值

	_bBoyRun = false;
	_boyAction = nullptr;
#ifndef LIGHTING
	// 從 runnderNode 取得 Cuber01_1 並建立 Sprite

	_boy = Sprite::createWithSpriteFrameName("boy_run_01.png");
	_boy->setPosition(320, 200);
	this->addChild(_boy, 10);

	// 建立 boy 所需的序列幀動畫
	auto boyanimCache = AnimationCache::getInstance();
	boyanimCache->addAnimationsWithFile("boyanim.plist");
	auto boyanim = boyanimCache->getAnimation("running");
	_boyAction = RepeatForever::create(Animate::create(boyanim));
	Director::getInstance()->getActionManager()->addAction(_boyAction, this->_boy, true);
#else

	// 建立光源的代表圖
	this->_sBulb = Sprite::createWithSpriteFrameName("lightbulb.png");
	this->_sBulb->setPosition(Vec2(400, 200));
	this->addChild(this->_sBulb, 20);

	// 建立特效
	this->_effect = LightEffect::create();
	this->_effect->retain();

	this->_vlightPos = Vec3(400, 200, 100);
	this->_effect->setLightPos(this->_vlightPos);
	this->_effect->setLightCutoffRadius(2000);
	this->_effect->setBrightness(1.5f);


	// 建立 SuperBoy animation
	// 利用 STL 的 vector 來儲存連續的 SpriteFrame
	Vector<SpriteFrame*> animFrames(11);
	char tmp[50];
	for (int j = 1; j <= 11; j++)
	{
		sprintf(tmp, "boy_run_%02d.png", j); // 產生 boy_run_01.png ~  boy_run_11.png										  
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(tmp)); //從 plist 中取得圖檔名稱並建立 spriteFrame
	}
	this->_boy = EffectSprite::createWithSpriteFrame(animFrames.front());
	Animation *superanim = Animation::createWithSpriteFrames(animFrames, 1.0f / 11);
	_boyAction = RepeatForever::create(Animate::create(superanim));
	this->_boy->runAction(_boyAction);	// 直接撥放動畫
	//Director::getInstance()->getActionManager()->addAction(_boyAction, this->_boy, true); // 加入 Action 設定成 Pause

	this->_boy->setPosition(340, 200);
	addChild(this->_boy, 10);
	this->_boy->setEffect(this->_effect, "boyrunning_n.png");
	// 保留方法一
	Director::getInstance()->getTextureCache()->addImage("boyrunning_n.png")->retain();
	// 保留方法二
	auto boyNormal = Sprite::create("boyrunning_n.png");
	this->addChild(boyNormal);	// 必須加入場景中，否則會被釋放將無法使用
	boyNormal->setVisible(false);


	//// 光源顏色
	//	this->_effect->setLightColor(Color3B::WHITE);

	//// 環境光源顏色
	this->_effect->setAmbientLightColor(Color3B::BLACK);

	//// 光源亮度
	//this->_effect->setBrightness(2.0f);

	//// 光源最大半徑
	//this->_effect->setLightCutoffRadius(1000.0f);

	//// 光源半亮度距離
	//	this->_effect->setLightHalfRadius(0.5f);

#endif




#ifndef MULTI_TOUCH
	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);		//加入觸碰離開事件
#else
	_NoJumps = 0;
	_myJump = JumpBy::create(0.5f, Point(0, 0), 150, 1);
	_myJump->retain();
	_runnerPt = _boy->getPosition();
	_mycallback = CallFunc::create(CC_CALLBACK_0(GameScene::actionFinished, this));

	_mycallback->retain();
	_jumpTouchID = -1;

	_listener1 = EventListenerTouchAllAtOnce::create();
	_listener1->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	_listener1->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	_listener1->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
#endif

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::doStep));

	return true;
}

void GameScene::resetFg(int n) // 重新產生前景1 中物件的位置與狀態
{


}

void GameScene::doStep(float dt)
{
	// 每秒前景往左移動 MOVESPEED 個PIXEL
	if(_bBoyRun ) {
		Point pt1 = _fgnode1->getPosition();
		pt1.x -= dt*MOVESPEED;
		if (pt1.x <= -640) {
			// 將道路移動到 +1280+640 ，但必須扣除本身超過 -640 多的部分，所以是 2 * 1280 + pt1.x
			pt1.x = 2 * 1280 + pt1.x;
			// 更新前景物件的顯示內容
#if CFGOBJECT_VERSION == 1
			_fg1obj[0]->resetObj(); _fg1obj[1]->resetObj();	_fg1obj[2]->resetObj();
#else
			_fg1obj[0].resetObj(); _fg1obj[1].resetObj(); _fg1obj[2].resetObj();
#endif
		}
		_fgnode1->setPosition(pt1);

		Point pt2 = _fgnode2->getPosition();
		pt2.x -= dt*MOVESPEED;
		if (pt2.x <= -640) {
			pt2.x = 2 * 1280 + pt2.x;
#if CFGOBJECT_VERSION == 1
			_fg2obj[0]->resetObj(); _fg2obj[1]->resetObj();	_fg2obj[2]->resetObj();
#else
			_fg2obj[0].resetObj(); _fg2obj[1].resetObj(); _fg2obj[2].resetObj();
#endif
		}
		_fgnode2->setPosition(pt2);
	}


	if (_bToMainScene) {
		// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		this->unschedule(schedule_selector(GameScene::doStep));
		// 設定場景切換的特效
		TransitionFade *pageTurn = TransitionFade::create(1.0F, MainScene::createScene());
		Director::getInstance()->replaceScene(pageTurn);
	}
	// 敵人出現在獨立的一層，就無須進行座標的轉換
}

#ifndef MULTI_TOUCH 
bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();

#ifdef LIGHTING
	this->_sBulb->setPosition(touchLoc);
	this->_vlightPos = Vec3(this->_sBulb->getPositionX(), this->_sBulb->getPositionY(),100);
	this->_effect->setLightPos(this->_vlightPos);
#endif

	_returnBtn->touchesBegin(touchLoc);

	if ( _runningBtn->touchesBegin(touchLoc) ) {
		_bBoyRun = true;
	}
	return true;
}

void  GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();

#ifdef LIGHTING
	this->_sBulb->setPosition(touchLoc);
	this->_vlightPos = Vec3(this->_sBulb->getPositionX(), this->_sBulb->getPositionY(), 100);
	this->_effect->setLightPos(this->_vlightPos);
#endif
	_returnBtn->touchesMoved(touchLoc);

	if ( _bBoyRun && !(_runningBtn->touchesMoved(touchLoc))) {
		_bBoyRun = false;
//		Director::getInstance()->getActionManager()->pauseTarget(this->_boy);
	}

}

void  GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();

#ifdef LIGHTING
	this->_sBulb->setPosition(touchLoc);
	this->_vlightPos = Vec3(this->_sBulb->getPositionX(), this->_sBulb->getPositionY(), 100);
	this->_effect->setLightPos(this->_vlightPos);
#endif

	if (_returnBtn->touchesEnded(touchLoc)) {
		_bToMainScene = true;
	}
	if (_bBoyRun && _runningBtn->touchesEnded(touchLoc) ) {
		_bBoyRun = false;
//		Director::getInstance()->getActionManager()->pauseTarget(this->_boy);
	}
}
#else

void GameScene::actionFinished()
{
	// do something on complete
	_NoJumps--;
	_jumpTouchID = -1; // 動作結束，清除
}

unordered_map<int, Vec2>touchMap;

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchLoc = touch->getLocation();
		int  touchId = touch->getID();
		touchMap.insert(unordered_map<int, Vec2>::value_type(touchId, touchLoc)); // 儲存觸控點座標與對應的 ID
			
		if (_runningBtn->touchesBegin(touchLoc)) {
			_bBoyRun = true;
			_iTouchID = touchId; // 必須記錄下目前是哪一個觸控點按在跑步的按鈕上
		}
		else {  // 並沒有按在 _runningBtn 上
			if (_NoJumps == 0) {  // 第一次被按下
				_boy->runAction(Sequence::create(_myJump, _mycallback, NULL));
				_NoJumps++;
				_jumpTouchID = touchId; // 記下這個 ID，如果以後要用到的話
			}
			_returnBtn->touchesBegin(touchLoc);
		}
	}
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchLoc = touch->getLocation();
		int  touchId = touch->getID();

		auto PreLoc = touchMap.at(touchId);	// 查詢這個ID 前一個觸控點的座標
		touchMap.erase(touchId);			// 刪除這個ID 在 MAP 中的內容
		touchMap.insert(unordered_map<int, Vec2>::value_type(touchId, touchLoc));  // 將目前這個新的加入

		if (_iTouchID == touchId && _bBoyRun ) {	// ID 相符而且正在跑中
			if (!_runningBtn->touchesMoved(touchLoc)) {	// 判斷是否已經移開跑步按扭
				_bBoyRun = false;
				_iTouchID = -1;
			}
		}
		_returnBtn->touchesMoved(touchLoc);
	}
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchLoc = touch->getLocation();
		int  touchId = touch->getID();
		auto PreLoc = touchMap.at(touchId);	// 查詢這個ID 前一個觸控點的座標
		touchMap.erase(touchId);			// 刪除這個ID 在 MAP 中的內容

		if (_iTouchID == touchId && _bBoyRun ) { // ID 相符而且正在跑中
			_runningBtn->touchesEnded(touchLoc); // 呼叫按鈕，讓狀態恢復
			_bBoyRun = false;
			_iTouchID = -1;
		}
		if (_returnBtn->touchesEnded(touchLoc)) {
			_bToMainScene = true;
		}
	}
}

void GameScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
	onTouchesEnded(touches, event);
}
#endif