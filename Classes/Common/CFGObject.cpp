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
	// Ū���ëإߪ��󪺤��e
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// �b�üƪ����ܤ@�U X �b�y��
	_objRoot->setPosition(pt);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// �H���M�w�@�Ӭ� true
	int i = rand() % n;
	_objSprite[i]->setVisible(true);
	this->addChild(_objRoot);
}
#else 

void CFGObject::init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Node &parent)
{
	_num = n;
	// Ū���ëإߪ��󪺤��e
	_objRoot = CSLoader::createNode(csbfilename);
	_LocPt = loc;
	Point pt = _LocPt;
	pt.x += (rand() % (SHIFT_X * 2) - SHIFT_X);	// �b�üƪ����ܤ@�U X �b�y��
	_objRoot->setPosition(pt);
	parent.addChild(_objRoot, -1);
	for (int i = 0; i < n; i++)
	{
		char objname[50];
		sprintf(objname, "object%1d", i + 1);
		_objSprite[i] = (Sprite *)_objRoot->getChildByName(objname);
		_objSprite[i]->setVisible(false);
	}
	// �H���M�w�@�Ӭ� true
	int i = rand() % n;
	_objSprite[i]->setVisible(true);
}

#endif


// ���ܪ��󪺦�m�P��ܪ����e
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

