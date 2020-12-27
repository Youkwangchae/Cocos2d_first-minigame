#include "DatabaseManager.h"
#include <string>
#include "DevConf.h"

#define DB_FILE_NAME "db.sqlite"

static DatabaseManager *_sharedSqlite = NULL;

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
	//_sharedSqlite�� ������
	if (_sharedSqlite != NULL) {
		_sharedSqlite->closeDB();
		_sharedSqlite = NULL;
	}
}

//DatabaseManager ��ü�� ��ȯ�Ѵ�.
DatabaseManager* DatabaseManager::getInstance() {
	//_sharedSqlite�� ������
	if (!_sharedSqlite) {
		_sharedSqlite = new DatabaseManager;
		_sharedSqlite->openDB();
	}

	return _sharedSqlite;
}

//�����ͺ��̽��� �����Ѵ�.

bool DatabaseManager::openDB() {

	//�����ͺ��̽��� path�� �����´�.
	std::string path = FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME;

	//�����ͺ��̽��� path�� ����Ѵ�.
	log("path : %s", path.c_str());

	//�����ͺ��̽��� open�Ѵ�.
	_result = sqlite3_open(path.c_str(), &_sqlite);

	if (_result != SQLITE_OK) {
		log("failed to create db");
		return false;
	}
	return true;

}


//�����ͺ��̽��� �ݴ´�.
void DatabaseManager::closeDB() {
	sqlite3_close(_sqlite);
}


