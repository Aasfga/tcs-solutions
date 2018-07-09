#include <iostream>
#include <memory>
#include <type_traits>

//#define moje


#ifdef moje
class Data
{
public:
	int A;

	Data(int a) noexcept(false)
	{
		A = a;
		std::cout << "constructor()\n";
	}

	Data(const Data &data) noexcept(false)
	{
		A = data.A;
		std::cout << "constructor(const TestingData &)\n";
	}

	Data(Data &&data) noexcept(true)
	{
		A = data.A;
		std::cout << "constructor(TestingData &&)\n";
	}


};
#endif


using namespace std;

class DataPair
{
private:
	unique_ptr<Data> first_, second_;

public:
//noexcept(is_nothrow_copy_constructible<Data>::value);


	DataPair(const Data &f, const Data &s) noexcept(is_nothrow_copy_constructible<Data>::value)
	{
		unique_ptr<Data> p(new Data(f));
		unique_ptr<Data> q(new Data(s));
		first_ = move(p);
		second_ = move(q);
	}

	DataPair(const Data &f, Data &&s) noexcept(is_nothrow_copy_constructible<Data>::value &&
	                                           is_nothrow_move_constructible<Data>::value)
	{
		unique_ptr<Data> p(new Data(f));
		unique_ptr<Data> q(new Data(move(s)));

		first_ = move(p);
		second_ = move(q);
	}

	DataPair(Data &&f, const Data &s) noexcept(is_nothrow_copy_constructible<Data>::value &&
	                                           is_nothrow_move_constructible<Data>::value)
	{
		unique_ptr<Data> q(new Data(s));
		unique_ptr<Data> p(new Data(move(f)));

		first_ = move(p);
		second_ = move(q);

	}

	DataPair(Data &&f, Data &&s) noexcept(is_nothrow_move_constructible<Data>::value ||
	                                      is_nothrow_copy_constructible<Data>::value)
	{
		if(!is_nothrow_move_constructible<Data>::value)
		{
			unique_ptr<Data> p(new Data(f));
			unique_ptr<Data> q(new Data(s));
			first_ = move(p);
			second_ = move(q);
		}
		else
		{
			unique_ptr<Data> p(new Data(move(f)));
			unique_ptr<Data> q(new Data(move(s)));

			first_ = move(p);
			second_ = move(q);
		}
	}

	DataPair(const DataPair &pair) noexcept(is_nothrow_copy_constructible<Data>::value)
	{
		unique_ptr<Data> p(new Data(*pair.first_));
		unique_ptr<Data> q(new Data(*pair.second_));

		first_ = move(p);
		second_ = move(q);
	}

	DataPair(DataPair &&pair) noexcept(is_nothrow_move_assignable<unique_ptr<Data>>::value)
	{

		first_ = move(pair.first_);
		second_ = move(pair.second_);
	}

	DataPair &operator=(const DataPair &pair) noexcept(is_nothrow_copy_constructible<Data>::value)
	{

		unique_ptr<Data> p(new Data(*pair.first_));
		unique_ptr<Data> q(new Data(*pair.second_));

		first_ = move(p);
		second_ = move(q);
		return *this;
	}

	DataPair &operator=(DataPair &&pair) noexcept
	{

		first_ = move(pair.first_);
		second_ = move(pair.second_);

		return *this;
	}
	//operator= najpierw kopiuje, a jezeli sie uda to przenosze


	const Data &first() const noexcept
	{
		return *first_.get();
	}

	const Data &second() const noexcept
	{
		return *second_.get();
	}
};


#ifdef moje
int main()
{

	Data a(0);
	Data b(1);
	Data c = move(b);
	DataPair q(a, b);
	DataPair p = q;
	cout<< p.first().A<< p.second().A << "\n";
	return 0;
}
#endif
