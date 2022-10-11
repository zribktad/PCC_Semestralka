//
// Created by zribktad  on 10/5/2022.
//

#ifndef SEM_READERINPUT_H
#define SEM_READERINPUT_H

#include <map>

class ReaderInput {
    std::string compressFile;
    std::map< std::string,unsigned char> codex;
public:
     bool readCompressFile(const std::string& fileName) ;

    ReaderInput();

    const std::string &getCompressFile() const;

    const std::map<std::string,unsigned char> &getCodex() const;
    static std::string openFile(const std::string & fileName);
};


#endif //SEM_READERINPUT_H
