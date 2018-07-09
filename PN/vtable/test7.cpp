#include <iostream>
#include "inject.h"
using namespace std;

struct Dummy
{
    virtual void dummy() { }
};

struct F
{
    virtual void f() { }
};

struct G : Dummy, virtual F
{
    virtual void g() { }
};

void callfg(G & g)
{
    g.f();
    g.g();
}

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
    inject<F>(a, b);
    callfg(a);
    callfg(b);
    return 0;
}