//���̺� ����
void DatabaseManager::createDB() {
	if (DB_INIT)//DB �ʱ�ȭ�� ���� ������ ������.
	{
		std::string path = FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME;
		closeDB(); //DB�� ������ ���������� ������ �ȵǹǷ� DB�� �ݾ��ش�.

		remove(path.c_str()); //DB�� ������ ����.

		openDB(); //DB�� �ݾ����� �ٽ� �����ش�.
	}

	for (int i = 0; i < 7; i++)
	{
		std::string query;
		switch (i)
		{
		case 0:
			//TB_FACE ���̺�
			query = "create table if not exists TB_FACE(";
			break;

		case 1:
			//TB_HAIR1 ���̺�
			query = "create table if not exists TB_HAIR1(";
			break;

		case 2:
			//TB_HAIR2 ���̺�
			query = "create table if not exists TB_HAIR2(";
			break;

		case 3:
			//TB_EYE ���̺�
			query = "create table if not exists TB_EYE(";
			break;

		case 4:
			//TB_MOUTH ���̺�
			query = "create table if not exists TB_MOUTH(";
			break;

		case 5:
			//TB_ETC ���̺�
			query = "create table if not exists TB_ETC(";
			break;

		case 6:
			//TB_BG ���̺�
			query = "create table if not exists TB_BG(";
			break;
		}

		query += "NO integer primary key autoincrement, ";

		//�̹��� ���ϸ�
		query += "IMG varChar(100), ";

		//�ش� �̹����� x��ġ
		query += "X double default 0, ";

		//�ش� �̹����� y��ġ
		query += "Y double default 0, ";

		//�� �̹������� ��︮�� �� 4������ ����Ʈ�� �����ȴ�. ���� RGB���� color�� ���� ����������
		query += "COLOR1_R interger default 0, ";
		query += "COLOR1_G interger default 0, ";
		query += "COLOR1_B interger default 0, ";

		query += "COLOR2_R interger default 0, ";
		query += "COLOR2_G interger default 0, ";
		query += "COLOR2_B interger default 0, ";

		query += "COLOR3_R interger default 0, ";
		query += "COLOR3_G interger default 0, ";
		query += "COLOR3_B interger default 0, ";

		query += "COLOR4_R interger default 0, ";
		query += "COLOR4_G interger default 0, ";
		query += "COLOR4_B interger default 0";

		query += ")";

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("createDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}

	{
		//ĳ���� ���� ���̺�
		string query = "create table if not exists TB_USER_CHARACTER(";
		query += "NO integer primary key autoincrement, ";

		//���� ��ȣ
		query += "HEAD_NO interger not null, ";

		//�� �����ȣ
		query += "HEAD_COLOR_NO interger not null, ";
		query += "HAIR1_NO interger not null, ";
		query += "HAIR1_COLOR_NO interger not null, ";
		query += "HAIR2_NO interger not null, ";
		query += "HAIR2_COLOR_NO interger not null, ";
		query += "EYE_NO interger not null, ";
		query += "EYE_COLOR_NO interger not null, ";
		query += "MOUTH_NO interger not null, ";
		query += "MOUTH_COLOR_NO interger not null, ";
		query += "ETC_NO interger not null, ";
		query += "ETC_COLOR_NO interger not null, ";
		query += "BG_NO interger not null, ";
		query += "BG_COLOR_NO interger not null";
		query += ")";

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("createDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}
}

void DatabaseManager::insertDB()
{
	{
		//�� 10�� row �߰�
		std::string query = "insert into TB_FACE(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";

		query += "values (1, 'face1.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(2, 'face2.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(3, 'face3.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(4, 'face4.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(5, 'face5.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(6, 'face6.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(7, 'face7.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(8, 'face8.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(9, 'face9.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219), ";
		query += "(10, 'face10.png', 156, 173, 254, 225, 194, 237, 188, 137, 184, 137, 105, 253, 242, 219)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);
		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");

		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}

	{

		//hair1 10�� row �߰�

		std::string query = "insert into TB_HAIR1(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";

		query += "values (1, 'hair_1.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(2, 'hair_2.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(3, 'hair_3.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(4, 'hair_4.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(5, 'hair_5.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(6, 'hair_6.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(7, 'hair_7.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(8, 'hair_8.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(9, 'hair_9.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(10, 'hair_10.png', 156, 151.5, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);

	}

	{

		//hair2 7�� row �߰�

		std::string query = "insert into TB_HAIR2(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";
		query += "values (1, 'hair_11.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(2, 'hair_12.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(3, 'hair_13.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(4, 'hair_14.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(5, 'hair_15.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(6, 'hair_16.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34), ";
		query += "(7, 'hair_17.png', 156, 157, 98, 59, 22, 170, 24, 13, 236, 183, 81, 62, 45, 34)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����
		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");

		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);

	}

	{

		//eye 10�� row �߰�

		std::string query = "insert into TB_EYE(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";
		query += "values (1, 'eye_1.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(2, 'eye_2.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(3, 'eye_3.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(4, 'eye_4.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(5, 'eye_5.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(6, 'eye_6.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(7, 'eye_7.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(8, 'eye_8.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(9, 'eye_9.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(10, 'eye_10.png', 156, 166.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);

	}

	{
		//mouth 10�� row �߰�
		std::string query = "insert into TB_MOUTH(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";
		query += "values (1, 'm_1.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(2, 'm_2.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(3, 'm_3.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(4, 'm_4.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(5, 'm_5.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(6, 'm_6.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(7, 'm_7.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(8, 'm_8.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(9, 'm_9.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(10, 'm_10.png', 156, 101.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����

		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");

		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}

	{
		//etc 10�� row �߰�
		std::string query = "insert into TB_ETC(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";
		query += "values (1, 'etc_1.png', 156, 259, 234, 104, 119, 126, 206, 244, 189, 243, 57, 146, 7, 131), ";
		query += "(2, 'etc_2.png', 198, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(3, 'etc_3.png', 156.5, 156.5, 234, 104, 119, 126, 206, 244, 189, 243, 57, 146, 7, 131), ";
		query += "(4, 'etc_4.png', 156.5, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(5, 'etc_5.png', 156, 272.5, 234, 104, 119, 126, 206, 244, 189, 243, 57, 146, 7, 131), ";
		query += "(6, 'etc_6.png', 156, 125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(7, 'etc_7.png', 156, 270, 234, 104, 119, 126, 206, 244, 189, 243, 57, 146, 7, 131), ";
		query += "(8, 'etc_8.png', 156, 267, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(9, 'etc_9.png', 156, 125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ";
		query += "(10, 'etc_10.png', 156, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)";
		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}

	{
		//bg 4�� row �߰�
		std::string query = "insert into TB_BG(NO, IMG, X, Y, COLOR1_R, COLOR1_G, COLOR1_B, COLOR2_R, COLOR2_G, COLOR2_B, COLOR3_R, COLOR3_G, COLOR3_B, COLOR4_R, COLOR4_G, COLOR4_B) ";
		query += "values (1, 'bg_1.png', 156, 155.5, 250, 237, 125, 171, 242, 0, 0, 216, 255, 243, 97, 166), ";
		query += "(2, 'bg_2.png', 156, 155.5, 250, 237, 125, 171, 242, 0, 0, 216, 255, 243, 97, 166), ";
		query += "(3, 'bg_3.png', 156, 155.5, 250, 237, 125, 171, 242, 0, 0, 216, 255, 243, 97, 166), ";
		query += "(4, 'bg_4.png', 156, 155.5, 250, 237, 125, 171, 242, 0, 0, 216, 255, 243, 97, 166)";
		//��ȣ�� ���ϸ��� ������ ��� �Ӽ��� ����
		
		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);
		if (_result == SQLITE_OK)
			log("insertDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}
/*
	std::string query = "insert into TB_FACE(N0) values (1), (2), (3)";
	
	_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

	if (_result == SQLITE_OK)
		log("insertDB() SUCCESS");
	else
		log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);*/
}

void DatabaseManager::selectDB()
{
	std::string query = "select * from TB_FACE";

	sqlite3_stmt *stmt = NULL;

	_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &stmt, NULL);

	if (_result == SQLITE_OK)
	{
		log("selectDB() SUCCESS");
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int no = sqlite3_column_int(stmt, 0);
			log("no, %d", no);
		}
	}
	else
		log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);

	sqlite3_finalize(stmt);
}

list<head*> DatabaseManager::selectDB(string table, int no)
{
	string query;

	if (no > 0)
	{	//no�� 0���� ũ�� �ش� ��ȣ�� �����ɴϴ�.
		char temp[10];
		
		//temp�� no�� ����ϴ�.
		sprintf(temp, "%d", no);//sprintf���� ù��°�� ������, �ι�°�� ���� ����, ����°�� ���� ����.
		query = "select * from " + table + " where NO = " + temp;
	}
	else if (no < 0) 
	{
		//no�� 0���� ������ �����ϰ� �ϳ��� ����մϴ�. (������ ���� ���)
		query = "select * from " + table + " order by random() limit 1";
	}
	else
	{
		//no�� 0�̸� ��ü�� �����ɴϴ�.
		query = "select * from " + table;
	}

	sqlite3_stmt *stmt = NULL;
	_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &stmt, NULL); //stmt�� query ����� ����ϴ�.
	list <head*> headList;

	if (_result == SQLITE_OK)
	{
		log("selectDB() SUCCESS");
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			head *pHead = new head; //���� ������ �����Դϴ�.
			//sqlite3_column_xxx�� ù��° �Ű������� sqlite3_stmt�� �ް� �ι�° �Ű������� �÷���ȣ�� ���� �����´�.
			pHead->no = sqlite3_column_int(stmt, 0);
			pHead->image = strdup((char *)sqlite3_column_text(stmt, 1));
			pHead->position = Point(sqlite3_column_double(stmt, 2), sqlite3_column_double(stmt, 3));
			pHead->color1 = Color3B(sqlite3_column_int(stmt, 4), sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6));
			pHead->color2 = Color3B(sqlite3_column_int(stmt, 7), sqlite3_column_int(stmt, 8), sqlite3_column_int(stmt, 9));
			pHead->color3 = Color3B(sqlite3_column_int(stmt, 10), sqlite3_column_int(stmt, 11), sqlite3_column_int(stmt, 12));
			pHead->color4 = Color3B(sqlite3_column_int(stmt, 13), sqlite3_column_int(stmt, 14), sqlite3_column_int(stmt, 15));

			pHead->isColor = true; //true�̸� ���� ���� �����ϵ��� �մϴ�.

			if (table == "TB_EYE")
				pHead->isColor = false; //eye�� ���󺯰� �Ұ�

			else if (table == "TB_MOUTH")
				pHead->isColor = false;	//mouth�� ���󺯰� �Ұ�


			else if (table == "TB_ETC") 
			{
				//ETC�� 2, 4, 6, 8, 9, 10 ���� ���� ����Ұ�
				if (pHead->no == 2 || pHead->no == 4 || pHead->no == 6 || pHead->no == 8 || pHead->no == 9 || pHead->no == 10)
					pHead->isColor = false;
			}
			headList.push_back(pHead);
		}
	}
	else
		log("ERROR CODE : %d", _result);

	sqlite3_finalize(stmt);

	return headList;
}

