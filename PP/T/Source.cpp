#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <time.h>

using namespace std;

class Position;
class StonePosition;

class Rules
{
public:

	virtual bool isCorrect(Position*, Position*) = 0;
	virtual bool endGame(Position*) = 0;
};

class StoneRules : public Rules
{
	bool isCorrect(Position *oldPos, Position *newPos);
	bool endGame(Position* position);
};

class Board {
private:
	Position* crrPos;
public:
	Board() { }
	Position* getCrrPos() {
		return crrPos;
	}

	void setCrrPos(Position* pos) {
		crrPos = pos;
	}
	void display();

};

class StoneBoard : public Board {
public:
	StoneBoard() { }
};

class Position 
{
public:
	virtual void display() = 0;
	virtual Position* changePosition(int number) = 0;

};

class StonePosition : public Position {
	friend class StoneRules;

private:
	int count;
public:

	void display()
	{
		cout << count << endl;
	}
	Position* changePosition(int count)
	{
		//cout << this->count - count;
		return new StonePosition(this->count - count);
	}

	StonePosition(const int count)
	{
		this->count = count;
	}

	int getCount()
	{
		return count;
	}

	friend ostream& operator<<(ostream&, StonePosition);
};

class Game {
protected:
	Board* board;
	Rules* rules;
public:
	Board* getBoard() {
		return board;
	}

	virtual bool checkMove(Position *p) = 0;

	virtual Position* getInitPosition() = 0;

	virtual bool endGame() = 0;

	virtual vector<Position*> getPossibleMoves(Position* position) = 0;
};

class StoneGame : public Game {
public:
	StoneGame() {
		this->board = new StoneBoard();
		this->rules = new StoneRules();
	}
	Position* getInitPosition() {
		return new StonePosition(21);
	}
	bool checkMove(Position *newPosition)
	{
		return rules->isCorrect(board->getCrrPos(), newPosition);
	}
	bool endGame()
	{
		return rules -> endGame(board ->getCrrPos());
	}

	virtual vector<Position*> getPossibleMoves(Position* position)
	{
		StonePosition* p = (StonePosition*)position;
		int count = p->getCount();
		vector<Position*> moves;
		moves.push_back(p->changePosition(1));
		if (count > 1)
		{
			moves.push_back(p->changePosition(count - 2 < 0 ? count : 2));
			
			if(count > 2)
				moves.push_back(p->changePosition(count - 3 < 0 ? count : 3));
		}
		

		return moves;
	}
};

ostream& operator<<(ostream& out, StonePosition position) {
	out << "Position(" << position.count << ")" << endl;
	return out;
}



class Player {
public:
	Player(const string name, Game* game) : name(name)
	{
	//	this->name = name;
		this->game = game;
	}

	virtual Position* makeMove() = 0;
	virtual void lose() = 0;
 const string name;
protected:
	
	Game* game;
};

class Human : public Player {
public:
	Human(const string name, Game* game) : Player(name, game) { }

	Position* makeMove() {
		Position *oldPosition = game->getBoard()->getCrrPos();
		cout << "Ile zabierasz kamieni?" << endl;
		cout << "Obecna liczba kamieni: ";
		game->getBoard()->display();
		//	string str; cin >> str;
		//Postion *newPosition = game->CreatePos(
		int stones;
		cin >> stones;
		Position *newPosition = oldPosition->changePosition(stones);


		return newPosition;
	}

	virtual void lose()
	{

	}
};

class Computer : public Player 
{
	Position* lastMove;
	Position* curMove;

public:


	Computer(const string name, Game* game) : Player(name, game) 
	{
		lastMove = 0;
		curMove = 0;
	}

	map<Position*, vector<Position*> > moves;


	Position* makeMove()
	{
		srand(time(NULL));
		Position* crrPos = game->getBoard()->getCrrPos();
		
		cout << "Ile zabierasz kamieni?" << endl;
		cout << "Obecna liczba kamieni: ";
		game->getBoard()->display();
		map<Position*, vector<Position*> >::iterator it;
		it = moves.find(crrPos);

		if (it == moves.end())
		{
			vector<Position*> possibleMoves = game->getPossibleMoves(crrPos );
			moves.insert(pair<Position*, vector<Position*> >(crrPos, possibleMoves));
		}

		it = moves.find(crrPos);

		if (it->second.size() == 0)
			return game->getBoard()->getCrrPos()->changePosition(1);

		lastMove = crrPos;;
		curMove = it->second[rand() % (it->second.size())];

		return curMove;
	}

	void lose()
	{

	}

	/*virtual void lose()
	{
		vector<Position*> *move = &moves.find(lastMove)->second;
		for (int i = 0; i < move->size; i++)
		{
			if ((*move)[i] == lastMove)
			{
				Position * tmp;
			}
		}
	}*/

};

class Judge
{
public:
	void addPlayer(Player* player) {
		players.push_back(player);
		numOfPlayers++;
	}

	Judge()
	{
		numOfPlayers = 0;
	}
	void setGame(Game* game) {
		this->game = game;
	}

	void startGame()
	{
		Position* p = game->getInitPosition();
		game->getBoard()->setCrrPos(p);
		board = game->getBoard();

		crrPlayer = 0;
		do
		{
			cout << "Wykonujne ruch gracz: " << players[crrPlayer] -> name  << endl;

			
			Position *newPosition;

			do
			{
				newPosition = players[crrPlayer]->makeMove();

			} while (!game->checkMove(newPosition));

			board->setCrrPos(newPosition);
			crrPlayer = (crrPlayer + 1) % numOfPlayers;
			
			
		} while (!game->endGame());
	}
private:
	vector<Player*> players;
	Game* game;
	Board* board;
	int crrPlayer;
	int numOfPlayers;

};

int main() {
	StoneGame game;

	Judge judge; judge.setGame(&game);

	Human human("Gracz1", &game); judge.addPlayer(&human);
	Computer computer("Gracz2", &game); judge.addPlayer(&computer);

	judge.startGame();
}


bool StoneRules::isCorrect(Position* oldPos, Position *newPos)
{
	int first = ((StonePosition*)oldPos)->count;
	int sec = ((StonePosition*)newPos)->count;
//	cout << first << endl;
//	cout << sec << endl;
//	cout << first - sec;
	if ((first - sec > 0) && (first - sec <= 3) && (sec >= 0) )
		return true;

	return false;
}

bool StoneRules::endGame(Position *position)
{
	if (((StonePosition*)position)->count <= 0)
		return true;

	return false;
}

void Board::display()
{
	crrPos->display();
}
