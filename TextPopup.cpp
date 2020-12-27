#include "TextPopup.h"

TextPopup::TextPopup()
{
}

TextPopup::~TextPopup()
{
}

TextPopup * TextPopup::create(std::string text, Object * listner, SEL_CallFunc selector, bool isCancelBT)
{
	TextPopup *ret = new TextPopup();

	if (ret && ret->init(text, listner, selector, isCancelBT))
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	
	return ret;
}

bool TextPopup::init(std::string text, Object * listner, SEL_CallFunc selector, bool isCancelBT)
{
	_listner = listner;
	_selector = selector;

	//���⿡ �˾��� �ۼ��Ѵ�.
	auto winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
	this->addChild(fadeBack);
	fadeBack->runAction(FadeTo::create(0.5f, 200));

	auto back = Sprite::create("pop_up_bg.png");
	back->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(back);

	//�� �߰�
	auto labelSize = Size(back->getContentSize().width, back->getContentSize().height - 50);

	//���ڿ��� ���� ����
	char utf8Text[255];


#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	//win32 utf-8
	wchar_t* wChar;
	wChar = new WCHAR[255];

	//wChar�� text�� ��´�.
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), strlen(text.c_str()) + 1, wChar, 255);

	//wChar�� ���ڵ��Ͽ� utf8Text�� ��´�.
	WideCharToMultiByte(CP_UTF8, 0, wChar, -1, utf8Text, 1024, NULL, NULL);

#endif

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	//win32�� �ƴϸ� utf8Text�� ���� text�� �ִ´�.
	sprintf(utf8Text, "%s", text.c_str());
#endif

	//�ؽ�Ʈ, ��Ʈ, ��Ʈũ��, ��ũ��, ����
	auto label = LabelTTF::create(utf8Text, "Arial", 30, labelSize, TextHAlignment::CENTER);
	label->setPosition(Point(back->getContentSize().width / 2, back->getContentSize().height / 2));
	label->setColor(Color3B::BLACK);
	back->addChild(label);

	Vector<MenuItem*> itemArray;
	auto okBT = MenuItemImage::create("btn_ok.png", "btn_ok_on.png", CC_CALLBACK_1(TextPopup::onClickOk, this));
	itemArray.pushBack(okBT);

	if (!isCancelBT) 
	{
		//cancelBT�� false�̸� OK ��ư�� �����Ѵ�.
		okBT->setPosition(Point(back->getContentSize().width / 2, 25));
	}
	else 
	{
		//CancelBT�� true�� OK��ư�� Cancel ��ư�� �Ѵ� �����Ѵ�.
		okBT->setPosition(Point(back->getContentSize().width / 2 - 50, 25));

		auto cancelBT = MenuItemImage::create("btn_cancel.png", "btn_cancel_on.png", CC_CALLBACK_1(TextPopup::onClickCancel, this));
		itemArray.pushBack(cancelBT);
		cancelBT->setPosition(Point(back->getContentSize().width / 2 + 50, 25));
	}

	auto menu = Menu::createWithArray(itemArray);
	menu->setPosition(Point::ZERO);
	back->addChild(menu);

	return true;
}

void TextPopup::onEnter()
{
	Layer::onEnter();

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
}

bool TextPopup::onTouchBegan(Touch * touch, Event * event)
{
	return true;
}

void TextPopup::onClickOk(Ref * object)
{
	if (_listner != NULL && _selector != NULL)
		(_listner->*_selector)();        //�޾� ���� selector�� ȣ���Ѵ�.
	onClickCancel(NULL);
}

void TextPopup::onClickCancel(Ref * object)
{
	this->removeFromParentAndCleanup(true);
}
