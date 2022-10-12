#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <bitset>
#include <cstring>
#include <random>
#include <chrono>
#include "huffmanCode.h"
#include "Writter.h"
#include "ReaderInput.h"
#include "Compressor.h"

bool CompareTwoFiles(const string &inputName, const string &outputName);

void testMethod(char *const *argv,const bool parallel = false);

void writeInfo();

using namespace std;

template<typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

bool isNumber(const string &s) {
    for (char i: s)
        if (isdigit(i) == false)
            return false;
    return true;
}

int get_random_int() {
    static std::mt19937 mt{std::random_device{}()};
    static std::uniform_int_distribution<> dist(65, 100);
    return dist(mt);
}

void generateFileWithCharacter(const string &filename, const unsigned long &size) {

    string fileStr;
    for (unsigned long i = 0; i < size; ++i) {
        char letter = get_random_int();
        fileStr.push_back(letter);
    }

    ofstream file(filename);
    file << fileStr;
    file.close();
}

int main(int argc, char **argv) {
    bool start = false;
    if (argc == 2 && (!strcmp(argv[1], "--help")) | !strcmp(argv[1], "-h")) {
        writeInfo();
        start = true;
    } else if (argc == 3) {
        if (!strcmp(argv[1], "--comp") | !strcmp(argv[1], "-c")) {
            start = true;
            std::string fileName = argv[2];
            Compressor::compressFile(fileName, fileName + ".huf");
        } else if (!strcmp(argv[1], "--test")) {
            cout << "Testing file program " << endl;
            start = true;
            if (isNumber(argv[2])) {
                testMethod(argv);

            } else {
                cout << "Error: Bad argument for number! (" << argv[2] << ")" << endl;
                return 1;
            }
        }
    } else if (argc == 4) {
        if (!strcmp(argv[1], "--decomp") | !strcmp(argv[1], "-dc")) {
            start = true;
            Compressor::decompress(argv[2], argv[3]);
        } else if ((!strcmp(argv[1], "--comp") | !strcmp(argv[1], "-c"))) {
            if ((!strcmp(argv[2], "-p") || !strcmp(argv[2], "--parallel"))) {
                std::string fileName = argv[3];
                Compressor::compressFileParallel(fileName, fileName + ".huf");
                start = true;
            } else if ((!strcmp(argv[3], "-p") || !strcmp(argv[3], "--parallel"))) {
                std::string fileName = argv[2];
                Compressor::compressFileParallel(fileName, fileName + ".huf");
                start = true;
            }
        } else if (!strcmp(argv[1], "--test")) {
            bool parallel = false;
            int num_idx = 2;
            cout << "Testing file program " << endl;
            if (!strcmp(argv[2], "-p") || !strcmp(argv[2], "--parallel")) {
                parallel = true;
                num_idx = 3;
            } else if (!strcmp(argv[3], "-p") || !strcmp(argv[3], "--parallel")) {
                parallel = true;
            }
            start = true;
            if (isNumber(argv[num_idx])) {
                testMethod(argv, parallel);

            } else {
                cout << "Error: Bad argument for number! (" << argv[2] << ")" << endl;
                return 1;
            }
        }
    }

    if (start) {
        cout << endl << "Have a good day." << endl;
    } else {
        cout << endl << "Do you want to help?" << endl;
        writeInfo();
    }


    return 0;
}

void writeInfo() {
    cout << "Compression program of ascii file using Huffman coding." << endl;
    cout << "--help or -h \t: Show instruction for program, " << endl;
    cout << "--comp or -c  \t: Compress file. exp: [ -c \"Input file name\"] " << endl;
    cout << "--decomp or -dc  \t: Decompress file. exp: [ -dc \"Input file name\" \"Output file name\" ] " << endl;
    cout
            << "--test  \t: Compress and Decompress generated test file.  exp: [ --test \"Size of generated characters\" ]. \n"
               "!Warning: Three files will be created. (TestInput.txt,TestCompress.huf,TestOutput.txt )" << endl;
    cout
            << "--parallel or -p \t: additional command for compression, run in parallel mode.exp: [ --test 50 -p or --comp -p  ]"
            << endl;
}

void testMethod(char *const *argv, const bool parallel ) {
    unsigned long size = atoi(argv[2]);
    const string inputName = "TestInput.txt";
    const string outputName = "TestOutput.txt";
    const string compName = "TestCompress.huf";
    generateFileWithCharacter(inputName, size);
    std::chrono::system_clock::time_point startc_p,endc_p;
    if (parallel) {
        startc_p = std::chrono::high_resolution_clock::now();
        Compressor::compressFileParallel(inputName, compName);
         endc_p = std::chrono::high_resolution_clock::now();
    }
   auto startc = std::chrono::high_resolution_clock::now();
    Compressor::compressFile(inputName, compName);
    auto endc = std::chrono::high_resolution_clock::now();


    auto startd = std::chrono::high_resolution_clock::now();
    Compressor::decompress(compName, outputName);
    auto endd = std::chrono::high_resolution_clock::now();
    cout <<"*****************************************************"<<endl;
    if(parallel)
    cout << "Compress time for parallel: Needed " << to_ms(endc_p - startc_p).count() << " ms to finish.\n";
    cout << "Compress time: Needed " << to_ms(endc - startc).count() << " ms to finish.\n";
    cout << "Decompress time: Needed " << to_ms(endd - startd).count() << " ms to finish.\n";
    cout << "Together time: Needed " << to_ms((endd - startd) + (endc - startc)).count() << " ms to finish.\n";

    if (CompareTwoFiles(inputName, outputName)) {
        cout << "Files are correct!" << endl;
    } else {
        cout << "Files are different!" << endl;
    }
}

bool CompareTwoFiles(const string &inputName, const string &outputName) {
    ifstream stream(inputName);
    string file1{istreambuf_iterator<char>(stream),
                 istreambuf_iterator<char>()};
    stream = ifstream(outputName);
    string file2{istreambuf_iterator<char>(stream),
                 istreambuf_iterator<char>()};

    bool result = file1 == file2;
    return result;
}
