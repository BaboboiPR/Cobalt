#pragma once
#include <iostream>
#include <string>

inline bool stob(const std::string &s) {
    if (s == "true") return true;
    else if (s == "false") return false;
    else if (s == "1") return true;
    else if (s == "0") return false;
    else
        {
        std::cerr << "Error: variable isnt a bool returning false" << s << std::endl;
        return false;
    }
}

