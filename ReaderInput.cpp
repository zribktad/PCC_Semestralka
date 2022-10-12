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

//        for (int i = 1; i < ((unsigned char) content[0]) * 3; i += 3) {
//             char &code_bin = content[i + 2];
//            auto size_of_bin = (unsigned short) content[i + 1];
//            cout << code_bin << " :  " << std::bitset<8>(code_bin).to_string().substr(8 - size_of_bin) << endl;
//            string code = std::bitset<8>(code_bin).to_string().substr(8 - size_of_bin);
//            codex.insert(pair<string, unsigned char>(code, content[i]));
//        }
        int counter =0,i=1;
        while(counter != content[0]){
            auto letter = content[i];
            i++;
            auto size_of_bin = (unsigned short) content[i];
            i++;
            string code;
            for (int j = 0; j < size_of_bin; ++j) {

                code.push_back(content[i]);
                i++;
            }
            codex.insert(pair<string, unsigned char>(code, letter));
            counter++;
        }

        compressFile = content.substr(i );
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


