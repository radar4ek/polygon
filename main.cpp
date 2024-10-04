#include <iostream>
#include "tests.h"
#include "lab.h"






// n - длинна строки
int main() {

    test_long_string();
    std::string w1,w2; // w1 - n байт, w2 - n байт
    while (std::cin >> w1 >> w2){

        if (IsIsomorphism(w1,w2)){
            std::cout << "true\n";
        } else{
            std::cout << "false\n";
        }
    }
    return 0;
}

