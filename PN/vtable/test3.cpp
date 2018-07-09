#include <iostream>
#include "inject.h"
using namespace std;

struct FG
{
    virtual void f() { }
    virtual void g() { }
};

void callfg(FG & fg)
{
    fg.f();
    fg.g();
}

struct Dummy
{
    virtual void dummy() { }
};

struct A : Dummy, virtual FG
{
    virtual void f() { cout << "A::f()" << endl; }
    virtual void g() { cout << "A::g()" << endl; }
};

struct B : Dummy, virtual FG
{
    virtual void f() { cout << "B::f()" << endl; }
    virtual void g() { cout << "B::g()" << endl; }
};

A a;
B b;

int main()
{
    callfg(a);
    callfg(b);
    inject<FG>(a, b);
    callfg(a);
    callfg(b);
    return 0;
}
