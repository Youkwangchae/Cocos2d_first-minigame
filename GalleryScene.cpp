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
	
		//Device�� ũ�⸦ �����ɴϴ�.
		Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

		//Sprite�� �����Ͽ� �̹����� �ҷ��ɴϴ�.
		auto back = Sprite::create("bg_gallery.png");

		//back�� �ش� ����Ʈ�� ��ġ��ŵ�ϴ�. ȭ���� ���߾ӿ� ��ġ�ϵ��� �߽��ϴ�.
		back->setPosition(Point(winSize.width / 2, winSize.height / 2));

		//this�� back�� �ڽ� ���� �߰��Ͽ����ϴ�.
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
	
		//LayerColor�� �⺻ AnchorPoint�� (0, 0)�̴�.
		galleryLayer->setPosition(Point(0, 48));

		//tag�� 1���������־���.
		this->addChild(galleryLayer, 0, 1);

		setItems();

		return true;	
}

void GalleryScene::onClickHome(Ref * object)
{
	log("onClickHome");
	auto Scene = StartScene::createScene();

	//StartScene���� �̵��ϴµ� ���� Scene�� StartScene���� ���ƿ��°� �ƴ϶� replaceScene�� ���־����ϴ�.
	Director::getInstance()->replaceScene(Scene);
}

void GalleryScene::onClickMake(Ref * object)
{
	log("onClickMake");
	//����ȭ���� ĳ���� ����� ȭ���̹Ƿ� ���� Scene�� Director���� pop�մϴ�.
	Director::getInstance()->popScene();
}

void GalleryScene::setItems()
{
	//galleryLayer�� this�� tag�� 1�� addchild ���־���. getChildByTag�� �ش� ����� tag��ȣ�� �´� �ڽ� ��带 ��ȯ�Ѵ�.
	auto galleryLayer = (LayerColor *)this->getChildByTag(1);

	//removeAllChildrenWithCleanup()�� �ڽĳ�� ���θ� �����Ѵ�.
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

		//getImage()���� setPosition�� �Ǿ��ֱ⿡ ���� �������� �ʾƵ� �ȴ�.
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
		MenuItem->setTag(item->no);        //���� ����� ���� ��ȣ�� �־��ش�.

		//55px �� ũ�⸦ ���̱� ���� scale�� ����
		float scale = 60 / back->getContentSize().width;

		MenuItem->setScale(scale);

		int row = (i - 1) / 4;    //0~3
		int col = (i - 1) % 4;    //0~3
		//row�� col�� ���Ѵ�.

		MenuItem->setAnchorPoint(Point(0, 1));
		//��ǥ�� �����Ҵ�
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
	//�Ű������� ���� headList�� �����´�.
	std::list<head *>headList = DatabaseManager::getInstance()->selectDB(tableName, rowNo);

	//ù��° ����ü ��������
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

	//������ ���� ��ġ�� �������ش�.
	sprite->setPosition(Point(((head->position.x)) *0.2f+6, ((head->position.y)) *0.2f+6));

	return sprite;
}

void GalleryScene::onClickItemCallback(Ref * object)
{
	auto MenuItem = (MenuItemSprite *)object;
	log("tag : %d", MenuItem->getTag()); 
	
	_removeNo = MenuItem->getTag();
	this->addChild(TextPopup::create("�����Ͻðڽ��ϱ�?", this, callfunc_selector(GalleryScene::removeItem), true), 10);
}

void GalleryScene::removeItem()
{
	DatabaseManager::getInstance()->deleteCharacterDB(_removeNo);
	setItems();
}

