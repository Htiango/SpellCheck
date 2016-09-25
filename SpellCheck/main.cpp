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

string filePath = "/Users/hty/Desktop/Speech Recognition/project/project3/";
string storyName = "story.txt";
string pureStoryName = "pureStory.txt";
string dictName = "dict.txt";
string storyCorrectName = "storycorrect.txt";
string pureStoryCorrectName = "pureStoryCorrect.txt";


vector<string> story;
vector<string> dict;
vector<string> storyCorrect;

void writeDis();
void multiConduct(bool disType);

//edit distance of input and dict word
unsigned int distances[STORY_SIZE];
//mapping of dict word id to input word id
int distanceId[STORY_SIZE];


int main(int argc, const char * argv[]) {
    

//    convertFile((filePath + storyName), (filePath + pureStoryName));
//    convertFile((filePath + storyCorrectName), (filePath + pureStoryCorrectName));
//    unsigned int result = levenshteinDis("a", "are");
//    cout << result << endl;


//    writeDis();
    
//    multiConduct(true);   // multi beam search
//    multiConduct(false);   // multi edit distance
    
 
//    printSinglePath("eleaphent", "elephant", filePath, "printPath.txt");
    
    
    ofstream out(filePath + "name.txt");
    print("helloyo", "helloyour", out);
    
    return 0;
}

void multiConduct(bool disType){
    vector<string> temp = {"elephant", "elegant", "sycophant"};
    map<string, int> resultMap;
    map <string, int>::iterator resultIter;
    
    string str = "eleaphent";
    
    
    multiLe(str, temp, resultMap, disType);
    
    for (resultIter = resultMap.begin(); resultIter != resultMap.end(); resultIter++) {
        cout << resultIter->first << " " << resultIter->second << endl;
    }
}

void writeDis(){
    
    unsigned int dis;
    int wordDis, wordDisPre ;
    string dPre;
    
    readFile(filePath + pureStoryName, story);
    readFile(filePath + dictName, dict);
    readFile(filePath + pureStoryCorrectName, storyCorrect);
//    cout << story.at(1) << endl;
    for (int i = 0; i < STORY_SIZE; i++) {
        //initialized to the biggest value of unsigned int
        distances[i] = UINT_MAX;
        //        cout << distances[i] << endl;

        string s = story.at(i);
        
        for (int j = 0; j < DICT_SIZE; j++) {
            
            string d = dict.at(j);

//            dis = levenshteinDis(s, d);         // edit distance
//            dis = resursiveLevenshtein(s, d);   // brute force
            dis = beamLevenshtein(s, d);        // beam search
            
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
    for (int i = 0; i < STORY_SIZE; i++) {
        storyResult << story.at(i) << " "
        << dict.at(distanceId[i]) << " "
        << distances[i] << "\n";
        //edit distance of alignment word and correct word
        storyResultWithCorrect << dict.at(distanceId[i]) << " "
        << storyCorrect.at(i) << " "
        << levenshteinDis(dict.at(distanceId[i]), storyCorrect.at(i)) << "\n";
        //calculate number of correct word after edit distance
        if (dict.at(distanceId[i]) == storyCorrect.at(i))
            correctCount++;
    }
    cout << "correct rate : " << (double)correctCount / STORY_SIZE << endl;
    storyResult.close();
}