#include <iostream>

using namespace std;


int search(string text, string pattern);

string retrieveString(string pattern)
{
	string ans = "";

	int size = pattern.size();
	for(int i = 0; i < size && pattern[i] != '?' && pattern[i] != '*'; i++)
	{
		ans += pattern[i];
	}

	return ans;
}


bool isEqual(char a, char b)
{
	if(b == '?' && a != ' ')
	{
		return 1;
	}

	if(a == b)
	{
		return 1;
	}

	if(b == '*')
	{
		return 1;
	}

	return 0;
}

int countStars(string text)
{
	int ans = 0;

	for(int i = 0; i < text.length(); i++)
	{
		if(text[i] == '*')
			ans++;
	}

	ans;
}

bool onlyStars(int i, string text)
{
	for(; i < text.size(); i++)
	{
		if(text[i] != '*')
			return 0;
	}

	return 1;
}

int isPattern(string text, string pattern)
{

	/*
	if(pattern.size() > text.size() + countStars(pattern))
	{
		return -1;
	}
*/

	int i;
	for( i = 0; i < pattern.size() && i < text.size(); i++)
	{
		if(pattern[i] == '*')
		{
			int space = text.find(' ', i);

			if(space != string::npos)
				text.erase(space);


			int star = search(text.substr(i), pattern.substr(i + 1));
			if(star >= 0)
			{
				return 1;
			}
			else
			{
				return -1;
			}

		}
		else if(!isEqual(text[i], pattern[i]))
		{
			return -1;
		}
	}

	if(i == text.size() && i != pattern.size() && !onlyStars(i, pattern))
		return -1;

	return 1;
}



int search(string text, string pattern)
{




	int ans = -1;
	for(int i = 0, size = text.size(); i <= size && ans == -1; i++)
	{
		if(isEqual(text[i], pattern[0]))
		{
			ans = isPattern(text.substr(i), pattern);
			if(ans > 0)
			{
				return i;
			}
		}
	}

	return ans;
}

int main()
{

	ios_base::sync_with_stdio(false);

	string pattern;
	string data;
	int sets;

	cin>> sets;
	getline(cin, data);

	while(sets--)
	{
		getline(cin, data);
		getline(cin, pattern);

		int ans = search(data, pattern);

		if(ans < 0)
			cout<< "NIE\n";
		else
			cout<< "TAK " << ans << "\n";
	}


//	string data = "ab cd";
//	cout<<  data.find('c', 3) << "xxx"<<endl;

	return 0;
}