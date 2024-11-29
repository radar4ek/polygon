
#include <iostream>
#include <chrono>
#include <cassert>
#include <random>
#include <fstream>
#include <thread>
#include <set>
#include <map>
using namespace std;

    bool canCross(vector<int>& stones) {
        if (stones[1] > 1){
            return false;
        }
        std::map<int,std::set<int>> dp;
        
        dp[1].insert(1);
        for (auto stone : stones){
            for (auto jump : dp[stone]){
                for (int i = -1;i <= 1;i++){
                    dp[stone + jump + i].insert(jump + i);
                }
            }
        }
        for (auto x : dp){
            std::cout << x.first << ":\n";
            for (auto i : x.second){
                std::cout << i << " ";
            }
            std::cout << '\n' << '\n';
        }
        return dp[stones[stones.size() - 1]].size() != 0;
    }

int main(){
    std::vector<int> x = {0,1,3,5,6,8,12,17};
    std::vector<int> y = {0,1,2,3,4,8,9,11};

    bool l = canCross(x);
    std::cout << l;
}

