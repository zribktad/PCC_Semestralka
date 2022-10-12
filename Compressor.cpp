//
// Created by zribktad on 6. 10. 2022.
//

#include <bitset>
#include <atomic>
#include "Compressor.h"
#include "ReaderInput.h"
#include "huffmanCode.h"
#include "Writter.h"
#include "StringWritter.h"
#include "parrallel_content.h"

using namespace std;

bool Compressor::compressFile(const std::string &inputFileName, const std::string &outputFileName) {
    ReaderInput readerInput;
    cout << "\nStart compression." << endl;
    auto inputFile = ReaderInput::openFile(inputFileName);
    if (inputFile.empty()) {
        cout << "Empty file! Process Stop." << endl;
        return false;
    }
    cout << "File opened. ( size:  " << inputFile.size() << "letters) " << endl;
    cout << "Counting letters." << endl;
    auto countForLetter = countLetters(inputFile);
    //showCountLetters(countForLetter);
    cout << "Creating huffman tree." << endl;
    Node *root = huffmanCode::huffmanTree(countForLetter);
    // huffmanCode::TreeShow(root);

    cout << "Creating translator." << endl;
    map<unsigned char, string> codex;
    huffmanCode::GetCodes(root, codex);
    //ShowCodex(codex);
    huffmanCode::removeTree(root);
    cout << "Creating compressFileParallel file." << endl;
    Writter::openFileOut(outputFileName);
    cout << "Writing the header to the file." << endl;
    Writter::writeHeader(codex);
    cout << "Translating file." << endl;
    translateFile(codex, inputFile);
    cout << "Finishing translation." << endl;
    Writter::closeFileOut();
    cout << "Compression finished." << endl;

    return true;
}

bool Compressor::compressFileParallel(const std::string &inputFileName, const std::string &outputFileName) {
    ReaderInput readerInput;
    cout << "\nStart compression in parallel mode." << endl;
    auto inputFile = ReaderInput::openFile(inputFileName);
    if (inputFile.empty()) {
        cout << "Empty file! Process Stop." << endl;
        return false;
    }
    cout << "File opened. ( size:  " << inputFile.size() << "letters) " << endl;
    cout << "Counting letters." << endl;
    auto countForLetter = countLetters(inputFile, true);
    //showCountLetters(countForLetter);
    cout << "Creating huffman tree." << endl;
    Node *root = huffmanCode::huffmanTree(countForLetter);
     //huffmanCode::TreeShow(root);
    cout << "Creating translator." << endl;
    map<unsigned char, string> codex;
    huffmanCode::GetCodes(root, codex);
    huffmanCode::removeTree(root);

    cout << "Creating compressFileParallel file." << endl;
    Writter::openFileOut(outputFileName);
    cout << "Writing the header to the file." << endl;
    Writter::writeHeader(codex);
    Writter::closeFile();
    cout << "Translating file." << endl;
    vector<std::string> compressData(NUM_OF_TRREADS_NOW);
    cout << "Translate to string of 0/1: " << endl;

    //ShowCodex(codex);
    PARALLEL_FOR_BEGIN(inputFile.size()) {
        compressData[thread_num] = translateFileToString(codex,inputFile,start,end);
                                       }PARALLEL_END();

    string compressStr = putTogetherString(compressData);
    alignStringToByte(compressStr);
    cout << "Translate from string to byte type." <<endl;
    vector<string> compressFin = StringToByteBig(compressStr);

    cout << "Connecting bytes." <<endl;
    string data = putTogetherString(compressFin);
    cout << "Saving file." << endl;
    Writter::saveStringToFile(data, outputFileName, true);
    cout << "Parallel compression finished." << endl;

    return true;
}

bool Compressor::decompress(const string &inputFileName, const string &outputFileName) {
    cout << "\nStart decompress file" << endl;
    ReaderInput readerInput;
    cout << "Reading file" << endl;
    if (!readerInput.readCompressFile(inputFileName)) {
        cout << "Error process stop!" << endl;
        return false;
    }
    string inputFile = readerInput.getCompressFile();
    auto codex = readerInput.getCodex();
    cout << "Translating..." << endl;
    string decompress = translateFileBack(codex, inputFile);
    cout << "Saving to file:" << outputFileName << endl;
    Writter::saveStringToFile(decompress, outputFileName, false);
    cout << "Decompression finished." << endl;
    return true;
}

