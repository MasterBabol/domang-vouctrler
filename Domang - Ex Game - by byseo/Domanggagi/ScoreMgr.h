#pragma once
class opzScoreMgr
{
public:
	static void S_Create();
	static void S_Delete();

	static void S_Reset(){_instance->_score = 0; }

	static int S_GetScore(){return _instance->_score;}

	static void S_AddScore(int point){_instance->_score += point;}

private:
	static opzScoreMgr *_instance;

	int _score;

	opzScoreMgr(void);
	~opzScoreMgr(void);
};