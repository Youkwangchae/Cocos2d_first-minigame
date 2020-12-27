#include "GameScene.h"
#include "DatabaseManager.h"
#include "GalleryScene.h"
#include "TextPopup.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

cocos2d::Scene * GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	// 1. super init first
	if (!Layer::init())
		return false;

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Pixel Peeker Polka - slower.mp3", true);

	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	//배경
	auto bg = Sprite::create("game_bg.png");
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(bg);

	//타이틀
	auto title = Sprite::create("game_title.png");
	title->setPosition(Point(winSize.width / 2, winSize.height - 20));
	this->addChild(title);

	//돌아가기 버튼
	auto back = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_1(GameScene::onClickBack, this));
	back->setPosition(Point(30, winSize.height - 20));

	auto menu = Menu::create(back, NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);

	//스코어 창
	auto score = Sprite::create("box_scroe.png");
	score->setPosition(Point(250, 410));
	this->addChild(score);
	//스코어 표시 Label
	_labelScore = LabelTTF::create("0", "arial", 20);
	_labelScore->setPosition(Point(score->getContentSize().width / 2, 22));
	
	//score Sprite에 추가해주었다.
	score->addChild(_labelScore);

	_targetBack = NULL;
	setTarget();

	_imagesBack = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, 340);
	_imagesBack->setPosition(Point(0, 0));   //LayerColor는 기본 AnchorPoint가 (0, 0)이다.
	this->addChild(_imagesBack);
	setImages();
	setCountDown();

	//타이머 바를 추가합니다.
	_timerBG = Sprite::create("gage_bg.png");
	_timerBG->setPosition(Point(winSize.width / 2, 365));
	this->addChild(_timerBG);

	auto progress_sprite = Sprite::create("gage_bar.png");
	_progressBar = ProgressTimer::create(progress_sprite);
	_progressBar->setPosition(Point(_timerBG->getContentSize().width / 2, _timerBG->getContentSize().height / 2));

	//프로그레스바가 변하는 형태
	_progressBar->setType(ProgressTimer::Type::BAR);

	//프로그레스바가 변하는 비율, Point(1, 0)의 경우 width만 변한다.
	_progressBar->setBarChangeRate(Point(1, 0));

	//프로그레스바가 변할 중심점 AnchorPoint와 비슷
	_progressBar->setMidpoint(Point(0, 0.5f));
	_progressBar->setPercentage(100);

	_timerBG->addChild(_progressBar);

	_labelCountDown = LabelTTF::create("5.0", "Arial", 20);
	_labelCountDown->setAnchorPoint(Point(0.5f, 0.5f));
	_labelCountDown->setPosition(Point(_timerBG->getContentSize().width / 2, _timerBG->getContentSize().height / 2));
	_timerBG->addChild(_labelCountDown);

	_score = 0;// score 초기화
	_stage = 1;// stage 초기화

	return true;
}

void GameScene::onClickBack(Ref * object)
{
	if (_isCountDown)
		return;
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	log("onClickBack");
	Director::getInstance()->popScene();
}

void GameScene::setTarget()
{
	if (_targetBack)
		_targetBack->removeFromParentAndCleanup(true);

	character *item = DatabaseManager::getInstance()->selectRandomGalleryDB();

	//타겟의 번호를 넣습니다.
	_targetNo = item->no;

	_targetBack = Sprite::create("s_bg_1.png");
	auto head = GalleryScene::getImage("TB_FACE", item->headNo, item->headColorNo);
	auto hair1 = GalleryScene::getImage("TB_HAIR1", item->hair1No, item->hair1ColorNo);
	auto hair2 = GalleryScene::getImage("TB_HAIR2", item->hair2No, item->hair2ColorNo);
	auto eye = GalleryScene::getImage("TB_EYE", item->eyeNo, item->eyeColorNo);
	auto mouth = GalleryScene::getImage("TB_MOUTH", item->mouthNo, item->mouthColorNo);
	auto etc = GalleryScene::getImage("TB_ETC", item->etcNo, item->etcColorNo);
	auto bg = GalleryScene::getImage("TB_BG", item->bgNo, item->bgColorNo);

	//getImage()에서 setPosition이 되어있기에 따로 지정하지 않아도 된다.
	_targetBack->addChild(bg, 0);
	_targetBack->addChild(hair2, 1);
	_targetBack->addChild(head, 2);
	_targetBack->addChild(eye, 3);
	_targetBack->addChild(mouth, 3);
	_targetBack->addChild(hair1, 4);
	_targetBack->addChild(etc, 5);

	auto frame = Sprite::create("box_2.png");
	frame->setPosition(Point(_targetBack->getContentSize().width / 2, _targetBack->getContentSize().height / 2));
	_targetBack->addChild(frame, 6);

	//50px 로 크기를 줄여줌
	float scale = 60 / _targetBack->getContentSize().width;
	_targetBack->setScale(scale);
	_targetBack->setPosition(Point(85, 410));

	this->addChild(_targetBack);
}

