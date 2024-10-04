#include <unordered_map>
#include <string>
#include  <iostream>
#include <memory>
bool IsIsomorphism(const std::string& word1,const std::string& word2){

    std::unordered_map<char ,char> symbols1; //40 байт + в худшем случае будет 2*n байт
    std::unordered_map<char ,char> symbols2; //40 байт + в худшем случае будет 2*n байт
    if (word1.size() != word2.size()){
        return false;
    }

    for (int i = 0;i < static_cast<int>(word1.size());i++){

        if (symbols1.count(word1[i]) == 0){
            if (symbols2.count(word2[i]) != 0){

                return false;
            }
            symbols1[word1[i]] = word2[i];
            symbols2[word2[i]] = word1[i];
        }else{
            if (symbols2.count(word2[i]) == 0){
                return false;
            }
            char s1 = symbols1[word1[i]]; // 1 байт
            char s2 = symbols2[word2[i]]; // 1 байт
            if ((s1 != word2[i]) || (s2 != word1[i])){
                return false;
            }
        }

    }
    return true;
}