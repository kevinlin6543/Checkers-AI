#include "board.h"

using std::endl;
using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::ifstream;
using std::stringstream;
using std::toupper;
using std::tolower;
using std::list;

bool board::isAI[2];
char colIndex[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
int board::timeLimit = 0;
int numNonJumps;
bool tieGame = false;
bool jumpAvail = false;

move::~move()
{
	for (list<jump*>::iterator it = jumpPoints.begin(); it != jumpPoints.end(); ++it)
	{
		--(*it)->numTimes;
		if ((*it)->numTimes == 0)
			delete (*it);
	}
}

board::board() {	reset(); }
board::~board()
{
	while (!moveList.empty())
	{
		delete moveList.front();
		moveList.pop_front();
	}
}

board::board(const board& b): color(b.color)
{
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
			pieces[i][j] = b.pieces[i][j];
}

bool board::isAITurn()
{
		if (color == P2RegularPiece && isAI[0])
			return true;
		if (color == P1RegularPiece && isAI[1])
			return true;
		return false;
}

void board::nextTurn()
{
		if (color == P1RegularPiece)
			color = P2RegularPiece;
		else
			color = P1RegularPiece;
}

bool board::movesAvailable()
{
	if (jumpsAvailable())
		return true;
	if (listMoves())
		return true;
	return false;
}

bool board::terminal()
{
	if (!movesAvailable())
		return true;
	return false;
}
void board::makeKing(const int& x, const int& y)
{
	if (x == 0 && pieces[x][y] == P1RegularPiece)
		pieces[x][y] = P1KingPiece;
	if (x == 7 && pieces[x][y] == P2RegularPiece)
		pieces[x][y] = P2KingPiece;
}
bool board::isValidPos(int x, int y)
{
	if (x >= 0 && x < 8 && y >= 0 && y < 4)
		return true;
	else
		return false;
}

void board::reset()
{
	color = P2RegularPiece;
	for (int i = 0; i != 3; ++i)
		for (int j = 0; j != 4; ++j)
			pieces[i][j] = P2RegularPiece;
	for (int i = 3; i != 5; ++i)
		for (int j = 0; j != 4; ++j)
			pieces[i][j] = 'e';
	for (int i = 5; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
			pieces[i][j] = P1RegularPiece;
}
void board::modifyBoard(ifstream& fileIn)
{
	string line;
	int count = 0;
	while (getline(fileIn, line) && count != 8)
	{
		removeRet(line);
		stringstream ss(line);
		for (int j = 0; j != 4; ++j)
			ss >> pieces[count][j];
		++count;
	}

	getline(fileIn, line);
	removeRet(line);
	stringstream ss(line);
	ss >> color;
	color = tolower(color);
	assert(color == P2RegularPiece || color == P1RegularPiece);
}

inline void board::removeRet(std::string& line)
{
    if (*line.rbegin() == '\r' || *line.rbegin() == '\n')
    	line.erase(line.length() - 1);
}

void board::convertMoves(const string& str)
{
	string::const_iterator it = str.begin();
	int temp = (*it) - '0';
	it += 2;
	cout << colIndex[(*it)-'0'] << 8 - temp;
	it += 2;
	while (*it != '-')
	{
		temp = (*it) - '0';
		cout << " -> ";
		it += 2;
		cout << colIndex[(*it)-'0'] << 8 - temp;
		it += 2;
	}
}

void board::inputCommand()
{
	printMoves();
	cout << "Enter move: ";
	string m;
	int moveNum;
	std::cin >> moveNum;
	while (moveNum - 1 >= moveList.size())
	{
		std::cout << "Invalid move.\nEnter another move: ";
		std::cin >> moveNum;
	}
	list<move*>::iterator it = moveList.begin();
	advance(it, moveNum - 1);
	makeMove(*it);

}

char *board::colorPieces(char piece) {
	char *buf = (char *) malloc(30);

	if (piece == P1RegularPiece) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1RegularPiece);
	}
	else if (piece == P1KingPiece) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1KingPiece);
	}
	else if (piece == P2RegularPiece) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2RegularPiece);
	}
	else if (piece == P2KingPiece) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2KingPiece);
	}
	else {
		sprintf(buf, "\033[%s;1;92m %c \033[0m", VALIDSQCOLOR.c_str(), EmptyPiece);
	}

	return buf;
}

