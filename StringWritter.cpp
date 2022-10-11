//
// Created by smajl on 8. 10. 2022.
//

#include <bitset>
#include <iostream>
#include "StringWritter.h"

void StringWritter::writeBite(const std::string &bite_str) {
    auto bite = std::bitset<8>(bite_str).to_ulong();
    if (space >= bite_str.size()) {
        buffer <<= bite_str.size();
        buffer |= bite;
        space -= bite_str.size();
        if (space == 0) {
            //      cout << "I write: " << std::bitset<8>(buffer).to_string() << endl;
          //  std::cout <<data.size() << std::endl;
            data.push_back(buffer);
            buffer = 0;
            space = CHAR_SIZE;
        }
    } else {
        unsigned char tmp_space = space;
        writeBite(bite_str.substr(0,tmp_space));
        writeBite(bite_str.substr(tmp_space));
    }
}

void StringWritter::writeBiteString(const std::string &bite_str) {
   data.append(bite_str);
}

 std::pair<std::string, short> StringWritter::closeWrittening() {
        if (space != 8) {
            buffer<<=space;
            data.push_back(buffer);
            data.push_back(space);
        }else{
            char none = 0;
            data.push_back(none);

        }
    return {data,space==8?0:space};
}
short StringWritter::closeWritteningString(const std::string & str) {

    short space = 8-(str.size()%8);
    return space==8?0:space;
}

const std::string &StringWritter::getData() const {
    return data;
}

StringWritter::StringWritter() {
    data = "";
    space = CHAR_SIZE;
    buffer =0 ;
}
