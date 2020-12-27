#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "DatabaseManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CharacterScene : public cocos2d::Layer

{

public:

	CharacterScene();
	~CharacterScene();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();


	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	// implement the "static create()" method manually

	CREATE_FUNC(CharacterScene);

	Sprite *_characterBg;

	void onClickMenu(Ref *object);
	void onClickHome(Ref *object);
	void onClickRandom(Ref *object);
	void onClickSave(Ref *object);
	void onClickGallery(Ref *object);

	void setImage(std::string tableName, int rowNo, int colorNo);
	Sprite *_face;
	Sprite *_hair1;
	Sprite *_hair2;
	Sprite *_eye;
	Sprite *_mouth;
	Sprite *_etc;
	Sprite *_bgStyle;

	//포지션에 따라 말풍선의 화살표의 위치를 변경합니다.
	void setBalloon(int position);
	Sprite *_arrow;
	Sprite *_balloon;

	ScrollView *_scrollView;

	void setSubMenuItem(int position);

	std::string _currentTableName;
	void showColorPopup(Ref *object);

	character *_characterInfo;
};
