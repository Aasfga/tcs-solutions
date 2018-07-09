#include <iostream>
#include "solutions.h"

using namespace std;


void insertData(queue &q, int n, int start = 0)
{
	for(int i = start; i < n + start; i++)
	{
		point p(i, i);
		q.enqueue(p);
	}
}

void deleteData(queue &q, int n)
{
	for(int i = 0; i < n; i++)
		q.dequeue();
}

int main()
{
	queue q;

	insertData(q, 20);
	q.print();
	deleteData(q, 10);
	q.print();
	insertData(q, 5, 15);
	q.print();
	q.resize(20);
	q.print();
	insertData(q, 5, 20);
	q.print();


	return 0;
}