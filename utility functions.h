#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
inline int stob(const std::string &s) {
    if (s == "true") return true;
    else if (s == "false") return false;
    else if (s == "1") return true;
    else if (s == "0") return false;
    else
        {
        std::cerr << "Error code 1: variable isn't a bool, returning -1" << s << std::endl;
        return -1;
    }
}

void reset_file(std::ifstream &file) {
    file.clear();                 // clear eof/fail flags
    file.seekg(0, std::ios::beg); // go back to start
}


int check_loop(std::string name[],std::string string_name) {
    for (int i = 0; i < 99; i++) {
        if (name[i] == string_name) {
            return i;
        }
    }
    return -1;
}

std::string table [10][10]
{
    {"Int","int"},
    {"Float","float"},
    {"String","string"},
    {"Bool","bool"}
};
bool check_var(std::string type_var,std::string token) {

    for (int i = 0; i < 4; i++) {
        if (table[i][0]==type_var && table[i][1]==token) {
            return true;
        }
    }
    std::cerr<<"Error code 3: Type of variable doesn't exist";
    return false;

}