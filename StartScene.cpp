#include "SimpleAudioEngine.h"
#include "CharacterScene.h"
#include "StartScene.h"
#include "GameScene.h"
#include "DatabaseManager.h"
#include "TextPopup.h"

USING_NS_CC;

Scene* StartScene::createScene()

{

	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);


	// return the scene
	return scene;

}



// on "init" you need to initialize your instance

bool StartScene::init()

{
	// 1. super init first
	if (!Scene::init())
		return false;
	

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto winSize = glview->getDesignResolutionSize();
	auto back = Sprite::create("title_bg.png");

	back->setPosition(Point(winSize.width / 2, winSize.height / 2));

	this->addChild(back);

	/*****타이틀 title을 넣습니다.*****/

	//Sprite를 생성하여 이미지를 불러옵니다.
	auto title = Sprite::create("title_text.png");
	//앵커 포인트를 Point(0.5f, 1)로 변경
	title->setAnchorPoint(Point(0.5f, 1));

	//title을 해당 포인트에 위치시킵니다. 화면의 가로 중앙에 위치하도록 했습니다.
	title->setPosition(Point(winSize.width / 2, winSize.height - 30));

	//this에 title을 자식 노드로 추가하였습니다.
	this->addChild(title);

	/*****캐릭터 character을 넣습니다.*****/

	//Sprite를 생성하여 이미지를 불러옵니다.
	auto character = Sprite::create("title_character.png");

	//character를 해당 포인트에 위치시킵니다. 화면의 정중앙에 위치하도록 했습니다.
	character->setPosition(Point(winSize.width / 2, winSize.height / 2));

	//display FPS를 지웁니다.
	director->setDisplayStats(false);

	//this에 character를 자식 노드로 추가하였습니다.
	this->addChild(character);

	//버튼 추가
	auto button1 = MenuItemImage::create("title_btn_1.png", "title_btn_1_on.png", "title_btn_1_dis.png", [&](Ref *sender) {
		log("onClickButton1");
		auto Scene = TransitionCrossFade::create(0.5f,CharacterScene::createScene());
		Director::getInstance()->pushScene(Scene);
	});
	
	//button1->setEnabled(false);
	button1->setPosition(Point(winSize.width / 2, 103));

	auto button2 = MenuItemImage::create("title_btn_2.png", "title_btn_2_on.png", CC_CALLBACK_1(StartScene::onClickButton2, this));
	button2->setPosition(Point(winSize.width / 2, 40));

	//생성된 버튼을 메뉴에 추가한다.

	auto menu = Menu::create(button1, button2, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//타이틀 스프라이트에 애니메이션 추가
	auto to = title->getPosition();
	auto from = Point(to.x, to.y + 100);
	
	//타이틀 스프라이트에 시작 위치를 지정.
	title->setPosition(from);

	//MoveTo로 지정된 노드를 해당 위치로 움직이는 액션을 만든다.
	auto action = MoveTo::create(0.5f, to);
	//타이틀 스프라이트에 액션 지정.
	title->runAction(action);
	return true;

}

void StartScene::onClickButton2(Ref * Object)
{
	log("onClickButton2");

	if (DatabaseManager::getInstance()->checkCount() < 20) 
	{
		this->addChild(TextPopup::create("저장된 이미지가 20개가 안됩니다.\n이미지를 먼저 생성하세요.", this, NULL, false), 99);
		return;
	}

	auto Scene = TransitionCrossFade::create(0.5f, GameScene::createScene());
	CCDirector::getInstance()->pushScene(Scene);
}