int DatabaseManager::insertCharacterDB(character * characterInfo)
{
	{
		//���� üũ
		sqlite3_stmt *pStmt = NULL;
		string query = "select count(*) from TB_USER_CHARACTER";
		_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &pStmt, NULL);

		if (_result == SQLITE_OK)
		{
			log("selectDB() SUCCESS");
			if (sqlite3_step(pStmt) == SQLITE_ROW)
			{
				int cnt = sqlite3_column_int(pStmt, 0);
				if (cnt >= 20) 
				{
					sqlite3_finalize(pStmt);
					return 2;
				}
			}
		}
		sqlite3_finalize(pStmt);
	}

	{
		//���� �̹��� üũ
		sqlite3_stmt *pStmt = NULL;
		string query = "select count(*) from TB_USER_CHARACTER where ";

		char temp[500];
		sprintf(temp, "HEAD_NO = %d AND HEAD_COLOR_NO = %d AND HAIR1_NO = %d AND HAIR1_COLOR_NO = %d AND HAIR2_NO = %d AND HAIR2_COLOR_NO = %d AND EYE_NO = %d AND EYE_COLOR_NO = %d AND MOUTH_NO = %d AND MOUTH_COLOR_NO = %d AND ETC_NO = %d AND ETC_COLOR_NO = %d AND BG_NO = %d AND BG_COLOR_NO = %d", characterInfo->headNo, characterInfo->headColorNo, characterInfo->hair1No, characterInfo->hair1ColorNo, characterInfo->hair2No, characterInfo->hair2ColorNo, characterInfo->eyeNo, characterInfo->eyeColorNo, characterInfo->mouthNo, characterInfo->mouthColorNo, characterInfo->etcNo, characterInfo->etcColorNo, characterInfo->bgNo, characterInfo->bgColorNo);
		query += temp;
		_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &pStmt, NULL);

		if (_result == SQLITE_OK)
		{
			log("selectDB() SUCCESS");

			if (sqlite3_step(pStmt) == SQLITE_ROW)
			{
				int cnt = sqlite3_column_int(pStmt, 0);
				if (cnt >= 1)
				{
					sqlite3_finalize(pStmt);
					return 3;
				}
			}
		}
		sqlite3_finalize(pStmt);
	}

	{
		//ĳ���� �ϳ� �߰�
		string query = "insert into TB_USER_CHARACTER(HEAD_NO, HEAD_COLOR_NO, HAIR1_NO, HAIR1_COLOR_NO, HAIR2_NO, HAIR2_COLOR_NO, EYE_NO, EYE_COLOR_NO, MOUTH_NO, MOUTH_COLOR_NO, ETC_NO, ETC_COLOR_NO, BG_NO, BG_COLOR_NO) ";
	
		char temp[255];
		sprintf(temp, "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", characterInfo->headNo, characterInfo->headColorNo, characterInfo->hair1No, characterInfo->hair1ColorNo, characterInfo->hair2No, characterInfo->hair2ColorNo, characterInfo->eyeNo, characterInfo->eyeColorNo, characterInfo->mouthNo, characterInfo->mouthColorNo, characterInfo->etcNo, characterInfo->etcColorNo, characterInfo->bgNo, characterInfo->bgColorNo);
		query += temp;
		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);
		
		if (_result == SQLITE_OK)
		{
			log("insertDB() SUCCESS");
			return 1;
		}
		else
		{
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
			return 0;
		}
	}
}

