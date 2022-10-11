//
// Created by zribktad  on 10/4/2022.
//

#include <bitset>
#include "Writter.h"

using namespace std;



void Writter::openFileOut(const string &fileName) {
    fileOut = fopen(fileName.c_str(), "wb");
    if (!fileOut) {
        throw std::runtime_error(" File not open!");
    }
}

void Writter::writeBite(const string &bite_str) {
    auto bite = std::bitset<8>(bite_str).to_ulong();
    if (space >= bite_str.size()) {
        buffer <<= bite_str.size();
        buffer |= bite;
        space -= bite_str.size();
        if (space == 0) {
             putc(buffer, fileOut);

            buffer = 0;
            space = CHAR_SIZE;
        }
    } else {
        unsigned char tmp_space = space;
        writeBite(bite_str.substr(0,tmp_space));
        writeBite(bite_str.substr(tmp_space));
    }
}

void Writter::closeFileOut() {
    if (fileOut) {
        if (space != 8) {
            buffer<<=space;
            putc(buffer, fileOut);
            putc(space, fileOut);
        }else{
            char none = 0;
            putc(none, fileOut);
        }
        fclose(fileOut);
    }
}

void Writter::writeHeader(const map<unsigned char, string> &codex) {
    unsigned char num =codex.size();
    fwrite(&num,1,1,fileOut);
    for (const auto & itr : codex) {
        putc(itr.first,fileOut);
        //fwrite(&itr.first,sizeof(unsigned char),1,fileOut);
        unsigned char size_num = itr.second.size();
        putc(size_num,fileOut);
       // fwrite(&size_num,sizeof(unsigned char),1,fileOut);
        unsigned char num= std::bitset<8>(itr.second).to_ulong();
        putc(num,fileOut);
    }


}
void Writter::closeFile(){
    fclose(fileOut);
}

void Writter::saveStringToFile(const string &data, const string &filename, const bool append) {
    ofstream file;
   if(append)
        file.open(filename,std::ios_base::app | ios::binary);
   else
        file.open(filename, ios::binary);
    file << data;
    file.close();


}
