#include "CharacterScene.h"
#include "DatabaseManager.h"
#include "DevConf.h"
#include "ColorPopup.h"
#include "TextPopup.h"
#include "GalleryScene.h"

USING_NS_CC;

CharacterScene::CharacterScene()
{
	_face = NULL;
	_hair1 = NULL;
	_hair2 = NULL;
	_eye = NULL;
	_mouth = NULL;
	_etc = NULL;
	_bgStyle = NULL;
	_balloon = NULL;
	_arrow = NULL;
	_scrollView = NULL;

	_characterInfo = new character;
}

CharacterScene::~CharacterScene()
{
	delete(_characterInfo); //생성된 객체를 해제합니다.
}

cocos2d::Scene * CharacterScene::createScene()
{
	auto scene = Scene::create();

	auto layer = CharacterScene::create();

	scene->addChild(layer);

	return scene;
}

bool CharacterScene::init()
{
	if(!Layer::init())
		return false;

	auto UserDefault = UserDefault::getInstance();//UserDefault는 하나의 키값에 따른 결과값을 저장합니다.
	bool isFirst = UserDefault->getBoolForKey("isFirst", true); //isFisrt값을 가져옵니다.

	if (isFirst || DB_INIT) 
	{
		DatabaseManager::getInstance()->createDB();
		DatabaseManager::getInstance()->insertDB();

		UserDefault->setBoolForKey("isFirst", false);//isFisrt값을 변경합니다.
		UserDefault->flush();
	}

	DatabaseManager::getInstance()->createDB();
	DatabaseManager::getInstance()->insertDB();
	DatabaseManager::getInstance()->selectDB();

	/*****Device의 크기를 가져옵니다.*****/

	//Director를 가져옵니다.
	auto director = Director::getInstance();

	//OpenGLView를 가져옵니다.
	auto glView = director->getOpenGLView();

	//OpenGLView에서 DesignResolutionSize를 가져옵니다.
	auto winSize = glView->getDesignResolutionSize();
	/*****Device의 크기를 가져옵니다.*****/


	//배경 추가
	auto back = Sprite::create("bg_main.png");
	back->setPosition(Point(winSize.width / 2, winSize.height/2));
	this->addChild(back);

	//캐릭터 박스 추가
	_characterBg = Sprite::create("bg_1.png");

	//Node의 기준을 상단중앙으로 변경함. setAnchorPoint의 값은 0~1. 1이 100%입니다.
	_characterBg->setAnchorPoint(Point(0.525, 1));

	//AnchorPoint를 상단의 중앙으로 변경하여 상단에서 15px만큼 아래로 내렸다.
	_characterBg->setPosition(Point(winSize.width*0.525, winSize.height+45));

	//back에 자식노드로 추가함
	back->addChild(_characterBg);

	//캐릭터 UI 선택 버튼 추가
	auto menu1Face = MenuItemImage::create("btn_face.png", "btn_face_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Face->setPosition(Point(50, 125));

	auto menu1Hair1 = MenuItemImage::create("btn_hair1.png", "btn_hair1_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Hair1->setPosition(Point(100, 125));

	auto menu1Hair2 = MenuItemImage::create("btn_hair2.png", "btn_hair2_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Hair2->setPosition(Point(150, 125));

	auto menu1Eye = MenuItemImage::create("btn_eye.png", "btn_eye_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Eye->setPosition(Point(200, 125));

	auto menu1Mouth = MenuItemImage::create("btn_mouth.png", "btn_mouth_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Mouth->setPosition(Point(250, 125));

	auto menu1Etc = MenuItemImage::create("btn_etc.png", "btn_etc_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Etc->setPosition(Point(300, 125));

	auto menu1Bg = MenuItemImage::create("btn_bg.png", "btn_bg_on.png", CC_CALLBACK_1(CharacterScene::onClickMenu, this));
	menu1Bg->setPosition(Point(350, 125));

	auto menu1 = Menu::create(menu1Face, menu1Hair1, menu1Hair2, menu1Eye, menu1Mouth, menu1Etc, menu1Bg, NULL);

	menu1->setPosition(Point::ZERO);

	this->addChild(menu1);

	//각 MenuItemImage에 태그 지정
	menu1Face->setTag(1);
	menu1Hair1->setTag(2);
	menu1Hair2->setTag(3);
	menu1Eye->setTag(4);
	menu1Mouth->setTag(5);
	menu1Etc->setTag(6);
	menu1Bg->setTag(7);

	//하단 메뉴 버튼의 좌표와 가로 세로크기 변수들
	float x, y, width, height;

	//하단 메뉴 버튼 추가
	auto menu2Home = MenuItemImage::create("menu_home.png", "menu_home_on.png", CC_CALLBACK_1(CharacterScene::onClickHome, this));
	width = menu2Home->getContentSize().width;
	height = menu2Home->getContentSize().height;
	x = width / 2;
	y = height / 2;
	menu2Home->setPosition(Point(x, y));

	auto menu2Random = MenuItemImage::create("menu_random.png", "menu_random_on.png", CC_CALLBACK_1(CharacterScene::onClickRandom, this));
	x = menu2Home->getPositionX() + width;
	menu2Random->setPosition(Point(x, y));

	auto menu2Save = MenuItemImage::create("menu_save.png", "menu_save_on.png", CC_CALLBACK_1(CharacterScene::onClickSave, this));
	x = menu2Random->getPositionX() + width;
	menu2Save->setPosition(Point(x, y));

	auto menu2Gallery = MenuItemImage::create("menu_gallary.png", "menu_gallary_on.png", CC_CALLBACK_1(CharacterScene::onClickGallery, this));
	x = menu2Save->getPositionX() + width;
	menu2Gallery->setPosition(Point(x, y));

	auto menu2 = Menu::create(menu2Home, menu2Random, menu2Save, menu2Gallery, NULL);
	menu2->setPosition(Point::ZERO);
	this->addChild(menu2);

	setImage("TB_FACE", -1, -1);
	setImage("TB_HAIR1", -1, -1);
	setImage("TB_HAIR2", -1, -1);
	setImage("TB_EYE", -1, -1);
	setImage("TB_MOUTH", -1, -1);
	setImage("TB_ETC", -1, -1);
	setImage("TB_BG", -1, -1);

	setBalloon(1); //1번이 기본으로 선택되도록 합니다.
	return true;
}

void CharacterScene::onClickMenu(Ref * object)
{
	log("onClickMenu");
	setBalloon(((Node*)object)->getTag()); //sender의 객체를 Node로 캐스팅하여 태그를 가져옵니다.
}

void CharacterScene::onClickHome(Ref * object)
{
	log("onClickHome");
	Director::getInstance()->popScene(); //Director에서 현재 Scene을 없앱니다. =  이전 Scene으로 돌아갑니다.
}

void CharacterScene::onClickRandom(Ref * object)
{
	log("onClickRandom");
	setImage("TB_FACE", -1, -1);
	setImage("TB_HAIR1", -1, -1);
	setImage("TB_HAIR2", -1, -1);
	setImage("TB_EYE", -1, -1);
	setImage("TB_MOUTH", -1, -1);
	setImage("TB_ETC", -1, -1);
	setImage("TB_BG", -1, -1);
}

void CharacterScene::onClickSave(Ref * object)
{
	log("onClickSave");
	int result = DatabaseManager::getInstance()->insertCharacterDB(_characterInfo);

	DatabaseManager::getInstance()->insertCharacterDB(_characterInfo);
	
	if (result == 1) //20개까지만 저장 가능합니다.
		this->addChild(TextPopup::create("저장이 완료되었습니다.", this, NULL, false), 10);
	else if (result == 2)
		this->addChild(TextPopup::create("저장이 실패하였습니다.\n저장 가능한 개수를 초과하였습니다.", this, NULL, false), 10);
	else if (result == 3)
		this->addChild(TextPopup::create("저장이 실패하였습니다.\n같은 이미지가 존재합니다.", this, NULL, false), 10);
	else
		this->addChild(TextPopup::create("저장이 실패하였습니다.", this, NULL, false), 10);
}

void CharacterScene::onClickGallery(Ref * object)
{
	log("onClickGallery");
	auto Scene = GalleryScene::createScene();
	Director::getInstance()->pushScene(Scene);
}

void CharacterScene::setImage(std::string tableName, int rowNo, int colorNo)
{
	auto headList = DatabaseManager::getInstance()->selectDB(tableName, rowNo);

	auto head = headList.front();//가져온 headList에서 첫번째 구조체를 가져옵니다.

	int zOrder = 0;
	if (tableName == "TB_FACE")
	{
		//_face가 NULL이 아니면
		if (_face != NULL) 
		{
			//부모로부터 자기를 없앱니다.
			_face->removeFromParentAndCleanup(true);
		}

		zOrder = 2;
		//데이터베이스에서 이미지의 파일명과 해당 파일의 좌표를 가져와 위치시킨다.
		_face = Sprite::create(head->image);
		_face->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_face, zOrder);
		_characterInfo->headNo = head->no;
	}
	else if (tableName == "TB_HAIR1") 
	{
		if (_hair1 != NULL) 
			_hair1->removeFromParentAndCleanup(true);
		
		zOrder = 4;
		_hair1 = Sprite::create(head->image);
		_hair1->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_hair1, zOrder);
		_characterInfo->hair1No = head->no;
	}

	else if (tableName == "TB_HAIR2") 
	{
		if (_hair2 != NULL)
			_hair2->removeFromParentAndCleanup(true);

		zOrder = 1;
		_hair2 = Sprite::create(head->image);
		_hair2->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_hair2, zOrder);
		_characterInfo->hair2No = head->no;
	}

	else if (tableName == "TB_EYE") 
	{
		if (_eye != NULL)
			_eye->removeFromParentAndCleanup(true);

		zOrder = 3;
		_eye = Sprite::create(head->image);
		_eye->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_eye, zOrder);
		_characterInfo->eyeNo = head->no;
	}

	else if (tableName == "TB_MOUTH") 
	{
		if (_mouth != NULL) 
			_mouth->removeFromParentAndCleanup(true);

		zOrder = 3;
		_mouth = Sprite::create(head->image);
		_mouth->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_mouth, zOrder);
		_characterInfo->mouthNo = head->no;
	}

	else if (tableName == "TB_ETC")
	{
		if (_etc != NULL) 
			_etc->removeFromParentAndCleanup(true);
	
		zOrder = 5;
		_etc = Sprite::create(head->image);
		_etc->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_etc, zOrder);
		_characterInfo->etcNo = head->no;
	}

	else if (tableName == "TB_BG") 
	{
		if (_bgStyle != NULL) 
			_bgStyle->removeFromParentAndCleanup(true);

		zOrder = 0;
		_bgStyle = Sprite::create(head->image);
		_bgStyle->setPosition((head->position) + Point(37,37));
		_characterBg->addChild(_bgStyle, zOrder);
		_characterInfo->bgNo = head->no;
	}

	if (head->isColor)
	{
		Color3B color;
		if (colorNo < 0)
		{
			srand(time(NULL));
			colorNo = rand() % 4 + 1;
		}

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

		if (tableName == "TB_FACE")
		{
			_face->setColor(color);
			_characterInfo->headColorNo = colorNo;
		}
		else if (tableName == "TB_HAIR1") 
		{
			_hair1->setColor(color);
			_characterInfo->hair1ColorNo = colorNo;
		}
		else if (tableName == "TB_HAIR2") 
		{
			_hair2->setColor(color);
			_characterInfo->hair2ColorNo = colorNo;
		}
		else if (tableName == "TB_EYE") 
		{
			_eye->setColor(color);
			_characterInfo->eyeColorNo = colorNo;
		}
		else if (tableName == "TB_MOUTH") 
		{
			_mouth->setColor(color);
			_characterInfo->mouthColorNo = colorNo;
		}
		else if (tableName == "TB_ETC") 
		{
			_etc->setColor(color);
			_characterInfo->etcColorNo = colorNo;
		}
		else if (tableName == "TB_BG") 
		{
			_bgStyle->setColor(color);
			_characterInfo->bgColorNo = colorNo;
		}
	}
}

