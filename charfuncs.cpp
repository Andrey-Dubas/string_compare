#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <utility>
#include <string.h>
#include <deque>
#include <fstream>
#include <sstream>

#include <charfuncs.h>


template<typename CHR>
using ComparativeHolder             = std::pair<std::basic_string<CHR>, std::basic_string<CHR>>;

template<typename CHR>
using ComparativeHolderCollection   = std::deque<ComparativeHolder<CHR>>;

template <typename CHR>
class CharFuncs{
public:
    /*
    static const CHR compareTable[];

    static int letterPos(CHR letter);

    static int letterDiff(CHR ch1, CHR ch2);

    static float compareFunction(std::basic_string<CHR> str1, std::basic_string<CHR> str2);
*/
};



template<typename CHR>
int letterPos(CHR letter){


    const CHR* curIndex = CharFuncs<CHR>::compareTable;

    while(curIndex != CharFuncs<CHR>::ends){
        if(*curIndex == letter){
            return curIndex - CharFuncs<CHR>::compareTable;
        }
        curIndex++;
    }
    return -1;
}

//"оа\nэе\nиы\nйую\n\nдт\nжшщ\nхфг\nбп\nмн\nзсц\nкъврлч\n\nou\naie\ny\n\ndt\ngj\nhf\bcksz\nmn\n\nvw\n\n \n-\n_\n+\n=\n\\\n|\n/\n\'\n\"\n<\n>\n~\n!\n@\n#\n$\n%\n^\n&\n*\n(\n)\n\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n\n";
template<typename CHR>
float letterDiff(CHR ch1, CHR ch2){
    int pos1 = letterPos(ch1), pos2 = letterPos(ch2);

    if(pos1 == -1 || pos2 == -1){
        return 0;
    }

    if(pos1 == pos2){
        return 1; // letters are the same
    }


    float koefSimilarity = 0.66;

    for(int i = std::min(pos1, pos2); i != std::max(pos1, pos2); ++i){

        if( CharFuncs<CHR>::compareTable[i] == CharFuncs<CHR>::endl){
            koefSimilarity = std::min<float>(koefSimilarity, 0.33);
            if(CharFuncs<CHR>::compareTable[i+1] == CharFuncs<CHR>::endl){
                return 0.0;
            }
        }
    }
    return koefSimilarity;
}



/*
template <typename CHR>
float compareFunction(std::basic_string<CHR> str1, std::basic_string<CHR> str2){
    int len1 = str1.length(), len2 = str2.length();
    int minlen = std::min(len1, len2), maxlen = std::max(len1, len2);
    float diff = maxlen - minlen;

    float subdiff1 = 0, subdiff2 = 0;
    if(str1.length() > 1){
        std::basic_string<CHR> substr1 = str1.substr(1);
        subdiff1 = compareFunction(substr1, str2);// * (std::min(len1 - 1, len2) / (float)std::min(len1, len2)) + diff;
    }

    if(str2.length() > 1){
        std::basic_string<CHR> substr2 = str2.substr(1);
        subdiff1 = compareFunction(str1, substr2);// * (std::min(len1 - 1, len2) / (float)std::min(len1, len2)) + diff;
    }

    for(int i = 0; i < std::min(len1, len2); ++i){
        float local = letterDiff(str1.at(i), str2.at(i));
        diff += local;
    }

    float originaldiff = diff / (float) maxlen;
    return std::max(std::max(subdiff1, subdiff2), originaldiff);
}
*/


template <typename CHR>
float compareFunction(std::basic_string<CHR> str1, std::basic_string<CHR> str2){
    int len1 = str1.length(), len2 = str2.length();
    int minlen = std::min(len1, len2), maxlen = std::max(len1, len2);
    float diff = 0;//maxlen - minlen;

    //std::cout << "compare " << str1.c_str() << " and " << str2.c_str() << std::endl;

    if(minlen == 0){
        return 0; //no chars left
    }

    const int singleCharSize = 1;
    float headSimilarity = letterDiff(str1[0], str2[0]);
    if(headSimilarity == 1){
        float tailSimilarity = compareFunction(str1.substr(singleCharSize), str2.substr(singleCharSize));
        return headSimilarity + tailSimilarity;
    }
    else{
        float subSimilarity1 = compareFunction(str1.substr(singleCharSize), str2);
        float subSimilarity2 = compareFunction(str1.substr(singleCharSize), str2);
        float tailSimilarity = compareFunction(str1.substr(singleCharSize), str2.substr(singleCharSize));
        return headSimilarity + std::max(std::max(subSimilarity1, subSimilarity2), tailSimilarity);
    }
}


template <typename CHR>
static std::basic_string<CHR> getFileLine(std::basic_fstream<CHR>& file){
    std::basic_stringstream<CHR> buffer;

    if(file.eof() == true){
        throw std::exception();
    }
    const size_t bufSize = 255;
    CHR buf[bufSize];
    file.getline(buf, bufSize);
    buffer << buf;
    //std::basic_ostream<CHR>::out << buf;
    std::cout << buffer.str().c_str() << std::endl;
    return buffer.str();
}


template<typename CHR>
ComparativeHolderCollection<CHR>
getComparableStrings(/*std::basic_string<CHR>*/ std::string fileName){
    std::basic_fstream<CHR> file;
    file.open(fileName, std::ios_base::in);

    if (file.is_open() == false){
        file.close();
        throw std::exception();
    }

    ComparativeHolderCollection<CHR> container;
    try{
        while(true){
            std::basic_string<CHR> str1 = getFileLine(file);
            std::basic_string<CHR> str2 = getFileLine(file);
            container.push_front(ComparativeHolder<CHR>(str1, str2));
        }
    }
    catch(std::exception ex){
    }
    file.close();
    return container;
}


typedef CharFuncs<char> ascii;
typedef CharFuncs<wchar_t> unicode;

template<>
struct CharFuncs<char>{
    static const char compareTable[];
    static const char endl = '\n';
    static const char ends = '\0';
};

const char CharFuncs<char>::compareTable[] = "оа\nэе\nиы\nйую\n\nдт\nжшщ\nхфг\nбп\nмн\nзсц\nкъврлч\n\nou\naie\ny\n\ndt\ngj\nhf\bcksz\nmn\nv\np\nw\n\n \n-\n_\n+\n=\n\\\n|\n/\n\'\n\"\n<\n>\n~\n!\n@\n#\n$\n%\n^\n&\n*\n(\n)\n\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n\n";

template<>
struct CharFuncs<wchar_t>{
    static const wchar_t compareTable[];
    static const wchar_t endl = L'\n';
    static const wchar_t ends = L'\0';
};

const wchar_t CharFuncs<wchar_t>::compareTable[] = L"оа\nэе\nиы\nйую\n\nдт\nжшщ\nхфг\nбп\nмн\nзсц\nкъврлч\n\nou\naie\ny\n\ndt\ngj\nhf\bcksz\nmn\n\nvw\n\n \n-\n_\n+\n=\n\\\n|\n/\n\'\n\"\n<\n>\n~\n!\n@\n#\n$\n%\n^\n&\n*\n(\n)\n\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n\n";




template< typename T >
class has_static {
    public:
    static T fuh(){ return T();}
};
