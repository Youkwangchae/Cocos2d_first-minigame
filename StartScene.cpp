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

	/*****Ÿ��Ʋ title�� �ֽ��ϴ�.*****/

	//Sprite�� �����Ͽ� �̹����� �ҷ��ɴϴ�.
	auto title = Sprite::create("title_text.png");
	//��Ŀ ����Ʈ�� Point(0.5f, 1)�� ����
	title->setAnchorPoint(Point(0.5f, 1));

	//title�� �ش� ����Ʈ�� ��ġ��ŵ�ϴ�. ȭ���� ���� �߾ӿ� ��ġ�ϵ��� �߽��ϴ�.
	title->setPosition(Point(winSize.width / 2, winSize.height - 30));

	//this�� title�� �ڽ� ���� �߰��Ͽ����ϴ�.
	this->addChild(title);

	/*****ĳ���� character�� �ֽ��ϴ�.*****/

	//Sprite�� �����Ͽ� �̹����� �ҷ��ɴϴ�.
	auto character = Sprite::create("title_character.png");

	//character�� �ش� ����Ʈ�� ��ġ��ŵ�ϴ�. ȭ���� ���߾ӿ� ��ġ�ϵ��� �߽��ϴ�.
	character->setPosition(Point(winSize.width / 2, winSize.height / 2));

	//display FPS�� ����ϴ�.
	director->setDisplayStats(false);

	//this�� character�� �ڽ� ���� �߰��Ͽ����ϴ�.
	this->addChild(character);

	//��ư �߰�
	auto button1 = MenuItemImage::create("title_btn_1.png", "title_btn_1_on.png", "title_btn_1_dis.png", [&](Ref *sender) {
		log("onClickButton1");
		auto Scene = TransitionCrossFade::create(0.5f,CharacterScene::createScene());
		Director::getInstance()->pushScene(Scene);
	});
	
	//button1->setEnabled(false);
	button1->setPosition(Point(winSize.width / 2, 103));

	auto button2 = MenuItemImage::create("title_btn_2.png", "title_btn_2_on.png", CC_CALLBACK_1(StartScene::onClickButton2, this));
	button2->setPosition(Point(winSize.width / 2, 40));

	//������ ��ư�� �޴��� �߰��Ѵ�.

	auto menu = Menu::create(button1, button2, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//Ÿ��Ʋ ��������Ʈ�� �ִϸ��̼� �߰�
	auto to = title->getPosition();
	auto from = Point(to.x, to.y + 100);
	
	//Ÿ��Ʋ ��������Ʈ�� ���� ��ġ�� ����.
	title->setPosition(from);

	//MoveTo�� ������ ��带 �ش� ��ġ�� �����̴� �׼��� �����.
	auto action = MoveTo::create(0.5f, to);
	//Ÿ��Ʋ ��������Ʈ�� �׼� ����.
	title->runAction(action);
	return true;

}

void StartScene::onClickButton2(Ref * Object)
{
	log("onClickButton2");

	if (DatabaseManager::getInstance()->checkCount() < 20) 
	{
		this->addChild(TextPopup::create("����� �̹����� 20���� �ȵ˴ϴ�.\n�̹����� ���� �����ϼ���.", this, NULL, false), 99);
		return;
	}

	auto Scene = TransitionCrossFade::create(0.5f, GameScene::createScene());
	CCDirector::getInstance()->pushScene(Scene);
}
