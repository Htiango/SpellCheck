#ifndef _PRINTPATH_H_
#define _PRINTPATH_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

//gap between characters when print path table
#define TABLE_GAP (3)

void printPath(const string& s1, const string& s2, string filePath, string fileName);

#endif