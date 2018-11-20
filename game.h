#ifndef GAME_H_
#define GAME_H_

#include <algorithm>
#include "board.h"
#include <time.h>
#include <iostream>
#include <limits>
#include <list>
#include <assert.h>
#include <algorithm>
#include <cctype>
#include <iostream>
template <class T>
class stPtr
{
	private:
		T* pData;
	public:
		stPtr(T* pVal): pData(pVal) {}
		~stPtr()	{delete pData;}
		T& operator*() {return *pData;}
		T* operator->() {return pData;}
};
class game
{
	stPtr<board> curBoard;
	static const int maxIterDepth;
	move* bestMove;
	move* tempBestMove;
	int maxdepth, curDepth;
	bool timeUp, gameOver, reachedEnd;
	time_t startTime, endTime, startTimeD, endTimeD;
	void printGame();
	void turnMessage();
	void aiTurn();
	int alphabeta(stPtr<board>&, int, int, int);
	void endMessage();
	void tieMessage();

public:
	game();
	void startGame();
};


#endif /* GAME_H_ */
