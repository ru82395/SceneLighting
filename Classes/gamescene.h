#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "EffectSprite/LightEffect.h"
#include "EffectSprite/EffectSprite.h"
#include "Common/CButton.h"
#include "Common/CFGObject.h"
#include "Common/const.h"

#define MULTI_TOUCH 1
//#define LIGHTING 1

class GameScene : public cocos2d::Layer
{
private:
	cocos2d::Point _fg1Loc[3];	// _fgnode1 的前景物件產生的位置
	cocos2d::Point _fg2Loc[3];	// _fgnode2 的前景物件產生的位置
	cocos2d::Node *_fgnode1, *_fgnode2;	// 前景的兩段道路
	
	cocos2d::Action *_boyAction;

#ifndef LIGHTING
	cocos2d::Sprite *_boy;
#else
	EffectSprite *_boy;
#endif

	CButton *_returnBtn;
	bool _bToMainScene;

	CButton *_runningBtn;
	bool  _bBoyRun;
	int   _iTouchID;

	// 光源
	cocos2d::Sprite *_sBulb;
	LightEffect *_effect;
	Vec3 _vlightPos;

#if CFGOBJECT_VERSION == 1
	// 前景動態生成的物件
	CFGObject *_fg1obj[3], *_fg2obj[3];
#else
	// 前景動態生成的物件
	CFGObject _fg1obj[3], _fg2obj[3];
#endif

#ifdef MULTI_TOUCH
	// For Double Jump
	cocos2d::JumpBy   *_myJump;
	int _NoJumps;
	cocos2d::CallFunc *_mycallback;
	cocos2d::Point _runnerPt;
	int _jumpTouchID;
#endif

public:
	~GameScene();
	GameScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void doStep(float dt);
	void resetFg(int n); // 重新產生前景1 中物件的位置與狀態

#ifndef MULTI_TOUCH
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件 
#else
	void actionFinished();

	cocos2d::EventListenerTouchAllAtOnce *_listener1;
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
#endif




																	   // implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __Game_SCENE_H__
