#include <iostream>

template <class T>
class A
{
public:
	template <class U>
	U fun(T t);
};

template <class U, class T>
U A<T>::fun(T t)
{
	return U();
}



int main()
{
	A<int> a;


	int b = a.fun<int>(5);

	std::cout<< b;
}