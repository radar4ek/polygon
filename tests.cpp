#include "lab.h"
#include <cassert>
#include <iostream>
#include <random>
void test_long_string(){
    assert(IsIsomorphism("egg","add"));
    assert(!IsIsomorphism("egg","abd"));
    assert(!IsIsomorphism("egg","a"));
    assert(!IsIsomorphism("e","abd"));
    assert(!IsIsomorphism("bar","foo"));
    assert(IsIsomorphism("aaaaaaaaaaa","bbbbbbbbbbb"));
    assert(!IsIsomorphism("aaaaaaaaaaa","bbbbbbbbbba"));
    assert(IsIsomorphism("0123456789","9876543210"));
    assert(!IsIsomorphism("0123456789","0876543210"));
    assert(IsIsomorphism("bob2005","pop1007"));
    std::string w1,w2;
    for (int i = 0;i < 10000000;i++){
        char x = (rand() + 97) % 122;
        w1 += x;
        w2 += (x+3) % 122;
    }
    assert(IsIsomorphism(w1,w2));
    std::string w12,w22;
    for (int i = 0;i < 10000000;i++){

        w1 += (rand() + 97) % 122;
        w2 += (rand() + 97) % 122;

    }
    assert(!IsIsomorphism(w1,w2));
    std::cout << "OK!";

}
