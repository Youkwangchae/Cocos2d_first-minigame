#include "GalleryScene.h"
#include "StartScene.h"
#include "DatabaseManager.h"
#include "TextPopup.h"

USING_NS_CC;

cocos2d::Scene * GalleryScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GalleryScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GalleryScene::init()
{
	// on "init" you need to initialize your instance
	
		//////////////////////////////
		// 1. super init first
		if (!Layer::init())
			return false;
	
		//Device의 크기를 가져옵니다.
		Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

		//Sprite를 생성하여 이미지를 불러옵니다.
		auto back = Sprite::create("bg_gallery.png");

		//back를 해당 포인트에 위치시킵니다. 화면의 정중앙에 위치하도록 했습니다.
		back->setPosition(Point(winSize.width / 2, winSize.height / 2));

		//this에 back을 자식 노드로 추가하였습니다.
		this->addChild(back);
		auto home = MenuItemImage::create("menu_home2.png", "menu_home2_on.png", CC_CALLBACK_1(GalleryScene::onClickHome, this));
		home->setAnchorPoint(Point(0, 0));
		home->setPosition(Point(0, 0));

		auto make = MenuItemImage::create("menu_make.png", "menu_make_on.png", CC_CALLBACK_1(GalleryScene::onClickMake, this));
		make->setAnchorPoint(Point(1, 0));
		make->setPosition(Point(winSize.width, 0));

		auto menu2 = Menu::create(home, make, NULL);
		menu2->setPosition(Point::ZERO);

		this->addChild(menu2);

		auto galleryLayer = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, 422);
	
		//LayerColor는 기본 AnchorPoint가 (0, 0)이다.
		galleryLayer->setPosition(Point(0, 48));

		//tag를 1로지정해주었다.
		this->addChild(galleryLayer, 0, 1);

		setItems();

		return true;	
}

void GalleryScene::onClickHome(Ref * object)
{
	log("onClickHome");
	auto Scene = StartScene::createScene();

	//StartScene으로 이동하는데 지금 Scene을 StartScene에서 돌아오는게 아니라서 replaceScene을 해주었습니다.
	Director::getInstance()->replaceScene(Scene);
}

void GalleryScene::onClickMake(Ref * object)
{
	log("onClickMake");
	//이전화면이 캐릭터 만드는 화면이므로 지금 Scene을 Director에서 pop합니다.
	Director::getInstance()->popScene();
}

void GalleryScene::setItems()
{
	//galleryLayer를 this에 tag가 1로 addchild 해주었다. getChildByTag는 해당 노드의 tag번호에 맞는 자식 노드를 반환한다.
	auto galleryLayer = (LayerColor *)this->getChildByTag(1);

	//removeAllChildrenWithCleanup()는 자식노드 전부를 삭제한다.
	galleryLayer->removeAllChildrenWithCleanup(true);

	std::list<character *>galleryList = DatabaseManager::getInstance()->selectGalleryDB(false);
	int size = galleryList.size();

	Vector<MenuItem*> itemArray;
	for (int i = 1; i <= size; i++) 
	{
		character *item = galleryList.front();

		auto back = Sprite::create("s_bg_1.png");
		auto head = getImage("TB_FACE", item->headNo, item->headColorNo);
		auto hair1 = getImage("TB_HAIR1", item->hair1No, item->hair1ColorNo);
		auto hair2 = getImage("TB_HAIR2", item->hair2No, item->hair2ColorNo);
		auto eye = getImage("TB_EYE", item->eyeNo, item->eyeColorNo);
		auto mouth = getImage("TB_MOUTH", item->mouthNo, item->mouthColorNo);
		auto etc = getImage("TB_ETC", item->etcNo, item->etcNo);
		auto bg = getImage("TB_BG", item->bgNo, item->bgColorNo);

		//getImage()에서 setPosition이 되어있기에 따로 지정하지 않아도 된다.
		back->addChild(bg, 0);
		back->addChild(hair2, 1);
		back->addChild(head, 2);
		back->addChild(eye, 3);
		back->addChild(mouth, 3);
		back->addChild(hair1, 4);
		back->addChild(etc, 5);

		auto frame = Sprite::create("box_gallery.png");
		frame->setPosition(Point(back->getContentSize().width / 2, back->getContentSize().height / 2));
		back->addChild(frame, 6);
		auto MenuItem = MenuItemSprite::create(back, NULL, CC_CALLBACK_1(GalleryScene::onClickItemCallback, this));
		MenuItem->setTag(item->no);        //삭제 기능을 위해 번호를 넣어준다.

		//55px 로 크기를 줄이기 위해 scale을 구함
		float scale = 60 / back->getContentSize().width;

		MenuItem->setScale(scale);

		int row = (i - 1) / 4;    //0~3
		int col = (i - 1) % 4;    //0~3
		//row와 col을 구한다.

		MenuItem->setAnchorPoint(Point(0, 1));
		//좌표를 동적할당
		float x = col * MenuItem->getContentSize().width * scale + (col + 3) * 20;
		float y = galleryLayer->getContentSize().height - (row * MenuItem->getContentSize().height * scale + (row + 1) * 15);
		MenuItem->setPosition(Point(x, y));
		itemArray.pushBack(MenuItem);

		galleryList.pop_front();
	}
	auto menu = Menu::createWithArray(itemArray);
	menu->setPosition(Point::ZERO);
	galleryLayer->addChild(menu);
}

cocos2d::Sprite * GalleryScene::getImage(std::string tableName, int rowNo, int colorNo)
{
	//매개변수에 맞춰 headList를 가져온다.
	std::list<head *>headList = DatabaseManager::getInstance()->selectDB(tableName, rowNo);

	//첫번째 구조체 가져오기
	head * head = headList.front();

	char smallImg[100];
	sprintf(smallImg, "s_%s", head->image);
	auto sprite = Sprite::create(smallImg);

	if (head->isColor) 
	{
		Color3B color;

		switch (colorNo)
		{
		case 1:
			color = head->color1;
			break;

		case 2:
			color = head->color2;
			break;

		case 3:
			color = head->color3;
			break;

		case 4:
			color = head->color4;
			break;

		}
		sprite->setColor(color);
	}

	//비율에 맞춰 위치를 조절해준다.
	sprite->setPosition(Point(((head->position.x)) *0.2f+6, ((head->position.y)) *0.2f+6));

	return sprite;
}

void GalleryScene::onClickItemCallback(Ref * object)
{
	auto MenuItem = (MenuItemSprite *)object;
	log("tag : %d", MenuItem->getTag()); 
	
	_removeNo = MenuItem->getTag();
	this->addChild(TextPopup::create("삭제하시겠습니까?", this, callfunc_selector(GalleryScene::removeItem), true), 10);
}

void GalleryScene::removeItem()
{
	DatabaseManager::getInstance()->deleteCharacterDB(_removeNo);
	setItems();
}

