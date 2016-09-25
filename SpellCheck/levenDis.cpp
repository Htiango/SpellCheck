#include "levenDis.h"


// get the levenshteinDis (use absolute edit distance pruning)
unsigned int levenshteinDis(const string str_input, const string str_tem){

    const size_t len_input = str_input.size(), len_tem = str_tem.size();
    vector<unsigned int > col(len_tem + 1), prevCol(len_tem + 1), colMax(len_tem + 1);
    
    unsigned int temp_up = len_tem - 1;
    unsigned int temp_down = 0;
    
    // initialize the max column
    for (unsigned int i = 0; i < len_tem + 1; i++) {
        colMax[i] = UINT_MAX / 2;
    }
    
    // initialize the first column
    for(unsigned int i = 0; i < len_tem + 1; i++)
    {
        prevCol[i] = i;
        if (prevCol[i] > EDIT_DIS) {
            temp_up = i;
            break;
        }
    }
    
    for(unsigned int i = 0; i < len_input ; i++)
    {
        col = colMax;
        if (temp_down == 0 ) {
            col[0] = i + 1;
            if (col[0] > EDIT_DIS) {
                temp_down = 1;
            }
        }
        else{
            temp_down += 1;
            if (temp_down > len_tem) {
                temp_down = len_tem;
            }
        }
            
        temp_up += 1;
        
        if (temp_down != 0) {
            col[temp_down] = min({prevCol[temp_down] + 1, prevCol[temp_down - 1] + (str_input[i] == str_tem[temp_down - 1] ? 0 : 1)});
        }
//
        unsigned int temp_value = temp_up >= len_tem ? len_tem : temp_up ;
        
        // the loop should only go to the middle part and forget about the upper and down part
        for (unsigned int j = temp_down; j < temp_value; j++) {
            col[j + 1] = min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (str_input[i] == str_tem[j] ? 0 : 1)});
            if (col[j + 1] > EDIT_DIS) {
                if (i > j) {
                    temp_down = j;
                }
                else{
                    temp_up = j;
                    break;
                }
            }
        }
        col.swap(prevCol);
    }
    return prevCol[len_tem];
}


// get the levenshteinDis (use absolute edit distance pruning)
unsigned int beamLevenshtein(const string str_input, const string str_tem){
    
    const size_t len_input = str_input.size(), len_tem = str_tem.size();
    vector<unsigned int > col(len_tem + 1), prevCol(len_tem + 1), colMax(len_tem + 1);
    
    unsigned int temp_up = len_tem - 1;
    
    unsigned int bestCost;
    
    // initialize the max column
    for (unsigned int i = 0; i < len_tem + 1; i++) {
        colMax[i] = UINT_MAX / 2;
    }

    prevCol = colMax;    // initialize the first column
    
    // re-initialize the first column
    for(unsigned int i = 0; i < len_tem + 1; i++)
    {
        if (i > BEAM){
            temp_up = i;
            break;
        }
        prevCol[i] = i;
    }
    
    for(unsigned int i = 0; i < len_input ; i++){
        col = colMax;
        
        col[0] = prevCol[0] + 1;
        bestCost = col[0];
        for (unsigned int j = 0; j < len_tem; j ++) {
            if (j < temp_up + 1){
                col[j + 1] = min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (str_input[i] == str_tem[j] ? 0 : 1)});
            }
            else{
                col[j + 1] = col[j] + 1;
            }
            if (col[j + 1] < bestCost) {
                bestCost = col[j + 1];
            }
            else if (col[j + 1] - bestCost > BEAM){
                temp_up = j + 1;
                break;
            }
        }
        col.swap(prevCol);
    }
    
    
    return prevCol[len_tem];
}

// brute force
unsigned int resursiveLevenshtein(const std::string& s1, const std::string& s2)
{
    const std::size_t len1 = s1.size(), len2 = s2.size();
    std::vector<unsigned int> col(len2+1), prevCol(len2+1);
    
    for (unsigned int i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i+1;
        for (unsigned int j = 0; j < len2; j++)
            // note that std::min({arg1, arg2, arg3}) works only in C++11,
            // for C++98 use std::min(std::min(arg1, arg2), arg3)
            col[j+1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) });
        col.swap(prevCol);
    }
    return prevCol[len2];
}



