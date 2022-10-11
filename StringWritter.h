//
// Created by zribktad  on 8. 10. 2022.
//

#ifndef SEM_STRINGWRITTER_H
#define SEM_STRINGWRITTER_H


#include <string>
#define CHAR_SIZE 8

class StringWritter {
private:
    std::string data;
    unsigned short space = CHAR_SIZE;
    unsigned char buffer =0 ;
public:

    void writeBite(const std::string &bite_str);
    std::pair<std::string, short> closeWrittening();

    StringWritter();

    void writeBiteString(const std::string &bite_str);

    static short closeWritteningString(const std::string & str);

    const std::string &getData() const;
};


#endif //SEM_STRINGWRITTER_H
