#include <iostream>
#include <chrono>
#include <cassert>
#include <random>
#include <fstream>
#include <thread>

using namespace std;

class MySorts{
public:
    void CocktailShakerSort(std::vector<int>& v){
        bool flag = true;
        int start = 0;
        int end = v.size() - 1;
        while (flag){ 
            flag = false;

            for (int i = start;i < end;i++){
                if (v[i] > v[i+1]){
                    std::swap(v[i],v[i+1]);
                    flag = true;
                }

            }
            end--;
            for (int i = end;i > start;i--){
                if(v[i] < v[i-1]){
                    std::swap(v[i],v[i-1]);
                }
            }
            start++;

        }
        
        
    } 
    // Памяти O(1) тк я только меняю элеменыт
    // В лучшем случае сложность будет O(n) если он отсортировон, иначе будет O(n^2) 
    

    void BucketSort(std::vector<int>& v){
        if (v.size() == 0){
            return;
        }
        
        std::vector<vector<int>> buckets(v.size()); //O(n) памяти
        int min_value = numeric_limits<int>::max();
        int max_value = numeric_limits<int>::min();
        
        for (int i = 0;i < v.size();i++){ // O(n) сложность
            max_value = std::max(max_value,v[i]);
            min_value = std::min(min_value,v[i]);
        }
        if (min_value == max_value){
            return;
        } 
        for (int i = 0;i < v.size();i++){// O(n) сложность
            int index_bucket = v.size() * (v[i] - min_value) / (max_value - min_value + 1);
            buckets[index_bucket].push_back(v[i]);
        }
        v.clear();
        for (int i = 0;i < buckets.size();i++){// O(n) сложность
            BucketSort(buckets[i]);// в худшем случает O(n), в лучшем O(1), в среднем (log(max-min))
            for (int j = 0;j < buckets[i].size();j++){
                v.push_back(buckets[i][j]);
            }
        }

    }
    // Памяти O(nlog(max-min)) из за рекурсии(на каждом ветлениее мы бдуем опять сохранять элементы)
    // Работает за O(nlog(max-min)). В лучшем случае за O(n) если n > max - min.
    // Худшего случая нет. Но чем больше разница max - min тем дольше будет работать код


    void TreeSort(std::vector<int>& v){
        Node* root = nullptr;// будет O(n) памяти тк он еще раз сохоранит каждый элемент
        for (int i = 0;i < v.size();i++){// O(n) сложность
            root = Insert(root,v[i]); // O(log(n)) сложность
        }
        v.clear();
        GetSortVector(v,root);// O(n) сложность
    }
    // Памяти O(n) 
    // Работает за O(n^2) в лучшем и среднем случае случае за O(nlogn)
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* Insert(Node* root, int value){
        
        if (root == nullptr){
            return new Node(value);
        }
        if (value < root->value){
            root->left = Insert(root->left,value);
        }else{
            root->right = Insert(root->right,value);
        }
        return root;
    }

    void GetSortVector(std::vector<int>& res,Node* root){

        if (root != nullptr){
            GetSortVector(res,root->left);
            res.push_back(root->value);
            GetSortVector(res,root->right);
        }
    }

};




void TestSortFunction(std::string sort_name) {
    std::vector<std::vector<int>> tests = {
        {},                       
        {1},                      
        {5, 3, 8, 1, 9},          
        {1, 2, 3, 4, 5},          
        {5, 4, 3, 2, 1},          
        {3, 1, 2, 1, 3},     
        {4,5,-3,-1,0}     
    };

    for (int i = 0; i < tests.size();i++) {
        std::vector<int> arr = tests[i];
        MySorts my_sorts;
        if (sort_name == "T"){
            my_sorts.TreeSort(arr);
        }else if (sort_name == "B"){
            my_sorts.BucketSort(arr);
        }else if (sort_name == "C"){
            my_sorts.CocktailShakerSort(arr);
        }else{
            assert(false);
        }
        
        assert(std::is_sorted(arr.begin(),arr.end()));
          
    }
    srand(time(nullptr));
    for (int i = 0;i < 10;i++){
        int n = rand() % 10000;
        std::vector<int> arr;
        for (int j =0 ;j < n;j++){
            arr.push_back(rand() % 100000);

        }
        MySorts my_sorts;
        if (sort_name == "T"){
            my_sorts.TreeSort(arr);
        }else if (sort_name == "B"){
            my_sorts.BucketSort(arr);
        }else if (sort_name == "C"){
            my_sorts.CocktailShakerSort(arr);
        }else{
            assert(false);
        }
        assert(std::is_sorted(arr.begin(),arr.end()));

    }
    std::cout << sort_name << " is OK\n";

    
}