void board::printBoard() {
	cout << "   \e(0\x6C\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6B\e(B" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << " \033[1;92m" << (8-i) << "\033[0m ";
		if (i % 2 == 0)
			cout << ("\e(0\x78\e(B\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m");

		for (int j = 0; j < 3; j++)
		{
			char *buf = colorPieces(pieces[i][j]);
			cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B" << ("\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m");
			free(buf);
		}

		char *buf = colorPieces(pieces[i][3]);
		cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B";
		free (buf);
		if (i % 2 == 1)
			cout << ("\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m\e(0\x78\e(B");
		cout << endl;

		if (i==7)
			cout << "   \e(0\x6D\e(B";
		else
			cout << "   \e(0\x74\e(B";
		if (i < 7)
			cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		else
			cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		if (i==7)
			cout << "\e(0\x6A\e(B";
		else
			cout << "\e(0\x75\e(B";
		cout << endl;
	}

	cout << "\033[1;92m     A   B   C   D   E   F   G   H  \033[0m\n" << endl;
	char regPiece[30], kingPiece[30];
	sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1RegularPiece);
	sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1KingPiece);
	cout << "Player 1 Pawn: " << regPiece << " King: " << kingPiece << endl;
	sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2RegularPiece);
	sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2KingPiece);
	cout << "Player 2 Pawn: " << regPiece << " King: " << kingPiece << endl;
}
void board::printMoves()
{
	if (!jumpAvail)
		numNonJumps++;
	else
		numNonJumps = 0;
	if (numNonJumps > 65)
		tieGame = true;
	if (color == P1RegularPiece)
		cout << "Player 2 turn" << endl;
	else cout << "Player 1 turn" << endl;
	cout << "Available Moves: " << endl;
	list<move*>::const_iterator it = moveList.begin();
	for (int i = 1; it != moveList.end(); ++it)
	{
		cout << "Move " << i << ": ";
		convertMoves((*it)->command);
		cout << endl;
		i++;
	}
	cout << endl;
}
void board::makeMove(move* m)
{
	if (!m->jumpPoints.empty())
	{
		list<jump*>::iterator it = m->jumpPoints.begin();
		for (; it != m->jumpPoints.end(); ++it)
			pieces[(*it)->x][(*it)->y] = 'e';
	}
	char c = pieces[m->xi][m->yi];
	pieces[m->xi][m->yi] = 'e';
	pieces[m->xf][m->yf] = c;

	makeKing(m->xf, m->yf);
	nextTurn();
}

void board::undoMove(move* m)
{
	if (!m->jumpPoints.empty())
	{
		for (list<jump*>::iterator it = m->jumpPoints.begin(); it != m->jumpPoints.end(); ++it)
		{
			pieces[(*it)->xs][(*it)->ys] = 'e';
			pieces[(*it)->x][(*it)->y] = (*it)->c;
			pieces[(*it)->xend][(*it)->yend] = 'e';
		}
	}
	pieces[m->xf][m->yf] = 'e';
	pieces[m->xi][m->yi] = m->movePiece;
}
int board::cornerDiagonal(char l, char w)
{
	int c = 0;
	if (tolower(pieces[0][0]) == l || tolower(pieces[1][0]) == l)
	{
		c += 9;
		if (tolower(pieces[0][0]) == w)
			c -= 3;
		if (tolower(pieces[1][0]) == w)
			c -= 3;
		if (tolower(pieces[1][1]) == w)
			c -= 1;
		if (tolower(pieces[2][0]) == w)
			c -= 1;
		if (tolower(pieces[2][1]) == w)
			c -= 1;
		if (tolower(pieces[3][1]) == w)
			c -= 1;
	}
	if (tolower(pieces[6][3]) == l || tolower(pieces[7][3]) == l)
	{
		c += 9;
		if (tolower(pieces[4][2]) == w)
			c -= 1;
		if (tolower(pieces[5][2]) == w)
			c -= 1;
		if (tolower(pieces[5][3]) == w)
			c -= 1;
		if (tolower(pieces[6][2]) == w)
			c -= 1;
		if (tolower(pieces[6][3]) == w)
			c -= 3;
		if (tolower(pieces[7][3]) == w)
			c -= 3;
	}
	return c;
}

int board::evaluate()
{
	int a1, a2, b, c, d;
 	a1 = a2 = b = c = d = 0;
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
		{
			if (pieces[i][j] == P2RegularPiece)
			{
				a1 += 2;
				if (i == 0)
					b += 9;
				else b += i;
				c += 1;
			}
			else if (pieces[i][j] == P1RegularPiece)
			{
				a2 -=2;
				if (i == 7)
					b -= 9;
				else b -= (7 - i);
				c -= 1;
			}
			else if (pieces[i][j] == P2KingPiece)
			{
				a1 += 3;
				c += 1;
			}
			else if (pieces[i][j] == P1KingPiece)
			{
				a2 -= 3;
				c -= 1;
			}
		}
	if (c > 0 && a2 >= -8)
		d -= cornerDiagonal(P1RegularPiece, P2RegularPiece);
	else if (c < 0 && a1 <= 8)
		d += cornerDiagonal(P2RegularPiece, P1RegularPiece);
	a1 *= 100000000;
	a2 *= 100000000;
	b *= 1000000;
	c *= 10000;
	d *= 100;
	int e = rand() % 100;
	if (color == P1RegularPiece)
		e = -e;
	return a1 + a2 + b + c + d + e;
}

