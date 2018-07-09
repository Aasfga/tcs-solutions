#include <iostream>

using namespace std;

int lastX[1000];
int lastY[1000];
int tmpX[1000];
int tmpY[1000];
int lastSize = 0;

class Board
{
	int **board;
	int height;
	int width;


	void setWalls()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				if(i == 0 || j == 0 || i == height - 1 || j == width - 1)
				{
					board[i][j] = 4;
				}
				else
				{
					board[i][j] = 0;
				}
			}
		}
	}

public:

	void setWall(int y, int x)
	{
		board[y][x] += 4;
	}

	void destroyWall(int y, int x)
	{
		board[y][x] -= 4;
	}

	friend ostream &operator<<(ostream &out, const Board &aBoard);


	void readBoard(int *mouseY, int *mouseX)
	{
		int c, k, cheeseX, cheeseY;

		scanf("%i%i%i%i%i%i", &k, &c, mouseY, mouseX, &cheeseY, &cheeseX);
		k = 1 << k;
		for(int i = 1; i < height - 1; i++)
		{
			for(int j = 1; j < width - 1; j++)
			{
				int marker;
				scanf("%i", &marker);
				board[i][j] = k & marker;
				board[i][j] *= 4;
			}
		}

		for(int i = 1; i < height - 1; i++)
		{
			for(int j = 1; j < width - 1; j++)
			{
				int marker;
				scanf("%i", &marker);

				if(marker == 0)
				{
					board[i][j] += 2;
				}
				if(marker == c)
				{
					board[i][j] += 1;
				}
			}
		}

		board[cheeseY][cheeseX] = 3;
	}

	Board()
	{
		scanf("%i %i", &height, &width);
		height += 2;
		width += 2;

		board = new int *[height];

		for(int i = 0; i < height; i++)
		{
			board[i] = new int[width];
		}

		setWalls();
	}


	~Board()
	{
		for(int i = 0; i < height; i++)
		{
			delete board[i];
		}

		delete board;
	}


	int howManyPossibleMoves(int mY, int mX, int hat, int size)
	{
		if(board[mY][mX] == 3)
		{
			tmpX[size] = mX;
			tmpY[size] = mY;
			size++;
			for(int i = 0; i < size; i++)
			{
				lastX[i] = tmpX[i];
				lastY[i] = tmpY[i];
			}

			lastSize = size;

			return 1;
		}
		else if(board[mY][mX] >= 4)
		{
			return 0;
		}
		else if(board[mY][mX] == 1)
		{
			if(hat == 0)
			{
				return 0;
			}
			else
			{
				hat--;
			}
		}
		else if(board[mY][mX] == 2)
		{
			hat++;
		}

		int answer = 0;
		setWall(mY, mX);

		tmpX[size] = mX;
		tmpY[size] = mY;

		answer += howManyPossibleMoves(mY, mX + 1, hat, size + 1);
		answer += howManyPossibleMoves(mY + 1, mX, hat, size + 1);
		answer += howManyPossibleMoves(mY, mX - 1, hat, size + 1);
		answer += howManyPossibleMoves(mY - 1, mX, hat, size + 1);

		destroyWall(mY, mX);

		return answer;
	}

};

ostream &operator<<(ostream &out, const Board &aBoard)
{
	for(int i = 0; i < aBoard.height; i++)
	{
		for(int j = 0; j < aBoard.width; j++)
		{
			if(aBoard.board[i][j] == 0)
			{
				printf(" ");
			}
			else if(aBoard.board[i][j] == 1)
			{
				printf("C");
			}
			else if(aBoard.board[i][j] == 2)
			{
				printf("H");
			}
			else if(aBoard.board[i][j] == 3)
			{
				printf("S");
			}
			else
			{
				printf("X");
			}
		}

		printf("\n");
	}

	return out;
}


int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		Board board;
		int mX, mY;
		board.readBoard(&mY, &mX);

		int ans = board.howManyPossibleMoves(mY, mX, 0, 0);

		if(ans == 0)
		{
			printf("NIE\n");
		}
		else
		{
			printf("TAK %i\n%i\n", ans, lastSize);
			for(int i = 0; i < lastSize - 1; i++)
				printf("%i %i, ", lastY[i], lastX[i]);

			printf("%i %i\n", lastY[lastSize - 1], lastX[lastSize - 1]);

		}
	}
	/*
	Board board;
	int a, b;
	board.readBoard(&a, &b);
	cout << board;
	printf("%i\n", board.howManyPossibleMoves(a, b, 0, 0));
	for(int i = 0; i < lastSize - 1; i++)
		printf("%i %i,", lastY[i], lastX[i]);

	printf("%i %i", lastY[lastSize - 1], lastX[lastSize - 1]);
*/
	return 0;
}