void CharacterScene::setBalloon(int position)
{
	if (_arrow ==  NULL)
	{
		//Device의 크기를 가져옵니다.
		auto winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
		_balloon = Sprite::create("box_list.png");
		_balloon->setPosition(Point(winSize.width / 2, 75));

		this->addChild(_balloon, 1);

		_arrow = Sprite::create("box_list2.png");

		//말풍선의 상단에 딱 붙이기 위해 화살표의 하단 가운데를 기준으로 설정해 주었습니다.
		_arrow->setAnchorPoint(Point(0.525, 0));

		_balloon->addChild(_arrow);
	}

	Point arrowPoint;
	switch (position)
	{
	case 1:
		arrowPoint = Point(50, _balloon->getContentSize().height);
		break;
	case 2:
		arrowPoint = Point(100, _balloon->getContentSize().height);
		break;
	case 3:
		arrowPoint = Point(150, _balloon->getContentSize().height);
		break;
	case 4:
		arrowPoint = Point(200, _balloon->getContentSize().height);
		break;
	case 5:
		arrowPoint = Point(250, _balloon->getContentSize().height);
		break;
	case 6:
		arrowPoint = Point(300, _balloon->getContentSize().height);
		break;
	case 7:
		arrowPoint = Point(350, _balloon->getContentSize().height);
		break;
	}

	_arrow->setPosition(arrowPoint);

	if (_scrollView != NULL)
		_scrollView->removeFromParentAndCleanup(true);

	//LayerColor는 Layer와 동일하나 색상을 지정할 수 있습니다.
	auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 500, _balloon->getContentSize().height);

	//첫번째 매개변수는 보여질 크기, 두번째 매개변수는 container입니다.
	_scrollView = ScrollView::create(_balloon->getContentSize(), layer);

	//ScrollView::Direction::HORIZONTAL : 가로 스크롤, ScrollView::Direction::VERTICAL : 세로 스크롤, ScrollView::Direction::BOTH : 둘다 스크롤
	_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);

	_balloon->addChild(_scrollView);

	setSubMenuItem(position);
}