void board::startup()
{
	reset();
	selectAI();
	bool b = true;
	tieGame = false;
	cout << "Load a game from a file? (Y/N):" << endl;
	char c = ' ';
	while (b)
	{
		cin >> c;
		if (tolower(c) == 'y' || tolower(c) == 'n')
			b = false;
	}
	if (tolower(c) == 'y')
	{
		string name;
		cout << "Enter filename: " << endl;
		cin >> name;
		ifstream fin(name.c_str());
		while (!fin.good())
		{
			cout << "Enter filename: " << endl;
			cin >> name;
			fin.open(name.c_str());
		}
		modifyBoard(fin);
	}
}
inline int board::createkey(int xs, int ys, int xj, int yj, int xe, int ye)
{
    return ye + xe*10 + yj*100 + xj*1000 + ys*10000 + xs*100000;
}
int board::reverse(int i)
{
    int num = 0;
    while (i/100 > 0)
    {
        num += i % 100;
        num *= 100;
        i /= 100;
    }
    num += i;
    return num;

}
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
    pieces[xs][ys] = 'e';
    int key = createkey(xs+1, ys+1, xj+1, yj+1, xe+1, ye+1);
    jump* jcheck = jp;
    while (jcheck != NULL)
    {
        if (key == jcheck->key || key == reverse(jcheck->key))
            return;
        jcheck = jcheck->prev;
    }
	jump* j = new jump(c, pieces[xj][yj], xs, ys, xj, yj, xe, ye, jp, key);
	if (jp != NULL)
		jp->noNextMove = false;
	jlist.push_front(j);
	jumpAvailable(jlist, c, xe, ye, j);
}
void board::createJumpMove(list<jump*>& jlist)
{
	if (!jlist.empty())
	{
		list<jump*>::const_iterator it = jlist.begin();
		for (; it != jlist.end(); ++it)
		{
			if ((*it)->noNextMove)
			{
				move* m = new move((*it)->jumpingPiece, -1, -1, -1, -1);
				jump* jp = (*it);
				while (jp != NULL)
				{
					m->jumpPoints.push_front(jp);
					++jp->numTimes;
					jp = jp->prev;
				}
				m->xi = m->jumpPoints.front()->xs;
				m->yi = m->jumpPoints.front()->ys;
				convert(m->jumpPoints.front()->xs, m->jumpPoints.front()->ys, m->command);
				for (list<jump*>::iterator it = m->jumpPoints.begin(); it != m->jumpPoints.end(); ++it)
				{
					convert((*it)->xend, (*it)->yend, m->command);
					if ((*it)->noNextMove)
					{
						m->xf = (*it)->xend;
						m->yf = (*it)->yend;
					}
				}

				moveList.push_back(m);
				m->command += "-1";
				undoMove(m);
			}
		}
	}
}
void board::jumpAvailable(list<jump*>& jlist, char c, int x, int y, jump* jp= NULL)
{
	if (tolower(c) == P2RegularPiece || c == P1KingPiece)
	{
		if (x % 2 == 0)
		{
			if (jumpConditions(x+1, y, x+2, y-1))
				createJump(jlist, c, x, y, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x+1, y+1, x+2, y+1))
				createJump(jlist, c, x, y, x+1, y+1, x+2, y+1, jp);
		}
		else
		{
			if (jumpConditions(x+1, y-1, x+2, y-1))
				createJump(jlist, c, x, y, x+1, y-1, x+2, y-1, jp);
			if (jumpConditions(x+1, y, x+2, y+1))
				createJump(jlist, c, x, y, x+1, y, x+2, y+1, jp);
		}
	}
	if (tolower(c) == P1RegularPiece || c == P2KingPiece)
	{
		if (x % 2 == 0)
		{
			if (jumpConditions(x-1, y+1, x-2, y+1))
				createJump(jlist, c, x, y, x-1, y+1, x-2, y+1, jp);
			if (jumpConditions(x-1, y, x-2, y-1))
				createJump(jlist, c, x, y, x-1, y, x-2, y-1, jp);
		}
		else
		{
			if (jumpConditions(x-1, y-1, x-2, y-1))
				createJump(jlist, c, x, y, x-1, y-1, x-2, y-1, jp);
			if (jumpConditions(x-1, y, x-2, y+1))
				createJump(jlist, c, x, y, x-1, y, x-2, y+1, jp);
		}
	}
}

