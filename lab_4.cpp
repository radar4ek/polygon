#include <iostream>
#include <chrono>
#include <map>
/*
10
1
2
3
4
5
6
7
8
9
10
*/

using namespace std;



bool Next_mask(std::vector<int>& mask){
    mask[0] += 1;
    for (int i = 0;i < (mask.size()-1);i++){
        if (mask[i] == 2){
            mask[i] = 0;
            mask[i+1]+= 1;
        }
    }
    if (mask[mask.size()-1] == 2){
        return false;
    }
    return true;
}


std::vector<vector<double>> Сlustering(std::vector<double>& numbers,int k){

    std::vector<std::pair<double,int>>  sorted_numbers; //O(n) памяти 
    std::map<double,std::vector<int>> number_indexs;//O(n) памяти
    for (int i = 0; i < numbers.size();i++){ // Сложность O(n)
        if(number_indexs.count(numbers[i]) == 0){
            sorted_numbers.push_back({numbers[i],0});
            number_indexs[numbers[i]].push_back(i);
        }else{
            sorted_numbers.push_back({numbers[i],number_indexs[numbers[i]].size()});
            number_indexs[numbers[i]].push_back(i);
        }
       
        
    }
    std::sort(sorted_numbers.begin(),sorted_numbers.end());// Сложность O(n* logn)

    double min_metric = numeric_limits<double>::max();
    std::vector<vector<std::pair<double,int>>> result_pairs; //O(n) памяти
    std::vector<int> mask(sorted_numbers.size()-1,0);//O(n) памяти
    bool end_flag = true;
    while (end_flag) // Сложность O(2^n)
    {
        
        if(count(mask.begin(),mask.end(),1) != k-1){
            end_flag = Next_mask(mask);
            continue;
        }
        std::vector<vector<std::pair<double,int>>> expected_cluster(k);//O(n) памяти
        int cluster_index = 0;
        for (int i = 0;i < sorted_numbers.size();i++){// Сложность O(n)
            expected_cluster[cluster_index].push_back(sorted_numbers[i]);
            if (mask[i] == 1){
                cluster_index+= 1;
            }
        }

        double now_metric = 0;
        for (int i = 0; i < k;i++){// Сложность O(n) тк мы переберем n элементов
            double arithmetic_mean = 0;
            for (int j = 0; j < expected_cluster[i].size();j++){
                arithmetic_mean += expected_cluster[i][j].first;
            }
            arithmetic_mean = arithmetic_mean / expected_cluster[i].size();
            for (int j = 0; j < expected_cluster[i].size();j++){
                now_metric += std::abs(arithmetic_mean - expected_cluster[i][j].first);
            }    
        }
        
        if (now_metric < min_metric){
            result_pairs = expected_cluster;
            min_metric = now_metric;
        }
        end_flag = Next_mask(mask);
        
    }
    std::vector<vector<double>> result(k); //O(n) памяти
    for (int i = 0; i < k;i++){// Сложность O(k)
        std::sort(result_pairs[i].begin(),result_pairs[i].end(),[number_indexs](std::pair<double,int> lhs,std::pair<double,int> rhs)
            {return number_indexs.at(lhs.first)[lhs.second] < number_indexs.at(rhs.first)[rhs.second];});// Сложность O(k* logk)
    }
    std::sort(result_pairs.begin(),result_pairs.end(),[number_indexs](std::vector<std::pair<double,int>> lhs,std::vector<std::pair<double,int>> rhs)
        {return number_indexs.at(lhs[0].first)[lhs[0].second] < number_indexs.at(rhs[0].first)[rhs[0].second];});// Сложность O(k* logk)

    for (int i = 0; i < k;i++){// Сложность O(n) тк мы скопируем n элементов
        for (int j = 0;j < result_pairs[i].size();j++){
            result[i].push_back(result_pairs[i][j].first);
        }
    }
    

    return result;

}
/*
Итог
Асимптотика будет O(2^n*n)
Сложность памяти будет O(n)
*/


int main(){
    int n;
    cin >> n;
    std::vector<double> v;
    for (int i = 0; i < n;i++){
        double x;
        cin >> x;
        v.push_back(x);
       
        
    }
    int k;
    cin >> k;

    auto start_timer = chrono::high_resolution_clock::now();
    vector<vector<double>> result = Сlustering(v,k);
    auto end_timer = chrono::high_resolution_clock::now();
    std::cout << "Clusters:\n";
    for (int i = 0;i < result.size();i++){
        for (int j = 0;j < result[i].size();j++){
            std::cout << result[i][j] << ' ';
        }
        std::cout << '\n';
    }

    auto duration_in_seconds = chrono::duration_cast<chrono::seconds>(end_timer - start_timer);
    auto duration_in_milliseconds = chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
    
    std::cout << "Time - " << duration_in_seconds.count() << " seconds or " << duration_in_milliseconds.count() << "milliseconds\n";

    return 0;
}