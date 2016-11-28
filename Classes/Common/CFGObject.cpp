#include "CFGObject.h"

#define SHIFT_X 30

USING_NS_CC;
using namespace cocostudio::timeline;


CFGObject::CFGObject()
{
	_objRoot = nullptr;
	for (int i = 0; i < 3; i++) _objSprite[i] = nullptr;
}


#if CFGOBJECT_VERSION == 1
CFGObject* CFGObject::create()
{
	CFGObject *fgobj = new (std::nothrow) CFGObject();
	if (fgobj)
	{
		fgobj->autorelease();
		return fgobj;
	}
	CC_SAFE_DELETE(fgobj);
	return nullptr;
}

void CFGObject::init(const char* csbfilename, int n, const cocos2d::Point &loc)
{
	_num = n;
	// 讀取並建立物件的內容
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// 在亂數的改變一下 X 軸座標
	_objRoot->setPosition(pt);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// 隨機決定一個為 true
	int i = rand() % n;
	_objSprite[i]->setVisible(true);
	this->addChild(_objRoot);
}
#else 

void CFGObject::init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Node &parent)
{
	_num = n;
	// 讀取並建立物件的內容
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// 在亂數的改變一下 X 軸座標
	_objRoot->setPosition(pt);
	parent.addChild(_objRoot, -1);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// 隨機決定一個為 true
	int i = rand() % n;
	_objSprite[i]->setVisible(true);
}

#endif


// 改變物件的位置與顯示的內容
void CFGObject::resetObj() 
{
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);
	_objRoot->setPosition(pt);
	switch (rand() % _num)
	{
	case 0:
		_objSprite[0]->setVisible(true);
		_objSprite[1]->setVisible(false);
		if (_num == 3 )_objSprite[2]->setVisible(false);
		break;
	case 1:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(true);
		if (_num == 3 )_objSprite[2]->setVisible(false);
		break;
	case 2:
		_objSprite[0]->setVisible(false);
		_objSprite[1]->setVisible(false);
		if (_num == 3 )_objSprite[2]->setVisible(true);
		break;
	default:
		break;
	}
}

