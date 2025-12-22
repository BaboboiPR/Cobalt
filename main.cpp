#include <iostream>
#include <string>
#include "utility functions.h"
#include <fstream>
using namespace std;

string tokens[100];
int form = 0;
string word;
int cnt = 0;
string varName;
string type_var;
bool loop = false;
int indent_level = 0;

void Tabs(ofstream& file) {
    for (int i = 0; i < indent_level; i++)
    {
        file << "\t";
    }
}

//C++ integration
void to_cpp(const long long m, ofstream& file, ifstream& file1)
{
    if (tokens[m] == "cpp_main")
    {
        while (getline(file1, word))
        {
            if (word == "end;") break;
            file << word << endl;
        }
    }
}
void to_cpp_ext(const long long m, ofstream& file, ifstream& file1)
{
    if (tokens[m] == "cpp_ext")
    {
        while (getline(file1, word))
        {
            if (word == "end;") break;
            file << word << endl;
        }
    }
}

//Output support
void print(const long long& m, ofstream& file)
{
    if (tokens[m] == "print")
    {
        file << "cout << " << tokens[m+1] << ";" << endl;
    }
    if (tokens[m] == "print!")
    {
        file << "cout <<\"" <<tokens[m+1] << "\";" << endl;
    }
}
//Input support
void input( const long long& m, ofstream& file)
{
    if (tokens[m] == "input")
    {
        file << "cin >> " << tokens[m+1] << ";" << endl;
    }
}
//Basic arithmetic support
void Add(ofstream& file, const long long& m)
{
    if (tokens[m] == "+" and tokens[m - 1].substr(0, 1) != "[")
    {
        file << tokens[m - 1] << " + " << tokens[m + 1] << ";" << endl;
    }
}
void Minus(ofstream& file, const long long& m)
{
    if (tokens[m] == "-")
    {
        file << tokens[m - 1] << " - " << tokens[m + 1] << ";" << endl;
    }
}
void Multiplication(ofstream& file, const long long& m)
{
    if (tokens[m] == "*")
    {
        file << tokens[m - 1] << " * " << tokens[m + 1] << ";" << endl;
    }
}
void Division(ofstream& file, const long long& m)
{
    if (tokens[m] == "/")
    {
        if (stoi(tokens[m + 1]) != 0) {
            file << tokens[m - 1] << " / " << tokens[m + 1] << ";" << endl;
        }
    }
}
//loop support
void While(ofstream& file, const long long& m)
{
    if (tokens[m] == "while")
    {
        file << "while" << "(" << tokens[m + 1] << tokens[m + 2] << tokens[m + 3] << "){" << "\n";
        loop = true;
        indent_level++;
    }
}
// void For(ofstream& file, const long long& m)
// {
//     if (tokens[m] == "for")
//     {
//         file << "for" << "(" << tokens[m + 1] << ";"<<tokens[m + 2] <<";" <<tokens[m + 3] << "){" << endl;
//         file << "\t";
//         loop = true;
//     }
//
// }



//differation between functions/for/.. support
void End(ofstream& file, const long long& m)
{
    if (tokens[m] == "end")
    {

        file<< "}" << "\n";
        loop = false;
        indent_level--;
    }
}
//library support
void import(ofstream& file, const long long& m)
{
    if (tokens[m] == "import")
    {
        if(tokens[m + 1]=="in_out") file << "#include <" << "iostream" << ">" << endl;
        if (tokens[m + 1] == "array")
        {
            file << "#include <" << "array" << ">" << endl;
            file << "#include <" << "vector" << ">" << endl;
            file << "#include <" << "unordered_map" << ">" << endl;
            file << "#include <" << "map" << ">" << endl;
        }
        if (tokens[m + 1] == "file")
        {
            file << "#include <" << "fstream" << ">" << endl;
        }
        if (tokens[m + 1] == "math")
        {
            file << "#include <" << "cmath" << ">" << endl;
		}
        if (tokens[m + 1] == "string")
        {
            file << "#include <" << "string" << ">" << endl;
        }
    }
}
//function main support
void func_main(ofstream& file, const long long& m) {
    if (tokens[m] == "main" and tokens[m - 1] == "fn") {
        file << "int main() {" << endl;
        indent_level++;
    }
}