void DatabaseManager::deleteCharacterDB(int no)
{
	{
		//�ش� ��ȣ�� row����
		//�ǵڿ� �����ֽ��ϴ�. where���� �������� ����
		string query = "delete from TB_USER_CHARACTER ";

		char temp[20];
		sprintf(temp, "where NO = %d", no);
		query += temp;
		_result = sqlite3_exec(_sqlite, query.c_str(), NULL, NULL, &_errorMSG);

		if (_result == SQLITE_OK)
			log("deleteDB() SUCCESS");
		else
			log("ERROR CODE : %d, ERROR MSG : %s", _result, _errorMSG);
	}
}

std::list<character*> DatabaseManager::selectGalleryDB(bool isRandom)
{
	list<character *> galleryItemList;
	string query = "select * from TB_USER_CHARACTER";
	if (isRandom)
		query += " order by random()";

	sqlite3_stmt *pStmt = NULL;
	_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &pStmt, NULL);

	if (_result == SQLITE_OK)
	{
		log("selectDB() SUCCESS");

		while (sqlite3_step(pStmt) == SQLITE_ROW)
		{
			character *item = new character;
			item->no = sqlite3_column_int(pStmt, 0);
			item->headNo = sqlite3_column_int(pStmt, 1);
			item->headColorNo = sqlite3_column_int(pStmt, 2);
			item->hair1No = sqlite3_column_int(pStmt, 3);
			item->hair1ColorNo = sqlite3_column_int(pStmt, 4);
			item->hair2No = sqlite3_column_int(pStmt, 5);
			item->hair2ColorNo = sqlite3_column_int(pStmt, 6);
			item->eyeNo = sqlite3_column_int(pStmt, 7);
			item->eyeColorNo = sqlite3_column_int(pStmt, 8);
			item->mouthNo = sqlite3_column_int(pStmt, 9);
			item->mouthColorNo = sqlite3_column_int(pStmt, 10);
			item->etcNo = sqlite3_column_int(pStmt, 11);
			item->etcColorNo = sqlite3_column_int(pStmt, 12);
			item->bgNo = sqlite3_column_int(pStmt, 13);
			item->bgColorNo = sqlite3_column_int(pStmt, 14);

			galleryItemList.push_back(item);
		}
	}
	else 
		log("ERROR CODE : %d", _result);
	
	sqlite3_finalize(pStmt);

	return galleryItemList;
}