void Tests(){
    TestSortFunction("C");
    TestSortFunction("T");
    TestSortFunction("B");
    std::cout << "OK\n";
}
void TestTime(std::string sort_name){
    srand(time(nullptr));
    //std::string name_f1 = sort_name + "_seconds.txt";
    //std::ofstream file1(name_f1, std::ios::app);
 
    for (int i =1000;i <=1000000;i+= 1000){
        std::vector<int> arr;
        for (int j =0 ;j < i;j++){
            arr.push_back(rand() % 100000);
        }
        MySorts my_sorts;
        if (sort_name == "T"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.TreeSort(arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start); 
            std::string x =std::to_string(duration.count() / 1000.0);    
            std::cout << "Time - " << duration.count() << " milliseconds for size = " << i <<"\n";
            //file1 << x << '\n';
        }else if (sort_name == "B"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.BucketSort(arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);          
            std::string x =std::to_string(duration.count() / 1000.0);      
            std::cout << "Time - " << duration.count() << " milliseconds for size = " << i << "\n";
            //file1 << x << '\n';
        }else if (sort_name == "C"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.CocktailShakerSort(arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(end - start);            
            std::string x = std::to_string(duration.count());    
            std::cout << "Time - " << duration.count() << " seconds for size = " << i << "\n";
            //file1 << x << '\n';
        }else{
            assert(false);
        }
        assert(std::is_sorted(arr.begin(),arr.end()));
    }
    //file1.close();
    std::cout << "Test Time is finish\n";

}

struct Data
{
    std::vector<int>& arr;
    std::ofstream& out;    
    std::string name;
};

void TestTimeThreads(Data data) {
    MySorts my_sorts;
        if (data.name == "T"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.TreeSort(data.arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start); 
            std::string x =std::to_string(duration.count() / 1000.0);    
            std::cout << "Time - " << duration.count() << " milliseconds for size = " << data.arr.size() <<"\n";
            data.out << x << '\n';
        }else if (data.name == "B"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.BucketSort(data.arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);          
            std::string x =std::to_string(duration.count() / 1000.0);      
            std::cout << "Time - " << duration.count() << " milliseconds for size = " << data.arr.size() << "\n";
            data.out << x << '\n';
        }else if (data.name == "C"){
            auto start = chrono::high_resolution_clock::now();
            my_sorts.CocktailShakerSort(data.arr);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
            std::string x = std::to_string(duration.count() / 1000.0);    
            std::cout << "Time - " << duration.count() << " milliseconds for size = " << data.arr.size() << "\n";
            data.out << x << '\n';
        }else{
            assert(false);
        }
    
}

void TestForBoxPlot(int v_size, std::string sort_name){
    std::string name_f1 = sort_name + "_seconds_for_box.txt";
    std::ofstream file1(name_f1, std::ios::app);
    for (int i = 0;i < 7;i++){
        int Threads = 8;
        std::vector<std::thread> threads;
        srand(time(nullptr) + i);
        std::vector<vector<int>> arrs(8);
      
        for (int j = 0;j < 8;j++){
            for (int j1 = 0;j1 < v_size;j1++){
                arrs[j].push_back(rand() % 100000);
            }
        }
        
           
        for (int j = 0; j < Threads; j++) {
            Data data = {arrs[j],file1,sort_name};
            threads.emplace_back(TestTimeThreads, data);
        }


        for (auto &t : threads) {
            t.join();
        }
        

    }
    file1.close();
}

void TestCaseCoctail(){
    std::vector<int> best;
    std::vector<int> bad;
    for (int i = 0;i < 10000;i++){
        best.push_back(i);
        bad.push_back(10000 - i);
    }
    MySorts my_sorts;
    auto start = chrono::high_resolution_clock::now();
    my_sorts.CocktailShakerSort(best);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    std::string x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time best case - " << x << " second for Coctail\n";

    start = chrono::high_resolution_clock::now();
    my_sorts.CocktailShakerSort(bad);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time bad case - " << x << " second for Coctail\n";
  
}

void TestCaseTree(){
    std::vector<int> best;
    std::vector<int> bad;
    for (int i = 0;i < 10000;i++){
        best.push_back(rand() % 10000);
        bad.push_back(i);
    }
    MySorts my_sorts;
    auto start = chrono::high_resolution_clock::now();
    my_sorts.TreeSort(best);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    std::string x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time best case - " << x << " second for Tree\n";

    start = chrono::high_resolution_clock::now();
    my_sorts.TreeSort(bad);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time bad case - " << x << " second for Tree\n";
  
}

void TestCaseBucket(){
    std::vector<int> best;
    std::vector<int> bad;
    bad.push_back(0);
    bad.push_back(10000);
    for (int i = 0;i < 10000;i++){
        best.push_back(rand() % 10);
        bad.push_back(rand() % 10000000);
    }
    MySorts my_sorts;
    auto start = chrono::high_resolution_clock::now();
    my_sorts.BucketSort(best);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    std::string x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time best case - " << x << " second for Bucket\n";

    start = chrono::high_resolution_clock::now();
    my_sorts.BucketSort(bad);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);            
    x = std::to_string(duration.count() / 1000.0);    
    std::cout << "Time bad case - " << x << " second for Bucket\n";
  
}





int main(){
    Tests();
    //TestTime("C");
    TestForBoxPlot(100000,"C");
    //TestCaseCoctail();
    //TestCaseTree();
    //TestCaseBucket();

    return 0;
}