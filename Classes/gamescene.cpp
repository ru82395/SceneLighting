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
	// ���� AnimationCache ���o���귽�A�_�h boyrunning.png ���|�Q���� 
	AnimationCache::destroyInstance();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("gamescene.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("boyrunning.plist");
	// ����
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

	// Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gamescene.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boyrunning.plist");

	auto rootNode = CSLoader::createNode("GameScene.csb");
	this->addChild(rootNode);

	// Ū���ëإ� _fgnode1
	// �Ĥ@�q�e��
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
	_fgnode1->addChild(_fg1obj[0], -1);	//�[�J_fgnode1 ��
	_fg1obj[1] = CFGObject::create();
	_fg1obj[1]->init("stonenode.csb", 3, _fg1Loc[1]);
	_fgnode1->addChild(_fg1obj[1], -1);	//�[�J_fgnode1 ��
	_fg1obj[2] = CFGObject::create();
	_fg1obj[2]->init("treenode.csb", 3, _fg1Loc[2]);
	_fgnode1->addChild(_fg1obj[2], -1);	//�[�J_fgnode1 ��
#else
	_fg1obj[0].init("treenode.csb", 3, _fg1Loc[0], *_fgnode1);
	_fg1obj[1].init("stonenode.csb", 3, _fg1Loc[1], *_fgnode1);
	_fg1obj[2].init("treenode.csb", 3, _fg1Loc[2], *_fgnode1);
#endif

	// �ĤG�q�e��
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
	_fgnode2->addChild(_fg2obj[0], -1);	// �[�J _fgnode2 ��
	_fg2obj[1] = CFGObject::create();
	_fg2obj[1]->init("treenode.csb", 3, _fg2Loc[1]);
	_fgnode2->addChild(_fg2obj[1], -1);	// �[�J _fgnode2 ��
	_fg2obj[2] = CFGObject::create();
	_fg2obj[2]->init("treenode.csb", 3, _fg2Loc[2]);
	_fgnode2->addChild(_fg2obj[2], -1);	// �[�J _fgnode2 ��
#else
	_fg2obj[0].init("stonenode.csb",3, _fg2Loc[0], *_fgnode2);
	_fg2obj[1].init("treenode.csb", 3, _fg2Loc[1], *_fgnode2);
	_fg2obj[2].init("treenode.csb", 3, _fg2Loc[2], *_fgnode2);
#endif


// ��^�����s
	_bToMainScene = false;
	_returnBtn = new CButton();
	_returnBtn->setButtonInfo("returnnormal.png", "returnon.png", *this, Point(124, 645));

	// �}�l�]�B���s
	_runningBtn = new CButton();
	_runningBtn->setButtonInfo("runbtnnormal.png", "runbtnon.png", *this, Point(110, 90));
	_iTouchID = -9999;	// �����@�ӭt��

	_bBoyRun = false;
	_boyAction = nullptr;
#ifndef LIGHTING
	// �q runnderNode ���o Cuber01_1 �ëإ� Sprite

	_boy = Sprite::createWithSpriteFrameName("boy_run_01.png");
	_boy->setPosition(320, 200);
	this->addChild(_boy, 10);

	// �إ� boy �һݪ��ǦC�V�ʵe
	auto boyanimCache = AnimationCache::getInstance();
	boyanimCache->addAnimationsWithFile("boyanim.plist");
	auto boyanim = boyanimCache->getAnimation("running");
	_boyAction = RepeatForever::create(Animate::create(boyanim));
	Director::getInstance()->getActionManager()->addAction(_boyAction, this->_boy, true);
#else

	// �إߥ������N���
	this->_sBulb = Sprite::createWithSpriteFrameName("lightbulb.png");
	this->_sBulb->setPosition(Vec2(400, 200));
	this->addChild(this->_sBulb, 20);

	// �إ߯S��
	this->_effect = LightEffect::create();
	this->_effect->retain();

	this->_vlightPos = Vec3(400, 200, 100);
	this->_effect->setLightPos(this->_vlightPos);
	this->_effect->setLightCutoffRadius(2000);
	this->_effect->setBrightness(1.5f);


	// �إ� SuperBoy animation
	// �Q�� STL �� vector ���x�s�s�� SpriteFrame
	Vector<SpriteFrame*> animFrames(11);
	char tmp[50];
	for (int j = 1; j <= 11; j++)
	{
		sprintf(tmp, "boy_run_%02d.png", j); // ���� boy_run_01.png ~  boy_run_11.png										  
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(tmp)); //�q plist �����o���ɦW�٨ëإ� spriteFrame
	}
	this->_boy = EffectSprite::createWithSpriteFrame(animFrames.front());
	Animation *superanim = Animation::createWithSpriteFrames(animFrames, 1.0f / 11);
	_boyAction = RepeatForever::create(Animate::create(superanim));
	this->_boy->runAction(_boyAction);	// ��������ʵe
	//Director::getInstance()->getActionManager()->addAction(_boyAction, this->_boy, true); // �[�J Action �]�w�� Pause

	this->_boy->setPosition(340, 200);
	addChild(this->_boy, 10);
	this->_boy->setEffect(this->_effect, "boyrunning_n.png");
	// �O�d��k�@
	Director::getInstance()->getTextureCache()->addImage("boyrunning_n.png")->retain();
	// �O�d��k�G
	auto boyNormal = Sprite::create("boyrunning_n.png");
	this->addChild(boyNormal);	// �����[�J�������A�_�h�|�Q����N�L�k�ϥ�
	boyNormal->setVisible(false);


	//// �����C��
	//	this->_effect->setLightColor(Color3B::WHITE);

	//// ���ҥ����C��
	this->_effect->setAmbientLightColor(Color3B::BLACK);

	//// �����G��
	//this->_effect->setBrightness(2.0f);

	//// �����̤j�b�|
	//this->_effect->setLightCutoffRadius(1000.0f);

	//// �����b�G�׶Z��
	//	this->_effect->setLightHalfRadius(0.5f);

