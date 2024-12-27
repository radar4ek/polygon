
#include <iostream>
#include <vector>
using namespace std;

int longestDecomposition(string text) {
    int left = 0;
    int right = text.size() - 1;
    int ans = 0;
    string_view te(text);
    // Весь цикл O(n^2)
    while (left <= right) { // O(n)  
        bool flag = false;
    
        for (int i = 1; i <= (right - left + 1) / 2; i++) { // O(n)
            if (te.substr(left, i) == te.substr(right - i + 1, i)) { //Пытаемся взвять как можно меньше
                ans += 2;
                left += i;
                right -= i;
                flag = true;
                break;
            }
        }
        if (!flag){// если не смогли выделить пару, то заканчиваем цикл
            break; 
        }
    }
    if (left <= right){// так останется центральный обьект
        ans += 1;
    }
    return ans;
}
//Общая сложность O(n^2)
//Память будет O(1) тк string_view хрнаит указатель а остльные элементы алгоритма это O(1) памяти

int main(){
    std::string x;
    std::cin >> x;
    std::cout << longestDecomposition(x);
}

