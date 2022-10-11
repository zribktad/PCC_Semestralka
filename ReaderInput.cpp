//
// Created by zribktad  on 10/5/2022.
//

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "ReaderInput.h"
#include <bitset>
#include <sstream>

using namespace std;

bool ReaderInput::readCompressFile(const std::string& fileName) {

    string content;
    try {
        std::ifstream file(fileName, ios::binary);
        std::stringstream strStream;
        strStream << file.rdbuf(); //read the file
        content = strStream.str();
        file.close();
        //cout << content <<endl;
    }catch (...){
        cout << "Error file not found!";
        return false;
    }

    try {

        for (int i = 1; i < ((unsigned char) content[0]) * 3; i += 3) {
            string code = std::bitset<8>(content[i + 2]).to_string().substr(8 - ((unsigned char) content[i + 1]));
            codex.insert(pair<string, unsigned char>(code, content[i]));
        }
        compressFile = content.substr(((unsigned char) content[0]) * 3 + 1);
        content.clear();
    }
    catch(...){
        if(content.empty())
        cout << ("File is empty!") << endl;
        else
        cout << ("File is corrupted!") << endl;
        content.clear();
        return false;
    }

    return true;
}

ReaderInput::ReaderInput() = default;

const string &ReaderInput::getCompressFile() const {
    return compressFile;
}

const map<string , unsigned char> &ReaderInput::getCodex() const {
    return codex;
}

 string ReaderInput::openFile(const string & fileName) {
     std::ifstream file(fileName );
     std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}


