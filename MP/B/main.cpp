#include <iostream>

using namespace std;



double abs(double x)
{
	if(x < 0)
	{
		return -x;
	}
	else
	{
		return x;
	}
}

class Cyclone
{
	int cords[2];
	int velocity[2];

public:

	Cyclone(double x, double y, double vx, double vy)
	{
		cords[0] = x;
		cords[1] = y;
		velocity[0] = vx;
		velocity[1] = vy;
	}

	Cyclone()
	{
		cin >> cords[0] >> cords[1] >> velocity[0] >> velocity[1];
	}

	void print(double t)
	{
		cout << "x = " << cords[0] + t * velocity[0] << ", y = " << cords[1] + t * velocity[1] << "\n";
	}

	void printAll()
	{
		cout << "x = " << cords[0] << ", y = " << cords[1] << ", vx = " << velocity[0] << ", vy = " << velocity[1]
		     << "\n";
	}

	double getX(double t)
	{
		return (double)(cords[0] + (double)(t * velocity[0]));
	}

	double getY(double t)
	{
		return (double)(cords[1] + (double)(t * velocity[1]));
	}

};


class Planet
{

public:
	Cyclone *cyclones;
	int size;

	double maxY(double t)
	{
		double max = cyclones[0].getY(t);

		for(int i = 1; i < size; i++)
		{
			if(cyclones[i].getY(t) > max)
			{
				max = cyclones[i].getY(t);
			}
		}
		return max;
	}

	double minY(double t)
	{
		double min = cyclones[0].getY(t);

		for(int i = 1; i < size; i++)
		{
			if(cyclones[i].getY(t) < min)
			{
				min = cyclones[i].getY(t);
			}
		}
		return min;
	}

	double maxX(double t)
	{
		double max = cyclones[0].getX(t);

		for(int i = 1; i < size; i++)
		{
			if(cyclones[i].getX(t) > max)
			{
				max = cyclones[i].getX(t);
			}
		}
		return max;
	}

	double minX(double t)
	{
		double min = cyclones[0].getX(t);

		for(int i = 1; i < size; i++)
		{
			if(cyclones[i].getX(t) < min)
			{
				min = cyclones[i].getX(t);
			}
		}
		return min;
	}

	double getSide(double t)
	{
		double up = maxY(t);
		double right = maxX(t);
		double down = minY(t);
		double left = minX(t);

		double a = up - down;
		double b = right - left;

		return a > b ? a : b;
	}

	Planet()
	{
		cin >> size;
		cyclones = new Cyclone[size];

	}

	void print()
	{
		for(int i = 0; i < size; i++)
		{
			cyclones[i].printAll();
		}
	}

	void getInitialValues(double *a, double *b)
	{
		int x = 0;
		int y = 1;
		int z = 2;

		double xSide = getSide(x);
		double ySide = getSide(y);
		double zSide = getSide(z);

		while(xSide > ySide && ySide > zSide)
		{
			x = y;
			y = z;
			z = 2 * y;
			xSide = ySide;
			ySide = zSide;
			zSide = getSide(z);
		}

		*a = x;
		*b = z;
	}

	double getTimeOfSmallestSquare()
	{
		double left;
		double right;
		getInitialValues(&left, &right);
		double first = left + (right - left) / 3;
		double second = left + 2 * (right - left) / 3;
		double fSide = getSide(first);
		double sSide = getSide(second);

		do
		{
			if(fSide < sSide)
			{
				right = second;
			}
			else if(fSide > sSide)
			{
				left = first;
			}
			else
			{
				left = first;
				right = second;
			}

			first = left + (right - left) / 3;
			second = left + 2 * (right - left) / 3;
			fSide = getSide(first);
			sSide = getSide(second);

		//} while(abs(lSide - fSide) > (double)1/100000);
		} while(right - left > (double)1/1000000000);

		return first;
	}

	~Planet()
	{
		delete[] cyclones;
	}
};



int maina()
{
	ios_base::sync_with_stdio(false);
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		Planet p;
		double t = p.getTimeOfSmallestSquare();
		cout.precision(10);
		cout<< fixed<< p.getSide(t)<< " "<< t<< "\n";
	}



	return 0;
}