void GameScene::setImages()
{
	_imagesBack->removeAllChildrenWithCleanup(true);

	auto galleryList = DatabaseManager::getInstance()->selectGalleryDB(true);

	int size = galleryList.size();



	Vector<MenuItem*> itemArray;

	for (int i = 1; i <= size; i++)
	{
		character *item = galleryList.front();

		auto back = Sprite::create("s_bg_1.png");
		auto head = GalleryScene::getImage("TB_FACE", item->headNo, item->headColorNo);
		auto hair1 = GalleryScene::getImage("TB_HAIR1", item->hair1No, item->hair1ColorNo);
		auto hair2 = GalleryScene::getImage("TB_HAIR2", item->hair2No, item->hair2ColorNo);
		auto eye = GalleryScene::getImage("TB_EYE", item->eyeNo, item->eyeColorNo);
		auto mouth = GalleryScene::getImage("TB_MOUTH", item->mouthNo, item->mouthColorNo);
		auto etc = GalleryScene::getImage("TB_ETC", item->etcNo, item->etcColorNo);
		auto bg = GalleryScene::getImage("TB_BG", item->bgNo, item->bgColorNo);

		//getImage()에서 setPosition이 되어있기에 따로 지정하지 않아도 된다.
		back->addChild(bg, 0);
		back->addChild(hair2, 1);
		back->addChild(head, 2);
		back->addChild(eye, 3);
		back->addChild(mouth, 3);
		back->addChild(hair1, 4);
		back->addChild(etc, 5);

		auto frame = Sprite::create("box_face.png");
		frame->setPosition(Point(back->getContentSize().width / 2, back->getContentSize().height / 2));
		back->addChild(frame, 6);

		auto menuItem = MenuItemSprite::create(back, NULL, CC_CALLBACK_1(GameScene::onClickCard, this));

		//기능을 위해 번호를 넣어준다.
		menuItem->setTag(item->no);

		//55px 로 크기를 줄여줌
		float scale = 55 / back->getContentSize().width;
		menuItem->setScale(scale);

		int row = (i - 1) / 4; //0~3
		int col = (i - 1) % 4; //0~3

		menuItem->setAnchorPoint(Point(0, 1));

		//좌표를 동적할당
		float x = col * menuItem->getContentSize().width * scale + (col + 1) * 20;
		float y = _imagesBack->getContentSize().height - (row * menuItem->getContentSize().height * scale + (row + 1) * 10);

		menuItem->setPosition(Point(x+20, y));
		itemArray.pushBack(menuItem);
		galleryList.pop_front();

		auto blindImg = Sprite::create("box_back.png");
		blindImg->setPosition(Point(menuItem->getContentSize().width / 2, menuItem->getContentSize().height / 2));
		menuItem->addChild(blindImg);

		back->setScaleX(0);

		//MenuItem에 추가되는 sprite는 앵커포인트가 0, 0으로 추가된다.
		back->setAnchorPoint(Point(0.5f, 0.5f));

		//따라서 앵커포인트를 수정하고 위치를 수정해주었다.
		back->setPosition(Point(back->getContentSize().width / 2, back->getContentSize().height / 2));



		//카운트다운 시간인 3초간 지연시키고 가로Scale을 0으로 에니메이션
		blindImg->runAction(Sequence::create(DelayTime::create(3), ScaleTo::create(0.2f, 0, 1), NULL));

		//카운트다운 시간 + 이전 에니메이션 동작시간인 0.2 초를 더해준값만큼 딜레이
		back->runAction(Sequence::create(DelayTime::create(3 + 0.2f), ScaleTo::create(0.2f, 1, 1), NULL));
	}

	auto menu = Menu::createWithArray(itemArray);
	menu->setPosition(Point::ZERO);
	_imagesBack->addChild(menu);
}

