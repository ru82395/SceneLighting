#pragma once // 一個非標準但是被廣泛支援的前置處理符號，讓該檔案載入到原始檔時，只會在該原始檔內被包含一次



#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Common/const.h"

#if CFGOBJECT_VERSION == 1

class CFGObject : public cocos2d::Node
{
private:
	int _num;		// 所包含可切換的物件個數
	cocos2d::Node   *_objRoot;	// 每一個物件包含三個可以切換顯示的物件
	cocos2d::Sprite *_objSprite[3];  // 每一個物件的顯示主體
	cocos2d::Point  _LocPt;		// 每一個物件的預設位置
	
public:
	CFGObject();
	static CFGObject* create(); // 建立物件的實體
	void init(const char* csbfilename, int n, const cocos2d::Point &loc);
	void resetObj();
};

#else

class CFGObject 
{
private:
	int _num;		// 所包含可切換的物件個數
	cocos2d::Node   *_objRoot;	// 每一個物件包含三個可以切換顯示的物件
	cocos2d::Sprite *_objSprite[3];  // 每一個物件的顯示主體
	cocos2d::Point  _LocPt;		// 每一個物件的預設位置

public:
	CFGObject();
	void init(const char* csbfilename, int n, const cocos2d::Point &loc, cocos2d::Node &parent);
	void resetObj();
};
#endif