int DatabaseManager::checkCount()
{
	int cnt = 0;
	{
		sqlite3_stmt *pStmt = NULL;
		string query = "select count(*) from TB_USER_CHARACTER";
		_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &pStmt, NULL);

		if (_result == SQLITE_OK) 
		{
			log("selectDB() SUCCESS");
			if (sqlite3_step(pStmt) == SQLITE_ROW) 
			{
				cnt = sqlite3_column_int(pStmt, 0);
			}
		}
		sqlite3_finalize(pStmt);
	}
	return cnt;
}


character * DatabaseManager::selectRandomGalleryDB()
{
	string query = "select * from TB_USER_CHARACTER ORDER BY random() limit 1";
	sqlite3_stmt *pStmt = NULL;
	_result = sqlite3_prepare_v2(_sqlite, query.c_str(), query.length(), &pStmt, NULL);

	character * item = new character();
	if (_result == SQLITE_OK)
	{
		log("selectDB() SUCCESS");
		if (sqlite3_step(pStmt) == SQLITE_ROW)
		{
			item->no = sqlite3_column_int(pStmt, 0);
			item->headNo = sqlite3_column_int(pStmt, 1);
			item->headColorNo = sqlite3_column_int(pStmt, 2);
			item->hair1No = sqlite3_column_int(pStmt, 3);
			item->hair1ColorNo = sqlite3_column_int(pStmt, 4);
			item->hair2No = sqlite3_column_int(pStmt, 5);
			item->hair2ColorNo = sqlite3_column_int(pStmt, 6);
			item->eyeNo = sqlite3_column_int(pStmt, 7);
			item->eyeColorNo = sqlite3_column_int(pStmt, 8);
			item->mouthNo = sqlite3_column_int(pStmt, 9);
			item->mouthColorNo = sqlite3_column_int(pStmt, 10);
			item->etcNo = sqlite3_column_int(pStmt, 11);
			item->etcColorNo = sqlite3_column_int(pStmt, 12);
			item->bgNo = sqlite3_column_int(pStmt, 13);
			item->bgColorNo = sqlite3_column_int(pStmt, 14);
		}
	}

	else
		log("ERROR CODE : %d", _result);
	sqlite3_finalize(pStmt);
	return item;
}


