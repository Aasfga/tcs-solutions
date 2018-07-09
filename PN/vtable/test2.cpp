#include <iostream>
#include "inject.h"
using namespace std;

struct F
{
    virtual void f() { }
};

void callf(F & f)
{
    f.f();
}

struct G
{
    virtual void g() { }
};

void callg(G & g)
{
    g.g();
}

struct Dummy
{
    virtual void dummy() { }
};

struct A : Dummy, virtual F, virtual G
{
    virtual void f() { cout << "A::f()" << endl; }
    virtual void g() { cout << "A::g()" << endl; }
};

struct B : Dummy, virtual F, virtual G
{
    virtual void f() { cout << "B::f()" << endl; }
    virtual void g() { cout << "B::g()" << endl; }
};

A a;
B b;

int main()
{
    callf(a);
    callg(a);
    callf(b);
    callg(b);
    inject<F>(a, b);
    inject<G>(b, a);
    callf(a);
    callg(a);
    callf(b);
    callg(b);
    return 0;
}
