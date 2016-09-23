#ifndef _FILEREAD_H_
#define _FILEREAD_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//convert file
#define BUFFER_SIZE (10000)
//story and storycorrect word number
#define STORY_SIZE (613)
//dict word number
#define DICT_SIZE (6618)

void convertFile(string inputPath, string outputPath) ;
void readFile(string filePath, vector<string>& vect) ;

#endif