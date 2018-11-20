#ifndef BOARD_H_
#define BOARD_H_

#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unistd.h>
#include <assert.h>
#include <sstream>
#include <cstdlib>
#include <limits>

const std::string INVALIDSQCOLOR = "48;5;0";
const std::string VALIDSQCOLOR 	=	"48;5;7";
const std::string PLAYER1COLOR 	=	"38;5;9";
const std::string PLAYER2COLOR 	=	"38;5;20";
extern int numNonJumps;

#define P1RegularPiece	'r'
#define P2RegularPiece	'b'
#define P1KingPiece		'R'
#define P2KingPiece		'B'
#define EmptyPiece		' '

class jump
{
	int numTimes, xs, ys, x, y, xend, yend, key;
	jump* prev;
	char jumpingPiece;
	bool noNextMove;
	char c;
	jump(char jpingp, char piece, int xs, int ys, int xc, int yc, int xe, int ye, jump* p, int k):
		prev(p), jumpingPiece(jpingp), noNextMove(true), numTimes(0), c(piece), xs(xs), ys(ys),
		 x(xc), y(yc), xend(xe), yend(ye), key(k){}

	friend class move;
	friend class board;
};

class move
{
	char movePiece;
	int xi, yi, xf, yf;
	std::string command;
	std::list<jump*> jumpPoints;
	move(char c, int xs, int ys, int xe, int ye): movePiece(c), xi(xs), yi(ys), xf(xe), yf(ye) {}
	~move();
	friend class board;
	friend class game;
};

class board
{
	char pieces[8][4];
	char color;
	std::list<move*> moveList;
	board();
	~board();
	board(const board&);
	static bool isAI[2];
	bool tieGame;
	static int timeLimit;
	static void selectAI();
	static void convertMoves(const std::string&);
	char getTurn(){ return color; }
	char *colorPieces(char);
	bool isAITurn();
	bool terminal(); // Tests for the win or lose case
	bool movesAvailable();
	bool isValidPos(int, int);
	bool jumpsAvailable();
	void jumpAvailable(std::list<jump*>&, char c, int, int, jump*);
	bool jumpConditions(int, int, int, int);
	bool listMoves();
	void nextTurn();
	void makeKing(const int&, const int&);
	void reset();
	void modifyBoard(std::ifstream&);
  void removeRet(std::string&);
	void createJump(std::list<jump*>&, char, int, int, int, int, int, int, jump*);
	void createJumpMove(std::list<jump*>&);
	void checkNeighbors(int&, int&);
	void createMove(const int&, const int&, int, int); //generates a code for each move which is easily parsed
	void convert(const int&, const int&, std::string&);
	void printBoard();
	void printMoves();
	void makeMove(move*);
	void undoMove(move*);
	void inputCommand();
	void startup();
  int createkey(int, int, int, int, int, int);
  int reverse(int);
	int convertY(const int&, const int&);
	int cornerDiagonal(char, char);
	int evaluate();
	friend class game;
	template <class T> friend class stPtr;
};
#endif /* BOARD_H_ */
