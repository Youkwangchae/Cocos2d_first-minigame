#include "ColorPopup.h"
#include "DatabaseManager.h"
#include "CharacterScene.h"

ColorPopup::ColorPopup()
{
}

ColorPopup::~ColorPopup()
{
}

ColorPopup * ColorPopup::create(std::string tableName, int no)
{
	ColorPopup *ret = new ColorPopup();

	if (ret && ret->init(tableName, no))
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);

	return ret;
}

bool ColorPopup::init(std::string tableName, int no)
{
	log("%s, %d", tableName.c_str(), no);

	_tableName = tableName;
	_no = no;

	//Device의 크기를 가져옵니다.
	auto winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto fadeBack = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
	this->addChild(fadeBack);
	fadeBack->runAction(FadeTo::create(0.5f, 200));

	auto back = Sprite::create("bg_color_box.png");
	back->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(back);

	//테이블 이름과 번호를 넣어 값을 가져온다.
	auto headList = DatabaseManager::getInstance()->selectDB(_tableName, _no);
	
	//첫번째 구조체 가져오기
	auto head = headList.front();

	auto color1 = Sprite::create("box_color.png");
	color1->setColor(head->color1);
	auto itemColor1 = MenuItemSprite::create(color1, NULL, CC_CALLBACK_1(ColorPopup::onClickColor, this));
	itemColor1->setTag(1);
	itemColor1->setPosition(Point(50, 45));

	auto color2 = Sprite::create("box_color.png");
	color2->setColor(head->color2);
	auto itemColor2 = MenuItemSprite::create(color2, NULL, CC_CALLBACK_1(ColorPopup::onClickColor, this));
	itemColor2->setTag(2);
	itemColor2->setPosition(Point(50 + 60, 45));

	auto color3 = Sprite::create("box_color.png");
	color3->setColor(head->color3); 
	auto itemColor3 = MenuItemSprite::create(color3, NULL, CC_CALLBACK_1(ColorPopup::onClickColor, this));
	itemColor3->setTag(3);
	itemColor3->setPosition(Point(50 + 120, 45));

	auto color4 = Sprite::create("box_color.png");
	color4->setColor(head->color4); 
	auto itemColor4 = MenuItemSprite::create(color4, NULL, CC_CALLBACK_1(ColorPopup::onClickColor, this));
	itemColor4->setTag(4);
	itemColor4->setPosition(Point(50 + 180, 45));

	auto menu = Menu::create(itemColor1, itemColor2, itemColor3, itemColor4, NULL);
	menu->setPosition(Point::ZERO);
	back->addChild(menu);

	return true;
}

void ColorPopup::onEnter()
{
	Layer::onEnter();

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
}

bool ColorPopup::onTouchBegan(Touch * touch, Event * event)
{
	return true;
}

void ColorPopup::onClickColor(Ref * object)
{
	log("onClickColor");
	int ColorTag = ((Node *)object)->getTag();

	//부모를 가져온다.
	auto parent = (CharacterScene *)this->getParent();

	//부모의 메서드를 호출한다.
	parent->setImage(_tableName, _no, ColorTag);
	this->removeFromParentAndCleanup(true);
}
