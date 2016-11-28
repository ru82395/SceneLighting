#include "MainScene.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

MainScene::~MainScene()
{
	delete _StartBtn;
	this->removeAllChildren();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	// 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainscene.plist");
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
    //addChild(rootNode);
	auto bgSprite = Sprite::createWithSpriteFrameName("background.png");
	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(bgSprite,0);


	_bToGameScene = false;
	_StartBtn = new CButton();
	_StartBtn->setButtonInfo("startnormal.png", "starton.png" , *this, Point(visibleSize.width / 2, visibleSize.height / 2));

	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::doStep));

    return true;
}

void MainScene::doStep(float dt)
{
	if ( _bToGameScene ) { // 切換到 SecondScene
		// 先將這個 SCENE 的 Update(這裡使用 OnFrameMove, 從 schedule update 中移出)
		this->unschedule(schedule_selector(MainScene::doStep));
//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// 設定場景切換的特效
//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(GameScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}

bool MainScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	_StartBtn->touchesBegin(touchLoc);
	return true;
}

void  MainScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();
	_StartBtn->touchesMoved(touchLoc);
}

void  MainScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();
	if (_StartBtn->touchesEnded(touchLoc)) { // 進行場景的切換
		_bToGameScene = true;
	}
}