// do levenshtein distance for multiple words at the same time
// bool -> if true then do beam search, else do edit dis
void multiLe (string& input, vector<string>& tem, map<string, int>& resultMap, bool disType){
    unsigned int temNum = tem.size();
    unsigned int inputLength = input.size();
    
    unsigned int wordLength;
    unsigned int bestCost;
    bool flag = false;
    
    vector<vector<unsigned int>> prevCol, col;
    vector<unsigned int> temp_up, temp_down;
    
    
    // count the longest length of the word in the vector and initialize the first column
    for (unsigned int i = 0; i < temNum; i++) {
        wordLength = tem.at(i).size();
        
        flag = false;
        
        vector<unsigned int> vecMax, vecPrev;
        
        for (unsigned int j = 0; j < wordLength + 1; j ++) {
//            vector<vector<unsigned int>>::const_iterator iter = colMax.begin();
//            colMax.insert(iter + i, )
            
            vecMax.push_back(UINT_MAX/2);
            
            if (disType) {
                if (j > BEAM) {
                    vecPrev.push_back(UINT_MAX/2);
//                    prevCol[i][j] = colMax[i][j];
                    if (!flag) {
                        temp_up.push_back(j);
                        flag = true;
                    }
                   
//                    break;
                }
                else
                    vecPrev.push_back(j);
            }
            else{
                if (j > EDIT_DIS) {
                    vecPrev.push_back(UINT_MAX/2);
                    if (!flag) {
                        temp_up.push_back(j);
                        flag = true;
                    }
//                    break;
                }
                else
                    vecPrev.push_back(j);
            }
        }
//        colMax.push_back(vecMax);
        prevCol.push_back(vecPrev);
        col.push_back(vecMax);
        temp_down.push_back(0);
    }
    
    // do the beam search
    if (disType) {
        for (unsigned int i = 0; i < inputLength; i ++) {
            
            for (unsigned int j = 0; j < temNum; j++) {
                wordLength = tem.at(j).size();

                col[j][0] = prevCol[j][0] + 1;
                bestCost = col[j][0];
                for (unsigned int k = 0; k < wordLength ; k ++) {
                    if (k < temp_up[j] + 1) {
                        col[j][k + 1] = min({prevCol[j][1 + k] + 1, col[j][k] + 1, prevCol[j][k] + (input[i] == tem[j][k] ? 0 : 1)});
                    }
                    else{
                        col[j][k + 1] = col[j][k] + 1;
                    }
                    
                    if (col[j][k + 1] < bestCost) {
                        bestCost = col[j][k + 1];
                    }
                    else if (col[j][k + 1] - bestCost > BEAM){
                        temp_up[j] = k + 1;
                        break;
                    }

                }
                col[j].swap(prevCol[j]);
            }
//            col.swap(prevCol);
        }
    }
    
    // do the edit distance
    else {
        for (unsigned int i = 0; i < inputLength; i ++) {
            
            for (unsigned int j = 0; j < temNum; j++) {
                wordLength = tem.at(j).size();
                
                if (temp_down[j] == 0 ) {
                    col[j][0] = i + 1;
                    if (col[j][0] > EDIT_DIS) {
                        temp_down[j] = 1;
                    }
                }
                else{
                    temp_down[j] += 1;
                    if (temp_down[j] > wordLength) {
                        temp_down[j] = wordLength;
                    }
                }
                
                temp_up[j] += 1;
                
                if (temp_down[j] != 0) {
                    col[j][temp_down[j]] = min({prevCol[j][temp_down[j]] + 1, prevCol[j][temp_down[j] - 1] + (input[i] == tem.at(j)[temp_down[j] - 1] ? 0 : 1)});
                }
                
                unsigned int temp_value = temp_up[j] >= wordLength ? wordLength : temp_up[j] ;
                
                // the loop should only go to the middle part and forget about the upper and down part
                for (unsigned int k = temp_down[j]; k < temp_value; k++) {
                    col[j][k + 1] = min({ prevCol[j][1 + k] + 1, col[j][k] + 1, prevCol[j][k] + (input[i] == tem.at(j)[k] ? 0 : 1)});
                    if (col[j][k + 1] > EDIT_DIS) {
                        if (i > k) {
                            temp_down[j] = k;
                        }
                        else{
                            temp_up[j] = k;
                            break;
                        }
                    }
                }
                col[j].swap(prevCol[j]);
            }
        }
    }
    
    size_t tempId = 0;
    unsigned int minDis = UINT_MAX;
    unsigned int dis;
    
    for (unsigned int i = 0; i < temNum; i ++) {
        wordLength = tem.at(i).size();
        dis = prevCol[i][wordLength];
        if (dis < minDis) {
            tempId = i;
            minDis = dis;
        }
    }
    
    resultMap.insert(pair<string, int>(tem.at(tempId), minDis));
}



