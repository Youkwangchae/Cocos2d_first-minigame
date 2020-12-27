#pragma once
#include "cocos2d.h"

USING_NS_CC;

class TextPopup : public Layer
{
public:
	TextPopup();
	~TextPopup();

	static TextPopup* create(std::string text, Object *listner, SEL_CallFunc selector, bool isCancelBT);
	bool init(std::string text, Object *listner, SEL_CallFunc selector, bool isCancelBT);

	//메서드호출시 받을 listner,
	Object *_listner;

	//OK버튼 선택시 callback으로 호출할 메서드, listner의 selector를 호출한다.
	SEL_CallFunc _selector;

	void onEnter();
	bool onTouchBegan(Touch* touch, Event* event);
	void onClickOk(Ref *object);
	void onClickCancel(Ref *object);
};

