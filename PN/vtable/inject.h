#ifndef INJECT_H_INCLUDED
#define INJECT_H_INCLUDED

#include <type_traits>
using namespace std;

class MagicDummy
{
    virtual int __magic_dummy()
    {}
};

template <class I, class S, class T,
    typename enable_if<                // enable this template only if:
        is_polymorphic<I>::value &&    // - I has a vtable
        is_base_of<I,S>::value &&      // - S implements I
        is_base_of<I,T>::value,        // - T implements I
    int>::type = 0>
void inject(S & source, T & target)
{
    class MagicUp : public virtual I, public  virtual MagicDummy
    {};
    
    struct MagicDown : public virtual MagicDummy, public virtual I
    {};

    ptrdiff_t **s_ptr = reinterpret_cast<ptrdiff_t**>(static_cast<I*>(&source));
    ptrdiff_t **t_ptr = reinterpret_cast<ptrdiff_t**>(static_cast<I*>(&target));

    
    MagicUp up;
    MagicDown down;
    ptrdiff_t **up_dummy = reinterpret_cast<ptrdiff_t**>(static_cast<MagicDummy*>(&up));
    ptrdiff_t **up_i = reinterpret_cast<ptrdiff_t**>(static_cast<I*>(&up));
    ptrdiff_t **down_dummy = reinterpret_cast<ptrdiff_t**>(static_cast<MagicDummy*>(&down));
    ptrdiff_t **down_i = reinterpret_cast<ptrdiff_t**>(static_cast<I*>(&down));

    int up_size = *down_i - *down_dummy - 1;
    int down_size = *up_dummy - *up_i - 3;
    int size = up_size + down_size;

    *t_ptr = new ptrdiff_t[size];
    ptrdiff_t *begin = *s_ptr - up_size;
    for(int i = 0; i < size; i++)
        (*t_ptr)[i] = begin[i];


    char *call_s = reinterpret_cast<char*>(s_ptr);
    char *call_t = reinterpret_cast<char*>(t_ptr);


    (*t_ptr)[0] += call_s - call_t;
    (*t_ptr)[1] += call_s - call_t;

    *t_ptr += up_size;
}

#endif
