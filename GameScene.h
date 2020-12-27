#pragma once
#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	cocos2d::LabelTTF * _labelScore;
	void onClickBack(Ref *object);

	cocos2d::Sprite* _targetBack;
	void setTarget();

	cocos2d::LayerColor* _imagesBack;
	void setImages();

	//ī��Ʈ �ٿ����̸� ��ġ�� ����ϱ� ���� ����
	bool _isCountDown;
	void setCountDown();

	//ī��Ʈ �ٿ��� ������ callback
	void setCountDownEnd(Ref *object);

	int _targetNo;
	void onClickCard(Ref *object);

	cocos2d::Sprite *_timerBG;
	cocos2d::ProgressTimer *_progressBar;
	void setTimer();

	cocos2d::LabelTTF *_labelCountDown;
	float _countDownTimer;
	void updateTimer(float time);

	int _score;

	void gameOver();

	int _stage;
	void nextStage();
};

