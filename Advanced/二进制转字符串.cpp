#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cwchar> 
#include <cwctype> 
#include <algorithm>
using namespace std;
namespace utilString {

    struct NoCaseCompareChar {
        bool operator()(char l, char r) const
        {
            bool bEqual = (l == r);
            if (bEqual) {
                return true;
            }
            if (isalpha(static_cast<unsigned char>(l))) {
                if (isupper(static_cast<unsigned char>(l))) {
                    return l == std::toupper(r);
                }
                else {
                    return l == std::tolower(r);
                }
            }

            return bEqual;
        }
        bool operator()(wchar_t l, wchar_t r) const
        {
            bool bEqual = (l == r);

            if (bEqual) {
                return true;
            }

            if (iswalpha(l)) {
                if (iswupper(l)) {
                    return l == static_cast<wchar_t>(std::towupper(r));
                }
                else {
                    return l == static_cast<wchar_t>(std::towlower(r));
                }
            }

            return bEqual;
        }
    } no_case_compare_char;

    bool startswith(const std::string& str, const std::string& pattern, bool case_sensitive)
    {
        if (str.length() < pattern.length()) {
            return false;
        }

        if (case_sensitive) {
            return std::equal(pattern.begin(), pattern.end(), str.begin());
        }
        else {
            return std::equal(pattern.begin(), pattern.end(), str.begin(), no_case_compare_char);
        }
    }

    bool endswith(const std::string& str, const std::string& pattern, bool case_sensitive)
    {
        if (str.length() < pattern.length()) {
            return false;
        }

        if (case_sensitive) {
            return equal(pattern.rbegin(), pattern.rend(), str.rbegin());
        }
        else {
            return equal(pattern.rbegin(), pattern.rend(), str.rbegin(), no_case_compare_char);
        }
    }

    std::string& removeLeft(std::string& str, const std::string& pattern)
    {
        if (not startswith(str, pattern, true)) {
            return str;
        }

        return str.erase(0, pattern.length());
    }

    std::string& removeWhiteSpacesIfNeeded(std::string& a)
    {
        while (a.size() > 1 && a.at(0) == ' ') {
            a.erase(0, 1);
        }
        while (a.size() > 1 && a.at(a.length() - 1) == ' ') {
            a.pop_back();
        }

        return a;
    }

    std::string& removeBracketsIfNeeded(std::string& a)
    {
        if (a.length() > 1 && a.at(0) == '"' && a.at(a.length() - 1) == '"') {
            a.pop_back();
            a.erase(0, 1);
        }
        if (a.length() > 1 && a.at(0) == '\'' && a.at(a.length() - 1) == '\'') {
            a.pop_back();
            a.erase(0, 1);
        }

        return a;
    }

    std::string& removeNoPrintIfNeeded(std::string& a)
    {
        //TODO:一个个删会不会效率太低
        while (a.size() > 1 && (not isprint(a.at(0)))) {
            a.erase(0, 1);
        }

        size_t index;
        for (index = 0; index < a.size(); index++) {
            if (not isprint(a[index])) {
                a.erase(index);
                break;
            }
        }

        /*
        while (a.size() > 1 && (not isprint(a.at(a.length() - 1)))) {
            a.pop_back();
        }
        */

        return a;
    }

    std::string& removeLeftStr(std::string& str, std::string& left)
    {

        return str;
    }

    std::string& chomp(std::string& str)
    {
        std::string::size_type pos = str.find_last_not_of("\n\r");
        if (pos != std::string::npos) {
            str.erase(pos + 1, str.length() - pos - 1);
        }

        return str;
    }

    std::string& tolower(std::string& str)
    {
        std::transform(str.begin(),
            str.end(),
            str.begin(),
            ::tolower);

        return str;
    }

    std::string& toupper(std::string& str)
    {
        std::transform(str.begin(),
            str.end(),
            str.begin(),
            ::toupper);

        return str;
    }

    std::vector<std::string> split(const std::string& to_split, char delimiter)
    {
        std::vector<std::string> tokens;

        std::stringstream stream(to_split);
        std::string item;
        while (std::getline(stream, item, delimiter)) {
            tokens.push_back(item);
        }

        return tokens;
    }

