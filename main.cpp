#include <iostream>
#include <string>
#include "utility functions.h"
#include <fstream>
using namespace std;


int form = 0;
string word;
int cnt = 0;

string varName;
string type_var[100];
string name_var[100];
bool loop = false;
int indent_level = 0;

string type_function = "";
string name_function = "";
bool function[100];
int function_count = 0;

string function_line;
string function_part;
bool func_exist = false ;

std::string str;
int j = 0;
int i = 0;
int pos = 0;
std::string tokens[100];

std::istringstream iss(function_part);
int pos_var = 0;

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
        if (stoi(tokens[m + 1]) == 0) {
            cerr << "Error code 0: Division by 0!" << endl;
        }
    }
}
//loop support
void While(ofstream& file, const long long& m)
{
    if (tokens[m] == "while") {
        if (tokens[m + 1] != "" and tokens[m + 2]!="")
        {
            file << "while" << "(" << tokens[m + 1] << tokens[m + 2] << tokens[m + 3] << "){" << "\n";
        }
        if (tokens[m + 1] != "" and tokens[m + 2]=="")
        {
            file << "while" << "(" << tokens[m + 1] << tokens[m + 2] << "){" << "\n";
        }
        if (tokens[m + 1] == "" and tokens[m + 2]=="")
        {
            file << "while" << "(" << tokens[m + 1]  << "){" << "\n";
        }

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
void End(ofstream &file, const long long &m, std::istringstream &line)
{
    if (tokens[m] == "end")
    {

        file<< "}" << "\n";
        loop = false;
        indent_level--;
    }
    if (func_exist)
    {
        func_exist=false;
        cerr << "Error code 2: ";
    }
}
//library support
bool imported[9];
void import(ofstream& file, const long long& m)
{
    if (tokens[m] == "import")
    {
        if(tokens[m + 1]=="in_out" and imported[0] == false) {
            file << "#include <" << "iostream" << ">" << endl;
            imported[0] = true;
        }
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
//another function support


void func(ofstream& file,long long& m) {
    if ((tokens[m] != "main" and tokens[m]!= "") and tokens[m - 1] == "fn" and function[function_count] == 0 and function[function_count+1] == false) {
        file << "auto " << tokens[m] <<"() {" << endl;
        name_function=tokens[m];
    }
    if (name_function == "main") {
        cerr << "Error code 2: Name function become main when it shouldn't";
    }
}
void func_calling(ofstream& file,long long& m) {
    if ((tokens[m] != "main" and tokens[m]!= "") and tokens[m - 1] == "fn!" and function[function_count] == 0 and function[function_count+1] == false) {
        file << tokens[m] <<"();" << endl;
    }
    if (name_function == "main") {
        cerr << "Error code 2: Name function become main when it shouldn't";
    }
}
//func_support

int type_checking;
void func_return(ofstream& file, const long long& m,ifstream& file1) {
    if (tokens[m] == "return") {
        type_function = tokens[m+2];
    }
    if (tokens[m] == "return" and name_function != "main") {
        file << "return "<<tokens[m+1].substr(0,tokens[m+1].size()-1) << ";" << endl;
        type_checking=check_loop(name_var,tokens[m+1].substr(0,tokens[m+1].size()-1));
        if (type_checking==-1) {
            cerr << "Error code 3: return error!";

        }
        if (tokens[m + 2] != type_var[type_checking]) {
            if (tokens[m+2]=="Int" and type_var[type_checking]=="int"){}
            else if (tokens[m+2]=="Float" and type_var[type_checking]=="float"){}
            else if (tokens[m+2]=="Bool" and type_var[type_checking]=="bool"){}
            else if (tokens[m+2]=="String" and type_var[type_checking]=="string"){}
            else {
                cerr << "Warning return type isn't the same as value";
            }

        }
        // bool _var_type = check_loop(&type_var[type_checking],tokens[m+2]);
        // if (_var_type==0) {
        //     cerr << "Warning return type isn't the same as value";
        // }
    }
}

//Initialization support
string intVariable(const long long& m, ofstream& file) {

        if (tokens[m + 1] != "") {
            // if (tokens[m + 1].substr(0, 1) == "[" and tokens[m + 3] != "") {
            //     file << tokens[m] << " " << tokens[m + 1];
            //     file << " = " << tokens[m + 3] << ";" << endl;
            // }
            if (tokens[m] == "int") {
                file << "int ";
                if (tokens[m + 2] == "=") {
                    file << tokens[m + 1] << ";" << endl;
                    name_var[pos_var] = tokens[m + 1];
                }
                type_var[pos_var] = "int";
            }
            else if (tokens[m] == "float") {
                file << "float ";
                if (tokens[m + 2] == "=") {
                    file << tokens[m + 1] << ";" << endl;
                    name_var[pos_var] = tokens[m + 1];
                }
                type_var[pos_var] = "float";
            }
            else if (tokens[m] == "bool") {
                file << "bool ";
                if (tokens[m + 2] == "=") {
                    file << tokens[m + 1] << ";" << endl;
                    name_var[pos_var] = tokens[m + 1];
                }
                type_var[pos_var] = "bool";
            }
            else if (tokens[m] == "fn") {

            }

            else {
                if (tokens[m] == "=") {
                    if (tokens[m + 2] == "" and tokens[m + 1] != "") {
                        Tabs(file);

                        if (type_var[pos_var] == "int") file << tokens[m - 1] << " = " << stoi(tokens[m + 1]) << ";" << endl;//stoi = string to integer
                        if (type_var[pos_var] == "float") file << tokens[m - 1] << " = " << stof(tokens[m + 1]) << ";" << endl;// string to float
                        if (type_var[pos_var] == "bool") file << tokens[m-1] << " = " << stob(tokens[m + 1]) << ";" << endl;// string to bool
                        pos_var++;
                    }
                    else {
                        file << tokens[m - 1] << " = ";
                    }
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
                func(file1, m);
                func_return(file1, m,file);
                func_calling(file1, m);


            End(file1, m,iss);
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
