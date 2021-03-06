#ifndef _LEVENDIS_H_
#define _LEVENDIS_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;


#define EDIT_DIS        (3)
#define BEAM            (3)

unsigned int levenshteinDis(const string str_input, const string str_tem);   // pruning use the edit distance
unsigned int beamLevenshtein(const string str_input, const string str_tem);   // pruning use the beam search
unsigned int resursiveLevenshtein(const std::string& s1, const std::string& s2);   // brute force
void multiLe (string& input, vector<string>& tem, map<string, int>& resultMap, bool disType); // multi pruning
unsigned int stringLevenshtein(vector<string>& input, vector<string>& tem);

#endif


