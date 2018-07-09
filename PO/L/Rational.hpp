//
// Created by Dominik Grybos on 07.06.2017.
//

#ifndef L_RATIONAL_HPP
#define L_RATIONAL_HPP

#include <iostream>

template<typename Number>
Number calcGCD(Number first, Number second)
{

	Number zero(0);

	if(first < zero)
		first = -first;
	if(second < zero)
		second = -second;

	while(second != zero)
	{
		Number t = first % second;
		first = second;
		second = t;
	}

	return first;
}

template<typename Number>
Number calcLCM(Number first, Number second)
{
	Number zero(0);

	if(first < zero)
		first = -first;
	if(second < zero)
		second = -second;

	Number gcd = calcGCD(first, second);

	return (first / gcd) * gcd * (second / gcd);
}

template<typename Number>
class Rational
{
	Number nominator;
	Number denominator;


	void minus()
	{
		Number zero(0);
		Number one(-1);

		if(denominator < zero && nominator < zero)
		{
			nominator *= one;
			denominator *= one;
		}
		else if(denominator < zero)
		{
			nominator *= one;
			denominator *= one;
		}
	}

	void normalize()
	{
		Number gcd = calcGCD(nominator, denominator);
		nominator /= gcd;
		denominator /= gcd;
	}

public:

	Rational(const Rational &r) : nominator(r.nominator), denominator(r.denominator)
	{}

	Rational() : nominator(0), denominator(1)
	{}

	Rational(Number n) : nominator(n), denominator(1)
	{}

	Rational(Number n, Number d) : nominator(n), denominator(d)
	{
		normalize();
		minus();
	}

	friend Rational<Number> operator-(const Rational<Number> &n)
	{
		Rational<Number> newNumber(-n.nominator, n.denominator);

		return newNumber;
	}

	friend Rational<Number> operator+(const Rational<Number> &n)
	{
		Rational<Number> newNumber(n);

		return newNumber;
	}

	friend Rational<Number> operator+(const Rational<Number> &f, const Rational<Number> &s)
	{
		Number lcm = calcLCM(f.denominator, s.denominator);
		Number l = (lcm / f.denominator) * f.nominator + (lcm / s.denominator) * s.nominator;
		Rational<Number> newNumber(l, lcm);

		newNumber.normalize();
		newNumber.minus();
		return newNumber;
	}

	friend Rational<Number> operator-(const Rational<Number> &f, const Rational<Number> &s)
	{

		Rational<Number> newNumber = f + (-s);
		return newNumber;
	}

	friend Rational<Number> operator*(const Rational<Number> &f, const Rational<Number> &s)
	{
		Number fGcd = calcGCD(f.nominator, s.denominator);
		Number sGcd = calcGCD(f.denominator, s.nominator);
		Rational<Number> x((f.nominator / fGcd) * (s.nominator / sGcd),
		                   (f.denominator / sGcd) * (s.denominator / fGcd));

		x.normalize();
		x.minus();

		return x;
	}

	friend Rational<Number> operator/(const Rational<Number> &f, const Rational<Number> &s)
	{
		Rational<Number> divider(s.denominator, s.nominator);
		Rational<Number> x = f*divider;
		x.minus();
		x.normalize();
		return x;
	}

	Rational<Number> operator+=(const Rational<Number> &n)
	{
		*this = *this + n;

		normalize();
		minus();
		return *this;
	}

	Rational<Number> operator-=(const Rational<Number> &n)
	{
		*this = *this - n;

		normalize();
		minus();
		return *this;
	}

	Rational<Number> operator*=(const Rational<Number> &n)
	{
		*this = *this * n;

		normalize();
		minus();
		return *this;
	}

	Rational<Number> operator/=(const Rational<Number> &n)
	{
		*this = *this / n;

		normalize();
		minus();
		return *this;
	}

	Rational<Number>& operator=(const Rational<Number> &n)
	{
		nominator = n.nominator;
		denominator = n.denominator;

		return *this;
	}


	friend bool operator<(const Rational<Number> &f, const Rational<Number> &s)
	{
//		Number a = f.nominator;
//		Number b = f.denominator;
//		Number x = s.nominator;
//		Number y = s.denominator;
//		Number zero(0);
//		bool rotate = false;
//
//		while(a != zero && b != zero)
//		{
//			Number fInt = a/b;
//			Number sInt = x/y;
//
//			if(fInt != sInt)
//			{
//				return rotate ? sInt < fInt : fInt < sInt;
//			}
//
//			a = a % b;
//			x = x % y;
//
//			std::swap(a, b);
//			std::swap(x, y);
//			rotate = !rotate;
//		}


		return f.nominator * s.denominator < s.nominator * f.denominator;
	}

	friend bool operator>(const Rational<Number> &f, const Rational<Number> &s)
	{
		return s < f;
	}

	friend bool operator==(const Rational<Number> &f, const Rational<Number> &s)
	{
		return !(f < s) && !(f > s);
	}

	friend bool operator!=(const Rational<Number> &first, const Rational<Number> &second)
	{
		return !(first == second);
	}

	friend bool operator<=(const Rational<Number> &first, const Rational<Number> &second)
	{
		return !(first > second);
	}

	friend bool operator>=(const Rational<Number> &first, const Rational<Number> &second)
	{
		return !(first < second);
	}

	Rational<Number>& operator++() &
	{
		nominator += denominator;

		return *this;
	}

	Rational<Number>& operator--() &
	{
		nominator -= denominator;

		return *this;
	}

	Rational<Number>&& operator++() &&
	{
		return std::move(*this);
	}

	Rational<Number>&& operator--() &&
	{
		return std::move(*this);
	}

	Rational<Number>&& operator++(int) &&
	{
		return std::move(*this);
	}

	Rational<Number>&& operator--(int) &&
	{
		return std::move(*this);
	}

	Rational<Number> operator++(int) &
	{
		Rational<Number> copy = *this;
		nominator += denominator;

		return copy;
	}

	Rational<Number> operator--(int) &
	{
		Rational<Number> copy = *this;
		nominator -= denominator;

		return copy;
	}

	Rational<Number> operator++() const &
	{
		Rational<Number> copy = *this;
		copy++;

		return copy;
	}

	const Rational<Number>& operator++(int) const &
	{
		return *this;
	}


	Rational<Number> operator--() const &
	{
		Rational<Number> copy = *this;
		copy--;

		return copy;
	}

	const Rational<Number>& operator--(int) const &
	{
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &out, const Rational<Number> &rational)
	{
		out << rational.nominator << "/" << rational.denominator;

		return out;
	}
};


#endif //L_RATIONAL_HPP
