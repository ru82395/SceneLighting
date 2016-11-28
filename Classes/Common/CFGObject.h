#pragma once // �@�ӫD�зǦ��O�Q�s�x�䴩���e�m�B�z�Ÿ��A�����ɮ׸��J���l�ɮɡA�u�|�b�ӭ�l�ɤ��Q�]�t�@��



#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Common/const.h"

#if CFGOBJECT_VERSION == 1

class CFGObject : public cocos2d::Node
{
private:
	int _num;		// �ҥ]�t�i����������Ӽ�
	cocos2d::Node   *_objRoot;	// �C�@�Ӫ���]�t�T�ӥi�H������ܪ�����
	cocos2d::Sprite *_objSprite[3];  // �C�@�Ӫ�����ܥD��
	cocos2d::Point  _LocPt;		// �C�@�Ӫ��󪺹w�]��m
	
public:
	CFGObject();
	static CFGObject* create(); // �إߪ��󪺹���
	void init(const char* csbfilename, int n, const cocos2d::Point &loc);
	void resetObj();
};

#else

class CFGObject 
{
private:
	int _num;		// �ҥ]�t�i����������Ӽ�
	cocos2d::Node   *_objRoot;	// �C�@�Ӫ���]�t�T�ӥi�H������ܪ�����
	cocos2d::Sprite *_objSprite[3];  // �C�@�Ӫ�����ܥD��
	cocos2d::Point  _LocPt;		// �C�@�Ӫ��󪺹w�]��m

public:
	CFGObject();
	void init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Node &parent);
	void resetObj();
};
#endif