[[maybe_unused]] void Compressor::ShowCodex(map<unsigned char, string> &codex) {
    for (auto &c: codex) {
        cout << c.second << " -> " << c.first << "  , ";
    }
    cout << endl;
}

vector<string> Compressor::StringToByteBig(const string &compressStr) {


     unsigned long batch_size = CHAR_SIZE * (((compressStr.size()) / (thread::hardware_concurrency()+1)) / (CHAR_SIZE));
    if (batch_size == 0)batch_size = compressStr.size();
    unsigned nb_threads =compressStr.size()/batch_size;
    const unsigned batch_remainder = compressStr.size() % batch_size;

    vector<string> compressFin(nb_threads+1);


    PARALLEL_TASKS_BEGIN(nb_threads){
         unsigned start_from =thread_num*batch_size;
         compressFin[thread_num] = StringToByteString(compressStr,start_from, start_from+batch_size);
                                            }PARALLEL_END();
    unsigned  long s=nb_threads*batch_size;
    compressFin[compressFin.size()-1] = StringToByteString(compressStr,s,s+batch_remainder );
    return compressFin;
}

string
Compressor::StringToByteString(const string &compressStr,unsigned long start, unsigned long end) {
    string translate;
    if (compressStr.size() % 8 == 0) {
        for (unsigned long i = start; i < end; i += 8)
        {
            translate.push_back(((char) bitset<8>(compressStr.substr(i, 8)).to_ulong()));
        }
    } else {
        throw runtime_error(" Bad format of block: " + compressStr.substr(start,end) + "  modulo:" + to_string((compressStr.substr(start,end).size() % 8)));
    }
    return translate;
}

void Compressor::alignStringToByte(string &compressStr) {
    short size = StringWritter::closeWritteningString(compressStr);
    for (int i = 0; i < size; ++i) {
        compressStr.push_back('0');
    }
    compressStr.append(bitset<8>(size).to_string());
}

string Compressor::putTogetherString(const vector<std::string> &compressData) {
    string compressStr;
    for (const auto & i : compressData) {
        compressStr.append(i);
    }
    return compressStr;
}


unordered_map<unsigned char, atomic<int>> Compressor::countLetters(const string &file, const bool parallel) {
    unordered_map<unsigned char, std::atomic<int>> map;
    if (parallel) {
        PARALLEL_FOR_BEGIN_LOOP(file.size()) {
                                            map[file[i]]++;
                                        }PARALLEL_END();
    } else {
        for (auto &c: file) {
            map[c]++;
        }
    }
    return map;
}


void Compressor::showCountLetters(unordered_map<unsigned char, atomic<int>> &map) {
    cout << "Letters : ";
    for (auto &itr: map) {
        cout << itr.first << ": " << itr.second << ", ";

    }
    cout << endl;
}

void Compressor::translateFile(map<unsigned char, string> &codex, const string &inputFile) {

    for (auto &c: inputFile) {
        Writter::writeBite(codex[c]);
    }


}

std::string Compressor::translateFileToString(map<unsigned char, string> &codex, const string &inputFile, const unsigned long start, const unsigned long end) {
    StringWritter sw;

    for (unsigned long   i = start; i < end; ++i) {
        char c = inputFile[i];
        sw.writeBiteString(codex[c]);
    }


    return sw.getData();

}



string Compressor::translateFileBack(map<string, unsigned char> &codex, const string &inputFile) {
    string decompress, compress;
    unsigned long index = 0;
    const unsigned long max_size = inputFile.size() - 1;
    while (compress.size() > 0 || max_size > index) {
        if (compress.size() < 16 && max_size > index) {
            const auto c = inputFile[index];
            compress.append(std::bitset<8>(c).to_string());
            index++;
            if (index == max_size) {
                compress = compress.substr(0, compress.size() - ((unsigned char) inputFile[max_size]));
            }
        }
        for (int i = 1; i <= compress.size(); ++i) {
            const string tmp = compress.substr(0, i);
            auto it = codex.find(tmp);
            if (it != codex.end()) {
                decompress.push_back(it->second);

                compress = compress.substr(i);

                break;
            }
        }
    }

    return decompress;
}

