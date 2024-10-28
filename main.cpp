#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void rand_ch(std::vector<int>& numbers,int k, std::vector<int>& res){
    int n = numbers.size();
    int i = 0;//O(1)
    int k_ = k;//O(1)
    for (int _ = 0 ;_ < k_;_++){//O(K)
        int elem_index = rand() % (n - k + 1) + i;    //O(1)    
        res.push_back(numbers[elem_index]);//O(1)
        k-= 1;//O(1)
        n = numbers.size() - elem_index -1;//O(1)
        i = elem_index + 1;//O(1)
    }

}

bool sr(const std::vector<int>& lhs,const std::vector<int>& rhs){
    for (int i = 0;i < lhs.size();i++){
        if (lhs[i] != rhs[i]){
            return false;
        }
    }
    return true;
}

bool tests(){
    for (int i = 0;i < 20;i++){
        int n = rand() % 100000;
        std::vector<int> a;
        for (int j =0 ;j < n;j++){
            a.push_back(rand() % 100000);

        }
        int k =  1 + rand() % (n-2);
        std::vector<int> res1;
        std::vector<int> res2;
        rand_ch(a,k,res1);
        rand_ch(a,k,res2);
        if ( res1.size() != k || res2.size() != k || sr(res1,res2)){
            return false;
        }
    }
    return true;
}




int main() {
    if (tests()){
        std::cout <<  "OK\n";
    }else{
        assert(false);
    }
    srand(time(nullptr));
    int n;
    cin >> n;
    std::vector<int> numbers;//O(n) памяти для хранение n элементов(читаются и записываются в цикле)
    for (int i = 0;i < n;i++){ // O(n) - чтение n элементов
        int x;
        cin >> x;
        numbers.push_back(x);
    }
    int k; //O(1)
    cin >> k;//O(1)
    std::vector<int> res;//O(k) памяти тк тут будут хранится k элементов
    rand_ch(numbers,k,res);
    for (int i = 0;i < static_cast<int>(res.size());i++){ //O(k) 
        std::cout << res[i] << ' ';
    }
    
    //сложность алгоритма не учитывая чтение и вывод O(k)
    // пространственная сложность алгоритма O(k) не учитывая первоначальный массив


}   
