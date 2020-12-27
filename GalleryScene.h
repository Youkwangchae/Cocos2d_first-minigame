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

	//�����۵��� �߰��� �޼���
	void setItems();

	//�������� �̹����� ��ȣ�� ���� ���߾� ������, CharacterScene�� setImage()�� ����ϰ� �����ϳ� Sprite�� return�ϵ��� ����
	static cocos2d::Sprite* getImage(std::string tableName, int rowNo, int colorNo); //StartSceneŬ�������� (�ٸ� Ŭ��������) ȣ���ϱ� ���� static���� �����߽��ϴ�.
	void onClickItemCallback(Ref *object);
	int _removeNo;
	void removeItem();
	
};

