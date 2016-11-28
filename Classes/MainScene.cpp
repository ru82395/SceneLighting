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

	// Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mainscene.plist");
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
    //addChild(rootNode);
	auto bgSprite = Sprite::createWithSpriteFrameName("background.png");
	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(bgSprite,0);


	_bToGameScene = false;
	_StartBtn = new CButton();
	_StartBtn->setButtonInfo("startnormal.png", "starton.png" , *this, Point(visibleSize.width / 2, visibleSize.height / 2));

	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(MainScene::doStep));

    return true;
}

void MainScene::doStep(float dt)
{
	if ( _bToGameScene ) { // ������ SecondScene
		// ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
		this->unschedule(schedule_selector(MainScene::doStep));
//		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("mainscene.plist");
		// �]�w�����������S��
//		TransitionFade *pageTurn = TransitionFade::create(1.0F, GameScene::createScene());
//		Director::getInstance()->replaceScene(pageTurn);
		Director::getInstance()->replaceScene(GameScene::createScene());
		//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}

bool MainScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();
	_StartBtn->touchesBegin(touchLoc);
	return true;
}

void  MainScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();
	_StartBtn->touchesMoved(touchLoc);
}

void  MainScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();
	if (_StartBtn->touchesEnded(touchLoc)) { // �i�����������
		_bToGameScene = true;
	}
}