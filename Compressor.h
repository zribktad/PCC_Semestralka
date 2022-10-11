//
// Created by smajl on 6. 10. 2022.
//

#ifndef SEM_COMPRESSOR_H
#define SEM_COMPRESSOR_H


#include <string>
#include <unordered_map>
#include <map>
#include <vector>

using namespace  std;


class Compressor {

    static unordered_map<unsigned char, atomic<int>> countLetters(const string &file, const bool parallel= false);
    static void showCountLetters(unordered_map<unsigned char, int> & map);
    static void translateFile(std::map<unsigned char, string> &codex, const string &inputFile);
    static string translateFileBack(std::map<string,unsigned char > &codex, const string &inputFile);
    static string translateFileToString(map<unsigned char, string> &codex, const string &inputFile, const unsigned long start, const unsigned long end);
    static string putTogetherString(const std::vector<string> &compressData);
    static void alignStringToByte(string &compressStr);
    static  string StringToByteString(const string &compressStr,unsigned long start, unsigned long end);

public:
   static bool compressFile(const std::string& inputFileName, const std::string& outputFileName);
   static bool decompress(const std::string& inputFileName, const std::string& outputFileName);
   static bool compressFileParallel(const std::string &inputFileName, const std::string &outputFileName);


    static vector<string> StringToByteBig(const string &compressStr);

    [[maybe_unused]] static void ShowCodex(map<unsigned char, string> &codex);
};


#endif //SEM_COMPRESSOR_H
