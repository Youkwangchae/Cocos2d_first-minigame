#pragma once
#include "cocos2d.h"
class GalleryScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GalleryScene);
	void onClickHome(Ref *object);
	void onClickMake(Ref *object);

	//아이템들을 추가할 메서드
	void setItems();

	//아이템의 이미지를 번호와 색상에 맞추어 가져옴, CharacterScene의 setImage()와 비슷하게 동작하나 Sprite를 return하도록 구현
	static cocos2d::Sprite* getImage(std::string tableName, int rowNo, int colorNo); //StartScene클래스에서 (다른 클래스에서) 호출하기 위해 static으로 지정했습니다.
	void onClickItemCallback(Ref *object);
	int _removeNo;
	void removeItem();
	
};

