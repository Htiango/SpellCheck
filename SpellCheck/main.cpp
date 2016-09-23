//
//  main.cpp
//  SpellCheck
//
//  Created by hty on 9/21/16.
//  Copyright © 2016 hty. All rights reserved.
//

#include "levenDis.h"
#include "fileRead.h"

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


//edit distance of input and dict word
unsigned int distances[STORY_SIZE];
//mapping of dict word id to input word id
int distanceId[STORY_SIZE];


int main(int argc, const char * argv[]) {
    

//    convertFile((filePath + storyName), (filePath + pureStoryName));
//    convertFile((filePath + storyCorrectName), (filePath + pureStoryCorrectName));
//    unsigned int result = levenshteinDis("a", "are");
//    cout << result << endl;


    readFile(filePath + pureStoryName, story);
    readFile(filePath + dictName, dict);
    readFile(filePath + pureStoryCorrectName, storyCorrect);
    cout << story.at(1) << endl;
    for (int i = 0; i < STORY_SIZE; i++) {
        //initialized to the biggest value of unsigned int
        distances[i] = UINT_MAX;
        for (int j = 0; j < DICT_SIZE; j++) {
            string s = story.at(i);
            string d = dict.at(j);
//            if (abs((int)(s.length() - d.length())) <= EDIT_DIS) {
                unsigned int dis = levenshteinDis(s, d);
//                unsigned int dis = levenshtein_distance(s, d);
                if (distances[i] > dis) {
                    distances[i] = dis;
                    distanceId[i] = j;
                }
//            }
        }
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
 
    
    return 0;
}
