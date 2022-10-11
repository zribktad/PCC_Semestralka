//
// Created by zribktad  on 10/4/2022.
//

#ifndef SEM_WRITTER_H
#define SEM_WRITTER_H

#include<iostream>
#include<fstream>
#include <map>

using namespace  std;
#define CHAR_SIZE 8

class Writter {

public:
    inline static unsigned char buffer=0;
    inline static unsigned short space = CHAR_SIZE;
    inline static  FILE *   fileOut = nullptr;


   static  void closeFileOut();
   static void writeBite(const string &  bite);
   static void writeHeader(const  std::map<unsigned char, string> & codex);
   static void saveStringToFile(const string &data, const string &filename, bool append);
   static void openFileOut(const string &fileName);

   static  void closeFile();
};





#endif //SEM_WRITTER_H
