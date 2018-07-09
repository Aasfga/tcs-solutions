#include <iostream>
#include <memory>
#include <type_traits>


#define DEBUG

#ifdef DEBUG
class Data
{
public:

	Data() noexcept(false)
	{
		std::cout << "constructor()\n";
	}

	Data(const Data &data) noexcept(false)
	{
		std::cout << "constructor(const TestingData &)\n";
	}

	Data(Data &&data) noexcept(false)
	{
		std::cout << "constructor(TestingData &&)\n";
	}


};
#define D(x) std::cout << x << std::endl;
#define DINFO(x) std::cout << #x << " = " << x << std::endl;
#else
#define D(X)
#define DINFO(x)
#endif

using namespace std;

class DataPair {
private:
    unique_ptr<Data> first_, second_;
public:
    //Copy constructor
    DataPair(DataPair & d) noexcept(is_nothrow_copy_constructible<Data>::value) {
        unique_ptr<Data> p1(new Data(*d.first_)); this->first_ = std::move(p1);
        unique_ptr<Data> p2(new Data(*d.second_)); this->first_ = std::move(p2);
    }

    //Move Constructor
    DataPair(DataPair && d) noexcept(is_nothrow_move_assignable<unique_ptr<Data>>::value) {
        this->first_  = std::move(d.first_);
        this->second_ = std::move(d.second_);
    }

    //Two Arguments Constructors
    DataPair(const Data &a, const Data &b) noexcept(is_nothrow_copy_constructible<Data>::value) {
        unique_ptr<Data> p1(new Data(a)); this->first_ = std::move(p1);
        unique_ptr<Data> p2(new Data(b)); this->first_ = std::move(p2);
    }

    DataPair(Data && a, const Data &b) noexcept(is_nothrow_copy_constructible<Data>::value &&
                                                is_nothrow_move_constructible<Data>::value) {
        this->first_ = unique_ptr<Data>(new Data(std::move(a)));
        unique_ptr<Data> p(new Data(b)); this->first_ = std::move(p);
    }

    DataPair(const Data &a, Data && b) noexcept(is_nothrow_copy_constructible<Data>::value &&
                                                is_nothrow_move_constructible<Data>::value) {
        unique_ptr<Data> p(new Data(a)); this->first_ = std::move(p);
        this->second_ = unique_ptr<Data>(new Data(std::move(b)));
    }

    DataPair(Data && a, Data && b) noexcept(is_nothrow_move_constructible<Data>::value ||
                                            is_nothrow_copy_constructible<Data>::value) {
        this->first_ = unique_ptr<Data>(new Data(std::move(a)));
        this->second_ = unique_ptr<Data>(new Data(std::move(b)));
    }

    // == operators
    DataPair &operator=(const DataPair &d) noexcept(is_nothrow_copy_constructible<Data>::value) {
        return *this;
    }

    DataPair &operator=(DataPair &&d) noexcept(is_nothrow_move_assignable<unique_ptr<Data>>::value) {
        return *this;
    }

    const Data& first() const noexcept {
        return *first_;
    }

    const Data& second() const noexcept {
        return *second_;
    }
};

#ifdef DEBUG
int maina() {
    Data a,b,c,d,e,f;
    cout << "1." << endl;
    DataPair(move(b), move(a));
    cout << "2." << endl;
    DataPair p(c,move(d));
    cout << "3." << endl;
    DataPair q(e,f);
    cout << "4." << endl;
    DataPair r(p);
    cout << "5." << endl;
    DataPair s(move(q));

    cout << boolalpha << "move constructor is noexcept? " << noexcept(DataPair(p)) << endl;
    return 0;
}
#endif