#endif




#ifndef MULTI_TOUCH
	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�
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

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::doStep));

	return true;
}

void GameScene::resetFg(int n) // ���s���ͫe��1 �����󪺦�m�P���A
{


}

void GameScene::doStep(float dt)
{
	// �C��e���������� MOVESPEED ��PIXEL
	if(_bBoyRun ) {
		Point pt1 = _fgnode1->getPosition();
		pt1.x -= dt*MOVESPEED;
		if (pt1.x <= -640) {
			// �N�D�����ʨ� +1280+640 �A���������������W�L -640 �h�������A�ҥH�O 2 * 1280 + pt1.x
			pt1.x = 2 * 1280 + pt1.x;
			// ��s�e��������ܤ��e
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
		// ���N�o�� SCENE �� Update(�o�̨ϥ� OnFrameMove, �q schedule update �����X)
		this->unschedule(schedule_selector(GameScene::doStep));
		// �]�w�����������S��
		TransitionFade *pageTurn = TransitionFade::create(1.0F, MainScene::createScene());
		Director::getInstance()->replaceScene(pageTurn);
	}
	// �ĤH�X�{�b�W�ߪ��@�h�A�N�L���i��y�Ъ��ഫ
}

#ifndef MULTI_TOUCH 
bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
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

void  GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
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

void  GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
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
	_jumpTouchID = -1; // �ʧ@�����A�M��
}

unordered_map<int, Vec2>touchMap;

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchLoc = touch->getLocation();
		int  touchId = touch->getID();
		touchMap.insert(unordered_map<int, Vec2>::value_type(touchId, touchLoc)); // �x�sĲ���I�y�лP������ ID
			
		if (_runningBtn->touchesBegin(touchLoc)) {
			_bBoyRun = true;
			_iTouchID = touchId; // �����O���U�ثe�O���@��Ĳ���I���b�]�B�����s�W
		}
		else {  // �èS�����b _runningBtn �W
			if (_NoJumps == 0) {  // �Ĥ@���Q���U
				_boy->runAction(Sequence::create(_myJump, _mycallback, NULL));
				_NoJumps++;
				_jumpTouchID = touchId; // �O�U�o�� ID�A�p�G�H��n�Ψ쪺��
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

		auto PreLoc = touchMap.at(touchId);	// �d�߳o��ID �e�@��Ĳ���I���y��
		touchMap.erase(touchId);			// �R���o��ID �b MAP �������e
		touchMap.insert(unordered_map<int, Vec2>::value_type(touchId, touchLoc));  // �N�ثe�o�ӷs���[�J

		if (_iTouchID == touchId && _bBoyRun ) {	// ID �۲ŦӥB���b�]��
			if (!_runningBtn->touchesMoved(touchLoc)) {	// �P�_�O�_�w�g���}�]�B����
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
		auto PreLoc = touchMap.at(touchId);	// �d�߳o��ID �e�@��Ĳ���I���y��
		touchMap.erase(touchId);			// �R���o��ID �b MAP �������e

		if (_iTouchID == touchId && _bBoyRun ) { // ID �۲ŦӥB���b�]��
			_runningBtn->touchesEnded(touchLoc); // �I�s���s�A�����A��_
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