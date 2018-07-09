#include<iostream>
#include<string>

using namespace std;

#include "solution.h"

void insertionSort(vector tab[], int n)
{
	for(int a = 1; a < n; a++)
	{
		vector c = tab[a];
		int x = a;
		for(; x > 0 && c[0] < tab[x - 1][0]; x--)
		{
			tab[x] = tab[x - 1];
			tab[x - 1] = c;
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	vector data;
	for(int i = 0; i < 8; i++)
	{ data.add("kkk"); }
	for(int i = 0; i < 8; i++)
	{ cout << data[i] << " "; }
	cout << endl;
	vector data2(80);
	data2.resize(8);
	data2[0] = "a";
	data2[1] = "A";
	data2[2] = "b";
	data2[3] = "B";
	data2[4] = "c";
	data2[5] = "d";
	data2[6] = "e";
	data2[7] = "f";
	cout << data2 << " " << data2.getSize() << endl;
	data2.insert(11, "ala");
	data2.insert(8, "koza");
	data2.insert(5, "Viola");
	cout << data2 << " " << data2.getSize() << endl;
	vector data3(data2);
	data3.erase(13);
	data3.erase(5);
	cout << data3 << " " << data3.getSize() << endl;
	cout << data3[5] << endl;
	vector data4(200, 5, "Karolina");
	cout << data4 << " " << data4.getSize() << endl;
	cin >> data4;
	cout << data4 << " " << data4.getSize() << endl;
	data4 = data;
	cout << data4 << " " << data4.getSize() << endl;
	data4.clear();
	data4.insert(0, "worek");
	cout << data4 << " " << data4.getSize() << endl;
	data2 = data2;
	data2[0] = "Tatry";
	cout << data2 << " " << data2.getSize() << endl;
	vector data5 = data2;
	data5.resize(5);
	data5[0] = "abecadlo";
	cout << data5 << " " << data5.getSize() << endl;
	vector tab[6];
	tab[0] = data;
	tab[1] = data2;
	tab[2] = data3;
	tab[3] = data4;
	tab[4] = data5;
	tab[1][0] = "mysza";
	tab[2][0] = "kot";
	insertionSort(tab, 5);
	cout << tab[0] << endl;
	cout << tab[1] << endl;
	cout << tab[2] << endl;
	cout << tab[3] << endl;
	cout << tab[4] << endl;
	cout << data2 << endl;
	return 0;
}