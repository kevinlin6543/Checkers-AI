#include <assert.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "game.h"
#include <iostream>
#include <limits>
#include <list>
#include "board.h"
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::min;
using std::list;

const int game::maxIterDepth = 20;

game::game(): curBoard(stPtr<board>(new board())), bestMove(NULL), tempBestMove(NULL), maxdepth(0),
		curDepth(0), timeUp(false), gameOver(false), reachedEnd(false), startTime(0), endTime(0),
		startTimeD(0), endTimeD(0) {}

void game::startGame()
{
	gameOver = false;
	curBoard->startup();
	while (!gameOver)
		printGame();
}

void game::endMessage()
{
	char answer;
	bool loop = true;
	gameOver = true;
	cout << "The game is over." << endl;
	cout << endl;
	if (curBoard->getTurn() == P1RegularPiece)
		cout << "Player 1 wins." << endl;
	else
		cout << "Player 2 wins." << endl;
	cout << "Do you want to play again? (Y/N):" << endl;
	cin >> answer;
	while (loop)
	{
		if (tolower(answer) == 'y')
		{
			loop = false;
			startGame();
		}
		else if (tolower(answer) == 'n')
			loop = false;
		else
		{
			cout << "Do you want to play again? (Y/N):" << endl;
			cin >> answer;
		}
	}
}
void game::tieMessage()
{
	char answer;
	bool loop = true;
	gameOver = true;
	cout << "The game is over." << endl;
	cout << endl;
	std::cout << "Game ended in a tie" << '\n';
	cout << "Do you want to play again? (Y/N):" << endl;
	cin >> answer;
	while (loop)
	{
		if (tolower(answer) == 'y')
		{
			loop = false;
			startGame();
		}
		else if (tolower(answer) == 'n')
			loop = false;
		else
		{
			cout << "Do you want to play again? (Y/N):" << endl;
			cin >> answer;
		}
	}
}

void game::aiTurn()
{
	curBoard->printMoves();
	cout << "The AI will make a move." << endl;
	if (curBoard->moveList.size() == 1)
	{
		bestMove = curBoard->moveList.front();
		time(&startTime);
		time(&endTime);
		curDepth = 0;
	}
	else
	{
		time(&startTime);
		for (int i = 1; i != maxIterDepth; ++i)
		{
			time(&startTimeD);
			maxdepth = i;
			alphabeta(curBoard, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
			time(&endTimeD);
			if (difftime(endTimeD, startTimeD) >= ((board::timeLimit)/2))
			{
				time(&endTime);
				timeUp = true;
				break;
			}
			if (timeUp)
				break;
			else
				bestMove = tempBestMove;
			if (reachedEnd)
				break;
		}
	}
	assert(bestMove != NULL);
	turnMessage();
}

void game::turnMessage()
{
	curBoard->makeMove(bestMove);
	cout << "Completed search to depth " << curDepth << "." << endl;
	if (timeUp && curDepth != maxIterDepth && !reachedEnd) //or no way to get to maxdepth since gamespace end has been reached
		cout << "Ran out of time searching to depth " << curDepth + 1 << "." << endl;
	cout << "Search Duration:  " << difftime(endTime, startTime) << " seconds" << endl;
	cout << "Move done: ";
	board::convertMoves(bestMove->command);
	cout << endl;
	timeUp = false;
	reachedEnd = false;
	bestMove = NULL;
	tempBestMove = NULL;
}

void game::printGame()
{
	if (curBoard->tieGame)
	 tieMessage();
	curBoard->printBoard();
	if (curBoard->terminal())
		endMessage();
	else if (curBoard->isAITurn())
		aiTurn();
	else
		curBoard->inputCommand();
}

int game::alphabeta(stPtr<board>& b, int depth, int alpha, int beta)
{
	if (depth != maxdepth && b->terminal())
	{
		curDepth = maxdepth;
		reachedEnd = true;
		if (b->getTurn() == P1RegularPiece)
			return std::numeric_limits<int>::max();
		else
			return std::numeric_limits<int>::min();
	}
	reachedEnd = false;
	if (depth == 0)
		return b->evaluate();
	list<move*>::iterator iter = b->moveList.begin();

	int localalpha = std::numeric_limits<int>::min();
	int localbeta = std::numeric_limits<int>::max();

	if (b->getTurn() == P2RegularPiece)
	{
		for (; iter != b->moveList.end(); ++iter)
		{
			time(&endTime);
			if (difftime(endTime, startTime) >= (board::timeLimit - 1))
			{
				timeUp = true;
				break;
			}
			b->makeMove(*iter);
			stPtr<board> newB(new board(*b));
			int value = alphabeta(newB, depth-1, alpha, min(localbeta, beta));
			b->undoMove(*iter);
			b->nextTurn();
			if (value > alpha)
			{
				alpha = value;
				if (depth == maxdepth)
					tempBestMove = (*iter);
			}
			if (alpha >= beta && depth < maxdepth)
				return alpha;
		}
		if (!timeUp && depth == maxdepth)
			curDepth = depth;
		return alpha;
	}

	else
	{
		for (; iter != b->moveList.end(); ++iter)
		{
			time(&endTime);
			if (difftime(endTime, startTime) >= (board::timeLimit - 1))
			{
				timeUp = true;
				break;
			}
			b->makeMove(*iter);
			stPtr<board> newB(new board(*b));
			int value = alphabeta(newB, depth-1, max(localalpha, alpha), beta);
			b->undoMove(*iter);
			b->nextTurn();
			if (value < beta)
			{
				beta = value;
				if (depth == maxdepth)
					tempBestMove = (*iter);
			}
			if (alpha >= beta)
				return beta;
		}
		if (!timeUp && depth == maxdepth)
			curDepth = depth;
		return beta;
	}
}