void GameScene::setCountDown()
{
	_isCountDown = true;

	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto CountDownLayer = LayerColor::create(Color4B(0, 0, 0, 180), winSize.width, winSize.height);
	this->addChild(CountDownLayer, 10);

	//라벨 생성
	auto count3 = LabelTTF::create("3", "Arial", 60);
	auto count2 = LabelTTF::create("2", "Arial", 60);
	auto count1 = LabelTTF::create("1", "Arial", 60);

	//처음에 투명하게 지정
	count3->setOpacity(0);
	count2->setOpacity(0);
	count1->setOpacity(0);

	count3->setPosition(Point(winSize.width / 2, winSize.height / 2));
	//node의 포지션을 가져올 수 있다.
	count2->setPosition(count3->getPosition());
	count1->setPosition(count3->getPosition());

	CountDownLayer->addChild(count3);
	CountDownLayer->addChild(count2);
	CountDownLayer->addChild(count1);

	auto action = Sequence::create(FadeIn::create(0), FadeOut::create(1), NULL);

	count3->runAction(action);
	count2->runAction(Sequence::create(DelayTime::create(1), action->clone(), NULL));
	count1->runAction(Sequence::create(DelayTime::create(2), action->clone(), NULL));

	CountDownLayer->runAction(Sequence::create(DelayTime::create(3), CallFuncN::create(CC_CALLBACK_1(GameScene::setCountDownEnd, this)), NULL));
}

void GameScene::setCountDownEnd(Ref * object)
{
	_isCountDown = false;
	((Node*)object)->removeFromParentAndCleanup(true);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/flip_sound.wav");
	setTimer();
}

void GameScene::onClickCard(Ref * object)
{
	//카운트 다운중이면 return;
	if (_isCountDown)
		return;

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click_sound.wav");

	auto selectImg = (MenuItemSprite *)object;
	int clickNo = selectImg->getTag();

	unschedule(schedule_selector(GameScene::updateTimer)); //카드를 고르면 타이머를 unscheduled하게 해서 멈추게 합니다.
	_progressBar->stopAllActions();

	if (_targetNo == clickNo)
	{
		//최고 10000점, 시간의 경과에 따라 점수가 낮아진다.
		_score = (_countDownTimer / 5.0f) * 10000;
		//기존의 _score에 현재점수를 더해준다.
		_score = _score + (_countDownTimer / 5.0) * 10000;
		char scoreChar[50];
		sprintf(scoreChar, "%d", _score);
		_labelScore->setString(scoreChar);

		char resultChar[100];
		sprintf(resultChar, "성공하였습니다.\n\n%d점을 획득하였습니다.", _score);
		
		if (_stage < 3) 
		{
			_stage++;
			//게임에 성공했고 아직 스테이지가 남았으므로 nextStage를 호출
			this->addChild(TextPopup::create(resultChar, this, callfunc_selector(GameScene::nextStage), false));
		}

		else 
		{
			//게임에 성공했으나 3스테이지까지 도달했으므로 gameOver를 호출
			this->addChild(TextPopup::create(resultChar, this, callfunc_selector(GameScene::gameOver), false));
		}
	}
	else 
	{
		char resultChar[100];
		sprintf(resultChar, "실패하였습니다.\n\n%d점을 획득하였습니다.", _score);
		this->addChild(TextPopup::create(resultChar, this, callfunc_selector(GameScene::gameOver), false));
	}

	
}

void GameScene::setTimer()
{
	_progressBar->runAction(ProgressFromTo::create(5, 100, 0));
	
	_countDownTimer = 5.0f;
	schedule(schedule_selector(GameScene::updateTimer));
}

void GameScene::updateTimer(float time)
{
	_countDownTimer -= time;

	if (_countDownTimer < 0) 
	{
		_countDownTimer = 0;
		unschedule(schedule_selector(GameScene::updateTimer));
		_labelCountDown->setString("0.0");

		char resultChar[100];
		sprintf(resultChar, "실패하였습니다.\n\n%d점을 획득하였습니다.", _score);
		this->addChild(TextPopup::create(resultChar, this, callfunc_selector(GameScene::gameOver), false));
	}

	log("_countDownTimer : %f", _countDownTimer);

	char str[10] = { 0 };
	sprintf(str, "%2.1f", _countDownTimer);
	_labelCountDown->setString(str);
}

void GameScene::gameOver()
{
	//버튼 선택으로 호출된 함수가 아니며, 받은 Object를 이용하여 처리하는 로직이 없으므로 NULL을 파라메터로 넣었습니다.
	onClickBack(NULL);
}

void GameScene::nextStage()
{
	setTarget();
	setImages();
	setCountDown();

	//타이머 바 초기화
	_progressBar->setPercentage(100);
	_labelCountDown->setString("5.0");
}
