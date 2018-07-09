#include <iostream>

class Board
{
	int position[9];


	int midHorizontal()
	{
		int tester = position[3];

		for(int i = 3; i < 6; i++)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int midVertical()
	{
		int tester = position[1];

		for(int i = 1; i < 9; i += 3)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int topHorizontal()
	{
		int tester = position[0];

		for(int i = 0; i < 3; i++)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int leftVertical()
	{
		int tester = position[0];

		for(int i = 0; i < 9; i += 3)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int rightVertical()
	{
		int tester = position[2];

		for(int i = 2; i < 9; i += 3)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int botHorizontal()
	{
		int tester = position[6];


		for(int i = 6; i < 9; i++)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int descendingDiagonal()
	{
		int tester = position[0];

		for(int i = 0; i < 9; i += 4)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int ascendingDiagonal()
	{
		int tester = position[2];

		for(int i = 2; i < 7; i += 2)
		{
			if(position[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int isFull()
	{
		for(int i = 0; i < 9; i++)
		{
			if(position[i] == 0)
			{
				return 0;
			}
		}


		return 2;
	}

public:

	int isEnd()
	{
		if(topHorizontal())
		{
			return topHorizontal();
		}
		if(leftVertical())
		{
			return leftVertical();
		}
		if(rightVertical())
		{
			return rightVertical();
		}
		if(botHorizontal())
		{
			return botHorizontal();
		}
		if(ascendingDiagonal())
		{
			return ascendingDiagonal();
		}
		if(descendingDiagonal())
		{
			return descendingDiagonal();
		}
		if(midHorizontal())
		{
			return midHorizontal();
		}
		if(midVertical())
		{
			return midVertical();
		}
		if(isFull())
		{
			return isFull();
		}

		return 0;
	}

	int max(int a, int b)
	{
		return a > b ? a : b;
	}
/*
	int findBestMove(Board board, const int tmpMark, const int mark)
	{
		for(int i = 0; i < 9; i++)
		{
			if(board.position[i] != 0)
				continue;

			board.position[i] = tmpMark;
			int end = isEnd();

			if(end == 0)
			{
				int ans = findBestMove(board, -tmpMark, mark);

				if(ans == -1)
					return -1;

				if(ans == 1)
					return 1;

			}
			else if(end == mark)
			{
				return 1;
			}
			else if(end == -mark)
			{
				return -1;
			}
			else if(end == 2)
				return 0;

		}


		return 0;
	}
*/
/*
	Board findBestMove(int mark)
	{
		int ans = -2;
		int max = -2;
		Board board(*this);


		for(int i = 0; i < 9; i++)
		{
			if(board.position[i] != 0)
				continue;

			board.position[i] = mark;
			int tmp = findBestMove(board, -mark, mark);

			if(tmp > max)
			{
				ans = i;
				max = tmp;
			}

			board.position[i] = 0;
		}

		board.position[ans] = mark;


		return board;
	}
*/

	int size()
	{

		int ans = 0;
		for(int i = 0; i < 9; i++)
		{
			if(position[i] != 0)
				ans++;
		}

		return ans;
	}

	int findBestMove(Board board, int tmpMark, int mark)
	{
		int ans = 0;


		for(int i = 0; i < 9; i++)
		{
			if(board.position[i] != 0)
				continue;

			board.position[i] = tmpMark;

			int end = board.isEnd();

			if(end == 0)
			{
				ans += findBestMove(board, -tmpMark, mark);
			}
			else if(end == -mark)
			{
				ans -= (9 - size())*10;
			}

			board.position[i] = 0;

		}

		return ans;
	}

	Board findBestMove(int mark)
	{
		Board board(*this);

		int max = -1;
		int ans = -1000000000;

		for(int i = 0; i < 9; i++)
		{
			if(board.position[i] != 0)
				continue;

			board.position[i] = mark;

			int tmp = findBestMove(board, -mark, mark);

			if(tmp > ans)
			{
				ans = tmp;
				max = i;
			}

			board.position[i] = 0;
		}

		board.position[max] = mark;


		return board;
	}

	Board &operator=(const Board &board)
	{
		for(int i = 0; i < 9; i++)
		{
			position[i] = board.position[i];
		}

		return *this;
	}

	Board(const Board &board)
	{
		for(int i = 0; i < 9; i++)
		{
			position[i] = board.position[i];
		}
	}

	Board()
	{
		for(int i = 0; i < 9; i++)
		{
			position[i] = 0;
		}
	}

	bool isValid(int x)
	{
		return x < 3 && x >= 0;
	}

	bool isEmpty(int y, int x)
	{
		return position[y * 3 + x] == 0;
	}

	bool isMarkable(int y, int x)
	{
		int a = isValid(y);
		int b = isValid(x);
		int c = isEmpty(y, x);
		return a && b && c;
	}

	bool nought(int y, int x)
	{
		if(!isMarkable(y, x))
		{
			return 0;
		}
		else
		{
			position[y * 3 + x] = 1;

			return 1;
		}
	}

	bool cross(int y, int x)
	{
		if(!isMarkable(y, x))
		{
			return 0;
		}
		else
		{
			position[y * 3 + x] = -1;

			return 1;
		}
	}

	int operator[](int i)
	{
		return position[i];
	}

	int calcDiff(const Board &board)
	{
		Board helper = board;

		for(int i = 0; i < 9; i++)
		{
			if(position[i] == helper.position[i])
			{
				helper.position[i] = 0;
			}
			else
			{
				helper.position[i] = 1;
			}
		}

		int ans = 0;

		for(int i = 0; i < 9; i++)
		{
			ans += helper.position[i];
		}


		return ans;
	}


	friend std::ostream &operator<<(std::ostream &out, const Board &board);


};

class Player
{

protected:
	std::string name;
	int mark;


public:

	Player(std::string aName)
	{
		mark = 0;
		name = aName;
	}

	void setMark(int aMark)
	{
		if(mark == 0)
		{
			mark = aMark;
		}
	}


	std::string getName()
	{
		return name;
	}

	virtual Board makeMove(Board &board) = 0;
};

class Human : public Player
{
public:

	Human(std::string name) : Player(name)
	{}

	Board makeMove(Board &board)
	{
		int y, x;

		Board helper = board;

		do
		{
			std::cout << board;
			printf("Podaj kordynaty y i x\n");
			scanf("%i%i", &y, &x);
			y--;
			x--;
		} while(!helper.isMarkable(y, x));

		if(mark == 1)
		{
			helper.nought(y, x);
		}
		else
		{
			helper.cross(y, x);
		}


		return helper;
	}

};

class Computer : public Player
{

public:

	Computer(std::string name) : Player(name)
	{
	}

	Board makeMove(Board &board)
	{
		Board newMove = board.findBestMove(mark);


		return newMove;
	}


};

class Game
{
	Board board;
	Player *players[2];

/*
	int midHorizontal()
	{
		int tester = board[3];

		for(int i = 3; i < 6; i++)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int midVertical()
	{
		int tester = board[1];

		for(int i = 1; i < 9; i += 3)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int topHorizontal()
	{
		int tester = board[0];

		for(int i = 0; i < 3; i++)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int leftVertical()
	{
		int tester = board[0];

		for(int i = 0; i < 9; i += 3)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int rightVertical()
	{
		int tester = board[2];

		for(int i = 2; i < 9; i += 3)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int botHorizontal()
	{
		int tester = board[6];


		for(int i = 6; i < 9; i++)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int descendingDiagonal()
	{
		int tester = board[0];

		for(int i = 0; i < 9; i += 4)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int ascendingDiagonal()
	{
		int tester = board[2];

		for(int i = 2; i < 7; i += 2)
		{
			if(board[i] != tester)
			{
				return 0;
			}
		}

		return tester;
	}

	int isFull()
	{
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
				return 0;
		}


		return 2;
	}

	int isEnd()
	{
		if(topHorizontal())
		{
			return topHorizontal();
		}
		if(leftVertical())
		{
			return leftVertical();
		}
		if(rightVertical())
		{
			return rightVertical();
		}
		if(botHorizontal())
		{
			return botHorizontal();
		}
		if(ascendingDiagonal())
		{
			return ascendingDiagonal();
		}
		if(descendingDiagonal())
		{
			return descendingDiagonal();
		}
		if(midHorizontal())
		{
			return midHorizontal();
		}
		if(midVertical())
		{
			return midVertical();
		}
		if(isFull())
			return isFull();

		return 0;
	}
*/


public:

	Game(Player *first, Player *second)
	{
		players[0] = first;
		players[1] = second;
	}

	void win(std::string name)
	{
		printf("Wygrał ");
		std::cout << name << std::endl;
	}

	void run()
	{
		players[0]->setMark(-1);
		players[1]->setMark(1);

		for(int i = 0; !board.isEnd(); i = 1 - i)
		{
			Board helper;

			do
			{

				helper = players[i]->makeMove(board);
			} while(!board.calcDiff(helper) == 1);

			board = helper;
		}


		int end = board.isEnd();

		std::cout << board;
		if(end == 1)
		{
			win(players[1]->getName());
		}
		else if(end == -1)
		{
			win(players[0]->getName());
		}
		else
		{
			printf("Remis!!\n");
		}

	}
};

std::ostream &operator<<(std::ostream &out, const Board &board)
{
	for(int i = 0; i < 9; i++)
	{
		if(i % 3 == 0)
		{
			if(i != 0)
			{
				printf("|\n");
			}
			for(int j = 0; j < 7; j++)
			{
				printf("-");
			}

			printf("\n");
		}

		if(board.position[i] == 1)
		{
			printf("|O");
		}
		else if(board.position[i] == -1)
		{
			printf("|X");
		}
		else
		{
			printf("| ");
		}
	}


	printf("|");
	printf("\n");
	for(int j = 0; j < 7; j++)
	{
		printf("-");
	}

	printf("\n");

	return out;
}

int main()
{
	Human dominik("Dominik");
	Computer alterEgo("kinimoD");

	Game game(&dominik, &alterEgo);
	game.run();

	return 0;
}