bool board::jumpsAvailable()
{
	while (!moveList.empty())
	{
		delete moveList.front();
		moveList.pop_front();
	}
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (pieces[i][j] == color || pieces[i][j] == toupper(color))
			{
        list<jump*> jlist;
        jumpAvailable(jlist, pieces[i][j], i, j, NULL);
				createJumpMove(jlist);
			}
		}
	}

	if (moveList.empty())
		return (jumpAvail = false);
	return (jumpAvail = true);
}

bool board::jumpConditions(int xj, int yj, int xe, int ye)
{
	if (isValidPos(xj, yj) && isValidPos(xe, ye) && pieces[xj][yj] != 'e' &&
			pieces[xj][yj] != color && pieces[xe][ye] == 'e' &&  pieces[xj][yj] != std::toupper(color))
		return true;
	return false;
}

void board::convert(const int& x, const int& y, string& s)
{
	 char c1 = '0' + x;
	 char c2;
	 if (x % 2 == 0)
	 {
		 c2 = '0' + (2*y + 1);
	 }
	 else
	 {
		 c2 = '0' + (2*y);
	 }
	 s += c1;
	 s += ' ';
	 s += c2;
	 s += ' ';
}

inline int board::convertY(const int& x, const int& y)
{
    if (x % 2 == 0)
        return (2*y + 1);
    else return (2*y);
}

void board::selectAI()
{
	bool b = true;
	char c = ' ';
	while (b)
	{
		cout << "Will player # 1 be a computer? (Y/N):" << endl;
		cin >> c;
		if (tolower(c) == 'y')
		{
			board::isAI[0] = true;
			b = false;
		}
		else if (tolower(c) == 'n')
		{
			board::isAI[0] = false;
			b = false;
		}
	}
	b = true;
	c = ' ';
	while (b)
	{
		cout << "Will player # 2 be a computer? (Y/N):" << endl;
		cin >> c;
		if (tolower(c) == 'y')
		{
			board::isAI[1] = true;
			b = false;
		}
		else if (tolower(c) == 'n')
		{
			board::isAI[1] = false;
			b = false;
		}
	}
	if (board::isAI[0] == true || board::isAI[1] == true)
	{
		cout << "Enter a time limit for the computer in seconds:" << endl;
		cin >> timeLimit;
		if (timeLimit > 60 || timeLimit < 3)
			cin >> timeLimit;
	}
}

void board::checkNeighbors(int& x, int& y)
{
	if (tolower(pieces[x][y]) == P2RegularPiece || pieces[x][y] == P1KingPiece)
	{
		if (x % 2 == 0)
		{
			createMove(x, y, x+1, y);
			createMove(x, y, x+1, y+1);
		}
		else
		{
			createMove(x, y, x+1, y);
			createMove(x, y, x+1, y-1);
		}
	}
	//check up movement
	if (tolower(pieces[x][y]) == P1RegularPiece || pieces[x][y] == P2KingPiece)
	{
		if (x % 2 == 0)
		{
			createMove(x, y, x-1, y);
			createMove(x, y, x-1, y+1);
		}
		else
		{
			createMove(x, y, x-1, y);
			createMove(x, y, x-1, y-1);
		}
	}
}

void board::createMove(const int& xi,const int& yi, int xf, int yf)
{
	if (isValidPos(xf, yf) && pieces[xf][yf] == 'e')
	{
		move* m = new move(pieces[xi][yi], xi, yi, xf, yf);
		moveList.push_back(m);
		convert(xi, yi, m->command);
		convert(xf, yf, m->command);
		m->command += "-1";
	}
}

bool board::listMoves()
{
	while (!moveList.empty())
	{
		delete moveList.front();
		moveList.pop_front();
	}
	if (color == P2RegularPiece)
	{
		for (int i = 0; i!= 8; ++i)
			for (int j = 0; j != 4; ++j)
				if (pieces[i][j] == color || pieces[i][j] == toupper(color))
					checkNeighbors(i, j);
	}
	else
	{
		for (int i = 7; i!= -1; --i)
			for (int j = 0; j != 4; ++j)
				if (pieces[i][j] == color || pieces[i][j] == toupper(color))
					checkNeighbors(i, j);
	}

	if (moveList.empty())
		return false;
	return true;
}
