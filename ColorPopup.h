#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ColorPopup : public Layer
{
public:
	ColorPopup();
	~ColorPopup();

	static ColorPopup *create(std::string tableName, int no);

	bool init(std::string tableName, int no);

	void onEnter();
	bool ColorPopup::onTouchBegan(Touch* touch, Event* event);

	std::string _tableName;
	int _no;

	void onClickColor(Ref *object);
};