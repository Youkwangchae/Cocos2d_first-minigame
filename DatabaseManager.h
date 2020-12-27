#pragma once
#ifndef __DATABASEMANAGER_H__
#define __DATABASEMANAGER_H__

#include "sqlite3.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

struct head
{
public:
	int no;
	char *image;
	bool isColor;

	//Point���� x, y���� ���ϴ�.
	Point position;

	//Color3B���� r, g, b���� ���ϴ�.
	Color3B color1, color2, color3, color4;
};

struct character
{
public:
	int no;
	int headNo;
	int headColorNo;
	int hair1No;
	int hair1ColorNo;
	int hair2No;
	int hair2ColorNo;
	int eyeNo;
	int eyeColorNo;
	int mouthNo;
	int mouthColorNo;
	int etcNo;
	int etcColorNo;
	int bgNo;
	int bgColorNo;
};

class DatabaseManager
{
private:
	bool openDB();
	void closeDB();
	sqlite3 *_sqlite;

public:
	DatabaseManager();
	~DatabaseManager();

	//�����޽����� ���� ����
	char *_errorMSG;

	//����� ���¸� ���� ����
	int _result;

	static DatabaseManager *getInstance();

	void createDB();
	void insertDB();
	void selectDB();

	list<head*> selectDB(string table, int no);

	int insertCharacterDB(character *characterInfo);
	void deleteCharacterDB(int no);

	std::list<character*> selectGalleryDB(bool isRandom);

	int checkCount();

	character* selectRandomGalleryDB();
};

#endif // __DATABASEMANAGER_H__

