//
//  main.cpp
//  SpellCheck
//
//  Created by hty on 9/21/16.
//  Copyright © 2016 hty. All rights reserved.
//

#include "levenDis.h"
#include "fileRead.h"
#include "printPath.h"

using namespace std;

void filePreProcessing();
void part3(bool disType);
void part2(vector<string>& temp, string& str, bool disType);
void part1(string& s1, string& s2, bool disType);
void part4();

const string filePath = "/Users/hty/Desktop/Speech Recognition/project/project3/";
const string storyName = "story.txt";
const string pureStoryName = "pureStory.txt";
const string dictName = "dict.txt";
const string storyCorrectName = "storycorrect.txt";
const string pureStoryCorrectName = "pureStoryCorrect.txt";


vector<string> story;
vector<string> dict;
vector<string> storyCorrect;


//edit distance of input and dict word
unsigned int distances[STORY_SIZE];
//mapping of dict word id to input word id
int distanceId[STORY_SIZE];


int main(int argc, const char * argv[]) {
    
//    multiConduct(true);   // multi beam search
//    multiConduct(false);   // multi edit distance
    readFile(filePath + pureStoryName , story);
    readFile(filePath + pureStoryCorrectName, storyCorrect);
    readFile(filePath + dictName, dict);

 
    cout << "-------------------------------PART1----------------------------------" << endl;
    cout << "HERE IS PART1!" << endl;
    string s1 = "helloyo";
    string s2 = "helloyour";
    part1(s1, s2, true);
    
    cout << "-------------------------------PART2----------------------------------" << endl;
    cout << "HERE IS PART2!" << endl;
    vector<string> temp = {"elephant", "elegant", "sycophant"};
    string str = "eleaphent";
    part2(temp, str, true);
    
    cout << "-------------------------------PART3----------------------------------" << endl;
    cout << "HERE IS PART3!" << endl;
    part3(true);
    
    cout << "-------------------------------PART4----------------------------------" << endl;
    cout << "HERE IS PART4!" << endl;
    part4();
    
    
//    ofstream out(filePath + "name.txt");
//    print("helloyo", "helloyour", out);
    
    return 0;
}


// remove all punctuation markers
void filePreProcessing(){
    convertFile((filePath + storyName), (filePath + pureStoryName));
    convertFile((filePath + storyCorrectName), (filePath + pureStoryCorrectName));
}


// compute the Levenshtein distance between two symbol strings and Display the trellis, the scores and the best path in table format
// bool -> if true then do beam search, else do edit dis
void part1(string& s1, string& s2, bool disType){
    unsigned int result;
    if (disType) {
        result = beamLevenshtein(s1, s2);
    }
    else
        result = levenshteinDis(s1, s2);
    cout << "The best score is " << result << endl;
    cout << "Now let's display the trellis, the scores and the best path(with *)."<< endl;
    
    ofstream out(filePath + "printPath.txt");
    print(s1, s2, out);
    cout << endl;
    out.close();
}


// do the multiple words' levenshtein distance simultaneously
void part2(vector<string>& temp, string& str, bool disType){
    map<string, int> resultMap;
    map <string, int>::iterator resultIter;
    vector<string> strStore(temp.size());
    
    
    multiLe(str, temp, resultMap, disType);
    
    int i = 0;
    for (resultIter = resultMap.begin(); resultIter != resultMap.end(); resultIter++) {
        cout << "The best fit template is '" << resultIter->first << "' with a best score " << resultIter->second << endl;
        cout << endl;
        strStore[i] = resultIter->first;
        i++;
    }
    
    int j = 0;
    ofstream out(filePath + "printPath.txt");
    for (int k = 0; k < temp.size(); k++) {
        cout << "Template " << k << endl;
        if (temp[k] != strStore[j]) {
            printSinglePath(str, temp[k], filePath, "path.txt");
        }
        else{
            print(str, temp[k], out);
            j++;
        }
        cout << endl;
    }
    out.close();
}

// write and show the best road of levenshtein distance
void part3(bool disType){
    
    unsigned int dis;
    int wordDis, wordDisPre ;
    string dPre;
//    cout << story.at(1) << endl;
    for (int i = 0; i < STORY_SIZE; i++) {
        //initialized to the biggest value of unsigned int
        distances[i] = UINT_MAX;
        //        cout << distances[i] << endl;

        string s = story.at(i);
        
        for (int j = 0; j < DICT_SIZE; j++) {
            
            string d = dict.at(j);
//            dis = resursiveLevenshtein(s, d);   // brute force
            
            if (disType) {
                dis = beamLevenshtein(s, d);        // beam search
            }
            else
                dis = levenshteinDis(s, d);         // edit distance
            
            if (distances[i] == dis) {
                wordDis = abs(int(s.size()) - int(d.size()));
                dPre = dict.at(distanceId[i]);
                wordDisPre = abs(int(s.size()) - int(dPre.size()));
                
                if (s[0] == d[0]) {
                    if (s[0] != dPre[0]) {
                        distances[i] = dis;
                        distanceId[i] = j;
                    }
                    else{
                        if (s[1] == d[1]) {
                            distances[i] = dis;
                            distanceId[i] = j;
                        }
                    }
                }
                
//                else if (wordDis == 0 && wordDisPre!= 0) {
//                    distances[i] = dis;
//                    distanceId[i] = j;
//                }
            }
            
            if (distances[i] > dis) {
                distances[i] = dis;
                distanceId[i] = j;
            }
        }
        
        printSinglePath(s, dict.at(distanceId[i]), filePath, "printPath.txt");
        cout << endl;
    }
    
    ofstream storyResult(filePath + "storyResult.txt");
    ofstream storyResultWithCorrect(filePath + "storyResultWithCorrect.txt");
    if (!storyResult.is_open()) {
        cout << "fail to open storyResult" << endl;
        exit(1);
    }
    if (!storyResultWithCorrect.is_open()) {
        cout << "fail to open storyResultWithCorrect" << endl;
        exit(1);
    }
    
    //correct word after edit distance
    int correctCount = 0;
    int levenshteinScore = 0;
    for (int i = 0; i < STORY_SIZE; i++) {
        if (disType) {
            levenshteinScore = beamLevenshtein(dict.at(distanceId[i]), storyCorrect.at(i));
        }
        else
            levenshteinScore =levenshteinDis(dict.at(distanceId[i]), storyCorrect.at(i));
        
        storyResult << story.at(i) << " "
        << dict.at(distanceId[i]) << " "
        << distances[i] << "\n";
        //edit distance of alignment word and correct word
        storyResultWithCorrect << dict.at(distanceId[i]) << " "
        << storyCorrect.at(i) << " "
        << levenshteinScore << "\n";
        //calculate number of correct word after edit distance
        if (dict.at(distanceId[i]) == storyCorrect.at(i))
            correctCount++;
    }
    cout << "correct rate : " << (double)correctCount / STORY_SIZE << endl;
    storyResult.close();
}


void part4(){
    unsigned int result = stringLevenshtein(story, storyCorrect);
    cout << result << endl;
    ofstream out(filePath + "test.txt");
    //0: substitution 1: insertion 2: deletion
    int errorRecord[3] = {0, 0, 0};
    printString(story, storyCorrect, out, errorRecord);
    cout << errorRecord[0] << " " << errorRecord[1] << " " << errorRecord[2] << endl;
    
}