    std::vector<std::string> extract(const uint8_t* input, uint32_t input_len, size_t minlen)
    {
        std::vector<std::string> ss;
        string stmp;

        uint32_t i;
        for (i = 0; i < input_len; i++) {
            uint8_t ch = input[i];
            if (isprint(ch)) {
                stmp.push_back(ch);
            }
            else {
                if (not stmp.empty()) {
                    if (stmp.length() >= minlen) {
                        ss.push_back(stmp);
                    }
                    stmp.clear();
                }
            }
        }

        return ss;
    }

    std::string& nomalize(std::string& str)
    {
        removeWhiteSpacesIfNeeded(str);
        removeBracketsIfNeeded(str);
        tolower(str);

        return str;
    }

}

std::string& removeNoPrintIfNeeded(std::string& a)
{
    while (a.size() > 1 && (not isprint(a.at(0)))) {
        a.erase(0, 1);
    }

    size_t index;
    for (index = 0; index < a.size(); index++) {
        if (a[index] == '\0') {
            a.erase(index);
            break;
        }
    }

    while (a.size() > 1 && (not isprint(a.at(a.length() - 1)))) {
        a.pop_back();
    }

    return a;
}

void hexstring_from_data(const void* data, size_t len, char* output) {
    const unsigned char* buf = (const unsigned char*)data;
    size_t i, j;
    for (i = j = 0; i < len; ++i) {
        char c;
        c = (buf[i] >> 4) & 0xf;
        c = (c > 9) ? c + 'a' - 10 : c + '0';
        output[j++] = c;
        c = (buf[i] & 0xf);
        c = (c > 9) ? c + 'a' - 10 : c + '0';
        output[j++] = c;
    }
}

std::string hexstring_from_data(const void* data, size_t len) {
    if (len == 0) {
        return std::string();
    }
    std::string result;
    result.resize(len * 2);
    hexstring_from_data(data, len, &result[0]);
    return result;
}

//字符串转十六进制字符流
std::string hexstring_from_data(const std::string& data) {
    return hexstring_from_data(data.c_str(), data.size());
}

void data_from_hexstring(const char* hexstring, size_t length, void* output) {
    unsigned char* buf = (unsigned char*)output;
    unsigned char byte;
    if (length % 2 != 0) {
        throw std::invalid_argument("data_from_hexstring length % 2 != 0");
    }
    for (size_t i = 0; i < length; ++i) {
        switch (hexstring[i]) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            byte = (hexstring[i] - 'a' + 10) << 4;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            byte = (hexstring[i] - 'A' + 10) << 4;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            byte = (hexstring[i] - '0') << 4;
            break;
        default:
            throw std::invalid_argument("data_from_hexstring invalid hexstring");
        }
        ++i;
        switch (hexstring[i]) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            byte |= hexstring[i] - 'a' + 10;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            byte |= hexstring[i] - 'A' + 10;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            byte |= hexstring[i] - '0';
            break;
        default:
            throw std::invalid_argument("data_from_hexstring invalid hexstring");
        }
        *buf++ = byte;
    }
}

std::string data_from_hexstring(const char* hexstring, size_t length) {
    if (length % 2 != 0) {
        throw std::invalid_argument("data_from_hexstring length % 2 != 0");
    }
    if (length == 0) {
        return std::string();
    }
    std::string result;
    result.resize(length / 2);
    data_from_hexstring(hexstring, length, &result[0]);
    return result;
}

//十六进制字节流转字符串
std::string data_from_hexstring(const std::string& hexstring) {
    return data_from_hexstring(hexstring.c_str(), hexstring.size());
}

int main1()
{
    std::string str = "53454c45435420612e2a2046524f4d205359535f636174616c6f672e5359535f757365722061000000420000000c000000000000000044000000065000450000000900000000005300000004";
    std::cout << "str: " << str << std::endl;
    string data = data_from_hexstring(str);
    std::cout << "data_from_hexstring: " << data << std::endl;
    string nopt = removeNoPrintIfNeeded(data);
    std::cout << "removeNoPrintIfNeeded: " << nopt << std::endl;
    //std::cout << "厉害厉害" << std::endl;       
    return 0;
}