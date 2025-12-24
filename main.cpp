    #include <iostream>
    #include <string>
    #include "utility functions.h"
    #include <fstream>
    #include <vector>
    using namespace std;

    variable arr[100];
    int line = 1;
    int form = 0;
    string word;
    int cnt = 0;

    string varName;

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
    token Token[100];

    bool ope_ = false;
    std::istringstream iss(function_part);
    int pos_var = 0;
    vector <string> line_str(100);
    void Tabs(ofstream& file) {
        for (int i = 0; i < indent_level; i++)
        {
            file << "\t";
        }
    }

    //C++ integration
    void to_cpp(const long long m, ofstream& file, ifstream& file1)
    {
        if (Token[m].value == "cpp_main")
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
        if (Token[m].value == "cpp_ext")
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
        if (Token[m].value == "print")
        {
            file << "cout << " << Token[m+1].value ;
            ope_ = true;
        }
        if (Token[m].value == "print!")
        {
            file << "cout <<\"" << Token[m+1].value  << "\"";
            ope_ = true;
        }
    }
    //Input support
    void input( const long long& m, ofstream& file)
    {
        if (Token[m].value == "input")
        {
            Token[m].type="input";
            file << "cin >> " << Token[m+1].value << ";" << endl;
            ope_=true;
        }
    }
    //Basic arithmetic support
    void Add(ofstream& file, const long long& m)
    {
        if (Token[m].value == "+")
        {
            if (Token[m+1].value  == "" or Token[m - 1].value=="") cerr << "Warning one or two variables are non_existent on line: "<<line<<"\n";
            file << Token[m - 1].value << " + " << Token[m + 1].value;
            ope_ = true;

        }
    }
    void Minus(ofstream& file, const long long& m)
    {
        if (Token[m].value == "-")
        {
            file << Token[m - 1].value << " - " << Token[m + 1].value;
            ope_ = true;
        }
    }
    void Multiplication(ofstream& file, const long long& m)
    {
        if (Token[m].value == "*")
        {
            file << Token[m - 1].value << " * " << Token[m + 1].value;
            ope_ = true;

        }
    }
    void Division(ofstream& file, const long long& m)
    {
        if (Token[m].value == "/")
        {
            if (Token[1].type == "int") {
                if (stoi(Token[m + 1].value) != 0) {
                    file << Token[m - 1].value << " / " << Token[m + 1].value;
                    ope_ = true;
                }
                if (stoi(Token[m + 1].value) == 0) {
                    cerr << "Error code 0: Division by 0: " << line<<endl;
                }
            }
            if (Token[1].type == "float") {
                if (stof(Token[m + 1].value) != 0) {
                    file << Token[m - 1].value << " / " << Token[m + 1].value;
                    ope_ = true;

                }
                if (stof(Token[m + 1].value) == 0) {
                    cerr << "Error code 0: Division by 0: " << line<<endl;
                }
            }
        }
    }
    //loop support
    void While(ofstream& file, const long long& m)
    {
        if (Token[m].value == "while") {
            if (Token[m + 1].value != "" and Token[m + 2].value !="")
            {
                file << "while" << "(" << Token[m + 1].value << Token[m + 2].value << Token[m + 3].value << "){" << "\n";
            }
            if (Token[m + 1].value != "" and Token[m + 2].value=="")
            {
                file << "while" << "(" << Token[m + 1].value << Token[m + 2].value << "){" << "\n";
            }
            if (Token[m + 1].value == "" and Token[m + 2].value=="")
            {
                file << "while" << "(" << Token[m + 1].value  << "){" << "\n";
            }

            loop = true;
            indent_level++;
        }
    }
    //type
    string fn_for_var;
    string var_name_calling;
    void var_fn(ofstream &file,const long long& m){
        cerr<<Token[m].value<<" ";
        // if (tokens[m]==typ[pos-1]) {
        //     file << fn_for_var[pos-1]<<"()"<<";\n";
        // }
        if (Token[m].value=="void" and Token[m+2].value=="->") {
            arr[pos_var].type=Token[m].value+"_func";
            Token[m+1].type="void_fn";
            var_name_calling=Token[m+1].value;
            arr[pos_var].name=Token[m+1].value;
            file << arr[pos_var].type.substr(0,arr[pos_var].type.size()-5) << " (*" << arr[pos_var].name << ")(" << "" <<");" << "\n";
            fn_for_var=Token[m+3].value;
            //cerr<<fn_for_var[pos];
            file << arr[pos_var].name << " = " << Token[m+3].value << ";" << "\n";
            pos_var++;
        }
        if (Token[m].value==var_name_calling+".call;") {
            file << var_name_calling+"()";
        };
        //cerr<<tokens[m]<<" ";

    }
    //differation between functions/for/.. support
    void End(ofstream &file, const long long &m, std::istringstream &line)
    {

        if (Token[m].value == "end")
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
        if (Token[m].value == "import")
        {

            if(Token[m + 1].value=="in_out" and imported[0] == false) {
                file << "#include <" << "iostream" << ">" << endl;
                imported[0] = true;
            }
            if (Token[m + 1].value == "array")
            {
                file << "#include <" << "array" << ">" << endl;
                file << "#include <" << "vector" << ">" << endl;
                file << "#include <" << "unordered_map" << ">" << endl;
                file << "#include <" << "map" << ">" << endl;
            }
            if (Token[m + 1].value == "file")
            {
                file << "#include <" << "fstream" << ">" << endl;
            }
            if (Token[m + 1].value == "math")
            {
                file << "#include <" << "cmath" << ">" << endl;
		    }
            if (Token[m + 1].value == "string")
            {
                file << "#include <" << "string" << ">" << endl;
            }
        }
    }



    //function main support
    void func_main(ofstream& file, const long long& m) {
        if (Token[m].value == "main" and Token[m - 1].value == "fn") {
            Token[m].type="main";
            file << "int main() {" << endl;
            indent_level++;
        }
    }
    //another function support


    void func(ofstream& file,long long& m) {
        if ((Token[m].value != "main" and Token[m].value != "") and Token[m - 1].value == "fn" and function[function_count] == 0 and function[function_count+1] == false) {
            file << "auto " << Token[m].value <<"() {" << endl;
            name_function=Token[m].value;
        }
        if (name_function == "main") {
            cerr << "Error code 2: Name function become main when it shouldn't";
        }
    }
    void func_calling(ofstream& file,long long& m) {
        if ((Token[m].value != "main" and Token[m].value!= "") and Token[m - 1].value == "fn!" and function[function_count] == 0 and function[function_count+1] == false) {
            file << Token[m].value <<"();" << endl;
        }
        if (name_function == "main") {
            cerr << "Error code 2: Name function become main when it shouldn't on line: " << line <<"\n";
        }
    }
    //func_support

    int type_checking;
    void func_return(ofstream& file, const long long& m,ifstream& file1) {
        if (Token[m].value == "return") {
            type_function = Token[m+2].value;
        }
        if (Token[m].value == "return" and name_function != "main") {
            file << "return "<<Token[m+1].value.substr(0,Token[m+1].value.size()-1) << ";" << endl;
            type_checking=check_loop(arr,Token[m+1].value.substr(0,Token[m+1].value.size()-1));
            if (Token[m + 2].value != arr[type_checking].type) {
                if (Token[m+2].value =="Int" and arr[type_checking].type=="int"){}
                else if (Token[m+2].value=="Float" and arr[type_checking].type=="float"){}
                else if (Token[m+2].value=="Bool" and arr[type_checking].type=="bool"){}
                else if (Token[m+2].value=="String" and arr[type_checking].type=="string"){}
                else {
                    if (type_checking >= 0) {
                        cout<< arr[type_checking].type<<"\n"<<"aia e";
                        cerr << "Warning return type may be not the same as value on line:" <<line <<"\n";
                    }
                    else {
                        cerr << "Warning return out of bonds on line: " <<line<<"\n";
                    }
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

            if (Token[m + 1].value != "") {
                if (Token[m].value == "int") {
                    file << "int ";
                    if (Token[m + 2].value == "=") {
                        file << Token[m + 1].value << ";" << endl;
                        arr[pos_var].name = Token[m + 1].value;
                        Token[m+1].type = "int";
                        arr[pos_var].type = "int";
                    }
                    pos_var++;
                    Token[m].type = "intializer";
                }
                else if (Token[m].value == "float") {
                    file << "float ";
                    if (Token[m + 2].value == "=") {
                        file << Token[m + 1].value << ";" << endl;
                        arr[pos_var].name = Token[m + 1].value;
                        Token[m+1].type = "float";
                        //cout<<Token[m+1].type<<"\n";
                        arr[pos_var].type = "float";
                    }
                    pos_var++;
                    Token[m].type = "intializer";
                }
                else if (Token[m].value == "bool") {
                    file << "bool ";
                    if (Token[m + 2].value == "=") {
                        file << Token[m + 1].value << ";" << endl;
                        arr[pos_var].name = Token[m + 1].value;
                        Token[m+1].type = "bool";
                        arr[pos_var].type = "bool";
                    }
                    pos_var++;
                    Token[m].type = "intializer";
                }
                else if (Token[m].value == "fn") {

                }

                else {
                    if (Token[m].value == "=") {
                        Token[m].type = "assign";
                        if (tokens[m + 2] == "" and tokens[m + 1] != "") {
                            Tabs(file);
                            if (arr[pos_var-1].type == "int") {
                                file << tokens[m - 1] << " = " << stoi(tokens[m + 1]);//stoi = string to integer
                                ope_ = true;
                                Token[m + 1].type = "value";
                                }
                            if (arr[pos_var-1].type == "float"){
                                file << tokens[m - 1] << " = " << stof(tokens[m + 1]);// string to float
                                ope_ = true;
                                Token[m + 1].type = "value";
                                }
                            if (arr[pos_var-1].type == "bool"){
                                file << tokens[m-1] << " = " << stob(tokens[m + 1]);// string to bool
                                ope_ = true;
                                Token[m + 1].type = "value";
                                }

                        }
                        else {
                            ope_ = true;
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
    ifstream file("file.c--");
    ofstream file1("file1.cpp");
    void decode() {
        file1 << "using namespace std;" << endl;
        while (getline(file, str)) {
            while (str[i] != ';' and i < str.size() ) {
                //tokenizations
                if (str[i] == ' ') {
                    Token[j].value = str.substr(pos, i - pos);
                    tokens[j] = str.substr(pos, i - pos);
                    //cout<<tokens[j]<<" "<<j<<endl;
                    j++;
                    pos = i + 1;


                }
                if (str[i + 1] == ';' or i + 1 >= str.size()) {
                    tokens[j] = str.substr(pos, i - pos + 1);
                    Token[j].value = str.substr(pos, i - pos + 1);
                    //cout<<tokens[j]<<" "<<j<<endl;
                    j++;
                    pos = i + 1;


                }
                if (str[i]=='=' and str[i - 1] != ' ' and str[i + 1] != ' ') {
                    tokens[j] = str.substr(pos, i - pos);
                    Token[j].value = str.substr(pos, i - pos);
                    tokens[j+1] = str[i];
                    Token[j+1].value = str[i];
                    //cout << tokens[j]<<" "<<j<<endl;
                    //cout<<tokens[j+1]<<" "<<j<<endl;
                    j=j+2;
                    pos = i + 1;

                }
                i++;
            }
            ope_ = false;
            Tabs(file1);
            for (long long m = 0; m < j; m++) {
                import(file1,m);
                func_main(file1,m);
                intVariable(m, file1);
                While(file1, m);
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
                if (line_str[line]!="") line_str[line]=line_str[line]+""+""+Token[m].value + " ";
                else line_str[line]=Token[m].value + " ";

                End(file1, m,iss);
                var_fn(file1, m);
            }
            line_str[line]+="\n";
            line++;
            if (ope_ == true) file1 <<";"<<endl;
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


        // cout<<arr[0].name<<endl;
        // cout<<arr[0].type<<endl;

        AST(line,line_str);
    }
