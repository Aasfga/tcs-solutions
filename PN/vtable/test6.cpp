#include <iostream>
#include "inject.h"
using namespace std;

struct F
{
    virtual void f() { }
};

struct G : virtual F
{
    virtual void g() { }
};

void callfg(G & g)
{
    g.f();
    g.g();
}

struct Dummy
{
    virtual void dummy() { }
};

struct A : Dummy, virtual G
{
    int calls = 0;
    virtual void f() { cout << "A::f()" << endl; calls++; }
    virtual void g() { cout << "A::g(), calls=" << calls << endl; }
};

struct B : Dummy, virtual G
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
    inject<G>(a, b);
    callfg(a);
    callfg(b);

    return 0;
}
