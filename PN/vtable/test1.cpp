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

struct Dummy
{
    virtual void dummy() { }
};

struct A : Dummy, virtual F
{
    virtual void f() { cout << "A::f()" << endl; }
};

struct B : Dummy, virtual F
{
    virtual void f() { cout << "B::f()" << endl; }
};

A a;
B b;

int main()
{
    callf(a);
    callf(b);
    inject<F>(a, b);
    callf(a);
    callf(b);
    return 0;
}