void CharacterScene::showColorPopup(Ref * object)
{
	auto node = ((Node*)object);
	log("tableName : %s, tag : %d", _currentTableName.c_str(), node->getTag());

	auto headList = DatabaseManager::getInstance()->selectDB(_currentTableName, node->getTag());
	auto head = headList.front();

	if (!head->isColor) 
	{
		//색상 선택이 불가능한 파트는 색상은 아무거나 넣어도 되므로 아무숫자나 넣었습니다.
		setImage(_currentTableName, head->no, -1);
		return;
	}

	ColorPopup* popup = ColorPopup::create(_currentTableName, node->getTag());
	//다른노드들 보다 zOrder를 위로 주어 최상단에 나오도록 합니다.
	this->addChild(popup, 10);
}

void CharacterScene::setSubMenuItem(int position)
{
	std::string table;

	switch (position)
	{
	case 1:
		table = "TB_FACE";
		break;
	case 2:
		table = "TB_HAIR1";
		break;
	case 3:
		table = "TB_HAIR2";
		break;
	case 4:
		table = "TB_EYE";
		break;
	case 5:
		table = "TB_MOUTH";
		break;
	case 6:
		table = "TB_ETC";
		break;
	case 7:
		table = "TB_BG";
		break;
	}

	_currentTableName = table;

	//이전에 데이터 베이스 구축당시 No를 0으로 넣으면 해당 테이블 전체를 가져오도록 구현했습니다.
	auto headList = DatabaseManager::getInstance()->selectDB(table, 0);

	//메뉴아이템을 동적으로 생성하기위해 Array를 사용하였다.
	Vector<MenuItem*> itemArray;

	//for구문에 headList.size()를 조건문에 넣으면 pop_front를 하면서 size()가 줄어들어 원하는 값보다 적게 반복문을 실행합니다. 따라서 미리 크기를 가져와 변수로 사용합니다.
	int listCnt = headList.size();

	float iconWidth = 0;
	for (int i = 0; i < listCnt; i++) 
	{
		//첫번째 구조체 가져오기
		auto head = headList.front();
		char icon[50];

		//아이콘은 i_파일명으로 이름을 정해놓았습니다. 이런식으로 사용하면 따로 아이콘 파일명을 가질 필요없이 사용이 가능합니다.
		sprintf(icon, "i_%s", head->image);
		auto item_img = Sprite::create(icon);

		if (iconWidth <= 0) 
		{
			//icon width 크기
			iconWidth = item_img->getContentSize().width;
		}

		auto item = MenuItemSprite::create(item_img, NULL, CC_CALLBACK_1(CharacterScene::showColorPopup, this));
		
		//번호를 tag로 지정했습니다.
		item->setTag(head->no);
		item->setAnchorPoint(Point(0, 0.5));

		//아이템의 가로크기 x i 만큼 + 5(padding) x i 만큼에 위치시켰습니다.
		item->setPosition(Point(i * item_img->getContentSize().width + 5 * i, _scrollView->getContentSize().height / 2));
		itemArray.pushBack(item);
		headList.pop_front();
	}

	auto menu = Menu::createWithArray(itemArray);
	menu->setPosition(Point::ZERO);
	//_scrollView->getContainer()->setLocalZOrder(-1);
	
	auto ContainerSize = Size(listCnt*iconWidth + 5 * (listCnt - 1), _scrollView->getContainer()->getContentSize().height);
	_scrollView->getContainer()->setContentSize(ContainerSize);
	_scrollView->getContainer()->addChild(menu);
}

