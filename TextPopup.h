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

	//�޼���ȣ��� ���� listner,
	Object *_listner;

	//OK��ư ���ý� callback���� ȣ���� �޼���, listner�� selector�� ȣ���Ѵ�.
	SEL_CallFunc _selector;

	void onEnter();
	bool onTouchBegan(Touch* touch, Event* event);
	void onClickOk(Ref *object);
	void onClickCancel(Ref *object);
};

