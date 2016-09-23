#include "fileRead.h"



void convertFile(string inputPath, string outputPath) {
    ofstream out(outputPath);
    ifstream in(inputPath);
    if (!in.is_open()) {
        cout << "fail to open file" << endl;
        exit(1);
    }
    char buffer[BUFFER_SIZE] = {'\0'};
    in.read(buffer, BUFFER_SIZE);
    in.close();
    int i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] >= 65 && buffer[i] <= 90) {
            buffer[i] += 32;
        }
        if (buffer[i] == ',' || buffer[i] == '\"' || buffer[i] == '.' ||
            buffer[i] == '!' || buffer[i] == '?' || buffer[i] == ';' ||
            buffer[i] == ':' || buffer[i] == ' ')
        {
            buffer[i] = '\n';
        }
        i++;
    }
    out << buffer;
    out.close();
}



void readFile(string filePath, vector<string>& vect) {
    ifstream in(filePath);

    if (!in.is_open())
        cout << "fail to open file" << endl;

    while (!in.eof()){
        char c[100];
        in.getline(c, 100);
        if (c[0] != '\n' && c[0] != ' ' && c[0] != '\0') {
            string str(c);
            vect.push_back(str);
        }
    }
    in.close();
}