//Initialization support
string intVariable(const long long& m, ofstream& file) {

        if (tokens[m + 1].substr(0, 1) == "[" and tokens[m + 3] != "") {
            file << tokens[m] << " " << tokens[m + 1];
            file << " = " << tokens[m + 3] << ";" << endl;
        }
        if (tokens[m] == "int") {
            file << "int ";
            if (tokens[m + 2] == "=") {
                file << tokens[m + 1] << ";" << endl;
            }

            if (tokens[m + 2].substr(0, 1) == "[") {
                file << tokens[m + 1];
                file << tokens[m + 2] << ";" << endl;
            }
            type_var = "int";

        }
        else if (tokens[m] == "float") {
            file << "float ";
            if (tokens[m + 2] == "=") {
                file << tokens[m + 1] << ";" << endl;

            }
            type_var = "float";
        }
        if (tokens[m] == "bool") {
            file << "bool ";
            if (tokens[m + 2] == "=") {
                file << tokens[m + 1] << ";" << endl;
            }
            type_var = "bool";
        }

        else {
            if (tokens[m] == "=" and tokens[m - 1].substr(0, 1) != "[") {
                cerr << tokens[m + 2];
                if (tokens[m + 2] == "" and tokens[m + 1] != "") {
                    if (type_var == "int") file << tokens[m - 1] << " = " << stoi(tokens[m + 1]) << ";" << endl;//stoi = string to integer
                    if (type_var == "float") file << tokens[m - 1] << " = " << stof(tokens[m + 1]) << ";" << endl;// string to float
                    if (type_var == "bool") file << tokens[m-1] << " = " << stob(tokens[m + 1]) << ";" << endl;// string to bool
                }
                else {
                    file << tokens[m - 1] << " = ";
                }
            }

        }

    return varName;
}

void NewLine(ofstream& file, const long long& m) {
    if (tokens[m] == "new_line") {
        file << "cout << endl;" << "\n";
    }
}
void decode() {
    ifstream file("file.c--");
    ofstream file1("file1.cpp");
    file1 << "using namespace std;" << endl;
    string str;
    int j = 0;
    int i = 0;
    int pos = 0;


    while (getline(file, str)) {
        while (str[i] != ';' and i < str.size() ) {
            //tokenizations
            if (str[i] == ' ') {
                tokens[j] = str.substr(pos, i - pos);
                cout<<tokens[j]<<" "<<j<<endl;
                j++;
                pos = i + 1;
            }
            if (str[i + 1] == ';' or i + 1 >= str.size()) {
                tokens[j] = str.substr(pos, i - pos + 1);
                cout<<tokens[j]<<" "<<j<<endl;
                j++;
                pos = i + 1;
            }
            if (str[i]=='=' and str[i - 1] != ' ' and str[i + 1] != ' ') {
                tokens[j] = str.substr(pos, i - pos);
                tokens[j+1] = str[i];
                cout << tokens[j]<<" "<<j<<endl;
                cout<<tokens[j+1]<<" "<<j<<endl;
                j=j+2;
                pos = i + 1;
            }
            i++;
        }
        Tabs(file1);
        for (long long m = 0; m < j; m++) {
            import(file1,m);
            func_main(file1,m);
            intVariable(m, file1);
            While(file1, m);
            //For(file1, m);
            print(m, file1);
            input(m, file1);
            Add(file1, m);
            Minus(file1, m);
            Multiplication(file1, m);
            Division(file1, m);
            NewLine(file1, m);
            to_cpp(m, file1, file);
            End(file1, m);

        }
        i = 0;
        pos = 0;
        str.clear();
        j = 0;

    }
    file.close();
    file1.close();
}

int main() {
    decode();
}
