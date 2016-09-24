#include "printPath.h"

//print the table of levenshtein distance
void printPath(const string& s1, const string& s2, string filePath, string fileName) {
    const std::size_t len1 = s1.size(), len2 = s2.size();
    int** printArray;
    printArray = new int*[len2 + 1];
    for (int i = 0; i < len2 + 1; i++) {
        printArray[i] = new int[len1 + 1];
        memset(printArray[i], 0, (len1 + 1) * sizeof(int));
    }
    
    std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);
    for (unsigned int i = 0; i < prevCol.size(); i++) {
        prevCol[i] = i;
        printArray[i][0] = i;
    }
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i + 1;
        printArray[1][i + 1] = i + 1;
        for (unsigned int j = 0; j < len2; j++) {
            col[j + 1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1) });
            printArray[j + 1][i] = col[j + 1];
        }
        col.swap(prevCol);
    }
    
    ofstream out(filePath + fileName);
    if (!out.is_open())
        cout << "fail to open file in printPath" << endl;
    
    char *c1 = new char[len1 + 1];
    strcpy(c1, s1.c_str());
    char *c2 = new char[len2 + 1];
    strcpy(c2, s2.c_str());
    
    for (int i = 0; i < len2 + 1; i++) {
        for (int j = 0; j < len1 + 1; j++) {
            if (j == 0) {
                if (i < len2) {
                    cout << setw(TABLE_GAP) << c2[len2 - 1 - i];
                    out << setw(TABLE_GAP) << c2[len2 - 1 - i];
                }
                else {
                    cout << setw(TABLE_GAP) << " ";
                    out << setw(TABLE_GAP) << " ";
                }
            }
            else {
                if (i != len2) {
                    cout << setw(TABLE_GAP) << printArray[len2 - i][j - 1];
                    out << setw(TABLE_GAP) << printArray[len2 - i][j - 1];
                }
                else {
                    cout << setw(TABLE_GAP) << c1[j - 1];
                    out << setw(TABLE_GAP) << c1[j - 1];
                }
            }
        }
        cout << endl;
        out << endl;
    }
    
    delete[] c1;
    delete[] c2;
    delete[] printArray;
}