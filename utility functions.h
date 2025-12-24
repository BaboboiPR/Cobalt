#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

struct variable {
    std::string name;
    std::string type;
    bool array = false;
};
struct token {
    std::string type;
    std::string value;
};
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


int check_loop(variable name[],std::string string_name) {
    for (int i = 0; i < 99; i++) {
        if (name[i].type == string_name) {
            return i;
        }
    }
    return -1;
}

//function pointer tutorial
// void hello() { std::cout << "Hello!\n"; }
// void (*a)();
// a = hello;
//variable = function
void var_to_func(std::string name_var,std::string type_var, std::string name_func,std::string op ,std::ofstream& file) {
    file << type_var << " (*" << name_var << ")(" << op <<");" << "\n";
    file << name_var << " = " << name_func << ";" << "\n";
}


void AST(int line,std::vector<std::string> line_str) {
    std::ofstream fin("file.bt");
    for (int i = 0; i <= line; i++) {
        std::cout << line_str[i] << std::endl;

        fin << line_str[i] << "\n";
    }
    fin.close();

}
